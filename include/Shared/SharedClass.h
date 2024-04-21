// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Shared/SharedObject.h"
#include "Shared/Internals/SharedClassInternals.h"

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
		CHECK_RET(WeakThis.IsValid(), nullptr);
		return WeakThis.Pin();
	}
	
	// Gets pointer as shared_ptr with provided type
	template<typename ChildType>
	FORCEINLINE TSharedPtr<ChildType> AsShared()
	{
		CHECK_RET(WeakThis.IsValid(), nullptr);
		return (TSharedPtr<ChildType>)WeakThis.Pin();
	}

public: // Private methods

	// Do not call this method DIRECTLY!
	FORCEINLINE void Init_Private(const TSharedPtr<ClassType>& InPtr)
	{
		CHECK_RET(!bIsSharedInitialized);
		
		WeakThis = InPtr;
		bIsSharedInitialized = true;
	}

private: // Fields

	mutable TWeakPtr<ClassType> WeakThis;

private: // Primitive fields

	bool bIsSharedInitialized;
};