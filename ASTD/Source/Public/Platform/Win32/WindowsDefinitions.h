
#pragma once

#include <intrin.h>

#define FORCEINLINE __forceinline
#define FORCENOINLINE __declspec(noinline)
#define FORCEINLINE_DEBUGGABLE __inline

#define DEBUG_BREAK() __debugbreak()