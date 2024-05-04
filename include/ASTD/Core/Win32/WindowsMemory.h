// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Win32/WindowsBuild.h"

// TODO(jan.kristian.fisera): Virtual memory allocations
// * see: https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
struct SWindowsPlatformMemory
{
	// Allocates new memory
	FORCEINLINE static void* Allocate(int64 size) { return HeapAlloc(GetProcessHeap(), 0, size); }

	// Allocates new memory and sets every bit to zero
	FORCEINLINE static void* AllocateZeroed(int64 size) { return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size); }

	// Deallocates memory
	FORCEINLINE static void Deallocate(void* ptr, int64 size) { HeapFree(GetProcessHeap(), 0, ptr); }

	// Copies block of memory from destionation to source (does not handle overlapping)
	FORCEINLINE static void* Copy(void* dest, const void* src, int64 size) { return CopyMemory(dest, src, size); }

	// Copies block of memory from destionation to source (handles overlapping)
	FORCEINLINE static void* Move(void* dest, const void* src, int64 size) { return MoveMemory(dest, src, size); }

	// Compares two blocks of memory
	FORCEINLINE static int32 Compare(const void* lhs, const void* rhs, int64 num) { return RtlEqualMemory(lhs, rhs, num); }
};
