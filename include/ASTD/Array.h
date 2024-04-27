// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <initializer_list>

#include "ASTD/Core/Build.h"

#include "ASTD/Archive.h"
#include "ASTD/TypeTraits.h"
#include "ASTD/Math.h"
#include "ASTD/Memory.h"

#include "ASTD/ArrayAllocator.h"

template<typename InElementType, typename InAllocator = TArrayAllocator<InElementType>>
class TArray
{
public:
	// Types
	/////////////////////////////////

	typedef InElementType ElementType;
	typedef InAllocator AllocatorType;

	typedef ElementType* ArrayIteratorType;
	typedef const ElementType* ConstArrayIteratorType;

	typedef typename AllocatorType::SizeType SizeType;
	typedef std::initializer_list<ElementType> ElementListType;

	// Asserts
	/////////////////////////////////

	static_assert(!TIsSame<ElementType, void>::Value && !TIsReference<ElementType>::Value, "Element type is not valid");
	static_assert(!TIsSame<AllocatorType, void>::Value && TIsSignedType<SizeType>::Value, "Allocator type is not valid");

	// Constructors
	/////////////////////////////////

	FORCEINLINE TArray() : _allocator(), _num(0) {}
	FORCEINLINE TArray(const TArray& other) : _allocator(), _num(0) { AppendImpl(other); }
	FORCEINLINE TArray(TArray&& other) : _allocator(), _num(0) { ReplaceImpl(Move(other)); }
	FORCEINLINE TArray(SizeType count, bool reserveOnly = false) : _allocator(), _num(0) { ResizeImpl(count, reserveOnly); }
	FORCEINLINE TArray(const ElementListType& list)
		: _allocator()
		, _num(0)
	{
		AppendImpl(list.begin(), list.size());
	}

	FORCEINLINE TArray(const ElementType* data, SizeType count)
		: _allocator()
		, _num(0)
	{
		AppendImpl(data, count);
	}

	// Destructor
	/////////////////////////////////

	FORCEINLINE ~TArray() { EmptyImpl(true); }

	// Compare operators
	/////////////////////////////////

	FORCEINLINE bool operator==(const TArray& other) const { return CompareAllocatorsPrivate(&_allocator, &other._allocator, _num); }
	FORCEINLINE bool operator!=(const TArray& other) const { return CompareAllocatorsPrivate(&_allocator, &other._allocator, _num); }

	// Assign operators
	/////////////////////////////////

	FORCEINLINE TArray& operator=(const TArray& other) { EmptyImpl(true); AppendImpl(other); return *this; }
	FORCEINLINE TArray& operator=(TArray&& other) { ReplaceImpl(Move(other)); return *this; }

	FORCEINLINE TArray& operator=(const ElementListType& list) { EmptyImpl(true); AppendImpl(list.begin(), list.size()); return *this; }

	// Get operators
	/////////////////////////////////

	FORCEINLINE ElementType& operator[](SizeType idx) { return *GetElementAtImpl(idx); }
	FORCEINLINE const ElementType& operator[](SizeType idx) const { return *GetElementAtImpl(idx); }

	// Property getters
	/////////////////////////////////

	FORCEINLINE const ElementType* GetData() const { return _allocator.GetData(); }
	FORCEINLINE ElementType* GetData() { return _allocator.GetData(); }

	FORCEINLINE SizeType GetNum() const { return _num; }
	FORCEINLINE SizeType GetReservedNum() const { return _allocator.GetSize(); }

	// Validations
	/////////////////////////////////

	FORCEINLINE bool IsEmpty() const { return _num == 0; }
	FORCEINLINE bool IsValidIndex(SizeType idx) const { return idx >= 0 && idx < _num; }

	FORCEINLINE SizeType GetFirstIndex() const { return 0; }
	FORCEINLINE SizeType GetLastIndex() const { return _num > 0 ? _num - 1 : 0; }

	// Append
	/////////////////////////////////

	FORCEINLINE void Append(const TArray& other) { AppendImpl(other); }
	FORCEINLINE void Append(TArray&& other) { AppendImpl(Move(other)); }

	FORCEINLINE void Append(const ElementType& val, SizeType numToAdd) { AddImpl(val, numToAdd); }
	FORCEINLINE void Append(const ElementListType& list) { AppendImpl(list.begin(), list.size()); }
	FORCEINLINE void Append(const ElementType* data, SizeType count) { AppendImpl(data, count); } 

	FORCEINLINE void AppendUnitialized(SizeType numToAdd) { if(numToAdd > 0) GrowImpl(_num + numToAdd); }

	// Replace
	/////////////////////////////////

	FORCEINLINE void Replace(TArray& other) { ReplaceImpl(Move(other)); }
	FORCEINLINE void Replace(TArray&& other) { ReplaceImpl(Move(other)); }

