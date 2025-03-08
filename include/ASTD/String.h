// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Archive.h"
#include "ASTD/Build.h"

#include "ASTD/Array.h"
#include "ASTD/CString.h"
#include "ASTD/Check.h"

struct SString
{
	// Types
	/////////////////////////////////

	typedef tchar CharType;
	typedef TArray<CharType> DataType;
	typedef typename DataType::SizeType SizeType;

	typedef CharType* StringIteratorType;
	typedef const CharType* ConstStringIteratorType;

	// Constructors
	/////////////////////////////////

	FORCEINLINE SString() { InitToEmpty(); }

	FORCEINLINE SString(const SString& other) { AppendStringImpl(other); }
	FORCEINLINE SString(SString&& other) noexcept { AppendStringImpl(Move(other)); }

	FORCEINLINE SString(const CharType* text) { AppendCharsImpl(text); }
	FORCEINLINE SString(const CharType* text, SizeType length) { AppendCharsImpl(text, length); }

	FORCEINLINE explicit SString(const DataType& data) { AppendDataImpl(data); }
	FORCEINLINE explicit SString(DataType&& data) noexcept { AppendDataImpl(Move(data)); }

	// Static fields
	/////////////////////////////////

	static const SString& GetEmpty()
	{
		static SString emptyString = SString();
		return emptyString;
	}

	// Compare operators
	/////////////////////////////////

	FORCEINLINE bool operator==(const SString& other) const { return _data == other._data; }
	FORCEINLINE bool operator!=(const SString& other) const { return !operator==(other); }

	// Assign operators
	/////////////////////////////////

	FORCEINLINE SString& operator=(const SString& other) { InitToEmpty(); AppendStringImpl(other); return *this; }
	FORCEINLINE SString& operator=(SString&& other) noexcept { InitToEmpty(); AppendStringImpl(Move(other)); return *this; }

	// Arithmetic operators
	/////////////////////////////////

	FORCEINLINE SString operator+(const SString& other) const { SString tmpStr(*this); tmpStr.AppendStringImpl(other); return *this; }
	FORCEINLINE SString operator+(SString&& other) const { SString tmpStr(*this); tmpStr.AppendStringImpl(Move(other)); return *this; }

	FORCEINLINE SString& operator+=(const SString& other) { AppendStringImpl(other); return *this; }
	FORCEINLINE SString& operator+=(SString&& other) { AppendStringImpl(Move(other)); return *this; }

	// Get operators
	/////////////////////////////////

	FORCEINLINE const CharType* operator*() const { return _data.GetData(); }
	FORCEINLINE CharType* operator*() { return _data.GetData(); }

	FORCEINLINE CharType operator[](SizeType idx) const { return _data[idx]; }

	// Path operators
	/////////////////////////////////

	FORCEINLINE SString operator/(const SString& other) const { SString tmpStr(*this); tmpStr.operator/=(other); return tmpStr; }
	FORCEINLINE SString operator/=(const SString& other) { AppendCharsImpl(TEXT("/")); AppendStringImpl(other); return *this; }

	// Property getters
	/////////////////////////////////

	FORCEINLINE const DataType& GetData() const { return _data; }

	FORCEINLINE const CharType* GetChars() const { return _data.GetData(); }
	FORCEINLINE CharType* GetChars() { return _data.GetData(); }

	FORCEINLINE SizeType GetLength() const { return _data.GetNum() > 1 ? _data.GetNum() - 1 : 0; }

	FORCEINLINE bool IsValidIndex(SizeType idx) const { return idx >= 0 && idx < GetLastCharIndex(); }
	FORCEINLINE bool IsEmpty() const { return GetLength() == 0; }

	// Construction
	/////////////////////////////////

	template<
		typename StringT,
		typename... VarTypes>
	static SString Printf(StringT&& fmt, VarTypes&&... args)
	{
		static_assert(TIsSame<StringT, CharType*>::Value || TIsSame<StringT, SString>::Value, "Format variable has to be string type");
		thread_local CharType buffer[SCString::LARGE_BUFFER_SIZE];

		// TODO: Replace with custom implementation
		if constexpr (TIsSame<CharType, wchar>::Value)
		{
			swprintf(buffer, SCString::LARGE_BUFFER_SIZE, fmt, Forward<VarTypes>(args)...);
		}
		else
		{
			snprintf(buffer, SCString::LARGE_BUFFER_SIZE, fmt, Forward<VarTypes>(args)...);
		}

		return SString(buffer);
	}

	// Conversions
	/////////////////////////////////

