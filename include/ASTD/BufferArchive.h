// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"

#include "ASTD/Archive.h"
#include "ASTD/Array.h"
#include "ASTD/TypeMethods.h"

template<template<typename> class BufferT>
struct TBufferArchive : public SArchive
{
	static_assert(TIsDynamicContainer<BufferT<uint8>>::Value, "Buffer Archive needs to contain dynamic container!");

	FORCEINLINE TBufferArchive(EArchiveMode mode, BufferT<uint8>&& data)
		: SArchive(EArchiveType::Binary, mode)
	{
		SetData(Forward(data));
	}

	FORCEINLINE virtual ~TBufferArchive() override
	{
		TBufferArchive::Flush();
	}

	// Data set/get
	/////////////////////////////////

	FORCEINLINE void SetData(BufferT<uint8>&& data) { _data = data; _pos = _data.GetNum() - 1; }
	FORCEINLINE const TArray<uint8>& GetData() const { return _data; }

	// SArchive overrides
	/////////////////////////////////

	FORCEINLINE virtual bool IsValid() const override { return true; }
	FORCEINLINE virtual void Flush() override { SetData(TArray<uint8>()); }
	FORCEINLINE virtual uint16 GetEndPos() const override { return _data.GetNum(); }
	FORCEINLINE virtual uint16 GetPos() const override { return _pos; }
	virtual bool SetPos(uint16 offset) override
	{
		if (_data.IsValidIndex(offset))
		{
			_pos = offset;
			return true;
		}

		return false;
	}
	virtual uint16 ReadRaw(void* ptr, uint16 num) override
	{
		if (!ptr || num == 0) return 0;
		else if (!AllowsRead()) return 0;

		const uint16 bytesToRead = SMath::Min<uint16>(num, _data.GetNum() - _pos);
		if (bytesToRead > 0)
		{
			SMemory::Copy(ptr, _data.begin() + _pos, bytesToRead);
			_pos += bytesToRead;
		}
		return bytesToRead;
	}
	virtual uint16 WriteRaw(const void* ptr, uint16 num) override
	{
		if (!ptr || num == 0) return 0;
		else if (!AllowsWrite()) return 0;

		const int32 newNum = _pos + num;
		if (newNum > _data.GetNum())
		{
			_data.Grow(newNum);
		}

		SMemory::Copy(_data.GetData() + _pos, ptr, num);

		_pos += num;
		return num;
	}

private:
	BufferT<uint8> _data;
	int32 _pos = -1;
};

typedef TBufferArchive<TArray> SArrayArchive;
