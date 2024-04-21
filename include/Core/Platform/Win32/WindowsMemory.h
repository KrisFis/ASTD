// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <cstdlib>
#include "Core/Platform/Base/BaseMemory.h"

// TODO(jan.kristian.fisera): Virtual memory allocations
// * see: https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
struct SWindowsPlatformMemory : public SBasePlatformMemory
{
	// Allocates new memory
	FORCEINLINE static void* Allocate(int64 size) 
	{ 
		_allocatedBytes += size;
		return malloc(size);
	}

	// Allocates new memory and sets every bit to zero
	FORCEINLINE static void* AllocateZeroed(int64 size) 
	{ 
		_allocatedBytes += size;
		return calloc(size, sizeof(uint8));
	}

	// Deallocates memory
	FORCEINLINE static void Deallocate(void* Ptr, int64 size)
	{ 
		_allocatedBytes -= size;
		return free(Ptr); 
	}

	// Gets allocated memory as specific type
	FORCEINLINE static double GetAllocatedBytes() { return (double)_allocatedBytes; }
	FORCEINLINE static double GetAllocatedKilobytes() { return (double)_allocatedBytes * KB_PER_BYTE; }
	FORCEINLINE static double GetAllocatedMegabytes() { return (double)_allocatedBytes * MB_PER_BYTE; }
	FORCEINLINE static double GetAllocatedGigabytes() { return (double)_allocatedBytes * GB_PER_BYTE; }

private:

	static uint64 _allocatedBytes;
};

uint64 SWindowsPlatformMemory::_allocatedBytes = 0;
