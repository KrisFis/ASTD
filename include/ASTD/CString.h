// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Build.h"
#include PLATFORM_HEADER(CString)

#include "ASTD/Memory.h"

typedef PLATFORM_PREFIXED_TYPE(S, PlatformCString) SPlatformCString;

struct SCString : public SPlatformCString
{
	static constexpr uint16 SMALL_BUFFER_SIZE = 1024;
	static constexpr uint16 LARGE_BUFFER_SIZE = 4096;
	static constexpr uint16 MAX_BUFFER_SIZE_INT32 = 33;
	static constexpr uint16 MAX_BUFFER_SIZE_INT64 = 65;
	static constexpr uint16 MAX_BUFFER_SIZE_DOUBLE = 309+40; // _CVTBUFSIZE

	// Checks whether the passed character is wide character
	template<typename CharType>
	FORCEINLINE static constexpr bool IsWideChar(CharType)
	{
		return TIsSame<CharType, wchar>::Value;
	}

	// Gets length of a string
	template<typename CharType>
	static uint32 GetLength(const CharType* str)
	{
		const CharType* current = str;
		while(*current != CHAR_TERM)
			++current;

		return static_cast<uint32>(current - str);
	}

	// When:
	// * Lhs bigger -> -1
	// * Rhs bigger -> 1
	// * Equals -> 0
	template<typename CharType>
	static int32 CompareLenght(const CharType* lhs, const CharType* rhs)
	{
		while(*lhs != CHAR_TERM)
		{
			if(*rhs == CHAR_TERM)
				return -1;

			++lhs;
			++rhs;
		}

		return *rhs == CHAR_TERM ? 0 : 1;
	}

	// When:
	// * Value bigger -> -1
	// * TestLen bigger -> 1
	// * Equals -> 0
	template<typename CharType>
	static int32 CompareLenght(const CharType* val, uint32 testLen)
	{
		while(*val != CHAR_TERM)
		{
			if(testLen == 0)
				return -1;

			++val;
			--testLen;
		}

		return testLen == 0 ? 0 : 1;
	}

	// Compares contents of two strings
	template<typename CharType>
	static int32 Compare(const CharType* lhs, const CharType* rhs)
	{
		while(*lhs != CHAR_TERM)
		{
			if(*lhs != *rhs)
				break;

			++lhs;
			++rhs;
		}

		return *lhs - *rhs;
	}

	template<typename CharType>
	static CharType* ToUpper(CharType* str)
	{
		CharType* current = str;
		while(*current != CHAR_TERM)
		{
			*current = ToUpperChar(*current);
			++current;
		}

		return str;
	}

	template<typename CharType>
	static CharType* ToLower(CharType* str)
	{
		CharType* current = str;
		while(*current != CHAR_TERM)
		{
			*current = ToLowerChar(*current);
			++current;
		}

		return str;
	}

	// Copies contents of one string to the other
	// * String memory should not overlap
	template<typename CharType>
	static CharType* Copy(CharType* dest, const CharType* src)
	{
		SMemory::Copy(
			dest,
			src,
			sizeof(CharType) * GetLength(src)
		);

		return dest;
	}

	// Copies contents of one string to the other
	// * String memory can overlap
	template<typename CharType>
	static CharType* Move(CharType* dest, const CharType* src)
	{
		SMemory::Move(
			dest,
			src,
			sizeof(CharType) * GetLength(src)
		);

		return dest;
	}

	// Finds the first occurrence of the substring "subStr" in the string "str"
	template<typename CharType>
	static const CharType* FindSubstring(const CharType* str, const CharType* subStr)
	{
		const uint32 subLen = GetLength(subStr);
		if(CompareLenght(str, subLen) <= 0)
		{
			while (*(str + subLen) != CHAR_TERM)
			{
				if (SMemory::Compare(str, subStr, subLen) == 0)
					return str;

				++str;
			}
		}

		return nullptr;
	}

	template<typename CharType>
	static const CharType* FindSubstringReversed(const CharType* str, const CharType* subStr)
	{
		const uint32 mainLen = GetLength(str);
		const uint32 subLen = GetLength(subStr);

		if(mainLen >= subLen)
		{
			const CharType* current = str + (mainLen - subLen);
			while(current >= str)
			{
				if (SMemory::Compare(str, subStr, subLen) == 0)
					return current;

				--current;
			}
		}

		return nullptr;
	}
};
