// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Platform/Win32/WindowsDefinitions.h"

struct SWindowsPlatformTime
{
	static double GetSecondsSinceEpoch()
	{
		int64 wintime; 
		GetSystemTimeAsFileTime((FILETIME*)&wintime);
		wintime -=116444736000000000i64;  //1jan1601 to 1jan1970
		return (double)(wintime / 10000000i64) + (double)(wintime % 10000000i64 * 100) / 1.e9;
	}
};
