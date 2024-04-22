// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <ctype.h>
#include <wctype.h>

struct SBasePlatformCString
{
	static constexpr uint8 MAX_BUFFER_SIZE_INT32 = 33;
	static constexpr uint8 MAX_BUFFER_SIZE_INT64 = 65;
	static constexpr uint16 MAX_BUFFER_SIZE_DOUBLE = 309+40; // _CVTBUFSIZE

	// Checks whether the passed character is wide character
	FORCEINLINE static bool IsWideChar(char) { return false; }
	FORCEINLINE static bool IsWideChar(wchar) { return true; }

	// Checks whether the passed character is alphabetic
	FORCEINLINE static bool IsAlphaChar(char val) { return isalpha(val); }
	FORCEINLINE static bool IsAlphaChar(wchar val) { return iswalpha(val); }

	// Checks whether the passed character is alphanumeric
	FORCEINLINE static bool IsAlphaNumericChar(char val) { return isalnum(val); }
	FORCEINLINE static bool IsAlphaNumericChar(wchar val) { return iswalnum(val); }

	// Checks whether the passed character is numeric
	FORCEINLINE static bool IsNumberChar(char val) { return !isalpha(val) && isalnum(val); }
	FORCEINLINE static bool IsNumberChar(wchar val) { return !iswalpha(val) && iswalnum(val); }

	// Checks whether the passed character is white-space.
	FORCEINLINE static bool IsWhitespaceChar(char val) { return isspace(val); }
	FORCEINLINE static bool IsWhitespaceChar(wchar val) { return iswspace(val); }

	// Checks whether the passed character is printable
	FORCEINLINE static bool IsPrintableChar(char val) { return isprint(val); }
	FORCEINLINE static bool IsPrintableChar(wchar val) { return iswprint(val); }

	// Checks whether the passed character is lowercase letter
	FORCEINLINE static bool IsLowerChar(char val) { return islower(val); }
	FORCEINLINE static bool IsLowerChar(wchar val) { return iswlower(val); }

	// Checks whether the passed character is an uppercase letter
	FORCEINLINE static bool IsUpperChar(char val) { return isupper(val); }
	FORCEINLINE static bool IsUpperChar(wchar val) { return iswupper(val); }

	// Converts uppercase letters to lowercase
	FORCEINLINE static char ToLowerChar(char val) { return tolower(val); }
	FORCEINLINE static wchar ToLowerChar(wchar val) { return towlower(val); }

	// Converts lowercase letters to uppercase
	FORCEINLINE static char ToUpperChar(char val) { return toupper(val); }
	FORCEINLINE static wchar ToUpperChar(wchar val) { return towupper(val); }
};
