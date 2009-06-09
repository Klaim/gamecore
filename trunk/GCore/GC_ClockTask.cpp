#include "GC_Task_ClockUpdate.h"

namespace gcore
{
	Task_ClockUpdate::Task_ClockUpdate( ClockManager& eventManager, TaskPriority priority /*= 0 */,const String& name /*= "" */ ) 
		:Task( priority , name ),
		m_clockManager(eventManager)
	{

	}

	Task_ClockUpdate::~Task_ClockUpdate()
	{

	}


	void Task_ClockUpdate::execute()
	{
		m_clockManager.updateClocks();
	}

}