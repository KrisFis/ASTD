
#pragma once

#include "IntegerTypes.h"
#include "SharedInternals.h"

#include <assert.h>

// Forward declarations
template<typename T>
class TSharedPtr;

template<typename T>
class TWeakPtr;

// Equivalent of std's shared_ptr
template<typename T>
class TSharedPtr
{

public: // Typedefs

	typedef T ObjectType;
	typedef TSharedPtr<T> SharedType;

public: // Constructor

	inline TSharedPtr(NSharedInternals::FNullType* = nullptr) {}
	inline explicit TSharedPtr(NSharedInternals::FReferencerBase* InReferencer) : Referencer(InReferencer) { Referencer.AddShared(); }

public: // Copy/Move constructors [SharedPtr]
	
	inline TSharedPtr(const SharedType& Other) { ReplaceBy(Other); }
	inline TSharedPtr(SharedType&& Other) noexcept { ReplaceBy(Forward<SharedType>(Other)); }
	
public: // Destructor

	~TSharedPtr() { Reset(); }
	
public: // Comparison operators [SharedPtr]

	inline bool operator==(const SharedType& Other) const { return Referencer.Get() == Other.Referencer.Get(); }
	inline bool operator!=(const SharedType& Other) const { return !operator==(Other); }

public: // Assignment operators

	inline TSharedPtr& operator=(NSharedInternals::FNullType*) { Reset(); return *this; }

public: // Assignment operators [SharedPtr]

	inline TSharedPtr& operator=(const SharedType& Other) { if(&Other != this) ReplaceBy(Other); return *this; }
	inline TSharedPtr& operator=(SharedType&& Other) { if(&Other != this) ReplaceBy(Forward<SharedType>(Other)); return *this; }

public: // Pointer operators

	inline ObjectType* operator->()	{ return Get(); }
	inline const ObjectType* operator->() const	{ return Get(); }
	
	inline ObjectType& operator*() { ENSURE_THIS(); return *Get(); }
	inline const ObjectType& operator*() const { ENSURE_THIS(); return *Get(); }

public: // Validation

	inline bool IsValid() const	{ return Referencer.IsSafeToDereference(); }
	inline bool IsUnique() const { return Referencer.IsUnique(); }

public: // Getters

	inline const ObjectType* Get() const { return Referencer.IsValid() ? Referencer->GetObject<ObjectType>() : nullptr; }
	inline ObjectType* Get() { return Referencer.IsValid() ? Referencer->GetObject<ObjectType>() : nullptr; }
	
public: // Other

	inline void Reset() { Referencer.RemoveShared(); Referencer.Set(nullptr); }

private: // Helper methods -> Replacing

	// const PtrType&
	// * Copy
	inline void ReplaceBy(const SharedType& Other)
	{
		// How it should work ? (Copy implementation)
		
		// * SharedPtr as argument:
		// ** 1) Remove shared reference from current
		// ** 2) Add shared reference to other
		// ** 3) Replace referencer
	
		Referencer.RemoveShared(); // 1
		Other.Referencer.AddShared(); // 2
		Referencer = Other.Referencer; // 3
	}
	
	// PtrType&&
	// * Move
	inline void ReplaceBy(SharedType&& Other)
	{
		// How it should work ? (move implementation)
		
		// * SharedPtr as argument:
		// ** 1) Remove shared reference from current
		// ** 2) Replace referencer
		// ** 3) Clear other referencer
		
		Referencer.RemoveShared(); // 1
		Referencer = Other.Referencer; // 2
		
		Other.Referencer.Set(nullptr); // 3
	}
	
private: // Fields

	mutable NSharedInternals::FReferencerProxy Referencer;

private: // Friends

	template<typename OtherT> friend class TSharedPtr;
	template<typename OtherT> friend class TWeakPtr;

	template<typename R, typename... ArgTypes>
	friend TSharedPtr<R> MakeShared(ArgTypes&&... Args);
	
	template<typename R, typename W>
	friend TSharedPtr<W> MakeShareable(R* Instance);

	template<typename R, typename W>
	friend TSharedPtr<R> CastShared(const TSharedPtr<W>& From);
};

// Equivalent of std's weak_ptr
template<typename T>
class TWeakPtr
{

public: // Typedefs

	typedef T ObjectType;
	typedef TWeakPtr<T> WeakType;
	typedef TSharedPtr<T> SharedType;

public: // Constructors

	inline TWeakPtr(NSharedInternals::FNullType* = nullptr) {}

public: // Copy/Move constructors [WeakPtr]

