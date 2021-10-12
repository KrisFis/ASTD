
#pragma once

#include "IntegerTypes.h"
#include "TypeTraits.h"
#include "EnsureMacros.h"

#include <cstdint>

namespace NSharedInternals
{
	struct FNullType {};
	enum EForceInit { Empty };
	
	class FReferencerBase
	{
	public: // Constructors
		
		inline FReferencerBase()
			: SharedCount(1)
			, WeakCount(0)
		{}
		
		inline FReferencerBase(EForceInit)
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
	
		inline TCustomReferencer(EForceInit Init)
			: FReferencerBase(Init)
			, Object(nullptr)
			, Deleter(nullptr)
		{}
	
		inline TCustomReferencer(ObjectType* InObject, DeleterType InDeleter)
			: FReferencerBase()
			, Object(InObject)
			, Deleter(InDeleter)
		{}
		
		virtual ~TCustomReferencer() override
		{
			ENSURE(Deleter);
			
			Deleter(Object);
		}
		
	protected: // FReferencer overrides
	
		virtual void* GetObjectImpl() const override 
		{ 
			return Object;
		}
		
		virtual void DeconstructObjectImpl() override 
		{
			ENSURE(Deleter);
		
			Deleter(Object);
			Object = nullptr;
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
}