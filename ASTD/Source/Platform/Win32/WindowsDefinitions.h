
#pragma once

#include "Build/Configurations.h"
#include "Build/PreprocessorHelpers.h"

// PLATFORM
////////////////////////////////////////////////////////////////////////

#define PLATFORM_HEADER(name) STRINGIFY(Platform/Win32/Windows ## name.h)
#define PLATFORM_STRUCT(name) SWindowsPlatform ## name
#define PLATFORM_CLASS(name) CWindowsPlatform ## name

#define DO_PRAGMA(command) __pragma(#command)

#define ASSEMBLY(code) __asm(code)

#define DEPRECATED [[deprecated]]
#define NODISCARD [[nodiscard]]

#define FORCEINLINE __forceinline
#define FORCENOINLINE __declspec(noinline)
#define FORCEINLINE_DEBUGGABLE __inline

#define DEBUG_BREAK() __debugbreak()

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

// DIAGNOSTICS
////////////////////////////////////////////////////////////////////////

#define DIAG_WARNINGS_PUSH() DO_PRAGMA( warning( push ) )
#define DIAG_WARNINGS_POP() DO_PRAGMA( warning( pop ) )
#define DIAG_WARNINGS_SUPRESS(value) DO_PRAGMA( warning( disable : value ) )

#define DIAG_WARNING_UNUSED_VALUE 4101
#define DIAG_WARNING_NULL_DEREFERENCE 6011

// OPTIMIZATIONS
////////////////////////////////////////////////////////////////////////

#define OPTIMIZATIONS_DISABLE() DO_PRAGMA( optimize("", off) )
#define OPTIMIZATIONS_RESET() DO_PRAGMA( optimize("", on) )

// Windows setup

#if USE_UNICODE
	#define UNICODE
#endif

// Disable CRT warnings
// #if !defined(_CRT_SECURE_NO_WARNINGS)
// 	#define _CRT_SECURE_NO_WARNINGS
// 	#define _CRT_NONSTDC_NO_WARNINGS
// #endif

#include <Windows.h>
#include <intrin.h>