	FORCEINLINE int32 ToInt32() const { return SCString::ToInt32(_data.GetData()); }
	FORCEINLINE int64 ToInt64() const { return SCString::ToInt64(_data.GetData()); }
	FORCEINLINE double ToDouble() const { return SCString::ToDouble(_data.GetData()); }

	static SString FromInt32(int32 val)
	{
		thread_local CharType buffer[SCString::MAX_BUFFER_SIZE_INT32];
		return SString(SCString::FromInt32(val, buffer, SCString::MAX_BUFFER_SIZE_INT32));
	}

	static SString FromInt64(int64 val)
	{
		thread_local CharType buffer[SCString::MAX_BUFFER_SIZE_INT64];
		return SString(SCString::FromInt64(val, buffer, SCString::MAX_BUFFER_SIZE_INT64));
	}

	static SString FromDouble(double val, uint8 digits)
	{
		thread_local CharType buffer[SCString::MAX_BUFFER_SIZE_DOUBLE];
		return SString(SCString::FromDouble(val, digits, buffer, SCString::MAX_BUFFER_SIZE_DOUBLE));
	}

	// Iterations
	/////////////////////////////////

	FORCEINLINE StringIteratorType begin() { return _data.GetNum() > 1 ? &_data[0] : nullptr; }
	FORCEINLINE ConstStringIteratorType begin() const { return _data.GetNum() > 1 ? &_data[0] : nullptr; }

	FORCEINLINE StringIteratorType end() { return _data.GetNum() > 1 ? &_data[GetLastCharIndex()] : nullptr; }
	FORCEINLINE ConstStringIteratorType end() const { return _data.GetNum() > 1 ? &_data[GetLastCharIndex()] : nullptr; }

	// Compares
	/////////////////////////////////

	FORCEINLINE int32 Compare(const SString& other, bool caseSensitive = true) const { return SCString::Compare(GetChars(), other.GetChars(), caseSensitive); }
	FORCEINLINE bool Equals(const SString& other, bool caseSensitive = true) const { return Compare(other, caseSensitive) == 0; }

	// Checks
	/////////////////////////////////

	FORCEINLINE bool IsWhitespace() const
	{
		if(GetLength() > 0)
		{
			const CharType* data = _data.GetData();
			while(*data != CHAR_TERM)
			{
				if(!SCString::IsWhitespaceChar(*data))
					return false;

				++data;
			}
		}

		return true;
	}

	FORCEINLINE bool StartsWith(const SString& val, bool caseSensitive = true) const
	{
		return IsAtIndexPrivate(*this, val, 0, caseSensitive);
	}

	FORCEINLINE bool EndsWith(const SString& val, bool caseSensitive = true) const
	{
		return IsAtIndexPrivate(*this, val, GetLastCharIndex() - val.GetLastCharIndex(), caseSensitive);
	}

	FORCEINLINE bool Contains(const SString& val, bool caseSensitive = true, bool fromStart = true) const
	{
		return !!SCString::Find(GetChars(), val.GetChars(), caseSensitive, fromStart);
	}

	FORCEINLINE SizeType Find(const SString& val, bool caseSensitive = true, bool fromStart = true) const
	{
		return SCString::FindIndex(GetChars(), val.GetChars(), caseSensitive, fromStart);
	}

	// Append
	/////////////////////////////////

	FORCEINLINE void Append(const SString& other) { AppendStringImpl(other); }
	FORCEINLINE void Append(SString&& other) { AppendStringImpl(Move(other)); }
	FORCEINLINE void Append(const CharType* other, SizeType num = INDEX_NONE) { AppendCharsImpl(other, num); }

	template<typename StringT, typename... ArgTypes>
	FORCEINLINE void AppendPrintf(StringT&& fmt, ArgTypes&&... args)
	{
		AppendStringImpl(
			Move(
				SString::Printf(
					Forward<StringT>(fmt),
					Forward<ArgTypes>(args)...
				)
			)
		);
	}

	// Const manipulation
	/////////////////////////////////

	bool Split(const SString& val, SString* outLeft, SString* outRight, bool caseSensitive = true, bool fromStart = true) const
	{
		const SizeType foundIdx = SCString::FindIndex(GetChars(), val.GetChars(), caseSensitive, fromStart);
		if (foundIdx == INDEX_NONE)
			return false;

		if(outLeft)
		{
			outLeft->_data = DataType(_data.GetData(), foundIdx + 1);
			outLeft->_data[foundIdx] = CHAR_TERM;
		}

		if(outRight)
		{
			outRight->_data = DataType(GetChars() + foundIdx + 1, _data.GetNum() - foundIdx);
		}

		return true;
	}

