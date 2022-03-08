
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

			IsTrivial = TIsTrivialType<ElementType>::Value,
			SupportCopy = TIsCopyConstructible<ElementType>::Value,
			SupportMove = TIsMoveConstructible<ElementType>::Value
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