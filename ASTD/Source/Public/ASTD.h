
#pragma once

// Main PCH
// * Should include all features and functionality of ASTD
/////////////////////////////////////////

// Build
#include "Build/BuildDefinitions.h"
#include "Build/PreprocessorHelpers.h"

// Platform
#include "Platform/PlatformDefinitions.h"
#include "Platform/PlatformTypes.h"
#include "Platform/PlatformMath.h"
#include "Platform/PlatformMemory.h"
#include "Platform/PlatformTime.h"

// TypeTraits
#include "TypeTraits/TypeMethods.h"
#include "TypeTraits/TypeTraits.h"
#include "TypeTraits/TypeTraitsMacros.h"

// Shared
#include "Shared/SharedObject.h"
#include "Shared/SharedClass.h"
#include "Shared/SharedUtils.h"

// Math
#include "Math/MathUtilities.h"

// Memory
#include "Memory/MemoryUtilities.h"

// Containers
#include "Containers/Array/Array.h"
#include "Containers/Array/Misc/ArrayPrivateUtils.h"
#include "Containers/Array/Misc/ArrayPrivateTypeTraits.h"
#include "Containers/Array/Allocator/ArrayAllocator.h"
#include "Containers/Array/Allocator/FixedArrayAllocator.h"
#include "Containers/ContainerTypes.h"

// Utilities
#include "Utilities/Ensures.h"