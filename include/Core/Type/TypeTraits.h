// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Type/Internals/TypeTraitsCore.h"
#include "Core/Type/Internals/TypeTraitsConstruct.h"
#include "Core/Type/Internals/TypeTraitsIsType.h"
#include "Core/Type/Internals/TypeTraitsInternals.h"

// [Size]
// * Gets size type without need of std (same as std::size_t)

typedef decltype(sizeof(0)) TSize;

// [Remove const reference]
// * Removes const and reference from specific type

template<typename T>
struct TRemoveConstReference
{
	typedef typename TRemoveConst<typename TRemoveReference<T>::Type>::Type Type;
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


// [Is arithmetic]
// * Checks whether specific type is arithmetic

template <typename T>
struct TIsArithmetic
{
	enum { Value =
			TIsIntegerType<T>::Value ||
			TIsFloatingType<T>::Value ||
			TIsCharacterType<T>::Value ||
			TIsBoolType<T>::Value
	};
};

// [Decay]
// * Returns the decayed type
// * ie. applies array-to-pointer and function-to-pointer conversions

template<typename T>
struct TDecay
{
	typedef typename NTypeInternals::TDecayHelper<
		typename TRemoveConstReference<T>::Type
	>::Type Type;
};

// [Pure]
// * Removes all qualifiers

template<typename T>
struct TPure
{
private:

	typedef typename TRemoveConstReference<T>::Type TestType;

public:

	typedef typename TChoose<
		TIsPointer<TestType>::Value, 
		typename TRemovePointer<TestType>::Type,
		typename NTypeInternals::TDecayHelper<TestType>::Type
	>::Type Type;
};

// [Get type]
// * Gets type variations

template<typename T>
struct TGetType
{
	typedef typename TPure<T>::Type Value;

	typedef Value& Reference;
	typedef const Value& ConstReference;

	typedef Value* Pointer;
	typedef const Value* ConstPointer;
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

	typedef typename NTypeInternals::TCallTraitsHelper<T, IsSmallType>::Type Param;
	typedef typename NTypeInternals::TCallTraitsHelper<T, IsSmallType>::ConstType ConstParam;

};
