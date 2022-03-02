
#pragma once

#include <malloc.h>
#include <memory.h>

#include "Platform/PlatformTypes.h"

struct SGenericPlatformMemory
{
	typedef decltype(sizeof(0)) SizeType;

	FORCEINLINE static void* Allocate(SizeType Size) { return malloc(Size); }
	FORCEINLINE static void* AllocateZeroed(SizeType Size) { return calloc(Size, sizeof(uint8)); }

	FORCEINLINE static void Deallocate(void* Ptr) { return free(Ptr); }

	FORCEINLINE static void* Copy(void* Destination, const void* Source, SizeType Size) { return memcpy(Destination, Source, Size); }
	FORCEINLINE static void* Move(void* Destination, const void* Source, SizeType Size) { return memmove(Destination, Source, Size); }

	FORCEINLINE static SizeType Compare(const void* Lhs, const void* Rhs, SizeType Num) { return memcmp(Lhs, Rhs, Num); }
};