// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <cstdarg>
#include <stdint.h>

// COMPILATION & BUILD
////////////////////////////////////////////////////////////////////////

#define DO_PRAGMA(command) _Pragma(#command)

#define ASSEMBLY(code) __asm__(code)

#define HAS_INCLUDE(file) __has_include(file)

#define DEPRECATED __attribute__ ((__deprecated__))
#define DEPRECATED_MSG(msg) __attribute__ ((__deprecated__(msg)))
#define NODISCARD [[__nodiscard__]]

#define FORCEINLINE inline __attribute__((always_inline))

#define FORCENOINLINE __attribute__((noinline))

#if BUILD_DEBUG
	#define FORCEINLINE_DEBUGGABLE inline
#else
	#define FORCEINLINE_DEBUGGABLE FORCEINLINE
#endif

#if COMPILER_CLANG
	#define DEBUG_BREAK() __builtin_debugtrap()
#else
	#define DEBUG_BREAK() __asm__ volatile("int3")
#endif

#define DLL_EXPORT
#define DLL_IMPORT

// DIAGNOSTICS
////////////////////////////////////////////////////////////////////////

#define DIAG_WARNINGS_PUSH() DO_PRAGMA(GCC diagnostic push)
#define DIAG_WARNINGS_POP() DO_PRAGMA(GCC diagnostic pop)
#define DIAG_WARNINGS_SUPPRESS(value) DO_PRAGMA(GCC diagnostic ignored value)

#define DIAG_WARNING_UNUSED_VALUE "-Wunused-value"
#define DIAG_WARNING_NULL_DEREFERENCE "-Wnull-dereference"

// OPTIMIZATIONS
// * Uses options, make sure it does not colide with anything
// * Otherwise update macros
////////////////////////////////////////////////////////////////////////

#define OPTIMIZATIONS_DISABLE() DO_PRAGMA(GCC push_options) DO_PRAGMA(GCC optimize("O0")) 
#define OPTIMIZATIONS_RESET() DO_PRAGMA(GCC pop_options)

// Integers
////////////////////////////////////////////////

typedef int8_t int8;
typedef uint8_t uint8;

typedef int16_t int16;
typedef uint16_t uint16;

typedef int32_t int32;
typedef uint32_t uint32;

typedef int64_t int64;
typedef uint64_t uint64;

// Characters
////////////////////////////////////////////////

typedef wchar_t wchar;
