
#pragma once

#include "Build/Configurations.h"
#include "Platform/PlatformTypes.h"

// Text definition

#if defined(TEXT)
	#undef TEXT
#endif

#define ANSITEXT(text) text
#define WIDETEXT(text) L ## text

#if USE_UNICODE
	#define TEXT(text) WIDETEXT(text)
	typedef wchar tchar;
#else
	#define TEXT(text) ANSITEXT(text)
	typedef char tchar;
#endif

#define INDEX_NONE -1
#define CHAR_TERM '\0'