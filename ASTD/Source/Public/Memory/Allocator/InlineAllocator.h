
#pragma once

#include "Memory/MemoryUtilities.h"

template<typename T>
class TInlineAllocator
{

public: // Typedefs

	typedef T ElementType;

public: // Constructor

	FORCEINLINE TInlineAllocator() : Data(nullptr), Count(0) {}
	FORCEINLINE ~TInlineAllocator()
	{
		if(Data)
		{
			SMemory::DeallocateTyped(Data, Count);
			Data = nullptr;
		}
	}
	
public: // Getters

	FORCEINLINE ElementType* GetData() const { return Data; }
	FORCEINLINE uint32 GetCount() const { return Count; }

public: // Elements manipulation

	FORCEINLINE ElementType* Allocate(uint32 Num)
	{
		ElementType* newData = SMemory::AllocateTyped<ElementType>(Count + Num);
		if(Data)
		{
			SMemory::MemoryMoveTyped(newData, Data, Count);
			SMemory::DeallocateTyped(Data, Count);
		}

		Data = newData;
		Count += Num;

		return Data + Count;
	}

private: // Fields

	ElementType* Data;
	uint32 Count;
};