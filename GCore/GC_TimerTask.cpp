#include "GC_Task_TimerUpdate.h"

namespace gcore
{
	Task_TimerUpdate::Task_TimerUpdate( TimerManager& timerManager,  TaskPriority priority ,const String& name )
	: Task( priority, name )
		, m_timerManager( timerManager )
	{ }

	Task_TimerUpdate::~Task_TimerUpdate()
	{

	}

	void Task_TimerUpdate::onActivate()
	{

	}

	void Task_TimerUpdate::onTerminate()
	{

	}

	void Task_TimerUpdate::execute()
	{ 
		m_timerManager.updateTimers(); 
	}

}