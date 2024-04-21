// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Type/TypeTraits.h"
#include "Core/Platform/PlatformMemory.h"

#if ASTD_OVERRIDE_NEW_DELETE
FORCEINLINE void* operator new(TSize size)
{
	return SPlatformMemory::Allocate((uint32)size);
}

FORCEINLINE void operator delete(void* ptr, TSize size)
{
	return SPlatformMemory::Deallocate(ptr, (uint32)size);
}
#endif

struct SMemory : public SPlatformMemory
{
	template<typename T>
	FORCEINLINE static T* AllocateElement(int64 num = 1)
	{
		return (T*)SPlatformMemory::Allocate(num * sizeof(T));
	}

	template<typename T>
	FORCEINLINE static T* AllocateElementZeroed(int64 num = 1)
	{
		return (T*)SPlatformMemory::AllocateZeroed(num * sizeof(T));
	}

	template<typename T>
	FORCEINLINE static void DeallocateElement(T* ptr, int64 num = 1)
	{
		return SPlatformMemory::Deallocate(ptr, num * sizeof(T));
	}

	template<typename T, typename... ArgTypes>
	FORCEINLINE static void ConstructElement(T* ptr, ArgTypes&&... Args)
	{
		if constexpr (!TIsTriviallyConstructible<T, ArgTypes...>::Value)
		{
			::new((void*)ptr) T(Forward<ArgTypes>(Args)...);
		}
	}

	template<typename T>
	FORCEINLINE static void CopyElement(T* To, const T* From, int64 num)
	{
		if constexpr (!TIsTriviallyCopyConstructible<T>::Value)
		{
			while(num-- > 0)
			{
				::new((void*)To) T(*From);
				++To; ++From;
			}
		}
		else
		{
			SPlatformMemory::Copy(
				To,
				From,
				sizeof(T) * num
			);
		}
	}

	template<typename T>
	FORCEINLINE static void CopyElement(T* ptr, const T* val)
	{
		CopyElement(ptr, val, 1);
	}

	template<typename T>
	FORCEINLINE static void CopyElement(T* ptr, const T& val)
	{
		CopyElement(ptr, &val, 1);
	}

	template<typename T>
	FORCEINLINE static void MoveElement(T* ptr, T* val)
	{
		if constexpr(!TIsTriviallyMoveConstructible<T>::Value)
		{
			::new((void*)ptr) T(Move(*val));
		}
		else
		{
			SPlatformMemory::Copy(
				ptr,
				val,
				sizeof(T)
			);
		}
	}

	template<typename T>
	FORCEINLINE static void MoveElement(T* ptr, T&& val)
	{
		MoveElement(ptr, &val);
	}

	// Destruct element
	template<typename T>
	FORCEINLINE static void DestructElement(T* ptr)
	{
		if constexpr(!TIsTriviallyDestructible<T>::Value)
		{
			ptr->~T();
		}
	}
};
