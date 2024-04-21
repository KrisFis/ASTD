// Copyright Alternity Arts. All Rights Reserved

#pragma once

// * TODO(kristian.fisera): Implement custom version of containers
// * Remains: TFastMap, TMap, TFastSet, TSet
////////////////////////////////////////////////

#include <map>
#include <unordered_map>
#include <unordered_set>

template<typename T, typename R>
using TMap = std::map<T, R>;

template<typename T, typename R>
using TFastMap = std::unordered_map<T, R>;

template<typename T>
using TFastSet = std::unordered_set<T>;