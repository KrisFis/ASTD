
#pragma once

#include "Core/Platform/Generic/GenericTime.h"

#include <time.h>

struct SLinuxPlatformTime : public SGenericPlatformTime
{
	static double GetSecondsSinceEpoch()
	{
		timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		return (double)ts.tv_sec + (double)ts.tv_nsec / NS_PER_SECOND;
	}
};