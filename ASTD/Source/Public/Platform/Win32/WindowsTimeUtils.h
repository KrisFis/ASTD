
#pragma once

#include "WindowsTypes.h"

namespace NPlatformTimeUtils
{
	static constexpr double MS_PER_SECOND = 10e2;
	static constexpr double MICROS_PER_SECOND = 10e5;
	static constexpr double NS_PER_SECOND = 10e8;

	static double GetSecondsSinceEpoch()
	{
		__int64 wintime; GetSystemTimeAsFileTime((FILETIME*)&wintime);
		wintime -=116444736000000000i64;  //1jan1601 to 1jan1970
		return (double)(wintime / 10000000i64) + (double)(wintime % 10000000i64 * 100) / NS_PER_SECOND;
	}
}