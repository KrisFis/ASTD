
#pragma once

#include "Containers/Internals/ContainerInternals.h"
#include "TypeTraits/TypeTraits.h"

namespace NArrayTypeTraits
{
	template<typename AllocatorType>
	struct TAllocatorInfo
	{
		typedef typename AllocatorType::SizeType SizeType;

		enum
		{
			IsValid = !TIsSame<AllocatorType, void>::Value && TIsSignedType<SizeType>::Value
		};
	};
}