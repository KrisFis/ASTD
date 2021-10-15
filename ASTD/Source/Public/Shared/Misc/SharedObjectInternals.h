
#pragma once

#include "ASTDCore.h"

namespace NSharedInternals
{
	struct FNullType {};
	
	class FReferencerBase
	{
	public: // Constructors
		
		FORCEINLINE FReferencerBase()
			: SharedCount(0)
			, WeakCount(0)
		{}
		
		virtual ~FReferencerBase() = default;
		
	public: // Getters
	
		FORCEINLINE bool HasAnyReference() const { return SharedCount + WeakCount > 0; }
		FORCEINLINE uint16 GetSharedNum() const { return SharedCount; }
		FORCEINLINE uint16 GetWeakCount() const { return WeakCount; }
		
		template<typename T>
		FORCEINLINE T* GetObject() const { return reinterpret_cast<T*>(GetObjectImpl()); }
		
		FORCEINLINE bool HasObject() const { return GetObjectImpl() != nullptr; }
		
	public: // Setters [Add]
	
		FORCEINLINE_DEBUGGABLE void AddShared() 
		{
			ENSURE_RET(SharedCount < UINT16_MAX);
			++SharedCount;
		}
		
		FORCEINLINE_DEBUGGABLE void AddWeak() 
		{
			ENSURE_RET(WeakCount < UINT16_MAX); // overflow
			++WeakCount;
		}
		
	public: // Setters [REMOVE]
	
		FORCEINLINE_DEBUGGABLE void RemoveShared()
		{
			ENSURE_RET(SharedCount > 0); // underflow
			if(SharedCount == 1) DeconstructObjectImpl();
			--SharedCount;
		}
		
		FORCEINLINE_DEBUGGABLE void RemoveWeak()
		{
			ENSURE_RET(WeakCount > 0); // underflow
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
	class TCustomReferencer : public FReferencerBase
	{
	public: // Typedefs
	
		typedef T ObjectType;
		typedef DeleterT DeleterType;
		
	public: // Constructors
	
		FORCEINLINE TCustomReferencer(ObjectType* InObject, DeleterType InDeleter)
			: FReferencerBase()
			, Object(InObject)
			, Deleter(InDeleter)
		{}
		
		virtual ~TCustomReferencer() override
		{
			if (Object)
			{
				ENSURE_NOT_NULL_RET(Deleter);
				Deleter(Object);
			}
		}
		
	protected: // FReferencer overrides
	
		FORCEINLINE virtual void* GetObjectImpl() const override 
		{ 
			return Object;
		}
		
		FORCEINLINE_DEBUGGABLE virtual void DeconstructObjectImpl() override 
		{
			if(Object)
			{
				ENSURE_NOT_NULL_RET(Deleter);
			
				Deleter(Object);
				Object = nullptr;
			}
		}
		
	private: // Fields
	
		ObjectType* Object;
		DeleterType Deleter;
	};
	
	template<typename ObjectType>
	FORCEINLINE FReferencerBase* NewCustomReferencer(ObjectType* Object)
	{
		return new TCustomReferencer(Object, [](ObjectType* ToDelete) { delete ToDelete; });
	}
	
	template<typename ObjectType, typename DeleterType>
	FORCEINLINE FReferencerBase* NewCustomReferencerWithDeleter(ObjectType* Object, DeleterType* Deleter)
	{
		return new TCustomReferencer(Object, Deleter);
	}
	
	FORCEINLINE void DeleteReferencer(FReferencerBase* Referencer)
	{
		delete Referencer;
	}
	
	// Contains helper methods for referencer
	// * Should be used internally
	// * Handles even deconstruction of referencer
	struct FReferencerProxy
	{
	
	public: // Constructors
	
		FORCEINLINE FReferencerProxy()
			: Referencer(nullptr)
		{}
	
		FORCEINLINE FReferencerProxy(FReferencerBase* InReferencer)
			: Referencer(InReferencer)
		{}
	
	public: // Pointer operators
	
		FORCEINLINE FReferencerBase* operator->() { return Get(); }
		FORCEINLINE const FReferencerBase* operator->() const { return Get(); }
		
		FORCEINLINE FReferencerBase& operator*() { return *Get(); }
		FORCEINLINE const FReferencerBase& operator*() const { return *Get(); }
	
	public: // Checkers
	
		FORCEINLINE bool IsValid() const { return Referencer != nullptr; }
		FORCEINLINE bool IsUnique() const { return Referencer != nullptr && Referencer->GetSharedNum() == 1; }
		FORCEINLINE bool IsSafeToDereference() const { return Referencer != nullptr && Referencer->GetSharedNum() > 0; }
	
	public: // Getters
	
		FORCEINLINE FReferencerBase* Get() const { return Referencer; }
	
	public: // Setters
	
		FORCEINLINE void Set(FReferencerBase* InReferencer) { Referencer = InReferencer; }
	
	public: // Helper methods [Add]
	
		FORCEINLINE void AddShared()
		{
			if(!IsValid()) return;
			
			Referencer->AddShared();
		}
	
		FORCEINLINE void AddWeak()
		{
			if(!IsValid()) return;
			
			Referencer->AddWeak();
		}
	
	public: // Helper methods [Remove]
	
		FORCEINLINE_DEBUGGABLE void RemoveShared()
		{
			if(!IsValid()) return;
			
			Referencer->RemoveShared();
			if(!Referencer->HasAnyReference())
			{
				DeleteReferencer(Referencer);
				Referencer = nullptr;
			}
		}
		
		FORCEINLINE_DEBUGGABLE void RemoveWeak()
		{
			if(!IsValid()) return;
			
			Referencer->RemoveWeak();
			if (!Referencer->HasAnyReference())
			{
				DeleteReferencer(Referencer);
				Referencer = nullptr;
			}
		}
	
	private: // Fields
	
		FReferencerBase* Referencer;
		
	};
}