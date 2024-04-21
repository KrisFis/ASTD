// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "Core/Types.h"
#include "Core/Type/TypeTraits.h"

#include "Containers/InitializerList/InitializerList.h"

template<typename InElementType, typename InAllocator>
class TSet
{
public: // Types

	typedef InElementType ElementType;
	typedef InAllocator AllocatorType;

	typedef ElementType* ArrayIteratorType;
	typedef const ElementType* ConstArrayIteratorType;

	typedef typename AllocatorType::SizeType SizeType;
	typedef TInitializerList<ElementType> ElementListType;

public: // Asserts

	static_assert(!TIsSame<ElementType, void>::Value && !TIsReference<ElementType>::Value, "Element type is not valid");
	static_assert(!TIsSame<AllocatorType, void>::Value && TIsSignedType<SizeType>::Value, "Allocator type is not valid");
};