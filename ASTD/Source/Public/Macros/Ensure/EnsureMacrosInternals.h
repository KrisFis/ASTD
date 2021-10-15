
#pragma once

#if DEBUG

	#include "PreprocessorMacros.h"
	#include "PlatformMacros.h"
	#include <iostream>

	#define ENSURE_IMPL(Condition, File, Line, BreakOnlyOnce) 													\
		([&](bool ConditionResult)																				\
		{ if(ConditionResult) return true;																		\
		std::cout << "Ensure condition has failed [" << File << ":" << Line << "]" << std::endl;	\
		if(BreakOnlyOnce) { static bool didBreak = false; if(!didBreak) { didBreak = true; DEBUG_BREAK(); }}	\
		else DEBUG_BREAK();																						\
		return false; }(Condition))

#else

	#define ENSURE_IMPL(...)
	#define ENSURE_CALL_IMPL(...)

#endif