	TArray<SString> SplitToArray(const SString& delimiter, bool discardEmpty = true, SizeType num = INDEX_NONE, bool caseSensitive = true) const
	{
		TArray<SString> result;

		SplitBySubstringPrivate(*this, delimiter, discardEmpty, caseSensitive, _data.GetNum(),
			[&result, &num](const CharType* ptr, SizeType count) -> bool
			{
				SString& newStr = result.AddUnitialized_GetRef();
				newStr._data = DataType(ptr, count);
				newStr._data.Add(CHAR_TERM);
				return (--num == 0);
			}
		);

		return result;
	}

	// Manipulation
	/////////////////////////////////

	SString Replace(const SString& from, const SString& to, SizeType num = INDEX_NONE, bool caseSensitive = true) const
	{
		SString newString(*this);
		newString.ReplaceInline(from, to, num, caseSensitive);
		return newString;
	}

	// -1 = All
	void ReplaceInline(const SString& from, const SString& to, SizeType num = INDEX_NONE, bool caseSensitive = true)
	{
		DataType newData(_data.GetNum(), true);
		SplitBySubstringPrivate(*this, from, false, caseSensitive, (num == -1) ? _data.GetNum() : num,
			[&newData, &to, &num](const CharType* ptr, SizeType count) -> bool
			{
				const bool isLast = (*(ptr + count + 1) == CHAR_TERM);

				if(count > 0)
				{
					newData.Append(ptr, count);
				}

				if(!isLast)
				{
					if(to._data.GetNum() > 1)
					{
						newData.Append(to._data.GetData(), to._data.GetNum() - 1);
					}
				}

				if(isLast)
				{
					newData.Add(CHAR_TERM);

					// is redundant, but if implementation changes this might save a day
					return true;
				}

				return false;
			}
		);

		if(newData.GetNum() > 0)
		{
			_data.Replace(newData);
		}
	}

	SString ToUpper() const
	{
		SString newString(*this);
		newString.ToUpperInline();
		return newString;
	}

	FORCEINLINE void ToUpperInline() { SCString::ToUpper(_data.GetData()); }

	SString ToLower() const
	{
		SString newString(*this);
		newString.ToLowerInline();
		return newString;
	}

	FORCEINLINE void ToLowerInline() { SCString::ToLower(_data.GetData()); }

	SString ChopRight(SizeType idx) const
	{
		SString newString(*this);
		newString.ChopRightInline(idx);
		return newString;
	}

	void ChopRightInline(SizeType idx)
	{
		if(!IsValidIndex(idx))
		{
			return;
		}

		_data.Shrink(idx + 1);
		_data[idx] = CHAR_TERM;
	}

	SString ChopLeft(SizeType idx) const
	{
		SString newString(*this);
		newString.ChopLeftInline(idx);
		return newString;
	}

	void ChopLeftInline(SizeType idx)
	{
		if(!IsValidIndex(idx))
		{
			return;
		}

		DataType newData(_data.GetNum() - idx);
		for(SizeType i = 0; i < newData.GetNum(); ++i)
		{
			newData[i] = _data[idx + i];
		}

		_data = newData;
	}

	SString ChopRange(SizeType firstIdx, SizeType secondIdx) const
	{
		SString newString(*this);
		newString.ChopRangeInline(firstIdx, secondIdx);
		return newString;
	}

	void ChopRangeInline(SizeType firstIdx, SizeType secondIdx)
	{
		if( !IsValidIndex(firstIdx) || !IsValidIndex(secondIdx))
		{
			return;
		}

		SizeType* biggerVal, *smallerVal;
		if(firstIdx > secondIdx)
		{
			biggerVal = &firstIdx;
			smallerVal = &secondIdx;
		}
		else
		{
			biggerVal = &secondIdx;
			smallerVal = &firstIdx;
		}

		ChopRightInline(*biggerVal);
		ChopLeftInline(*smallerVal);
	}

	// Reset
	/////////////////////////////////

	FORCEINLINE void Reset() { EmptyImpl(true); }
	FORCEINLINE void Empty(bool releaseResources = true) { EmptyImpl(releaseResources); }

	// Other
	/////////////////////////////////

	FORCEINLINE void Reserve(SizeType num) { _data.Reserve(num + 1); } // termination character
	FORCEINLINE void ShrinkToFit() { _data.ShrinkToFit(); }

private:
	FORCEINLINE void InitToEmpty()
	{
		_data.Empty(1);
		AddTermChecked(_data);
	}
	
