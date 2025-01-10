// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Build.h"

#include "ASTD/_internal/SharedClassInternals.h"
#include "ASTD/SharedObject.h"

template<typename T, typename... ArgTypes>
static TSharedPtr<T> MakeShared(ArgTypes&&... args)
{
	return NSharedInternals::InitAsSharedClass<T>(NSharedInternals::NewCustomReferencer(new T(Forward<ArgTypes>(args)...)));
}

template<typename T, typename R = T>
static TSharedPtr<R> MakeShareable(T* instance)
{
	static_assert(TIsDerivedFrom<R,T>::Value, "Unrelated instance type to return type");
	return NSharedInternals::InitAsSharedClass<R>(NSharedInternals::NewCustomReferencer(instance));
}
