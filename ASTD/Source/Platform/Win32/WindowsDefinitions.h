
#pragma once

#include "Build/PreprocessorHelpers.h"

#include <intrin.h>

#define ASSEMBLY(code) __asm(code)

#define FORCEINLINE __forceinline
#define FORCENOINLINE __declspec(noinline)
#define FORCEINLINE_DEBUGGABLE __inline

#define DEBUG_BREAK() __debugbreak()

#define PLATFORM_HEADER(name) STRINGIFY(Platform/Win32/Windows ## name.h)
#define PLATFORM_STRUCT(name) SWindowsPlatform ## name

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)