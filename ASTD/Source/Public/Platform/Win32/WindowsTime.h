
#pragma once

#include <Windows.h>
#include "Platform/Generic/GenericTime.h"

struct SWindowsPlatformTime : public SGenericPlatformTime
{
	static double GetSecondsSinceEpoch()
	{
		int64 wintime; 
		GetSystemTimeAsFileTime((FILETIME*)&wintime);
		wintime -=116444736000000000i64;  //1jan1601 to 1jan1970
		return (double)(wintime / 10000000i64) + (double)(wintime % 10000000i64 * 100) / NS_PER_SECOND;
	}
};