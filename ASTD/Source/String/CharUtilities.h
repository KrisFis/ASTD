
#pragma once

#include "Type/TypeUtilities.h"

#include <ctype.h>
#include <wctype.h>

namespace NCharUtilities
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
}