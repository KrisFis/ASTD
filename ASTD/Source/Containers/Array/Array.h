
#pragma once

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeTraits.h"

#include "Containers/Array/Allocator/ArrayAllocator.h"
#include "Containers/Array/Internals/ArrayTypeTraits.h"

#include "Containers/InitializerList/InitializerList.h"

template<typename InElementType, typename InAllocator = TArrayAllocator<InElementType>>
class TArray
{
public: // Types

	typedef InElementType ElementType;
	typedef NArrayTypeTraits::TElementInfo<ElementType> ElementInfo;

	typedef InAllocator AllocatorType;
	typedef NArrayTypeTraits::TAllocatorInfo<AllocatorType> AllocatorInfo;

	typedef ElementType* ArrayIteratorType;
	typedef const ElementType* ConstArrayIteratorType;

	typedef typename AllocatorType::SizeType SizeType;
	typedef TInitializerList<ElementType> ElementListType;

public: // Asserts

	static_assert(ElementInfo::IsValid, "Element type is not valid");
	static_assert(AllocatorInfo::IsValid, "Allocator type is not valid");

public: // Constructors

	FORCEINLINE TArray() : Allocator(), Count(0) {}
	FORCEINLINE TArray(const TArray& Other) : Allocator(), Count(0) { FillToEmptyImpl(Other); }
	FORCEINLINE TArray(TArray&& Other) : Allocator(), Count(0) { FillToEmptyImpl(Move(Other)); }
	FORCEINLINE TArray(const ElementListType& InList)
		: Allocator()
		, Count(0)
	{ 
		FillToEmptyImpl(InList.begin(), InList.size()); 
	}

	FORCEINLINE TArray(const ElementType* InData, SizeType InCount) 
		: Allocator()
		, Count(0)
	{
		FillToEmptyImpl(InData, InCount);
	}

public: // Destructor

	FORCEINLINE ~TArray() { EmptyImpl(); }

public: // Compare operators

	FORCEINLINE bool operator==(const TArray& Other) const { return CompareAllocatorsImpl(&Allocator, &Other.Allocator, Count); }
	FORCEINLINE bool operator!=(const TArray& Other) const { return CompareAllocatorsImpl(&Allocator, &Other.Allocator, Count); }

public: // Assign operators

	FORCEINLINE TArray& operator=(const TArray& Other) { EmptyImpl(); FillToEmptyImpl(Other); return *this; }
	FORCEINLINE TArray& operator=(TArray&& Other) { EmptyImpl(); FillToEmptyImpl(Move(Other)); return *this; }

	FORCEINLINE TArray& operator=(const ElementListType& InList) { EmptyImpl(); FillToEmptyImpl(InList.begin(), InList.size()); return *this; }

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
	FORCEINLINE bool IsValidIndex(SizeType Idx) const { return Idx < Count; }

	FORCEINLINE SizeType GetFirstIndex() const { return 0; }
	FORCEINLINE SizeType GetLastIndex() const { return Count > 0 ? Count - 1 : 0; }

public: // Append

	FORCEINLINE void Append(const TArray& Other) { AppendImpl(Other); }
	FORCEINLINE void Append(TArray&& Other) { AppendImpl(Move(Other)); }

	FORCEINLINE void Append(const ElementType& Value, SizeType NumToAdd) { AddImpl(Value, NumToAdd); }
	FORCEINLINE void Append(const ElementListType& InList) { AppendImpl(InList.begin(), InList.size()); }
	FORCEINLINE void AppendUnitialized(SizeType NumToAdd) { AddUnitializedImpl(NumToAdd); }

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
			if(CompareElementsImpl(GetElementAtImpl(i), &Value))
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

	FORCEINLINE void ShrinkToFit() { ShrinkImpl(Count); }
	FORCEINLINE void Shrink(SizeType Num) { ShrinkImpl(Num); }
	FORCEINLINE void Reserve(SizeType Num) { ReserveImpl(Num); }

	FORCEINLINE void Reset() { EmptyImpl(); }
	FORCEINLINE void Empty() { EmptyImpl(); }

public: // Iterators

