
#pragma once

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeTraits.h"

#include "Containers/Array/Allocator/ArrayAllocator.h"
#include "Containers/Array/Internals/ArrayInternalUtils.h"
#include "Containers/Array/ArrayIterator.h"

// TODO(jan.kristian.fisera): Implement
// * Type traits
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

	FORCEINLINE void RemoveAt(SizeType Index)
	{
		// TODO(jan.kristian.fisera): Add better check
		if(!IsValidIndex(Index)) return;
		RemoveImpl(Index);
	}

	FORCEINLINE ElementType RemoveAt_GetCopy(SizeType Index)
	{
		// TODO(jan.kristian.fisera): Add better check
		if(!IsValidIndex(Index)) return;

		// Tries to use move constructor
		ElementType copy(Move(*GetElementAtImpl(Index)));
		RemoveImpl(Index);

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

	void Shrink(SizeType Num)
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

		AllocatorType tmp;
		NArrayInternalUtils::AllocatorCopyData(
			tmp, Allocator, Num
		);

		NArrayInternalUtils::AllocatorMoveData(
			Allocator, tmp
		);
	}

	void Reserve(SizeType Num)
	{
		if(Num <= Allocator.GetCount()) return;
		Allocator.Allocate(Allocator.GetCount() - Num);
	}

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
		ElementType* newElement = GetNextImpl();
		NMemoryUtilities::CallCopyConstructor(newElement, Value);
		return newElement;
	}

	ElementType* AddImpl(ElementType&& Value)
	{
		ElementType* newElement = GetNextImpl();
		NMemoryUtilities::CallMoveConstructor(newElement, Move(Value));
		return newElement;
	}

	void RemoveImpl(SizeType Index)
	{
		// Pointer still can be used
		// * NOTE(jan.kristian.fisera): Should we keep track of removed ?
		NMemoryUtilities::CallDestructor(GetElementAtImpl(Index));

		// NOTE(jan.kristian.fisera): Automatic shrinkin ?
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
			NArrayInternalUtils::AllocatorCopyData(
				Allocator, InData, InCount
			);

			Count = InCount;
		}
	}

	void CopyFrom(const TArray& Other)
	{
		EmptyImpl();

		if(Other.Allocator.GetData())
		{
			NArrayInternalUtils::AllocatorCopyData(
				Allocator, Other.Allocator
			);

			Count = Other.Count;
		}
	}

	void MoveFrom(ElementType* InData, SizeType InCount)
	{
		EmptyImpl();

		if(InData)
		{
			NArrayInternalUtils::AllocatorMoveData(
				Allocator, InData, InCount
			);

			Count = InCount;
		}
	}

	void MoveFrom(TArray&& Other)
	{
		EmptyImpl();

		if(Other.Allocator.GetData())
		{
			NArrayInternalUtils::AllocatorMoveData(
				Allocator, Move(Other.Allocator)
			);

			Count = Other.Count;
		}

		Other.Count = 0;
	}

private: // Helpers -> Others

	ElementType* GetNextImpl()
	{
		if(Allocator.GetCount() < Count + 1)
		{
			Allocator.Allocate((Count + 1) * 2);
		}

		++Count;
		return Allocator.GetData() + (Count - 1);
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