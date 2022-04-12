
#pragma once

#include "Core/Types.h"
#include "Core/Type/TypeTraits.h"
#include "Containers/String/String.h"

namespace NLogInternals
{
	template<typename T>
	struct TGetSupportedType
	{
		enum 
		{
			Int = TIsIntegerType<T>::Value,
			String = TIsSame<T, SString>::Value,
			StringW = TIsSame<typename TRemovePointer<T>::Type, wchar>::Value,
			Other = !Int && !String && !StringW
		};
	};
}