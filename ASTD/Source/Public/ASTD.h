
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
#include "Memory/Allocator/InlineAllocator.h"
#include "Memory/Allocator/ListAllocator.h"
#include "Memory/Allocator/TwoWayListAllocator.h"

// Containers
#include "Containers/ContainerTypes.h"

// Utilities
#include "Utilities/Ensures.h"