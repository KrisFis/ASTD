
#pragma once

#include "Build/BuildDefinitions.h"
#include "TypeTraits/TypeMethods.h"
#include "TypeTraits/TypeTraits.h"
#include "Platform/PlatformMemory.h"

namespace NMemoryUtilities
{
	template<typename ElementType, typename... ArgTypes>
	FORCEINLINE static void CallConstructor(ElementType* Object, ArgTypes&&... Args)
	{
		::new((void*)Object) ElementType(Forward<ArgTypes>(Args)...);
	}

	template<typename ElementType>
	FORCEINLINE static void CallCopyConstructor(ElementType* Object, const ElementType& Value)
	{
		::new((void*)Object) ElementType(Value);
	}

	template<typename ElementType>
	FORCEINLINE static void CallMoveConstructor(ElementType* Object, ElementType&& Value)
	{
		::new((void*)Object) ElementType(Move(Value));
	}

	template<typename ElementType>
	FORCEINLINE static void CallDestructor(ElementType* Object)
	{
		Object->~ElementType();
	}
}

typedef SPlatformMemory SMemory;