#include "GC_Task_EventProcess.h"

#include "GC_EventManager.h"

namespace gcore
{

	Task_EventProcess::Task_EventProcess( EventManager& eventManager, TaskPriority priority /*= 0 */,const String& name /*= "" */ ) 
		:Task( priority , name ),
		m_eventManager(eventManager)
	{

	}

	Task_EventProcess::~Task_EventProcess()
	{

	}


	void Task_EventProcess::execute()
	{
		m_eventManager.process();
	}

}

