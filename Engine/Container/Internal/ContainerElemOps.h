#pragma once
#include <Engine/Utility/Memory/MemUtils.h>

#include <new>

namespace ost
{
	namespace ContainerUtils
	{
		template<typename T>
		struct TElementInfo
		{
			static constexpr size_t ByteSize = sizeof(T);

			static constexpr bool MemCopyable = std::is_trivially_copyable<T>::value;
			static constexpr bool MoveConstruct = std::is_move_constructible<T>::value;
			static constexpr bool CopyConstruct = std::is_move_constructible<T>::value;

			static constexpr bool MoveAssign = std::is_move_constructible<T>::value;
			static constexpr bool CopyAssign = std::is_move_constructible<T>::value;

			static constexpr bool InvokeDtor = std::is_move_constructible<T>::value;
		};

		template<typename T>
		struct TElemOps
		{
		public:
			static void AddElement(T& at, const T& elem)
			{
				if (TElementInfo<T>::MemCopyable)
				{
					MemCopyT<T>(&at, &elem, 1);
				}
				else
				{
					new(&at) T(elem);
				}
			}

			static void AddElement(T& at, T& elem)
			{
				if (TElementInfo<T>::MemCopyable)
				{
					MemCopyT<T>(&at, &elem, 1);
				}
				else if (TElementInfo<T>::MoveConstruct)
				{
					new(&at) T(Forward(elem));
				}
				else
				{
					new(&at) T(Forward(elem));
				}
			}

			template<typename ... TArgs>
			static void EmplaceElement(T& at, TArgs&&... args)
			{
				new(&at) T(args...);
			}

			static T* ResizeBuffer(T* currentBuffer, size_t buffUseSize, size_t buffReserveSize, size_t newSize)
			{
				T* nBuff = AllocMemT<T>(newSize);
				if (TElementInfo<T>::MemCopyable)
				{
					MemCopyT<T>(nBuff, currentBuffer, buffUseSize);
				}
				else
				{
					for (size_t idx = 0; idx < buffUseSize; ++idx)
					{
						T& dest = nBuff[idx];

						if (TElementInfo<T>::MoveAssign)
						{
							T& source = currentBuffer[idx];
							new(&dest) T(Move(source));
						}
						else
						{
							const T& source = currentBuffer[idx];
							new(&dest) T(source);
						}
					}
				}

				ReleaseMemT<T>(currentBuffer, buffReserveSize);
				return nBuff;
			}

			static void MoveElement(T& current, T& to)
			{
				if (TElementInfo<T>::MemCopyable)
				{
					MemCopyT<T>(&to, &current, 1);
				}
				else if (TElementInfo<T>::MoveConstruct)
				{
					new(&to) T(Move(current));
				}
				else
				{
					const T& currCRef = current;
					new(&to) T(currCRef);
				}

				FillMem(&current, sizeof(T), 0);
			}

			static void CopyBuffer(const T* source, T* dest, size_t elemCount)
			{
				if (TElementInfo<T>::MemCopyable)
				{
					MemCopyT<T>(dest, source, elemCount);
				}
				else
				{
					for (size_t i = 0; i < elemCount; ++i)
					{
						T& destElem = dest[i];
						const T& sourceElem = source[i];
						new(&destElem) T(sourceElem);
					}
				}
			}
		};
	}
}