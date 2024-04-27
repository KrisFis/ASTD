// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTD/Core/Build.h"

#include "ASTD/String.h"
#include "ASTD/Array.h"
#include "ASTD/Archive.h"
#include "ASTD/TypeMethods.h"

namespace NArchive
{
	template<typename ElementType>
	static bool DumpToArray(SArchive& ar, TArray<ElementType>& outArray)
	{
		if (!ar.IsValid()) return false;
		else if (!ar.AllowsRead()) return false;

		const uint16 numBytes = ar.GetNumTyped<ElementType>();
		if (numBytes == 0) return false;

		ar.SetPos(0);

		{
			outArray.Resize(numBytes);
			ar.Read(outArray.GetData(), outArray.GetNum());
		}
		return true;
	}

	static bool DumpToString(SArchive& ar, SString& outString)
	{
		TArray<tchar> data;
		if (DumpToArray(ar, data))
		{
			outString = SString(Move(data));
			return true;
		}

		return false;
	}
}