	// Add
	/////////////////////////////////

	FORCEINLINE SizeType Add(const ElementType& val)
	{
		AppendImpl(&val, 1);
		return _num - 1;
	}

	FORCEINLINE SizeType Add(ElementType&& val)
	{
		AppendImpl(&val, 1, true);
		return _num - 1;
	}

	FORCEINLINE SizeType AddUnitialized()
	{
		AddUnitializedImpl();
		return _num - 1;
	}

	FORCEINLINE ElementType& Add_GetRef(const ElementType& val)
	{
		AppendImpl(&val, 1);
		return *GetElementAtImpl(_num - 1);
	}

	FORCEINLINE ElementType& Add_GetRef(ElementType&& val)
	{
		AppendImpl(&val, 1, true);
		return *GetElementAtImpl(_num - 1);
	}

	FORCEINLINE ElementType& AddUnitialized_GetRef()
	{
		AddUnitializedImpl();
		return *GetElementAtImpl(_num - 1);
	}

	FORCEINLINE void Push(const ElementType& val) { AddImpl(val); }
	FORCEINLINE void Push(ElementType&& val) { AddImpl(Move(val)); }

	// Remove
	/////////////////////////////////
	// * Swap is faster version of Remove
	// * but does not preserve order

	FORCEINLINE void Remove(const ElementType& val)
	{
		SizeType foundIndex = FindIndex(val);
		if(foundIndex != INDEX_NONE)
		{
			RemoveImpl(foundIndex);
		}
	}

	FORCEINLINE void RemoveSwap(const ElementType& val)
	{
		SizeType foundIndex = FindIndex(val);
		if(foundIndex != INDEX_NONE)
		{
			RemoveSwapImpl(foundIndex);
		}
	}

	FORCEINLINE void RemoveAt(SizeType idx)
	{
		if(!IsValidIndex(idx)) return;
		RemoveImpl(idx);
	}

	FORCEINLINE void RemoveAtSwap(SizeType idx)
	{
		if(!IsValidIndex(idx)) return;
		RemoveSwapImpl(idx);
	}

	FORCEINLINE ElementType RemoveAt_GetCopy(SizeType idx)
	{
		if(!IsValidIndex(idx)) return {};

		// Tries to use move constructor
		ElementType copy(Move(*GetElementAtImpl(idx)));
		RemoveImpl(idx);

		return copy;
	}

	FORCEINLINE ElementType RemoveAtSwap_GetCopy(SizeType idx)
	{
		if(!IsValidIndex(idx)) return {};

		// Tries to use move constructor
		ElementType copy(Move(*GetElementAtImpl(idx)));
		RemoveSwapImpl(idx);

		return copy;
	}

	FORCEINLINE void Pop()
	{
		if(_num > 0)
		{
			RemoveImpl(_num - 1);
		}
	}

	// Swap
	/////////////////////////////////

	FORCEINLINE void Swap(SizeType firstIdx, SizeType secondIdx)
	{
		if(!IsValidIndex(firstIdx) || !IsValidIndex(secondIdx)) return;

		SwapImpl(firstIdx, secondIdx, 1);
	}

	FORCEINLINE void SwapRange(SizeType firstIdx, SizeType secondIdx, SizeType num = 1)
	{
		if(!IsValidIndex(firstIdx) || !IsValidIndex(secondIdx)) return;
		else if(firstIdx + num > _num || secondIdx + num > _num) return;

		SwapImpl(firstIdx, secondIdx, num);
	}

	// Get
	/////////////////////////////////

	FORCEINLINE const ElementType* GetAt(SizeType idx) const { return IsValidIndex(idx) ? GetElementAtImpl(idx) : nullptr; }
	FORCEINLINE ElementType* GetAt(SizeType idx) { return IsValidIndex(idx) ? GetElementAtImpl(idx) : nullptr; }

	FORCEINLINE const ElementType* GetFirst() const { return _num > 0 ? GetElementAtImpl(0) : nullptr; }
	FORCEINLINE ElementType* GetFirst() { return _num > 0 ? GetElementAtImpl(0) : nullptr; }

	FORCEINLINE const ElementType* GetLast() const { return _num > 0 ? GetElementAtImpl(_num - 1) : nullptr; }
	FORCEINLINE ElementType* GetLast() { return _num > 0 ? GetElementAtImpl(_num - 1) : nullptr; }

	// Find Index
	/////////////////////////////////

