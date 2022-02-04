
#pragma once

#include "ASTDBuild.h"
#include "ASTDPlatform.h"

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
	#define ENSURE_CALL_IMPL(...)

#endif