
#pragma once

#include "BuildMacros.h"

// TODO(kristian.fisera): This should be seperated to multiple files for each platform

#if COMPILER_MSVC
	
	#include <intrin.h>
	
	#define FORCEINLINE __forceinline
	#define FORCENOINLINE __declspec(noinline)
	#define FORCEINLINE_DEBUGGABLE __inline

	#define DEBUG_BREAK() __debugbreak()
	
#elif COMPILER_GNUC

	#define FORCEINLINE __attribute__ ((always_inline))
	#define FORCENOINLINE __attribute__ ((noinline))
	#define FORCEINLINE_DEBUGGABLE inline
	
	#define DEBUG_BREAK() asm { int 3 }

#else

	#define FORCEINLINE inline
	#define FORCENOINLINE
	#define FORCEINLINE_DEBUGGABLE inline

#endif