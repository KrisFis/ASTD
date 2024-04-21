// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Shared/SharedClass.h"
#include "Shared/SharedObject.h"

template<typename T, typename... ArgTypes>
static TSharedPtr<T> MakeShared(ArgTypes&&... Args)
{
	return NSharedInternals::InitAsSharedClass<T>(NSharedInternals::NewCustomReferencer(new T(Forward<ArgTypes>(Args)...)));
}

template<typename T, typename R = T>
static TSharedPtr<R> MakeShareable(T* Instance)
{
	static_assert(TIsDerivedFrom<R,T>::Value, "Unrelated instance type to return type");
	return NSharedInternals::InitAsSharedClass<R>(NSharedInternals::NewCustomReferencer(Instance));
}