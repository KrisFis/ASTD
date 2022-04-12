
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
	template<typename T, typename R>
	T* Allocate(R Num)
	{
		return SMemory::Allocate(Num * sizeof(T));
	}

	template<typename T, typename R>
	T* AllocateZeroed(R Num)
	{
		return SMemory::AllocateZeroed(Num * sizeof(T));
	}

	// Deallocate elements
	template<typename T, typename R>
	void Deallocate(T* Ptr, R Num)
	{
		return SMemory::Deallocate(Ptr, Num * sizeof(T));
	}

	// Construct element
	template<typename T, typename... ArgTypes>
	void Construct(T* Ptr, ArgTypes&&... Args)
	{
		if constexpr (!TIsTriviallyConstructible<T, ArgTypes...>::Value)
		{
			::new((void*)Object) T(Forward<ArgTypes>(Args)...);
		}
	}

	// Copy construct element
	template<typename T>
	void Construct<T, const T&>(T* Ptr, const T& Value)
	{
		if constexpr(!TIsTriviallyCopyConstructible<T>::Value)
		{
			::new((void*)Object) T(Value);
		}
		else
		{
			SMemory::Copy(
				Ptr,
				&Value,
				sizeof(T)
			);
		}
	}

	// Move construct element
	template<typename T>
	void Construct<T, T&&>(T* Ptr, T&& Value)
	{
		if constexpr(!TIsTriviallyMoveConstructible<T>::Value)
		{
			::new((void*)Object) T(Move(Value));
		}
		else
		{
			SMemory::Copy(
				Ptr,
				&Value,
				sizeof(T)
			);
		}
	}

	// Destruct element
	template<typename T>
	void Destruct(T* Ptr)
	{
		if constexpr(!TIsTriviallyDestructible<T>::Value)
		{
			Object->~T();
		}
	}
}