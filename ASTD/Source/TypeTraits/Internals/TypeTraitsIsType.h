
#pragma once

// [Is abstract type]
// * Checks whether specific type is abstract

template<typename T>
struct TIsAbstractType { enum { Value = __is_abstract(T) }; };

// [Is POD type]
// * Checks whether specific type is POD (C-lang compatible type)

template<typename T>
struct TIsPODType { enum {Value = __is_pod(T) }; };

// [Is union type]
// * Checks whether specific type is union

template<typename T>
struct TIsUnionType { enum {Value = __is_union(T) }; };
