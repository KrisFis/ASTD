
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

public: // Operators

	FORCEINLINE bool operator==(const TArrayAllocator& Other) { return Data == Other.Data; }
	FORCEINLINE bool operator!=(const TArrayAllocator& Other) { return !operator==(Other); }

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
		static constexpr TSize ELEMENT_SIZE = sizeof(ElementType);

		ElementType* newData = (ElementType*)SMemory::Allocate(ELEMENT_SIZE * (Count + Num));
		if(Data)
		{
			SMemory::Copy(newData, Data, ELEMENT_SIZE * Num);
			SMemory::Deallocate(Data);
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
			SMemory::Deallocate(Data);

			Data = nullptr;
			Count = 0;
		}
	}

private: // Fields

	ElementType* Data;
	SizeType Count;
};