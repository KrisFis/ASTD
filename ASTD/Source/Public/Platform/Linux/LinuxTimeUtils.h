
#pragma once

#include "LinuxTypes.h"

#include <time.h>

namespace NPlatformTimeUtils
{
	static constexpr double MS_PER_SECOND = 10e2;
	static constexpr double MICROS_PER_SECOND = 10e5;
	static constexpr double NS_PER_SECOND = 10e8;

	static double GetSecondsSinceEpoch()
	{
		timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		return (double)ts.tv_sec + (double)ts.tv_nsec / NS_PER_SECOND;
	}
}