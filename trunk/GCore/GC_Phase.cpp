#include <algorithm>
#include "GC_String.h"
#include "GC_Phase.h"


namespace gcore
{
	Phase::Phase( const String& name ) 
		: m_name( name )
		, m_state( Phase::UNLOADED )
		, m_listenerListChanged( false )
	{
		GC_ASSERT( name != "" , "Tried to create a Phase with an empty name!" );
	}
	
	void Phase::requestLoad()
	{
		GC_ASSERT( m_state == Phase::UNLOADED , String( "Load requested to phase " ) + m_name + " but it is not unloaded!" );

		changeState( Phase::LOADING );
		if( this->onLoadRequested() )
		{
			changeState( Phase::LOADED );
		}
	}

	void Phase::requestActivate()
	{
		GC_ASSERT( m_state == Phase::LOADED , String( "Activate requested to phase " ) + m_name + " but it is not loaded!" );

		changeState( Phase::ACTIVATING );
		if( this->onActivateRequested() )
		{
			changeState( Phase::ACTIVE );
		}
	}

	void Phase::requestTerminate()
	{
		GC_ASSERT( m_state == Phase::ACTIVE , String( "Terminate requested to phase " ) + m_name + " but it is not active!" );

		changeState( Phase::TERMINATING );
		if( this->onTerminateRequested() )
		{
			changeState( Phase::LOADED );
		}
	}

	void Phase::requestUnload()
	{
		GC_ASSERT( m_state == Phase::LOADED , String( "Unload requested to phase " ) + m_name + " but it is not loaded!" );

		changeState( Phase::UNLOADING );
		if( this->onUnloadRequested() )
		{
			changeState( Phase::UNLOADED );
		}
	}

	void Phase::registerListener( Phase::Listener* phaseListener )
	{
		// checks!

		GC_ASSERT( phaseListener != nullptr, String( "Tried to register a null listener in the phase ") + m_name  );
		GC_ASSERT( std::find( m_listenerList.begin(), m_listenerList.end(), phaseListener ) == m_listenerList.end(), String( "Tried to regiser an already registered listener in the phase " ) + m_name );
		///////

		m_listenerList.push_back( phaseListener );
		m_listenerListChanged = true;
	}

	void Phase::unregisterListener( Phase::Listener* phaseListener )
	{
		// checks!

		GC_ASSERT( phaseListener != nullptr, String( "Tried to unregister a null listener in the phase ") + m_name  );
		GC_ASSERT( std::find( m_listenerList.begin(), m_listenerList.end(), phaseListener ) != m_listenerList.end(), String( "Tried to unregiser a non registered listener in the phase " ) + m_name );
		///////

		m_listenerList.remove( phaseListener );
		m_listenerListChanged = true;
	}

	void Phase::unregisterAllListeners()
	{
		if( !m_listenerList.empty() ) 
		{
			m_listenerList.clear();
			m_listenerListChanged = true;
		}

	}

	void Phase::changeState( Phase::State newState )
	{
		const Phase::State previous_state = m_state;
		m_state = newState;

		// notify listeners about the change
		/*	Note:
			to be sure we don't modify the listener list while the notification
			we use another list that is not modified while we notify.
		*/
		if( m_listenerListChanged )
		{
			// we need to update the notification list first
			m_notificationList = std::vector< Phase::Listener* >( m_listenerList.begin(), m_listenerList.end() );

			m_listenerListChanged = false;
		}

		const std::size_t listenerCount = m_notificationList.size();
		for( std::size_t i = 0; i < listenerCount; ++i )
		{
			Phase::Listener* phaseListener = m_notificationList[ i ];
			GC_ASSERT( phaseListener != nullptr, String( "Found a null listener in phase " ) + m_name );

			phaseListener->onPhaseStateChanged( *this, previous_state );
		}

	}

	
}