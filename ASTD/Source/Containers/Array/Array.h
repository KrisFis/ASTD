
#pragma once

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeTraits.h"

#include "Containers/Array/Allocator/ArrayAllocator.h"
#include "Containers/Array/Internals/ArrayInternalUtils.h"

// TODO(jan.kristian.fisera): Implement
// * Type traits
// * Iterators
template<typename InElementType, typename InAllocator = CArrayAllocator>
class TArray
{

	static_assert(!TIsSame<InAllocator, void>::Value, "Invalid allocator type");

private: // Setup

	typedef InElementType ElementType;
	typedef InAllocator AllocatorType;

	static constexpr TSize ELEMENT_SIZE = SizeOf<ElementType>();

public: // Constructors

	FORCEINLINE TArray() : Allocator(), Count(0) {}
	FORCEINLINE TArray(const TArray& Other) { CopyFrom(Other); }
	FORCEINLINE TArray(TArray&& Other) { MoveFrom(Move(Other)); }

public: // Destructor

	~TArray() {}

public: // Operators

	TArray& operator=(const TArray& Other) { CopyFrom(Other); return *this; }
	TArray& operator=(TArray&& Other) { MoveFrom(Move(Other)); return *this; }

	ElementType& operator[](uint32 Index) { return *GetElementAtImpl(Index); }
	const ElementType& operator[](uint32 Index) const { return *GetElementAtImpl(Index); }

public: // Property getters

	FORCEINLINE const ElementType* GetData() const { return GetDataImpl(); }
	FORCEINLINE ElementType* GetData() { return GetDataImpl(); }

	FORCEINLINE uint32 GetCount() const { return Count; }
	FORCEINLINE uint32 GetReserved() const { return Allocator.GetCount(); }

public: // Validations

	FORCEINLINE bool IsValidIndex(uint32 Idx) const { return Idx < Count; }

public: // Add

	uint32 Add(const ElementType& Value)
	{
		AddImpl(Value);
		return Count - 1;
	}

	uint32 Add(ElementType&& Value)
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

	FORCEINLINE void RemoveAt(uint32 Index)
	{
		// TODO(jan.kristian.fisera): Add better check
		if(!IsValidIndex(Index)) return;
		RemoveImpl(Index);
	}

	FORCEINLINE ElementType RemoveAt_GetCopy(uint32 Index)
	{
		// TODO(jan.kristian.fisera): Add better check
		if(!IsValidIndex(Index)) return;

		// Tries to use move constructor
		ElementType copy(Move(*GetElementAtImpl(Index)));
		RemoveImpl(Index);

		return copy;
	}

public: // Get

	const ElementType* GetAt(uint32 Index) const
	{
		if(!IsValidIndex(Index)) return nullptr;
		return GetElementAtImpl(Index);
	}

	ElementType* GetAt(uint32 Index)
	{
		if(!IsValidIndex(Index)) return nullptr;
		return GetElementAtImpl(Index);
	}

public: // Find Index

	int64 FindIndex(const ElementType& Value) const 
	{ 
		for(uint32 i = 0; i < Count; ++i)
		{
			// Compare bytes instead of using == operator (that might not be provided)
			if(SMemory::Compare(GetElementAtImpl(i), (void*)&Value, ELEMENT_SIZE) == 0)
			{
				return i;
			}
		}

		return -1;
	}

	template<typename Functor>
	int64 FindIndexByFunc(Functor&& Func) const
	{
		for(uint32 i = 0; i < Count; ++i)
		{
			if(Func((const ElementType&)*GetElementAtImpl(i)))
			{
				return i;
			}
		}

		return -1;
	}

	template<typename KeyType>
	int64 FindIndexByKey(KeyType Key) const
	{
		for(uint32 i = 0; i < Count; ++i)
		{
			if(*GetElementAtImpl(i) == Key)
			{
				return i;
			}
		}

		return -1;
	}

public: // Find Element

