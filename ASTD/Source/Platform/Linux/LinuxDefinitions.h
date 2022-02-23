
#pragma once

#include "Build/PreprocessorHelpers.h"

#define ASSEMBLY(code) __asm__(code)

#define FORCEINLINE __attribute__((always_inline))
#define FORCENOINLINE __attribute__((noinline))
#define FORCEINLINE_DEBUGGABLE inline

#if __has_builtin(__builtin_debugtrap)
	#define DEBUG_BREAK() __builtin_debugtrap()
#else
	#define DEBUG_BREAK() ASSEMBLY("int3")
#endif

#define PLATFORM_HEADER(name) STRINGIFY(Platform/Linux/Linux ## name.h)
#define PLATFORM_STRUCT(name) SLinuxPlatform ## name

#define DLL_EXPORT
#define DLL_IMPORT