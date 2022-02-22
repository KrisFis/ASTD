
#pragma once

#include "Memory/MemoryUtilities.h"

// Main allocator used by TArray
// * Has data inlined
class CArrayAllocator
{

public: // Constructor

	FORCEINLINE CArrayAllocator() : Data(nullptr), Count(0) {}

public: // Destructor

	FORCEINLINE ~CArrayAllocator() { Release(); }

public: // Operators

	FORCEINLINE bool operator==(const CArrayAllocator& Other) { return Data == Other.Data; }
	FORCEINLINE bool operator!=(const CArrayAllocator& Other) { return !operator==(Other); }

public: // Getters

	// Gets/Sets allocated data
	FORCEINLINE void* GetData() const { return Data; }
	FORCEINLINE void SetData(void* InData) { Data = InData; }

	// Gets/Sets allocated count
	FORCEINLINE uint64 GetCount() const { return Count; }
	FORCEINLINE void SetCount(uint64 InCount) { Count = InCount; }

public: // Manipulation

	// Allocates new elements
	// @param - element size
	// @param - how many of elements should be allocated
	// @return - array of new elements
	void* Allocate(uint32 ElementSize, uint32 Num)
	{
		void* newData = SMemory::Allocate(ElementSize * (Count + Num));
		if(Data)
		{
			SMemory::Move(newData, Data, ElementSize * Num);
			SMemory::Deallocate(Data);
		}

		void* elementPtr = (uint8*)newData + (ElementSize * Num);

		Data = newData;
		Count += Num;

		return elementPtr;
	}

	// Releases resources
	void Release()
	{	
		if(Data)
		{
			SMemory::Deallocate(Data);

			Data = nullptr;
			Count = 0;
		}
	}

private: // Fields

	void* Data;
	uint32 Count;
};