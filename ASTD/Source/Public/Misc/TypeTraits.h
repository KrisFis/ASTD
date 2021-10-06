
#pragma once

// [Enable if]
// * Enables compilation of specific template function/struct when condition met

template<bool T, typename R = void> 
struct TEnableIf;

template<typename R> struct TEnableIf<true, R> { typedef R Result; };
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
// * Checks whether provided type is const

template<typename T> struct TIsConst { enum { Value = false }; };
template<typename T> struct TIsConst<const T> { enum { Value = true }; };

// [Remove Const]
// * Removes "const" from type

template<typename T> struct TRemoveConst { typedef T Type; };
template<typename T> struct TRemoveConst<const T> { typedef T Type; };

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

// [Is Derived]
// * Checks whether specific type is derived from other type

template<typename DerivedType, typename BaseType>
struct TIsDerivedFrom
{

private: // Typedefs

	typedef char No[1];
	typedef char Yes[2];
	
private: // Testing

	static Yes& Test(BaseType*);
	static No& Test(...);

	static DerivedType* ChildPtr() { return nullptr; }

public: // Value
	
	enum { Value = sizeof(Test(ChildPtr())) == sizeof(Yes) };
};

// [Is Same]
// * Checks whether specified types are the same

template<typename T, typename R> struct TIsSame { enum { Value = false }; };
template<typename T> struct TIsSame<T, T> { enum { Value = true }; };

// [Is Castable]
// * Checks whether specific types could be casted to each other

template<typename T, typename R>
struct TIsCastable { enum { Value = TIsDerivedFrom<T, R>::Value || TIsDerivedFrom<R, T>::Value }; };

// [Move]
// * Indicates that an specific object may be "moved from"

template<typename T>
typename TRemoveReference<T>::Type&& Move(T&& Obj) { return static_cast<typename TRemoveReference<T>::Type&&>(Obj); }

// [Forward]
// * Passed specific object with his type provided

template<typename T>
typename TRemoveReference<T>::Type& Forward(T& Obj) { return Obj; }

template<typename T>
typename TRemoveReference<T>::Type&& Forward(T&& Obj) { return Obj; }