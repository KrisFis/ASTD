
#include <malloc.h>

#include "LinuxTypes.h"

// TODO(jan.kristian.fisera): Virtual memory allocations
// * see: https://linux.die.net/man/2/mmap
namespace NMemory
{
	template<typename ElementType = void>
	ElementType* Alloc(uint32 NumOfElements)
	{
		return (ElementType*)malloc(sizeof(ElementType)*NumOfElements);
	}

	template<typename ElementType = void>
	ElementType* AllocZeroed(uint32 NumOfElements)
	{
		return (ElementType*)calloc(NumOfElements, sizeof(ElementType));
	}

	void Dealoc(void* Pointer)
	{
		return free(Pointer);
	}
}

// Template specializations
////////////////////////////////////////////////////////////////////

template<>
void* NMemory::Alloc<void>(uint32 Size)
{
	return malloc(Size);
}

template<>
void* NMemory::AllocZeroed<void>(uint32 Size)
{
	return calloc(Size, sizeof(uint8));
}