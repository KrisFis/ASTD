// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Memory.h"
#include "Containers/Array/Allocator/ArrayAllocator.h"

// Default array allocator but with fixed num of elements
template<typename InElementType, uint32 InNumLimit>
class TFixedArrayAllocator
{

public: // Typedef

	typedef InElementType ElementType;
	typedef typename TArrayAllocator<ElementType>::SizeType SizeType;

public: // Getters

	FORCEINLINE ElementType* GetData() const { return Allocator.GetData(); }
	FORCEINLINE void SetData(ElementType* InData) { Allocator.SetData(InData); }
	FORCEINLINE SizeType GetCount() const { return Allocator.GetCount(); }
	FORCEINLINE void GetCount(SizeType InCount) { Allocator.SetCount(InCount); }

public: // Manipulation

	FORCEINLINE ElementType* Allocate(SizeType Num)
	{
		CHECK_RET(GetCount() + Num <= InNumLimit, nullptr);
		return Allocator.Allocate(Num);
	}

	FORCEINLINE void Release() { Allocator.Release(); }

private: // Fields

	TArrayAllocator<ElementType> Allocator;
};