
#pragma once

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeTraits.h"
#include "Math/MathUtilities.h"

#include "Containers/Array/Allocator/ArrayAllocator.h"

#include "Containers/InitializerList/InitializerList.h"

template<typename InElementType, typename InAllocator = TArrayAllocator<InElementType>>
class TArray
{
public: // Types

	typedef InElementType ElementType;
	typedef InAllocator AllocatorType;

	typedef ElementType* ArrayIteratorType;
	typedef const ElementType* ConstArrayIteratorType;

	typedef typename AllocatorType::SizeType SizeType;
	typedef TInitializerList<ElementType> ElementListType;

public: // Asserts

	static_assert(!TIsSame<ElementType, void>::Value && !TIsReference<ElementType>::Value, "Element type is not valid");
	static_assert(!TIsSame<AllocatorType, void>::Value && TIsSignedType<SizeType>::Value, "Allocator type is not valid");

public: // Constructors

	FORCEINLINE TArray() : Allocator(), Count(0) {}
	FORCEINLINE TArray(const TArray& Other) : Allocator(), Count(0) { AppendImpl(Other); }
	FORCEINLINE TArray(TArray&& Other) : Allocator(), Count(0) { ReplaceImpl(Move(Other)); }
	FORCEINLINE TArray(SizeType InCount, bool ReserveOnly = false) : Allocator(), Count(0) { ResizeImpl(InCount, ReserveOnly); }
	FORCEINLINE TArray(const ElementListType& InList)
		: Allocator()
		, Count(0)
	{ 
		AppendImpl(InList.begin(), InList.size()); 
	}

	FORCEINLINE TArray(const ElementType* InData, SizeType InCount) 
		: Allocator()
		, Count(0)
	{
		AppendImpl(InData, InCount);
	}

public: // Destructor

	FORCEINLINE ~TArray() { EmptyImpl(true); }

public: // Compare operators

	FORCEINLINE bool operator==(const TArray& Other) const { return CompareAllocatorsPrivate(&Allocator, &Other.Allocator, Count); }
	FORCEINLINE bool operator!=(const TArray& Other) const { return CompareAllocatorsPrivate(&Allocator, &Other.Allocator, Count); }

public: // Assign operators

	FORCEINLINE TArray& operator=(const TArray& Other) { EmptyImpl(true); AppendImpl(Other); return *this; }
	FORCEINLINE TArray& operator=(TArray&& Other) { ReplaceImpl(Move(Other)); return *this; }

	FORCEINLINE TArray& operator=(const ElementListType& InList) { EmptyImpl(true); AppendImpl(InList.begin(), InList.size()); return *this; }

public: // Get operators

	FORCEINLINE ElementType& operator[](SizeType Index) { return *GetElementAtImpl(Index); }
	FORCEINLINE const ElementType& operator[](SizeType Index) const { return *GetElementAtImpl(Index); }

public: // Property getters

	FORCEINLINE const ElementType* GetData() const { return Allocator.GetData(); }
	FORCEINLINE ElementType* GetData() { return Allocator.GetData(); }

	FORCEINLINE SizeType GetCount() const { return Count; }
	FORCEINLINE SizeType GetReserved() const { return Allocator.GetCount(); }

public: // Validations

	FORCEINLINE bool IsEmpty() const { return Count == 0; }
	FORCEINLINE bool IsValidIndex(SizeType Idx) const { return Idx >= 0 && Idx < Count; }

	FORCEINLINE SizeType GetFirstIndex() const { return 0; }
	FORCEINLINE SizeType GetLastIndex() const { return Count > 0 ? Count - 1 : 0; }

public: // Append

	FORCEINLINE void Append(const TArray& Other) { AppendImpl(Other); }
	FORCEINLINE void Append(TArray&& Other) { AppendImpl(Move(Other)); }

	FORCEINLINE void Append(const ElementType& Value, SizeType NumToAdd) { AddImpl(Value, NumToAdd); }
	FORCEINLINE void Append(const ElementListType& InList) { AppendImpl(InList.begin(), InList.size()); }
	FORCEINLINE void Append(const ElementType* InData, SizeType InCount) { AppendImpl(InData, InCount); } 

