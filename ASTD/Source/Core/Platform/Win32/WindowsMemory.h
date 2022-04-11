
#pragma once

#include "Core/Platform/Base/BaseMemory.h"

// TODO(jan.kristian.fisera): Virtual memory allocations
// * see: https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
struct SWindowsPlatformMemory : public SBasePlatformMemory
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
	FORCEINLINE static double GetAllocatedBytes() { return (double)AllocatedBytes; }
	FORCEINLINE static double GetAllocatedKilobytes() { return (double)AllocatedBytes * KB_PER_BYTE; }
	FORCEINLINE static double GetAllocatedMegabytes() { return (double)AllocatedBytes * MB_PER_BYTE; }
	FORCEINLINE static double GetAllocatedGigabytes() { return (double)AllocatedBytes * GB_PER_BYTE; }

private:

	static uint64 AllocatedBytes;
};

uint64 SWindowsPlatformMemory::AllocatedBytes = 0;