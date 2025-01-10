// Copyright Alternity Arts. All Rights Reserved

#pragma once

#if PLATFORM_WINDOWS
	#define PLATFORM_NAME Windows
	#define PLATFORM_FOLDER Win32
	#include "ASTD/Win32/WindowsBuild.h"
#elif PLATFORM_LINUX
	#define PLATFORM_NAME Linux
	#include "ASTD/Linux/LinuxBuild.h"
#elif PLATFORM_APPLE
	#define PLATFORM_NAME Apple
	#include "ASTD/Apple/AppleBuild.h"
#else
	#error "Unsupported platform"
#endif

#ifndef PLATFORM_FOLDER
	#define PLATFORM_FOLDER PLATFORM_NAME
#endif

#define PLATFORM_HEADER(name) <PLATFORM_FOLDER/PLATFORM_NAME ## name.h>

#define PLATFORM_TYPE(name) PLATFORM_NAME ## name
#define PLATFORM_PREFIXED_TYPE(prefix, name) CONCAT(CONCAT(prefix, PLATFORM_NAME), name)