	FORCEINLINE ArrayIteratorType begin() { return Count > 0 ? GetElementAtImpl(0) : nullptr; }
	FORCEINLINE ConstArrayIteratorType begin() const { return Count > 0 ? GetElementAtImpl(0) : nullptr; }

	FORCEINLINE ArrayIteratorType end() { return Count > 0 ? GetElementAtImpl(Count) : nullptr; }
	FORCEINLINE ConstArrayIteratorType end() const { return Count > 0 ? GetElementAtImpl(Count) : nullptr; }

private: // Helpers -> Getters

	FORCEINLINE ElementType* GetElementAtImpl(SizeType Idx) const { return Allocator.GetData() + Idx; }

private: // Helpers -> Manipulation

	void AddUnitializedImpl(SizeType InCount = 1)
	{
		if(InCount > 0)
		{
			Count += InCount;
			RelocateIfNeededImpl();
		}
	}

	void AddImpl(const ElementType& Value, SizeType InCount = 1)
	{
		if(InCount > 0)
		{
			const SizeType oldCount = Count;

			Count += InCount;
			RelocateIfNeededImpl();

			CopyElementImpl(Allocator.GetData() + oldCount, Value, InCount);
		}
	}

	void AddImpl(ElementType&& Value)
	{
		++Count;
		RelocateIfNeededImpl();

		MoveElementImpl(Allocator.GetData(), &Value, 1);
	}

	void RemoveSwapImpl(SizeType Index)
	{
		DestructImpl(GetElementAtImpl(Index), 1);

		if(Index != Count - 1)
		{
			// Swaps last element with this
			SMemory::Copy(
				GetElementAtImpl(Index),
				GetElementAtImpl(Count - 1),
				sizeof(ElementType)
			);
		}

		--Count;

		RelocateIfNeededImpl();
	}

	void RemoveImpl(SizeType Index)
	{
		DestructImpl(GetElementAtImpl(Index), 1);

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

		RelocateIfNeededImpl();
	}

