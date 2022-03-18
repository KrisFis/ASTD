
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

	FORCEINLINE SString() { FillToEmptyImpl(); }
	FORCEINLINE SString(const SString& Other) { FillToEmptyImpl(Other); }
	FORCEINLINE SString(SString&& Other) { FillToEmptyImpl(Move(Other)); }
	FORCEINLINE SString(const CharType* Text) { FillToEmptyImpl(Text); }
	FORCEINLINE SString(CharType Character) { FillToEmptyImpl(Character); }

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

	FORCEINLINE const CharType* operator*() const { return Data.GetData(); }
	FORCEINLINE CharType operator[](SizeType Index) const { return Data[Index]; }

public: // Property getters

	FORCEINLINE const DataType& GetData() const { return Data; }

	FORCEINLINE const CharType* GetChars() const { return Data.GetData(); }
	FORCEINLINE CharType* GetChars() { return Data.GetData(); }

	FORCEINLINE SizeType GetLength() const { return Data.GetCount() > 1 ? Data.GetCount() - 1 : 0; }

	FORCEINLINE bool IsValidIndex(SizeType Index) const { return Index >= 0 && Index < GetLastCharIndex(); }
	FORCEINLINE bool IsEmpty() const { return GetLength() == 0; }

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
				return CompareImpl(ToLower(), Other.ToLower());
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
		return FindImpl(Value, CaseSensitive, FromStart) != INDEX_NONE;
	}

	FORCEINLINE SizeType Find(const SString& Value, bool CaseSensitive = true, bool FromStart = true) const
	{
		return FindImpl(Value, CaseSensitive, FromStart);
	}

	bool Split(const SString& Value, SString* OutLeft, SString* OutRight, bool CaseSensitive = true, bool FromStart = true) const
	{
		SizeType foundIdx = FindImpl(Value, CaseSensitive, FromStart);
		if(foundIdx == INDEX_NONE)
			return false;

		if(OutLeft)
		{
			*OutLeft = ChopRight(foundIdx);
		}

		if(OutRight)
		{
			*OutRight = ChopLeft(foundIdx + Value.GetLastCharIndex());
		}

		return true;
	}

	TArray<SString> SplitToArray(const SString& Delimiter, bool CaseSensitive = true, bool DiscardEmpty = true) const
	{
		TArray<SString> result;

		const CharType* init = Data.GetData();
		const SizeType delimiterLen = Delimiter.GetLength();
		while(const CharType* current = SCString::FindSubstring(init, Delimiter.Data.GetData()))
		{
			if (!DiscardEmpty || current-init)
			{
				SString& newStr = result.AddUnitialized_GetRef();
				newStr.Data = DataType(init, static_cast<SizeType>(current - init));
				newStr.Data.Add((CharType)CHAR_TERM);
			}

			init = current + Delimiter.GetLength();
		}

		if (!DiscardEmpty || *init != CHAR_TERM)
		{
			SString& newStr = result.AddUnitialized_GetRef();
			newStr.Data = DataType(init, Data.GetCount() - static_cast<SizeType>(init - Data.GetData()) + 1);
		}

		return result;
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
		return FindImpl(Value, CaseSensitive, FromStart) != INDEX_NONE;
	}

	FORCEINLINE SizeType Find(CharType Value, bool CaseSensitive = true, bool FromStart = true) const
	{
		return FindImpl(Value, CaseSensitive, FromStart);
	}

	bool Split(CharType Value, SString* OutLeft, SString* OutRight, bool CaseSensitive = true, bool FromStart = true) const
	{
		SizeType foundIdx = FindImpl(Value, CaseSensitive, FromStart);
		if(foundIdx == INDEX_NONE)
			return false;

		if(OutLeft)
		{
			*OutLeft = ChopRight(foundIdx);
		}

		if(OutRight)
		{
			*OutRight = ChopLeft(foundIdx + 1);
		}

		return true;
	}

