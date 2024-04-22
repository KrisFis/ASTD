// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"

#include "ASTD/Internal/TypeTraitsCore.h"

// [Move]
// * Indicates that an specific object should be "moved"

template<typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& Move(T& obj) { return static_cast<typename TRemoveReference<T>::Type&&>(obj); }

template<typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& Move(T&& obj) { return static_cast<typename TRemoveReference<T>::Type&&>(obj); }

// [Forward]
// * "Forwards" object's "passed by" type

template<typename T>
FORCEINLINE typename TRemoveReference<T>::Type& Forward(T& obj) { return obj; }

template<typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& Forward(T&& obj) { return obj; }

// [Decl Val]
// * Converts to a reference type, making it possible to use member functions in decltype expressions without the need to go through constructors.

template<typename T>
typename TRemoveReference<T>::Type&& DeclVal();
