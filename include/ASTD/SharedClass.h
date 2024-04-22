// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"

#include "ASTD/SharedObject.h"

template<typename T>
class TSharedClass
{
	// Typedefs
	/////////////////////////////////

	typedef T ClassType;

	// Constructors
	/////////////////////////////////

	FORCEINLINE TSharedClass() : _isSharedInitialized(false) {}
	
	// Getters
	/////////////////////////////////

	FORCEINLINE bool IsSharedInitialized() const { return _isSharedInitialized; }

	// External method
	/////////////////////////////////

	// Gets pointer as shared_ptr
	FORCEINLINE TSharedPtr<ClassType> AsShared()
	{
		CHECK_RET(_weakThis.IsValid(), nullptr);
		return _weakThis.Pin();
	}
	
	// Gets pointer as shared_ptr with provided type
	template<typename ChildType>
	FORCEINLINE TSharedPtr<ChildType> AsShared()
	{
		CHECK_RET(_weakThis.IsValid(), nullptr);
		return _weakThis.Pin();
	}

private:

	// Do not call this method DIRECTLY!
	FORCEINLINE void Init_Private(const TSharedPtr<ClassType>& ptr)
	{
		CHECK_RET(!_isSharedInitialized);
		
		_weakThis = ptr;
		_isSharedInitialized = true;
	}

	mutable TWeakPtr<ClassType> _weakThis;
	bool _isSharedInitialized;
};