	template<
		typename DataT,
		typename TEnableIf<TIsSame<typename TDecay<DataT>::Type, DataType>::Value>::Type* = nullptr>
	void AppendDataImpl(DataT&& data)
	{
		RemoveTerm(_data);
		_data.Append(MoveIfPossible(data));
		AddTerm(_data);
	}

	template<
		typename StringT,
		typename TEnableIf<TIsSame<typename TDecay<StringT>::Type, SString>::Value>::Type* = nullptr>
	void AppendStringImpl(StringT&& str)
	{
		RemoveTerm(_data);
		_data.Append(MoveIfPossible(str._data));
		AddTerm(_data);
	}

	void AppendCharsImpl(const CharType* text, SizeType textLen = INDEX_NONE)
	{
		RemoveTerm(_data);
		if (text)
		{
			_data.Append(text, textLen > 0 ? textLen : SCString::GetLength(text) + 1);
		}
		AddTerm(_data);
	}

	FORCEINLINE void EmptyImpl(bool keepResources) { _data.Empty(keepResources); }
	FORCEINLINE SizeType GetLastCharIndex() const { return _data.GetNum() - 2; }

	FORCEINLINE_DEBUGGABLE static bool HasTerm(const DataType& data) { return !data.IsEmpty() && *data.GetLast() == CHAR_TERM; }
	FORCEINLINE static void AddTermChecked(DataType& data) { data.Add(CHAR_TERM); }
	FORCEINLINE static void AddTerm(DataType& data) { if (!HasTerm(data)) { AddTermChecked(data); }}
	FORCEINLINE static void RemoveTermChecked(DataType& data) { data.RemoveAt(data.GetNum() - 1); }
	FORCEINLINE static void RemoveTerm(DataType& data) { if (HasTerm(data)) { RemoveTermChecked(data); }}

	static bool IsAtIndexPrivate(const SString& str, const SString& val, SizeType idx, bool caseSensitive)
	{
		if(idx < 0 || str.GetLength() < idx + val.GetLength())
			return false;

		for(SizeType i = idx; i < idx + val.GetLastCharIndex(); ++i)
		{
			CharType lhs = str._data[i];
			CharType rhs = val._data[i];

			if(!caseSensitive)
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

	template<typename FuncType>
	static void SplitBySubstringPrivate(
		const SString& str, const SString& substr,
		bool ignoreEmpty, bool caseSensitive,
		SizeType maxSplits,
		FuncType&& functor)
	{
		const SizeType mainLen = str.GetLength();
		const SizeType subLen = substr.GetLength();

		if(mainLen > 0 && mainLen > subLen)
		{
			const TArray<CharType> mainStr = caseSensitive ? str._data : str.ToLower()._data;
			const TArray<CharType> subStr = caseSensitive ? substr._data : substr.ToLower()._data;

			const CharType* init = mainStr.GetData();
			const CharType* subChars = subStr.GetData();

			while(const CharType* current = SCString::Find(init, subChars, caseSensitive))
			{
				if (!ignoreEmpty || current-init)
				{
					const SizeType currIdx = PTR_DIFF_TYPED(SizeType, init, mainStr.GetData());
					const SizeType count = PTR_DIFF_TYPED(SizeType, current, init);

					if(functor(mainStr.GetData() + currIdx, count))
						return;
				}

				init = current + subLen;

				if(--maxSplits == 0)
				{
					break;
				}
			}

			if (!ignoreEmpty || *init != CHAR_TERM)
			{
				const SizeType currIdx = PTR_DIFF_TYPED(SizeType, init, mainStr.GetData());
				const SizeType count = mainStr.GetNum() - currIdx;

				functor(mainStr.GetData() + currIdx, count);
			}
		}
	}

	DataType _data = {};
};

template<>
struct TContainerTypeTraits<SString> : public TContainerTypeTraits<void>
{
	using ElementType = SString::CharType;
	using AllocatorType = SString::DataType::AllocatorType;

	enum
	{
		IsContainer = true,
		IsDynamic = true,
		InlineMemory = true
	};
};

// Archive operator<< && operator>>
////////////////////////////////////////////

FORCEINLINE_DEBUGGABLE static SArchive& operator<<(SArchive& ar, const SString& str)
{
	ar.Write(str.GetChars(), str.GetLength());
	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, SString& str)
{
	SString::DataType newData;
	ar >> newData;
	str = SString(Move(newData));
	return ar;
}
