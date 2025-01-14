// Copyright Alternity Arts. All Rights Reserved

#pragma once

// Windows setup

#if ASTD_USE_UNICODE
	#define UNICODE
#endif

// Disable CRT warnings
#pragma warning(disable : 4996)
#define _CRT_NONSTDC_NO_WARNINGS

#include <Windows.h>
#include <intrin.h>

// COMPILATION & BUILD
////////////////////////////////////////////////////////////////////////

#define DO_PRAGMA(command) __pragma(command)

#define ASSEMBLY(code) __asm(code)

#define HAS_INCLUDE(file) __has_include(file)

#define DEPRECATED [[deprecated]]
#define NODISCARD [[nodiscard]]

#define FORCEINLINE __forceinline
#define FORCENOINLINE __declspec(noinline)

#if BUILD_DEBUG
	#define FORCEINLINE_DEBUGGABLE __inline
#else
	#define FORCEINLINE_DEBUGGABLE FORCEINLINE
#endif

#define DEBUG_BREAK() __debugbreak()

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

// VARIADIC
////////////////////////////////////////////////////////////////////////

#define VA_LIST va_list
#define VA_ARG(list, type) va_arg(list, type)
#define VA_START(list, param) va_start(list, param)
#define VA_COPY(destinationList, sourceList) va_copy(destinationList, sourceList)
#define VA_END(list) va_end(list)

// DIAGNOSTICS
////////////////////////////////////////////////////////////////////////

#define DIAG_WARNINGS_PUSH() DO_PRAGMA( warning( push ) )
#define DIAG_WARNINGS_POP() DO_PRAGMA( warning( pop ) )
#define DIAG_WARNINGS_SUPPRESS(value) DO_PRAGMA( warning( disable : value ) )

#define DIAG_WARNING_UNUSED_VALUE 4101
#define DIAG_WARNING_NULL_DEREFERENCE 6011

// OPTIMIZATIONS
////////////////////////////////////////////////////////////////////////

#define OPTIMIZATIONS_DISABLE() DO_PRAGMA( optimize("", off) )
#define OPTIMIZATIONS_RESET() DO_PRAGMA( optimize("", on) )

// Integers
////////////////////////////////////////////////

typedef __int8 int8;
typedef unsigned __int8 uint8;

typedef __int16 int16;
typedef unsigned __int16 uint16;

typedef __int32 int32;
typedef unsigned __int32 uint32;

typedef __int64 int64;
typedef unsigned __int64 uint64;

// Characters
////////////////////////////////////////////////

typedef __wchar_t wchar;
