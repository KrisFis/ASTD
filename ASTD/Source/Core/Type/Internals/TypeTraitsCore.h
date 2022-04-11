
#pragma once

#include "Core/Types.h"

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
// * Checks whether provided type is const, volatile or both

template<typename T> struct TIsConst { enum { Value = false }; };
template<typename T> struct TIsConst<const T> { enum { Value = true }; };
template<typename T> struct TIsConst<volatile T> { enum { Value = true }; };
template<typename T> struct TIsConst<const volatile T> { enum { Value = true }; };

// [Remove Const]
// * Removes "const" and "volatile" from type

template<typename T> struct TRemoveConst { typedef T Type; };
template<typename T> struct TRemoveConst<const T> { typedef T Type; };
template<typename T> struct TRemoveConst<volatile T> { typedef T Type; };
template<typename T> struct TRemoveConst<const volatile T> { typedef T Type; };

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

// [Is Array]
// * Checks whether specific type is array

template<typename T> struct TIsArray { enum { Value = false }; };
template<typename T> struct TIsArray<T[]> { enum { Value = true }; };
template<typename T, uint32 N> struct TIsArray<T[N]> { enum { Value = true }; };

// [Is Function]
// * Checks whether specific type is function

template<typename T> struct TIsFunctionType { enum { Value = false }; };
template <typename RetType, typename... Params> struct TIsFunctionType<RetType(Params...)> { enum { Value = true }; };

// [Is Bool Type]
// * Checks whether specific type is bool type

template<typename T> struct TIsBoolType { enum { Value = false }; };
template<> struct TIsBoolType<bool> { enum { Value = true }; };

// [Is Floating Type]
// * Checks whether specific type is floating type
// * Floating types are: float, double

template<typename T> struct TIsFloatingType { enum { Value = false }; };
template<> struct TIsFloatingType<float> { enum { Value = true }; };
template<> struct TIsFloatingType<double> { enum { Value = true }; };
template<> struct TIsFloatingType<long double> { enum { Value = true }; };

// [Is Integer Type]
// * Checks whether specific type is integer type
// * Integer types are: int8, int16, int32, int64, uint8, uint16, uint32, uint64

template<typename T> struct TIsIntegerType { enum { Value = false }; };
template<> struct TIsIntegerType<int8> { enum { Value = true }; };
template<> struct TIsIntegerType<int16> { enum { Value = true }; };
template<> struct TIsIntegerType<int32> { enum { Value = true }; };
template<> struct TIsIntegerType<int64> { enum { Value = true }; };
template<> struct TIsIntegerType<uint8> { enum { Value = true }; };
template<> struct TIsIntegerType<uint16> { enum { Value = true }; };
template<> struct TIsIntegerType<uint32> { enum { Value = true }; };
template<> struct TIsIntegerType<uint64> { enum { Value = true }; };

// [Is Character Type]
// * Checks whether specific type is character type
// * Character types are: char, wchar

template<typename T> struct TIsCharacterType { enum { Value = false }; };
template<> struct TIsCharacterType<char> { enum { Value = true }; };
template<> struct TIsCharacterType<wchar> { enum { Value = true }; };

// [Is signed type]
// * Checks whether specific type is signed type
// * Signed types are types that is missing "unsigned" qualifier

template<typename T> struct TIsSignedType { enum { Value = true }; };
template<> struct TIsSignedType<uint8> { enum { Value = false }; };
template<> struct TIsSignedType<uint16> { enum { Value = false }; };
template<> struct TIsSignedType<uint32> { enum { Value = false }; };
template<> struct TIsSignedType<uint64> { enum { Value = false }; };