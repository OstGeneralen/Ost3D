// Ost3D Common - EnumFlag.h

#pragma once
#include <concepts>
#include <type_traits>

// ------------------------------------------------------------

namespace ost
{
	template<typename T> concept EnumType = std::is_enum<T>::value;

	// Default to false for enums, enforce IS_FLAGS(EnumType) to use as flags
	template<EnumType> struct IsFlagEnum : std::false_type {};

	template<typename T> concept FlagEnumType = IsFlagEnum<T>::value && EnumType<T>;

	// All FlagEnumType Enums get a | operator!
	template<FlagEnumType T>
	T operator|(T l, T r)
	{
		return static_cast<T>(static_cast<unsigned>(l) | static_cast<unsigned>(r));
	}

	template<FlagEnumType T>
	T operator&(T l, T r)
	{
		return static_cast<T>(static_cast<unsigned>(l) & static_cast<unsigned>(r));
	}

	template<FlagEnumType T>
	T operator~(T f)
	{
		return static_cast<T>(~static_cast<unsigned>(f));
	}

	// Flags utility type to help with checking for flags
	template<FlagEnumType T>
	struct Flags
	{
		Flags() = default;
		Flags(const Flags&) = default;
		Flags(T e) : _e(e) {}

		Flags& operator=(T e) { _e = e; return *this; }
		bool Has(T flag) const { return (static_cast<unsigned>(_e) & static_cast<unsigned>(flag)) != 0; }
	private:
		T _e;
	};

	// Conversion method to make TEnum into Flags<TEnum> type
	// This one may be redundant as the compiler seems able to handle this automatically
	template<FlagEnumType T> static Flags<T> FlagsConvert(T e) { return Flags<T>(e); }

	// Macro to make an enum type a flag type
	// Use: enum class YourEnum { ... }; IS_FLAGS(YourEnum);
}

#define IS_FLAGS( enumType ) template<> struct ost::IsFlagEnum< enumType > : std::true_type {}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------