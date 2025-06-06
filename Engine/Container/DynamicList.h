#pragma once
#include <Engine/OstEngineTypedefs.h>
#include <Engine/Utility/Memory/MemUtils.h>
#include <Engine/Container/Internal/ContainerElemOps.h>

#define DYNAMIC_LIST_ASSERTIONS 1

#if DYNAMIC_LIST_ASSERTIONS
#include <cassert>
#define DYNAMIC_LIST_ASSERT(cond) assert(cond)
#else
#define DYNAMIC_LIST_ASSERT(cond)
#endif // DYNAMIC_LIST_ASSERTIONS

namespace ost
{
	template<typename TElem>
	class TDynamicList
	{
	private:
		// If we cannot memcpy, move or copy construct we have no way to actually store this element type
		static_assert(
			ContainerUtils::TElementInfo<TElem>::MemCopyable ||
			ContainerUtils::TElementInfo<TElem>::MoveConstruct ||
			ContainerUtils::TElementInfo<TElem>::CopyConstruct);
	public:
		TDynamicList() : TDynamicList(10u)
		{
		}

		TDynamicList(size_t reserveSize)
			: _data{ AllocMemT<TElem>(reserveSize) }
			, _usedSize{ 0u }
			, _reservedSize{ reserveSize }
		{
		}

		TDynamicList(const TDynamicList& l)
			: TDynamicList(l._reservedSize)
		{
			ContainerUtils::TElemOps<TElem>::CopyBuffer(l._data, _data, l._usedSize);
			_usedSize = l._usedSize;
		}

		TDynamicList(TDynamicList&& l) noexcept
			: _data{ l._data }
			, _usedSize{ l._usedSize }
			, _reservedSize{ l._reservedSize }
		{
			l._data = nullptr;
			l._usedSize = 0u;
			l._reservedSize = 0u;
		}

		~TDynamicList()
		{
			ClearAndReleaseCurrent();
		}

		TElem& Add(const TElem& e)
		{
			if (_usedSize == _reservedSize)
			{
				Resize(_reservedSize * 2);
			}

			ContainerUtils::TElemOps<TElem>::AddElement(_data[_usedSize], e);
			_usedSize++;
			return Last();
		}

		TElem& Add(TElem&& e)
		{
			if (_usedSize == _reservedSize)
			{
				Resize(_reservedSize * 2);
			}

			ContainerUtils::TElemOps<TElem>::AddElement(_data[_usedSize], Forward(e));
			_usedSize++;
			return Last();
		}

		template<typename ... TArgs>
		TElem& Emplace(TArgs&&... args)
		{
			if (_usedSize == _reservedSize)
			{
				Resize(_reservedSize * 2);
			}

			ContainerUtils::TElemOps<TElem>::EmplaceElement(_data[_usedSize], args...);
			_usedSize++;
			return Last();
		}

		void Remove(size_t atIdx)
		{
			DYNAMIC_LIST_ASSERT(atIdx < _usedSize);

			if (ContainerUtils::TElementInfo<TElem>::InvokeDtor)
			{
				_data[atIdx].~TElem();
			}

			if (_usedSize == 1)
			{
				_usedSize = 0;
				return;
			}

			_usedSize--;

			// Cyclic erase (move last element to location which we removed at)
			ContainerUtils::TElemOps<TElem>::MoveElement(_data[_usedSize], _data[atIdx]);
		}

		TElem& operator[](size_t idx) { return GetElemAt(idx); }
		const TElem& operator[](size_t idx) const { return GetElemAt(idx); }

		void operator=(const TDynamicList& l)
		{
			Clear();
			Resize(l._reservedSize);
			_usedSize = l._usedSize;
			ContainerUtils::TElemOps<TElem>::CopyBuffer(l._data, _data, _usedSize);
		}

		void operator=(TDynamicList&& l) noexcept
		{
			ClearAndReleaseCurrent();
			Swap(l._data, _data);
			Swap(l._reservedSize, _reservedSize);
			Swap(l._usedSize, _usedSize);
		}

		void Resize(size_t newSize)
		{
			DYNAMIC_LIST_ASSERT(newSize > _usedSize);

			_data = ContainerUtils::TElemOps<TElem>::ResizeBuffer(_data, _usedSize, _reservedSize, newSize);
			_reservedSize = newSize;
		}

		void Clear()
		{
			if (ContainerUtils::TElementInfo<TElem>::InvokeDtor)
			{
				for (size_t i = 0; i < _usedSize; ++i)
				{
					_data[i].~TElem();
				}
			}
			_usedSize = 0;
		}

		size_t Size() const
		{
			return _usedSize;
		}
		size_t ReservedSize() const
		{
			return _reservedSize;
		}

		TElem& Last()
		{
			DYNAMIC_LIST_ASSERT(_usedSize > 0);
			return GetElemAt(_usedSize - 1);
		}
		const TElem& Last() const
		{
			DYNAMIC_LIST_ASSERT(_usedSize > 0);
			return GetElemAt(_usedSize - 1);
		}

		bool IsEmpty() const { return _usedSize == 0; }

		bool TryPop(TElem& into)
		{
			if (IsEmpty()) return false;

			TElem& lastElem = Last();

			if (ContainerUtils::TElementInfo<TElem>::MemCopyable)
			{
				MemCopyT<TElem>(&into, &lastElem, 1);
			}
			else if (ContainerUtils::TElementInfo<TElem>::MoveAssign)
			{
				into = Move(lastElem);
			}
			else
			{
				into = lastElem;
			}

			Remove(_usedSize - 1);
			return true;
		}

	private:
		TElem& GetElemAt(size_t idx)
		{
			DYNAMIC_LIST_ASSERT(idx < _usedSize);
			return _data[idx];
		}
		const TElem& GetElemAt(size_t idx) const
		{
			DYNAMIC_LIST_ASSERT(idx < _usedSize);
			return _data[idx];
		}

		void ClearAndReleaseCurrent()
		{
			Clear();
			if (_data)
			{
				ReleaseMemT<TElem>(_data, _reservedSize);
				_data = nullptr;
				_reservedSize = 0;
			}
		}

		TElem* _data;
		size_t _usedSize;
		size_t _reservedSize;
	};
}