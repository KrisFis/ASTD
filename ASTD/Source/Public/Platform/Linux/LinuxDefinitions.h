
#pragma once

#define FORCEINLINE __attribute__ ((always_inline)) inline
#define FORCENOINLINE __attribute__ ((noinline))
#define FORCEINLINE_DEBUGGABLE inline
	
#if __has_builtin(__builtin_debugtrap)
	#define DEBUG_BREAK() __builtin_debugtrap()
#else
	#define DEBUG_BREAK() throw
	//#define DEBUG_BREAK() asm { int3 }
#endif