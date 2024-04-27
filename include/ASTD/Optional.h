// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"

#include "ASTD/TypeTraits.h"
#include "ASTD/TypeMethods.h"
#include "ASTD/Memory.h"

template<typename InElementType>
class TOptional
{
public:

	// Typedefs
	/////////////////////////////////

	typedef InElementType ElementType;

	// Asserts
	/////////////////////////////////

	static_assert(!TIsSame<ElementType, void>::Value && !TIsReference<ElementType>::Value, "Element type is not valid");

	// Constructors
	/////////////////////////////////

	FORCEINLINE TOptional() = default;
	FORCEINLINE TOptional(const TOptional& other) { FillToEmpty(other); }
	FORCEINLINE TOptional(TOptional&& other) { FillToEmpty(Move(other)); }
	FORCEINLINE TOptional(const ElementType& InValue) { FillToEmpty(InValue); }
	FORCEINLINE TOptional(ElementType&& InValue) { FillToEmpty(Move(InValue)); }

	// Destructor
	/////////////////////////////////

	FORCEINLINE ~TOptional() { Reset(); }

	// Comparison operators
	/////////////////////////////////

	FORCEINLINE bool operator==(const TOptional& other) const { return ComparePrivate(*this, other); }
	FORCEINLINE bool operator!=(const TOptional& other) const { return !ComparePrivate(*this, other); }

	// Assign operators
	/////////////////////////////////

	FORCEINLINE TOptional& operator=(const TOptional& other) { Reset(); FillToEmpty(other); return *this; }
	FORCEINLINE TOptional& operator=(TOptional&& other) { Reset(); FillToEmpty(Move(other)); return *this; }

	FORCEINLINE TOptional& operator=(const ElementType& InValue) { Reset(); FillToEmpty(InValue); return *this; }
	FORCEINLINE TOptional& operator=(ElementType&& InValue) { Reset(); FillToEmpty(Move(InValue)); return *this; }

	// Dereference operators
	/////////////////////////////////

	FORCEINLINE const ElementType* operator->() const { return _data; }
	FORCEINLINE ElementType* operator->() { return _data; }

	// Checks
	/////////////////////////////////

	FORCEINLINE bool IsSet() const { return _data != nullptr; }

	// Getters
	/////////////////////////////////

	// Gets copy
	FORCEINLINE ElementType Get(const ElementType& defaultValue) const { return IsSet() ? *_data : defaultValue; }
	FORCEINLINE ElementType Get() const { return GetDefaultedImpl(); }

	// Gets reference, but can crash
	FORCEINLINE const ElementType& GetRef() const { return *_data; }
	FORCEINLINE ElementType& GetRef() { return *_data; }

	// Manipulation
	/////////////////////////////////

	FORCEINLINE void Set(const ElementType& InValue) { Reset(); FillToEmpty(InValue); }
	FORCEINLINE void Set(ElementType&& InValue) { Reset(); FillToEmpty(Move(InValue)); }

	void Reset()
	{
		if(_data)
		{
			SMemory::DestructElement(_data);
			SMemory::DeallocateElement(_data);
			_data = nullptr;
		}
	}

private:

	void FillToEmpty(const ElementType& InValue)
	{
		_data = SMemory::AllocateElement<ElementType>();
		SMemory::CopyElement(_data, InValue);
	}

	void FillToEmpty(ElementType&& InValue)
	{
		_data = SMemory::AllocateElement<ElementType>();
		SMemory::MoveElement(_data, Move(InValue));
	}

	FORCEINLINE void FillToEmpty(const TOptional& other) 
	{
		if(other.IsSet())
		{
			FillToEmpty(*other._data);
		}
	}

	FORCEINLINE void FillToEmpty(TOptional&& other)
	{
		if(other.IsSet())
		{
			_data = other._data;
			other._data = nullptr;
		}
	}

	ElementType GetDefaultedImpl()
	{
		if constexpr(TIsConstructible<ElementType>::Value)
		{
			return IsSet() ? *_data : ElementType();
		}
		else
		{
			static_assert(sizeof(ElementType) < 0, "Unsuppported type for default construction");
			return DeclVal<ElementType>();
		}
	}

	static bool ComparePrivate(const TOptional& Lhs, const TOptional& Rhs)
	{
		if(Lhs.IsSet() == Rhs.IsSet())
		{
			return Lhs.IsSet() && SMemory::Compare(Lhs._data, Rhs._data, sizeof(ElementType)) == 0;
		}

		return false;
	}

	ElementType* _data = nullptr;
};

// Archive operator<< && operator>>
////////////////////////////////////////////

template<typename T>
FORCEINLINE_DEBUGGABLE static SArchive& operator<<(SArchive& ar, const TOptional<T>& optional)
{
	if (optional.IsSet())
	{
		ar << optional.GetRef();
	}

	return ar;
}

template<typename T>
FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, TOptional<T>& optional)
{
	if (optional.IsSet())
	{
		ar >> optional.GetRef();
	}

	return ar;
}
