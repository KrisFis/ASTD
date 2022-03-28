
#pragma once

#include "TypeTraits/Internals/TypeTraitsCore.h"

namespace NTypeTraitsInternals
{
	template<typename T>
	struct TDecayHelper
	{
		typedef T Type;
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

	template <typename T, bool IsSmall>
	struct TCallTraitsHelper
	{
		typedef const T& Type;
		typedef const T& ConstType;
	};
	template <typename T>
	struct TCallTraitsHelper<T, true>
	{
		typedef const T Type;
		typedef const T ConstType;
	};
	template <typename T>
	struct TCallTraitsHelper<T*, true>
	{
		typedef T* Type;
		typedef const T* ConstType;
	};

}