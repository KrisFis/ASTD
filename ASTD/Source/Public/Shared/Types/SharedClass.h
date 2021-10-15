
#pragma once

#include "ASTDCore.h"

#include "SharedClassInternals.h"
#include "AlternityShared.h"

template<typename T>
class TSharedClass
{
public: // Typedefs

	typedef T ClassType;
	
public: // Constructors

	FORCEINLINE TSharedClass()
		: bIsSharedInitialized(false)
	{}
	
public: // Getters

	FORCEINLINE bool IsSharedInitialized() const { return bIsSharedInitialized; }

public: // External method

	// Gets pointer as shared_ptr
	FORCEINLINE TSharedPtr<ClassType> AsShared()
	{
		ENSURE_RET(WeakThis.IsValid(), nullptr);
		return WeakThis.Pin();
	}
	
	// Gets pointer as shared_ptr with provided type
	template<typename ChildType>
	FORCEINLINE TSharedPtr<ChildType> AsShared()
	{
		ENSURE_RET(WeakThis.IsValid(), nullptr);
		return CastShared<ChildType>(WeakThis.Pin());
	}

public: // Private methods

	FORCEINLINE void Init_Private(const TSharedPtr<ClassType>& InPtr)
	{
		ENSURE_RET(!bIsSharedInitialized);
		
		WeakThis = InPtr;
		bIsSharedInitialized = true;
	}

private: // Fields

	mutable TWeakPtr<ClassType> WeakThis;

private: // Primitive fields

	bool bIsSharedInitialized;

};