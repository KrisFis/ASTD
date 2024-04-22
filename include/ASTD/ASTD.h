// Copyright Alternity Arts. All Rights Reserved

#pragma once

// Main PCH
// * Should include all features and functionality of ASTD
/////////////////////////////////////////

// Look at "Configurations.h" to see defines to that affects features provided

// IMPORTANT CORE
#include "ASTD/Core/Build.h"
#include "ASTD/Core/Configuration.h"

// TYPE MANIPULATION/GENERATION/SFINAE
#include "ASTD/TypeTraits.h"
#include "ASTD/TypeMethods.h"
#include "ASTD/TypeTraitsMacros.h"

// UTILITIES
#include "ASTD/Math.h"
#include "ASTD/Memory.h"
#include "ASTD/Time.h"

// CONTAINERS
#include "ASTD/Array.h"
#include "ASTD/Queue.h"
#include "ASTD/Optional.h"

// ALLOCATORS
#include "ASTD/QueueAllocator.h"
#include "ASTD/ArrayAllocator.h"
#include "ASTD/FixedArrayAllocator.h"

// STRINGS
#include "ASTD/String.h"
#include "ASTD/CString.h"

// SMART POINTERS
#include "ASTD/SharedObject.h"
#include "ASTD/SharedClass.h"
#include "ASTD/SharedUtils.h"

// VALIDATIONS
#include "ASTD/Check.h"
#include "ASTD/IsValid.h"