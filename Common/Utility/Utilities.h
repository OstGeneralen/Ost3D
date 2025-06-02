#pragma once
namespace Util
{
	template<typename T> struct RefRemove { using Type = T; };
	template<typename T> using RefRemovedType = RefRemove<T>::Type;

	template<typename T>
	constexpr RefRemovedType<T>&& Move(T& t) noexcept
	{
		return static_cast<RefRemovedType<T>&&>(t);
	}

	template<typename T>
	constexpr RefRemovedType<T>&& Move(T&& t) noexcept
	{
		return static_cast<RefRemovedType<T>&&>(t);
	}

	template<typename T>
	constexpr RefRemovedType<T>&& Forward(T& t)
	{
		return static_cast<RefRemovedType<T>&&>(t);
	}

	template<typename T>
	constexpr RefRemovedType<T>&& Forward(T&& t)
	{
		return static_cast<RefRemovedType<T>&&>(t);
	}

	template<typename T>
	void Swap(T& a, T& b)
	{
		T tmp = a;
		a = Move(b);
		b = Move(tmp);
	}

	extern void MemCopy(void* destination, const void* source, int size);
}