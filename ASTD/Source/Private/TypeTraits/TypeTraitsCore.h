
#pragma once

#include "ASTDPlatform.h"

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

// [Is Array Type]
// * Checks whether specific type is array type

template<typename T> struct TIsArrayType { enum { Value = false }; };
template<typename T> struct TIsArrayType<T[]> { enum { Value = true }; };
template<typename T, uint32 N> struct TIsArrayType<T[N]> { enum { Value = true }; };

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

// [Is Primitive Type]
// * Checks whether specific type is primitive type
// * Primitive types are: array type, floating type, integer type, bool type

template<typename T> struct TIsPrimitiveType { enum { Value = TIsArrayType<T>::Value || TIsFloatingType<T>::Value || TIsIntegerType<T>::Value || TIsBoolType<T>::Value }; };