
#pragma once

#include "Utilities/Internals/EnsuresInternals.h"

#if DO_ENSURES
	
	// [No Return]
	#define ENSURE(expression) ENSURE_IMPL(expression, __FILE__, __LINE__, true)
	
	// TODO(kristian.fisera): Make better handling (for pointers etc..)
	#define ENSURE_THIS() ENSURE((this && this->IsValid()))
	#define ENSURE_VALID(type) ENSURE(type.IsValid())

	// [Return]
	#define ENSURE_RET(expression, ...) if(!ENSURE_IMPL(expression, __FILE__, __LINE__, true)) { return __VA_ARGS__; }
	
	// TODO(kristian.fisera): Make better handling (for pointers etc..)
	#define ENSURE_THIS_RET(...) ENSURE_RET((this && this->IsValid()), __VA_ARGS__)
	#define ENSURE_VALID_RET(type, ...) ENSURE_RET(type.IsValid(), __VA_ARGS__)

#else
	
	// [No Return]
	#define ENSURE(...)
	#define ENSURE_THIS()
	#define ENSURE_VALID()
	
	// [Return]
	#define ENSURE_RET(...)
	#define ENSURE_THIS_RET(...)
	#define ENSURE_VALID_RET(...)

#endif