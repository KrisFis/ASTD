// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Platform/Base/BaseTime.h"

#include <time.h>

struct SLinuxPlatformTime : public SBasePlatformTime
{
	static double GetSecondsSinceEpoch()
	{
		timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		return (double)ts.tv_sec + (double)ts.tv_nsec / NS_PER_SECOND;
	}
};