
#pragma once

#include "IntegerTypes.h"

// [Const bool]
// * False and true type

template<bool T> struct TConstBool { static constexpr bool Value = T; };
template<typename> struct TTrueValue : TConstBool<true> {};
template<typename> struct TFalseValue : TConstBool<false> {};

// [Enable if]
// * Enables compilation of specific template function/struct when condition met

template<bool T, typename R = void> struct TEnableIf;
template<typename R> struct TEnableIf<true, R> { typedef R Type; };
template<typename R> struct TEnableIf<false, R> {};

// [Is Reference]
// * Checks whether provided type is reference

template<typename T> struct TIsReference { enum { Value = false }; };
template<typename T> struct TIsReference<T&> { enum { Value = true }; };
template<typename T> struct TIsReference<T&&> { enum { Value = true }; };

// [Is LValue]
// * Checks whether provided type is lvalue reference

template<typename T> struct TIsLValue { enum { Value = false }; };
template<typename T> struct TIsLValue<T&> { enum { Value = true }; };

// [Is RValue]
// * Checks whether provided type is rvalue reference

template<typename T> struct TIsRValue { enum { Value = false }; };
template<typename T> struct TIsRValue<T&&> { enum { Value = true }; };

// [Remove reference]
// * Removes reference from type

template <typename T> struct TRemoveReference      { typedef T Type; };
template <typename T> struct TRemoveReference<T& > { typedef T Type; };
template <typename T> struct TRemoveReference<T&&> { typedef T Type; };

// [Is Const]
// * Checks whether provided type is const, volatiole or both

template<typename T> struct TIsConstVolatile { enum { Value = false }; };
template<typename T> struct TIsConstVolatile<const T> { enum { Value = true }; };
template<typename T> struct TIsConstVolatile<volatile T> { enum { Value = true }; };
template<typename T> struct TIsConstVolatile<const volatile T> { enum { Value = true }; };

// [Remove Const]
// * Removes "const" and "volatile" from type

template<typename T> struct TRemoveConstVolatile { typedef T Type; };
template<typename T> struct TRemoveConstVolatile<const T> { typedef T Type; };
template<typename T> struct TRemoveConstVolatile<volatile T> { typedef T Type; };
template<typename T> struct TRemoveConstVolatile<const volatile T> { typedef T Type; };

// [Is Pointer]
// * Checks whether provided type is pointer

template <typename T> struct TIsPointer { enum { Value = false }; };
template <typename T> struct TIsPointer<T*> { enum { Value = true }; };
template <typename T> struct TIsPointer<const T*> { enum { Value = true }; };

// [Remove Pointer]
// * Removes pointer from provided type

template<typename T> struct TRemovePointer { typedef T Type; };
template<typename T> struct TRemovePointer<T*> { typedef T Type; };
template<typename T> struct TRemovePointer<const T*> { typedef T Type; };