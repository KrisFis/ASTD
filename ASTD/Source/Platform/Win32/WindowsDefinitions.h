
#pragma once

#include "Build/Configurations.h"
#include "Build/PreprocessorHelpers.h"

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

#define PLATFORM_HEADER(name) STRINGIFY(Platform/Win32/Windows ## name.h)
#define PLATFORM_STRUCT(name) SWindowsPlatform ## name
#define PLATFORM_CLASS(name) CWindowsPlatform ## name

#define ASSEMBLY(code) __asm(code)

#define DEPRECATED [[deprecated]]
#define NODISCARD [[nodiscard]]

#define FORCEINLINE __forceinline
#define FORCENOINLINE __declspec(noinline)
#define FORCEINLINE_DEBUGGABLE __inline

#define DEBUG_BREAK() __debugbreak()

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)