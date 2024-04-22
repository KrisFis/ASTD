// Copyright Alternity Arts. All Rights Reserved

#pragma once

// Whether we should use unicode
#ifndef USE_UNICODE
	#define USE_UNICODE PLATFORM_WINDOWS
#endif

// Whether we should allow checks in build. See Check.h
#ifndef DO_CHECKS
	#define DO_CHECKS BUILD_DEBUG
#endif

// Whether ASTD can override new/delete. See Memory.h
#ifndef ASTD_OVERRIDE_NEW_DELETE
	#define ASTD_OVERRIDE_NEW_DELETE 1
#endif