	FORCEINLINE void AppendUnitialized(SizeType NumToAdd) { if(NumToAdd > 0) GrowImpl(Count + NumToAdd); }

public: // Replace

	FORCEINLINE void Replace(TArray& Other) { ReplaceImpl(Move(Other)); }
	FORCEINLINE void Replace(TArray&& Other) { ReplaceImpl(Move(Other)); }

public: // Add

	FORCEINLINE SizeType Add(const ElementType& Value)
	{
		AddImpl(Value);
		return Count - 1;
	}

	FORCEINLINE SizeType Add(ElementType&& Value)
	{
		AddImpl(Move(Value));
		return Count - 1;
	}

	FORCEINLINE SizeType AddUnitialized()
	{
		AddUnitializedImpl();
		return Count - 1;
	}

	FORCEINLINE ElementType& Add_GetRef(const ElementType& Value)
	{
		AddImpl(Value);
		return *GetElementAtImpl(Count - 1);
	}

	FORCEINLINE ElementType& Add_GetRef(ElementType&& Value)
	{
		AddImpl(Move(Value));
		return *GetElementAtImpl(Count - 1);
	}

	FORCEINLINE ElementType& AddUnitialized_GetRef()
	{
		AddUnitializedImpl();
		return *GetElementAtImpl(Count - 1);
	}

	FORCEINLINE void Push(const ElementType& Value) { AddImpl(Value); }
	FORCEINLINE void Push(ElementType&& Value) { AddImpl(Move(Value)); }

public: // Remove
		// * Swap is faster version of Remove
		// * but does not preserve order

	FORCEINLINE void Remove(const ElementType& Value)
	{
		SizeType foundIndex = FindIndex(Value);
		if(foundIndex != INDEX_NONE)
		{
			RemoveImpl(foundIndex);
		}
	}

	FORCEINLINE void RemoveSwap(const ElementType& Value)
	{
		SizeType foundIndex = FindIndex(Value);
		if(foundIndex != INDEX_NONE)
		{
			RemoveSwapImpl(foundIndex);
		}
	}

	FORCEINLINE void RemoveAt(SizeType Index)
	{
		if(!IsValidIndex(Index)) return;
		RemoveImpl(Index);
	}

	FORCEINLINE void RemoveAtSwap(SizeType Index)
	{
		if(!IsValidIndex(Index)) return;
		RemoveSwapImpl(Index);
	}

	FORCEINLINE ElementType RemoveAt_GetCopy(SizeType Index)
	{
		if(!IsValidIndex(Index)) return;

		// Tries to use move constructor
		ElementType copy(Move(*GetElementAtImpl(Index)));
		RemoveImpl(Index);

		return copy;
	}

	FORCEINLINE ElementType RemoveAtSwap_GetCopy(SizeType Index)
	{
		if(!IsValidIndex(Index)) return;

		// Tries to use move constructor
		ElementType copy(Move(*GetElementAtImpl(Index)));
		RemoveSwapImpl(Index);

		return copy;
	}

	FORCEINLINE void Pop() 
	{ 
		if(Count > 0)
		{
			RemoveImpl(Count - 1); 
		}
	}

public: // Swap

	FORCEINLINE void Swap(SizeType FirstIdx, SizeType SecondIdx) 
	{
		if(!IsValidIndex(FirstIdx) || !IsValidIndex(SecondIdx)) return;

		SwapImpl(FirstIdx, SecondIdx, 1);
	}

	FORCEINLINE void SwapRange(SizeType FirstIdx, SizeType SecondIdx, SizeType Num = 1) 
	{
		if(!IsValidIndex(FirstIdx) || !IsValidIndex(SecondIdx)) return;
		else if(FirstIdx + Num > Count || SecondIdx + Num > Count) return;
		
		SwapImpl(FirstIdx, SecondIdx, Num);
	}

public: // Get

	FORCEINLINE const ElementType* GetAt(SizeType Index) const { return IsValidIndex(Index) ? GetElementAtImpl(Index) : nullptr; }
	FORCEINLINE ElementType* GetAt(SizeType Index) { return IsValidIndex(Index) ? GetElementAtImpl(Index) : nullptr; }

