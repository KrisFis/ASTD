// Copyright Alternity Arts. All Rights Reserved

#pragma once

// Main PCH
// * Should include all features and functionality of ASTD
/////////////////////////////////////////

// Look at "Configurations.h" to see defines to that affects features provided

// Core | Build
#include "Core/Build/BuildDefinitions.h"
#include "Core/Build/PreprocessorHelpers.h"
#include "Core/Build/Configurations.h"

// Core | Platform
#include "Core/Platform/PlatformDefinitions.h"
#include "Core/Platform/PlatformTypes.h"
#include "Core/Platform/PlatformMemory.h"
#include "Core/Platform/PlatformTime.h"
#include "Core/Platform/PlatformCString.h"

// Core | Types
#include "Core/Type/TypeTraits.h"
#include "Core/Type/TypeMethods.h"
#include "Core/Type/TypeTraitsMacros.h"

// Core
#include "Core/Types.h"
#include "Core/Math.h"
#include "Core/Memory.h"
#include "Core/Time.h"
#include "Core/CString.h"

// Containers
#include "Containers/InitializerList/InitializerList.h"

#include "Containers/Array/Array.h"
#include "Containers/Array/Allocator/ArrayAllocator.h"
#include "Containers/Array/Allocator/FixedArrayAllocator.h"

#include "Containers/Set/Set.h"
#include "Containers/Set/Allocator/SetAllocator.h"

#include "Containers/Queue/Queue.h"
#include "Containers/Queue/Allocator/QueueAllocator.h"

#include "Containers/Optional/Optional.h"

#include "Containers/String/String.h"

#include "Containers/STDTypes.h"

// Shared
#include "Shared/SharedObject.h"
#include "Shared/SharedClass.h"
#include "Shared/SharedUtils.h"

// Utils
#include "Core/Memory.h"

// Validation
#include "Validation/Check.h"
#include "Validation/IsValid.h"