	void ShrinkImpl(SizeType Num)
	{
		if(Num >= Allocator.GetCount()) return;
		else if(Num == 0)
		{
			EmptyImpl();
			return;
		}

		DestructImpl(GetElementAtImpl(Num), Count - Num);

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

	void ReserveImpl(SizeType Num)
	{
		if(Num <= Allocator.GetCount()) return;
		Allocator.Allocate(Num - Allocator.GetCount());
	}

	void EmptyImpl() 
	{
		if(Count > 0)
		{
			DestructImpl(Allocator.GetData(), Count);

			Allocator.Release();
			Count = 0; 
		}
	}

private: // Helpers -> Cross manipulation (Array)

	void AppendImpl(const ElementType* InData, SizeType InCount, bool preferMove = false)
	{
		if(InCount > 0)
		{
			const SizeType oldCount = Count;

			Count += InCount;
			RelocateIfNeededImpl();

			if(preferMove)
				MoveElementsImpl(Allocator.GetData() + oldCount, InData, InCount);
			else
				CopyElementsImpl(Allocator.GetData() + oldCount, InData, InCount);
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

	void FillToEmptyImpl(const ElementType* InData, SizeType InCount, bool preferMove = false)
	{
		if(InData)
		{
			Allocator.Allocate(InCount);

			if(preferMove)
				MoveElementsImpl(Allocator.GetData(), InData, InCount);
			else
				CopyElementsImpl(Allocator.GetData(), InData, InCount);

			Count = InCount;
		}
	}
	
	FORCEINLINE void FillToEmptyImpl(const TArray& Other) { FillToEmptyImpl(Other.GetData(), Other.Count); }

	void FillToEmptyImpl(TArray&& Other)
	{
		FillToEmptyImpl(Other.GetData(), Other.GetCount(), true);

		{
			Other.Allocator.SetData(nullptr);
			Other.Allocator.SetCount(0);
			Other.Count = 0;
		}
	}

private: // Helpers -> Others

	// Make sure elements array and values array does NOT OVERLAP
	void CopyElementsImpl(ElementType* ElementArray, const ElementType* ValuesArray, SizeType Count)
	{
		if constexpr (ElementInfo::SupportCopy && !(ElementInfo::IsTrivial))
		{
			for(SizeType i = 0; i < Count; ++i)
			{
				NMemoryUtilities::CallCopyConstructor(ElementArray + i, *(ValuesArray + i));
			}
		}
		else
		{
			SMemory::Copy(
				ElementArray,
				ValuesArray,
				sizeof(ElementType) * Count
			);
		}
	}

	// Construction from one value, that is reused
	void CopyElementImpl(ElementType* ElementArray, const ElementType& Value, SizeType Count)
	{
		if constexpr (ElementInfo::SupportCopy && !(ElementInfo::IsTrivial))
		{
			for(SizeType i = 0; i < Count; ++i)
			{
				NMemoryUtilities::CallCopyConstructor(ElementArray + i, Value);
			}
		}
		else
		{
			for(SizeType i = 0; i < Count; ++i)
			{
				SMemory::Copy(
					ElementArray + i,
					&Value,
					sizeof(ElementType)
				);
			}
		}
	}

	void MoveElementsImpl(ElementType* ElementArray, const ElementType* ValuesArray, SizeType Count)
	{
		if constexpr (ElementInfo::SupportMove && !(ElementInfo::IsTrivial))
		{
			for(SizeType i = 0; i < Count; ++i)
			{
				NMemoryUtilities::CallMoveConstructor(ElementArray + i, Move(*(const_cast<ElementType*>(ValuesArray + i))));
			}
		}
		else
		{
			SMemory::Copy(
				ElementArray,
				ValuesArray,
				sizeof(ElementType) * Count
			);
		}
	}

	// Construction from one value, that is reused
	void MoveElementImpl(ElementType* ElementArray, ElementType&& Value, SizeType Count)
	{
		if constexpr (ElementInfo::SupportMove && !(ElementInfo::IsTrivial))
		{
			// Having count greater than 1 can be unpredictable in specific case where object modifies value in move constructor
			for(SizeType i = 0; i < Count; ++i)
			{
				NMemoryUtilities::CallMoveConstructor(ElementArray + i, Move(Value));
			}
		}
		else
		{
			for(SizeType i = 0; i < Count; ++i)
			{
				SMemory::Copy(
					ElementArray + i,
					&Value,
					sizeof(ElementType)
				);
			}
		}
	}

	void DestructImpl(ElementType* ElementArray, SizeType Count)
	{
		if constexpr (!ElementInfo::IsTrivial)
		{
			for(SizeType i = 0; i < Count; ++i)
			{
				NMemoryUtilities::CallDestructor(ElementArray + i);
			}
		}
	}

	void RelocateIfNeededImpl()
	{
		const SizeType reserved = Allocator.GetCount();
		const SizeType nextReserved = reserved == 0 ? 2 : 2 * reserved;
		const SizeType prevReserved = reserved == 0 ? 0 : reserved / 2;

		if(Count > reserved)
		{
			ReserveImpl(nextReserved);
		}
		else if(Count <= prevReserved)
		{
			ShrinkImpl(prevReserved);
		}
	}

	FORCEINLINE bool CompareAllocatorsImpl(const AllocatorType* Lhs, const AllocatorType* Rhs, SizeType InCount) const
	{
		return (Lhs->GetCount() >= InCount && Rhs->GetCount() >= InCount) ? 
			SMemory::Compare(Lhs->GetData(), Rhs->GetData(), sizeof(ElementType) * InCount) == 0 : false;
	}

	FORCEINLINE bool CompareElementsImpl(const ElementType* Lhs, const ElementType* Rhs) const
	{
		// Compare bytes instead of using == operator (that might not be provided)
		return SMemory::Compare(Lhs, Rhs, sizeof(ElementType)) == 0;
	}

private: // Fields

	AllocatorType Allocator;
	SizeType Count;
};