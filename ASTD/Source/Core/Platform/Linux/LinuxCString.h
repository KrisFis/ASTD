
#pragma once

#include "Core/Platform/Base/BaseCString.h"

#include <cstdlib>
#include <wchar.h>

struct SLinuxPlatformCString : public SBasePlatformCString
{
	// Converts the string pointed to, by the argument str to an integer
	FORCEINLINE static int32 ToInt32(const char* Value) { return atoi(Value); }
	FORCEINLINE static int32 ToInt32(const wchar* Value) { return wcstol(Value, nullptr, 10); }

	// Converts integer value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromInt32(int32 Value, char* Buffer) { sprintf(Buffer, "%d", Value); return Buffer; }
	FORCEINLINE static wchar* FromInt32(int32 Value, wchar* Buffer) { swprintf(Buffer, MAX_BUFFER_SIZE_INT32, L"%d", Value); return Buffer; }

	// Converts the string pointed to, by the argument str to a long integer
	FORCEINLINE static int64 ToInt64(const char* Value) { return atol(Value); }
	FORCEINLINE static int64 ToInt64(const wchar* Value) { return wcstol(Value, nullptr, 10); }

	// Converts integer value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromInt64(int64 Value, char* Buffer) { sprintf(Buffer, "%ld", Value); return Buffer; }
	FORCEINLINE static wchar* FromInt64(int64 Value, wchar* Buffer) { swprintf(Buffer, MAX_BUFFER_SIZE_INT64, L"%ld", Value); return Buffer; }

	// Converts the string pointed to, by the argument str to a floating-point number
	FORCEINLINE static double ToDouble(const char* Value) { return atof(Value); }
	FORCEINLINE static double ToDouble(const wchar* Value) { return wcstod(Value, nullptr); }

	// Converts double value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromDouble(double Value, int32 Digits, char* Buffer) { sprintf(Buffer, "%.*f", Digits, Value); return Buffer; }
	FORCEINLINE static wchar* FromDouble(double Value, int32 Digits, wchar* Buffer) { swprintf(Buffer, MAX_BUFFER_SIZE_DOUBLE, L"%.*f", Digits, Value); return Buffer; }
};