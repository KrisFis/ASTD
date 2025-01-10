// Copyright Alternity Arts. All Rights Reserved

#pragma once

// Cpp standard check

#if __cplusplus < 202002L
	#error "C++20 standard is required to compile the code"
#endif

// Build type
// * Supported: Debug, Release
// * Example: BUILD_RELEASE
/////////////////////////////////

#ifndef BUILD_RELEASE
	#define BUILD_RELEASE 0
#endif

#ifndef BUILD_DEBUG
	#define BUILD_DEBUG 0
#endif

#if BUILD_RELEASE == BUILD_DEBUG
	#error "Please specify 'BUILD_RELEASE=1' or 'BUILD_DEBUG=1'"
#endif

// Compiler
// * Supported: MSVC, GNUC, GNUC_CLANG, GNUC_INTEL, GNUC_GCC
// * Example: COMPILER_GNUC_CLANG
/////////////////////////////////

#if defined(_MSC_VER)
	#define COMPILER_MSVC 1
#elif defined(__llvm__) && defined(__clang__)
	#define COMPILER_CLANG 1
#elif defined(__INTEL_COMPILER)
	#define COMPILER_INTEL 1
#elif defined(__GNUC__)
	#define COMPILER_GCC 1
#else
	#error "Unsupported compiler"
#endif

#ifndef COMPILER_MSVC
	#define COMPILER_MSVC 0
#endif

#ifndef COMPILER_CLANG
	#define COMPILER_CLANG 0
#endif

#ifndef COMPILER_INTEL
	#define COMPILER_INTEL 0
#endif

#ifndef COMPILER_GCC
	#define COMPILER_GCC 0
#endif

// Architecture
// * Supported: 64, 32
// * Example: ARCHITECTURE_64
/////////////////////////////////

#if defined(_WIN32) || defined(__i386__)
	#define ARCHITECTURE_32 1
#elif defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
	#define ARCHITECTURE_64 1
#else
	#error "Unsupported architecture"
#endif

#ifndef ARCHITECTURE_32
	#define ARCHITECTURE_32 0
#endif

#ifndef ARCHITECTURE_64
	#define ARCHITECTURE_64 0
#endif

// Platform
// * Supported: Windows, Linux
// * Example: PLATFORM_WINDOWS
/////////////////////////////////

#if defined(_WIN32) || defined(_WIN64)
	#define PLATFORM_WINDOWS 1
#elif defined(__linux__)
	#define PLATFORM_LINUX 1
#elif defined(__APPLE__)
	#define PLATFORM_APPLE 1
#else
	#error "Unsupported platform"
#endif

#ifndef PLATFORM_WINDOWS
	#define PLATFORM_WINDOWS 0
#endif

#ifndef PLATFORM_LINUX
	#define PLATFORM_LINUX 0
#endif

#ifndef PLATFORM_WINDOWS
	#define PLATFORM_WINDOWS 0
#endif

// Other
// * Order dependant
/////////////////////////////////

#include "ASTD/_internal/MacroInternals.h"
#include "ASTD/_internal/BuildConfiguration.h"
#include "ASTD/_internal/BuildPlatform.h"
#include "ASTD/_internal/BuildTypes.h"
