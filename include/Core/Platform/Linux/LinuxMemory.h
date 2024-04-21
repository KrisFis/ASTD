// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <cstdlib>
#include "Core/Platform/Base/BaseMemory.h"

// TODO(jan.kristian.fisera): Virtual memory allocations
// * see: https://linux.die.net/man/2/mmap
struct SLinuxPlatformMemory : public SBasePlatformMemory
{
	// Allocates new memory
	FORCEINLINE static void* Allocate(int64 size)
	{ 
		AllocatedBytes += size; 
		return malloc(size); 
	}

	// Allocates new memory and sets every bit to zero
	FORCEINLINE static void* AllocateZeroed(int64 size)
	{ 
		AllocatedBytes += size;
		return calloc(size, sizeof(uint8));
	}

	// Deallocates memory
	FORCEINLINE static void Deallocate(void* Ptr, int64 size)
	{ 
		AllocatedBytes -= size;
		return free(Ptr); 
	}

	// Gets allocated memory as specific type
	FORCEINLINE static double GetAllocatedBytes() { return AllocatedBytes; }
	FORCEINLINE static double GetAllocatedKilobytes() { return AllocatedBytes * KB_PER_BYTE; }
	FORCEINLINE static double GetAllocatedMegabytes() { return AllocatedBytes * MB_PER_BYTE; }
	FORCEINLINE static double GetAllocatedGigabytes() { return AllocatedBytes * GB_PER_BYTE; }

private:

	static uint64 AllocatedBytes;
};

uint64 SLinuxPlatformMemory::AllocatedBytes = 0;
