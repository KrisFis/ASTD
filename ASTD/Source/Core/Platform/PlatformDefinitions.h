
#pragma once

#include "Core/Build/BuildDefinitions.h"

#if PLATFORM_WINDOWS
	#include "Core/Platform/Win32/WindowsDefinitions.h"
#elif PLATFORM_LINUX
	#include "Core/Platform/Linux/LinuxDefinitions.h"
#else
	#error "Unsupported platform"
#endif