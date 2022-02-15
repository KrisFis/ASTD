
#pragma once

#include "Memory/MemoryUtilities.h"

// Main allocator used by TArray
// * Has data inlined
class CArrayAllocator
{

public: // Constructor

	FORCEINLINE CArrayAllocator() : Data(nullptr), Size(0) {}

public: // Destructor

	FORCEINLINE ~CArrayAllocator() { Release(); }

public: // Operators

	FORCEINLINE bool operator==(const CArrayAllocator& Other) { return Data == Other.Data; }
	FORCEINLINE bool operator!=(const CArrayAllocator& Other) { return !operator==(Other); }

public: // Getters

	// Gets allocated data
	FORCEINLINE void* GetData() const { return Data; }

	// Gets allocated data size
	FORCEINLINE uint64 GetSize() const { return Size; }

public: // Manipulation

	// Allocates new elements
	// @param - element size
	// @param - how many of elements should be allocated
	// @return - array of new elements
	void* Allocate(uint32 ElementSize, uint32 Num)
	{
		void* newData = SMemory::Allocate(Size + (ElementSize * Num));
		if(Data)
		{
			SMemory::Move(newData, Data, Size);
			SMemory::Deallocate(Data, Size);
		}

		void* elementPtr = (uint8*)newData + Size;

		Data = newData;
		Size += ElementSize * Num;

		return elementPtr;
	}

	// Releases resources
	void Release()
	{	
		if(Data)
		{
			SMemory::Deallocate(Data, Size);

			Data = nullptr;
			Size = 0;
		}
	}

	FORCEINLINE void Replace(void* InData, uint64 InSize) { Data = InData; Size = InSize; }

private: // Fields

	void* Data;
	uint64 Size;
};