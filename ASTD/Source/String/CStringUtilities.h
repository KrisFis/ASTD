
#pragma once

#include "Platform/PlatformCString.h"
#include "Memory/MemoryUtilities.h"

struct SCString : public SPlatformCString
{
	// Gets length of string
	template<typename CharType>
	static uint32 GetLength(const CharType* Value)
	{
		CharType* current = Value;
		while(*current != CHAR_TERM)
			++current;

		return (current - Value);
	}

	// Copies contents of one string to the other
	// * String memory should not overlap
	template<typename CharType>
	static CharType* Copy(CharType* Destination, const CharType* Source)
	{
		SMemory::Copy(
			Destination,
			Source,
			sizeof(CharType) * Strlen(Source)
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
			sizeof(CharType) * Strlen(Source)
		);

		return Destination;
	}

	// Compares contents of two strings
	template<typename CharType>
	static int32 Compare(const CharType* Lhs, const CharType* Rhs)
	{
		for(uint32 i = 0;;++i)
		{
			const int32 result = Lhs[i] - Rhs[i];

			if(result != 0) return result;
			else if (Lhs[i] == CHAR_TERM) return 0;
		}
	}
};