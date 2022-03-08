
#pragma once

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeTraits.h"
#include "Containers/String/String.h"

namespace NLoggerTypeTraits
{
	template<typename T>
	struct TGetSupportedType
	{
		enum 
		{
			Int = TIsIntegerType<typename TDecay<T>::Type>::Value,
			String = TIsSame<T, SString>::Value,
			StringW = TIsSame<typename TRemovePointer<T>::Type, wchar>::Value,
			Other = !Int && !String && !StringW
		};
	};
}