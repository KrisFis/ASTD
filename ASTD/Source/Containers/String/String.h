
#pragma once

#include "Type/TypeUtilities.h"
#include "Math/MathUtilities.h"
#include "String/CStringUtilities.h"

#include "Containers/Array/Array.h"

enum class EStringSearchCase : uint8
{
	Sensitive,
	Ignore
};

struct SString
{

public: // Types

	typedef tchar CharType;
	typedef TArray<CharType> DataType;
	typedef typename DataType::SizeType SizeType;

	typedef CharType* StringIteratorType;
	typedef const CharType* ConstStringIteratorType;

public: // Constructors

	FORCEINLINE SString() {}
	FORCEINLINE SString(const SString& Other) { FillToEmptyImpl(Other); }
	FORCEINLINE SString(SString&& Other) { FillToEmptyImpl(Move(Other)); }
	FORCEINLINE SString(const CharType* Text) { FillToEmptyImpl(Text); }

public: // Static fields

	static SString GetEmpty()
	{
		static SString emptyString = SString();
		return emptyString;
	}

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
	FORCEINLINE const CharType* GetChars() const { return Data.GetData(); }

	FORCEINLINE SizeType GetLength() const { return SMath::Max<SizeType>(Data.GetCount() - 1, 0); }

	FORCEINLINE bool IsValidIndex(SizeType Index) const { return Index >= 0 && (Data.GetCount() - 1) > Index; }
	FORCEINLINE bool IsEmpty() const { return Data.GetCount() > 1; }

public: // Iterations

	FORCEINLINE StringIteratorType begin() { return !IsEmpty() ? &Data[0] : nullptr; }
	FORCEINLINE ConstStringIteratorType begin() const { return !IsEmpty() ? &Data[0] : nullptr; }

	FORCEINLINE StringIteratorType end() { return !IsEmpty() ? &Data[Data.GetLastIndex()-1] : nullptr; }
	FORCEINLINE ConstStringIteratorType end() const { return !IsEmpty() ? &Data[Data.GetLastIndex()-1] : nullptr; }

public: // Compares

	FORCEINLINE int32 Compare(const SString& Other) const { return CompareImpl(*this, Other); }
	bool EqualsTo(const SString& Other, EStringSearchCase SearchCase = EStringSearchCase::Sensitive) const
	{
		if(Data.GetCount() != Other.Data.GetCount())
		{
			return false;
		}

		int32 result = 0;
		if(SearchCase == EStringSearchCase::Sensitive)
		{
			result = CompareImpl(*this, Other);
		}
		else
		{
			result = CompareImpl(ToLower(), Other.ToLower());
		}

		return (result == 0);
	}

public: // Checks

	// bool Contains(const SString& Other, EStringSearchCase SearchCase = EStringSearchCase::Sensitive) const;
	// bool Contains(CharType Other, EStringSearchCase SearchCase = EStringSearchCase::Sensitive) const;

	// bool StartsWith(const SString& Other, EStringSearchCase SearchCase = EStringSearchCase::Sensitive) const;
	// bool StartsWith(CharType Other, EStringSearchCase SearchCase = EStringSearchCase::Sensitive) const;

	// bool EndsWith(const SString& Other, EStringSearchCase SearchCase = EStringSearchCase::Sensitive) const;
	// bool EndsWith(CharType Other, EStringSearchCase SearchCase = EStringSearchCase::Sensitive) const;

	// SizeType Find(const SString& Other, EStringSearchCase SearchCase = EStringSearchCase::Sensitive) const;
	// SizeType Find(CharType Other, EStringSearchCase SearchCase = EStringSearchCase::Sensitive) const;

public: // Append

	FORCEINLINE void Append(const SString& Other) { AppendImpl(Other); }
	FORCEINLINE void Append(SString&& Other) { AppendImpl(Move(Other)); }

	FORCEINLINE SString& Append_GetRef(const SString& Other) { AppendImpl(Other); return *this; }
	FORCEINLINE SString& Append_GetRef(SString&& Other) { AppendImpl(Move(Other)); return *this; }

public: // Manipulation

	SString ToUpper() const
	{
		SString newString(*this);
		newString.ToUpperInline();
		return newString;
	}

	void ToUpperInline()
	{
		for(SizeType i = 0; i < Data.GetCount() - 1; ++i)
		{
			Data[i] = SCString::ToUpperChar(Data[i]);
		}
	}

	SString ToLower() const
	{
		SString newString(*this);
		newString.ToUpperInline();
		return newString;
	}

	void ToLowerInline()
	{
		for(SizeType i = 0; i < Data.GetCount() - 1; ++i)
		{
			Data[i] = SCString::ToLowerChar(Data[i]);
		}
	}

	SString ChopRight(SizeType Index) const
	{
		SString newString(*this);
		newString.ChopRightInline(Index);
		return newString;
	}

	void ChopRightInline(SizeType Index)
	{
		if(!IsValidIndex(Index))
		{
			return;
		}

		Data.Shrink(Index + 1);
		Data[Index] = CHAR_TERM;
	}

	SString ChopLeft(SizeType Index) const
	{
		SString newString(*this);
		newString.ChopLeftInline(Index);
		return newString;
	}

	void ChopLeftInline(SizeType Index)
	{
		if(!IsValidIndex(Index))
		{
			return;
		}

		DataType newData(Data.GetCount() - Index);
		for(SizeType i = 0; i < newData.GetCount(); ++i)
		{
			newData[i] = Data[Index + i];
		}

		Data = newData;
	}

	// bool Split(const SString& Other, SString* OutLeft, SString* OutRight, EStringSearchCase SearchCase = EStringSearchCase::Sensitive) const;

public: // Reset

	FORCEINLINE void Empty() { EmptyImpl(); }
	FORCEINLINE void Reset() { EmptyImpl(); }

public: // Other

	void ShrinkToFit();

private: // Helper methods

	FORCEINLINE void AppendImpl(const SString& Other) { Data.Append(Other.Data); }
	FORCEINLINE void AppendImpl(SString&& Other) { Data.Append(Other.Data); Other.Empty(); }

	FORCEINLINE void FillToEmptyImpl(const CharType* Text) { Data = GetDataFromChars(Text); }
	FORCEINLINE void FillToEmptyImpl(const SString& Other) { Data = Other.Data; }
	FORCEINLINE void FillToEmptyImpl(SString&& Other) { Data = Other.Data; Other.Reset(); }

	FORCEINLINE void EmptyImpl() { Data.Empty(); }

	FORCEINLINE static int32 CompareImpl(const SString& Lhs, const SString& Rhs) { return SCString::Compare(Lhs.Data.GetData(), Rhs.Data.GetData()); }

	static DataType GetDataFromChars(const CharType* Chars)
	{
		if(Chars)
		{
			uint32 lenght = 1; // take EOF into account

			const CharType* currentChar = Chars;
			while(*currentChar != CHAR_TERM)
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

private: // Fields

	DataType Data;

};