
#pragma once

#include <stdint.h>

// Integers
////////////////////////////////////////////////

typedef int8_t int8;
typedef uint8_t uint8;

typedef int16_t int16;
typedef uint16_t uint16;

typedef int32_t int32;
typedef uint32_t uint32;

typedef int64_t int64;
typedef uint64_t uint64;

// Characters
////////////////////////////////////////////////

typedef char pchar; // platform char

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