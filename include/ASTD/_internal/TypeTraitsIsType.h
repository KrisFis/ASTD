// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Build.h"

// [Is abstract type]
// * Checks whether specific type is abstract

template<typename T>
struct TIsAbstractType { enum { Value = __is_abstract(T) }; };

// [Is class type]
// * Checks whether specific type is class

template<typename T>
struct TIsClassType { enum { Value = __is_class(T) }; };

// [Is empty type]
// * Checks whether specific type is empty
// ** has no non-static members/fields and if so, then each is bit-fields of zero length
// ** this check is applied to each base class, if everything passes with true, then result is true

template<typename T>
struct TIsEmptyType { enum { Value = __is_empty(T) }; };

// [Is POD type]
// * Checks whether specific type is POD (C-lang compatible type)

template<typename T>
struct TIsPODType { enum { Value = __is_pod(T) }; };

// [Is union type]
// * Checks whether specific type is union

template<typename T>
struct TIsUnionType { enum { Value = __is_union(T) }; };
