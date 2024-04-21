// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Type/Internals/TypeTraitsCore.h"

// [Move]
// * Indicates that an specific object may be "moved from"

template<typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& Move(T& obj) { return static_cast<typename TRemoveReference<T>::Type&&>(obj); }

template<typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& Move(T&& obj) { return static_cast<typename TRemoveReference<T>::Type&&>(obj); }

// [Forward]
// * Passed specific object with his type provided

template<typename T>
FORCEINLINE typename TRemoveReference<T>::Type& Forward(T& obj) { return obj; }

template<typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& Forward(T&& obj) { return obj; }

// [Decl Val]
// * Converts to a reference type, making it possible to use member functions in decltype expressions without the need to go through constructors.

template<typename T>
typename TRemoveReference<T>::Type&& DeclVal();
