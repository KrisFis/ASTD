
#pragma once

#include "TypeTraits/TypeTraits.h"

namespace NContainerInternals
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
}