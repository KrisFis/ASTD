// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Platform/PlatformTypes.h"

#include <ctype.h>
#include <wctype.h>

struct SBasePlatformCString
{
	static constexpr uint8 MAX_BUFFER_SIZE_INT32 = 33;
	static constexpr uint8 MAX_BUFFER_SIZE_INT64 = 65;
	static constexpr uint16 MAX_BUFFER_SIZE_DOUBLE = 309+40; // _CVTBUFSIZE

	// Checks whether the passed character is wide character
	FORCEINLINE static bool IsWideChar(char Value) { return false; }
	FORCEINLINE static bool IsWideChar(wchar Value) { return true; }

	// Checks whether the passed character is alphabetic
	FORCEINLINE static bool IsAlphaChar(char Value) { return isalpha(Value); }
	FORCEINLINE static bool IsAlphaChar(wchar Value) { return iswalpha(Value); }

	// Checks whether the passed character is alphanumeric
	FORCEINLINE static bool IsAlphaNumericChar(char Value) { return isalnum(Value); }
	FORCEINLINE static bool IsAlphaNumericChar(wchar Value) { return iswalnum(Value); }

	// Checks whether the passed character is numeric
	FORCEINLINE static bool IsNumberChar(char Value) { return !isalpha(Value) && isalnum(Value); }
	FORCEINLINE static bool IsNumberChar(wchar Value) { return !iswalpha(Value) && iswalnum(Value); }

	// Checks whether the passed character is white-space.
	FORCEINLINE static bool IsWhitespaceChar(char Value) { return isspace(Value); }
	FORCEINLINE static bool IsWhitespaceChar(wchar Value) { return iswspace(Value); }

	// Checks whether the passed character is printable
	FORCEINLINE static bool IsPrintableChar(char Value) { return isprint(Value); }
	FORCEINLINE static bool IsPrintableChar(wchar Value) { return iswprint(Value); }

	// Checks whether the passed character is lowercase letter
	FORCEINLINE static bool IsLowerChar(char Value) { return islower(Value); }
	FORCEINLINE static bool IsLowerChar(wchar Value) { return iswlower(Value); }

	// Checks whether the passed character is an uppercase letter
	FORCEINLINE static bool IsUpperChar(char Value) { return isupper(Value); }
	FORCEINLINE static bool IsUpperChar(wchar Value) { return iswupper(Value); }

	// Converts uppercase letters to lowercase
	FORCEINLINE static char ToLowerChar(char Value) { return tolower(Value); }
	FORCEINLINE static wchar ToLowerChar(wchar Value) { return towlower(Value); }

	// Converts lowercase letters to uppercase
	FORCEINLINE static char ToUpperChar(char Value) { return toupper(Value); }
	FORCEINLINE static wchar ToUpperChar(wchar Value) { return towupper(Value); }
};