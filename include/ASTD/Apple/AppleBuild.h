// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <cstdint>

// COMPILATION & BUILD
////////////////////////////////////////////////////////////////////////

#define DO_PRAGMA(command) _Pragma(#command)

#define ASSEMBLY(code) __asm__(code)

#define HAS_INCLUDE(file) __has_include(file)

#define DEPRECATED __attribute__ ((__deprecated__))
#define DEPRECATED_MSG(msg) __attribute__ ((__deprecated__(msg)))
#define NODISCARD [[nodiscard]]

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

#define DLL_EXPORT __attribute__((visibility("default")))
#define DLL_IMPORT

// DIAGNOSTICS
////////////////////////////////////////////////////////////////////////

#define DIAG_WARNINGS_PUSH() DO_PRAGMA(GCC diagnostic push)
#define DIAG_WARNINGS_POP() DO_PRAGMA(GCC diagnostic pop)
#define DIAG_WARNINGS_SUPPRESS(value) DO_PRAGMA(GCC diagnostic ignored value)

#define DIAG_WARNING_UNUSED_VALUE "-Wunused-value"
#define DIAG_WARNING_NULL_DEREFERENCE "-Wnull-dereference"
#define DIAG_WARNING_IMPLICIT_NARROWING "-Wconversion"
#define DIAG_WARNING_SHIFT_OVERFLOW "-Wno-shift-overflow"
#define DIAG_WARNING_SHIFT_NEGATIVE_VALUE "-Wno-shift-negative-value"

#if ASTD_DEFAULT_WARNING_SUPPRESS
	DIAG_WARNINGS_SUPPRESS(DIAG_WARNING_IMPLICIT_NARROWING)
#endif


// OPTIMIZATIONS
// * Uses options, make sure it does not collide with anything
// * Otherwise update macros
////////////////////////////////////////////////////////////////////////

#define OPTIMIZATIONS_DISABLE() DO_PRAGMA(GCC push_options) DO_PRAGMA(GCC optimize("O0"))
#define OPTIMIZATIONS_RESET() DO_PRAGMA(GCC pop_options)

// Prediction
////////////////////////////////////////////////////////////////////////

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

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

#if __SIZEOF_WCHAR_T__ == 2
#define UTF16_WCHAR 1
#else
#define UTF16_WCHAR 0
#endif

#ifdef __cpp_char8_t
#define HAS_NATIVE_CHAR8 1
#else
#define HAS_NATIVE_CHAR8 0
#endif

#if HAS_NATIVE_CHAR8
typedef char8_t char8;
#else
typedef char char8;
#endif

typedef char16_t char16;
typedef char32_t char32;
typedef wchar_t wchar;
typedef char achar;

#ifdef UNICODE
typedef wchar tchar;
#else
typedef achar tchar;
#endif