public: // Append

	FORCEINLINE void Append(const SString& Other) { AppendImpl(Other); }
	FORCEINLINE void Append(SString&& Other) { AppendImpl(Move(Other)); }
	FORCEINLINE void Append(CharType Other) { AppendImpl(Other); }

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

	SString ChopRange(SizeType FirstIndex, SizeType SecondIndex) const
	{
		SString newString(*this);
		newString.ChopRangeInline(FirstIndex, SecondIndex);
		return newString;
	}

	void ChopRangeInline(SizeType FirstIndex, SizeType SecondIndex)
	{
		if( !IsValidIndex(FirstIndex) || !IsValidIndex(SecondIndex))
		{
			return;
		}

		SizeType* biggerVal, *smallerVal;
		if(FirstIndex > SecondIndex)
		{
			biggerVal = &FirstIndex;
			smallerVal = &SecondIndex;
		}
		else
		{
			biggerVal = &SecondIndex;
			smallerVal = &FirstIndex;
		}

		ChopRightInline(*biggerVal);
		ChopLeftInline(*smallerVal);
	}

public: // Reset

	FORCEINLINE void Empty() { EmptyImpl(); }
	FORCEINLINE void Reset() { EmptyImpl(); }

public: // Other

	FORCEINLINE void ShrinkToFit() { Data.ShrinkToFit(); }

private: // Helper methods

	FORCEINLINE void AppendImpl(const SString& Other) { AppendImpl(Other.Data, true); }
	FORCEINLINE void AppendImpl(SString&& Other) { AppendImpl(Other.Data, true); Other.Empty(); }

	void AppendImpl(const DataType& InData, bool IncludesTerminationCharacter)
	{
		Data.RemoveAt(Data.GetCount() - 1); // Remove termination character
		Data.Append(InData.GetData(), InData.GetCount());

		if(!IncludesTerminationCharacter)
		{
			Data.Add((CharType)CHAR_TERM);
		}
	}
	
	void AppendImpl(CharType Other) 
	{
		Data.Add(Other);

		// Add termination character to end
		Data.Swap(Data.GetCount() - 1, Data.GetCount() - 2);
	}

	void FillToEmptyImpl() { Data.Add((CharType)CHAR_TERM); }
	void FillToEmptyImpl(CharType Character) { Data.Append({Character, (CharType)CHAR_TERM});}
	void FillToEmptyImpl(const CharType* Text) { Data = GetDataFromChars(Text); }
	void FillToEmptyImpl(const SString& Other) { Data = Other.Data; }
	void FillToEmptyImpl(SString&& Other) { Data = Other.Data; Other.Reset(); }

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

	FORCEINLINE SizeType FindImpl(const SString& Value, bool CaseSensitive, bool FromStart) const
	{
		return CaseSensitive ? FindSubstringImpl(*this, Value, !FromStart) : FindSubstringImpl(ToLower(), Value.ToLower(), !FromStart);
	}

	SizeType FindImpl(CharType Value, bool CaseSensitive, bool FromStart) const
	{
		return CaseSensitive ? FindSubstringImpl(*this, SString(Value), !FromStart) : FindSubstringImpl(ToLower(), SString(RecaseChar(Value, true)), !FromStart);
	}

	FORCEINLINE static int32 CompareImpl(const SString& Lhs, const SString& Rhs) { return SCString::Compare(Lhs.Data.GetData(), Rhs.Data.GetData()); }
	static SizeType FindSubstringImpl(const SString& Main, const SString& Sub, bool Backwards) 
	{ 
		const CharType* foundPtr = 
			Backwards ? 
				SCString::FindSubstringBackwards(Main.Data.GetData(), Sub.Data.GetData()) : 
				SCString::FindSubstring(Main.Data.GetData(), Sub.Data.GetData());

		if(foundPtr)
		{
			return static_cast<SizeType>(foundPtr - Main.Data.GetData());
		}
		else
		{
			return INDEX_NONE;
		}
	}

	static CharType RecaseChar(CharType Value, bool IsSensitive) { return !IsSensitive ? SCString::ToLowerChar(Value) : Value; }
	static DataType GetDataFromChars(const CharType* Chars)
	{
		if(Chars)
			return DataType(Chars, SCString::GetLength(Chars) + 1);
		else
			return DataType({(CharType)CHAR_TERM});
	}

private: // Fields

	DataType Data;

};