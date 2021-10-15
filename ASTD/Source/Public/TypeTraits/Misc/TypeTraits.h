
#pragma once

#include "TypeTraitsInternals.h"

// [Decay]
// * Returns the decayed type, meaning it removes all references, qualifiers and
// * applies array-to-pointer and function-to-pointer conversions.

template<typename T>
struct TDecay
{
	typedef typename NTypeTraitsInternals::TDecayHelper<typename TRemoveReference<T>::Type>::Type Type;
};

// [Is Derived]
// * Checks whether specific type is derived from other type

template<typename DerivedType, typename BaseType>
struct TIsDerivedFrom
{

private: // Typedefs

	typedef char No[1];
	typedef char Yes[2];
	
private: // Testing

	static Yes& Test(BaseType*);
	static No& Test(...);

	static DerivedType* ChildPtr() { return nullptr; }

public: // Value
	
	enum { Value = sizeof(Test(ChildPtr())) == sizeof(Yes) };
};

// [Is Same]
// * Checks whether specified types are the same

template<typename T, typename R> struct TIsSame { enum { Value = false }; };
template<typename T> struct TIsSame<T, T> { enum { Value = true }; };

// [Is Castable]
// * Checks whether specific types could be casted to each other

template<typename T, typename R>
struct TIsCastable { enum { Value = TIsDerivedFrom<T, R>::Value || TIsDerivedFrom<R, T>::Value }; };

// [Move]
// * Indicates that an specific object may be "moved from"

template<typename T>
typename TRemoveReference<T>::Type&& Move(T&& Obj) { return static_cast<typename TRemoveReference<T>::Type&&>(Obj); }

// [Forward]
// * Passed specific object with his type provided

template<typename T>
typename TRemoveReference<T>::Type& Forward(T& Obj) { return Obj; }

template<typename T>
typename TRemoveReference<T>::Type&& Forward(T&& Obj) { return Obj; }