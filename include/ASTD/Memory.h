// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Build.h"
#include PLATFORM_HEADER(Memory)

#include <new>

#include "ASTD/TypeTraits.h"

typedef PLATFORM_PREFIXED_TYPE(S, PlatformMemory) SPlatformMemory;

struct SMemory : public SPlatformMemory
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
	FORCEINLINE static T* MallocObject(int64 num = 1)
	{
#if ASTD_TRACK_MEMORY
		GetAllocatedBytesImpl() += num * sizeof(T);
#endif
		return (T*)SPlatformMemory::Malloc(num * sizeof(T));
	}

	template<typename T>
	FORCEINLINE static T* CallocObject(int64 num = 1)
	{
#if ASTD_TRACK_MEMORY
		GetAllocatedBytesImpl() += num * sizeof(T);
#endif
		return (T*)SPlatformMemory::Calloc(num * sizeof(T));
	}

	template<typename T>
	FORCEINLINE static void FreeObject(T* ptr, int64 num = 1)
	{
#if ASTD_TRACK_MEMORY
		GetAllocatedBytesImpl() -= num * sizeof(T);
#endif
		return SPlatformMemory::Free(ptr, num * sizeof(T));
	}

	template<typename T, typename... ArgTypes>
	FORCEINLINE static void ConstructObject(T* ptr, ArgTypes&&... Args)
	{
		if constexpr (!TIsTriviallyConstructible<T, ArgTypes...>::Value)
		{
			::new((void*)ptr) T(Forward<ArgTypes>(Args)...);
		}
	}

	// Destruct element
	template<typename T>
	FORCEINLINE static void DestructObject(T* ptr)
	{
		if constexpr(!TIsTriviallyDestructible<T>::Value)
		{
			ptr->~T();
		}
	}

	template<typename T>
	FORCEINLINE static void CopyObject(T* to, const T* from, uint32 num = 1)
	{
		if constexpr (!TIsTriviallyCopyConstructible<T>::Value)
		{
			int64 signedNum = num;
			while(signedNum-- > 0)
			{
				::new((void*)to) T(*from);

				++to;
				++from;
			}
		}
		else
		{
			SPlatformMemory::Memcpy(
				to,
				from,
				sizeof(T) * num
			);
		}
	}

	template<typename T>
	FORCEINLINE static void MoveObject(T* to, T* from)
	{
		if constexpr(!TIsTriviallyMoveConstructible<T>::Value)
		{
			::new((void*)to) T(SPlatformMemory::Memmove(*from));
		}
		else
		{
			SPlatformMemory::Memmove(
				to,
				from,
				sizeof(T)
			);
		}
	}

#if ASTD_TRACK_MEMORY
	FORCEINLINE static uint64 GetAllocatedBytes() { return GetAllocatedBytesImpl(); }
#endif

private:

#if ASTD_TRACK_MEMORY
	static uint64& GetAllocatedBytesImpl()
	{
		static uint64 bytes = 0;
		return bytes;
	}
#endif
};

#if ASTD_NEW_DELETE
void* operator new(TSize size)
{
	return SMemory::MallocObject<uint8>((uint32)size);
}

void operator delete(void* ptr, TSize size) noexcept
{
	return SMemory::FreeObject<uint8>((uint8*)ptr, (uint32)size);
}
#endif
