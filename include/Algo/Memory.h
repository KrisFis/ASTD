// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Type/TypeTraits.h"
#include "Core/Memory.h"

namespace NAlgo
{
	template<typename T>
	FORCEINLINE T* AllocateElement(int64 num = 1)
	{
		return (T*)SMemory::Allocate(num * sizeof(T));
	}

	template<typename T>
	FORCEINLINE T* AllocateElementZeroed(int64 num = 1)
	{
		return (T*)SMemory::AllocateZeroed(num * sizeof(T));
	}

	template<typename T>
	FORCEINLINE void DeallocateElement(T* ptr, int64 num = 1)
	{
		return SMemory::Deallocate(ptr, num * sizeof(T));
	}

	template<typename T, typename... ArgTypes>
	FORCEINLINE void ConstructElement(T* ptr, ArgTypes&&... Args)
	{
		if constexpr (!TIsTriviallyConstructible<T, ArgTypes...>::Value)
		{
			::new((void*)ptr) T(Forward<ArgTypes>(Args)...);
		}
	}

	template<typename T>
	FORCEINLINE void CopyElement(T* To, const T* From, int64 num)
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
			SMemory::Copy(
				To,
				From,
				sizeof(T) * num
			);
		}
	}

	template<typename T>
	FORCEINLINE void CopyElement(T* ptr, const T* val)
	{
		CopyElement(ptr, val, 1);
	}

	template<typename T>
	FORCEINLINE void CopyElement(T* ptr, const T& val)
	{
		CopyElement(ptr, &val, 1);
	}

	template<typename T>
	FORCEINLINE void MoveElement(T* ptr, T* val)
	{
		if constexpr(!TIsTriviallyMoveConstructible<T>::Value)
		{
			::new((void*)ptr) T(Move(*val));
		}
		else
		{
			SMemory::Copy(
				ptr,
				val,
				sizeof(T)
			);
		}
	}

	template<typename T>
	FORCEINLINE void MoveElement(T* ptr, T&& val)
	{
		MoveElement(ptr, &val);
	}

	// Destruct element
	template<typename T>
	FORCEINLINE void DestructElement(T* ptr)
	{
		if constexpr(!TIsTriviallyDestructible<T>::Value)
		{
			ptr->~T();
		}
	}
}
