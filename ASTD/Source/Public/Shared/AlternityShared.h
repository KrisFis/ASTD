
#pragma once

#include "SharedInternals.h"
#include "SharedObject.h"
#include "TypeTraits.h"

//////////////////////////////////////////////////
// Utils
//////////////////////////////////////////////////

template<typename T, typename... ArgTypes>
inline TSharedPtr<T> MakeShared(ArgTypes&&... Args)
{
	TSharedPtr<T> newPtr;
	newPtr.Referencer = NSharedInternals::NewCustomReferencer(
		new T(Forward<ArgTypes>(Args)...)
	);
	
	return newPtr;
}

template<typename T, typename R = T>
inline TSharedPtr<R> MakeShareable(T* Instance)
{
	static_assert(TIsDerivedFrom<R,T>::Value, "Unrelated instance type to return type");
	
	TSharedPtr<R> newPtr;
	newPtr.Referencer = NSharedInternals::NewCustomReferencer(Instance);
	
	return newPtr;
}

template<typename T, typename R = T>
inline TSharedPtr<T> CastShared(const TSharedPtr<R>& From)
{
	static_assert(!TIsSame<T,R>::Value, "Attempt to cast to same type");
	static_assert(TIsCastable<T,R>::Value, "Unrelated types for casting");
	
	// We only need a check for valid cast, cast is made afterwards by Get()
	TSharedPtr<T> newPtr;
	newPtr.Referencer = From.Referencer;
	return newPtr;
}