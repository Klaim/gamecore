#include "GC_Task.h"
#include "GC_TaskManager.h"

namespace GCore
{
		
	void Task::activate(TaskManager* taskManager)
	{
		GC_ASSERT( taskManager != nullptr, "Tried to activate a task with a null task manager!" );
		/////////////////////////////
		if(!m_taskManager)
		{
			taskManager->activateTask(this);
		}
		else
		{
			GC_EXCEPTION(String("Tried to pause Task ")+m_name+" but it's not registered in a TaskManager!"); 
		}
	}

	void Task::pause()
	{
		if(m_taskManager)
		{
			m_taskManager->pauseTask(this);
		}
		else
		{
			GC_EXCEPTION(String("Tried to pause Task ")+m_name+" but it's not registered in a TaskManager!"); 
		}
	}

	void Task::resume()
	{
		if(m_taskManager)
		{
			m_taskManager->resumeTask(this);
		}
		else
		{
			GC_EXCEPTION(String("Tried to resume Task ")+m_name+" but it's not registered in a TaskManager!"); 
		}
	}

	void Task::terminate()
	{
		if(m_taskManager)
		{
			m_taskManager->terminateTask(this);
		}
		else
		{
			GC_EXCEPTION(String("Tried to terminate Task ")+m_name+" but it's not registered in a TaskManager!"); 
		}
	}

	void Task::setPriority(const TaskPriority& priority)
	{
		if(m_taskManager)
		{
			m_taskManager->changeTaskPriority(this, priority);
		}
		else
		{
			m_priority = priority;
		}
	}

}