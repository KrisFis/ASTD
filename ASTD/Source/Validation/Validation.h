
#pragma once

#include "Validation/Internals/ValidationInternals.h"

// TODO(kristian.fisera):
// * Remove VALID and THIS
// * Implement better FATAL check

#if DO_VALIDATION
	
	#define CHECK(expression) CHECK_IMPL(expression)
	#define CHECKF(expression) CHECKF_IMPL(expression)

#else
	
	#define CHECK(expression) (!!(expression))
	#define CHECKF(expression) (!!(expression))

#endif

// Object validation

#define CHECK_VALID(object) CHECK(object.IsValid())
#define CHECKF_VALID(object) CHECKF(object.IsValid())

#define CHECK_RET(expression, ...) if(!CHECK(expression)) { return __VA_ARGS__; }
#define CHECK_VALID_RET(object, ...) CHECK_RET(object.IsValid(), __VA_ARGS__)