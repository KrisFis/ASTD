
#pragma once

#include <malloc.h>
#include <memory.h>

#include "Platform/PlatformTypes.h"

struct SGenericPlatformMemory
{
	// Allocates new memory
	FORCEINLINE static void* Allocate(uint64 Size) { return malloc(Size); }

	// Allocates new memory and sets every bit to zero
	FORCEINLINE static void* AllocateZeroed(uint64 Size) { return calloc(Size, sizeof(uint8)); }

	// Deallocates memory
	FORCEINLINE static void Deallocate(void* Ptr) { return free(Ptr); }

	// Copies block of memory from destionation to source (does not handle overlapping)
	FORCEINLINE static void* Copy(void* Destination, const void* Source, uint64 Size) { return memcpy(Destination, Source, Size); }

	// Copies block of memory from destionation to source (handles overlapping)
	FORCEINLINE static void* Move(void* Destination, const void* Source, uint64 Size) { return memmove(Destination, Source, Size); }

	// Compares two blocks of memory
	FORCEINLINE static int32 Compare(const void* Lhs, const void* Rhs, uint64 Num) { return memcmp(Lhs, Rhs, Num); }
};