	FORCEINLINE const ElementType* GetFirst() const { return Count > 0 ? GetElementAtImpl(0) : nullptr; }
	FORCEINLINE ElementType* GetFirst() { return Count > 0 ? GetElementAtImpl(0) : nullptr; }

	FORCEINLINE const ElementType* GetLast() const { return Count > 0 ? GetElementAtImpl(Count - 1) : nullptr; }
	FORCEINLINE ElementType* GetLast() { return Count > 0 ? GetElementAtImpl(Count - 1) : nullptr; }

public: // Find Index

	SizeType FindIndex(const ElementType& Value) const 
	{
		for(SizeType i = 0; i < Count; ++i)
		{
			if(CompareElementsPrivate(GetElementAtImpl(i), &Value))
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

	template<typename Functor>
	SizeType FindIndexByFunc(Functor&& Func) const
	{
		for(SizeType i = 0; i < Count; ++i)
		{
			if(Func((const ElementType&)*GetElementAtImpl(i)))
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

	template<typename KeyType>
	SizeType FindIndexByKey(KeyType Key) const
	{
		for(SizeType i = 0; i < Count; ++i)
		{
			if(*GetElementAtImpl(i) == Key)
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

public: // Find Element

	template<typename Functor>
	const ElementType* FindByFunc(Functor&& Func) const
	{
		const SizeType foundIdx = FindIndexByFunc(Move(Func));
		return foundIdx != INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename Functor>
	ElementType* FindByFunc(Functor&& Func)
	{
		const SizeType foundIdx = FindIndexByFunc(Move(Func));
		return foundIdx != INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename KeyType>
	const ElementType* FindByKey(KeyType Key) const
	{
		const SizeType foundIdx = FindByKey(Key);
		return foundIdx != INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename KeyType>
	ElementType* FindByKey(KeyType Key)
	{
		const SizeType foundIdx = FindByKey(Key);
		return foundIdx != INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

public: // Contains

	FORCEINLINE bool Contains(const ElementType& Value) const { return FindIndex(Value) != INDEX_NONE; }

	template<typename Functor>
	FORCEINLINE bool ContainsByFunc(Functor&& Func) const { return FindByFunc(Move(Func)) != INDEX_NONE; }

	template<typename KeyType>
	FORCEINLINE bool ContainsByKey(KeyType Key) const { return FindByKey(Key) != INDEX_NONE; }

public: // Other

	FORCEINLINE void ShrinkToFit() { if(Count < Allocator.GetCount()) ShrinkImpl(Count); }
	FORCEINLINE void Shrink(SizeType Num) { if(Num < Count) ShrinkImpl(Num); }
	FORCEINLINE void Grow(SizeType Num) { if(Num > Count) GrowImpl(Num); }

	FORCEINLINE void Resize(SizeType Num) { ResizeImpl(Num, false); }
	FORCEINLINE void Reserve(SizeType Num) { if(Num > Count) ReserveImpl(Num); }

	FORCEINLINE void Reset() { EmptyImpl(true); }
	FORCEINLINE void Empty(bool ReleaseResources = true) { EmptyImpl(ReleaseResources); }

public: // Iterators

	FORCEINLINE ArrayIteratorType begin() { return Count > 0 ? GetElementAtImpl(0) : nullptr; }
	FORCEINLINE ConstArrayIteratorType begin() const { return Count > 0 ? GetElementAtImpl(0) : nullptr; }

	FORCEINLINE ArrayIteratorType end() { return Count > 0 ? GetElementAtImpl(Count) : nullptr; }
	FORCEINLINE ConstArrayIteratorType end() const { return Count > 0 ? GetElementAtImpl(Count) : nullptr; }

private: // Helper methods

	FORCEINLINE ElementType* GetElementAtImpl(SizeType Idx) const { return Allocator.GetData() + Idx; }

	void AddImpl(const ElementType& Value, SizeType InCount = 1)
	{
		if(InCount > 0)
		{
			const SizeType oldCount = Count;

			Count += InCount;
			RealocateIfNeededImpl();

			CopyElementPrivate(Allocator.GetData() + oldCount, Value, InCount);
		}
	}

	void AddImpl(ElementType&& Value)
	{
		++Count;
		RealocateIfNeededImpl();

		MoveElementPrivate(Allocator.GetData() + Count - 1, Move(Value), 1);
	}

	void AddUnitializedImpl(SizeType InCount = 1)
	{
		if(InCount > 0)
		{
			Count += InCount;
			RealocateIfNeededImpl();
		}
	}

	void RemoveSwapImpl(SizeType Index)
	{
		DestructPrivate(GetElementAtImpl(Index), 1);

		if(Index != Count - 1)
		{
			// Swaps last element with this
			SMemory::Move(
				GetElementAtImpl(Index),
				GetElementAtImpl(Count - 1),
				sizeof(ElementType)
			);
		}

		--Count;

		RealocateIfNeededImpl();
	}

	void RemoveImpl(SizeType Index)
	{
		DestructPrivate(GetElementAtImpl(Index), 1);

		if(Index != Count - 1)
		{
			// Moves entire allocation by one index down

			// NOTE(jan.kristian.fisera): 
			// * Is it worth to cache start index and try to move from start in case that would be less iterations ?
			SMemory::Move(
				GetElementAtImpl(Index),
				GetElementAtImpl(Index + 1),
				sizeof(ElementType) * (Count - Index - 1)
			);
		}

		--Count;

		RealocateIfNeededImpl();
	}

	void SwapImpl(SizeType FirstIdx, SizeType SecondIdx, SizeType Num)
	{
		// Copy to temporary storage at heep
		AllocatorType tmp;
		tmp.Allocate(Num);
		SMemory::Copy(
			tmp.GetData(),
			GetElementAtImpl(FirstIdx),
			sizeof(ElementType) * Num
		);

		// Do swap to first index
		// * elements from second idx to first
		SMemory::Copy(
			GetElementAtImpl(FirstIdx),
			GetElementAtImpl(SecondIdx),
			sizeof(ElementType) * Num
		);

		// Do swap to second index
		// * copied elements from first idx to second
		SMemory::Copy(
			GetElementAtImpl(SecondIdx),
			tmp.GetData(),
			sizeof(ElementType) * Num
		);
	}

	void ShrinkImpl(SizeType Num)
	{
		if(Num == 0)
		{
			EmptyImpl(true);
		}
		else
		{
			DestructPrivate(GetElementAtImpl(Num), Count - Num);

			// Copy to temporary allocator
			AllocatorType tmp;
			tmp.Allocate(Num);
			SMemory::Copy(
				tmp.GetData(),
				Allocator.GetData(),
				sizeof(ElementType) * Num
			);

			// Move data back to main allocator
			Allocator.Release();
			Allocator.Allocate(Num);
			SMemory::Copy(
				Allocator.GetData(),
				tmp.GetData(),
				sizeof(ElementType) * Num
			);

			tmp.SetData(nullptr);
			tmp.SetCount(0);
		}
	}

	void GrowImpl(SizeType Num)
	{
		Count = Num;
		ReserveImpl(Num);
	}

	void ReserveImpl(SizeType Num)
	{
		Allocator.Allocate(Num - Allocator.GetCount());
	}

	void EmptyImpl(bool Release)
	{
		if(Count > 0)
		{
			DestructPrivate(Allocator.GetData(), Count);

			if(Release)
			{
				Allocator.Release();
			}

			Count = 0; 
		}
	}

	void AppendImpl(const ElementType* InData, SizeType InCount, bool preferMove = false)
	{
		if(InCount > 0)
		{
			const SizeType oldCount = Count;

			Count += InCount;
			RealocateIfNeededImpl();

			if(preferMove)
				MoveElementsPrivate(Allocator.GetData() + oldCount, InData, InCount);
			else
				CopyElementsPrivate(Allocator.GetData() + oldCount, InData, InCount);
		}
	}

	void AppendImpl(TArray&& Other)
	{
		AppendImpl(Other.GetData(), Other.Count, true);

		{
			Other.Allocator.Release();
			Other.Count = 0;
		}
	}

	FORCEINLINE void AppendImpl(const TArray& Other) { AppendImpl(Other.GetData(), Other.Count); }

	void ReplaceImpl(TArray&& Other)
	{
		EmptyImpl(true);

		Allocator.SetData(Other.Allocator.GetData());
		Allocator.SetCount(Other.Allocator.GetCount());
		Count = Other.Count;

		Other.Allocator.SetData(nullptr);
		Other.Allocator.SetCount(0);
		Other.Count = 0;
	}

	void ResizeImpl(SizeType Num, bool ReserveOnly)
	{
		if(Num > Count)
		{
			if(ReserveOnly)
				ReserveImpl(Num);
			else
				GrowImpl(Num);
		}
		else if(Num < Count)
		{
			ShrinkImpl(Num);
		}
	}

	void RealocateIfNeededImpl()
	{
		const SizeType reserved = Allocator.GetCount();

		if(Count > reserved)
		{
			ReserveImpl(SMath::CeilToPowerOfTwo<uint64>(Count));
		}
	}

	// Make sure elements array and values array does NOT OVERLAP
	static void CopyElementsPrivate(ElementType* ElementArray, const ElementType* ValuesArray, SizeType InCount)
	{
		if constexpr (!TIsTriviallyCopyConstructible<ElementType>::Value)
		{
			for(SizeType i = 0; i < InCount; ++i)
			{
				SMemory::CallCopyConstructor(ElementArray + i, *(ValuesArray + i));
			}
		}
		else
		{
			SMemory::Copy(
				ElementArray,
				ValuesArray,
				sizeof(ElementType) * InCount
			);
		}
	}

	// Construction from one value, that is reused
	static void CopyElementPrivate(ElementType* ElementArray, const ElementType& Value, SizeType InCount)
	{
		if constexpr (!TIsTriviallyCopyConstructible<ElementType>::Value)
		{
			for(SizeType i = 0; i < InCount; ++i)
			{
				SMemory::CallCopyConstructor(ElementArray + i, Value);
			}
		}
		else
		{
			for(SizeType i = 0; i < InCount; ++i)
			{
				SMemory::Copy(
					ElementArray + i,
					&Value,
					sizeof(ElementType)
				);
			}
		}
	}

	static void MoveElementsPrivate(ElementType* ElementArray, const ElementType* ValuesArray, SizeType InCount)
	{
		if constexpr (!TIsTriviallyMoveConstructible<ElementType>::Value)
		{
			for(SizeType i = 0; i < InCount; ++i)
			{
				SMemory::CallMoveConstructor(ElementArray + i, Move(*(const_cast<ElementType*>(ValuesArray + i))));
			}
		}
		else
		{
			SMemory::Copy(
				ElementArray,
				ValuesArray,
				sizeof(ElementType) * InCount
			);
		}
	}

	// Construction from one value, that is reused
	static void MoveElementPrivate(ElementType* ElementArray, ElementType&& Value, SizeType InCount)
	{
		if constexpr (!TIsTriviallyMoveConstructible<ElementType>::Value)
		{
			// Having count greater than 1 can be unpredictable in specific case where object modifies value in move constructor
			for(SizeType i = 0; i < InCount; ++i)
			{
				SMemory::CallMoveConstructor(ElementArray + i, Move(Value));
			}
		}
		else
		{
			for(SizeType i = 0; i < InCount; ++i)
			{
				SMemory::Copy(
					ElementArray + i,
					&Value,
					sizeof(ElementType)
				);
			}
		}
	}

	static void DestructPrivate(ElementType* ElementArray, SizeType InCount)
	{
		if constexpr (!TIsTriviallyDestructible<ElementType>::Value)
		{
			for(SizeType i = 0; i < InCount; ++i)
			{
				SMemory::CallDestructor(ElementArray + i);
			}
		}
	}

	FORCEINLINE static bool CompareAllocatorsPrivate(const AllocatorType* Lhs, const AllocatorType* Rhs, SizeType InCount)
	{
		return (Lhs->GetCount() >= InCount && Rhs->GetCount() >= InCount) ? 
			SMemory::Compare(Lhs->GetData(), Rhs->GetData(), sizeof(ElementType) * InCount) == 0 : false;
	}

	FORCEINLINE static bool CompareElementsPrivate(const ElementType* Lhs, const ElementType* Rhs)
	{
		// Compare bytes instead of using == operator (that might not be provided)
		return SMemory::Compare(Lhs, Rhs, sizeof(ElementType)) == 0;
	}

private: // Fields

	AllocatorType Allocator;
	SizeType Count;
};