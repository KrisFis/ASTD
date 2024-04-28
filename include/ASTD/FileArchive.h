// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"

#include "Archive.h"

// TODO: Remove direct posix calls
struct SCFileArchive : public SArchive
{
	FORCEINLINE SCFileArchive(EArchiveType type, EArchiveMode mode, const tchar* filename, bool overwrite)
		: SArchive(type, mode)
		, _filename(filename)
		, _overwrite(overwrite)
	{
		OpenImpl();
	}

	FORCEINLINE virtual ~SCFileArchive() override
	{
		CloseImpl();
	}

	FORCEINLINE FILE* GetFile() const { return _file; }

	// SArchive overrides
	/////////////////////////////////

	FORCEINLINE virtual bool IsValid() const override { return !!_file; }
	FORCEINLINE virtual void Flush() override { fflush(_file); }
	virtual SizeType GetMaxOffset() const override
	{
		const SizeType currOff = ftell(_file);
		fseek(_file, 0, SEEK_END);
		const SizeType result = ftell(_file);
		fseek(_file, currOff, SEEK_SET);
		return result;
	}
	FORCEINLINE_DEBUGGABLE virtual SizeType GetOffset() const override
	{
		return ftell(_file);
	}
	FORCEINLINE_DEBUGGABLE virtual bool SetOffset(SizeType offset) override
	{
		return fseek(_file, offset, SEEK_SET) == 0;
	}
	FORCEINLINE_DEBUGGABLE virtual SizeType ReadBytes(void* ptr, SizeType num) override
	{
		return fread(ptr, sizeof(uint8), num, _file);
	}
	FORCEINLINE_DEBUGGABLE virtual SizeType WriteBytes(const void* ptr, SizeType num) override
	{
		return fwrite(ptr, sizeof(uint8), num, _file);
	}

private:
	void OpenImpl()
	{
		if (!_file)
		{
			switch (GetMode())
			{
				case EArchiveMode::Read:
					_file = fopen(_filename, "r");
				break;
				case EArchiveMode::Write:
					_file = fopen(_filename, "w");
				break;
				case EArchiveMode::ReadWrite:
				{
					const tchar* mode = _overwrite ? "w+" : "r+";
					_file = fopen(_filename, mode);
					if (!_file)
					{
						_file = fopen(_filename, "w+");
					}
				}
				break;
			}

		}
	}

	void CloseImpl()
	{
		if (_file)
		{
			fclose(_file);
			_file = nullptr;
		}
	}

	FILE* _file = nullptr;
	const tchar* _filename = nullptr;
	bool _overwrite = false;
};
