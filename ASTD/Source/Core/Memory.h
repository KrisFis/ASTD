
#pragma once

#include "Core/Type/TypeTraits.h"
#include "Core/Type/TypeMethods.h"
#include "Core/Platform/PlatformMemory.h"

void* operator new(TSize Size)
{
	return SPlatformMemory::Allocate((uint32)Size);
}

void operator delete(void* Ptr, TSize Size)
{
	return SPlatformMemory::Deallocate(Ptr, (uint32)Size);
}

typedef SPlatformMemory SMemory;

namespace NMemoryType
{
	// Allocates elements
	template<typename T>
	FORCEINLINE T* Allocate(int64 Num = 1)
	{
		return (T*)SMemory::Allocate(Num * sizeof(T));
	}

	template<typename T>
	FORCEINLINE T* AllocateZeroed(int64 Num = 1)
	{
		return (T*)SMemory::AllocateZeroed(Num * sizeof(T));
	}

	// Deallocate elements
	template<typename T>
	FORCEINLINE void Deallocate(T* Ptr, int64 Num = 1)
	{
		return SMemory::Deallocate(Ptr, Num * sizeof(T));
	}

	// Construct element
	template<typename T, typename... ArgTypes>
	FORCEINLINE void Construct(T* Ptr, ArgTypes&&... Args)
	{
		if constexpr (!TIsTriviallyConstructible<T, ArgTypes...>::Value)
		{
			::new((void*)Ptr) T(Forward<ArgTypes>(Args)...);
		}
	}

	template<typename T>
	FORCEINLINE void CopyConstruct(T* To, const T* From, int64 Num)
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
	FORCEINLINE void CopyConstruct(T* Ptr, const T* Value)
	{
		CopyConstruct(Ptr, Value, 1);
	}

	template<typename T>
	FORCEINLINE void CopyConstruct(T* Ptr, const T& Value)
	{
		CopyConstruct(Ptr, &Value, 1);
	}

	template<typename T>
	FORCEINLINE void MoveConstruct(T* Ptr, T* Value)
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
	FORCEINLINE void MoveConstruct(T* Ptr, T&& Value)
	{
		MoveConstruct(Ptr, &Value);
	}

	// Destruct element
	template<typename T>
	FORCEINLINE void Destruct(T* Ptr)
	{
		if constexpr(!TIsTriviallyDestructible<T>::Value)
		{
			Ptr->~T();
		}
	}

	template<typename T>
	FORCEINLINE int64 Compare(const T* Lhs, const T* Rhs)
	{
		return SMemory::Compare(Lhs, Rhs, sizeof(T));
	}

	template<typename T>
	FORCEINLINE int64 Compare(const T& Lhs, const T& Rhs)
	{
		return Compare(&Lhs, &Rhs);
	}
}