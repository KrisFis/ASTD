// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"

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
	// Types
	/////////////////////////

	typedef int64 SizeType;

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
	virtual void Flush() = 0;

	// Position
	/////////////////////////

	FORCEINLINE bool GetIsBegin() const { return GetOffset() == 0; }
	FORCEINLINE bool GetIsEnd() const { return GetOffset() == GetMaxOffset(); }

	// Gets number of bytes
	FORCEINLINE SizeType GetNumBytes() const { return GetMaxOffset(); }
	FORCEINLINE bool IsEmpty() const { return GetMaxOffset() == 0; }

	template<typename T>
	inline uint16 GetNum() const
	{
		if constexpr (sizeof(T) == sizeof(uint8))
		{
			return GetNumBytes();
		}
		else
		{
			const uint16 allBytes = GetNumBytes();
			static constexpr const uint16 tBytes = sizeof(T);

			uint16 result = 0;
			if (allBytes > 0)
			{
				const uint16 reminder = allBytes % tBytes;
				result = (allBytes - reminder) / tBytes;
			}

			return result;
		}
	}

	// Gets end position
	virtual SizeType GetMaxOffset() const = 0;
	FORCEINLINE SizeType GCount() const { return GetMaxOffset(); }

	// Gets current position (tell)
	virtual SizeType GetOffset() const = 0;
	FORCEINLINE SizeType Tell() const { return GetOffset(); }

	// Moves to position (seek)
	virtual bool SetOffset(SizeType offset) = 0;
	FORCEINLINE bool Seek(SizeType offset) { return SetOffset(offset); }

	// Read / Write
	/////////////////////////

	// Reads bytes and moves while doing so
	virtual SizeType ReadBytes(void* ptr, SizeType num) = 0;

	template<typename T>
	FORCEINLINE SizeType Read(T* ptr, SizeType num) { return ReadBytes(ptr, sizeof(T) * num); }

	// Writes bytes and moves while doing so
	virtual SizeType WriteBytes(const void* ptr, SizeType num) = 0;

	template<typename T>
	FORCEINLINE SizeType Write(const T* ptr, SizeType num) { return WriteBytes(ptr, sizeof(T) * num); }

	// Packet
	// * Packet is a bunch of bytes basically
	/////////////////////////

	// Func: (const void* packet, SizeType numOfBytes) -> bool
	template<SizeType MaxPacketSize, typename FuncType>
	bool ReadPacketsUntil(FuncType&& func)
	{
		uint8 packet[MaxPacketSize];
		while(true)
		{
			const SizeType readBytes = ReadRaw(&packet, MaxPacketSize);
			if (readBytes > 0)
			{
				if (func((const void*)&packet, readBytes)) break;
			}

			if (readBytes < MaxPacketSize)
			{
				break;
			}
		}

		return true;
	}

	// Func: (const void* packet, SizeType numOfBytes) -> bool
	template<SizeType MaxPacketSize, typename FuncType>
	FORCEINLINE_DEBUGGABLE bool ReadPacketsUntil(SizeType startOffset, FuncType&& func)
	{
		if (!SetOffset(startOffset)) return false;
		return ReadPacketsUntil<MaxPacketSize>(Forward(func));
	}

	// Container
	// * Takes all data and copies to provided container
	/////////////////////////

	template<typename ContainerT, typename ContainerTT = TContainerTypeTraits<ContainerT>>
	typename TEnableIf<ContainerTT::IsContainer, bool>::Type CopyToContainer(ContainerT& outContainer)
	{
		if (!AllowsRead()) return false;

		const uint16 numBytes = GetNumBytes();
		if (numBytes == 0) return false;

		SetOffset(0);
		outContainer.Resize(numBytes);
		*this >> outContainer;
		return true;
	}

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
		const uint16 remainingBytes = (otherAr.GetMaxOffset() - otherAr.GetOffset());
		if (remainingBytes > 0)
		{
			uint8* buffer = SMemory::AllocateElement<uint8>(remainingBytes);
			{
				otherAr.ReadBytes(buffer, remainingBytes);
				ar.WriteBytes(buffer, remainingBytes);
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

template<typename ContainerT, typename ContainerTT = TContainerTypeTraits<ContainerT>>
inline static typename TEnableIf<ContainerTT::IsContainer, SArchive&>::Type operator<<(SArchive& ar, const ContainerT& container)
{
	if constexpr (ContainerTT::InlineMemory)
	{
		ar.Write(container.Begin(), container.GetNum());
	}
	else
	{
		for (auto it = container.Begin(); it != container.End(); ++it)
		{
			ar << *it;
		}
	}

	return ar;
}

template<typename ContainerT, typename ContainerTT = TContainerTypeTraits<ContainerT>>
inline static typename TEnableIf<ContainerTT::IsContainer, SArchive&>::Type operator>>(SArchive& ar, ContainerT& container)
{
	if constexpr (ContainerTT::InlineMemory)
	{
		ar.Read(container.Begin(), container.GetNum());
	}
	else
	{
		for (auto it = container.begin(); it != container.end(); ++it)
		{
			ar >> *it;
		}
	}

	return ar;
}

static SArchive& operator<<(SArchive& ar, const int32 val)
{
	if (ar.IsBinary())
	{
		ar.Write(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_INT32];
		if (SCString::FromInt32(val, buffer, SCString::MAX_BUFFER_SIZE_INT32))
		{
			ar.WriteBytes(buffer, SCString::GetLength(buffer));
		}
	}

	return ar;
}

static SArchive& operator>>(SArchive& ar, int32& val)
{
	if (ar.IsBinary())
	{
		ar.Read(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_INT32];
		{
			tchar c;
			uint16 i = 0;
			while (i < SCString::MAX_BUFFER_SIZE_INT32 - 1 && ar.ReadBytes(&c, 1) == 1)
			{
				if ((c >= '0' && c <= '9') || c == '-')
				{
					buffer[i++] = c;
				}
				else
				{
					ar.SetOffset(ar.GetOffset() - 1);
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
		ar.Write(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_INT64];
		if (SCString::FromInt64(val, buffer, SCString::MAX_BUFFER_SIZE_INT64))
		{
			ar.WriteBytes(buffer, SCString::GetLength(buffer));
		}
	}

	return ar;
}

static SArchive& operator>>(SArchive& ar, int64& val)
{
	if (ar.IsBinary())
	{
		ar.Read(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_INT64];
		{
			tchar c;
			uint16 i = 0;
			while (i < SCString::MAX_BUFFER_SIZE_INT64 - 1 && ar.ReadBytes(&c, 1) == 1)
			{
				if ((c >= '0' && c <= '9') || c == '-')
				{
					buffer[i++] = c;
				}
				else
				{
					ar.SetOffset(ar.GetOffset() - 1);
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
		ar.Write(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_DOUBLE];
		if (SCString::FromDouble(val, 4, buffer, SCString::MAX_BUFFER_SIZE_DOUBLE))
		{
			ar.WriteBytes(buffer, SCString::GetLength(buffer));
		}
	}

	return ar;
}

static SArchive& operator>>(SArchive& ar, double& val)
{
	if (ar.IsBinary())
	{
		ar.Read(&val, 1);
	}
	else if (ar.IsString())
	{
		thread_local tchar buffer[SCString::MAX_BUFFER_SIZE_DOUBLE];
		{
			tchar c;
			uint16 i = 0;
			while (i < SCString::MAX_BUFFER_SIZE_DOUBLE - 1 && ar.ReadBytes(&c, 1) == 1)
			{
				if ((c >= '0' && c <= '9') || c == '.' || c == '-')
				{
					buffer[i++] = c;
				}
				else
				{
					ar.SetOffset(ar.GetOffset() - 1);
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
	ar.WriteBytes(&val, 1);
	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, tchar& val)
{
	ar.ReadBytes(&val, 1);
	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator<<(SArchive& ar, const tchar* val)
{
	ar.WriteBytes(val, SCString::GetLength(val));
	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, tchar* val)
{
	ar.ReadBytes(val, SCString::GetLength(val));
	return ar;
}
