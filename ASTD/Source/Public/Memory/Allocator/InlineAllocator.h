
#pragma once

#include "Memory/Allocator/AllocatorBase.h"
#include "Memory/MemoryUtilities.h"

template<typename T>
class TInlineAllocator : public TAllocatorBase<T>
{

public: // Typedefs

	typedef typename TAllocatorBase<T>::ElementType ElementType;

public: // Constructor

	FORCEINLINE TInlineAllocator() : Data(nullptr), Count(0) {}
	FORCEINLINE virtual ~TInlineAllocator() override
	{
		if(Data)
		{
			SMemory::DeallocateTyped(Data, Count);
			Data = nullptr;
		}
	}

public: // TAllocatorBase overrides

	FORCEINLINE virtual void* GetData() const override { return (void*)Data; }
	FORCEINLINE virtual uint32 GetCount() const override { return Count; }

	FORCEINLINE virtual ElementType* Allocate(uint32 Num) override
	{
		// NOTE(jan.kristian.fisera): Unmanaged memory raise in allocations
		ElementType* newData = SMemory::AllocateTyped<ElementType>(Count + Num);
		if(Data)
		{
			SMemory::MemoryMoveTyped(newData, Data, Count);
			SMemory::DeallocateTyped(Data, Count);
		}

		Data = newData;
		Count += Num;

		return Data + Count;
	}

	FORCEINLINE virtual void Deallocate(ElementType* Ptr, uint32 Num) override
	{
		SMemory::DeallocateTyped(Ptr, Num);
	}

	FORCEINLINE virtual void Construct(ElementType* Ptr, const ElementType& Value) override
	{
		::new((void*)Ptr) ElementType(Value);
	}

	FORCEINLINE virtual void Destruct(ElementType* Ptr) override
	{
		Ptr->~ElementType();
	}

private: // Fields

	ElementType* Data;
	uint32 Count;
};