// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <initializer_list>

#include "ASTD/Build.h"

#include "ASTD/Math.h"
#include "ASTD/Memory.h"
#include "ASTD/TypeTraits.h"

#include "ASTD/ArrayAllocator.h"

template<typename ElementT, typename AllocatorT>
class TArray
{
public:
	// Types
	/////////////////////////////////

	typedef ElementT ElementType;
	typedef AllocatorT AllocatorType;

	typedef ElementT* ArrayIteratorType;
	typedef const ElementT* ConstArrayIteratorType;

	typedef typename AllocatorT::SizeType SizeType;
	typedef std::initializer_list<ElementT> ElementListType;

	// Asserts
	/////////////////////////////////

	static_assert(!TIsSame<ElementT, void>::Value && !TIsReference<ElementT>::Value, "Element type is not valid");
	static_assert(!TIsSame<AllocatorT, void>::Value && TIsSigned<SizeType>::Value, "Allocator type is not valid");

	// Constructors
	/////////////////////////////////

	FORCEINLINE TArray() : _allocator(), _num(0) {}
	FORCEINLINE TArray(const TArray& other) : _allocator(), _num(0) { AppendImpl(other); }
	FORCEINLINE TArray(TArray&& other) : _allocator(), _num(0) { ReplaceImpl(Move(other)); }
	FORCEINLINE TArray(SizeType num, bool reserveOnly = false) : _allocator(), _num(0) { ResizeImpl(num, reserveOnly); }
	FORCEINLINE TArray(const ElementListType& list)
		: _allocator()
		, _num(0)
	{
		AppendImpl(list.begin(), list.size());
	}

