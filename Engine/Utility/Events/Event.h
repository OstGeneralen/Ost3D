#pragma once
#include <functional>
#include <unordered_map>
#include <Engine/Utility/Ops.h>
#include <Engine/OstEngineTypedefs.h>

namespace ost
{
	struct HEventListener
	{
	public:
		HEventListener()
			: _unregisterF{}
			, _listenerUID{0}
		{
		}
		HEventListener(uint32 id, std::function<void(HEventListener&)> unregF)
			: _unregisterF(unregF)
			, _listenerUID(id)
		{ 
		}
		HEventListener(const HEventListener&) = delete;
		HEventListener(HEventListener&& hel) noexcept
			: _unregisterF(hel._unregisterF)
			, _listenerUID(hel._listenerUID)
		{
			hel._unregisterF = {};
			hel._listenerUID = 0;
		}
		~HEventListener()
		{
			Reset();
		}

		operator bool() const
		{
			return _listenerUID != 0;
		}
		explicit operator uint32() const 
		{ 
			return _listenerUID; 
		}
		HEventListener& operator=(const HEventListener&) = delete;
		HEventListener& operator=(HEventListener&& hel) noexcept
		{
			_unregisterF = Move(hel._unregisterF);
			_listenerUID = Move(hel._listenerUID);
			hel._unregisterF = {};
			hel._listenerUID = 0;
			return *this;
		}

		void Reset()
		{
			if (_listenerUID != 0 && _unregisterF)
			{
				_unregisterF(*this);
				_listenerUID = 0;
				_unregisterF = {};
			}
		}
	private:
		std::function<void(HEventListener&)> _unregisterF;
		uint32 _listenerUID;
	};

	template<typename TEventData>
	struct TEvent
	{
		using TEventListen_f = std::function<void(const TEventData&)>;

		void Raise(const TEventData& data)
		{
			for (auto& [id,listener] : _listeners)
			{
				listener(data);
			}
		}
		
		HEventListener Listen(TEventListen_f listenF)
		{
			const uint32 assignedID = _nextEventUID;
			_nextEventUID++;
			_listeners[assignedID] = listenF;

			return HEventListener{ assignedID, [&](HEventListener& l) { StopListen(l); } };
		}

	private:
		void StopListen(HEventListener& listenerUID)
		{
			_listeners.erase(static_cast<uint32>(listenerUID));
		}

		uint32 _nextEventUID = 1;
		std::unordered_map<uint32, TEventListen_f> _listeners;
	};
}