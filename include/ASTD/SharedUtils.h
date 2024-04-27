// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"

#include "ASTD/Internal/SharedClassInternals.h"
#include "ASTD/SharedObject.h"

namespace NShared
{
	template<typename T, typename... ArgTypes>
	static TSharedPtr<T> Make(ArgTypes&&... args)
	{
		return NSharedInternals::InitAsSharedClass<T>(NSharedInternals::NewCustomReferencer(new T(Forward<ArgTypes>(args)...)));
	}

	template<typename T, typename R = T>
	static TSharedPtr<R> MakeFromInstance(T* instance)
	{
		static_assert(TIsDerivedFrom<R,T>::Value, "Unrelated instance type to return type");
		return NSharedInternals::InitAsSharedClass<R>(NSharedInternals::NewCustomReferencer(instance));
	}
}
