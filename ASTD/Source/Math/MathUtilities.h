
#pragma once

#include "Platform/PlatformMath.h"

// Definitions
///////////////////////////////////////////////////////////

#ifdef PI
	#undef PI
#endif

// PI = 3.1415926535897932384626433832795f
#define PI 3.1415926535897932f

#define TINY_NUMBER 1.e-8f
#define SMALL_NUMBER 1.e-4f

// Types
///////////////////////////////////////////////////////////

// Just as typedef for now
typedef SPlatformMath SMath;