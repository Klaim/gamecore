#ifndef GCORE_EVENTLISTENER_H
#define GCORE_EVENTLISTENER_H
#pragma once

#include <functional>

#include "GC_Common.h"
#include "GC_Event.h"

namespace gcore
{
	class EventManager;

	/** Receive and manage specific Events.
		
		An EventListener object must be registered in an EventManager to receive
		the Events that the EventManager will receive.
		On registering, an EventType must be specified. The EventListener will
		then receive only Events that are of this EventType.
		It the EventType is equal to 0, the listener will receive all types of event.
		
		@remark
		This is a virtual base class : the heritor class must define 
		the management of the Event received.
		@remark
		An EventListener can be registered by more than one EventManager.

		@see EventManager, Event

	*/
	class GCORE_API EventListener
	{
	public:

		/** Constructor.
			@param eventType Event type to catch once registered in an EventManager. If equal to 0, this EventListener will receive all events from the EventManager.
		*/
		EventListener(EventType eventType):eventTypeToCatch(eventType)
		{}

		/** User defined Event reception.
			This method will be called when an Event of the same type is
			processed by the EventManager.
			@param eventToProcess Smart pointer to the Event object.
			@param eventManager EventManager object that processed the Event.
		*/
		virtual void catchEvent(const EventPtr& eventToProcess, EventManager& eventManager) = 0;

		///Event type to catch once registered in an EventManager.
		const EventType& getEventTypeToCatch() const {return eventTypeToCatch;}

	protected:

	private:

		///Event type to catch once registered in an EventManager.
		const EventType eventTypeToCatch;
	};

	/// Function-like object that can catch events.
	typedef std::tr1::function< void (const EventPtr& , EventManager& ) > EventListenerFunction;

	/** Proxy event listener that only redirect event catches to a provided function-like object.
		note : seems obsolete ... should be replaced by boost::signal
	*/
	class ProxyEventListener : EventListener
	{
	public:
		ProxyEventListener( const EventListenerFunction& catherFunction, EventType eventType )
			: EventListener( eventType )
			, m_catcherFunction( catherFunction )
		{}

		inline void catchEvent(const EventPtr& eventToProcess, EventManager& eventManager)
		{
			m_catcherFunction( eventToProcess, eventManager );
		}

	private:
		
		EventListenerFunction m_catcherFunction;
	};

}

#endif