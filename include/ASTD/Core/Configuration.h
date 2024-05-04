// Copyright Alternity Arts. All Rights Reserved

#pragma once

// Whether we should use unicode
#ifndef ASTD_USE_UNICODE
	#define ASTD_USE_UNICODE PLATFORM_WINDOWS
#endif

// Whether we should allow checks in build. See Check.h
#ifndef ASTD_DO_CHECKS
	#define ASTD_DO_CHECKS BUILD_DEBUG
#endif

// Whether we want to track SMemory allocations, see Memory.h
#ifndef ASTD_TRACK_MEMORY
	#define ASTD_TRACK_MEMORY BUILD_DEBUG
#endif

// Whether we want "new" and "delete" to use ASTD memory alloc. See Memory.h
#ifndef ASTD_NEW_DELETE
	#define ASTD_NEW_DELETE 1
#endif
