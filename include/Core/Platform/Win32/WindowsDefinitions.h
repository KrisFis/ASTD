// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Build/BuildDefinitions.h"
#include "Core/Build/Configurations.h"
#include "Core/Build/PreprocessorHelpers.h"

// Windows setup

#if USE_UNICODE
	#define UNICODE
#endif

// Disable CRT warnings
#pragma warning(disable : 4996)
#define _CRT_NONSTDC_NO_WARNINGS

#include <Windows.h>
#include <intrin.h>

// PLATFORM
////////////////////////////////////////////////////////////////////////

#define PLATFORM_HEADER(name) STRINGIFY(Core/Platform/Win32/Windows ## name.h)
#define PLATFORM_STRUCT(name) SWindowsPlatform ## name
#define PLATFORM_CLASS(name) CWindowsPlatform ## name

#define DO_PRAGMA(command) __pragma(command)

#define ASSEMBLY(code) __asm(code)

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
