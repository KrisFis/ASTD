// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"

#include "ASTD/Check.h"
#include "ASTD/CString.h"

enum class EArchiveType : uint8
{
	Binary = 0,
	String
};

enum class EArchiveMode : uint8
{
	Read = 0,
	Write,
	ReadWrite
};

struct SArchive
{
	// Constructors
	/////////////////////////

	FORCEINLINE SArchive() = delete;
	FORCEINLINE SArchive(EArchiveType type, EArchiveMode mode)
		: _type(type)
		, _mode(mode)
	{}

	FORCEINLINE SArchive(const SArchive& other) = delete;
	FORCEINLINE SArchive(SArchive&& other) = delete;

	FORCEINLINE virtual ~SArchive() = default;

	// Type & Mode
	/////////////////////////

	FORCEINLINE EArchiveType GetType() const { return _type; }
	FORCEINLINE bool IsBinary() const { return _type == EArchiveType::Binary; }
	FORCEINLINE bool IsString() const { return _type == EArchiveType::String; }

	FORCEINLINE EArchiveMode GetMode() const { return _mode; }
	FORCEINLINE bool AllowsRead() const { return _mode == EArchiveMode::Read || _mode == EArchiveMode::ReadWrite; }
	FORCEINLINE bool AllowsWrite() const { return _mode == EArchiveMode::Write || _mode == EArchiveMode::ReadWrite; }

	// Essentials
	/////////////////////////

	virtual bool IsValid() const = 0;

	virtual void Reset() = 0;

	// Position
	/////////////////////////

	FORCEINLINE bool GetIsBegin() const { return GetPos() == 0; }
	FORCEINLINE bool GetIsEnd() const { return GetPos() == GetEndPos(); }

	// Gets number of bytes
	FORCEINLINE uint16 GetNum() const { return GetEndPos(); }
	FORCEINLINE bool IsEmpty() const { return GetNum() > 0; }

	template<typename T>
	inline uint16 GetNumTyped() const
	{
		if constexpr (sizeof(T) == sizeof(uint8))
		{
			return GetNum();
		}
		else
		{
			const uint16 endPos = GetEndPos();
			static constexpr const uint16 tSize = sizeof(T);

			uint16 result = 0;
			if (endPos > 0)
			{
				const uint16 reminder = endPos % tSize;
				result = (endPos - reminder) / tSize;
			}

			return result;
		}
	}

	// Gets end position
	virtual uint16 GetEndPos() const = 0;
	FORCEINLINE uint16 GCount() const { return GetEndPos(); }

	// Gets current position (tell)
	virtual uint16 GetPos() const = 0;
	FORCEINLINE uint16 Tell() const { return GetPos(); }

	// Moves to position (seek)
	virtual bool SetPos(uint16 offset) = 0;
	FORCEINLINE bool Seek(uint16 offset) { return SetPos(offset); }

	// Read / Write
	/////////////////////////

	///// Reads bytes and moves while doing so
	virtual uint16 Read(void* ptr, uint16 num) = 0;

	template<typename T>
	FORCEINLINE uint16 ReadTyped(T* ptr, uint16 num) { return Read(ptr, sizeof(T) * num); }

	// Writes bytes and moves while doing so
	virtual uint16 Write(const void* ptr, uint16 num) = 0;

	template<typename T>
	FORCEINLINE uint16 WriteTyped(const T* ptr, uint16 num) { return Write(ptr, sizeof(T) * num); }

private:
	EArchiveType _type = EArchiveType::Binary;
	EArchiveMode _mode = EArchiveMode::ReadWrite;
};

// Archive operator<< && operator>>
////////////////////////////////////////////

