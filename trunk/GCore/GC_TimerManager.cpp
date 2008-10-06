
#include <boost/pool/object_pool.hpp>

#include "GC_TimerManager.h"
#include "GC_Timer.h"

namespace gcore
{


	TimerManager::TimerManager()
		: m_timerPool( new boost::object_pool< Timer >() )
	{
		
	}

	TimerManager::~TimerManager()
	{
		delete m_timerPool;
	}

	Timer* TimerManager::createTimer( const Clock& clock, const String& name /*= "" */ )
	{
		GC_ASSERT( m_namedTimersIndex.find( name ) == m_namedTimersIndex.end() , String("Tried to create a timer with a name already used! Name : ") + name );

		// create the timer
		Timer* timer = m_timerPool->construct( name, clock );

		// register the timer 
		m_timerList.push_back( timer );

		if( timer->getName() != "" )
		{
			// register in the index
			m_namedTimersIndex[ timer->getName() ] = timer;
		}

		// ready to be used!
		return timer;
	}

	void TimerManager::destroyTimer( Timer* timer )
	{
		GC_ASSERT( timer != nullptr, "Tried to destroy a null timer!" );
		GC_ASSERT( m_timerPool->is_from( timer ) , String( "Tried to destroy a timer that was not created by this manager! Timer name : ") + timer->getName() );

		// unregister timer
		if( timer->getName() != "")
		{
			m_namedTimersIndex.erase( m_namedTimersIndex.find( timer->getName() ) );
		}

		m_timerList.erase( std::remove( m_timerList.begin(), m_timerList.end(), timer ), m_timerList.end() );

		// then destroy timer
		m_timerPool->destroy( timer );

	}

	void TimerManager::destroyAllTimers()
	{
		m_namedTimersIndex.clear();
		
		const std::size_t timerCount = m_timerList.size();
		for( std::size_t i= 0; i < timerCount; ++i )
		{
			m_timerPool->destroy( m_timerList[i] );
		}

		m_timerList.clear();


	}

	Timer* TimerManager::getTimer( const String& name )
	{
		std::map< const String, Timer* >::iterator it = m_namedTimersIndex.find(name);

		if( it != m_namedTimersIndex.end() ) return it->second;
		else return nullptr;
	}

	void TimerManager::updateTimers()
	{
		const std::size_t timerCount = m_timerList.size();
		for( std::size_t i= 0; i < timerCount; ++i )
		{
			GC_ASSERT( m_timerList[i] != nullptr, "Found a null Timer in TimerManager!" );
			m_timerList[i]->update();
		}



	}
}