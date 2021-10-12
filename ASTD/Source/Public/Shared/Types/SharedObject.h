
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

public: // Constructor

	inline TSharedPtr(NSharedInternals::FNullType* = nullptr)
		: Referencer(nullptr)
	{}

	template<typename WeakType, typename = TEnableIf<TIsSame<WeakType, TWeakPtr<ObjectType>>::Value>>
	inline explicit TSharedPtr(const WeakType& WeakPtr)
		: Referencer(WeakPtr.Referencer)
	{
		AddReference();
	}
	
public: // Destructor

	~TSharedPtr()
	{
		Reset();
	}

public: // Copy/Move constructors [SharedPtr]

	inline TSharedPtr(const TSharedPtr& Other)
		: Referencer(Other.Referencer)
	{
		AddReference();
	}

	inline TSharedPtr(TSharedPtr&& Other)
		: Referencer(Other.Referencer)
	{
		Other.Referencer = nullptr;
	}

public: // Operators

	inline bool operator==(const TSharedPtr& Other) const { return Referencer == Other.Referencer; }
	inline bool operator!=(const TSharedPtr& Other) const { return !operator==(Other); }

	inline TSharedPtr& operator=(const TSharedPtr& Other)
	{
		if (this == &Other) return *this;

		RemoveReference();
		Referencer = Other.Referencer;
		if (Referencer) Referencer->AddShared();
		return *this;
	}

	inline TSharedPtr& operator=(TSharedPtr&& Other) noexcept
	{
		if (this == &Other) return *this;

		RemoveReference();
		Referencer = Other.Referencer;
		Other.Referencer = nullptr;
		return *this;
	}

	inline TSharedPtr& operator=(NSharedInternals::FNullType*) { Reset(); }
	
	inline const ObjectType* operator->() const	{ return Get(); }
	inline ObjectType* operator->()	{ return Get(); }
	inline const ObjectType& operator*() const { ENSURE_THIS(); return *Get(); }
	inline ObjectType& operator*() { ENSURE_THIS(); return *Get(); }

public: // Validation

	inline bool IsValid() const { return Referencer && Referencer->GetSharedNum() > 0; }
	inline bool IsUnique() const { return Referencer && Referencer->GetSharedNum() == 1; }

public: // Getters

	inline const ObjectType* Get() const { return Referencer ? Referencer->GetObject<ObjectType>() : nullptr; }
	inline ObjectType* Get() { return Referencer ? Referencer->GetObject<ObjectType>() : nullptr; }
	
public: // Other

	inline void Reset() 
	{ 
		RemoveReference(); 
		Referencer = nullptr;
	}

private: // Helper methods

	inline void AddReference()
	{
		if(Referencer)
			Referencer->AddShared();
	}

	inline void RemoveReference()
	{
		if(Referencer)
		{
			Referencer->RemoveShared();
			if(!Referencer->HasAnyReference())
			{
				NSharedInternals::DeleteReferencer(Referencer);
				Referencer = nullptr;
			}
		}
	}

private: // Fields

	NSharedInternals::FReferencerBase* Referencer;

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

public: // Constructors

	inline TWeakPtr(NSharedInternals::FNullType* = nullptr)
			: Referencer(nullptr)
	{}

	template<typename SharedType, typename = TEnableIf<TIsSame<SharedType, TSharedPtr<ObjectType>>::Value>>
	inline explicit TWeakPtr(const SharedType& SharedPtr)
			: Referencer(SharedPtr.Referencer)
	{
		AddReference();
	}

public: // Copy/Move constructors [WeakPtr]

	inline TWeakPtr(const TWeakPtr& Other)
			: Referencer(Other.Referencer)
	{
		AddReference();
	}

	inline TWeakPtr(TWeakPtr&& Other) noexcept
			: Referencer(Other.Referencer)
	{
		Other.Referencer = nullptr;
	}

public: // Destructor

	inline ~TWeakPtr()
	{
		Reset();
	}

public: // Operators

	inline bool operator==(const TWeakPtr& Other) const	{ return Referencer == Other.Referencer; }
	inline bool operator!=(const TWeakPtr& Other) const	{ return !operator==(Other); }
	
	inline TWeakPtr& operator=(const NSharedInternals::FNullType*) { Reset(); }
	
	inline TWeakPtr& operator=(const TWeakPtr& Other)
	{
		if (&Other == this) return *this;

		RemoveReference();
		Referencer = Other.Referencer;
		if (Referencer) Referencer->AddWeak();

		return *this;
	}

	inline TWeakPtr& operator=(TWeakPtr&& Other)
	{
		if (&Other == this) return *this;

		RemoveReference();
		Referencer = Other.Referencer;
		Other.Referencer = nullptr;
		return *this;
	}

public: // Validity

	inline bool IsValid() const	{ return Referencer && (Referencer->GetSharedNum() > 0); }

public: // Other

	inline void Reset()
	{
		RemoveReference();
	}

	inline TSharedPtr<ObjectType> Pin() const
	{
		if (!IsValid()) return nullptr;
		return TSharedPtr<ObjectType>(*this);
	}

private: // Helper methods

	inline void AddReference()
	{
		if(Referencer)
		{
			Referencer->AddWeak();
		}
	}

	inline void RemoveReference()
	{
		if (Referencer)
		{
			Referencer->RemoveWeak();
			if (!Referencer->HasAnyReference())
				NSharedInternals::DeleteReferencer(Referencer);
		}
	}

private: // Fields

	NSharedInternals::FReferencerBase* Referencer;

private: // Friend class

	template<typename OtherT> friend class TSharedPtr;
	template<typename OtherT> friend class TWeakPtr;
};