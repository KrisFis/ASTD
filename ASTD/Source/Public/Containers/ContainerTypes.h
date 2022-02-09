
#pragma once

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