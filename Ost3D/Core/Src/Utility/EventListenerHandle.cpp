// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#include "Public/Utility/Event/EventListenerHandle.h"

// ------------------------------------------------------------

ost::core::EventListenerHandle::EventListenerHandle()
	: _invoke{}
	, _boundInternalEvent{ nullptr }
	, _handleUID{ event_internal::EVENT_LISTENER_UID_INVALID }
{
}

// ------------------------------------------------------------

ost::core::EventListenerHandle::EventListenerHandle(EventListenerHandle&& h) noexcept
	: EventListenerHandle()
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

// ------------------------------------------------------------

ost::core::EventListenerHandle::~EventListenerHandle()
{
	if (_boundInternalEvent && _handleUID != event_internal::EVENT_LISTENER_UID_INVALID)
	{
		_boundInternalEvent->NotifyListenerDestroyed(_handleUID);
	}
	_boundInternalEvent = nullptr;
	_handleUID = event_internal::EVENT_LISTENER_UID_INVALID;
}

// ------------------------------------------------------------

bool ost::core::EventListenerHandle::Valid() const
{
	return _boundInternalEvent != nullptr && _handleUID != event_internal::EVENT_LISTENER_UID_INVALID;
}

// ------------------------------------------------------------

void ost::core::EventListenerHandle::BindCallback(std::function<void()> f)
{
	_invoke = f;
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------