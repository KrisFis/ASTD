
#pragma once

// [Is constructible]
// * Checks whether specific type is constructible from specific types

template<typename T, typename... ArgTypes>
struct TIsConstructible { enum { Value = __is_constructible(T, ArgTypes...) }; };

// [Has trivial constructor]
// * Checks whether specific type has trivial (empty) constructor

template<typename T>
struct THasTrivialConstructor { enum { Value = __has_trivial_constructor(T) }; };

// [Has trivial copy constructor]
// * Checks whether specific type has trivial (empty) copy constructor

template<typename T>
struct THasTrivialCopyConstructor { enum { Value = __has_trivial_copy(T) }; };

// [Has trivial copy assign]
// * Checks whether specific type has trivial (empty) copy assign operator

template<typename T>
struct THasTrivialCopyAssign { enum { Value = __has_trivial_assign(T) }; };

// [Has trivial destructor]
// * Checks whether specific type has trivial (empty) destructor

template<typename T>
struct THasTrivialDestructor { enum { Value = __has_trivial_destructor(T) }; };

// [Has virtual destructor]
// * Checks whether specific type has virtual destructor

template<typename T>
struct THasVirtualDestructor { enum { Value = __has_virtual_destructor(T) }; };

// [Is Trivial type]
// * Checks whether specific type is trivial

template<typename T>
struct TIsTrivialType 
{ 
	enum { Value = 
		THasTrivialConstructor<T>::Value && 
		THasTrivialDestructor<T>::Value && 
		THasTrivialCopyConstructor<T>::Value && 
		THasTrivialCopyAssign<T>::Value 
	}; 
};

// [Is copy constructible]
// * Checks whether specific type is copy constructible

template<typename T>
struct TIsCopyConstructible { enum { Value = TIsConstructible<T, const T&>::Value }; };

// [Is move constructible]
// * Checks whether specific type is move constructible

template<typename T>
struct TIsMoveConstructible { enum { Value = TIsConstructible<T, T&&>::Value }; };