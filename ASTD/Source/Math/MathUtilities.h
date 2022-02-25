
#pragma once

#include <math.h>

// Definitions
///////////////////////////////////////////////////////////

#ifdef PI
	#undef PI
#endif

// PI = 3.1415926535897932384626433832795f
#define PI 3.1415926535897932f

#define TINY_NUMBER 1.e-8f
#define SMALL_NUMBER 1.e-4f

// Types
///////////////////////////////////////////////////////////

struct SMath
{
	template<typename T>
	FORCEINLINE static T Min(T ValueA, T ValueB) { return ValueA > ValueB ? ValueB : ValueA; }

	template<typename T>
	FORCEINLINE static T Max(T ValueA, T ValueB) { return ValueA > ValueB ? ValueA : ValueB; }

	template<typename T>
	FORCEINLINE static T Clamp(T Value, T Min, T Max) { return Value > Max ? Max : (Min > Value ? Min : Value); }

	template<typename T>
	FORCEINLINE static T Abs(T Value) { return Value >= (T)0 ? Value : -Value; }

	template<typename T>
	FORCEINLINE static T Floor(T Value) { return floor(Value); }

	template<typename T>
	FORCEINLINE static T Ceil(T Value) { return ceil(Value); }

	template <typename T>
	FORCEINLINE static bool IsPowerOfTwo(T Value) { return ((Value & (Value - 1)) == (T)0); }
};