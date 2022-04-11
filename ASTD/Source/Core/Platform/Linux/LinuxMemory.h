
#pragma once

#include "Core/Platform/Generic/GenericMemory.h"

// TODO(jan.kristian.fisera): Virtual memory allocations
// * see: https://linux.die.net/man/2/mmap
struct SLinuxPlatformMemory : public SGenericPlatformMemory
{
	// Allocates new memory
	FORCEINLINE static void* Allocate(int64 Size)
	{ 
		AllocatedBytes += Size; 
		return malloc(Size); 
	}

	// Allocates new memory and sets every bit to zero
	FORCEINLINE static void* AllocateZeroed(int64 Size)
	{ 
		AllocatedBytes += Size;
		return calloc(Size, sizeof(uint8));
	}

	// Deallocates memory
	FORCEINLINE static void Deallocate(void* Ptr, int64 Size)
	{ 
		AllocatedBytes -= Size;
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