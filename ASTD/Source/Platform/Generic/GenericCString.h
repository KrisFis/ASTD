
#pragma once

#include "Platform/PlatformTypes.h"

#include <ctype.h>
#include <cstdlib>

#include <wctype.h>
#include <wchar.h>

struct SGenericPlatformCString
{
	// Checks whether the passed character is wide character
	FORCEINLINE static bool IsWide(char Value) { return false; }
	FORCEINLINE static bool IsWide(wchar Value) { return true; }

	// Checks whether the passed character is alphabetic
	FORCEINLINE static bool IsAlpha(char Value) { return isalpha(Value); }
	FORCEINLINE static bool IsAlpha(wchar Value) { return iswalpha(Value); }

	// Checks whether the passed character is alphanumeric
	FORCEINLINE static bool IsAlphaNumeric(char Value) { return isalnum(Value); }
	FORCEINLINE static bool IsAlphaNumeric(wchar Value) { return iswalnum(Value); }

	// Checks whether the passed character is numeric
	FORCEINLINE static bool IsNumber(char Value) { return !isalpha(Value) && isalnum(Value); }
	FORCEINLINE static bool IsNumber(wchar Value) { return !iswalpha(Value) && iswalnum(Value); }

	// Checks whether the passed character is white-space.
	FORCEINLINE static bool IsWhitespace(char Value) { return isspace(Value); }
	FORCEINLINE static bool IsWhitespace(wchar Value) { return iswspace(Value); }

	// Checks whether the passed character is printable
	FORCEINLINE static bool IsPrintable(char Value) { return isprint(Value); }
	FORCEINLINE static bool IsPrintable(wchar Value) { return iswprint(Value); }

	// Checks whether the passed character is lowercase letter
	FORCEINLINE static bool IsLower(char Value) { return islower(Value); }
	FORCEINLINE static bool IsLower(wchar Value) { return iswlower(Value); }

	// Checks whether the passed character is an uppercase letter
	FORCEINLINE static bool IsUpper(char Value) { return isupper(Value); }
	FORCEINLINE static bool IsUpper(wchar Value) { return iswupper(Value); }

	// Converts uppercase letters to lowercase
	FORCEINLINE static char ToLower(char Value) { return tolower(Value); }
	FORCEINLINE static wchar ToLower(wchar Value) { return towlower(Value); }

	// Converts lowercase letters to uppercase
	FORCEINLINE static char ToUpper(char Value) { return toupper(Value); }
	FORCEINLINE static wchar ToUpper(wchar Value) { return towupper(Value); }

	// Converts the string pointed to, by the argument str to a floating-point number
	FORCEINLINE static double ToDouble(const char* Value) { return atof(Value); }
	FORCEINLINE static double ToDouble(const char* Value, char** Next) { return strtod(Value, Next); }

	FORCEINLINE static double ToDouble(const wchar* Value) { return wcstod(Value, nullptr); }
	FORCEINLINE static double ToDouble(const wchar* Value, wchar** Next) { return wcstod(Value, Next); }

	// Converts the string pointed to, by the argument str to an integer
	FORCEINLINE static int32 ToInt(const char* Value) { return atoi(Value); }
	FORCEINLINE static int32 ToInt(const char* Value, char** Next, int32 Base) { return (int32)strtol(Value, Next, Base); }

	FORCEINLINE static int32 ToInt(const wchar* Value) { return wcstol(Value, nullptr, 10); }
	FORCEINLINE static int32 ToInt(const wchar* Value, wchar** Next, int32 Base) { return (int32)wcstol(Value, Next, Base); }

	// Converts the string pointed to, by the argument str to a long integer
	FORCEINLINE static int64 ToLong(const char* Value) { return atol(Value); }
	FORCEINLINE static int64 ToLong(const char* Value, char** Next, int32 Base) { return strtol(Value, Next, Base); }

	FORCEINLINE static int64 ToLong(const wchar* Value) { return wcstol(Value, nullptr, 10); }
	FORCEINLINE static int64 ToLong(const wchar* Value, wchar** Next, int32 Base) { return wcstol(Value, Next, Base); }

	// Converts the string pointed to, by the argument str to an unsigned long integer
	FORCEINLINE static uint64 ToULong(const char* Value) { return strtoul(Value, nullptr, 10); }
	FORCEINLINE static uint64 ToULong(const char* Value, char** Next, int32 Base) { return strtoul(Value, Next, Base); }

	FORCEINLINE static uint64 ToULong(const wchar* Value) { return wcstoul(Value, nullptr, 10); }
	FORCEINLINE static uint64 ToULong(const wchar* Value, wchar** Next, int32 Base) { return wcstoul(Value, Next, Base); }
};