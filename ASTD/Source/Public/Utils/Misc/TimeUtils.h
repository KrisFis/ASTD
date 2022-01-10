
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

		SYSTEMTIME system_time;
		GetSystemTime(&system_time);

		return (double)system_time.wMilliseconds;

#else
		return (double)clock();
#endif
	}
}