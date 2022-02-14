
#pragma once

#include "Build/BuildDefinitions.h"
#include "TypeTraits/TypeMethods.h"
#include "TypeTraits/TypeTraits.h"
#include "Platform/PlatformMemory.h"

// Helpers
///////////////////////////////////////////////////////////

struct SMemoryUtils
{
	template<typename ElementType, typename... ArgTypes>
	FORCEINLINE static void CallConstructor(ElementType* Object, ArgTypes... Args)
	{
		::new((void*)Object) ElementType(Forward<ArgTypes>(Args)...);
	}

	template<typename ElementType>
	FORCEINLINE static void CallDestructor(ElementType* Object)
	{
		Object->~ElementType();
	}
};

// Types
///////////////////////////////////////////////////////////

struct SMemory : public SPlatformMemory
{
	template<typename T, typename TEnableIf<!TIsSame<T, void>::Value>::Type* = nullptr>
	FORCEINLINE static T* AllocateTyped(uint32 Count = 1) 
	{ return reinterpret_cast<T*>(SPlatformMemory::Allocate(SizeOf<T>() * Count)); }

	template<typename T, typename TEnableIf<!TIsSame<T, void>::Value>::Type* = nullptr>
	FORCEINLINE static T* AllocateZeroedTyped(uint32 Count = 1) 
	{ return reinterpret_cast<T*>(SPlatformMemory::AllocateZeroed(SizeOf<T>() * Count)); }

	template<typename T, typename TEnableIf<!TIsSame<T, void>::Value>::Type* = nullptr>
	FORCEINLINE static void DeallocateTyped(T* Ptr, uint32 Count = 1)
	{ SPlatformMemory::Deallocate(Ptr, SizeOf<T>() * Count); }

	template<typename T, typename TEnableIf<!TIsSame<T, void>::Value>::Type* = nullptr>
	FORCEINLINE static T* MemoryCopyTyped(T* Destination, T* Source, uint32 Num) 
	{ return reinterpret_cast<T*>(SPlatformMemory::MemoryCopy(Destination, Source, SizeOf<T>() * Num)); }

	template<typename T, typename TEnableIf<!TIsSame<T, void>::Value>::Type* = nullptr>
	FORCEINLINE static T* MemoryMoveTyped(T* Destination, T* Source, uint32 Num) 
	{ return reinterpret_cast<T*>(SPlatformMemory::MemoryMove(Destination, Source, SizeOf<T>() * Num)); }
};