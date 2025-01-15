// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <cstdlib>
#include <memory.h>

#include "ASTD/Apple/AppleBuild.h"

// TODO(jan.kristian.fisera): Virtual memory allocations
// * see: https://linux.die.net/man/2/mmap
struct SApplePlatformMemory
{
	// Allocates new memory
	FORCEINLINE static void* Malloc(int64 size) { return malloc(size); }

	// Allocates new memory and sets every bit to zero
	FORCEINLINE static void* Calloc(int64 size)	{ return calloc(size, 1); }

	// Deallocates memory
	FORCEINLINE static void Free(void* ptr, int64 size) { return free(ptr); }

	// Copies block of memory from destionation to source (does not handle overlapping)
	FORCEINLINE static void* Memcpy(void* dest, const void* src, int64 size) { return memcpy(dest, src, size); }

	// Copies block of memory from destionation to source (handles overlapping)
	FORCEINLINE static void* Memmove(void* dest, const void* src, int64 size) { return memmove(dest, src, size); }

	// Compares two blocks of memory
	FORCEINLINE static int32 Memcmp(const void* lhs, const void* rhs, int64 num) { return memcmp(lhs, rhs, num); }
};
