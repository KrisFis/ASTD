// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Type/TypeTraits.h"
#include "Core/Memory.h"

namespace NAlgo
{
	template<typename T>
	FORCEINLINE T* AllocateElement(int64 Num = 1)
	{
		return (T*)SMemory::Allocate(Num * sizeof(T));
	}

	template<typename T>
	FORCEINLINE T* AllocateElementZeroed(int64 Num = 1)
	{
		return (T*)SMemory::AllocateZeroed(Num * sizeof(T));
	}

	template<typename T>
	FORCEINLINE void DeallocateElement(T* Ptr, int64 Num = 1)
	{
		return SMemory::Deallocate(Ptr, Num * sizeof(T));
	}

	template<typename T, typename... ArgTypes>
	FORCEINLINE void ConstructElement(T* Ptr, ArgTypes&&... Args)
	{
		if constexpr (!TIsTriviallyConstructible<T, ArgTypes...>::Value)
		{
			::new((void*)Ptr) T(Forward<ArgTypes>(Args)...);
		}
	}

	template<typename T>
	FORCEINLINE void CopyElement(T* To, const T* From, int64 Num)
	{
		if constexpr (!TIsTriviallyCopyConstructible<T>::Value)
		{
			while(Num-- > 0)
			{
				::new((void*)To) T(*From);
				++To; ++From;
			}
		}
		else
		{
			SMemory::Copy(
				To,
				From,
				sizeof(T) * Num
			);
		}
	}

	template<typename T>
	FORCEINLINE void CopyElement(T* Ptr, const T* Value)
	{
		CopyElement(Ptr, Value, 1);
	}

	template<typename T>
	FORCEINLINE void CopyElement(T* Ptr, const T& Value)
	{
		CopyElement(Ptr, &Value, 1);
	}

	template<typename T>
	FORCEINLINE void MoveElement(T* Ptr, T* Value)
	{
		if constexpr(!TIsTriviallyMoveConstructible<T>::Value)
		{
			::new((void*)Ptr) T(Move(*Value));
		}
		else
		{
			SMemory::Copy(
				Ptr,
				Value,
				sizeof(T)
			);
		}
	}

	template<typename T>
	FORCEINLINE void MoveElement(T* Ptr, T&& Value)
	{
		MoveElement(Ptr, &Value);
	}

	// Destruct element
	template<typename T>
	FORCEINLINE void DestructElement(T* Ptr)
	{
		if constexpr(!TIsTriviallyDestructible<T>::Value)
		{
			Ptr->~T();
		}
	}
}