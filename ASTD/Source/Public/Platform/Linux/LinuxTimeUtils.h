
#pragma once

#include "Platform/Base/BaseTime.h"

#include <time.h>

struct SPlatformTime : public SBasePlatformTime
{
	static double GetSecondsSinceEpoch()
	{
		timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		return (double)ts.tv_sec + (double)ts.tv_nsec / NS_PER_SECOND;
	}
};