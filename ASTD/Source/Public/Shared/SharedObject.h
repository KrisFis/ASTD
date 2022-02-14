
#pragma once

#include "Shared/SharedObjectInternals.h"

// Equivalent of std's shared_ptr
template<typename T>
class TSharedPtr
{

public: // Typedefs

	typedef T ObjectType;
	typedef TSharedPtr<T> SharedType;

public: // Constructor

	FORCEINLINE TSharedPtr(NSharedInternals::SNullType* = nullptr) {}
	FORCEINLINE TSharedPtr(NSharedInternals::CReferencerBase& Referencer) : ReferencerProxy(&Referencer) { ReferencerProxy.AddShared(); }

public: // Copy/Move constructors [SharedPtr]
	
	FORCEINLINE TSharedPtr(const SharedType& Other) { ReplaceBy(Other); }
	FORCEINLINE TSharedPtr(SharedType&& Other) noexcept { ReplaceBy(Forward<SharedType>(Other)); }
	
public: // Destructor

	FORCEINLINE ~TSharedPtr() { Reset(); }
	
public: // Cast operators

	template<typename OtherT, typename TEnableIf<TIsDerivedFrom<OtherT, T>::Value>::Type* = nullptr>
	FORCEINLINE operator TSharedPtr<OtherT>() const { return ReferencerProxy.IsValid() ? TSharedPtr<OtherT>(*ReferencerProxy) : nullptr; }
	
	template<typename OtherT, typename TEnableIf<TIsBaseOf<OtherT, T>::Value>::Type* = nullptr>
	FORCEINLINE explicit operator TSharedPtr<OtherT>() const { return ReferencerProxy.IsValid() ? TSharedPtr<OtherT>(*ReferencerProxy) : nullptr; }

public: // Comparison operators [SharedPtr]

	FORCEINLINE bool operator==(const SharedType& Other) const { return ReferencerProxy == Other.ReferencerProxy; }
	FORCEINLINE bool operator!=(const SharedType& Other) const { return !operator==(Other); }

public: // Assignment operators

	FORCEINLINE TSharedPtr& operator=(NSharedInternals::SNullType*) { Reset(); return *this; }

public: // Assignment operators [SharedPtr]

	FORCEINLINE TSharedPtr& operator=(const SharedType& Other) { if(&Other != this) ReplaceBy(Other); return *this; }
	FORCEINLINE TSharedPtr& operator=(SharedType&& Other) { if(&Other != this) ReplaceBy(Forward<SharedType>(Other)); return *this; }

public: // Pointer operators

	FORCEINLINE ObjectType* operator->() const { return Get(); }
	FORCEINLINE ObjectType& operator*() const { ENSURE_THIS(); return *Get(); }

public: // Validation

	FORCEINLINE bool IsValid() const	{ return ReferencerProxy.IsSafeToDereference(); }
	FORCEINLINE bool IsUnique() const { return ReferencerProxy.IsUnique(); }

public: // Getters

	FORCEINLINE ObjectType* Get() const { return ReferencerProxy.IsValid() ? ReferencerProxy->GetObject<ObjectType>() : nullptr; }
	FORCEINLINE ObjectType& GetRef() const { return *Get(); }
	
public: // Other

	FORCEINLINE void Reset() { ReferencerProxy.RemoveShared(); ReferencerProxy.Set(nullptr); }

private: // Helper methods -> Replacing

	// const PtrType&
	// * Copy
	FORCEINLINE_DEBUGGABLE void ReplaceBy(const SharedType& Other)
	{
		// How it should work ? (Copy implementation)
		
		// * SharedPtr as argument:
		// ** 1) Remove shared reference from current
		// ** 2) Add shared reference to other
		// ** 3) Replace referencer
	
		ReferencerProxy.RemoveShared(); // 1
		Other.ReferencerProxy.AddShared(); // 2
		ReferencerProxy = Other.ReferencerProxy; // 3
	}
	
	// PtrType&&
	// * Move
	FORCEINLINE_DEBUGGABLE void ReplaceBy(SharedType&& Other)
	{
		// How it should work ? (move implementation)
		
		// * SharedPtr as argument:
		// ** 1) Remove shared reference from current
		// ** 2) Replace referencer
		// ** 3) Clear other referencer
		
		ReferencerProxy.RemoveShared(); // 1
		ReferencerProxy = Other.ReferencerProxy; // 2
		
		Other.ReferencerProxy.Set(nullptr); // 3
	}
	
private: // Fields

	mutable NSharedInternals::SReferencerProxy ReferencerProxy;

private: // Friends

	template<typename OtherT> friend class TSharedPtr;
	template<typename OtherT> friend class TWeakPtr;
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

	FORCEINLINE TWeakPtr(NSharedInternals::SNullType* = nullptr) {}
	FORCEINLINE TWeakPtr(NSharedInternals::CReferencerBase& Referencer) : ReferencerProxy(&Referencer) { ReferencerProxy.AddWeak();}

public: // Copy/Move constructors [WeakPtr]

