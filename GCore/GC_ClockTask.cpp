#include "GC_ClockTask.h"

namespace GCore
{
	ClockTask::ClockTask( ClockManager& eventManager, TaskPriority priority /*= 0 */,const String& name /*= "" */ ) 
		:Task( priority , name ),
		m_clockManager(eventManager)
	{

	}

	ClockTask::~ClockTask()
	{

	}


	void ClockTask::execute()
	{
		m_clockManager.updateClocks();
	}

}