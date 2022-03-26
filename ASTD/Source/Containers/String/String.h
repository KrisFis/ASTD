
#pragma once

#include "Type/TypeUtilities.h"
#include "String/CStringUtilities.h"
#include "Validation/Validation.h"

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
	FORCEINLINE CharType* operator*() { return Data.GetData(); }

	FORCEINLINE CharType operator[](SizeType Index) const { return Data[Index]; }

public: // Property getters

	FORCEINLINE const DataType& GetData() const { return Data; }

	FORCEINLINE const CharType* GetChars() const { return Data.GetData(); }
	FORCEINLINE CharType* GetChars() { return Data.GetData(); }

	FORCEINLINE SizeType GetLength() const { return Data.GetCount() > 1 ? Data.GetCount() - 1 : 0; }

	FORCEINLINE bool IsValidIndex(SizeType Index) const { return Index >= 0 && Index < GetLastCharIndex(); }
	FORCEINLINE bool IsEmpty() const { return GetLength() == 0; }

public: // Conversions

	FORCEINLINE int32 ToInt32() const { return SCString::ToInt32(Data.GetData()); }
	FORCEINLINE int64 ToInt64() const { return SCString::ToInt64(Data.GetData()); }
	FORCEINLINE double ToDouble() const { return SCString::ToDouble(Data.GetData()); }

	static SString FromInt32(int32 Value) 
	{ 
		static CharType buffer[SCString::MAX_BUFFER_SIZE_INT32];
		return SString(SCString::FromInt32(Value, buffer)); 
	}

	static SString FromInt64(int64 Value)
	{
		static CharType buffer[SCString::MAX_BUFFER_SIZE_INT64];
		return SString(SCString::FromInt64(Value, buffer));
	}

	static SString FromDouble(double Value, uint8 Digits)
	{
		static CharType buffer[SCString::MAX_BUFFER_SIZE_DOUBLE];
		return SString(SCString::FromDouble(Value, Digits, buffer));
	}

public: // Iterations

	FORCEINLINE StringIteratorType begin() { return Data.GetCount() > 1 ? &Data[0] : nullptr; }
	FORCEINLINE ConstStringIteratorType begin() const { return Data.GetCount() > 1 ? &Data[0] : nullptr; }

	FORCEINLINE StringIteratorType end() { return Data.GetCount() > 1 ? &Data[GetLastCharIndex()] : nullptr; }
	FORCEINLINE ConstStringIteratorType end() const { return Data.GetCount() > 1 ? &Data[GetLastCharIndex()] : nullptr; }

public: // Compares

	FORCEINLINE int32 Compare(const SString& Other, bool CaseSensitive = true) const { return ComparePrivate(*this, Other, CaseSensitive); }
	FORCEINLINE bool EqualsTo(const SString& Other, bool CaseSensitive = true) const { return ComparePrivate(*this, Other, CaseSensitive) == 0; }

public: // Checks

	FORCEINLINE bool IsWhitespace() const
	{
		return ContainsOnlyWhitespacesPrivate(*this);
	}

	FORCEINLINE bool StartsWith(const SString& Value, bool CaseSensitive = true) const
	{
		return IsAtIndexPrivate(*this, Value, 0, CaseSensitive);
	}

	FORCEINLINE bool EndsWith(const SString& Value, bool CaseSensitive = true) const
	{
		return IsAtIndexPrivate(*this, Value, GetLastCharIndex() - Value.GetLastCharIndex(), CaseSensitive);
	}

	FORCEINLINE bool Contains(const SString& Value, bool CaseSensitive = true, bool FromStart = true) const
	{
		return FindOccurencePrivate(*this, Value, CaseSensitive, FromStart) != INDEX_NONE;
	}

	FORCEINLINE SizeType Find(const SString& Value, bool CaseSensitive = true, bool FromStart = true) const
	{
		return FindOccurencePrivate(*this, Value, CaseSensitive, FromStart);
	}

public: // Append

	FORCEINLINE void Append(const SString& Other) { AppendImpl(Other); }
	FORCEINLINE void Append(SString&& Other) { AppendImpl(Move(Other)); }
	FORCEINLINE void Append(CharType Other) { AppendImpl(Other); }

	FORCEINLINE SString& Append_GetRef(const SString& Other) { AppendImpl(Other); return *this; }
	FORCEINLINE SString& Append_GetRef(SString&& Other) { AppendImpl(Move(Other)); return *this; }

