// Copyright Alternity Arts. All Rights Reserved

#pragma once

// SIMPLE TYPES
/////////////////////////////////

struct SArchive;
struct SString;

// TEMPLATED TYPES
/////////////////////////////////
template<typename ElementT, typename AllocatorT = void>
class TArray;

template<typename ElementT>
class TOptional;

template<typename ElementT, typename AllocatorT = void>
class TQueue;

template<typename T>
class TSharedClass;

template<typename T>
class TSharedPtr;

template<typename T>
class TWeakPtr;