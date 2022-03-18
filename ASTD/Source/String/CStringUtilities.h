
#pragma once

#include "Platform/PlatformCString.h"
#include "Memory/MemoryUtilities.h"

struct SCString : public SPlatformCString
{
	// Compares contents of two strings
	template<typename CharType>
	static int32 Compare(const CharType* Lhs, const CharType* Rhs)
	{
		while(*Lhs != CHAR_TERM)
		{
			if(*Lhs != *Rhs)
				break;

			++Lhs;
			++Rhs;
		}

		return *Lhs - *Rhs;
	}

	// Gets length of string
	template<typename CharType>
	static uint32 GetLength(const CharType* Value)
	{
		const CharType* current = Value;
		while(*current != CHAR_TERM)
			++current;

		return static_cast<uint32>(current - Value);
	}

	// Copies contents of one string to the other
	// * String memory should not overlap
	template<typename CharType>
	static CharType* Copy(CharType* Destination, const CharType* Source)
	{
		SMemory::Copy(
			Destination,
			Source,
			sizeof(CharType) * GetLength(Source)
		);

		return Destination;
	}

	// Copies contents of one string to the other
	// * String memory can overlap
	template<typename CharType>
	static CharType* Move(CharType* Destination, const CharType* Source)
	{
		SMemory::Move(
			Destination,
			Source,
			sizeof(CharType) * GetLength(Source)
		);

		return Destination;
	}

	// Finds the first occurrence of the substring "Sub" in the string "Main"
	template<typename CharType>
	static const CharType* FindSubstring(const CharType* Main, const CharType* Sub)
	{
		const uint32 subLen = GetLength(Sub);

		while (*Main != CHAR_TERM)
		{
			if (SMemory::Compare(Main, Sub, subLen) == 0)
				return Main;

			++Main;
		}

		return nullptr;
	}

	template<typename CharType>
	static const CharType* FindSubstringBackwards(const CharType* Main, const CharType* Sub)
	{
		const uint32 mainLen = GetLength(Main);
		const uint32 subLen = GetLength(Sub);
		const CharType* current = Main + (mainLen - subLen);
		while(current >= Main)
		{
			if (SMemory::Compare(Main, Sub, subLen) == 0)
				return current;

			--current;
		}

		return nullptr;
	}
};