
#pragma once

#include "Type/TypeUtilities.h"
#include "Math/MathUtilities.h"

#include "Containers/Array/Array.h"

// NOTE(jan.kristian.fisera):
// * Methods to add: Split, StartsWith, EndsWith, Find, ShrinkToFit, Contains, ToUpper, ToLower, Compare
struct SString
{

public: // Types

	typedef tchar CharType;
	typedef TArray<CharType> DataType;
	typedef typename DataType::SizeType SizeType;

public: // Constructors

	FORCEINLINE SString() {}
	FORCEINLINE SString(const SString& Other) { FillToEmptyImpl(Other); }
	FORCEINLINE SString(SString&& Other) { FillToEmptyImpl(Move(Other)); }
	FORCEINLINE SString(const CharType* Text) { FillToEmptyImpl(Text); }

public: // Compare operators

	FORCEINLINE bool operator==(const SString& Other) const { return Data == Other.Data; }
	FORCEINLINE bool operator!=(const SString& Other) const { return !operator==(Other); }

public: // Assign operators

	FORCEINLINE SString& operator=(const SString& Other) { EmptyImpl(); FillToEmptyImpl(Other); return *this; }
	FORCEINLINE SString& operator=(SString&& Other) { EmptyImpl(); FillToEmptyImpl(Move(Other)); return *this; }

	FORCEINLINE SString& operator+=(const SString& Other) { AppendImpl(Other); return *this; }
	FORCEINLINE SString& operator+=(SString&& Other) { AppendImpl(Move(Other)); return *this; }

public: // Arithmetic operators

	FORCEINLINE SString operator+(const SString& Other) { SString tmpStr(*this); return tmpStr.Append_GetRef(Other); }
	FORCEINLINE SString operator+(SString&& Other) { SString tmpStr(*this); return tmpStr.Append_GetRef(Move(Other)); }

public: // Get operators

	FORCEINLINE tchar operator[](SizeType Index) const { return Data[Index]; }

public: // Property getters

	FORCEINLINE const DataType& GetData() const { return Data; }
	FORCEINLINE const CharType* GetChars() const { return GetCharsFromData(Data); }

	// Omits EOF
	FORCEINLINE SizeType GetLength() const { return SMath::Max<SizeType>(Data.GetCount() - 1, 0); }
	FORCEINLINE bool IsEmpty() const { return Data.GetCount() > 1; }

public: // Append

	FORCEINLINE void Append(const SString& Other) { AppendImpl(Other); }
	FORCEINLINE void Append(SString&& Other) { AppendImpl(Move(Other)); }

	FORCEINLINE SString& Append_GetRef(const SString& Other) { AppendImpl(Other); return *this; }
	FORCEINLINE SString& Append_GetRef(SString&& Other) { AppendImpl(Move(Other)); return *this; }

public: // Other

	FORCEINLINE void Empty() { EmptyImpl(); }
	FORCEINLINE void Reset() { EmptyImpl(); }

private: // Helpers -> Manipulation

	FORCEINLINE void AppendImpl(const SString& Other) { Data.Append(Other.Data); }
	FORCEINLINE void AppendImpl(SString&& Other) { Data.Append(Other.Data); Other.Empty(); }

private: // Helpers -> Cross manipulation

	FORCEINLINE void FillToEmptyImpl(const CharType* Text) { Data = GetDataFromChars(Text); }
	FORCEINLINE void FillToEmptyImpl(const SString& Other) { Data = Other.Data; }
	FORCEINLINE void FillToEmptyImpl(SString&& Other) { Data = Other.Data; Other.Reset(); }

private: // Helpers -> Other

	FORCEINLINE void EmptyImpl() { Data.Empty(); }

private: // Helpers -> Utils

	static DataType GetDataFromChars(const CharType* Chars)
	{
		if(Chars)
		{
			uint32 lenght = 1; // take EOF into account

			const CharType* currentChar = Chars;
			while(*currentChar != NULL_CHAR)
			{
				++lenght;
				++currentChar;
			}

			return DataType(Chars, lenght);
		}
		else
		{
			return DataType();
		}
	}

	static const CharType* GetCharsFromData(const DataType& InData) 
	{ 
		return InData.GetData();
	}

private: // Fields

	DataType Data;

};