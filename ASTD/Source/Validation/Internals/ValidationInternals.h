
#pragma once

#include "Platform/PlatformDefinitions.h"

// As default CHECKs are used for debug build
#if !defined(DO_VALIDATION)
	#define DO_VALIDATION BUILD_DEBUG
#endif

#if DO_VALIDATION

	#include <iostream>

	#define CHECK_IMPL(expression, file, line, breakOnce) 													\
		((!!(expression)) || []()																			\
		{ std::cout << "CHECK failed [" << file << ":" << line << "]" << std::endl;						\
		if(breakOnce) { static bool didBreak = false; if(!didBreak) { didBreak = true; DEBUG_BREAK(); }}	\
		else DEBUG_BREAK();																					\
		return false; }())

#else

	#define CHECK_IMPL(...)

#endif