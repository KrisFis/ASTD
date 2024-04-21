// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <memory.h>

#include "Core/Platform/PlatformTypes.h"

struct SBasePlatformMemory
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
	
	// Copies block of memory from destionation to source (does not handle overlapping)
	FORCEINLINE static void* Copy(void* dest, const void* src, int64 size) { return memcpy(dest, src, size); }

	// Copies block of memory from destionation to source (handles overlapping)
	FORCEINLINE static void* Move(void* dest, const void* src, int64 size) { return memmove(dest, src, size); }

	// Compares two blocks of memory
	FORCEINLINE static int32 Compare(const void* lhs, const void* rhs, int64 num) { return memcmp(lhs, rhs, num); }
};
