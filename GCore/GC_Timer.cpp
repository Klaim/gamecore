#include "GC_Timer.h"
#include "GC_Clock.h"

namespace gcore
{

	Timer::Timer( const String& name, const Clock& clock ) : m_name( name )
		, m_clock( clock )
		, m_triggerCount( 0 )
		, m_waitTime( 0 )
		, m_timeSinceLastTrigger ( 0 )
		, m_timerListenerListChanged( false )
		, m_triggerOnce( false )
	{

	}

	Timer::~Timer()
	{

	}

	void Timer::registerListener( TimerListener* timerListener )
	{
		GC_ASSERT( timerListener != nullptr, "Tried to register a null listener in Timer!" );
		GC_ASSERT( std::find( m_timerListenerList.begin(), m_timerListenerList.end(), timerListener) == m_timerListenerList.end() , "Tried to register a listener in a Timer twice!" );

		m_timerListenerList.push_back(timerListener);
		m_timerListenerListChanged = true;
	}

	void Timer::unregisterListener( TimerListener* timerListener )
	{
		GC_ASSERT( timerListener != nullptr, "Tried to unregister a null listener in Timer!" );

		m_timerListenerList.erase( std::remove( m_timerListenerList.begin(), m_timerListenerList.end(), timerListener ) , m_timerListenerList.end());
		m_timerListenerListChanged = true;
	}

	void Timer::unregisterAllListeners()
	{
		m_timerListenerList.clear();
	}

	void Timer::update()
	{
		if( m_waitTime == 0 ) return;	// wait time 0 is equal to no wait time set
		if( m_triggerOnce && m_triggerCount > 0 ) return; // already triggered one time

		// update time passed
		m_timeSinceLastTrigger += m_clock.getDeltaTime();

		while( m_timeSinceLastTrigger >= m_waitTime ) // make sure we do more than one trigger if necessary
		{
			// we have to trigger
			++m_triggerCount;

			// trigger all listeners
			if( m_timerListenerListChanged )
			{
				m_triggerList = m_timerListenerList;
				m_timerListenerListChanged = false;
			}

			const std::size_t listenerCount = m_triggerList.size();
			for( unsigned int i = 0; i < listenerCount; ++i )
			{
				TimerListener* timerListener = m_triggerList[i];
				timerListener->onTimerTrigger( *this );
			}

			// time passed
			m_timeSinceLastTrigger -= m_waitTime;

			if( m_triggerOnce ) break; // trigger only one time!
		}
	}

	void Timer::reset()
	{
		m_triggerCount = 0;
		m_timeSinceLastTrigger = 0;
	}
}