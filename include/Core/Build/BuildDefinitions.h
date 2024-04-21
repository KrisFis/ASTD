// Copyright Alternity Arts. All Rights Reserved

#pragma once

// Build type
// * Supported: Debug, Release
// * Example: BUILD_RELEASE

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

#define COMPILER_MSVC 0
#define COMPILER_CLANG 0
#define COMPILER_INTEL 0
#define COMPILER_GCC 0

#if defined(_MSVC_VER)
	#undef COMPILER_MSVC
	#define COMPILER_MSVC 1
#elif defined(__llvm__) && defined(__clang__)
	#undef COMPILER_CLANG
	#define COMPILER_CLANG 1
#elif defined(__INTEL_COMPILER)
	#undef COMPILER_INTEL
	#define COMPILER_INTEL 1
#elif defined(__GNUC__)
	#undef COMPILER_GCC
	#define COMPILER_GCC 1
#else
	#error "Unsupported compiler"
#endif

// Architecture
// * Supported: 64, 32
// * Example: ARCHITECTURE_64

#define ARCHITECTURE_32 0
#define ARCHITECTURE_64 0

#if defined(_WIN32) || defined(__i386__)
	#undef ARCHITECTURE_32
	#define ARCHITECTURE_32 1
#elif defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
	#undef ARCHITECTURE_64
	#define ARCHITECTURE_64 1
#else
	#error "Unsupported architecture"
#endif

// OS
// * Supported: Windows, Android, Linux, APPLE
// * Example: PLATFORM_WINDOWS

#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 0
#define PLATFORM_ANDROID 0
#define PLATFORM_MAC 0

#if defined(_WIN32) || defined(_WIN64)
	#undef PLATFORM_WINDOWS
	#define PLATFORM_WINDOWS 1
#elif defined(__linux__) && defined(__ANDROID__)
	#undef PLATFORM_ANDROID
	#define PLATFORM_ANDROID 1
#elif defined(__linux__)
	#undef PLATFORM_LINUX
	#define PLATFORM_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
	#define PLATFORM_MAC 1
#else
	#error "Unsupported platform"
#endif
