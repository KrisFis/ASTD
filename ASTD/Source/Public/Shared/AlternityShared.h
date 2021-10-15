
#pragma once

#include "ASTDCore.h"

#include "SharedObject.h"
#include "SharedObjectInternals.h"
#include "SharedClassInternals.h"

//////////////////////////////////////////////////
// Utils
//////////////////////////////////////////////////

template<typename T, typename... ArgTypes>
inline TSharedPtr<T> MakeShared(ArgTypes&&... Args)
{
	return NSharedInternals::InitAsSharedClass<T>(NSharedInternals::NewCustomReferencer(new T(Forward<ArgTypes>(Args)...)));
}

template<typename T, typename R = T>
inline TSharedPtr<R> MakeShareable(T* Instance)
{
	static_assert(TIsDerivedFrom<R,T>::Value, "Unrelated instance type to return type");
	return NSharedInternals::InitAsSharedClass<R>(NSharedInternals::NewCustomReferencer(Instance));
}

template<typename T, typename R = T>
inline TSharedPtr<T> CastShared(const TSharedPtr<R>& From)
{
	static_assert(!TIsSame<T,R>::Value, "Attempt to cast to same type");
	static_assert(TIsCastable<T,R>::Value, "Unrelated types for casting");
	
	// We only need a check for valid cast, cast is made afterwards by Get()
	return TSharedPtr<T>(From.Referencer.Get());
}