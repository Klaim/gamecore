#include "GC_ChronicTask.h"

namespace gcore
{
	ChronicTask::ChronicTask( TaskPriority priority /*= 0 */,const String& name /*= "" */ ) 
		: Task( priority, name )
		, m_timerTriggered( false )
		, m_timer( nullptr )
	{

	}

	ChronicTask::~ChronicTask()
	{
		if( m_timer != nullptr) m_timer->unregisterListener( this );
	}

	void ChronicTask::setTimer( Timer* timer )
	{
		if( m_timer != nullptr )
		{
			// unregister from the current timer
			m_timer->unregisterListener( this );
		}

		m_timer = timer;

		if( m_timer != nullptr )
		{
			// register in the new timer
			m_timer->registerListener( this );
		}
	}

	void ChronicTask::onTimerTrigger( Timer& timer )
	{
		if( state() == TS_ACTIVE )
		{
			m_timerTriggered = true;
		}
	}

	void ChronicTask::onExecute()
	{
		// is it time to execute
		if( m_timerTriggered )
		{
			// execute one time now!
			this->execute();

			m_timerTriggered = false;
		}
	}
}