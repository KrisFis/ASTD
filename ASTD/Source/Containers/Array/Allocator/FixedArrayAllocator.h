
#pragma once

#include "Memory/MemoryUtilities.h"
#include "Containers/Array/Allocator/ArrayAllocator.h"

// Default array allocator but with fixed num of elements
template<uint32 InNumLimit>
class TFixedArrayAllocator
{

public: // Typedef

	typedef CArrayAllocator::SizeType SizeType;

public: // Operators

	template<uint32 OtherLimit>
	FORCEINLINE bool operator==(const TFixedArrayAllocator<OtherLimit>& Other) { return Allocator == Other.Allocator; }

	template<uint32 OtherLimit>
	FORCEINLINE bool operator!=(const TFixedArrayAllocator<OtherLimit>& Other) { return !operator==(Other); }

public: // Getters

	FORCEINLINE void* GetData() const { return Allocator.GetData(); }
	FORCEINLINE void SetData(void* InData) { Allocator.SetData(InData); }
	FORCEINLINE SizeType GetCount() const { return Allocator.GetCount(); }
	FORCEINLINE void GetCount(SizeType InCount) { Allocator.SetCount(InCount); }

public: // Manipulation

	FORCEINLINE void* Allocate(uint32 ElementSize, SizeType Num)
	{
		CHECK_RET(GetCount() + Num <= InNumLimit, nullptr);
		return Allocator.Allocate(ElementSize, Num);
	}

	FORCEINLINE void Release() { Allocator.Release(); }

private: // Fields

	CArrayAllocator Allocator;
};