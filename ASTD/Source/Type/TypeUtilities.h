
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

// Int definitions

#define INDEX_NONE -1

// Other

// NOTE(jan.kristian.fisera): Hope this defines all compilers
// * Not able to replicate this class
template<typename T>
using TInitializerList = std::initializer_list<T>;