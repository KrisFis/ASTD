
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
	private:

		typedef typename TDecay<T>::Type PureType;

		typedef char Yes[1];
		typedef char No[2];

		template<typename TestType> static Yes& HasMethodTest(decltype(&TestType::Init_Private));
		template<typename TestType>	static No& HasMethodTest(...);
		
		template<typename TestType> static Yes& HasTypedefTest(decltype(&TestType::ClassType));
		template<typename TestType> static No& HasTypedefTest(...);
		
	public:
	
		enum 
		{ 
			HasMethod = sizeof(HasMethodTest<PureType>(0)) == sizeof(Yes),
			HasTypedef = sizeof(HasTypedefTest<PureType>(0)) == sizeof(Yes),
			
			Value = HasMethod && HasTypedef
		};
	
	};
	
	// Is shared class
	template<typename ChildType, typename TEnableIf<TIsSharedClassType<ChildType>::Value>::Type* = nullptr>
	FORCEINLINE_DEBUGGABLE TSharedPtr<ChildType> InitAsSharedClass(CReferencerBase* Referencer) 
	{ 
		auto resultPtr = TSharedPtr<ChildType>(Referencer);
		resultPtr->Init_Private(TSharedPtr<typename ChildType::ClassType>(Referencer)); 
		return resultPtr;
	}
	
	// Is not shared class
	template<typename ChildType, typename TEnableIf<!TIsSharedClassType<ChildType>::Value>::Type* = nullptr>
	FORCEINLINE_DEBUGGABLE TSharedPtr<ChildType> InitAsSharedClass(CReferencerBase* Referencer) 
	{ return TSharedPtr<ChildType>(Referencer); }
	
}