
#pragma once

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeTraits.h"
#include "TypeTraits/TypeMethods.h"
#include "Memory/MemoryUtilities.h"

namespace NArrayInternalUtils
{
	// zero num is not supported!
	template<typename AllocatorType>
	void AllocatorCopyData(AllocatorType Allocator, const typename AllocatorType::ElementType* Data, typename AllocatorType::SizeType Num)
	{
		Allocator.Release();
		Allocator.Allocate(Num);

		SMemory::Copy(
			Allocator.GetData(),
			Data,
			sizeof(AllocatorType::ElementType) * Num
		);
	}

	// zero num is all elements
	template<typename ElementType, typename AllocatorType>
	FORCEINLINE void AllocatorCopyData(AllocatorType Destination, AllocatorType Source)
	{
		AllocatorCopyData(
			Destination,
			Source.GetData(),
			Source.GetCount()
		);
	}

	template<typename AllocatorType>
	FORCEINLINE void AllocatorCopyData(AllocatorType Destination, AllocatorType Source, typename AllocatorType::SizeType Num)
	{
		AllocatorCopyData(
			Destination,
			Source.GetData(),
			SMath::Min(Source.GetCount(), Num)
		);
	}

	template<typename AllocatorType>
	void AllocatorReplace(AllocatorType Allocator, typename AllocatorType::ElementType* Data, typename AllocatorType::SizeType Num)
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