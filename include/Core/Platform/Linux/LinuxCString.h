// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <cstdio>
#include <wchar.h>

#include "Core/Platform/Base/BaseCString.h"

struct SLinuxPlatformCString : public SBasePlatformCString
{
	// Converts the string pointed to, by the argument str to an integer
	FORCEINLINE static int32 ToInt32(const char* val) { return atoi(val); }
	FORCEINLINE static int32 ToInt32(const wchar* val) { return wcstol(val, nullptr, 10); }

	// Converts integer value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromInt32(int32 val, char* buf) { sprintf(buf, "%d", val); return buf; }
	FORCEINLINE static wchar* FromInt32(int32 val, wchar* buf) { swprintf(buf, MAX_BUFFER_SIZE_INT32, L"%d", val); return buf; }

	// Converts the string pointed to, by the argument str to a long integer
	FORCEINLINE static int64 ToInt64(const char* val) { return atol(val); }
	FORCEINLINE static int64 ToInt64(const wchar* val) { return wcstol(val, nullptr, 10); }

	// Converts integer value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromInt64(int64 val, char* buf) { sprintf(buf, "%ld", val); return buf; }
	FORCEINLINE static wchar* FromInt64(int64 val, wchar* buf) { swprintf(buf, MAX_BUFFER_SIZE_INT64, L"%ld", val); return buf; }

	// Converts the string pointed to, by the argument str to a floating-point number
	FORCEINLINE static double ToDouble(const char* val) { return atof(val); }
	FORCEINLINE static double ToDouble(const wchar* val) { return wcstod(val, nullptr); }

	// Converts double value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromDouble(double val, int32 Digits, char* buf) { sprintf(buf, "%.*f", Digits, val); return buf; }
	FORCEINLINE static wchar* FromDouble(double val, int32 Digits, wchar* buf) { swprintf(buf, MAX_BUFFER_SIZE_DOUBLE, L"%.*f", Digits, val); return buf; }
};
