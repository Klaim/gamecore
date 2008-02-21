#include "GC_EventTask.h"

namespace gcore
{

	EventTask::EventTask( EventManager& eventManager, TaskPriority priority /*= 0 */,const String& name /*= "" */ ) 
		:Task( priority , name ),
		m_eventManager(eventManager)
	{

	}

	EventTask::~EventTask()
	{

	}


	void EventTask::execute()
	{
		m_eventManager.processEvents();
	}

}

