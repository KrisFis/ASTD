
#pragma once

#include "Platform/Generic/GenericCString.h"

#include <stdlib.h>

struct SWindowsPlatformCString : public SGenericPlatformCString
{
	// Converts the string pointed to, by the argument str to an integer
	FORCEINLINE static int32 ToInt32(const char* Value) { return atoi(Value); }
	FORCEINLINE static int32 ToInt32(const wchar* Value) { return _wtoi(Value); }

	// Converts integer value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromInt32(int32 Value, char* Buffer) { return _itoa(Value, Buffer, 10); }
	FORCEINLINE static wchar* FromInt32(int32 Value, wchar* Buffer) { return _itow(Value, Buffer, 10); }

	// Converts the string pointed to, by the argument str to a long integer
	FORCEINLINE static int64 ToInt64(const char* Value) { return _atoi64(Value); }
	FORCEINLINE static int64 ToInt64(const wchar* Value) { return _wtoi64(Value); }

	// Converts integer value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromInt64(int64 Value, char* Buffer) { return _i64toa(Value, Buffer, 10); }
	FORCEINLINE static wchar* FromInt64(int64 Value, wchar* Buffer) { return _i64tow(Value, Buffer, 10); }

	// Converts the string pointed to, by the argument str to a floating-point number
	FORCEINLINE static double ToDouble(const char* Value) { return atof(Value); }
	FORCEINLINE static double ToDouble(const wchar* Value) { return _wtof(Value); }

	// Converts double value to string in specified base, string is saved to provided buffer
	FORCEINLINE static char* FromDouble(double Value, int32 Digits, char* Buffer) { return _gcvt(Value, Digits, Buffer); }
	static wchar* FromDouble(double Value, int32 Digits, wchar* Buffer) 
	{ 
		char buf[_CVTBUFSIZE];
		_gcvt(Value, Digits, buf);

		for(uint16 i = 0;;++i)
		{
			Buffer[i] = (wchar)buf[i];
			if(buf[i] == '\0')
				break;
		}

		return Buffer; 
	}
};