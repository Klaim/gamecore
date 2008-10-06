#include <algorithm>
#include "GC_String.h"
#include "GC_Phase.h"


namespace gcore
{
	Phase::Phase( const String& name ) 
		: m_name( name )
		, m_state( PS_UNLOADED )
		, m_listenerListChanged( false )
	{
		GC_ASSERT( name != "" , "Tried to create a Phase with an empty name!" );
	}

	Phase::~Phase()
	{

	}
	
	void Phase::requestLoad()
	{
		GC_ASSERT( m_state == PS_UNLOADED , String( "Load requested to phase " ) + m_name + " but it is not unloaded!" );

		changeState( PS_LOADING );
		if( this->onLoadRequested() )
		{
			changeState( PS_LOADED );
		}
	}

	void Phase::requestActivate()
	{
		GC_ASSERT( m_state == PS_LOADED , String( "Activate requested to phase " ) + m_name + " but it is not loaded!" );

		changeState( PS_ACTIVATING );
		if( this->onActivateRequested() )
		{
			changeState( PS_ACTIVE );
		}
	}

	void Phase::requestTerminate()
	{
		GC_ASSERT( m_state == PS_ACTIVE , String( "Terminate requested to phase " ) + m_name + " but it is not active!" );

		changeState( PS_TERMINATING );
		if( this->onTerminateRequested() )
		{
			changeState( PS_LOADED );
		}
	}

	void Phase::requestUnload()
	{
		GC_ASSERT( m_state == PS_LOADED , String( "Unload requested to phase " ) + m_name + " but it is not loaded!" );

		changeState( PS_UNLOADING );
		if( this->onUnloadRequested() )
		{
			changeState( PS_UNLOADED );
		}
	}

	void Phase::registerListener( PhaseListener* phaseListener )
	{
		// checks!

		GC_ASSERT( phaseListener != nullptr, String( "Tried to register a null listener in the phase ") + m_name  );
		GC_ASSERT( std::find( m_listenerList.begin(), m_listenerList.end(), phaseListener ) == m_listenerList.end(), String( "Tried to regiser an already registered listener in the phase " ) + m_name );
		///////

		m_listenerList.push_back( phaseListener );
		m_listenerListChanged = true;
	}

	void Phase::unregisterListener( PhaseListener* phaseListener )
	{
		// checks!

		GC_ASSERT( phaseListener != nullptr, String( "Tried to unregister a null listener in the phase ") + m_name  );
		GC_ASSERT( std::find( m_listenerList.begin(), m_listenerList.end(), phaseListener ) != m_listenerList.end(), String( "Tried to regiser a non registered listener in the phase " ) + m_name );
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

	void Phase::changeState( PhaseState newState )
	{
		m_state = newState;

		// notify listeners about the change
		/*	Note:
			to be sure we don't modify the listener list while the notification
			we use another list that is not modified while we notify.
		*/
		if( m_listenerListChanged )
		{
			// we need to update the notification list first
			m_notificationList = std::vector< PhaseListener* >( m_listenerList.begin(), m_listenerList.end() );

			m_listenerListChanged = false;
		}

		const std::size_t listenerCount = m_notificationList.size();
		for( std::size_t i = 0; i < listenerCount; ++i )
		{
			PhaseListener* phaseListener = m_notificationList[ i ];
			GC_ASSERT( phaseListener != nullptr, String( "Found a null listener in phase " ) + m_name );

			phaseListener->onPhaseStateChanged( *this );
		}

	}

	
}