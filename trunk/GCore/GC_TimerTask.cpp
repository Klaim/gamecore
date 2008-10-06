#include "GC_TimerTask.h"

namespace gcore
{
	TimerTask::TimerTask( TimerManager& timerManager,  TaskPriority priority ,const String& name )
	: Task( priority, name )
		, m_timerManager( timerManager )
	{ }

	TimerTask::~TimerTask()
	{

	}

	void TimerTask::onActivate()
	{

	}

	void TimerTask::onTerminate()
	{

	}

	void TimerTask::execute()
	{ 
		m_timerManager.updateTimers(); 
	}

}