
#pragma once

#include "Build/Configurations.h"
#include "Platform/PlatformTypes.h"

// Text definition

#if defined(TEXT)
	#undef TEXT
#endif

#if USE_UNICODE
	#define TEXT(text) L ## text
	typedef wchar tchar;
#else
	#define TEXT(text) text
	typedef char tchar;
#endif

#define INDEX_NONE -1
