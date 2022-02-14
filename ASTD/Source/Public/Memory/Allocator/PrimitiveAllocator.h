
#pragma once

#include "Memory/MemoryUtilities.h"

// Very primitive allocator, that allocates memory
// * Does a lot of fragmentation
// * Does NOT hold reference to any memory block
template<typename T>
class TPrimitiveAllocator
{

public: // Typedefs

	typedef T ElementType;

public: // Allocations 

	FORCEINLINE ElementType* Allocate(uint32 Num)
	{
		return SMemory::AllocateTyped<ElementType>(Num);
	}

	FORCEINLINE void Deallocate(ElementType* Ptr, uint32 Num)
	{
		SMemory::DeallocateTyped(Ptr, Num);
	}
};