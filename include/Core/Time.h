// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Platform/PlatformDefinitions.h"
#include PLATFORM_HEADER(Time)

struct STime : public PLATFORM_STRUCT(Time)
{
	static constexpr long double NS_PER_SECOND = 1.e9; // nanoseconds
	static constexpr long double MICROS_PER_SECOND = 1.e6; // microseconds
	static constexpr long double MS_PER_SECOND = 1.e3; // miliseconds
	static constexpr long double HOURS_PER_SECOND = 2.7e-4; // hours
	static constexpr long double DAYS_PER_SECOND = 1.1574074e-4; // days
};
