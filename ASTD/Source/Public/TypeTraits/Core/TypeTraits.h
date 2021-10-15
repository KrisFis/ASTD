
#pragma once

#include "AlternityMacros.h"
#include "TypeTraitsInternals.h"

// [Decay]
// * Returns the decayed type, meaning it removes all references, qualifiers and
// * applies array-to-pointer and function-to-pointer conversions.

template<typename T>
struct TDecay
{
	typedef typename NTypeTraitsInternals::TDecayHelper<typename TRemoveReference<T>::Type>::Type Type;
};

// [Choose]
// * Chooses between two different types based on a value

template<bool Value, typename TrueType, typename FalseType> struct TChoose;
template<typename TrueType, typename FalseType> struct TChoose<true, TrueType, FalseType> { typedef TrueType Type; };
template<typename TrueType, typename FalseType> struct TChoose<false, TrueType, FalseType> { typedef FalseType Type; };

// [Choose Delayed]
// * Same as choose, but type is delayed by getting type from type

template<bool Value, typename TrueTrait, typename FalseTrait> struct TChooseDelayed;
template<typename TrueTrait, typename FalseTrait> struct TChooseDelayed<true, TrueTrait, FalseTrait> { typedef typename TrueTrait::Type Type; };
template<typename TrueTrait, typename FalseTrait> struct TChooseDelayed<false, TrueTrait, FalseTrait> { typedef typename FalseTrait::Type Type; };


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
FORCEINLINE typename TRemoveReference<T>::Type&& Move(T&& Obj) { return static_cast<typename TRemoveReference<T>::Type&&>(Obj); }

// [Forward]
// * Passed specific object with his type provided

template<typename T>
FORCEINLINE typename TRemoveReference<T>::Type& Forward(T& Obj) { return Obj; }

template<typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& Forward(T&& Obj) { return Obj; }

// [DeclVal]
// * Converts to a reference type, making it possible to use member functions in decltype expressions without the need to go through constructors.

template<typename T>
typename TRemoveReference<T>::Type&& DeclVal();