	FORCEINLINE TWeakPtr(const WeakType& Other) { ReplaceBy(Other); }
	FORCEINLINE TWeakPtr(WeakType&& Other) noexcept { ReplaceBy(Forward<WeakType>(Other)); }

public: // Copy/Move constructors [SharedPtr]

	FORCEINLINE explicit TWeakPtr(const SharedType& Other) { ReplaceBy(Other); }
	FORCEINLINE explicit TWeakPtr(SharedType&& Other) noexcept { ReplaceBy(Forward<SharedType>(Other)); }

public: // Destructor

	FORCEINLINE ~TWeakPtr() { Reset(); }

public: // Cast operators

	template<typename OtherT, typename TEnableIf<TIsDerivedFrom<OtherT, T>::Value>::Type* = nullptr>
	FORCEINLINE operator TWeakPtr<OtherT>() const { return ReferencerProxy.IsValid() ? TWeakPtr<OtherT>(*ReferencerProxy) : nullptr; }
	
	template<typename OtherT, typename TEnableIf<TIsBaseOf<OtherT, T>::Value>::Type* = nullptr>
	FORCEINLINE explicit operator TWeakPtr<OtherT>() const { return ReferencerProxy.IsValid() ? TWeakPtr<OtherT>(*ReferencerProxy) : nullptr; }

public: // Comparison operators [WeakPtr]

	FORCEINLINE bool operator==(const WeakType& Other) const	{ return ReferencerProxy == Other.ReferencerProxy; }
	FORCEINLINE bool operator!=(const WeakType& Other) const	{ return !operator==(Other); }
	
public: // Comparison operators [SharedPtr]
	
	FORCEINLINE bool operator==(const SharedType& Other) const	{ return ReferencerProxy == Other.ReferencerProxy; }
	FORCEINLINE bool operator!=(const SharedType& Other) const	{ return !operator==(Other); }
	
public: // Assignment operators

	FORCEINLINE TWeakPtr& operator=(const NSharedInternals::SNullType*) { Reset(); }
	
public: // Assignment operators [WeakPtr]

	FORCEINLINE TWeakPtr& operator=(const WeakType& Other) { if (&Other != this) ReplaceBy(Other); return *this; };
	FORCEINLINE TWeakPtr& operator=(WeakType&& Other) { if (&Other != this) ReplaceBy(Forward<WeakType>(Other)); return *this; };
	
public: // Assignment operators [SharedPtr]

	FORCEINLINE TWeakPtr& operator=(const SharedType& Other) { ReplaceBy(Other); return *this; };
	FORCEINLINE TWeakPtr& operator=(SharedType&& Other) { ReplaceBy(Forward<SharedType>(Other)); return *this; };
	
public: // Pointer operators
		// * Our weak pointer supports dereferencing without shared_ptr

	FORCEINLINE ObjectType* operator->() const { return Get(); }
	FORCEINLINE ObjectType& operator*() const { ENSURE_THIS(); return *Get(); }
	
public: // Validity

	FORCEINLINE bool IsValid() const	{ return ReferencerProxy.IsSafeToDereference(); }

public: // Getters

	FORCEINLINE ObjectType* Get() const { return ReferencerProxy.IsValid() ? ReferencerProxy->GetObject<ObjectType>() : nullptr; }
	FORCEINLINE ObjectType& GetRef() const { return *Get(); }
	
public: // Other

	FORCEINLINE void Reset() { ReferencerProxy.RemoveWeak(); ReferencerProxy.Set(nullptr); }

	FORCEINLINE SharedType Pin() const
	{
		if (!IsValid()) return SharedType();
		return SharedType(ReferencerProxy);
	}

private: // Helper methods -> Replacing

	// const PtrType&
	// * Copy
	template<typename PtrType>
	FORCEINLINE_DEBUGGABLE void ReplaceBy(const PtrType& Other)
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
	
		ReferencerProxy.RemoveWeak(); // 1
		Other.ReferencerProxy.AddWeak(); // 2
		ReferencerProxy = Other.ReferencerProxy; // 3
	}
	
	// PtrType&&
	// * Move
	template<typename PtrType>
	FORCEINLINE_DEBUGGABLE void ReplaceBy(PtrType&& Other)
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
			Other.ReferencerProxy.AddWeak(); // -2
			Other.ReferencerProxy.RemoveShared(); // -1
		}
		
		ReferencerProxy.RemoveWeak(); // 1
		ReferencerProxy = Other.ReferencerProxy; // 2
		
		Other.ReferencerProxy.Set(nullptr); // 3
	}

private: // Fields

	mutable NSharedInternals::SReferencerProxy ReferencerProxy;

private: // Friend class

	template<typename OtherT> friend class TSharedPtr;
	template<typename OtherT> friend class TWeakPtr;
};