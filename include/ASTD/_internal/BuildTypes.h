// Copyright Alternity Arts. All Rights Reserved

#pragma once

#define PTR_DIFF(Ptr1, Ptr2) static_cast<int64>(Ptr1 - Ptr2)
#define PTR_DIFF_TYPED(RetType, Ptr1, Ptr2) static_cast<RetType>(Ptr1 -Ptr2)

#if defined(TEXT)
	#undef TEXT
#endif

#define ANSITEXT(text) text
#define WIDETEXT(text) L ## text

#if ASTD_USE_UNICODE
	#define TEXT(text) WIDETEXT(text)
	typedef wchar tchar;
#else
	#define TEXT(text) ANSITEXT(text)
	typedef char tchar;
#endif

#define INDEX_NONE -1
#define CHAR_TERM '\0'
#define CHAR_SLASH '/'
#define CHAR_NEWLINE '\n'