
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

		template<typename TestType> static Yes& Test(decltype(&TestType::Init_Private));
		template<typename TestType>	static No& Test(...);
		
	public:
	
		enum { Value = sizeof(Test<PureType>(0)) == sizeof(Yes) };
	
	};
	
	// Is shared class
	template<typename ChildType, typename TEnableIf<TIsSharedClassType<ChildType>::Value>::Type* = nullptr>
	FORCEINLINE_DEBUGGABLE TSharedPtr<ChildType> InitAsSharedClass(CReferencerBase* Referencer) 
	{ 
		auto resultPtr = TSharedPtr<ChildType>(Referencer);
		resultPtr->Init_Private(resultPtr); 
		return resultPtr;
	}
	
	// Is not shared class
	template<typename ChildType, typename TEnableIf<!TIsSharedClassType<ChildType>::Value>::Type* = nullptr>
	FORCEINLINE_DEBUGGABLE TSharedPtr<ChildType> InitAsSharedClass(CReferencerBase* Referencer) 
	{ return TSharedPtr<ChildType>(Referencer); }
	
}