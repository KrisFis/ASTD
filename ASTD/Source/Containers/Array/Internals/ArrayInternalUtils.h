
#pragma once

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeTraits.h"
#include "TypeTraits/TypeMethods.h"
#include "Memory/MemoryUtilities.h"

namespace NArrayInternalUtils
{
	// zero num is not supported!
	template<typename ElementType, typename AllocatorType>
	void AllocatorCopyData(AllocatorType Allocator, void* Data, uint32 Num)
	{
		static constexpr TSize elementSize = SizeOf<ElementType>();

		Allocator.Release();
		Allocator.Allocate(
			elementSize,
			Num
		);

		SMemory::Copy(
			Allocator.GetData(),
			Data,
			elementSize * Num
		);
	}

	// zero num is all elements
	template<typename ElementType, typename AllocatorType>
	FORCEINLINE void AllocatorCopyData(AllocatorType Destination, AllocatorType Source, uint32 Num = 0)
	{
		AllocatorCopyData<ElementType>(
			Destination,
			Source.GetData(),
			(Num == 0) ? Source.GetCount() : SMath::Min(Source.GetCount(), Num)
		);
	}

	template<typename AllocatorType>
	void AllocatorReplace(AllocatorType Allocator, void* Data, uint32 Num)
	{
		Allocator.Release();
		Allocator.SetData(Data);
		Allocator.SetCount(Num);
	}

	// Source can be either lvalue or xvalue
	template<typename DestAllocatorType, typename SrcAllocatorType>
	void AllocatorReplace(DestAllocatorType Destination, SrcAllocatorType Source)
	{
		AllocatorReplace(Destination, Source.GetData(), Source.GetCount());
		
		Source.SetData(nullptr);
		Source.SetCount(0);
	}
}