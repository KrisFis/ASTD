
#pragma once

#include "IntegerTypes.h"
#include "TypeTraits.h"
#include "EnsureMacros.h"

#include <cstdint>

namespace NSharedInternals
{
	struct FNullType {};
	
	class FReferencerBase
	{
	public: // Constructors
		
		inline FReferencerBase()
			: SharedCount(0)
			, WeakCount(0)
		{}
		
		virtual ~FReferencerBase() = default;
		
	public: // Getters
	
		inline bool HasAnyReference() const { return SharedCount + WeakCount > 0; }
		inline uint16 GetSharedNum() const { return SharedCount; }
		inline uint16 GetWeakCount() const { return WeakCount; }
		
		template<typename T>
		inline T* GetObject() const { return reinterpret_cast<T*>(GetObjectImpl()); }
		
		inline bool HasObject() const { return GetObjectImpl() != nullptr; }
		
	public: // Setters [Add]
	
		inline void AddShared() 
		{
			ENSURE(SharedCount < UINT16_MAX);
			++SharedCount;
		}
		
		inline void AddWeak() 
		{
			ENSURE(WeakCount < UINT16_MAX); // overflow
			++WeakCount;
		}
		
	public: // Setters [REMOVE]
	
		inline void RemoveShared()
		{
			ENSURE(SharedCount > 0); // underflow
			if(SharedCount == 1) DeconstructObjectImpl();
			--SharedCount;
		}
		
		inline void RemoveWeak()
		{
			ENSURE(WeakCount > 0); // underflow
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
	
		inline TCustomReferencer(ObjectType* InObject, DeleterType InDeleter)
			: FReferencerBase()
			, Object(InObject)
			, Deleter(InDeleter)
		{}
		
		virtual ~TCustomReferencer() override
		{
			if (Object)
			{
				ENSURE(Deleter);
				Deleter(Object);
			}
		}
		
	protected: // FReferencer overrides
	
		virtual void* GetObjectImpl() const override 
		{ 
			return Object;
		}
		
		virtual void DeconstructObjectImpl() override 
		{
			if(Object)
			{
				ENSURE(Deleter);
			
				Deleter(Object);
				Object = nullptr;
			}
		}
		
	private: // Fields
	
		ObjectType* Object;
		DeleterType Deleter;
	};
	
	template<typename ObjectType>
	inline FReferencerBase* NewCustomReferencer(ObjectType* Object)
	{
		return new TCustomReferencer(Object, [](ObjectType* ToDelete) { delete ToDelete; });
	}
	
	template<typename ObjectType, typename DeleterType>
	inline FReferencerBase* NewCustomReferencerWithDeleter(ObjectType* Object, DeleterType* Deleter)
	{
		return new TCustomReferencer(Object, Deleter);
	}
	
	inline void DeleteReferencer(FReferencerBase* Referencer)
	{
		delete Referencer;
	}
	
	// Contains helper methods for referencer
	// * Should be used internally
	// * Handles even deconstruction of referencer
	struct FReferencerProxy
	{
	
	public: // Constructors
	
		inline FReferencerProxy()
			: Referencer(nullptr)
		{}
	
		inline FReferencerProxy(FReferencerBase* InReferencer)
			: Referencer(InReferencer)
		{}
	
	public: // Pointer operators
	
		inline FReferencerBase* operator->() { return Get(); }
		inline const FReferencerBase* operator->() const { return Get(); }
		
		inline FReferencerBase& operator*() { return *Get(); }
		inline const FReferencerBase& operator*() const { return *Get(); }
	
	public: // Checkers
	
		inline bool IsValid() const { return Referencer != nullptr; }
		inline bool IsUnique() const { return Referencer != nullptr && Referencer->GetSharedNum() == 1; }
		inline bool IsSafeToDereference() const { return Referencer != nullptr && Referencer->GetSharedNum() > 0; }
	
	public: // Getters
	
		inline FReferencerBase* Get() const { return Referencer; }
	
	public: // Setters
	
		inline void Set(FReferencerBase* InReferencer) { Referencer = InReferencer; }
	
	public: // Helper methods [Add]
	
		inline void AddShared()
		{
			if(!IsValid()) return;
			
			Referencer->AddShared();
		}
	
		inline void AddWeak()
		{
			if(!IsValid()) return;
			
			Referencer->AddWeak();
		}
	
	public: // Helper methods [Remove]
	
		inline void RemoveShared()
		{
			if(!IsValid()) return;
			
			Referencer->RemoveShared();
			if(!Referencer->HasAnyReference())
			{
				DeleteReferencer(Referencer);
				Referencer = nullptr;
			}
		}
		
		inline void RemoveWeak()
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