// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#pragma once
#include <functional>
#include <utility>

#include <Ost3D/Core/Public/Utility/Event/EventInternal.h>

// ------------------------------------------------------------

namespace ost
{
	namespace core
	{
		struct EventListenerHandle
		{
			friend Event;

			EventListenerHandle();
			EventListenerHandle(const EventListenerHandle&) = delete;
			EventListenerHandle(EventListenerHandle&& h) noexcept;
			~EventListenerHandle();

			bool Valid() const;
			void BindCallback(std::function<void()> f);
		private:
			std::function<void()> _invoke;
			event_internal::IEventInternal* _boundInternalEvent;
			uint32 _handleUID{ event_internal::EVENT_LISTENER_UID_INVALID };
		};

		template<typename TEventData>
		struct TEventListenerHandle
		{
			friend TEvent<TEventData>;

			TEventListenerHandle()
				: _invoke{}
				, _boundInternalEvent{ nullptr }
				, _handleUID{ event_internal::EVENT_LISTENER_UID_INVALID }
			{
			}
			TEventListenerHandle(const TEventListenerHandle&) = delete;
			TEventListenerHandle(TEventListenerHandle&& h) noexcept
				: TEventListenerHandle()
			{
				std::swap(_invoke, h._invoke);
				std::swap(_boundInternalEvent, h._boundInternalEvent);
				std::swap(_handleUID, h._handleUID);

				// Since this event listener has now "moved" in memory we repoint it if there is a bound event
				if (_boundInternalEvent != nullptr)
				{
					_boundInternalEvent->RepointEventListener(*this);
				}
			}
			~TEventListenerHandle()
			{
				if (_boundInternalEvent && _handleUID != event_internal::EVENT_LISTENER_UID_INVALID)
				{
					_boundInternalEvent->NotifyListenerDestroyed(_handleUID);
				}
				_boundInternalEvent = nullptr;
				_handleUID = event_internal::EVENT_LISTENER_UID_INVALID;
			}

			bool Valid() const
			{
				return _boundInternalEvent != nullptr && _handleUID != event_internal::EVENT_LISTENER_UID_INVALID;
			}
			void BindCallback(std::function<void(const TEventData&)> f)
			{
				_invoke = f;
			}
		private:
			std::function<void(const TEventData&)> _invoke;
			event_internal::TEventInternal<TEventData>* _boundInternalEvent;
			uint32 _handleUID{ event_internal::EVENT_LISTENER_UID_INVALID };
		};
	}
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------