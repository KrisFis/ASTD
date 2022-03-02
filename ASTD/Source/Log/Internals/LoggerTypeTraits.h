
#pragma once

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeTraits.h"

namespace NLoggerTypeTraits
{
	template<typename T>
	struct TGetSupportedType
	{
		enum 
		{
			Int = TIsIntegerType<typename TDecay<T>::Type>::Value,
			StringW = TIsSame<typename TRemovePointer<T>::Type, wchar>::Value,
			Other = !Int && !StringW
		};
	};
}