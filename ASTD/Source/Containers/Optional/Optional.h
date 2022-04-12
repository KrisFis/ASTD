
#pragma once

#include "Core/Types.h"
#include "Core/Type/TypeTraits.h"
#include "Core/Type/TypeMethods.h"

#include "Core/Memory.h"

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
	FORCEINLINE ElementType Get(const ElementType& Default) const { return IsSet() ? *Value : Default; }
	FORCEINLINE ElementType Get() const { return GetDefaultedImpl(); }

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
			NMemoryType::Destruct(Value);
			NMemoryType::Deallocate(Value);
			Value = nullptr;
		}
	}

private: // Helper methods

	void FillToEmpty(const ElementType& InValue)
	{
		Value = NMemoryType::Allocate<ElementType>();
		NMemoryType::CopyConstruct(Value, InValue);
	}

	void FillToEmpty(ElementType&& InValue)
	{
		Value = NMemoryType::Allocate<ElementType>();
		NMemoryType::MoveConstruct(Value, Move(InValue));
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
			Value = Other.Value;
			Other.Value = nullptr;
		}
	}

	ElementType GetDefaultedImpl()
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