
#pragma once

#include "Core/Types.h"
#include "Validation/Check.h"

namespace NSharedInternals
{
	struct SNullType {};
	
	class CReferencerBase
	{
	public: // Constructors
		
		FORCEINLINE CReferencerBase()
			: SharedCount(0)
			, WeakCount(0)
		{}
		
		virtual ~CReferencerBase() = default;
		
	public: // Getters
	
		FORCEINLINE bool HasAnyReference() const { return SharedCount + WeakCount > 0; }
		FORCEINLINE uint16 GetSharedNum() const { return SharedCount; }
		FORCEINLINE uint16 GetWeakCount() const { return WeakCount; }
		
		template<typename T>
		FORCEINLINE T* GetObject() const { return reinterpret_cast<T*>(GetObjectImpl()); }
		
		FORCEINLINE bool HasObject() const { return GetObjectImpl() != nullptr; }
		
	public: // Setters [Add]
	
		FORCEINLINE void AddShared() 
		{
			CHECK_RET(SharedCount < UINT16_MAX);
			++SharedCount;
		}
		
		FORCEINLINE void AddWeak() 
		{
			CHECK_RET(WeakCount < UINT16_MAX); // overflow
			++WeakCount;
		}
		
	public: // Setters [REMOVE]
	
		FORCEINLINE void RemoveShared()
		{
			CHECK_RET(SharedCount > 0); // underflow
			if(SharedCount == 1) DeconstructObjectImpl();
			--SharedCount;
		}
		
		FORCEINLINE void RemoveWeak()
		{
			CHECK_RET(WeakCount > 0); // underflow
			--WeakCount;
		}
		
	protected: // Children methods
	
		virtual void* GetObjectImpl() const = 0;
		virtual void DeconstructObjectImpl() = 0;
		
	protected: // Children fields
	
		uint16 SharedCount;
		uint16 WeakCount;
	};
	
	template<typename T, typename DeleterT = void>
	class TCustomReferencer : public CReferencerBase
	{
	public: // Typedefs
	
		typedef T ObjectType;
		typedef DeleterT DeleterType;
		
	public: // Constructors
	
		FORCEINLINE TCustomReferencer(ObjectType* InObject, DeleterType InDeleter)
			: CReferencerBase()
			, Object(InObject)
			, Deleter(InDeleter)
		{}
		
		virtual ~TCustomReferencer() override
		{
			if (Object)
			{
				CHECK_RET(Deleter);
				Deleter(Object);
			}
		}
		
	protected: // FReferencer overrides
	
		FORCEINLINE virtual void* GetObjectImpl() const override 
		{ 
			return Object;
		}
		
		FORCEINLINE virtual void DeconstructObjectImpl() override 
		{
			if(Object)
			{
				CHECK_RET(Deleter);
			
				Deleter(Object);
				Object = nullptr;
			}
		}
		
	private: // Fields
	
		ObjectType* Object;
		DeleterType Deleter;
	};
	
	template<typename ObjectType>
	FORCEINLINE static CReferencerBase* NewCustomReferencer(ObjectType* Object)
	{
		return new TCustomReferencer<ObjectType>(Object, [](ObjectType* ToDelete) { delete ToDelete; });
	}
	
	template<typename ObjectType, typename DeleterType>
	FORCEINLINE static CReferencerBase* NewCustomReferencerWithDeleter(ObjectType* Object, DeleterType* Deleter)
	{
		return new TCustomReferencer<ObjectType>(Object, Deleter);
	}
	
	FORCEINLINE static void DeleteReferencer(CReferencerBase* Referencer)
	{
		delete Referencer;
	}
	
	// Contains helper methods for referencer
	// * Should be used internally
	// * Handles even deconstruction of referencer
	struct SReferencerProxy
	{
	
	public: // Constructors
	
		FORCEINLINE SReferencerProxy()
			: Inner(nullptr)
		{}
	
		FORCEINLINE SReferencerProxy(CReferencerBase* InReferencer)
			: Inner(InReferencer)
		{}
	
	public: // Compare operators

		FORCEINLINE bool operator==(const SReferencerProxy& Other) const { return Inner == Other.Inner; }
		FORCEINLINE bool operator!=(const SReferencerProxy& Other) const { return !operator==(Other); }

	public: // Pointer operators

		FORCEINLINE CReferencerBase* operator->() { return Get(); }
		FORCEINLINE const CReferencerBase* operator->() const { return Get(); }
		
		FORCEINLINE CReferencerBase& operator*() { return *Get(); }
		FORCEINLINE const CReferencerBase& operator*() const { return *Get(); }
	
	public: // Checkers
	
		FORCEINLINE bool IsValid() const { return Inner != nullptr; }
		FORCEINLINE bool IsUnique() const { return Inner != nullptr && Inner->GetSharedNum() == 1; }
		FORCEINLINE bool IsSafeToDereference() const { return Inner != nullptr && Inner->GetSharedNum() > 0; }
	
	public: // Getters
	
		FORCEINLINE CReferencerBase* Get() const { return Inner; }
	
	public: // Setters
	
		FORCEINLINE void Set(CReferencerBase* InReferencer) { Inner = InReferencer; }
	
	public: // Helper methods [Add]
	
		FORCEINLINE void AddShared()
		{
			if(!IsValid()) return;
			
			Inner->AddShared();
		}
	
		FORCEINLINE void AddWeak()
		{
			if(!IsValid()) return;
			
			Inner->AddWeak();
		}
	
	public: // Helper methods [Remove]
	
		FORCEINLINE void RemoveShared()
		{
			if(!IsValid()) return;
			
			Inner->RemoveShared();
			if(!Inner->HasAnyReference())
			{
				DeleteReferencer(Inner);
				Inner = nullptr;
			}
		}
		
		FORCEINLINE void RemoveWeak()
		{
			if(!IsValid()) return;
			
			Inner->RemoveWeak();
			if (!Inner->HasAnyReference())
			{
				DeleteReferencer(Inner);
				Inner = nullptr;
			}
		}
	
	private: // Fields
	
		CReferencerBase* Inner;
		
	};
}