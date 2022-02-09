
#pragma once

#include "Platform/PlatformTypes.h"

struct SBasePlatformMath
{
	template<typename T>
	FORCEINLINE static T Min(T ValueA, T ValueB) { return ValueA > ValueB ? ValueB : ValueA; }

	template<typename T>
	FORCEINLINE static T Max(T ValueA, T ValueB) { return ValueA > ValueB ? ValueA : ValueB; }

	template<typename T>
	FORCEINLINE static T Clamp(T Value, T Min, T Max) { return Value > Max ? Max : (Min > Value ? Min : Value); }

	template<typename T>
	FORCEINLINE static T Abs(T Value) { return Value >= (T)0 ? Value : -Value; }

	template <typename T>
	FORCEINLINE static bool IsPowerOfTwo(T Value) { return ((Value & (Value - 1)) == (T)0); }
};