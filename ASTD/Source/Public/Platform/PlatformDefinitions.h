
#pragma once

#include "Build/BuildDefinitions.h"

#if PLATFORM_WINDOWS
	#include "Platform/Win32/WindowsDefinitions.h"
#elif PLATFORM_LINUX
	#include "Platform/Linux/LinuxDefinitions.h"
#else
	#error "Unsupported platform"
#endif