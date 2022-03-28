
#pragma once

#include "Type/TypeUtilities.h"
#include "TypeTraits/TypeTraits.h"
#include "TypeTraits/TypeTraitsMacros.h"

namespace NChecksPrivateTraits
{
	template<typename TestType, typename Enable = void>
	struct FGenericIsValidCheck;

	DECLARE_HAS_METHOD_TRAIT(THasIsValidMethod, IsValid())

	template<typename TestType>
	struct FGenericIsValidCheck<TestType, typename TEnableIf<!TIsPointer<TestType>::Value && THasIsValidMethod<TestType>::Value>::Type>
	{
		FORCEINLINE static constexpr bool IsValid(const TestType& Test)
		{
			return Test.IsValid();
		}
	};

	template<typename TestType>
	struct FGenericIsValidCheck<TestType, typename TEnableIf<TIsPointer<TestType>::Value && THasIsValidMethod<typename TRemovePointer<TestType>::Type>::Value>::Type>
	{
		FORCEINLINE static constexpr bool IsValid(const TestType* Test)
		{
			return Test && Test->IsValid();
		}
	};

	template<typename TestType>
	struct FGenericIsValidCheck<TestType, typename TEnableIf<TIsPointer<TestType>::Value && !THasIsValidMethod<typename TRemovePointer<TestType>::Type>::Value>::Type>
	{
		FORCEINLINE static constexpr bool IsValid(const TestType* Test)
		{
			return Test != nullptr;
		}
	};

	DECLARE_HAS_FIELD_TRAIT(THasIsValidField, IsValid)

	template<typename TestType>
	struct FGenericIsValidCheckTraits
	{
		typedef typename TChoose<TIsPointer<TestType>::Value, typename TGetType<TestType>::ConstPointer, typename TGetType<TestType>::ConstReference>::Type DesiredType;

		enum { HasGenericIsValidFunc = THasIsValidField<FGenericIsValidCheck<TestType>>::Value };
	};

	DECLARE_HAS_GLOBAL_METHOD_TRAIT(THasGlobalIsValid, IsValid(DeclVal<TestType>()))
}

template<typename TestType,
		 typename = typename TEnableIf<!NChecksPrivateTraits::THasGlobalIsValid<TestType>::Value && NChecksPrivateTraits::FGenericIsValidCheckTraits<TestType>::HasGenericIsValidFunc>::Type>
FORCEINLINE constexpr bool IsValid(const TestType& Test)
{
	typedef typename NChecksPrivateTraits::FGenericIsValidCheckTraits<TestType>::DesiredType DesiredType;

	return NChecksPrivateTraits::FGenericIsValidCheck<DesiredType>::IsValid(Test);
}

template<typename TestType,
		typename = typename TEnableIf<!NChecksPrivateTraits::THasGlobalIsValid<TestType>::Value && !NChecksPrivateTraits::FGenericIsValidCheckTraits<TestType>::HasGenericIsValidFunc>::Type>
FORCEINLINE constexpr bool IsValid(TestType Test)
{
	static_assert(sizeof(TestType) < 0, "IsValid() function overload for type is not implemented");

	return false;
}