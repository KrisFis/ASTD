
#pragma once

#include "Memory/MemoryUtilities.h"

// Main allocator used by TArray
// * Has data inlined
template<typename InElementType>
class TArrayAllocator
{

public: // Types

	typedef InElementType ElementType;
	typedef int64 SizeType;

public: // Constructor

	FORCEINLINE TArrayAllocator() : Data(nullptr), Count(0) {}

public: // Destructor

	FORCEINLINE ~TArrayAllocator() { Release(); }

public: // Getters

	// Gets/Sets allocated data
	FORCEINLINE ElementType* GetData() const { return Data; }
	FORCEINLINE void SetData(ElementType* InData) { Data = InData; }

	// Gets/Sets allocated count
	FORCEINLINE SizeType GetCount() const { return Count; }
	FORCEINLINE void SetCount(SizeType InCount) { Count = InCount; }

public: // Manipulation

	// Allocates new elements
	// @param - element size
	// @param - how many of elements should be allocated
	// @return - array of new elements
	ElementType* Allocate(SizeType Num)
	{
		ElementType* newData = (ElementType*)SMemory::Allocate(sizeof(ElementType) * (Count + Num));
		if(Data)
		{
			SMemory::Copy(newData, Data, sizeof(ElementType) * Count);
			SMemory::Deallocate(Data, sizeof(ElementType) * Count);
		}

		ElementType* elementPtr = newData + Count;

		Data = newData;
		Count += Num;

		return elementPtr;
	}

	// Releases resources
	void Release()
	{	
		if(Data)
		{
			SMemory::Deallocate(Data, sizeof(ElementType) * Count);

			Data = nullptr;
			Count = 0;
		}
	}

private: // Fields

	ElementType* Data;
	SizeType Count;
};