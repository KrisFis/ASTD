
#pragma once

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeTraits.h"
#include "TypeTraits/TypeMethods.h"

#include "Memory/MemoryUtilities.h"

template<typename InElementType>
class TOptional
{

public: // Typedefs

	typedef InElementType ElementType;

public: // Asserts

	static_assert(!TIsSame<ElementType, void>::Value && !TIsReference<ElementType>::Value, "Element type is not valid");

public: // Constructors

	FORCEINLINE TOptional() : Value(nullptr) {}
	FORCEINLINE TOptional(const TOptional& Other) : Value(nullptr) { FillToEmpty(Other); }
	FORCEINLINE TOptional(TOptional&& Other) : Value(nullptr) { FillToEmpty(Move(Other)); }
	FORCEINLINE TOptional(const ElementType& InValue) : Value(nullptr) { FillToEmpty(InValue); }
	FORCEINLINE TOptional(ElementType&& InValue) : Value(nullptr) { FillToEmpty(Move(InValue)); }

public: // Destructor

	FORCEINLINE ~TOptional() { Reset(); }

public: // Comparison operators

	FORCEINLINE bool operator==(const TOptional& Other) const { return ComparePrivate(*this, Other); }
	FORCEINLINE bool operator!=(const TOptional& Other) const { return !ComparePrivate(*this, Other); }

public: // Assign operators

	FORCEINLINE TOptional& operator=(const TOptional& Other) { Reset(); FillToEmpty(Other); return *this; }
	FORCEINLINE TOptional& operator=(TOptional&& Other) { Reset(); FillToEmpty(Move(Other)); return *this; }

	FORCEINLINE TOptional& operator=(const ElementType& InValue) { Reset(); FillToEmpty(InValue); return *this; }
	FORCEINLINE TOptional& operator=(ElementType&& InValue) { Reset(); FillToEmpty(Move(InValue)); return *this; }

public: // Dereference operators

	FORCEINLINE const ElementType* operator->() const { return Value; }
	FORCEINLINE ElementType* operator->() { return Value; }

public: // Checks

	FORCEINLINE bool IsSet() const { return Value != nullptr; }

public: // Getters

	// Gets copy
	FORCEINLINE ElementType Get(const ElementType& Default) { return IsSet() ? *Value : Default; }
	FORCEINLINE ElementType Get() { return GetDefaultedImpl(); }

	// Gets reference, but can crash
	FORCEINLINE const ElementType& GetRef() const { return *Value; }
	FORCEINLINE ElementType& GetRef() { return *Value; }

public: // Manipulation

	FORCEINLINE void Set(const ElementType& InValue) { Reset(); FillToEmpty(InValue); }
	FORCEINLINE void Set(ElementType&& InValue) { Reset(); FillToEmpty(Move(InValue)); }

	void Reset()
	{
		if(Value)
		{
			DestructElementPrivate(Value);
			SMemory::Deallocate(Value);
			Value = nullptr;
		}
	}

private: // Helper methods

	void FillToEmpty(const ElementType& InValue)
	{
		Value = (ElementType*)SMemory::Allocate(sizeof(ElementType));
		CopyElementPrivate(Value, &InValue);
	}

	void FillToEmpty(ElementType&& InValue)
	{
		Value = (ElementType*)SMemory::Allocate(sizeof(ElementType));
		MoveElementPrivate(Value, &InValue);
	}

	FORCEINLINE void FillToEmpty(const TOptional& Other) 
	{
		if(Other.IsSet())
		{
			FillToEmpty(*Other.Value);
		}
	}

	FORCEINLINE void FillToEmpty(TOptional&& Other)
	{
		if(Other.IsSet())
		{
			FillToEmpty(Move(*Other.Value));
			Other.Value = nullptr;
		}
	}

	void GetDefaultedImpl()
	{
		if constexpr(TIsConstructible<ElementType>::Value)
		{
			return IsSet() ? *Value : ElementType();
		}
		else
		{
			static_assert(sizeof(ElementType) < 0, "Unsuppported type for default construction");
		}
	}

	static void CopyElementPrivate(ElementType* DestValue, const ElementType* SourceVal)
	{
		if constexpr (!TIsTriviallyCopyConstructible<ElementType>::Value)
		{
			SMemory::CallCopyConstructor(DestValue, *SourceVal);
		}
		else
		{
			SMemory::Copy(
				DestValue,
				SourceVal,
				sizeof(ElementType)
			);
		}
	}

	static void MoveElementPrivate(ElementType* DestValue, ElementType* SourceVal)
	{
		if constexpr (!TIsTriviallyMoveConstructible<ElementType>::Value)
		{
			SMemory::CallMoveConstructor(DestValue, Move(*SourceVal));
		}
		else
		{
			SMemory::Copy(
				DestValue,
				SourceVal,
				sizeof(ElementType)
			);
		}
	}

	static void DestructElementPrivate(ElementType* Value)
	{
		if constexpr(!TIsTriviallyDestructible<ElementType>::Value)
		{
			SMemory::CallDestructor(Value);
		}
	}

	static bool ComparePrivate(const TOptional& Lhs, const TOptional& Rhs)
	{
		if(Lhs.IsSet() == Rhs.IsSet())
		{
			return Lhs.IsSet() && (SMemory::Compare(Lhs.Value, Rhs.Value, sizeof(ElementType)) == 0);
		}

		return false;
	}

private: // Fields

	ElementType* Value;

};