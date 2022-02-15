
#pragma once

#include "Memory/MemoryUtilities.h"
#include "Containers/Array/Allocator/ArrayAllocator.h"

// Default array allocator but with fixed num of elements
template<uint32 InNumLimit>
class TFixedArrayAllocator
{

public: // Operators

	template<uint32 OtherLimit>
	FORCEINLINE bool operator==(const TFixedArrayAllocator<OtherLimit>& Other) { return Allocator == Other.Allocator; }

	template<uint32 OtherLimit>
	FORCEINLINE bool operator!=(const TFixedArrayAllocator<OtherLimit>& Other) { return !operator==(Other); }

public: // Getters

	FORCEINLINE void* GetData() const { return Allocator.GetData(); }
	FORCEINLINE uint64 GetSize() const { return Allocator.GetSize(); }

public: // Manipulation

	FORCEINLINE void* Allocate(uint32 ElementSize, uint32 Num)
	{
		ENSURE_RET(((Allocator.GetSize() / ElementSize) + Num) <= InNumLimit, nullptr);
		return Allocator.Allocate(ElementSize, Num);
	}

	FORCEINLINE void Reset() { Allocator.Reset(); }

private: // Fields

	CArrayAllocator Allocator;

};