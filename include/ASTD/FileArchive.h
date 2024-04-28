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
	virtual uint16 GetEndPos() const override
	{
		const uint16 currOff = ftell(_file);
		fseek(_file, 0, SEEK_END);
		const uint16 result = ftell(_file);
		fseek(_file, currOff, SEEK_SET);
		return result;
	}
	FORCEINLINE_DEBUGGABLE virtual uint16 GetPos() const override
	{
		return ftell(_file);
	}
	FORCEINLINE_DEBUGGABLE virtual bool SetPos(uint16 offset) override
	{
		return fseek(_file, offset, SEEK_SET) == 0;
	}
	FORCEINLINE_DEBUGGABLE virtual uint16 ReadRaw(void* ptr, uint16 num) override
	{
		return fread(ptr, sizeof(tchar), num, _file);
	}
	FORCEINLINE_DEBUGGABLE virtual uint16 WriteRaw(const void* ptr, uint16 num) override
	{
		return fwrite(ptr, sizeof(tchar), num, _file);
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
