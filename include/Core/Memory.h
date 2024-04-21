// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Type/TypeTraits.h"
#include "Core/Type/TypeMethods.h"
#include "Core/Platform/PlatformMemory.h"

void* operator new(TSize Size)
{
	return SPlatformMemory::Allocate((uint32)Size);
}

void operator delete(void* Ptr, TSize Size)
{
	return SPlatformMemory::Deallocate(Ptr, (uint32)Size);
}

typedef SPlatformMemory SMemory;