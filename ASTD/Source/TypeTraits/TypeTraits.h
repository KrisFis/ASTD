
#pragma once

#include "TypeTraits/Internals/TypeTraitsCore.h"
#include "TypeTraits/Internals/TypeTraitsConstruct.h"
#include "TypeTraits/Internals/TypeTraitsIsType.h"
#include "TypeTraits/Internals/TypeTraitsInternals.h"

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

// [Is Derived From]
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

// [Is Base Of]
// * Checks whether specific type is base of other type

template<typename BaseType, typename DerivedType>
struct TIsBaseOf : TIsDerivedFrom<DerivedType, BaseType> {};

// [Is Same]
// * Checks whether specified types are the same

template<typename T, typename R> struct TIsSame { enum { Value = false }; };
template<typename T> struct TIsSame<T, T> { enum { Value = true }; };

// [Is Castable]
// * Checks whether specific types could be casted to each other

template<typename T, typename R>
struct TIsCastable { enum { Value = TIsDerivedFrom<T, R>::Value || TIsBaseOf<T, R>::Value }; };

// [Get Nth type]
// * Gets Nth type from parameter pack

template<uint32 N, typename T, typename... ArgTypes> struct TGetNthType { typedef typename TGetNthType<N - 1, ArgTypes...>::Type Type; };
template<typename T, typename... ArgTypes> struct TGetNthType<0, T, ArgTypes...> { typedef T Type; };

// [Size]
// * Gets size without need of std

typedef decltype(sizeof(0)) TSize;

// [Decay]
// * Returns the decayed type, meaning it removes all references, qualifiers and
// * applies array-to-pointer and function-to-pointer conversions.

template<typename T>
struct TDecay 
{ 
	typedef typename NTypeTraitsInternals::TDecayHelper<
		typename TRemoveConst<typename TRemoveReference<T>::Type>::Type
	>::Type Type; 
};

// [Get type]
// * Gets type variations

template<typename T>
struct TGetType
{
	typedef T Value;

	typedef T& Reference;
	typedef const T& ConstReference;

	typedef T* Pointer;
	typedef const T* ConstPointer;
};

// [Call traits]
// * Determines which type will be used for call 
// * Similar to boost's call_traits, ie. having info about optimizations of which type is used

template <typename T>
struct TCallTraits : TGetType<T>
{
private:

	enum { IsSmallType = ((sizeof(T) <= sizeof(void*)) && TIsPODType<T>::Value ) || TIsArithmetic<T>::Value };

public:

	typedef typename NTypeTraitsInternals::TCallTraitsHelper<T, IsSmallType>::Type Param;
	typedef typename NTypeTraitsInternals::TCallTraitsHelper<T, IsSmallType>::ConstType ConstParam;

};