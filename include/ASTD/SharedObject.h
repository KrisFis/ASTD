// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"

#include "ASTD/Internal/SharedObjectInternals.h"

// Equivalent of std's shared_ptr
template<typename T>
class TSharedPtr
{
	template<typename OtherT> friend class TSharedPtr;
	template<typename OtherT> friend class TWeakPtr;

public:

	// Typedefs
	/////////////////////////////////

	typedef T ObjectType;

	// Constructor
	/////////////////////////////////

	FORCEINLINE TSharedPtr(NSharedInternals::SNullType* = nullptr) {}
	FORCEINLINE TSharedPtr(NSharedInternals::CReferencerBase& ref) : _referencerProxy(&ref) { _referencerProxy.AddShared(); }

	// Copy/Move constructors [SharedPtr]
	/////////////////////////////////
	
	FORCEINLINE TSharedPtr(const TSharedPtr& other) { ReplaceBy(other); }
	FORCEINLINE TSharedPtr(TSharedPtr&& other) { ReplaceBy(Forward<TSharedPtr>(other)); }
	
	// Destructor
	/////////////////////////////////

	FORCEINLINE ~TSharedPtr() { Reset(); }
	
	// Cast operators
	/////////////////////////////////

	template<typename OtherT, typename TEnableIf<TIsDerivedFrom<OtherT, T>::Value>::Type* = nullptr>
	FORCEINLINE operator TSharedPtr<OtherT>() const { return _referencerProxy.IsValid() ? TSharedPtr<OtherT>(*_referencerProxy) : nullptr; }
	
	template<typename OtherT, typename TEnableIf<TIsBaseOf<OtherT, T>::Value>::Type* = nullptr>
	FORCEINLINE explicit operator TSharedPtr<OtherT>() const { return _referencerProxy.IsValid() ? TSharedPtr<OtherT>(*_referencerProxy) : nullptr; }

	// Comparison operators [SharedPtr]
	/////////////////////////////////

	FORCEINLINE bool operator==(const TSharedPtr& other) const { return _referencerProxy == other._referencerProxy; }
	FORCEINLINE bool operator!=(const TSharedPtr& other) const { return !operator==(other); }

	// Assignment operators
	/////////////////////////////////

	FORCEINLINE TSharedPtr& operator=(NSharedInternals::SNullType*) { Reset(); return *this; }

	// Assignment operators [SharedPtr]
	/////////////////////////////////

	FORCEINLINE TSharedPtr& operator=(const TSharedPtr& Other) { if(&Other != this) ReplaceBy(Other); return *this; }
	FORCEINLINE TSharedPtr& operator=(TSharedPtr&& Other) { if(&Other != this) ReplaceBy(Forward<TSharedPtr>(Other)); return *this; }

	// Pointer operators
	/////////////////////////////////

	FORCEINLINE ObjectType* operator->() const { return Get(); }
	FORCEINLINE ObjectType& operator*() const { return *Get(); }

	// Validation
	/////////////////////////////////

	FORCEINLINE bool IsValid() const { return _referencerProxy.IsSafeToDereference(); }
	FORCEINLINE bool IsUnique() const { return _referencerProxy.IsUnique(); }

	// Getters
	/////////////////////////////////

	FORCEINLINE ObjectType* Get() const { return _referencerProxy.IsValid() ? _referencerProxy->GetObject<ObjectType>() : nullptr; }
	FORCEINLINE ObjectType& GetRef() const { return *Get(); }

	// Other
	/////////////////////////////////

	FORCEINLINE void Reset() { _referencerProxy.RemoveShared(); _referencerProxy.Set(nullptr); }

private:

	// const PtrType&
	// * Copy
	FORCEINLINE_DEBUGGABLE void ReplaceBy(const TSharedPtr& other)
	{
		// How it should work ? (Copy implementation)
		
		// * SharedPtr as argument:
		// ** 1) Remove shared reference from current
		// ** 2) Add shared reference to other
		// ** 3) Replace referencer
	
		_referencerProxy.RemoveShared(); // 1
		other._referencerProxy.AddShared(); // 2
		_referencerProxy = other._referencerProxy; // 3
	}
	
	// PtrType&&
	// * Move
	FORCEINLINE_DEBUGGABLE void ReplaceBy(TSharedPtr&& other)
	{
		// How it should work ? (move implementation)
		
		// * SharedPtr as argument:
		// ** 1) Remove shared reference from current
		// ** 2) Replace referencer
		// ** 3) Clear other referencer
		
		_referencerProxy.RemoveShared(); // 1
		_referencerProxy = other._referencerProxy; // 2
		
		other._referencerProxy.Set(nullptr); // 3
	}
	
	mutable NSharedInternals::SReferencerProxy _referencerProxy;
};

// Equivalent of std's weak_ptr
template<typename T>
class TWeakPtr
{
	template<typename OtherT> friend class TSharedPtr;
	template<typename OtherT> friend class TWeakPtr;

public:

	// Typedefs
	/////////////////////////////////

	typedef T ObjectType;

	// Constructors
	/////////////////////////////////

	FORCEINLINE TWeakPtr(NSharedInternals::SNullType* = nullptr) {}
	FORCEINLINE TWeakPtr(NSharedInternals::CReferencerBase& ref) : _referencerProxy(&ref) { _referencerProxy.AddWeak();}

	// Copy/Move constructors [WeakPtr]
	/////////////////////////////////

