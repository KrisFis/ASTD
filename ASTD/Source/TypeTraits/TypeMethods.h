
#pragma once

#include "TypeTraits/Internals/TypeTraitsCore.h"

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

// [Decl Val]
// * Converts to a reference type, making it possible to use member functions in decltype expressions without the need to go through constructors.

template<typename T>
typename TRemoveReference<T>::Type&& DeclVal();

// [Size Of]
// * Returns size of specific type
// * Is only wrapper around sizeof(T)

template<typename T>
FORCEINLINE constexpr uint32 SizeOf() { return sizeof(T); }