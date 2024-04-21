// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Platform/Base/BaseCString.h"

#include <stdlib.h>

struct SWindowsPlatformCString : public SBasePlatformCString
{
	// Converts the string pointed to, by the argument str to an integer
	FORCEINLINE static int32 ToInt32(const char* val) { return atoi(val); }
	FORCEINLINE static int32 ToInt32(const wchar* val) { return _wtoi(val); }

	// Converts integer value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromInt32(int32 val, char* buf) { return _itoa(val, buf, 10); }
	FORCEINLINE static wchar* FromInt32(int32 val, wchar* buf) { return _itow(val, buf, 10); }

	// Converts the string pointed to, by the argument str to a long integer
	FORCEINLINE static int64 ToInt64(const char* val) { return _atoi64(val); }
	FORCEINLINE static int64 ToInt64(const wchar* val) { return _wtoi64(val); }

	// Converts integer value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromInt64(int64 val, char* buf) { return _i64toa(val, buf, 10); }
	FORCEINLINE static wchar* FromInt64(int64 val, wchar* buf) { return _i64tow(val, buf, 10); }

	// Converts the string pointed to, by the argument str to a floating-point number
	FORCEINLINE static double ToDouble(const char* val) { return atof(val); }
	FORCEINLINE static double ToDouble(const wchar* val) { return _wtof(val); }

	// Converts double value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromDouble(double val, int32 Digits, char* buf) { return _gcvt(val, Digits, buf); }
	static wchar* FromDouble(double val, int32 Digits, wchar* buf)
	{
		char tmpBuf[_CVTBUFSIZE];
		_gcvt(val, Digits, tmpBuf);

		for(uint16 i = 0;;++i)
		{
			buf[i] = (wchar)tmpBuf[i];
			if(tmpBuf[i] == '\0')
				break;
		}

		return buf;
	}
};
