
#pragma once

#include "Platform/PlatformTypes.h"

template<typename T>
class TAllocatorBase
{

public: // Typedefs

	typedef T ElementType;

public: // Destructor

	virtual ~TAllocatorBase() {}

public: // Interface

	virtual ElementType* Allocate(uint32 Num) = 0;
	virtual void Deallocate(ElementType* Ptr, uint32 Num) = 0;

	// TODO(jan.kristian.fisera): Add support for move
	virtual void Construct(ElementType* Ptr, const ElementType& Value) = 0;

	virtual void Destruct(ElementType* Ptr) = 0;
};