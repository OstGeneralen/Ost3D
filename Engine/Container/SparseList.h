#pragma once
#include <unordered_set>
#include <Engine/Container/Internal/ContainerElemOps.h>
#include <Engine/Utility/Memory/MemUtils.h>
#include <Engine/Utility/Ops.h>

namespace ost
{
	template<typename T>
	class TSparseList
	{
	public:
		TSparseList()
			: TSparseList(10)
		{
		}

		TSparseList(size_t reserved)
			: _elements{ AllocMemT<T>(reserved)}
			, _size{0}
			, _reservedSize{reserved}
			, _freeIndexes{}
		{
		}

		TSparseList(const TSparseList& l)
			: _elements{ AllocMemT<T>(l._reservedSize)}
			, _reservedSize{l._reservedSize}
			, _freeIndexes{l._freeIndexes}
		{
			ContainerUtils::TElemOps<T>::CopyBuffer(l._elements, _elements, l._reservedSize);
		}


		TSparseList(TSparseList&& l) noexcept
			: _elements(l._elements)
			, _reservedSize{l._reservedSize}
			, _size{l._size }
			, _freeIndexes{Move(l._freeIndexes)}
		{
			l._elements = nullptr;
			l._reservedSize = 0;
			l._size = 0;
		}

		template<typename ... TArgs>
		T& Emplace(TArgs&& ... args)
		{
			size_t insertIndex = GetInsertIndex();
			if (insertIndex == _reservedSize)
			{
				Resize(_reservedSize * 2);
			}

			ContainerUtils::TElemOps<T>::EmplaceElement(_elements[insertIndex], Forward(args...));
			if (insertIndex == _size) _size++;

			return _elements[insertIndex];
		}


		T& Add(const T& element)
		{
			size_t insertIndex = GetInsertIndex();
			if (insertIndex == _reservedSize)
			{
				Resize(_reservedSize * 2);
			}

			ContainerUtils::TElemOps<T>::AddElement(_elements[insertIndex], element);

			if (insertIndex == _size) _size++;
			return _elements[insertIndex];
		}

		void Remove(size_t at)
		{
			if (ContainerUtils::TElementInfo<T>::InvokeDtor)
			{
				_elements[at].~T();
			}
			_freeIndexes.insert(at);
		}

		T& operator[](size_t idx)
		{
			return _elements[idx];
		}

		const T& operator[](size_t idx) const
		{
			return _elements[idx];
		}

		void Resize(size_t newSize)
		{
			_elements = ContainerUtils::TElemOps<T>::ResizeBuffer(_elements, _reservedSize, _reservedSize, newSize);
			_reservedSize = newSize;
		}
	private:
		size_t GetInsertIndex()
		{
			size_t idx = _size;
			if (_freeIndexes.size() != 0)
			{
				idx = *(_freeIndexes.begin());
				_freeIndexes.erase(idx);
			}
			return idx;
		}
		
		T* _elements;
		size_t _size;
		size_t _reservedSize;
		std::unordered_set<size_t> _freeIndexes;
	};
}
