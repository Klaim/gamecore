#include "GC_EventManager.h"

#include <algorithm>

namespace gcore
{

	EventManager::EventManager()
	{
		// arbitrary optimizations
		m_processListeners.reserve( 32 ); 
		
	}


	EventManager::~EventManager()
	{
		clear();
	}

	void EventManager::addListener( EventListener& eventListener, Event::TypeId typeToCatch )
	{
		// get the listeners for this event type or create it
		EventListenerList& listeners = m_listenerRegister[ typeToCatch ]; 

		// check
		GC_ASSERT( std::find( listeners.begin(), listeners.end(), &eventListener ) == listeners.end(), "Tried to add the same listener twice for event " << typeToCatch );

		// register the listener
		listeners.push_back( &eventListener );

	}

	void EventManager::removeListener( EventListener& eventListener )
	{
		if( m_listenerRegister.empty() )
		{
			GC_ASSERT( !m_listenerRegister.empty(), "Tried to remove a listener from an empty event manager!" );
			return; 
		}

		EventListenerRegister::iterator register_it = m_listenerRegister.begin();

		for( ; register_it != m_listenerRegister.end(); ++register_it )
		{
			EventListenerList& listeners = register_it->second;

			if( listeners.empty() ) continue; // be lazy;

			listeners.remove( &eventListener );
		}

	}

	void EventManager::removeListener(  Event::TypeId typeToCatch, EventListener& eventListener )
	{
		EventListenerRegister::iterator it = m_listenerRegister.find( typeToCatch );

		GC_ASSERT( it != m_listenerRegister.end(), "Tried to remove listener from unregistered event type : " << typeToCatch );

		if( it != m_listenerRegister.end() )
		{
			EventListenerList& listeners = it->second;
			listeners.remove( &eventListener );
		}

	}

	void EventManager::removeAllListeners()
	{
		m_listenerRegister.clear();
	}

	void EventManager::clear()
	{
		removeAllListeners();
	}

	void EventManager::send( const EventPtr& e , bool immediate )
	{
		GC_ASSERT_NOT_NULL( e.get() );

		if( immediate )
		{
			processEvent( e );
		}
		else
		{
			m_eventQueue.push_back( e );
		}

	}

	void EventManager::process()
	{
		if( m_eventQueue.empty() ) return; // be lazy!!!

		// first we have to swap the current event queue with the process one (empty)
		m_processEventQueue.clear();
		m_processEventQueue.swap( m_eventQueue ); // ready for the next event sent

		// now we have to process each event in the processing queue
		for( EventQueue::iterator it = m_processEventQueue.begin(); it != m_processEventQueue.end(); ++it )
		{
			const EventPtr e = *it;
			GC_ASSERT_NOT_NULL( e.get() );
			processEvent( e );
		}

	}

	void EventManager::processEvent( const EventPtr& e )
	{
		GC_ASSERT_NOT_NULL( e.get() );

		if( m_listenerRegister.empty() ) return; // no listener registered at all!

		// first dispatch this event to listeners registered to listen to all events
		EventListenerList& listenAllList = m_listenerRegister[ Event::TypeId() ];
		dispatchEvent( e, listenAllList );

		// retrieve the event listener list for this event type if it exists
		EventListenerRegister::iterator register_it = m_listenerRegister.find( e->type() );

		if( register_it == m_listenerRegister.end() ) return; // no listeners for this event!
		
		// now dispatch the event to those listeners waiting for it
		EventListenerList& listeners = register_it->second;
		dispatchEvent( e, listeners );

	}

	void EventManager::dispatchEvent( const EventPtr& e, EventListenerList& listeners )
	{
		GC_ASSERT_NOT_NULL( e.get() );

		if( listeners.empty() ) return; // be lazy!!

		// prepare this dispatch by copying the listener list in the one used to dispatch
		m_processListeners.reserve( listeners.size() );
		m_processListeners.assign( listeners.begin(), listeners.end() );

		// ok now we have to send the event to all those catchers
		for( EventListenerBuffer::iterator it = m_processListeners.begin(); it != m_processListeners.end(); ++it )
		{
			EventListener* listener = *it;
			GC_ASSERT_NOT_NULL( listener );

			listener->catchEvent( e, *this ); // catch!
		}
	}
}