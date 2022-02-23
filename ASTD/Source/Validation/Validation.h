
#pragma once

#include "Validation/Internals/ValidationInternals.h"

#if DO_VALIDATION
	
	// [No Return]
	#define CHECK(expression) CHECK_IMPL(expression, __FILE__, __LINE__, true)
	
	// TODO(kristian.fisera): Make better handling (for pointers etc..)
	#define CHECK_THIS() CHECK((this && this->IsValid()))
	#define CHECK_VALID(type) CHECK(type.IsValid())

	// [Return]
	#define CHECK_RET(expression, ...) if(!CHECK_IMPL(expression, __FILE__, __LINE__, true)) { return __VA_ARGS__; }
	
	// TODO(kristian.fisera): Make better handling (for pointers etc..)
	#define CHECK_THIS_RET(...) CHECK_RET((this && this->IsValid()), __VA_ARGS__)
	#define CHECK_VALID_RET(type, ...) CHECK_RET(type.IsValid(), __VA_ARGS__)

#else
	
	// [No Return]
	#define CHECK(...)
	#define CHECK_THIS()
	#define CHECK_VALID()
	
	// [Return]
	#define CHECK_RET(...)
	#define CHECK_THIS_RET(...)
	#define CHECK_VALID_RET(...)

#endif