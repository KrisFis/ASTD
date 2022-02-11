
#pragma once

#include "Memory/Allocator/AllocatorBase.h"
#include "Memory/MemoryUtilities.h"

// Very primitive allocator, that allocates memory
// * Does a lot of fragmentation
// * Does NOT hold reference to any memory block
template<typename T>
class TPrimitiveAllocator : public TAllocatorBase<T>
{

public: // Typedefs

	typedef typename TAllocatorBase<T>::ElementType ElementType;

public: // TAllocatorBase overrides

	FORCEINLINE virtual ElementType* Allocate(uint32 Num) override
	{
		// NOTE(jan.kristian.fisera): Unmanaged memory raise in allocations
		return SMemory::AllocateTyped<ElementType>(Num);
	}

	FORCEINLINE virtual void Deallocate(ElementType* Ptr, uint32 Num) override
	{
		SMemory::DeallocateTyped(Ptr, Num);
	}

	FORCEINLINE virtual void Construct(ElementType* Ptr, const ElementType& Value) override
	{
		::new((void*)Ptr) ElementType(Value);
	}

	FORCEINLINE virtual void Destruct(ElementType* Ptr) override
	{
		Ptr->~ElementType();
	}
};