	FORCEINLINE TWeakPtr(const TWeakPtr& other) { ReplaceBy(other); }
	FORCEINLINE TWeakPtr(TWeakPtr&& other) noexcept { ReplaceBy(Forward<TWeakPtr>(other)); }

	// Copy/Move constructors [SharedPtr]
	/////////////////////////////////

	FORCEINLINE explicit TWeakPtr(const TSharedPtr<T>& other) { ReplaceBy(other); }
	FORCEINLINE explicit TWeakPtr(TSharedPtr<T>&& other) noexcept { ReplaceBy(Forward<TSharedPtr<T>>(other)); }

	// Destructor
	/////////////////////////////////

	FORCEINLINE ~TWeakPtr() { Reset(); }

	// Cast operators
	/////////////////////////////////

	template<typename OtherT, typename TEnableIf<TIsDerivedFrom<OtherT, T>::Value>::Type* = nullptr>
	FORCEINLINE operator TWeakPtr<OtherT>() const { return _referencerProxy.IsValid() ? TWeakPtr<OtherT>(*_referencerProxy) : nullptr; }
	
	template<typename OtherT, typename TEnableIf<TIsBaseOf<OtherT, T>::Value>::Type* = nullptr>
	FORCEINLINE explicit operator TWeakPtr<OtherT>() const { return _referencerProxy.IsValid() ? TWeakPtr<OtherT>(*_referencerProxy) : nullptr; }

	// Comparison operators [WeakPtr]
	/////////////////////////////////

	FORCEINLINE bool operator==(const TWeakPtr& other) const	{ return _referencerProxy == other._referencerProxy; }
	FORCEINLINE bool operator!=(const TWeakPtr& other) const	{ return !operator==(other); }
	
	// Comparison operators [SharedPtr]
	/////////////////////////////////
	
	FORCEINLINE bool operator==(const TSharedPtr<T>& other) const	{ return _referencerProxy == other._referencerProxy; }
	FORCEINLINE bool operator!=(const TSharedPtr<T>& other) const	{ return !operator==(other); }
	
	// Assignment operators
	/////////////////////////////////

	FORCEINLINE TWeakPtr& operator=(const NSharedInternals::SNullType*) { Reset(); return *this; }
	
	// Assignment operators [WeakPtr]
	/////////////////////////////////

	FORCEINLINE TWeakPtr& operator=(const TWeakPtr& other) { if (&other != this) ReplaceBy(other); return *this; }
	FORCEINLINE TWeakPtr& operator=(TWeakPtr&& other) { if (&other != this) ReplaceBy(Forward<TWeakPtr>(other)); return *this; }
	
	// Assignment operators [SharedPtr]
	/////////////////////////////////

	FORCEINLINE TWeakPtr& operator=(const TSharedPtr<T>& other) { ReplaceBy(other); return *this; }
	FORCEINLINE TWeakPtr& operator=(TSharedPtr<T>&& other) { ReplaceBy(Forward<TSharedPtr<T>>(other)); return *this; }
	
	// Pointer operators
	/////////////////////////////////
	// * Our weak pointer supports dereferencing without shared_ptr

	FORCEINLINE ObjectType* operator->() const { return Get(); }
	FORCEINLINE ObjectType& operator*() const { return *Get(); }
	
	// Validity
	/////////////////////////////////

	FORCEINLINE bool IsValid() const { return _referencerProxy.IsSafeToDereference(); }

	// Getters
	/////////////////////////////////

	FORCEINLINE ObjectType* Get() const { return _referencerProxy.IsValid() ? _referencerProxy->GetObject<ObjectType>() : nullptr; }
	FORCEINLINE ObjectType& GetRef() const { return *Get(); }
	
	// Other
	/////////////////////////////////

	FORCEINLINE void Reset() { _referencerProxy.RemoveWeak(); _referencerProxy.Set(nullptr); }

	FORCEINLINE TSharedPtr<T> Pin() const { return IsValid() ? TSharedPtr<T>(_referencerProxy) : TSharedPtr<T>(); }

private:

	// const PtrType&
	// * Copy
	template<typename PtrType>
	FORCEINLINE_DEBUGGABLE void ReplaceBy(const PtrType& other)
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
	
		_referencerProxy.RemoveWeak(); // 1
		other._referencerProxy.AddWeak(); // 2
		_referencerProxy = other._referencerProxy; // 3
	}
	
	// PtrType&&
	// * Move
	template<typename PtrType>
	FORCEINLINE_DEBUGGABLE void ReplaceBy(PtrType&& other)
	{
		// How it should work ? (move implementation)
		
		// * WeakPtr as argument:
		// ** 1) Remove weak reference from current
		// ** 2) Replace referencer
		// ** 3) Clear other referencer
		
		// * SharedPtr as argument:
		// ** We MUST CHECK that reference of other does not get destroyed (if valid before passing)
		// ** -2) Add weak reference to other
		// ** -1) Remove shared reference from other
		// ** 1) Remove weak reference from current
		// ** 2) Replace referencer
		// ** 3) Clear other referencer
		
		// * Only SharedPtr
		if(TIsSame<typename TRemoveReference<PtrType>::Type, TSharedPtr<T>>::Value)
		{
			other._referencerProxy.AddWeak(); // -2
			other._referencerProxy.RemoveShared(); // -1
		}
		
		_referencerProxy.RemoveWeak(); // 1
		_referencerProxy = other._referencerProxy; // 2
		
		other._referencerProxy.Set(nullptr); // 3
	}

	mutable NSharedInternals::SReferencerProxy _referencerProxy;
};
