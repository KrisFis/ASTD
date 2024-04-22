// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <time.h>

struct SLinuxPlatformTime
{
	static double GetSecondsSinceEpoch()
	{
		timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		return (double)ts.tv_sec + (double)ts.tv_nsec / 1.e9;
	}
};
