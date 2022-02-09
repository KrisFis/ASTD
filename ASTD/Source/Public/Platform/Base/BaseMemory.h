
#pragma once

#include <malloc.h>

#include "Platform/PlatformTypes.h"

struct SBasePlatformMemory
{
	template<typename T = void>
	FORCEINLINE static T* Alloc(uint32 NumOfElements) { return (T*)malloc(sizeof(T)*NumOfElements); }

	template<typename T = void>
	FORCEINLINE static T* AllocZeroed(uint32 NumOfElements) { return (T*)calloc(NumOfElements, sizeof(T));	}
	
	template<typename T = void>
	FORCEINLINE static void Dealoc(T* Pointer) { return free(Pointer); }
};

// Template specializations
////////////////////////////////////////////////////////////////////

template<>
FORCEINLINE void* SBasePlatformMemory::Alloc<void>(uint32 Size)
{
	return malloc(Size);
}

template<>
FORCEINLINE void* SBasePlatformMemory::AllocZeroed<void>(uint32 Size)
{
	return calloc(Size, sizeof(uint8));
}