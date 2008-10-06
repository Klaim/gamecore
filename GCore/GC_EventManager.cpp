#include "GC_EventManager.h"


namespace gcore
{


	/** Constructor. */
	EventManager::EventManager()
	{
		
	}

	/** Destructor. */
	EventManager::~EventManager()
	{
		
	}

	/** Process an Event object.
		This will call each EventListener::catchEvent() of EventListener registered that have the same
		EventType than the Event sent.
		@param eventToProcess Event to process.
	*/
	void EventManager::processEvent(const EventPtr& eventToProcess)
	{
		if(!eventToProcess.get()) GC_EXCEPTION("Tried to process a null Event.");

		typedef std::multimap<EventType, EventListener*  >::iterator I;
		std::pair<I,I> result=m_eventListenerPool.equal_range(eventToProcess->getType());//Get EventListeners waiting for the same type of Event
		for(I itc=result.first;itc!=result.second;++itc)
		{
			EventListener*  el=itc->second;
			if(el)el->catchEvent(eventToProcess,(*this));//Catch Event!
		}

		// now send the event to listeners registered to get an event type equal to 0
		if( eventToProcess->getType() != 0) // we already did it in the upper code?
		{
			std::pair<I,I> result=m_eventListenerPool.equal_range(0);//Get EventListeners waiting for the type 0
			for(I itc=result.first;itc!=result.second;++itc)
			{
				EventListener*  el=itc->second;
				if(el)el->catchEvent(eventToProcess,(*this));//Catch Event!
			}
		}
		

	}

	/** Register an EventListener.
		@remark	If the EventListener object is already registered in this
		EventManager, an Exception will occur.
		@param eventListener EventListener object to register.
	*/
	void EventManager::registerEventListener(EventListener* eventListener)
	{
		if(!eventListener) GC_EXCEPTION( "Tried to add a null EventListener.");

		using namespace std;

		typedef multimap< EventType, EventListener* >::iterator I;

		for(I it = m_eventListenerPool.begin() ; it != m_eventListenerPool.end() ; ++it)
		{
			EventListener*  e=it->second;
			if(e==eventListener)
			{
				//Tried to add this object twice!
				GC_EXCEPTION("Tried to add an EventListener twice!");
			}
		}

		m_eventListenerPool.insert(std::make_pair(eventListener->getEventTypeToCatch(),eventListener));

		
	}

	/** Unregister an EventListener.
		@remark	If the EventListener object is not registered in this
		EventManager, an Exception will occur.
		@param eventListener EventListener object to register.
	*/
	void EventManager::unregisterEventListener(EventListener* eventListener)
	{
		if(!eventListener) GC_EXCEPTION( "Tried to remove a null EventListener!");
		
		
		typedef std::multimap<EventType, EventListener*  >::iterator I;
		I it;
		std::pair<I,I> result;

		//find the eventListener
		result=m_eventListenerPool.equal_range(eventListener->getEventTypeToCatch());
		for(it=result.first;it!=result.second;++it)
		{
			EventListener*  e=it->second;
			if(e==eventListener)
			{
				m_eventListenerPool.erase(it);
				return;
			}
		}
		//event catcher not found!!
		GC_EXCEPTION("EventListener not found!");


	}


	/** Remove all EventListeners registered.

	*/
	void EventManager::clear()
	{
		m_eventListenerPool.clear();
	}

	/** Send an event.
		@remark The Event will be managed automatically via EventPtr smart pointer. It will
		then been destroyed once there is no more reference to it.
		@param eventToSend Event object to send.
		@param immediate If true, the Event will be processed immediately. If false, the Event is registered and will
		be processed on the next call of EventManager::processEvents().
	*/
	void EventManager::sendEvent( const EventPtr& eventToSend , bool immediate/*=false*/ )
	{
		if(!eventToSend.get()) GC_EXCEPTION( "Tried to send a null Event!");

		if(immediate)//Processing of the Event must be done NOW!
		{
			processEvent( eventToSend );
			return;
		}

		m_eventList.push_back( eventToSend );
	}

	/** Process all buffered Events.
		For each Event that have not been sent immediatly, this will call
		each EventListener::catchEvent() of EventListener registered that have the same
		EventType than the Event sent.
	*/
	void EventManager::processEvents()
	{
		if(m_eventList.empty()) return;
		//make a copy of the event list to keep it right while the execution (in cancellation cases)
		std::list<EventPtr> processEventList(m_eventList);
		//process each event
		for(std::list<EventPtr>::iterator it = processEventList.begin(); it!= processEventList.end(); ++it)
		{
			processEvent( (*it) );
		}

		//clear the event list
		m_eventList.clear();
	}

	/**	Cancel an Event sent.
		@param eventToCancel Event to Cancel.
	*/
	void EventManager::cancelEvent( const EventPtr& eventToCancel )
{
		if(!eventToCancel.get()) GC_EXCEPTION( "Tried to cancel a null Event!");

		m_eventList.remove( eventToCancel );
		
	}

	class _Predicate_MatchEventType
	{
	private:
		EventType type;
	public:

		_Predicate_MatchEventType(EventType _type):type(_type){};

		bool operator() (const EventPtr& eventPtr)
		{
			if(eventPtr->getType() == type)
				return true;

			return false;
		}

	};

	/** Cancel all Events of the precised EventType.
		@remark This will cancel only buffered Events as the others have already been processed.
		@param eventType Type of Events to cancel.
	*/
	void EventManager::cancelEvents(EventType type)
	{
		m_eventList.remove_if( _Predicate_MatchEventType(type) );
	}

	/** Delete all buffered Events not processed.
	*/
	void EventManager::cancelAllEvents()
	{
		m_eventListenerPool.clear();
	}


}