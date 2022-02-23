
#pragma once

#include "Types/Types.h"
#include "TypeTraits/TypeTraits.h"
#include "TypeTraits/TypeMethods.h"
#include "Memory/MemoryUtilities.h"

namespace NArrayInternalUtils
{
	template<typename ElementType, typename DestAllocatorType, typename SrcAllocatorType>
	void AllocatorCopyData(DestAllocatorType Destination, SrcAllocatorType Source, uint32 Num = 0)
	{
		static constexpr TSize elementSize = SizeOf<ElementType>();
		const uint32 copyNum = (Num == 0) ? Source.GetCount() : SMath::Min(Source.GetCount(), Num);

		Destination.Release();
		Destination.Allocate(
			elementSize, 
			copyNum
		);

		SMemory::Copy(
			Destination.GetData(),
			Source.GetData(),
			elementSize * copyNum
		);
	}

	template<typename DestAllocatorType, typename SrcAllocatorType>
	void AllocatorReplace(DestAllocatorType Destination, SrcAllocatorType Source)
	{
		Destination.Release();
		Destination.SetData(Source.GetData());
		Destination.SetCount(Source.GetCount());

		Source.SetData(nullptr);
		Source.SetCount(0);
	}
}