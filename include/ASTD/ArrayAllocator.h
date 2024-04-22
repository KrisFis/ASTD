// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"

#include "ASTD/Memory.h"

// Main allocator used by TArray
// * Has data inlined
template<typename InElementType>
class TArrayAllocator
{
public:

	// Types
	/////////////////////////////////

	typedef InElementType ElementType;
	typedef int64 SizeType;

	// Constructor
	/////////////////////////////////

	FORCEINLINE TArrayAllocator() = default;

	// Destructor
	/////////////////////////////////

	FORCEINLINE ~TArrayAllocator() { Release(); }

	// Getters
	/////////////////////////////////

	// Gets/Sets allocated data
	FORCEINLINE ElementType* GetData() const { return _data; }
	FORCEINLINE void SetData(ElementType* data) { _data = data; }

	// Gets/Sets allocated count
	FORCEINLINE SizeType GetCount() const { return _count; }
	FORCEINLINE void SetCount(SizeType count) { _count = count; }

	// Manipulation
	/////////////////////////////////

	// Allocates new elements
	// @param - element size
	// @param - how many of elements should be allocated
	// @return - array of new elements
	ElementType* Allocate(SizeType num)
	{
		ElementType* newData = SMemory::AllocateElement<ElementType>(_count + num);
		if(_data)
		{
			SMemory::Copy(newData, _data, sizeof(ElementType) * _count);
			SMemory::Deallocate(_data, sizeof(ElementType) * _count);
		}

		ElementType* elementPtr = newData + _count;

		_data = newData;
		_count += num;

		return elementPtr;
	}

	// Releases resources
	void Release()
	{
		if(_data)
		{
			SMemory::DeallocateElement(_data, _count);

			_data = nullptr;
			_count = 0;
		}
	}

private:

	ElementType* _data = nullptr;
	SizeType _count = 0;
};
