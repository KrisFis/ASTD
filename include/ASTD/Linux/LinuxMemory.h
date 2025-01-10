// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <cstdlib>
#include <memory.h>

#include "ASTD/Build/Linux/LinuxBuild.h"

// TODO(jan.kristian.fisera): Virtual memory allocations
// * see: https://linux.die.net/man/2/mmap
struct SLinuxPlatformMemory
{
	// Allocates new memory
	FORCEINLINE static void* Allocate(int64 size) { return malloc(size); }

	// Allocates new memory and sets every bit to zero
	FORCEINLINE static void* AllocateZeroed(int64 size)	{ return calloc(size, 1); }

	// Deallocates memory
	FORCEINLINE static void Deallocate(void* ptr, int64 size) { return free(ptr); }

	// Copies block of memory from destionation to source (does not handle overlapping)
	FORCEINLINE static void* Copy(void* dest, const void* src, int64 size) { return memcpy(dest, src, size); }

	// Copies block of memory from destionation to source (handles overlapping)
	FORCEINLINE static void* Move(void* dest, const void* src, int64 size) { return memmove(dest, src, size); }

	// Compares two blocks of memory
	FORCEINLINE static int32 Compare(const void* lhs, const void* rhs, int64 num) { return memcmp(lhs, rhs, num); }
};
