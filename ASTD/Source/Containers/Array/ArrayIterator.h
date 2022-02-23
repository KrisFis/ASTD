
#pragma once

#include "Type/TypeUtilities.h"

template<typename InElementType>
class TArrayIterator
{

public: // Types

	typedef InElementType ElementType;

public: // Constructors

	FORCEINLINE TArrayIterator()
		: Value(nullptr)
	{}

	FORCEINLINE TArrayIterator(ElementType* InValue)
		: Value(InValue)
	{}

	FORCEINLINE TArrayIterator(const TArrayIterator& Other)
		: Value(Other.Value)
	{}

	FORCEINLINE TArrayIterator(TArrayIterator&& Other)
		: Value(Other.Value)
	{
		Other.Value = nullptr;
	}

public: // Operators for loop
		// * should be checked against nullptr

	FORCEINLINE operator bool() { return Value != nullptr; }

	FORCEINLINE bool operator==(const TArrayIterator& Other) { return Value == Other.Value; }
	FORCEINLINE bool operator!=(const TArrayIterator& Other) { return Value != Other.Value; }

	FORCEINLINE ElementType& operator*() { return *Value;}

	TArrayIterator& operator++() { Value += 1; return *this;}
	TArrayIterator operator++(int) { TArrayIterator retVal(*this); Value += 1; return retVal; }

	TArrayIterator& operator--() { Value -= 1; return *this;}
	TArrayIterator operator--(int) { TArrayIterator retVal(*this); Value -= 1; return retVal; }

private:

	ElementType* Value;
};