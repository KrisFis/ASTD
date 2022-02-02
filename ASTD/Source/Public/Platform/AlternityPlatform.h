
#pragma once

#include "BuildMacros.h"
#include "PreprocessorMacros.h"

#if PLATFORM_WINDOWS

#include "WindowsPlatform.h"
#define PLATFORM_HEADER(name) STRINGIFY(Windows ## name ## .h)

#elif PLATFORM_LINUX

#include "LinuxPlatform.h"
#define PLATFORM_HEADER(name) STRINGIFY(Linux ## name ## .h)

#else

#error "Platform not supported"

#endif