
#pragma once

// [Is constructible]
// * Checks whether specific type is constructible from specific types

template<typename T, typename... ArgTypes>
struct TIsConstructible { enum { Value = __is_constructible(T, ArgTypes...) }; };

// [Is copy constructible]
// * Checks whether specific type is copy constructible

template<typename T>
struct TIsCopyConstructible { enum { Value = TIsConstructible<T, const T&>::Value }; };

// [Is move constructible]
// * Checks whether specific type is move constructible

template<typename T>
struct TIsMoveConstructible { enum { Value = TIsConstructible<T, T&&>::Value }; };

// Trivial type
////////////////////////////////////////////////////////////////

// [Is trivially constructible]
// * Checks whether specific type is trivially constructible from specific types

template<typename T, typename... ArgTypes>
struct TIsTriviallyConstructible { enum { Value = __is_trivially_constructible(T, ArgTypes...) }; };

// [Is trivially destructible]
// * Checks whether specific type has trivial (empty) destructor

template<typename T>
struct TIsTriviallyDestructible { enum { Value = __has_trivial_destructor(T) }; };

// [Is trivially copy constructible]
// * Checks whether specific type has trivial (empty) copy constructor

template<typename T>
struct TIsTriviallyCopyConstructible { enum { Value = TIsTriviallyConstructible<T, const T&>::Value }; };

// [Is trivially move constructible]
// * Checks whether specific type has trivial (empty) move constructor

template<typename T>
struct TIsTriviallyMoveConstructible { enum { Value = TIsTriviallyConstructible<T, T&&>::Value }; };

// [Is trivial type]
// * Checks whether specific type is trivial

template<typename T>
struct TIsTrivialType
{ 
	enum { Value = 
		TIsTriviallyConstructible<T>::Value &&
		TIsTriviallyDestructible<T>::Value &&
		TIsTriviallyCopyConstructible<T>::Value &&
		TIsTriviallyMoveConstructible<T>::Value
	}; 
};

// Virtual
////////////////////////////////////////////////////////////////

// [Has virtual destructor]
// * Checks whether specific type has virtual destructor

template<typename T>
struct THasVirtualDestructor { enum { Value = __has_virtual_destructor(T) }; };