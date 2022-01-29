
#pragma once

#include "ASTDCore.h"
#include "SharedObject.h"

// Forward declaration
template<typename T> class TSharedClass;

namespace NSharedInternals
{
	// Checks whether WeakThis field exists
	// * Thats indicator of SharedClass
	template<typename T>
	struct TIsSharedClassType
	{
	
	private: // Typedefs

		typedef typename TDecay<T>::Type PureType;

	private: // WeakThis test
		
		template<typename TestType> static auto TestHasSharedInit(int32)->TTrueValue<decltype(DeclVal<TestType>().IsSharedInitialized())>;
		template<typename TestType>	static auto TestHasSharedInit(int64)->TConstBool<false>;
		
		template<typename TestType> struct FGetHasSharedInitTest : decltype(TestHasSharedInit<TestType>(0)) {};

	private: // Private init method

		template<typename TestType> static auto TestHasAsShared(int32)->TTrueValue<decltype(DeclVal<TestType>().AsShared())>;
		template<typename TestType>	static auto TestHasAsShared(int64)->TConstBool<false>;
		
		template<typename TestType> struct FGetHasAsSharedTest : decltype(TestHasAsShared<TestType>(0)) {};

	public:
	
		enum 
		{
			HasSharedInit = FGetHasSharedInitTest<PureType>::Value,
			HasAsShared = FGetHasAsSharedTest<PureType>::Value,
			
			Value = HasSharedInit && HasAsShared
		};
	
	};
	
	// Is shared class
	template<typename ChildType, typename TEnableIf<TIsSharedClassType<ChildType>::Value>::Type* = nullptr>
	FORCEINLINE_DEBUGGABLE TSharedPtr<ChildType> InitAsSharedClass(CReferencerBase* Referencer) 
	{ 
		auto resultPtr = TSharedPtr<ChildType>(*Referencer);
		resultPtr->Init_Private(TSharedPtr<typename ChildType::ClassType>(Referencer)); 
		return resultPtr;
	}
	
	// Is not shared class
	template<typename ChildType, typename TEnableIf<!TIsSharedClassType<ChildType>::Value>::Type* = nullptr>
	FORCEINLINE_DEBUGGABLE TSharedPtr<ChildType> InitAsSharedClass(CReferencerBase* Referencer) 
	{ return TSharedPtr<ChildType>(*Referencer); }
	
}