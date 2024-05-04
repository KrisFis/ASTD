// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"

#include "ASTD/ArrayArchive.h"

template<int32 FileNo, typename AllocatorT = typename TArray<tchar>::AllocatorType>
struct TPrintArchive : public TArrayArchive<tchar, AllocatorT>
{
	typedef TArrayArchive<tchar, AllocatorT> Super;
	typedef typename Super::SizeType SizeType;

	static_assert(FileNo >= 0 && FileNo <= 2, "Unsupported file no");

	TPrintArchive()
		: Super(EArchiveType::String, FileNo == SMisc::STDIN_FILE_NO ? EArchiveMode::Read : EArchiveMode::Write)
	{}

	virtual ~TPrintArchive() override
	{
		TPrintArchive::Flush();
	}

	// File
	/////////////////////////

	FORCEINLINE int32 GetFileNo() const { return FileNo; }

	// SArchive overrides
	/////////////////////////////////

	virtual void Flush() override
	{
		if (Super::AllowsWrite())
		{
			const auto& data = Super::GetData();
			if (!data.IsEmpty())
			{
				SMisc::WriteStd(FileNo, data.GetData(), data.GetNum());
			}
		}

		Super::Flush();
	}

	virtual SizeType ReadBytes(void* ptr, SizeType size) override
	{
		if (!ptr || size == 0) return 0;
		else if (!Super::AllowsRead()) return 0;

		// TODO: Save to internal array!
		return SMisc::ReadStd(FileNo, ptr, size);
	}
};

typedef TPrintArchive<SMisc::STDIN_FILE_NO> SInputArchive;
typedef TPrintArchive<SMisc::STDOUT_FILE_NO> SOutputArchive;
typedef TPrintArchive<SMisc::STDERR_FILE_NO> SErrorArchive;
