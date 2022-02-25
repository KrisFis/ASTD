
#pragma once

#include "Build/Configurations.h"
#include "Platform/PlatformDefinitions.h"

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