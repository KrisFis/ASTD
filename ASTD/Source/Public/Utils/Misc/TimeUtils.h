
#pragma once

#include "ASTDCore.h"

#if PLATFORM_LINUX
#include <time.h>
#elif PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <ctime>
#endif

namespace NTimeUtils
{
	static constexpr double MS_PER_SECOND = 10e2;
	static constexpr double MICROS_PER_SECOND = 10e5;
	static constexpr double NS_PER_SECOND = 10e8;

	static double GetSecondsSinceEpoch()
	{
#if PLATFORM_LINUX

		timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		return (double)ts.tv_sec + (double)ts.tv_nsec / NS_PER_SECOND;

#elif PLATFORM_WINDOWS

		__int64 wintime; GetSystemTimeAsFileTime((FILETIME*)&wintime);
		wintime -=116444736000000000i64;  //1jan1601 to 1jan1970
		return (double)(wintime / 10000000i64) + (double)(wintime % 10000000i64 * 100) / NS_PER_SECOND;

#else
		return (double)clock();
#endif
	}
}