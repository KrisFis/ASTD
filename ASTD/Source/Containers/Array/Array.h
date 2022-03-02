
#pragma once

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeTraits.h"

#include "Containers/Array/Allocator/ArrayAllocator.h"
#include "Containers/Array/ArrayIterator.h"

template<typename InElementType, typename InAllocator = TArrayAllocator<InElementType>>
class TArray
{
private: // Setup

	typedef InElementType ElementType;
	typedef InAllocator AllocatorType;
	typedef TArrayIterator<ElementType> ArrayIteratorType;
	typedef TArrayIterator<const ElementType> ConstArrayIteratorType;
	typedef typename AllocatorType::SizeType SizeType;

public: // Asserts

	static_assert(!TIsSame<InAllocator, void>::Value, "Invalid allocator type");
	static_assert(TIsSignedType<SizeType>::Value, "Unsigned types are not supported");

public: // Constructors

	FORCEINLINE TArray() : Allocator(), Count(0) {}
	FORCEINLINE TArray(const TArray& Other) : Allocator(), Count(0) { CopyFrom(Other); }
	FORCEINLINE TArray(TArray&& Other) : Allocator(), Count(0) { MoveFrom(Move(Other)); }
	FORCEINLINE TArray(ElementType* InData, SizeType InCount, bool Copy = true) 
		: Allocator()
		, Count(0)
	{
		if(Copy) CopyFrom(InData, InCount);
		else MoveFrom(InData, InCount); 
	}

public: // Destructor

	FORCEINLINE ~TArray() { EmptyImpl(); }

public: // Operators

	FORCEINLINE bool operator==(const TArray& Other) const { return Allocator == Other.Allocator; }
	FORCEINLINE bool operator!=(const TArray& Other) const { return Allocator != Other.Allocator; }

	FORCEINLINE TArray& operator=(const TArray& Other) { CopyFrom(Other); return *this; }
	FORCEINLINE TArray& operator=(TArray&& Other) { MoveFrom(Move(Other)); return *this; }

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

public: // Add

	SizeType Add(const ElementType& Value)
	{
		AddImpl(Value);
		return Count - 1;
	}

	SizeType Add(ElementType&& Value)
	{
		AddImpl(Move(Value));
		return Count - 1;
	}

	FORCEINLINE ElementType& Add_GetRef(const ElementType& Value)
	{
		return *AddImpl(Value);
	}

	FORCEINLINE ElementType& Add_GetRef(ElementType&& Value)
	{
		return *AddImpl(Move(Value));
	}

public: // Remove
		// * Swap is faster version of Remove
		// * but does not preserve order

	FORCEINLINE void RemoveAt(SizeType Index)
	{
		if(!IsValidIndex(Index)) return;
		RemoveImpl(Index);
	}

	FORCEINLINE ElementType RemoveAt_GetCopy(SizeType Index)
	{
		if(!IsValidIndex(Index)) return;

		// Tries to use move constructor
		ElementType copy(Move(*GetElementAtImpl(Index)));
		RemoveImpl(Index);

		return copy;
	}

	FORCEINLINE void RemoveAtSwap(SizeType Index)
	{
		if(!IsValidIndex(Index)) return;
		RemoveSwapImpl(Index);
	}

	FORCEINLINE ElementType RemoveAtSwap_GetCopy(SizeType Index)
	{
		if(!IsValidIndex(Index)) return;

		// Tries to use move constructor
		ElementType copy(Move(*GetElementAtImpl(Index)));
		RemoveSwapImpl(Index);

		return copy;
	}

public: // Get

	FORCEINLINE const ElementType* GetAt(SizeType Index) const
	{
		if(!IsValidIndex(Index)) return nullptr;
		return IsValidIndex(Index) ? GetElementAtImpl(Index) : nullptr;
	}

	FORCEINLINE ElementType* GetAt(SizeType Index)
	{
		return IsValidIndex(Index) ? GetElementAtImpl(Index) : nullptr;
	}

	FORCEINLINE const ElementType* GetFirst() const
	{
		return Count > 0 ? GetElementAtImpl(0) : nullptr;
	}

	FORCEINLINE ElementType* GetFirst()
	{
		return Count > 0 ? GetElementAtImpl(0) : nullptr;
	}

	FORCEINLINE const ElementType* GetLast() const
	{
		return Count > 0 ? GetElementAtImpl(Count - 1) : nullptr;
	}

	FORCEINLINE ElementType* GetLast()
	{
		return Count > 0 ? GetElementAtImpl(Count - 1) : nullptr;
	}

public: // Find Index

