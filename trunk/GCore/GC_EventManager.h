#ifndef GCORE_EVENTMANAGER_H
#define GCORE_EVENTMANAGER_H
#pragma once


#include <unordered_map>
#include <list>
#include <vector>

#include "GC_Common.h"
#include "GC_Event.h"
#include "GC_EventListener.h"


namespace gcore	
{

	/** Manage Events and EventListeners.

		An EventManager will receive all Events that are sent to him
		and reroute them to the appropriate EventListeners that have been registered
		to listen to the event type.
		@par
		The Event objects can be sent in two manners :
		- immediate send : the Event will be sent to the EventListeners immediately;
		- buffered send : the Event is registered and processed later, when EventManager::process() is called.

		@remark
		EventListener objects registered in an EventManager will 
		have been registered to listen to the event type.
		@remark
		An EventListener registered to listen to a null event type will catch all events sent.

	*/
	class GCORE_API EventManager 
	{
	public:

		EventManager();

		/** Clear on destruction. */
		~EventManager();	


		/** Register an EventListener to catch events of a specific type.
			@param eventListener EventListener object to register.
			@param typeToCatch Type of the events to catch or a null value to listen to all the events.
		*/
		void addListener( EventListener& eventListener, Event::TypeId typeToCatch = Event::TypeId() );

		/** Unregister an EventListener from catching any event.
			@param eventListener EventListener object to register.
		*/
		void removeListener(EventListener& eventListener);


		/** Unregister an EventListener from catching a specific type of event.
			@param eventListener EventListener object to register.
		*/
		void removeListener( Event::TypeId typeToCatch, EventListener& eventListener);

		/** Remove all EventListeners registered.
		*/
		void removeAllListeners();

		/** Remove all listeners and cancel all buffered events. */
		void clear();

		/** Send an event.
			@remark The Event will be managed automatically via EventPtr smart pointer. It will
			then been destroyed once there is no more reference to it.
			@param eventToSend Event object to send.
			@param immediate If true, the Event will be processed immediately. If false, the Event is registered and will
			be processed on the next call of EventManager::process().
		 */
		void send(const EventPtr& eventToSend , bool immediate=false);

		/** Process all buffered Events.
			For each Event that have not been sent immediately, this will call
			each EventListener::catchEvent() of EventListener registered that have the same
			Event::TypeId than the Event sent.
			@remark EventListeners waiting for an Event::TypeId of value 0 will receive events of all types.
		*/
		void process();


	private:

		typedef std::list< EventListener* > EventListenerList;
		typedef std::vector< EventListener* > EventListenerBuffer;
		typedef std::tr1::unordered_map< Event::TypeId, EventListenerList > EventListenerRegister;
		typedef std::vector< const EventPtr > EventQueue;

		/// Listeners registered for each event type.
		EventListenerRegister m_listenerRegister;

		/// Buffered Events
		EventQueue m_eventQueue;

		/// Event queue used while processing events.
		EventQueue m_processEventQueue;

		/// List of listeners used while processing an event.
		EventListenerBuffer m_processListeners;

		void processEvent( const EventPtr& e );
		void dispatchEvent( const EventPtr& e, EventListenerList& listeners );

	};

	
} // gcore


#endif