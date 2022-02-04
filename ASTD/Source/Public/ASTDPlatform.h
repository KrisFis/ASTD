
#pragma once

#include "ASTDBuild.h"

#if PLATFORM_WINDOWS

#include "Platform/Win32/WindowsPlatform.h"
#define PLATFORM_HEADER(name) STRINGIFY(Platform/Win32/Windows ## name.h)

#elif PLATFORM_LINUX

#include "Platform/Linux/LinuxPlatform.h"
#define PLATFORM_HEADER(name) STRINGIFY(Platform/Linux/Linux ## name.h)

#else

#error "Platform not supported"

#endif