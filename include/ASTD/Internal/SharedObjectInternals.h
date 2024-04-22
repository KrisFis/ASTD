// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"

#include "ASTD/Check.h"

namespace NSharedInternals
{
	struct SNullType {};

	class CReferencerBase
	{
	public:
		virtual ~CReferencerBase() = default;

		// Getters
		/////////////////////////////////

		FORCEINLINE bool HasAnyReference() const { return _sharedNum + _weakCount > 0; }
		FORCEINLINE uint16 GetSharedNum() const { return _sharedNum; }
		FORCEINLINE uint16 GetWeakCount() const { return _weakCount; }

		template<typename T>
		FORCEINLINE T* GetObject() const { return reinterpret_cast<T*>(GetObjectImpl()); }

		FORCEINLINE bool HasObject() const { return GetObjectImpl() != nullptr; }

		// Setters [Add]
		/////////////////////////////////

		FORCEINLINE void AddShared()
		{
			CHECK_RET(_sharedNum < UINT16_MAX);
			++_sharedNum;
		}

		FORCEINLINE void AddWeak()
		{
			CHECK_RET(_weakCount < UINT16_MAX); // overflow
			++_weakCount;
		}

		// Setters [REMOVE]
		/////////////////////////////////

		FORCEINLINE void RemoveShared()
		{
			CHECK_RET(_sharedNum > 0); // underflow
			if(_sharedNum == 1) DeconstructObjectImpl();
			--_sharedNum;
		}

		FORCEINLINE void RemoveWeak()
		{
			CHECK_RET(_weakCount > 0); // underflow
			--_weakCount;
		}

	protected: // Children methods

		virtual void* GetObjectImpl() const = 0;
		virtual void DeconstructObjectImpl() = 0;

	protected: // Children fields

		uint16 _sharedNum = 0;
		uint16 _weakCount = 0;
	};

	template<typename T, typename DeleterT = void>
	class TCustomReferencer : public CReferencerBase
	{
	public:
		// Typedefs
		/////////////////////////////////

		typedef T ObjectType;
		typedef DeleterT DeleterType;

		// Constructors
		/////////////////////////////////

		FORCEINLINE TCustomReferencer(ObjectType* InObject, DeleterType InDeleter)
			: CReferencerBase()
			, _object(InObject)
			, _deleter(InDeleter)
		{}
		
		virtual ~TCustomReferencer() override
		{
			if (_object)
			{
				CHECK_RET(_deleter);
				Deleter(_object);
			}
		}
		
	protected: // FReferencer overrides
	
		FORCEINLINE virtual void* GetObjectImpl() const override 
		{ 
			return _object;
		}
		
		FORCEINLINE virtual void DeconstructObjectImpl() override 
		{
			if(_object)
			{
				CHECK_RET(_deleter);
			
				Deleter(_object);
				_object = nullptr;
			}
		}
		
	private: // Fields
	
		ObjectType* _object;
		DeleterType _deleter;
	};
	
	template<typename ObjectType>
	FORCEINLINE static CReferencerBase* NewCustomReferencer(ObjectType* obj)
	{
		return new TCustomReferencer<ObjectType>(obj, [](ObjectType* obj) { delete obj; });
	}
	
	template<typename ObjectType, typename DeleterType>
	FORCEINLINE static CReferencerBase* NewCustomReferencerWithDeleter(ObjectType* obj, DeleterType* delObj)
	{
		return new TCustomReferencer<ObjectType>(obj, delObj);
	}
	
	FORCEINLINE static void DeleteReferencer(CReferencerBase* referencer)
	{
		delete referencer;
	}
	
	// Contains helper methods for referencer
	// * Should be used internally
	// * Handles even deconstruction of referencer
	struct SReferencerProxy
	{
		// Constructors
		/////////////////////////////////

		FORCEINLINE SReferencerProxy(CReferencerBase* InReferencer)
			: _inner(InReferencer)
		{}
	
		// Compare operators
		/////////////////////////////////

		FORCEINLINE bool operator==(const SReferencerProxy& other) const { return _inner == other._inner; }
		FORCEINLINE bool operator!=(const SReferencerProxy& other) const { return !operator==(other); }

		// Pointer operators
		/////////////////////////////////

		FORCEINLINE CReferencerBase* operator->() { return Get(); }
		FORCEINLINE const CReferencerBase* operator->() const { return Get(); }
		
		FORCEINLINE CReferencerBase& operator*() { return *Get(); }
		FORCEINLINE const CReferencerBase& operator*() const { return *Get(); }
	
		// Checkers
		/////////////////////////////////
	
		FORCEINLINE bool IsValid() const { return _inner != nullptr; }
		FORCEINLINE bool IsUnique() const { return _inner != nullptr && _inner->GetSharedNum() == 1; }
		FORCEINLINE bool IsSafeToDereference() const { return _inner != nullptr && _inner->GetSharedNum() > 0; }
	
		// Getters
		/////////////////////////////////
	
		FORCEINLINE CReferencerBase* Get() const { return _inner; }
	
		// Setters
		/////////////////////////////////
	
		FORCEINLINE void Set(CReferencerBase* referencer) { _inner = referencer; }
	
		// Helper methods [Add]
		/////////////////////////////////
	
		FORCEINLINE void AddShared()
		{
			if(!IsValid()) return;
			
			_inner->AddShared();
		}
	
		FORCEINLINE void AddWeak()
		{
			if(!IsValid()) return;
			
			_inner->AddWeak();
		}
	
		// Helper methods [Remove]
		/////////////////////////////////
	
		FORCEINLINE void RemoveShared()
		{
			if(!IsValid()) return;
			
			_inner->RemoveShared();
			if(!_inner->HasAnyReference())
			{
				DeleteReferencer(_inner);
				_inner = nullptr;
			}
		}
		
		FORCEINLINE void RemoveWeak()
		{
			if(!IsValid()) return;
			
			_inner->RemoveWeak();
			if (!_inner->HasAnyReference())
			{
				DeleteReferencer(_inner);
				_inner = nullptr;
			}
		}

	private:

		CReferencerBase* _inner = nullptr;
	};
}