public: // Const manipulation

	bool Split(const SString& Value, SString* OutLeft, SString* OutRight, bool CaseSensitive = true, bool FromStart = true) const
	{
		const CharType* foundPtr = FindSubstringPrivate(*this, Value, CaseSensitive, FromStart);
		if(!foundPtr)
			return false;

		const SizeType asIndex = PTR_DIFF_TYPED(SizeType, foundPtr, Data.GetData());

		if(OutLeft)
		{
			OutLeft->Data = DataType(Data.GetData(), asIndex);
		}

		if(OutRight)
		{
			OutRight->Data = DataType(foundPtr, Data.GetCount() - asIndex + 1);
		}

		return true;
	}

	TArray<SString> SplitToArray(const SString& Delimiter, bool DiscardEmpty = true, SizeType Num = -1, bool CaseSensitive = true) const
	{
		TArray<SString> result;

		SplitBySubstringPrivate(*this, Delimiter, DiscardEmpty, CaseSensitive,
			[&result, &Num](const CharType* Ptr, SizeType Count) -> bool
			{
				SString& newStr = result.AddUnitialized_GetRef();
				newStr.Data = DataType(Ptr, Count);
				newStr.Data.Add(CHAR_TERM);
				return (--Num == 0);
			}
		);

		return result;
	}

