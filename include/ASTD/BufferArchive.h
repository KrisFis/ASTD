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

	FORCEINLINE void SetData(BufferT<uint8>&& data) { _data = data; _offset = _data.GetNum(); }
	FORCEINLINE const TArray<uint8>& GetData() const { return _data; }

	// SArchive overrides
	/////////////////////////////////

	FORCEINLINE virtual bool IsValid() const override { return true; }
	FORCEINLINE virtual void Flush() override { SetData(TArray<uint8>()); }
	FORCEINLINE virtual SizeType GetMaxOffset() const override { return _data.GetNum(); }
	FORCEINLINE virtual SizeType GetOffset() const override { return _offset; }
	virtual bool SetOffset(SizeType offset) override
	{
		if (_data.IsValidIndex(offset))
		{
			_offset = offset;
			return true;
		}

		return false;
	}
	virtual SizeType ReadBytes(void* ptr, SizeType num) override
	{
		if (!ptr || num == 0) return 0;
		else if (!AllowsRead()) return 0;

		const SizeType bytesToRead = SMath::Min<SizeType>(num, _data.GetNum() - _offset);
		if (bytesToRead > 0)
		{
			SMemory::Copy(ptr, _data.Begin() + _offset, bytesToRead);
			_offset += bytesToRead;
		}
		return bytesToRead;
	}
	virtual SizeType WriteBytes(const void* ptr, SizeType num) override
	{
		if (!ptr || num == 0) return 0;
		else if (!AllowsWrite()) return 0;

		const SizeType newNum = _offset + num;
		if (newNum > _data.GetNum())
		{
			_data.Grow(newNum);
		}

		SMemory::Copy(_data.GetData() + _offset, ptr, num);

		_offset += num;
		return num;
	}

private:
	BufferT<uint8> _data;
	SizeType _offset = 0;
};

typedef TBufferArchive<TArray> SArrayArchive;
