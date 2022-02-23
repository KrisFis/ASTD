
#pragma once

#include "Platform/PlatformDefinitions.h"

// As default ensures are used for debug build
#if !defined(DO_ENSURES)
	#define DO_ENSURES BUILD_DEBUG
#endif

#if DO_ENSURES

	#include <iostream>

	#define ENSURE_IMPL(expression, file, line, breakOnce) 													\
		((!!(expression)) || []()																			\
		{ std::cout << "Ensure failed [" << file << ":" << line << "]" << std::endl;						\
		if(breakOnce) { static bool didBreak = false; if(!didBreak) { didBreak = true; DEBUG_BREAK(); }}	\
		else DEBUG_BREAK();																					\
		return false; }())

#else

	#define ENSURE_IMPL(...)

#endif