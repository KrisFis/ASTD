
#pragma once

#include "Build/BuildDefinitions.h"
#include "TypeTraits/TypeTraits.h"
#include "Platform/PlatformMemory.h"

// Allocation info
///////////////////////////////////////////////////////////

#if BUILD_DEBUG

void* operator new(TSize Size) 
{
	std::cout << "Allocated memory of size [" << Size << "]\n";
	return ::operator new(Size);
}

void operator delete(void* Ptr, TSize Size) noexcept
{
	std::cout << "Deallocated memory of size [" << Size << "]\n";
	::operator delete(Ptr, Size);
}

#endif // BUILD_DEBUG

// Types
///////////////////////////////////////////////////////////

struct SMemory : public SPlatformMemory
{
	template<typename T, typename TEnableIf<!TIsSame<T, void>::Value>::Type* = nullptr>
	FORCEINLINE static T* AllocateTyped(uint32 Count) 
	{ return reinterpret_cast<T*>(SPlatformMemory::Allocate(TSizeOf<T>() * Count)); }

	template<typename T, typename TEnableIf<!TIsSame<T, void>::Value>::Type* = nullptr>
	FORCEINLINE static T* AllocateZeroedTyped(uint32 Count) 
	{ return reinterpret_cast<T*>(SPlatformMemory::AllocateZeroed(TSizeOf<T>() * Count)); }

	template<typename T, typename TEnableIf<!TIsSame<T, void>::Value>::Type* = nullptr>
	FORCEINLINE static void DeallocateTyped(T* Ptr, uint32 Count)
	{ SPlatformMemory::Deallocate(Ptr, TSizeOf<T>() * Count); }

	template<typename T, typename TEnableIf<!TIsSame<T, void>::Value>::Type* = nullptr>
	FORCEINLINE static T* MemoryCopyTyped(T* Destination, T* Source, uint32 Num) 
	{ return reinterpret_cast<T*>(SPlatformMemory::MemoryCopy(Destination, Source, TSizeOf<T>() * Num)); }

	template<typename T, typename TEnableIf<!TIsSame<T, void>::Value>::Type* = nullptr>
	FORCEINLINE static T* MemoryMoveTyped(T* Destination, T* Source, uint32 Num) 
	{ return reinterpret_cast<T*>(SPlatformMemory::MemoryMove(Destination, Source, TSizeOf<T>() * Num)); }
};