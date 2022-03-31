
#pragma once

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeMethods.h"
#include "Validation/Check.h"

#include "Memory/MemoryUtilities.h"

#include "Containers/Internals/ContainerInternals.h"

template<typename InElementType>
class TOptional
{

public: // Typedefs

	typedef InElementType ElementType;
	typedef NContainerInternals::TElementInfo<ElementType> ElementInfo;

public: // Asserts

	static_assert(ElementInfo::IsValid, "Element type is not valid");

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

	FORCEINLINE const ElementType* operator->() const { CHECKF(IsSet()); return Value; }
	FORCEINLINE ElementType* operator->() { CHECKF(IsSet()); return Value; }

public: // Checks

	FORCEINLINE bool IsSet() const { return Value != nullptr; }

public: // Getters

	// Gets copy
	FORCEINLINE ElementType Get(const ElementType& Default) { return IsSet() ? *Value : Default; }
	FORCEINLINE ElementType Get() 
	{
		// TODO(jan.kristian.fisera): Has trivial constructor should be prefered much better
		if constexpr(ElementInfo::IsTrivial)
		{
			return IsSet() ? *Value : ElementInfo();
		}
		else
		{
			static_assert(sizeof(ElementType) < 0, "Unsuppported type for default construction");
		}
	}

	// Gets reference, but can crash
	FORCEINLINE const ElementType& GetRef() const { CHECKF(IsSet()); return *Value; }
	FORCEINLINE ElementType& GetRef() { CHECKF(IsSet()); return *Value; }

public: // Manipulation

	FORCEINLINE void Set(const ElementType& InValue) { Reset(); FillToEmpty(InValue); }
	FORCEINLINE void Set(ElementType&& InValue) { Reset(); FillToEmpty(Move(InValue)); }

	void Reset() 
	{
		if(Value)
		{
			if constexpr(!ElementInfo::IsTrivial)
			{
				SMemory::CallDestructor(Value);
			}

			SMemory::Deallocate(Value);
			Value = nullptr;
		}
	}

private: // Helper methods

	void FillToEmpty(const ElementType& InValue)
	{
		Value = (ElementType*)SMemory::Allocate(sizeof(ElementType));
		if constexpr (ElementInfo::SupportCopy && !(ElementInfo::IsTrivial))
		{
			SMemory::CallCopyConstructor(Value, InValue);
		}
		else
		{
			SMemory::Copy(
				Value,
				&InValue,
				sizeof(ElementType)
			);
		}
	}

	void FillToEmpty(ElementType&& InValue)
	{
		Value = (ElementType*)SMemory::Allocate(sizeof(ElementType));
		if constexpr (ElementInfo::SupportMove && !(ElementInfo::IsTrivial))
		{
			SMemory::CallMoveConstructor(Value, Move(InValue));
		}
		else
		{
			SMemory::Copy(
				Value,
				&InValue,
				sizeof(ElementType)
			);
		}
	}

	void FillToEmpty(const TOptional& Other) 
	{
		if(Other.IsSet())
			FillToEmpty(*Other.Value);
	}

	void FillToEmpty(TOptional&& Other)
	{
		if(Other.IsSet())
		{
			FillToEmpty(Move(*Other.Value));
			Other.Value = nullptr;
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