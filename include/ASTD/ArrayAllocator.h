// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Build.h"

#include "ASTD/Memory.h"

// Main allocator used by TArray
// * Has data inlined
template<typename ElementT>
class TArrayAllocator
{
public:

	// Types
	/////////////////////////////////

	typedef ElementT ElementType;
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
	FORCEINLINE SizeType GetSize() const { return _size; }
	FORCEINLINE void SetSize(SizeType count) { _size = count; }

	// Manipulation
	/////////////////////////////////

	// Allocates new elements
	// @param - element size
	// @param - how many of elements should be allocated
	// @return - array of new elements
	ElementType* Allocate(SizeType num)
	{
		ElementType* newData = SMemory::AllocateElement<ElementType>(_size + num);
		if(_data)
		{
			SMemory::Copy(newData, _data, sizeof(ElementType) * _size);
			SMemory::Deallocate(_data, sizeof(ElementType) * _size);
		}

		ElementType* elementPtr = newData + _size;

		_data = newData;
		_size += num;

		return elementPtr;
	}

	// Releases resources
	void Release()
	{
		if(_data)
		{
			SMemory::DeallocateElement(_data, _size);

			_data = nullptr;
			_size = 0;
		}
	}

private:

	ElementType* _data = nullptr;
	SizeType _size = 0;
};