public: // Manipulation

	SString Replace(const SString& From, const SString& To, SizeType Num = -1, bool CaseSensitive = true) const
	{
		SString newString(*this);
		newString.ReplaceInline(From, To, Num, CaseSensitive);
		return newString;
	}

	// -1 = All
	void ReplaceInline(const SString& From, const SString& To, SizeType Num = -1, bool CaseSensitive = true)
	{
		CHECK_RET(Num != 0);

		DataType newData(Data.GetCount(), true);
		SplitBySubstringPrivate(*this, From, false, CaseSensitive,
			[&newData, &To, &Num](const CharType* Ptr, SizeType Count) -> bool
			{
				const bool isLast = (*(Ptr + Count + 1) == CHAR_TERM);

				if(!isLast)
				{
					if(Count > 0)
					{
						newData.Append(Ptr, Count);
					}

					if(To.Data.GetCount() > 1)
					{
						newData.Append(To.Data.GetData(), To.Data.GetCount() - 1);
					}
				}

				if(--Num == 0 || isLast)
				{
					newData.Add(CHAR_TERM);
					return true;
				}

				return false;
			}
		);

		if(newData.GetCount() > 0)
		{
			Data.Emplace(newData);
		}
	}

	SString ToUpper() const
	{
		SString newString(*this);
		newString.ToUpperInline();
		return newString;
	}

	FORCEINLINE void ToUpperInline() { SCString::ToUpper(Data.GetData()); }

	SString ToLower() const
	{
		SString newString(*this);
		newString.ToLowerInline();
		return newString;
	}

	FORCEINLINE void ToLowerInline() { SCString::ToLower(Data.GetData()); }

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
	FORCEINLINE void AppendImpl(CharType Other) { Data.Add(Other); Data.Swap(Data.GetCount() - 1, Data.GetCount() - 2); }

	void AppendImpl(const DataType& InData, bool IncludesTerminationCharacter)
	{
		Data.RemoveAt(Data.GetCount() - 1); // Remove termination character
		Data.Append(InData.GetData(), InData.GetCount());

		if(!IncludesTerminationCharacter)
		{
			Data.Add(CHAR_TERM);
		}
	}
	
	FORCEINLINE void FillToEmptyImpl() { Data.Add(CHAR_TERM); }
	FORCEINLINE void FillToEmptyImpl(CharType Character) { Data.Append({Character, CHAR_TERM});}
	FORCEINLINE void FillToEmptyImpl(const SString& Other) { Data = Other.Data; }
	FORCEINLINE void FillToEmptyImpl(SString&& Other) { Data = Other.Data; Other.Reset(); }

	void FillToEmptyImpl(const CharType* Text) 
	{
		if(Text)
			Data = DataType(Text, SCString::GetLength(Text) + 1);
		else
			Data = DataType({CHAR_TERM});
	}

	FORCEINLINE void EmptyImpl() { Data.Empty(); }
	FORCEINLINE SizeType GetLastCharIndex() const { return Data.GetCount() - 2; }

	static bool IsAtIndexPrivate(const SString& Main, const SString& Value, SizeType Index, bool CaseSensitive)
	{
		if(Index < 0 || Main.GetLength() < Index + Value.GetLength())
			return false;

		for(SizeType i = Index; i < Index + Value.GetLastCharIndex(); ++i)
		{
			CharType lhs = Main.Data[i];
			CharType rhs = Value.Data[i];

			if(!CaseSensitive)
			{
				lhs = SCString::ToLowerChar(lhs);
				rhs = SCString::ToLowerChar(rhs);
			}

			if(lhs != rhs)
			{
				return false;
			}
		}

		return true;
	}

	static SizeType FindOccurencePrivate(const SString& Main, const SString& Sub, bool CaseSensitive, bool FromStart)
	{
		const CharType* foundPtr = FindSubstringPrivate(Main, Sub, CaseSensitive, FromStart);
		if(foundPtr)
		{
			return PTR_DIFF_TYPED(SizeType, foundPtr, Main.Data.GetData());
		}
		else
		{
			return INDEX_NONE;
		}
	}

	template<typename FuncType>
	static void SplitBySubstringPrivate(
		const SString& Main, const SString& Sub, 
		bool IgnoreEmpty, bool CaseSensitive,
		FuncType&& Functor)
	{
		const SizeType mainLen = Main.GetLength();
		const SizeType subLen = Sub.GetLength();

		if(mainLen > 0 && mainLen > subLen)
		{
			const TArray<CharType> mainStr = CaseSensitive ? Main.Data : Main.ToLower().Data;
			const TArray<CharType> subStr = CaseSensitive ? Sub.Data : Sub.ToLower().Data;

			const CharType* init = mainStr.GetData();
			const CharType* subChars = subStr.GetData();

			while(const CharType* current = SCString::FindSubstring(init, subChars))
			{
				if (!IgnoreEmpty || current-init)
				{
					const SizeType currIdx = PTR_DIFF_TYPED(SizeType, init, mainStr.GetData());
					const SizeType count = PTR_DIFF_TYPED(SizeType, current, init);

					if(Functor(mainStr.GetData() + currIdx, count))
						return;
				}

				init = current + subLen;
			}

			if (!IgnoreEmpty || *init != CHAR_TERM)
			{
				const SizeType currIdx = PTR_DIFF_TYPED(SizeType, init, mainStr.GetData());
				const SizeType count = mainStr.GetCount() - currIdx;

				Functor(mainStr.GetData() + currIdx, count);
			}
		}
	}

	static bool ContainsOnlyWhitespacesPrivate(const SString& Value)
	{
		if(Value.GetLength() > 0)
		{
			const CharType* data = Value.Data.GetData();
			while(*data != CHAR_TERM)
			{
				if(!SCString::IsWhitespaceChar(*data))
					return false;

				++data;
			}
		}

		return true;
	}

	static int32 ComparePrivate(const SString& Lhs, const SString& Rhs, bool CaseSensitive) 
	{
		return CaseSensitive ?
			SCString::Compare(Lhs.Data.GetData(), Rhs.Data.GetData()) :
			SCString::Compare(Lhs.ToLower().Data.GetData(), Rhs.ToLower().Data.GetData());
	}

	static const CharType* FindSubstringPrivate(const SString& Main, const SString& Sub, bool CaseSensitive, bool FromStart)
	{
		if(CaseSensitive)
		{
			return FromStart ? 
				SCString::FindSubstring(Main.Data.GetData(), Sub.Data.GetData()) :
				SCString::FindSubstringReversed(Main.Data.GetData(), Sub.Data.GetData());
		}
		else
		{
			return FromStart ? 
				SCString::FindSubstring(Main.ToLower().Data.GetData(), Sub.ToLower().Data.GetData()) :
				SCString::FindSubstringReversed(Main.ToLower().Data.GetData(), Sub.ToLower().Data.GetData());
		}
	}

private: // Fields

	DataType Data;

};