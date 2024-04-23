// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"
#include PLATFORM_HEADER(Memory)

#include "ASTD/TypeTraits.h"

struct SMemory : public PLATFORM_STRUCT(Memory)
{
	static constexpr long double BITS_PER_BYTE = 8; // bits
	static constexpr long double KiB_PER_BYTE = 1.e-3; // kibibytes
	static constexpr long double Kb_PER_BYTE = 7.8e-3; // kilobits
	static constexpr long double KB_PER_BYTE = 1.e-3; // kilobytes
	static constexpr long double MiB_PER_BYTE = 9.53674e-7; // mebibytes
	static constexpr long double Mb_PER_BYTE = 7.629395e-6; // megabits
	static constexpr long double MB_PER_BYTE = 1.e-6; // megabytes
	static constexpr long double GiB_PER_BYTE = 9.31e-10; // gibibytes
	static constexpr long double Gb_PER_BYTE = 7.451e-9; // gigabits
	static constexpr long double GB_PER_BYTE = 1.e-9; // gigabytes
	static constexpr long double TiB_PER_BYTE = 1.e-12; // tebibytes
	static constexpr long double Tb_PER_BYTE = 7.e-12; // terabits
	static constexpr long double TB_PER_BYTE = 1.e-12; // terabytes

	template<typename T>
	FORCEINLINE static T* AllocateElement(int64 num = 1)
	{
		return (T*)Allocate(num * sizeof(T));
	}

	template<typename T>
	FORCEINLINE static T* AllocateElementZeroed(int64 num = 1)
	{
		return (T*)AllocateZeroed(num * sizeof(T));
	}

	template<typename T>
	FORCEINLINE static void DeallocateElement(T* ptr, int64 num = 1)
	{
		return Deallocate(ptr, num * sizeof(T));
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
			Copy(
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
			Copy(
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

#if ASTD_OVERRIDE_NEW_DELETE
inline void* operator new(TSize size)
{
	return SMemory::Allocate((uint32)size);
}

inline void operator delete(void* ptr, TSize size) noexcept
{
	return SMemory::Deallocate(ptr, (uint32)size);
}
#endif