	SizeType FindIndex(const ElementType& val) const
	{
		for(SizeType i = 0; i < _num; ++i)
		{
			if(CompareElementsPrivate(GetElementAtImpl(i), &val))
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

	template<typename Functor>
	SizeType FindIndexByFunc(Functor&& func) const
	{
		for(SizeType i = 0; i < _num; ++i)
		{
			if(func((const ElementType&)*GetElementAtImpl(i)))
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

	template<typename KeyType>
	SizeType FindIndexByKey(KeyType key) const
	{
		for(SizeType i = 0; i < _num; ++i)
		{
			if(*GetElementAtImpl(i) == key)
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

	// Find Element
	/////////////////////////////////

	template<typename Functor>
	const ElementType* FindByFunc(Functor&& func) const
	{
		const SizeType foundIdx = FindIndexByFunc(Move(func));
		return foundIdx != INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename Functor>
	ElementType* FindByFunc(Functor&& func)
	{
		const SizeType foundIdx = FindIndexByFunc(Move(func));
		return foundIdx != INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename KeyType>
	const ElementType* FindByKey(KeyType key) const
	{
		const SizeType foundIdx = FindByKey(key);
		return foundIdx != INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename KeyType>
	ElementType* FindByKey(KeyType key)
	{
		const SizeType foundIdx = FindByKey(key);
		return foundIdx != INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	// Contains
	/////////////////////////////////

	FORCEINLINE bool Contains(const ElementType& val) const { return FindIndex(val) != INDEX_NONE; }

	template<typename Functor>
	FORCEINLINE bool ContainsByFunc(Functor&& func) const { return FindByFunc(Move(func)) != INDEX_NONE; }

	template<typename KeyType>
	FORCEINLINE bool ContainsByKey(KeyType key) const { return FindByKey(key) != INDEX_NONE; }

	// Other
	/////////////////////////////////

	FORCEINLINE void ShrinkToFit() { if(_num < _allocator.GetSize()) ShrinkImpl(_num); }
	FORCEINLINE void Shrink(SizeType num) { if(num < _num) ShrinkImpl(num); }
	FORCEINLINE void Grow(SizeType num) { if(num > _num) GrowImpl(num); }

	FORCEINLINE void Resize(SizeType num) { ResizeImpl(num, false); }
	FORCEINLINE void Reserve(SizeType num) { if(num > _num) ReserveImpl(num); }

	FORCEINLINE void Reset() { EmptyImpl(true); }
	FORCEINLINE void Empty(bool releaseResources = true) { EmptyImpl(releaseResources); }

	// Iterators
	/////////////////////////////////

	FORCEINLINE ArrayIteratorType begin() { return _num > 0 ? GetElementAtImpl(0) : nullptr; }
	FORCEINLINE ConstArrayIteratorType begin() const { return _num > 0 ? GetElementAtImpl(0) : nullptr; }

	FORCEINLINE ArrayIteratorType end() { return _num > 0 ? GetElementAtImpl(_num) : nullptr; }
	FORCEINLINE ConstArrayIteratorType end() const { return _num > 0 ? GetElementAtImpl(_num) : nullptr; }

private:

	FORCEINLINE ElementType* GetElementAtImpl(SizeType idx) const { return _allocator.GetData() + idx; }

	void AddUnitializedImpl(SizeType count = 1)
	{
		if(count > 0)
		{
			_num += count;
			RealocateIfNeededImpl();
		}
	}

	void RemoveSwapImpl(SizeType idx)
	{
		DestructElementsPrivate(GetElementAtImpl(idx));

		if(idx != _num - 1)
		{
			// Swaps last element with this
			SMemory::Move(
				GetElementAtImpl(idx),
				GetElementAtImpl(_num - 1),
				sizeof(ElementType)
			);
		}

		--_num;

		RealocateIfNeededImpl();
	}

	void RemoveImpl(SizeType idx)
	{
		DestructElementsPrivate(GetElementAtImpl(idx));

		if(idx != _num - 1)
		{
			// Moves entire allocation by one index down

			// NOTE(jan.kristian.fisera): 
			// * Is it worth to cache start index and try to move from start in case that would be less iterations ?
			SMemory::Move(
				GetElementAtImpl(idx),
				GetElementAtImpl(idx + 1),
				sizeof(ElementType) * (_num - idx - 1)
			);
		}

		--_num;

		RealocateIfNeededImpl();
	}

	void SwapImpl(SizeType firstIdx, SizeType secondIdx, SizeType num)
	{
		// Copy to temporary storage at heep
		AllocatorType tmp;
		tmp.Allocate(num);
		SMemory::Copy(
			tmp.GetData(),
			GetElementAtImpl(firstIdx),
			sizeof(ElementType) * num
		);

		// Do swap to first index
		// * elements from second idx to first
		SMemory::Copy(
			GetElementAtImpl(firstIdx),
			GetElementAtImpl(secondIdx),
			sizeof(ElementType) * num
		);

		// Do swap to second index
		// * copied elements from first idx to second
		SMemory::Copy(
			GetElementAtImpl(secondIdx),
			tmp.GetData(),
			sizeof(ElementType) * num
		);
	}

	void ShrinkImpl(SizeType num)
	{
		if(num == 0)
		{
			EmptyImpl(true);
		}
		else
		{
			DestructElementsPrivate(GetElementAtImpl(num), _num - num);

			// Copy to temporary allocator
			AllocatorType tmp;
			tmp.Allocate(num);
			SMemory::Copy(
				tmp.GetData(),
				_allocator.GetData(),
				sizeof(ElementType) * num
			);

			// Move data back to main allocator
			_allocator.Release();
			_allocator.Allocate(num);
			SMemory::Copy(
				_allocator.GetData(),
				tmp.GetData(),
				sizeof(ElementType) * num
			);

			tmp.SetData(nullptr);
			tmp.SetSize(0);

			_num = num;
		}
	}

	void GrowImpl(SizeType num)
	{
		_num = num;
		ReserveImpl(num);
	}

	void ReserveImpl(SizeType num)
	{
		_allocator.Allocate(num - _allocator.GetSize());
	}

	void EmptyImpl(bool release)
	{
		if(_num > 0)
		{
			DestructElementsPrivate(_allocator.GetData(), _num);

			if(release)
			{
				_allocator.Release();
			}

			_num = 0; 
		}
	}

	void AppendImpl(const ElementType* data, SizeType count)
	{
		if(count > 0)
		{
			const SizeType oldCount = _num;

			_num += count;
			RealocateIfNeededImpl();
			SMemory::CopyElement(_allocator.GetData() + oldCount, data, count);
		}
	}

	void AppendImpl(ElementType* data, SizeType count, bool preferMove)
	{
		if(preferMove && _num == 1)
		{
			const SizeType oldCount = _num;

			_num += count;
			RealocateIfNeededImpl();

			SMemory::MoveElement(_allocator.GetData() + oldCount, data);
		}
		else
		{
			AppendImpl(data, count);
		}
	}

	void AppendImpl(TArray&& other)
	{
		AppendImpl(other.GetData(), other._num, true);

		{
			other._allocator.Release();
			other._num = 0;
		}
	}

	FORCEINLINE void AppendImpl(const TArray& other) { AppendImpl(other.GetData(), other._num); }

	void ReplaceImpl(TArray&& other)
	{
		EmptyImpl(true);

		_allocator.SetData(other._allocator.GetData());
		_allocator.SetSize(other._allocator.GetSize());
		_num = other._num;

		other._allocator.SetData(nullptr);
		other._allocator.SetSize(0);
		other._num = 0;
	}

	void ResizeImpl(SizeType num, bool reserveOnly)
	{
		if(num > _num)
		{
			if(reserveOnly)
				ReserveImpl(num);
			else
				GrowImpl(num);
		}
		else if(num < _num)
		{
			ShrinkImpl(num);
		}
	}

	void RealocateIfNeededImpl()
	{
		const SizeType reserved = _allocator.GetSize();

		if(_num > reserved)
		{
			ReserveImpl(SMath::CeilToPowerOfTwo<uint64>(_num));
		}
	}

	FORCEINLINE static void DestructElementsPrivate(ElementType* element, SizeType count = 1)
	{
		for(SizeType i = 0; i < count; ++i)
		{
			SMemory::DestructElement(element);
			++element;
		}
	}

	FORCEINLINE static bool CompareAllocatorsPrivate(const AllocatorType* lhs, const AllocatorType* rhs, SizeType count)
	{
		return (lhs->GetSize() >= count && rhs->GetSize() >= count) ?
			SMemory::Compare(lhs->GetData(), rhs->GetData(), sizeof(ElementType) * count) == 0 : false;
	}

	FORCEINLINE static bool CompareElementsPrivate(const ElementType* lhs, const ElementType* rhs)
	{
		// Compare bytes instead of using == operator (that might not be provided)
		return SMemory::Compare(lhs, rhs, sizeof(ElementType)) == 0;
	}

	AllocatorType _allocator = {};
	SizeType _num = INDEX_NONE;
};

// Archive operator<< && operator>>
////////////////////////////////////////////

template<typename T>
static SArchive& operator<<(SArchive& ar, const TArray<T>& array)
{
	for (uint16 i = 0; i < array.GetNum(); ++i)
	{
		ar << array[i];
	}

	return ar;
}

template<typename T>
static SArchive& operator>>(SArchive& ar, TArray<T>& array)
{
	for (uint16 i = 0; i < array.GetNum(); ++i)
	{
		ar >> array[i];
	}

	return ar;
}
