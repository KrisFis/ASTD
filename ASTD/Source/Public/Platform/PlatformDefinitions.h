
#pragma once

#include "Build/BuildDefinitions.h"
#include "Build/PreprocessorHelpers.h"

#if PLATFORM_WINDOWS

#include "Platform/Win32/WindowsDefinitions.h"
#define PLATFORM_HEADER(name) STRINGIFY(Platform/Win32/Windows ## name.h)

#elif PLATFORM_LINUX

#include "Platform/Linux/LinuxDefinitions.h"
#define PLATFORM_HEADER(name) STRINGIFY(Platform/Linux/Linux ## name.h)

#else

#error "Unsupported platform"

#endif