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
#include "ASTD/TypeMethods.h"
#include "ASTD/TypeTraits.h"
#include "ASTD/TypeTraitsMacros.h"

// UTILITIES
#include "ASTD/Math.h"
#include "ASTD/Memory.h"
#include "ASTD/Time.h"

// CONTAINERS
#include "ASTD/Array.h"
#include "ASTD/Optional.h"
#include "ASTD/Queue.h"

// ALLOCATORS
#include "ASTD/ArrayAllocator.h"
#include "ASTD/FixedArrayAllocator.h"
#include "ASTD/QueueAllocator.h"

// STRINGS
#include "ASTD/CString.h"
#include "ASTD/String.h"

// SMART POINTERS
#include "ASTD/SharedClass.h"
#include "ASTD/SharedObject.h"
#include "ASTD/SharedUtils.h"

// EXTRAS -> CHECKS
#include "ASTD/Check.h"
#include "ASTD/IsValid.h"

// EXTRAS -> ARCHIVE
#include "ASTD/Archive.h"
#include "ASTD/BufferArchive.h"
#include "ASTD/FileArchive.h"
