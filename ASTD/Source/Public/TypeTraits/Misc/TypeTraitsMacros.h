
#pragma once

#include "TypeTraitsCore.h"

#define DECLARE_HAS_METHOD_TRAIT(InMethodName, InMethodCall)																	\
	template <typename CheckType>																								\
	struct THas ## InMethodName ## Method																						\
	{																															\
	private:																													\
																																\
		typedef typename TDecay<CheckType>::Type PureType;																		\
																																\
		template<typename TestType> static auto TestHasMethod(int32)->TTrueValue<decltype(DeclVal<TestType>().InMethodCall)>;	\
																																\
		template<typename> static auto TestHasMethod(int64)->TConstBool<false>;													\
																																\
		template<class TestType> struct FGetTestValue : decltype(TestHasMethod<TestType>(0)){};									\
																																\
	public:																														\
																																\
		enum { Value = FGetTestValue<PureType>::Value };																	\
	};

#define DECLARE_HAS_FIELD_TRAIT(InFieldName)																					\
	template <typename CheckType>																								\
	struct THas ## InFieldName ## Field																							\
	{																															\
	private:																													\
																																\
		typedef typename TDecay<CheckType>::Type PureType;																		\
																																\
		template<typename TestType> static auto TestHasField(int32)->TTrueValue<decltype(&TestType::InFieldName)>;				\
																																\
		template<typename> static auto TestHasField(int64)->TConstBool<false>;													\
																																\
		template<class TestType> struct FGetTestValue : decltype(TestHasField<TestType>(0)){};									\
																																\
	public:																														\
																																\
		enum { Value = FGetTestValue<PureType>::Value };																		\
	};