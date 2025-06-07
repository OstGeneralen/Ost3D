// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#pragma once
#include <unordered_map>
#include <utility>
#include <cassert>

#include <Ost3D/Core/Public/Utility/Event/EventInternal.h>
#include <Ost3D/Core/Public/Utility/Event/EventListenerHandle.h>

// ------------------------------------------------------------

namespace ost
{
	namespace core
	{
		class Event : private event_internal::IEventInternal
		{

		public:
			Event(const Event&) = delete;
			Event(Event&&) noexcept = delete;
		public:
			Event() = default;
			~Event();

			Event& operator+=(EventListenerHandle& eventListener);
			Event& operator-=(EventListenerHandle& eventListener);
			void operator()();
		private:
			void RepointEventListener(EventListenerHandle& handle) override;
			void NotifyListenerDestroyed(uint32 uid) override;

			std::unordered_map<uint32, EventListenerHandle*> _listeners;
			uint32 _nextListenerUID{ 0u };
		};

		// ------------------------------------------------------------

		template<typename TEventData>
		class TEvent : private event_internal::TEventInternal<TEventData>
		{
		public:
			using ListenerHandle_t = TEventListenerHandle<TEventData>;

		public:
			TEvent(const TEvent&) = delete;
			TEvent(TEvent&&) noexcept = delete;
		public:
			TEvent() = default;
			~TEvent()
			{
				for (auto [id, listenerPtr] : _listeners)
				{
					listenerPtr->_boundInternalEvent = nullptr;
					listenerPtr->_handleUID = event_internal::EVENT_LISTENER_UID_INVALID;
				}

				_listeners.clear();
			}

			TEvent& operator+= (ListenerHandle_t&eventListener)
			{
				assert(eventListener._boundInternalEvent == nullptr); // This event listener should not already be in a valid state

				const uint32 assignedID = _nextListenerUID++;
				eventListener._handleUID = assignedID;
				eventListener._boundInternalEvent = this;

				_listeners.insert({ assignedID, &eventListener });
				return *this;
			}
			TEvent& operator-=(ListenerHandle_t& eventListener)
			{
				auto found = _listeners.find(eventListener._handleUID);
				if (found != _listeners.end())
				{
					found->second->_boundInternalEvent = nullptr;
					found->second->_handleUID = event_internal::EVENT_LISTENER_UID_INVALID;
					_listeners.erase(found);
				}
				return *this;
			}
			void operator()(const TEventData& eventData)
			{
				for (auto [id, listenerPtr] : _listeners)
				{
					if (listenerPtr->_invoke)
					{
						listenerPtr->_invoke(eventData);
					}
				}
			}
		private:
			void RepointEventListener(ListenerHandle_t& handle) override
			{
				// Since EventListeners are movable, we need to handle repointing them when this happens
				auto foundListener = _listeners.find(handle._handleUID);
				if (foundListener != _listeners.end())
				{
					foundListener->second = &handle;
				}
			}
			void NotifyListenerDestroyed(uint32 uid) override
			{
				auto found = _listeners.find(uid);
				if (found != _listeners.end()) _listeners.erase(found);
			}

		private:
			std::unordered_map<uint32, ListenerHandle_t*> _listeners;
			uint32 _nextListenerUID{ 0u };
		};

		// ------------------------------------------------------------
	}
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------