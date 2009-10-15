#include "GC_TimedTask.h"


namespace gcore
{
	TimedTask::TimedTask( bool terminateOnceDone /*= false */, TaskPriority priority /*= 0 */,const String& name /*= "" */ ) 
		: Task( priority, name )
		, m_timerTriggered( false )
		, m_timer( nullptr )
		, m_terminateOnceDone( terminateOnceDone )
	{

	}

	TimedTask::~TimedTask()
	{
		if( m_timer != nullptr) m_timer->unregisterListener( this );
	}

	void TimedTask::setTimer( Timer* timer )
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

	void TimedTask::onTimerTrigger( Timer& timer )
	{
		if( state() == TS_ACTIVE )
		{
			m_timerTriggered = true;
		}
	}

	void TimedTask::onExecute()
	{
		// did we reach the end of the execution?
		if( !m_timerTriggered )
		{
			// we have some time left,
			// execute the task as usual
			this->execute();
		}
		else
		{
			// triggered!
			// no time left, we have to stop this task
			m_timerTriggered = false;

			// now we can pause or terminate this task
			if( m_terminateOnceDone ) this->terminate();
			else this->pause();
		}
	}
}