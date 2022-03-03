
#pragma once

#include "TypeTraits/TypeTraits.h"
#include <type_traits>

namespace NArrayTypeTraits
{
	template<typename ElementType>
	struct TElementInfo
	{
		enum
		{
			IsValid = !TIsSame<ElementType, void>::Value && !TIsReference<ElementType>::Value,

			IsTrivial = TIsPointer<ElementType>::Value || TIsPrimitiveType<ElementType>::Value,
			SupportCopy = std::is_copy_constructible<ElementType>::value,
			SupportMove = std::is_move_constructible<ElementType>::value
		};
	};

	template<typename AllocatorType>
	struct TAllocatorInfo
	{
		typedef typename AllocatorType::SizeType SizeType;

		enum
		{
			IsValid = !TIsSame<AllocatorType, void>::Value && TIsSignedType<SizeType>::Value
		};
	};
}