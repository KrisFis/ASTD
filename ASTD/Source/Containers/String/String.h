
#pragma once

#include "Type/TypeUtilities.h"
#include "String/CStringUtilities.h"

#include "Containers/Array/Array.h"

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

	FORCEINLINE SizeType GetLength() const { return Data.GetCount() > 1 ? Data.GetCount() - 1 : 0; }

	FORCEINLINE bool IsValidIndex(SizeType Index) const { return Index >= 0 && Index < GetLastCharIndex(); }
	FORCEINLINE bool IsEmpty() const { return Data.GetCount() > 1; }

public: // Iterations

	FORCEINLINE StringIteratorType begin() { return Data.GetCount() > 1 ? &Data[0] : nullptr; }
	FORCEINLINE ConstStringIteratorType begin() const { return Data.GetCount() > 1 ? &Data[0] : nullptr; }

	FORCEINLINE StringIteratorType end() { return Data.GetCount() > 1 ? &Data[GetLastCharIndex()] : nullptr; }
	FORCEINLINE ConstStringIteratorType end() const { return Data.GetCount() > 1 ? &Data[GetLastCharIndex()] : nullptr; }

public: // Compares

	FORCEINLINE int32 Compare(const SString& Other) const { return CompareImpl(*this, Other); }
	bool EqualsTo(const SString& Other, bool CaseSensitive = true) const
	{
		if(Data.GetCount() == Other.Data.GetCount())
		{
			if(CaseSensitive)
				return CompareImpl(*this, Other);
			else
				return CompareImpl(RecaseString(*this, false), RecaseString(Other, false));
		}

		return false;
	}

public: // Checks [string]

	FORCEINLINE bool StartsWith(const SString& Value, bool CaseSensitive = true) const
	{
		return IsAtIndexImpl(0, Value, CaseSensitive);
	}

	FORCEINLINE bool EndsWith(const SString& Value, bool CaseSensitive = true) const
	{
		return IsAtIndexImpl(GetLastCharIndex() - Value.GetLastCharIndex(), Value, CaseSensitive);
	}

	FORCEINLINE bool Contains(const SString& Value, bool CaseSensitive = true, bool FromStart = true) const
	{
		return Find(Value, CaseSensitive, FromStart) != INDEX_NONE;
	}

	FORCEINLINE SizeType Find(const SString& Value, bool CaseSensitive = true, bool FromStart = true) const
	{
		return FromStart ? FindFromStartImpl(Value, CaseSensitive) : FindFromEndImpl(Value, CaseSensitive);
	}

public: // Checks [char]

	FORCEINLINE bool StartsWith(CharType Value, bool CaseSensitive = true) const
	{
		return IsAtIndexImpl(0, Value, CaseSensitive);
	}

	bool EndsWith(CharType Value, bool CaseSensitive = true) const
	{
		return IsAtIndexImpl(GetLastCharIndex(), Value, CaseSensitive);
	}

	FORCEINLINE bool Contains(CharType Value, bool CaseSensitive = true, bool FromStart = true) const
	{
		return Find(Value, CaseSensitive, FromStart) != INDEX_NONE;
	}

	FORCEINLINE SizeType Find(CharType Value, bool CaseSensitive = true, bool FromStart = true) const
	{
		return FromStart ? FindFromStartImpl(Value, CaseSensitive) : FindFromEndImpl(Value, CaseSensitive);
	}

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
		for(SizeType i = 0; i < GetLastCharIndex(); ++i)
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
		for(SizeType i = 0; i < GetLastCharIndex(); ++i)
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

	// bool Split(const SString& Other, SString* OutLeft, SString* OutRight, bool CaseSensitive = true) const;

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

	FORCEINLINE SizeType GetLastCharIndex() const { return Data.GetCount() - 2; }

	bool IsAtIndexImpl(SizeType Index, const SString& Value, bool CaseSensitive) const
	{
		if(Index < 0 || GetLength() < Index + Value.GetLength())
			return false;

		for(SizeType i = Index; i < Index + Value.GetLastCharIndex(); ++i)
		{
			CharType lhs = RecaseChar(Data[i], CaseSensitive);
			CharType rhs = RecaseChar(Value.Data[i], CaseSensitive);

			if(lhs != rhs)
			{
				return false;
			}
		}

		return true;
	}

	bool IsAtIndexImpl(SizeType Index, CharType Value, bool CaseSensitive) const
	{
		return (Index >= 0) ? RecaseChar(Data[Index], CaseSensitive) == RecaseChar(Value, CaseSensitive) : false;
	}

	SizeType FindFromStartImpl(const SString& Value, bool CaseSensitive) const
	{
		if(Value.IsEmpty() || (GetLength() < Value.GetLength()))
			return INDEX_NONE;

		for(SizeType i = 0; i < GetLastCharIndex(); ++i)
		{
			bool wasFound = true;

			CharType curChar = RecaseChar(Data[i], CaseSensitive);

			for(SizeType y = 0; y < Value.GetLastCharIndex(); ++y)
			{
				CharType curOtherChar = RecaseChar(Value.Data[i], CaseSensitive);

				if(curChar == curOtherChar)
				{
					wasFound = false;
					break;
				}
			}

			if(wasFound)
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

	SizeType FindFromStartImpl(CharType Value, bool CaseSensitive) const
	{
		CharType curOtherChar = RecaseChar(Value, CaseSensitive);

		for(SizeType i = 0; i < GetLastCharIndex(); ++i)
		{
			CharType curChar = RecaseChar(Data[i], CaseSensitive);

			if(curChar == curOtherChar)
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

	SizeType FindFromEndImpl(const SString& Value, bool CaseSensitive) const
	{
		if(Value.IsEmpty() || (GetLength() < Value.GetLength()))
			return INDEX_NONE;

		for(SizeType i = GetLastCharIndex(); i >= 0; --i)
		{
			bool wasFound = true;

			CharType curChar = RecaseChar(Data[i], CaseSensitive);

			for(SizeType y = Value.GetLastCharIndex(); y >= 0; --y)
			{
				CharType curOtherChar = RecaseChar(Value.Data[i], CaseSensitive);

				if(curChar == curOtherChar)
				{
					wasFound = false;
					break;
				}
			}

			if(wasFound)
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

	SizeType FindFromEndImpl(CharType Value, bool CaseSensitive) const
	{
		CharType curOtherChar = RecaseChar(Value, CaseSensitive);

		for(SizeType i = GetLastCharIndex(); i >= 0; --i)
		{
			CharType curChar = RecaseChar(Data[i], CaseSensitive);

			if(curChar == curOtherChar)
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

	FORCEINLINE static int32 CompareImpl(const SString& Lhs, const SString& Rhs) { return SCString::Compare(Lhs.Data.GetData(), Rhs.Data.GetData()); }

	static CharType RecaseChar(CharType Value, bool IsSensitive) { return !IsSensitive ? SCString::ToLowerChar(Value) : Value; }
	static SString RecaseString(const SString& Value, bool IsSensitive) { return !IsSensitive ? Value.ToLower() : Value; }

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