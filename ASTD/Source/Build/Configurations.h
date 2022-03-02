
#pragma once

#include "Build/BuildDefinitions.h"

// Whether we should use unicode
#if !defined(USE_UNICODE)
	#define USE_UNICODE PLATFORM_WINDOWS
#endif

// As default CHECKs are used for debug build
#if !defined(DO_VALIDATION)
	#define DO_VALIDATION BUILD_DEBUG
#endif