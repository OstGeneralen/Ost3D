// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#include "Public/Utility/Event/Event.h"

// ------------------------------------------------------------

ost::core::Event::~Event()
{
	for (auto [id, listenerPtr] : _listeners)
	{
		listenerPtr->_boundInternalEvent = nullptr;
		listenerPtr->_handleUID = event_internal::EVENT_LISTENER_UID_INVALID;
	}

	_listeners.clear();
}

// ------------------------------------------------------------

ost::core::Event& ost::core::Event::operator+=(EventListenerHandle& eventListener)
{
	assert(eventListener._boundInternalEvent == nullptr); // This event listener should not already be in a valid state

	const uint32 assignedID = _nextListenerUID++;
	eventListener._handleUID = assignedID;
	eventListener._boundInternalEvent = this;

	_listeners.insert({ assignedID, &eventListener });
	return *this;
}

// ------------------------------------------------------------

ost::core::Event& ost::core::Event::operator-=(EventListenerHandle& eventListener)
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

// ------------------------------------------------------------

void ost::core::Event::operator()()
{
	for (auto [id, listenerPtr] : _listeners)
	{
		if (listenerPtr->_invoke)
		{
			listenerPtr->_invoke();
		}
	}
}

// ------------------------------------------------------------

void ost::core::Event::RepointEventListener(EventListenerHandle& handle)
{
	// Since EventListeners are movable, we need to handle repointing them when this happens
	auto foundListener = _listeners.find(handle._handleUID);
	if (foundListener != _listeners.end())
	{
		foundListener->second = &handle;
	}
}

// ------------------------------------------------------------

void ost::core::Event::NotifyListenerDestroyed(uint32 uid)
{
	auto found = _listeners.find(uid);
	if (found != _listeners.end()) _listeners.erase(found);
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------