	inline TWeakPtr(const WeakType& Other) { ReplaceBy(Other); }
	inline TWeakPtr(WeakType&& Other) noexcept { ReplaceBy(Forward<WeakType>(Other)); }

public: // Copy/Move constructors [SharedPtr]

	inline explicit TWeakPtr(const SharedType& Other) { ReplaceBy(Other); }
	inline explicit TWeakPtr(SharedType&& Other) noexcept { ReplaceBy(Forward<SharedType>(Other)); }

public: // Destructor

	inline ~TWeakPtr() { Reset(); }

public: // Comparison operators [WeakPtr]

	inline bool operator==(const WeakType& Other) const	{ return Referencer.Get() == Other.Referencer.Get(); }
	inline bool operator!=(const WeakType& Other) const	{ return !operator==(Other); }
	
public: // Comparison operators [SharedPtr]
	
	inline bool operator==(const SharedType& Other) const	{ return Referencer.Get() == Other.Referencer.Get(); }
	inline bool operator!=(const SharedType& Other) const	{ return !operator==(Other); }
	
public: // Assignment operators

	inline TWeakPtr& operator=(const NSharedInternals::FNullType*) { Reset(); }
	
public: // Assignment operators [WeakPtr]

	inline TWeakPtr& operator=(const WeakType& Other) { if (&Other != this) ReplaceBy(Other); return *this; };
	inline TWeakPtr& operator=(WeakType&& Other) { if (&Other != this) ReplaceBy(Forward<WeakType>(Other)); return *this; };
	
public: // Assignment operators [SharedPtr]

	inline TWeakPtr& operator=(const SharedType& Other) { ReplaceBy(Other); return *this; };
	inline TWeakPtr& operator=(SharedType&& Other) { ReplaceBy(Forward<SharedType>(Other)); return *this; };
	
public: // Pointer operators
		// * Our weak pointer supports dereferencing without shared_ptr

	inline ObjectType* operator->()	{ return Get(); }
	inline const ObjectType* operator->() const	{ return Get(); }
	
	inline ObjectType& operator*() { ENSURE_THIS(); return *Get(); }
	inline const ObjectType& operator*() const { ENSURE_THIS(); return *Get(); }
	
public: // Validity

	inline bool IsValid() const	{ return Referencer.IsSafeToDereference(); }

public: // Getters

	inline const ObjectType* Get() const { return Referencer.IsValid() ? Referencer->GetObject<ObjectType>() : nullptr; }
	inline ObjectType* Get() { return Referencer.IsValid() ? Referencer->GetObject<ObjectType>() : nullptr; }
	
public: // Other

	inline void Reset() { Referencer.RemoveWeak(); Referencer.Set(nullptr); }

	inline SharedType Pin() const
	{
		if (!IsValid()) return SharedType();
		return SharedType(Referencer.Get());
	}

private: // Helper methods -> Replacing

	// const PtrType&
	// * Copy
	template<typename PtrType>
	inline void ReplaceBy(const PtrType& Other)
	{
		// How it should work ? (Copy implementation)
		
		// * WeakPtr as argument:
		// ** 1) Remove weak reference from current
		// ** 2) Add weak reference to other
		// ** 3) Replace referencer
		
		// * SharedPtr as argument:
		// ** 1) Remove weak reference from current
		// ** 2) Add weak reference to other
		// ** 3) Replace referencer
	
		Referencer.RemoveWeak(); // 1
		Other.Referencer.AddWeak(); // 2
		Referencer = Other.Referencer; // 3
	}
	
	// PtrType&&
	// * Move
	template<typename PtrType>
	inline void ReplaceBy(PtrType&& Other)
	{
		// How it should work ? (move implementation)
		
		// * WeakPtr as argument:
		// ** 1) Remove weak reference from current
		// ** 2) Replace referencer
		// ** 3) Clear other referencer
		
		// * SharedPtr as argument:
		// ** We MUST ensure that reference of other does not get destroyed (if valid before passing)
		// ** -2) Add weak reference to other
		// ** -1) Remove shared reference from other
		// ** 1) Remove weak reference from current
		// ** 2) Replace referencer
		// ** 3) Clear other referencer
		
		// * Only SharedPtr
		if(TIsSame<typename TRemoveReference<PtrType>::Type, SharedType>::Value)
		{
			Other.Referencer.AddWeak(); // -2
			Other.Referencer.RemoveShared(); // -1
		}
		
		Referencer.RemoveWeak(); // 1
		Referencer = Other.Referencer; // 2
		
		Other.Referencer.Set(nullptr); // 3
	}

private: // Fields

	mutable NSharedInternals::FReferencerProxy Referencer;

private: // Friend class

	template<typename OtherT> friend class TSharedPtr;
	template<typename OtherT> friend class TWeakPtr;
};