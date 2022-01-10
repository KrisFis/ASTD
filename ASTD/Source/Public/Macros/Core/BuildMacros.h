
#pragma once

// Build type
// * Supported: Debug, Release
// * Example: BUILD_RELEASE

#if defined(_DEBUG)
	#define BUILD_DEBUG 1
#elif defined(_RELEASE)
	#define BUILD_RELEASE 1
#else
	#error "Unsupported build type"
#endif

// Compiler
// * Supported: MSVC, GNUC, GNUC_CLANG, GNUC_INTEL, GNUC_GCC
// ** In case GNUC is defined, then one of GNUC type should be also
// * Example: COMPILER_GNUC && COMPILER_GNUC_CLANG

#if defined(_MSC_VER)
	#define COMPILER_MSVC 1
#elif defined(__GNUC__)
		#define COMPILER_GNUC 1
	#if defined(__clang__)
		#define COMPILAR_GNUC_CLANG 1
	#elif defined(__INTEL_COMPILER)
		#define COMPILER_GNUC_INTEL 1
	#elif !defined(__llvm__)
		// Should be GCC
		#define COMPILER_GNUC_GCC 1
	#else
		#error "Unsupported compiler of GNU C"
	#endif
#else
	#error "Unsupported compiler"
#endif

// Architecture
// * Supported: 64, 32
// * Example: ARCHITECTURE_64

#if defined(_MSC_VER)
	#if defined(_WIN64)
		#define ARCHITECTURE_64 1
	#elif defined(_WIN32)
		#define ARCHITECTURE_32 1
	#else
		#error "Unsupported architecture of MSVC"
	#endif
#elif defined(__GNUC__)
	#if defined(__i386__)
		#define ARCHITECTURE_32 1
	#elif defined(__x86_64__) || defined(__aarch64__)
		#define ARCHITECTURE_64 1
	#else
		#error "Unsupported architecture of GNU C" 
	#endif
#else
	#error "Unsupported architecture"
#endif

// OS
// * Supported: Windows, Android, Linux, APPLE
// * Example: PLATFORM_WINDOWS

#if defined(_WIN32)
	#define PLATFORM_WINDOWS 1
#elif defined(__linux__)
	#if defined(__ANDROID__)
		#define PLATFORM_ANDROID 1
	#else
		#define PLATFORM_LINUX 1
	#endif
#elif defined(__APPLE__) && defined(__MACH__)
	// "TargetConditionals.h" contains Targets
	#define PLATFORM_APPLE 1
#else
	#error "Unsupported platform"
#endif