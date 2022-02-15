
#pragma once

#include <malloc.h>
#include <memory.h>

#include "TypeTraits/TypeTraits.h"
#include "Platform/PlatformTypes.h"

struct SGenericPlatformMemory
{
	FORCEINLINE static void* Allocate(TSize Size) { return malloc(Size); }
	FORCEINLINE static void* AllocateZeroed(TSize Size) { return calloc(Size, sizeof(uint8)); }

	FORCEINLINE static void Deallocate(void* Ptr, TSize Size) { return free(Ptr); }

	FORCEINLINE static void* Copy(void* Destination, void* Source, TSize Size) { return memcpy(Destination, Source, Size); }
	FORCEINLINE static void* Move(void* Destination, void* Source, TSize Size) { return memmove(Destination, Source, Size); }
};