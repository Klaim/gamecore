#ifndef GCORE_EVENTMANAGER_H
#define GCORE_EVENTMANAGER_H
#pragma once


#include "GC_String.h"
#include <map>
#include <list>

#include "GC_Common.h"
#include "GC_Event.h"
#include "GC_EventListener.h"


namespace GCore	
{

	/** Manage Events and EventListeners.

		An EventManager will receive all Events that are sent to him
		and reroute them to the appropriate EventListeners.
		@par
		The Event objects can be sent in two manners :
		- immediate sent : the Event will be sent to the EventListeners immediately;
		- buffered sent : the Event is registered and processed later, when EventManager::processEvents() is called.

		@remark
		EventListener objects registered in an EventManager will 
		only receive the Event objects that have the same EventType.
		But if the Event type waited by the listener is equal to 0, the listener will receive all the events sent.

	*/
	class GCORE_API EventManager 
	{
	public:


		/** Register an EventListener.
			@remark	If the EventListener object is already registered in this
			EventManager, an Exception will occur.
			@param eventListener EventListener object to register.
		*/
		void registerEventListener(EventListener* eventListener);

		/** Unregister an EventListener.
			@remark	If the EventListener object is not registered in this
			EventManager, an Exception will occur.
			@param eventListener EventListener object to register.
		*/
		void unregisterEventListener(EventListener* eventListener);


		/** Remove all EventListeners registered.
		*/
		void clear();

		/** Send an event.
			@remark The Event will be managed automatically via EventPtr smart pointer. It will
			then been destroyed once there is no more reference to it.
			@param eventToSend Event object to send.
			@param immediate If true, the Event will be processed immediately. If false, the Event is registered and will
			be processed on the next call of EventManager::processEvents().
		 */
		void sendEvent(EventPtr eventToSend , bool immediate=false);

		/** Process all buffered Events.
			For each Event that have not been sent immediately, this will call
			each EventListener::catchEvent() of EventListener registered that have the same
			EventType than the Event sent.
			@remark EventListeners waiting for an EventType of value 0 will receive events of all types.
		*/
		void processEvents();

		/**	Cancel an Event sent.
			@param eventToCancel Event to Cancel.
		*/
		void cancelEvent(EventPtr eventToCancel);

		/** Cancel all Events of the provided EventType.
			@remark This will cancel only buffered Events as the others have already been processed.
			@param eventType Type of Events to cancel.
			
		*/
		void cancelEvents(EventType type);

		/** Delete all buffered Events not processed.
		*/
		void cancelAllEvents();

		//////////////////////////////////////////////////////////////////////////

	
		/** Constructor. */
		EventManager();	

		/** Destructor. */
		~EventManager();	

	private:

		
		/** EventListeners sorted by EventType.*/
		std::multimap<EventType, EventListener*  >	m_eventListenerPool;

		/** Buffered Events*/
		std::list< EventPtr >			m_eventList;			 

		/** Process an Event object.
			This will call each EventListener::catchEvent() of EventListener registered that have the same
			EventType than the Event sent.
			@param	eventToProcess Event to process.
		*/
		void processEvent(const EventPtr& eventToProcess);
	
	protected:
	
	
	
		
	
	};

	

	

} // GCore


#endif