static SArchive& operator<<(SArchive& ar, SArchive& otherAr)
{
	if (ar.GetMode() == otherAr.GetMode() && otherAr.AllowsRead())
	{
		const uint16 remainingBytes = (otherAr.GetEndPos() - otherAr.GetPos());
		if (remainingBytes > 0)
		{
			uint8* buffer = SMemory::AllocateElement<uint8>(remainingBytes);
			{
				otherAr.Read(buffer, remainingBytes);
				ar.Write(buffer, remainingBytes);
			}
			SMemory::DeallocateElement(buffer);
		}
	}

	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, SArchive& otherAr)
{
	otherAr << ar; // just switch streaming
	return ar;
}

static SArchive& operator<<(SArchive& ar, const int32 val)
{
	if (ar.IsBinary())
	{
		ar.WriteTyped(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_INT32];
		if (SCString::FromInt32(val, buffer, SCString::MAX_BUFFER_SIZE_INT32))
		{
			ar.Write(buffer, SCString::GetLength(buffer));
		}
	}

	return ar;
}

static SArchive& operator>>(SArchive& ar, int32& val)
{
	if (ar.IsBinary())
	{
		ar.ReadTyped(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_INT32];
		{
			tchar c;
			uint16 i = 0;
			while (i < SCString::MAX_BUFFER_SIZE_INT32 - 1 && ar.Read(&c, 1) == 1)
			{
				if ((c >= '0' && c <= '9') || c == '-')
				{
					buffer[i++] = c;
				}
				else
				{
					ar.SetPos(ar.GetPos() - 1);
					break;
				}
			}
			buffer[i] = CHAR_TERM;
			val = SCString::ToInt32(buffer);
		}
	}

	return ar;
}

static SArchive& operator<<(SArchive& ar, const int64 val)
{
	if (ar.IsBinary())
	{
		ar.WriteTyped(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_INT64];
		if (SCString::FromInt64(val, buffer, SCString::MAX_BUFFER_SIZE_INT64))
		{
			ar.Write(buffer, SCString::GetLength(buffer));
		}
	}

	return ar;
}

static SArchive& operator>>(SArchive& ar, int64& val)
{
	if (ar.IsBinary())
	{
		ar.ReadTyped(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_INT64];
		{
			tchar c;
			uint16 i = 0;
			while (i < SCString::MAX_BUFFER_SIZE_INT64 - 1 && ar.Read(&c, 1) == 1)
			{
				if ((c >= '0' && c <= '9') || c == '-')
				{
					buffer[i++] = c;
				}
				else
				{
					ar.SetPos(ar.GetPos() - 1);
					break;
				}
			}
			buffer[i] = CHAR_TERM;
			val = SCString::ToInt64(buffer);
		}
	}

	return ar;
}

static SArchive& operator<<(SArchive& ar, const double val)
{
	if (ar.IsBinary())
	{
		ar.WriteTyped(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_DOUBLE];
		if (SCString::FromDouble(val, 4, buffer, SCString::MAX_BUFFER_SIZE_DOUBLE))
		{
			ar.Write(buffer, SCString::GetLength(buffer));
		}
	}

	return ar;
}

static SArchive& operator>>(SArchive& ar, double& val)
{
	if (ar.IsBinary())
	{
		ar.ReadTyped(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_DOUBLE];
		{
			tchar c;
			uint16 i = 0;
			while (i < SCString::MAX_BUFFER_SIZE_DOUBLE - 1 && ar.Read(&c, 1) == 1)
			{
				if ((c >= '0' && c <= '9') || c == '.' || c == '-')
				{
					buffer[i++] = c;
				}
				else
				{
					ar.SetPos(ar.GetPos() - 1);
					break;
				}
			}
			buffer[i] = CHAR_TERM;
			val = SCString::ToDouble(buffer);
		}
	}

	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator<<(SArchive& ar, tchar val)
{
	ar.Write(&val, 1);
	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, tchar& val)
{
	ar.Read(&val, 1);
	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator<<(SArchive& ar, const tchar* val)
{
	ar.Write(val, SCString::GetLength(val));
	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, tchar* val)
{
	ar.Read(val, SCString::GetLength(val));
	return ar;
}
