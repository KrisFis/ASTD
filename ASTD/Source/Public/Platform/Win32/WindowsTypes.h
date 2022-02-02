
#pragma once

#include <stdint.h>

// Integers
////////////////////////////////////////////////

typedef __int8 int8;
typedef unsigned __int8 uint8;

typedef __int16 int16;
typedef unsigned __int16 uint16;

typedef __int32 int32;
typedef unsigned __int32 uint32;

typedef __int64 int64;
typedef unsigned __int64 uint64;


// Characters
////////////////////////////////////////////////

typedef __wchar_t wchar;
typedef wchar pchar; // platform char

// Containers
// * TODO(kristian.fisera): Implement custom version of containers
// * Remains: TFastMap, TMap, TFastSet, TSet, TArray, FString
////////////////////////////////////////////////

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <vector>
#include <string>

using FString = std::string;

template<typename T, typename R>
using TFastMap = std::unordered_map<T, R>;

template<typename T, typename R>
using TMap = std::map<T, R>;

template<typename T>
using TArray = std::vector<T>;

template<typename T>
using TFastSet = std::unordered_set<T>;

template<typename T>
using TSet = std::set<T>;