	SizeType FindIndex(const ElementType& Value) const 
	{
		for(SizeType i = 0; i < Count; ++i)
		{
			if(IsSameElementImpl(GetElementAtImpl(i), (void*)&Value))
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

	FORCEINLINE ArrayIteratorType begin() { return ArrayIteratorType(Count > 0 ? GetElementAtImpl(0) : nullptr); }
	FORCEINLINE ConstArrayIteratorType begin() const { return ConstArrayIteratorType(Count > 0 ? GetElementAtImpl(0) : nullptr); }

	FORCEINLINE ArrayIteratorType end() { return ArrayIteratorType(Count > 0 ? GetElementAtImpl(Count) : nullptr); }
	FORCEINLINE ConstArrayIteratorType end() const { return ConstArrayIteratorType(Count > 0 ? GetElementAtImpl(Count) : nullptr); }

private: // Helpers -> Getters

	FORCEINLINE ElementType* GetElementAtImpl(SizeType Idx) const { return Allocator.GetData() + Idx; }

private: // Helpers -> Manipulation

	ElementType* AddImpl(const ElementType& Value)
	{
		++Count;
		RelocateIfNeededImpl();

		ElementType* newElement = GetElementAtImpl(Count - 1);
		NMemoryUtilities::CallCopyConstructor(newElement, Value);
		
		return newElement;
	}

	ElementType* AddImpl(ElementType&& Value)
	{
		++Count;
		RelocateIfNeededImpl();

		ElementType* newElement = GetElementAtImpl(Count - 1);
		NMemoryUtilities::CallCopyConstructor(newElement, Move(Value));
		
		return newElement;
	}

	void RemoveSwapImpl(SizeType Index)
	{
		// Pointer still can be used
		NMemoryUtilities::CallDestructor(GetElementAtImpl(Index));

		if(Index != Count - 1)
		{
			// Swaps last element with this
			SMemory::Copy(
				GetElementAtImpl(Index),
				GetLast(),
				sizeof(ElementType)
			);
		}

		--Count;

		RelocateIfNeededImpl();
	}

	void RemoveImpl(SizeType Index)
	{
		// Pointer still can be used
		NMemoryUtilities::CallDestructor(GetElementAtImpl(Index));

		if(Index != Count - 1)
		{
			// Moves entire allocation by one index down
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

		for(SizeType i = Num; i < Count; ++i)
		{
			NMemoryUtilities::CallDestructor(
				GetElementAtImpl(i)
			);
		}

		// Copy to temporary allocator
		AllocatorType tmp;
		SMemory::Copy(
			tmp.GetData(),
			Allocator.GetData(),
			sizeof(ElementType) * Num
		);

		// Move data back to main allocator
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
		Allocator.Allocate(Allocator.GetCount() - Num);
	}

	void EmptyImpl() 
	{
		if(Count > 0)
		{
			for(SizeType i = 0; i < Count; ++i)
			{
				NMemoryUtilities::CallDestructor(
					GetElementAtImpl(i)
				);
			}

			Allocator.Release();
			Count = 0; 
		}
	}

private: // Helpers -> Cross manipulation (Array)

	void CopyFrom(const ElementType* InData, SizeType InCount)
	{
		EmptyImpl();

		if(InData)
		{
			Allocator.Allocate(InCount);

			SMemory::Copy(
				Allocator.GetData(),
				InData,
				sizeof(ElementType) * InCount
			);

			Count = InCount;
		}
	}
	
	void CopyFrom(const TArray& Other) { CopyFrom(Other.GetData(), Other.Count); }
	
	void MoveFrom(ElementType* InData, SizeType InCount)
	{
		EmptyImpl();

		if(InData)
		{
			Allocator.SetData(InData);
			Allocator.SetCount(InCount);
			Count = InCount;
		}
	}

	void MoveFrom(TArray&& Other)
	{
		MoveFrom(Other.GetData(), Other.Count);

		{
			Other.Allocator.SetData(nullptr);
			Other.Allocator.SetCount(0);
			Other.Count = 0;
		}
	}

private: // Helpers -> Others

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

	FORCEINLINE bool IsSameElementImpl(const ElementType* Lhs, const ElementType* Rhs)
	{
		// Compare bytes instead of using == operator (that might not be provided)
		return SMemory::Compare(Lhs, Rhs, sizeof(ElementType)) == 0;
	}

private: // Fields

	AllocatorType Allocator;
	SizeType Count;
};