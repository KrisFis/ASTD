
#pragma once

#if DEBUG
	
	// Assert
	#include <cassert>
	#define ENSURE(condition, ...) if(!(condition)) { assert(false); return __VA_ARGS__; }
	#define ENSURE_NO_ENTRY(...) assert(false); return __VA_ARGS__;
	#define ENSURE_THIS(...) ENSURE(IsValid(), __VA_ARGS__)

#elif RELEASE
	
	#define ENSURE(...)
	#define ENSURE_NO_ENTRY(...)
	#define ENSURE_THIS(...)

#else
	#error "Missing build type definition"
#endif