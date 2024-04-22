// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Build/BuildDefinitions.h"
#include "Core/Build/PreprocessorHelpers.h"

// PLATFORM
////////////////////////////////////////////////////////////////////////

#define PLATFORM_HEADER(name) STRINGIFY(Core/Platform/Linux/Linux ## name.h)
#define PLATFORM_STRUCT(name) SLinuxPlatform ## name
#define PLATFORM_CLASS(name) CLinuxPlatform ## name

// COMPILATION & BUILD
////////////////////////////////////////////////////////////////////////

#define DO_PRAGMA(command) _Pragma(#command)

#define ASSEMBLY(code) __asm__(code)

#define DEPRECATED __attribute__ ((__deprecated__))
#define NODISCARD [[__nodiscard__]]

#define FORCEINLINE inline __attribute__((always_inline))

#define FORCENOINLINE __attribute__((noinline))

#if BUILD_DEBUG
	#define FORCEINLINE_DEBUGGABLE inline
#else
	#define FORCEINLINE_DEBUGGABLE FORCEINLINE
#endif

#if __has_builtin(__builtin_debugtrap)
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
#define DIAG_WARNINGS_SUPRESS(value) DO_PRAGMA(GCC diagnostic ignored value)

#define DIAG_WARNING_UNUSED_VALUE "-Wunused-value"
#define DIAG_WARNING_NULL_DEREFERENCE "-Wnull-dereference"

// OPTIMIZATIONS
// * Uses options, make sure it does not colide with anything
// * Otherwise update macros
////////////////////////////////////////////////////////////////////////

#define OPTIMIZATIONS_DISABLE() DO_PRAGMA(GCC push_options) DO_PRAGMA(GCC optimize("O0")) 
#define OPTIMIZATIONS_RESET() DO_PRAGMA(GCC pop_options)
