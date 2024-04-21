// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include <initializer_list>

// NOTE(jan.kristian.fisera): Hope this defines all compilers
// * Not able to replicate this class
template<typename T>
using TInitializerList = std::initializer_list<T>;
