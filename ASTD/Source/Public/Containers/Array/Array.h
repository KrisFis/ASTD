
#pragma once

#include "Platform/PlatformTypes.h"
#include "TypeTraits/TypeTraits.h"
#include "Containers/Array/Allocator/ArrayAllocator.h"

template<typename InElementType, typename InAllocator = CArrayAllocator>
class TArray
{

	static_assert(!TIsSame<InAllocator, void>::Value, "Invalid allocator type");

public: // Typedefs

	typedef InElementType ElementType;
	typedef InAllocator AllocatorType;

public: // Constructors

	FORCEINLINE TArray() : Allocator(), Count(0), Reserved(0) {}
	FORCEINLINE TArray(const TArray& Other) { CopyFrom(Other); }
	FORCEINLINE TArray(TArray&& Other) { MoveFrom(Move(Other)); }

public: // Destructor

	~TArray() {}

public: // Property getters

	FORCEINLINE const ElementType* GetData() const { return GetDataImpl(); }
	FORCEINLINE ElementType* GetData() { return GetDataImpl(); }

	FORCEINLINE uint32 GetCount() const { return Count; }
	FORCEINLINE uint32 GetReserved() const { return Reserved; }

public: // Validations

	FORCEINLINE bool IsValidIndex(uint32 Idx) const { return Idx < Count; }

public: // Reservation

	void Reserve(uint32 Num)
	{
		if(Num <= Reserved) return;
		GrowToImpl(Num);
	}
	
public: // Add

	void Add(const ElementType&)
	{
		// IMPLEMENT!
	}

	void Add(ElementType&&)
	{
		// IMPLEMENT!
	}

private: // Helpers -> Getters

	FORCEINLINE ElementType* GetDataImpl() const { return (ElementType*)Allocator.GetData(); }
	FORCEINLINE ElementType* GetElementAtImpl(uint32 Idx) const { return (ElementType*)Allocator.GetData() + Idx; }

private: // Helpers -> Allocations

	FORCEINLINE void GrowToImpl(uint32 Num) { GrowByImpl(Num - Reserved); }
	FORCEINLINE void ShrinkToImpl(uint32 Num) { ShrinkByImpl(Reserved - Num); }

	void GrowByImpl(uint32 Num)
	{
		Allocator.Allocate(SizeOf<ElementType>(), Num);
		Reserved += Num;
	}

	void ShrinkByImpl(uint32 Num)
	{
		const uint32 newReserved = Reserved - Num;
		const uint32 newCount = SMath::Min(newReserved, Count);

		for(uint32 i = Count - 1; i >= newCount; --i)
		{
			SMemoryUtilities::CallDestructor(
				GetElementAtImpl(i)
			);
		}

		AllocatorType tmpAllocator;
		tmpAllocator.Allocate(SizeOf<ElementType>(), newReserved);
		SMemory::Move(tmpAllocator.GetData(), Allocator.GetData(), newCount);

		Allocator.Release();
		Allocator.Replace(tmpAllocator.GetData(), tmpAllocator.GetSize());
		tmpAllocator.Replace(nullptr, 0);

		Count = newCount;
		Reserved = newReserved;
	}

private: // Helpers -> Cross manipulation (Array)

	FORCEINLINE void CopyTo(TArray& Other) { Other.CopyFrom(*this); }

	void CopyFrom(const TArray& Other)
	{
		Allocator.Release();
		Allocator.Allocate(Other.Reserved);

		SMemory::MemoryCopyTyped<ElementType>(
			Allocator.GetData(), 
			Other.Allocator.GetData(), 
			Other.Count
		);

		Count = Other.Count;
		Reserved = Other.Reserved;
	}

	FORCEINLINE void MoveTo(TArray& Other) { Other.MoveFrom(*this); }

	template<typename OtherArrayType>
	void MoveFrom(OtherArrayType&& Other)
	{
		Allocator.Release();
		Allocator.Replace(
			Other.Allocator.GetData(), 
			Other.Allocator.GetSize()
		);

		Count = Other.Count;
		Reserved = Other.Reserved;

		{
			Other.Allocator.Replace(nullptr, 0);
			Other.Count = 0;
			Other.Reserved = 0;
		}
	}

private: // Fields

	AllocatorType Allocator;
	uint32 Count;
	uint32 Reserved;
};