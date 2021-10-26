
#pragma once

#include "TypeTraitsCore.h"

namespace NTypeTraitsInternals
{
	template<typename T>
	struct TDecayHelper
	{
		typedef typename TRemoveConstVolatile<T>::Type Type;
	};
	
	template <typename T>
	struct TDecayHelper<T[]>
	{
		typedef T* Type;
	};
	
	template <typename T, uint32 N>
	struct TDecayHelper<T[N]>
	{
		typedef T* Type;
	};

	template <typename RetType, typename... Params>
	struct TDecayHelper<RetType(Params...)>
	{
		typedef RetType (*Type)(Params...);
	};
}