	FORCEINLINE TArray(const ElementT* data, SizeType num)
		: _allocator()
		, _num(0)
	{
		AppendImpl(data, num);
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

	FORCEINLINE ElementT& operator[](SizeType idx) { return *GetElementAtImpl(idx); }
	FORCEINLINE const ElementT& operator[](SizeType idx) const { return *GetElementAtImpl(idx); }

	// Property getters
	/////////////////////////////////

	FORCEINLINE const ElementT* GetData() const { return _allocator.GetData(); }
	FORCEINLINE ElementT* GetData() { return _allocator.GetData(); }

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

	FORCEINLINE void Append(const ElementT& val, SizeType numToAdd) { AddImpl(val, numToAdd); }
	FORCEINLINE void Append(const ElementListType& list) { AppendImpl(list.begin(), list.size()); }
	FORCEINLINE void Append(const ElementT* data, SizeType num) { AppendImpl(data, num); }

	FORCEINLINE void AppendUnitialized(SizeType numToAdd) { if(numToAdd > 0) GrowImpl(_num + numToAdd); }

	// Replace
	/////////////////////////////////

	FORCEINLINE void Replace(TArray& other) { ReplaceImpl(Move(other)); }
	FORCEINLINE void Replace(TArray&& other) { ReplaceImpl(Move(other)); }

	// Add
	/////////////////////////////////

	FORCEINLINE SizeType Add(const ElementT& val)
	{
		AppendImpl(&val, 1);
		return _num - 1;
	}

	FORCEINLINE SizeType Add(ElementT&& val)
	{
		AppendImpl(&val, 1, true);
		return _num - 1;
	}

	FORCEINLINE SizeType AddUnitialized()
	{
		AddUnitializedImpl();
		return _num - 1;
	}

	FORCEINLINE ElementT& Add_GetRef(const ElementT& val)
	{
		AppendImpl(&val, 1);
		return *GetElementAtImpl(_num - 1);
	}

	FORCEINLINE ElementT& Add_GetRef(ElementT&& val)
	{
		AppendImpl(&val, 1, true);
		return *GetElementAtImpl(_num - 1);
	}

	FORCEINLINE ElementT& AddUnitialized_GetRef()
	{
		AddUnitializedImpl();
		return *GetElementAtImpl(_num - 1);
	}

	FORCEINLINE void Push(const ElementT& val) { AddImpl(val); }
	FORCEINLINE void Push(ElementT&& val) { AddImpl(Move(val)); }

	// Remove
	/////////////////////////////////
	// * Swap is faster version of Remove
	// * but does not preserve order

	FORCEINLINE void Remove(const ElementT& val)
	{
		SizeType foundIndex = FindIndex(val);
		if(foundIndex != INDEX_NONE)
		{
			RemoveImpl(foundIndex);
		}
	}

	FORCEINLINE void RemoveSwap(const ElementT& val)
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

	FORCEINLINE ElementT RemoveAt_GetCopy(SizeType idx)
	{
		if(!IsValidIndex(idx)) return {};

		// Tries to use move constructor
		ElementT copy(Move(*GetElementAtImpl(idx)));
		RemoveImpl(idx);

		return copy;
	}

	FORCEINLINE ElementT RemoveAtSwap_GetCopy(SizeType idx)
	{
		if(!IsValidIndex(idx)) return {};

		// Tries to use move constructor
		ElementT copy(Move(*GetElementAtImpl(idx)));
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

	FORCEINLINE const ElementT* GetAt(SizeType idx) const { return IsValidIndex(idx) ? GetElementAtImpl(idx) : nullptr; }
	FORCEINLINE ElementT* GetAt(SizeType idx) { return IsValidIndex(idx) ? GetElementAtImpl(idx) : nullptr; }

	FORCEINLINE const ElementT* GetFirst() const { return _num > 0 ? GetElementAtImpl(0) : nullptr; }
	FORCEINLINE ElementT* GetFirst() { return _num > 0 ? GetElementAtImpl(0) : nullptr; }

	FORCEINLINE const ElementT* GetLast() const { return _num > 0 ? GetElementAtImpl(_num - 1) : nullptr; }
	FORCEINLINE ElementT* GetLast() { return _num > 0 ? GetElementAtImpl(_num - 1) : nullptr; }

	// Find Index
	/////////////////////////////////

	SizeType FindIndex(const ElementT& val) const
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
			if(func((const ElementT&)*GetElementAtImpl(i)))
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
	const ElementT* FindByFunc(Functor&& func) const
	{
		const SizeType foundIdx = FindIndexByFunc(Move(func));
		return foundIdx != INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename Functor>
	ElementT* FindByFunc(Functor&& func)
	{
		const SizeType foundIdx = FindIndexByFunc(Move(func));
		return foundIdx != INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename KeyType>
	const ElementT* FindByKey(KeyType key) const
	{
		const SizeType foundIdx = FindByKey(key);
		return foundIdx != INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	template<typename KeyType>
	ElementT* FindByKey(KeyType key)
	{
		const SizeType foundIdx = FindByKey(key);
		return foundIdx != INDEX_NONE ? GetElementAtImpl(foundIdx) : nullptr;
	}

	// Contains
	/////////////////////////////////

	FORCEINLINE bool Contains(const ElementT& val) const { return FindIndex(val) != INDEX_NONE; }

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
	FORCEINLINE void Empty(SizeType newNum = 0) { EmptyImpl(newNum); }
	FORCEINLINE void Empty(bool keepResources) { EmptyImpl(keepResources ? _allocator.GetSize() : 0); }

	// Iterators
	/////////////////////////////////

	FORCEINLINE ArrayIteratorType Begin() { return _num > 0 ? GetElementAtImpl(0) : nullptr; }
	FORCEINLINE ArrayIteratorType begin() { return Begin(); }

	FORCEINLINE ConstArrayIteratorType Begin() const { return _num > 0 ? GetElementAtImpl(0) : nullptr; }
	FORCEINLINE ConstArrayIteratorType begin() const { return Begin(); }

	FORCEINLINE ArrayIteratorType End() { return _num > 0 ? GetElementAtImpl(_num) : nullptr; }
	FORCEINLINE ArrayIteratorType end() { return End(); }

	FORCEINLINE ConstArrayIteratorType End() const { return _num > 0 ? GetElementAtImpl(_num) : nullptr; }
	FORCEINLINE ConstArrayIteratorType end() const { return End(); }

private:

	FORCEINLINE ElementT* GetElementAtImpl(SizeType idx) const { return _allocator.GetData() + idx; }

	void AddUnitializedImpl(SizeType num = 1)
	{
		if(num > 0)
		{
			_num += num;
			ReallocateIfNeededImpl();
		}
	}

	void RemoveSwapImpl(SizeType idx)
	{
		DestructElementsPrivate(GetElementAtImpl(idx));

		if(idx != _num - 1)
		{
			// Swaps last element with this
			SMemory::Memmove(
				GetElementAtImpl(idx),
				GetElementAtImpl(_num - 1),
				sizeof(ElementT)
			);
		}

		--_num;

		ReallocateIfNeededImpl();
	}

	void RemoveImpl(SizeType idx)
	{
		DestructElementsPrivate(GetElementAtImpl(idx));

		if(idx != _num - 1)
		{
			// Moves entire allocation by one index down

			// NOTE(jan.kristian.fisera):
			// * Is it worth to cache start index and try to move from start in case that would be less iterations ?
			SMemory::Memmove(
				GetElementAtImpl(idx),
				GetElementAtImpl(idx + 1),
				sizeof(ElementT) * (_num - idx - 1)
			);
		}

		--_num;

		ReallocateIfNeededImpl();
	}

	void SwapImpl(SizeType firstIdx, SizeType secondIdx, SizeType num)
	{
		// Copy to temporary storage
		AllocatorT tmp;
		tmp.Allocate(num);
		SMemory::CopyObject(
			tmp.GetData(),
			GetElementAtImpl(firstIdx),
			sizeof(ElementT) * num
		);

		// Do swap to first index
		// * elements from second idx to first
		SMemory::CopyObject(
			GetElementAtImpl(firstIdx),
			GetElementAtImpl(secondIdx),
			sizeof(ElementT) * num
		);

		// Do swap to second index
		// * copied elements from first idx to second
		SMemory::CopyObject(
			GetElementAtImpl(secondIdx),
			tmp.GetData(),
			sizeof(ElementT) * num
		);
	}

	void ShrinkImpl(SizeType num)
	{
		if (_allocator.GetSize() == num)
		{
			// No Shrink
			return;
		}

		if (_num > num)
		{
			// Destruct what needs to be destructed
			DestructElementsPrivate(GetElementAtImpl(num), _num - num);
			_num = num;
		}

		// Copy to temporary allocator
		AllocatorT tmp;
		tmp.Allocate(num);
		SMemory::CopyObject(
			tmp.GetData(),
			_allocator.GetData(),
			num
		);

		// Move data back to main allocator
		_allocator.Release();
		_allocator.Allocate(num);
		SMemory::CopyObject(
			_allocator.GetData(),
			tmp.GetData(),
			num
		);

		tmp.SetData(nullptr);
		tmp.SetSize(0);
	}

	void GrowImpl(SizeType num)
	{
		_num = num;
		ReallocateIfNeededImpl();
	}

	void ReserveImpl(SizeType num)
	{
		_allocator.Allocate(num - _allocator.GetSize());
	}

	void EmptyImpl(SizeType newNum)
	{
		if (_num > 0)
		{
			DestructElementsPrivate(_allocator.GetData(), _num);
			_num = 0;
		}

		if (newNum > _allocator.GetSize())
		{
			ReserveImpl(newNum);
		}
		else if (newNum < _allocator.GetSize())
		{
			if (newNum == 0)
			{
				_allocator.Release();
			}
			else
			{
				ShrinkImpl(newNum);
			}
		}
	}

	void AppendImpl(const ElementT* data, SizeType num)
	{
		if(num > 0)
		{
			const SizeType oldCount = _num;

			_num += num;
			ReallocateIfNeededImpl();
			SMemory::CopyObject(_allocator.GetData() + oldCount, data, num);
		}
	}

	void AppendImpl(ElementT* data, SizeType num, bool preferMove)
	{
		if(preferMove && _num == 1)
		{
			const SizeType oldCount = _num;

			_num += num;
			ReallocateIfNeededImpl();

			SMemory::MoveObject(_allocator.GetData() + oldCount, data);
		}
		else
		{
			AppendImpl(data, num);
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

	void ReallocateIfNeededImpl()
	{
		const SizeType reserved = _allocator.GetSize();
		if(_num > reserved)
		{
			ReserveImpl(SMath::CeilToPowerOfTwo((uint64)_num));
		}
	}

	FORCEINLINE static void DestructElementsPrivate(ElementT* element, SizeType num = 1)
	{
		for(SizeType i = 0; i < num; ++i)
		{
			SMemory::DestructObject(element);
			++element;
		}
	}

	FORCEINLINE static bool CompareAllocatorsPrivate(const AllocatorT* lhs, const AllocatorT* rhs, SizeType size)
	{
		return (lhs->GetSize() >= size && rhs->GetSize() >= size) ?
			SMemory::Memcmp(lhs->GetData(), rhs->GetData(), sizeof(ElementT) * size) == 0 : false;
	}

	FORCEINLINE static bool CompareElementsPrivate(const ElementT* lhs, const ElementT* rhs)
	{
		// Compare bytes instead of using == operator (that might not be provided)
		return SMemory::Memcmp(lhs, rhs, sizeof(ElementT)) == 0;
	}

	AllocatorT _allocator = {};
	SizeType _num = INDEX_NONE;
};

template<typename ElementT, typename AllocatorT>
struct TContainerTypeTraits<TArray<ElementT, AllocatorT>> : public TContainerTypeTraits<void>
{
	using ElementType = ElementT;
	using AllocatorType = AllocatorT;

	enum
	{
		IsContainer = true,
		IsDynamic = true,
		InlineMemory = TIsSame<AllocatorT, TArrayAllocator<ElementT>>::Value
	};
};
