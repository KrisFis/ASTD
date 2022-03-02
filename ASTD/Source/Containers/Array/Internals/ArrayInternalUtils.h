
#pragma once

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeTraits.h"
#include "TypeTraits/TypeMethods.h"
#include "Memory/MemoryUtilities.h"

namespace NArrayInternalUtils
{
	// zero num is not supported!
	template<typename AllocatorType>
	void AllocatorCopyData(AllocatorType& Allocator, const typename AllocatorType::ElementType* Data, typename AllocatorType::SizeType Num)
	{
		Allocator.Allocate(Num);

		SMemory::Copy(
			Allocator.GetData(),
			Data,
			sizeof(typename AllocatorType::ElementType) * Num
		);
	}

	// zero num is all elements
	template<typename AllocatorType>
	FORCEINLINE void AllocatorCopyData(AllocatorType& Destination, const AllocatorType& Source)
	{
		AllocatorCopyData(
			Destination,
			Source.GetData(),
			Source.GetCount()
		);
	}

	template<typename AllocatorType>
	FORCEINLINE void AllocatorCopyData(AllocatorType& Destination, const AllocatorType& Source, typename AllocatorType::SizeType Num)
	{
		AllocatorCopyData(
			Destination,
			Source.GetData(),
			SMath::Min(Source.GetCount(), Num)
		);
	}

	template<typename AllocatorType>
	void AllocatorMoveData(AllocatorType& Allocator, typename AllocatorType::ElementType* Data, typename AllocatorType::SizeType Num)
	{
		Allocator.SetData(Data);
		Allocator.SetCount(Num);
	}

	// Source can be either lvalue or xvalue
	template<typename DestAllocatorType, typename SrcAllocatorType>
	void AllocatorMoveData(DestAllocatorType& Destination, SrcAllocatorType&& Source)
	{
		AllocatorMoveData(Destination, Source.GetData(), Source.GetCount());
		
		Source.SetData(nullptr);
		Source.SetCount(0);
	}
}