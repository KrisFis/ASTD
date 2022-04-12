
#pragma once

#include <iostream>

#include "Core/Types.h"
#include "Core/Type/TypeTraits.h"

#include "Log/Internals/LoggerTypeTraits.h"

// Very primitive logger
struct SLogger
{
	FORCEINLINE static SLogger Begin() { return SLogger(); }
	FORCEINLINE static const char* End() { return "\n"; }

	FORCEINLINE static void EmptyLine() { std::cout << std::endl; }

	template<typename T, typename TEnableIf<NLogInternals::TGetSupportedType<T>::Int>::Type* = nullptr>
	FORCEINLINE_DEBUGGABLE const SLogger& operator<<(T Value) const
	{
		std::cout << (int64)Value;
		return *this;
	}

	template<typename T, typename TEnableIf<NLogInternals::TGetSupportedType<T>::String>::Type* = nullptr>
	FORCEINLINE_DEBUGGABLE const SLogger& operator<<(T Value) const
	{
		return operator<<(Value.GetChars());
	}

	template<typename T, typename TEnableIf<NLogInternals::TGetSupportedType<T>::StringW>::Type* = nullptr>
	FORCEINLINE_DEBUGGABLE const SLogger& operator<<(T Value) const
	{
		std::wcout << Value;
		return *this;
	}

	template<typename T, typename TEnableIf<NLogInternals::TGetSupportedType<T>::Other>::Type* = nullptr>
	FORCEINLINE_DEBUGGABLE const SLogger& operator<<(T Value) const
	{
		std::cout << Value;
		return *this;
	}

private: // Constructor

	SLogger() = default;
};