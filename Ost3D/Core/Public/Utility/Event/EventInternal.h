// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#pragma once

#include <Ost3D/Core/Public/TypeDefs.h>

// ------------------------------------------------------------

namespace ost
{
	namespace core
	{
		// ------------------------------------------------------------
		// Forward Declares of the Event and ListenerHandle types
		// ------------------------------------------------------------
		class Event;
		struct EventListenerHandle;
		template<typename TEventData> class TEvent;
		template<typename TEventData> struct TEventListenerHandle;

		// ------------------------------------------------------------
		// Internal data and types
		// You should not have to directly interact with these
		// ------------------------------------------------------------
		namespace event_internal
		{
			constexpr static uint32 EVENT_LISTENER_UID_INVALID = 0xFFFFFFFFu;

			class IEventInternal
			{
			public:
				virtual ~IEventInternal() = default;
				virtual void RepointEventListener(EventListenerHandle& handle) = 0;
				virtual void NotifyListenerDestroyed(uint32 listenerUID) = 0;
			};

			template<typename TEventData>
			class TEventInternal
			{
			public:
				virtual ~TEventInternal() = default;
				virtual void RepointEventListener(TEventListenerHandle<TEventData>& handle) = 0;
				virtual void NotifyListenerDestroyed(uint32 listenerUID) = 0;
			};
		}
	}
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------