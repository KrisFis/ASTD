// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Build/Configurations.h"
#include "Core/Platform/PlatformDefinitions.h"

#if DO_CHECKS

	#include <iostream>

	namespace NValidationInternals
	{
		DIAG_WARNINGS_PUSH()
		DIAG_WARNINGS_SUPRESS(DIAG_WARNING_NULL_DEREFERENCE)
		OPTIMIZATIONS_DISABLE();

		FORCEINLINE static void Crash() { *((uint8*)0) = 0; }

		OPTIMIZATIONS_RESET();
		DIAG_WARNINGS_POP()

		static void LogFailed(const char* Expression, const char* File, int32 Line)
		{
			// TODO(jan.kristian.fisera): Make use of native logging, instead of std::cerr
			std::cerr << "Check failed: \"" << Expression << '"' << " at " << File << ':' << Line << std::endl;
		}
	}

	#define CHECK_IMPL(expression)												\
		((!!(expression)) || []()												\
		{ 																		\
			NValidationInternals::LogFailed(#expression, __FILE__, __LINE__);		\
			static bool didBreak = false; 										\
			if(!didBreak) 														\
			{ 																	\
				didBreak = true; DEBUG_BREAK(); 								\
			}																	\
			return false; 														\
		}())

	#define CHECKF_IMPL(expression)												\
		((!!(expression)) || []()												\
		{ 																		\
			NValidationInternals::LogFailed(#expression, __FILE__, __LINE__);	\
			DEBUG_BREAK();														\
			NValidationInternals::Crash();										\
			return false; 														\
		}())

#else

	#define CHECK_IMPL(expression) (!!(expression))
	#define CHECKF_IMPL(expression) (!!(expression))

#endif
