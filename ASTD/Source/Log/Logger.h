
#pragma once

#include <iostream>

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeTraits.h"

// Very primitive logger
struct SLogger
{
	FORCEINLINE static SLogger Begin() { return SLogger(); }
	FORCEINLINE static const char* End() { return "\n"; }

	template<typename T, typename TEnableIf<TIsIntegerType<T>::Value>::Type* = nullptr>
	FORCEINLINE const SLogger& operator<<(T Value) const
	{
		std::cout << (TSize)Value;
		return *this;
	}

	template<typename T, typename TEnableIf<!TIsIntegerType<T>::Value>::Type* = nullptr>
	FORCEINLINE const SLogger& operator<<(T Value) const
	{
		std::cout << Value;
		return *this;
	}

private: // Constructor

	SLogger() = default;
};