	template<typename Functor>
	const ElementType* FindByFunc(Functor&& Func) const
	{
		const int64 foundIdx = FindIndexByFunc(Move(Func));
		return foundIdx != -1 ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename Functor>
	ElementType* FindByFunc(Functor&& Func)
	{
		const int64 foundIdx = FindIndexByFunc(Move(Func));
		return foundIdx != -1 ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename KeyType>
	const ElementType* FindByKey(KeyType Key) const
	{
		const int64 foundIdx = FindByKey(Key);
		return foundIdx != -1 ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename KeyType>
	ElementType* FindByKey(KeyType Key)
	{
		const int64 foundIdx = FindByKey(Key);
		return foundIdx != -1 ? GetElementAtImpl(foundIdx) : nullptr;
	}

public: // Contains

	FORCEINLINE bool Contains(const ElementType& Value) const { return FindIndex(Value) != -1; }

	template<typename Functor>
	FORCEINLINE bool ContainsByFunc(Functor&& Func) const { return FindByFunc(Move(Func)) != -1; }

	template<typename KeyType>
	FORCEINLINE bool ContainsByKey(KeyType Key) const { return FindByKey(Key) != -1; }

public: // Other

	void Shrink(uint32 Num)
	{
		if(Num >= Allocator.GetCount()) return;
		else if(Num == 0)
		{
			RemoveAll();
			return;
		}

		for(uint32 i = Num; i < Count; ++i)
		{
			NMemoryUtilities::CallDestructor(
				GetElementAtImpl(i)
			);
		}

		AllocatorType tmp;
		NArrayInternalUtils::AllocatorCopyData<ElementType>(
			tmp, Allocator, Num
		);

		NArrayInternalUtils::AllocatorReplace(
			Allocator, tmp
		);
	}

	void Reserve(uint32 Num)
	{
		if(Num <= Allocator.GetCount()) return;
		Allocator.Allocate(ELEMENT_SIZE, Allocator.GetCount() - Num);
	}

	FORCEINLINE void Reset() { RemoveAll(); }
	FORCEINLINE void Empty() { RemoveAll(); }

private: // Helpers -> Getters

	FORCEINLINE ElementType* GetDataImpl() const { return (ElementType*)Allocator.GetData(); }
	FORCEINLINE ElementType* GetElementAtImpl(uint32 Idx) const { return (ElementType*)Allocator.GetData() + Idx; }

private: // Helpers -> Manipulation

	ElementType* AddImpl(const ElementType& Value)
	{
		ElementType* newElement = GetNextImpl();
		NMemoryUtilities::CallConstructor(newElement, Value);
		return newElement;
	}

	ElementType* AddImpl(ElementType&& Value)
	{
		ElementType* newElement = GetNextImpl();
		NMemoryUtilities::CallConstructor(newElement, Move(Value));
		return newElement;
	}

	void RemoveImpl(uint32 Index)
	{
		// Pointer still can be used
		// * NOTE(jan.kristian.fisera): Should we keep track of removed ?
		NMemoryUtilities::CallDestructor(GetElementAtImpl(Index));

		// NOTE(jan.kristian.fisera): Automatic shrinkin ?
	}

	void RemoveAll() 
	{
		if(Count > 0)
		{
			for(uint32 i = 0; i < Count; ++i)
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

	void CopyFrom(const TArray& Other)
	{
		NArrayInternalUtils::AllocatorCopyData<ElementType>(
			Allocator, Other.Allocator
		);

		Count = Other.Count;
	}

	void MoveFrom(TArray&& Other)
	{
		NArrayInternalUtils::AllocatorReplace(
			Allocator, Move(Other.Allocator)
		);

		Count = Other.Count;
		Other.Count = 0;
	}

private: // Helpers -> Others

	ElementType* GetNextImpl()
	{
		if(Allocator.GetCount() < Count + 1)
		{
			Allocator.Allocate(ELEMENT_SIZE, (Count + 1) * 2);
		}

		++Count;
		return ((ElementType*)Allocator.GetData()) + (Count - 1);
	}

private: // Fields

	AllocatorType Allocator;
	uint32 Count;
};