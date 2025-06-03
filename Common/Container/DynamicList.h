#pragma once
#include <Common/Utility/Utilities.h>
#include <Common/DataDefinitions.h>
#include <Common/Utility/MemAlloc.h>
#include <new>
#include <Common/Container/ContainerConfigs.h>

#if DYNAMIC_LIST_ASSERTIONS
#include <cassert>
#define DYNAMIC_LIST_ASSERT(cond) assert(cond)
#else
#define DYNAMIC_LIST_ASSERT(cond)
#endif // DYNAMIC_LIST_ASSERTIONS



template<typename TElem>
class TDynamicList
{
private:
	constexpr static size_t c_elemSize = sizeof(TElem);
	constexpr static bool c_invokeDtor = std::is_destructible<TElem>::value;

	constexpr static bool c_canMemCopy = std::is_trivially_copyable<TElem>::value;

	constexpr static bool c_ctorMove = std::is_move_constructible<TElem>::value;
	constexpr static bool c_ctorCpy = std::is_copy_constructible<TElem>::value;

	constexpr static bool c_assignMove = std::is_move_assignable<TElem>::value;
	constexpr static bool c_assignCopy = std::is_copy_assignable<TElem>::value;

	// If we cannot memcpy, move or copy construct we have no way to actually store this element type
	static_assert(c_canMemCopy || c_ctorMove || c_ctorCpy);
public:
	TDynamicList() : TDynamicList(10u)
	{
	}

	TDynamicList(size_t reserveSize)
		: _data{ MemAlloc::AllocTMemory<TElem>(reserveSize) }
		, _usedSize{ 0u }
		, _reservedSize{ reserveSize }
	{
	}

	TDynamicList(const TDynamicList& l)
		: TDynamicList(l._reservedSize)
	{
		if (c_canMemCopy)
		{
			Util::MemCopy(_data, l._data, l._usedSize * sizeof(TElem));
		}
		else
		{
			// Copy
		}
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
		Clear();
		if (_data)
		{
			delete[] _data;
			_data = nullptr;
		}
	}

	TElem& Add(const TElem& e)
	{
		if (_usedSize == _reservedSize)
		{
			Resize(_reservedSize * 2);
		}

		TElem* atPtr = &(_data[_usedSize]);
		if (c_canMemCopy)
		{
			Util::MemCopy(atPtr, &e, sizeof(TElem));
		}
		else
		{
			new(atPtr) TElem(e);
		}
		_usedSize++;
		return *atPtr;
	}

	TElem& Add(TElem&& e)
	{
		if (_usedSize == _reservedSize)
		{
			Resize(_reservedSize * 2);
		}

		TElem* atPtr = &(_data[_usedSize]);
		new(atPtr) TElem(Util::Forward(e));
		_usedSize++;
		return *atPtr;
	}

	template<typename ... TArgs>
	TElem& Emplace(TArgs&&... args)
	{
		if (_usedSize == _reservedSize)
		{
			Resize(_reservedSize * 2);
		}

		TElem* atPtr = &(_data[_usedSize]);
		new(atPtr) TElem(args...);
		_usedSize++;
		return *atPtr;
	}

	void Remove(size_t atIdx)
	{
		DYNAMIC_LIST_ASSERT(atIdx < _usedSize);

		if (c_invokeDtor)
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
		TElem* cycleElemPtr = &(_data[_usedSize]);
		TElem* newAddrPtr = &(_data[atIdx]);

		if (c_canMemCopy)
		{
			Util::MemCopy(newAddrPtr, cycleElemPtr, sizeof(TElem));
		}
		else if (c_ctorMove)
		{
			TElem& cycleRef = *cycleElemPtr;
			new(newAddrPtr) TElem(Util::Move(cycleRef));
		}
		else if (c_ctorCpy)
		{
			const TElem& cycleRef = *cycleElemPtr;
			new(newAddrPtr) TElem(cycleRef);
		}

		MemAlloc::FillMemory(cycleElemPtr, sizeof(TElem), 0);
	}

	TElem& operator[](size_t idx)  {  return GetElemAt(idx);  }
	const TElem& operator[](size_t idx) const { return GetElemAt(idx); }

	void Resize(size_t newSize)
	{
		DYNAMIC_LIST_ASSERT(newSize > _usedSize);

		TElem* newBuff = MemAlloc::AllocTMemory<TElem>(newSize);
		if (c_canMemCopy)
		{
			Util::MemCopyT<TElem>(newBuff, _data, _usedSize);
		}
		else if (c_ctorMove)
		{
			for (size_t moveElemIdx = 0; moveElemIdx < _usedSize; ++moveElemIdx)
			{
				TElem& sourceElem = _data[moveElemIdx];
				TElem* destinationAddr = &(newBuff[moveElemIdx]);
				new(destinationAddr) TElem(Util::Move(sourceElem));
			}
		}
		else
		{
			for (size_t moveElemIdx = 0; moveElemIdx < _usedSize; ++moveElemIdx)
			{
				const TElem& sourceElem = _data[moveElemIdx];
				TElem* destinationAddr = &(newBuff[moveElemIdx]);
				new(destinationAddr) TElem(sourceElem);
			}
		}

		_reservedSize = newSize;
		delete[] _data;
		_data = newBuff;
	}

	void Clear()
	{
		if (c_invokeDtor)
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

		if (c_canMemCopy)
		{
			Util::MemCopyT<TElem>(&into, &lastElem, 1);
		}
		else if (c_assignMove)
		{
			into = Util::Move(lastElem);
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

	TElem* _data;
	size_t _usedSize;
	size_t _reservedSize;
};
