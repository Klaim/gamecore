#include "TaskSystem.h"
#include "Exception.h"

TaskSystem::TaskSystem()
:m_activeTaskList_First(NULL),m_activeTaskList_Last(NULL)
	,m_activeTaskCount(0)
{

}

TaskSystem::~TaskSystem()
{
	terminateAllTasks();
}


unsigned long TaskSystem::countPausedTasks()
{
	return (unsigned long)m_pausedTaskList.size();
}

unsigned long TaskSystem::countActiveTasks()
{
	return m_activeTaskCount;
}

unsigned long TaskSystem::countNamedTasks()
{
	return (unsigned long)m_namedTasksIndex.size();
}

unsigned long TaskSystem::countTotalTasks()
{
	return (unsigned long)(countPausedTasks() + countActiveTasks());
}



void TaskSystem::insertActiveTask(Task* task)
{
	//task not null
	ASSERT(task);
	//task not already registered
	if( task->m_state == TS_ACTIVE )
		EXCEPTION("Tried to insert an already active task in active tasks list!");

	/////

	if(m_activeTaskList_First && m_activeTaskList_Last)
	{
		//insert taking account of priority order (the first is higher)
		bool inserted = false;
		for(Task* it=m_activeTaskList_Last; it != NULL; it = it->m_prev)
		{
			/*
				note:
				if multiple tasks have the same priority, 
				the new task is inserted after the last same
				priority task
			*/
			if(task->m_priority <= it->m_priority)
			{
				//just under the higher task priority : insert task here
				
				task->m_prev = it;

				if(it->m_next)
				{
					it->m_next->m_prev = task;
					task->m_next = it->m_next;
				}
				else
				{
					//last
					m_activeTaskList_Last = task;
					task->m_next = NULL;
				}

				it->m_next = task;
				inserted = true;
				break;
			}
		}
		if(!inserted)
		{
			//we went to the top of the list without inserting the task :
			//register as last one
			task->m_next = m_activeTaskList_First;
			task->m_prev = NULL;
			m_activeTaskList_First->m_prev = task;
			m_activeTaskList_First = task;

		}


	}
	else
	{
		//First and last
		m_activeTaskList_First = task;
		task->m_prev = NULL;
		m_activeTaskList_Last = task;
		task->m_next = NULL;
	}

}
void TaskSystem::removeActiveTask(Task* task)
{
	//task not null
	ASSERT(task);
	//task registered in active list
	if( task->m_state != TS_ACTIVE )
		EXCEPTION("Tried to remove a non active task from active tasks list!");

	//////
	
	if(task->m_prev)
	{
		task->m_prev->m_next = task->m_next;
	}
	else
	{
		//first
		m_activeTaskList_First = task->m_next;

	}

	if(task->m_next)
	{
		task->m_next->m_prev = task->m_prev;
	}
	else
	{
		//last
		m_activeTaskList_Last = task->m_prev;
	}

	task->m_next = NULL;
	task->m_prev = NULL;

}

void TaskSystem::activateTask(Task* task)
{
	//task not null
	ASSERT(task);
	//task not already registered
	if( task->m_state != TS_UNACTIVE )
		EXCEPTION("Tried to activate an already registered task!");
	
	////////////////////////////////////

	//register name if not empty
	if(task->getName() != "" )
	{
		//task name must not be already registered
		if( m_namedTasksIndex.find( task->getName() ) != m_namedTasksIndex.end() )
			EXCEPTION("Tried to activate a task with an already registered name!");
	
		m_namedTasksIndex[task->getName()];
	}
	
	//register in active list
	insertActiveTask(task);
	++m_activeTaskCount;

	//change state
	task->m_state = TS_ACTIVE;


	//user defined initialization
	task->onActivate();
	
}

void TaskSystem::pauseTask(Task* task)
{
	//task not null
	ASSERT(task);
	//task registered in active list
	if( task->m_state != TS_ACTIVE )
		EXCEPTION("Tried to pause a non active task!");

	
	/////////////////////////////
	//Remove from active tasks list
	removeActiveTask(task);
	--m_activeTaskCount;

	//insert task in paused list
	m_pausedTaskList.push_back(task);

	//change state
	task->m_state = TS_PAUSED;

	//user defined pause
	task->onPaused();

}

void TaskSystem::resumeTask(Task* task)
{
	//task not null
	ASSERT(task);
	//task registered in paused list
	if( task->m_state != TS_PAUSED )
		EXCEPTION("Tried to resume a non paused task!");
	
	//////////////////////////////
	//Remove from paused tasks list
	m_pausedTaskList.remove(task);

	//insert task in active list
	insertActiveTask(task);
	++m_activeTaskCount;

	//change state
	task->m_state = TS_ACTIVE;

	//user defined resume
	task->onResumed();

}

void TaskSystem::terminateTask(Task* task)
{
	//task not null
	ASSERT(task);
	//task registered
	if( task->m_state == TS_UNACTIVE )
		EXCEPTION("Tried to terminate a non registered task!");

	/////////////////////////////
	//remove task name from index if not empty
	if(task->getName() != "")
	{
		m_namedTasksIndex.erase(task->getName());
	}

	//remove from the current list
	if(task->m_state == TS_ACTIVE)
	{
		//from active tasks list
		removeActiveTask(task);
	}
	else if(task->m_state == TS_PAUSED)
	{
		//from paused tasks list
		m_pausedTaskList.remove(task);
	}
	else
	{
		//hu?!
		EXCEPTION("Tried to terminate a non active nor paused task!");
	}

	--m_activeTaskCount;

	//change state
	task->m_state = TS_UNACTIVE;

	//user defined termination
	task->onTerminate();

}


void TaskSystem::changePriority(Task* task,const TaskPriority& newPriority)
{
	//task not null
	ASSERT(task);
	//if current priority is the same, don"t change anything
	if(task->m_priority == newPriority) return;
	
	//////////////////////////
	//change priority 
	TaskPriority oldPriority = task->m_priority;
	task->m_priority = newPriority;
	
	//if task is in active list, move it to keep priority order
	if(task->m_state ==  TS_ACTIVE)
	{
		//K.I.S.S.  : remove and insert
		removeActiveTask(task);
		insertActiveTask(task);
	}

}


Task* TaskSystem::getRegisteredTask(const std::string& name)
{
	if( m_namedTasksIndex.find(name) != m_namedTasksIndex.end() )
		return NULL; //Not found

	else return m_namedTasksIndex[name];
}

void TaskSystem::executeTasks()
{
	for(Task* currentTask = m_activeTaskList_First; currentTask!=NULL; currentTask = currentTask->m_next)
	{
		currentTask->execute();
	}
}

void TaskSystem::pauseAllTasks()
{
	//pause all active tasks
	for(Task* itTask = m_activeTaskList_First; itTask != NULL; itTask = itTask->m_next)
	{
		//insert task in paused list
		m_pausedTaskList.push_back(itTask);

		//change state
		itTask->m_state = TS_PAUSED;

		//user defined pause
		itTask->onPaused();
	}

	//this is like clearing the active list
	m_activeTaskList_First = NULL;
	m_activeTaskList_Last = NULL;

	m_activeTaskCount = 0;
}

void TaskSystem::resumeAllTasks()
{
	//resume all paused tasks
	
	//we use a seperate list to keep track of tasks if the paused list is modified
	std::list<Task*> resumingTaskList( m_pausedTaskList );
	std::list<Task*>::iterator it = resumingTaskList.begin();
	Task* lastTask =NULL;
	for(it; it!=resumingTaskList.end(); ++it)
	{
		Task* task = (*it);
		//insert task in active list
		insertActiveTask(task);
		++m_activeTaskCount;
	
		//change state
		task->m_state = TS_ACTIVE;

		//user defined resume
		task->onResumed();
	}

	//clear paused tasks list
	m_pausedTaskList.clear();

}

void TaskSystem::terminateAllTasks()
{
	///////////////////////////////////
	//terminate active tasks :
	for(Task* itTask = m_activeTaskList_First; itTask != NULL; itTask = itTask->m_next)
	{
		//change state
		itTask->m_state = TS_UNACTIVE;
		//user defined termination
		itTask->onTerminate();
	}

	//this is like clearing the list
	m_activeTaskList_First = NULL;
	m_activeTaskList_Last = NULL;

	m_activeTaskCount = 0;

	///////////////////////////////
	//terminate paused tasks :

	//we use a seperate list to keep track of tasks if the paused list is modified
	std::list<Task*> deadTaskList(m_pausedTaskList);
	std::list<Task*>::iterator it;

	for(it=deadTaskList.begin(); it!=deadTaskList.end(); ++it)
	{
		Task* task = (*it);
		
		//change state
		task->m_state = TS_UNACTIVE;

		//user defined termination
		task->onTerminate();
	}

	//clear the list
	m_pausedTaskList.clear();
	//deadTaskList should be automatically cleared

	///////////////////////////////
	//clear name index:
	m_namedTasksIndex.clear();

}


void TaskSystem::terminateAllPausedTasks()
{
	//we use a seperate list to keep track of tasks if the paused list is modified
	std::list<Task*> deadTaskList(m_pausedTaskList);
	std::list<Task*>::iterator it;

	for(it=deadTaskList.begin(); it!=deadTaskList.end(); ++it)
	{
		Task* task = (*it);
		
		//change state
		task->m_state = TS_UNACTIVE;

		//remove task name from index if not empty
		if(task->getName() != "")
		{
			m_namedTasksIndex.erase(task->getName());
		}

		//user defined termination
		task->onTerminate();
	}

	//clear the list
	m_pausedTaskList.clear();

	//deadTaskList should be automatically cleared
}

void TaskSystem::terminateAllActiveTasks()
{
	for(Task* it = m_activeTaskList_First; it != NULL; it = it->m_next)
	{
		
		//change state
		it->m_state = TS_UNACTIVE;

		//remove task name from index if not empty
		if(it->getName() != "")
		{
			m_namedTasksIndex.erase(it->getName());
		}

		//user defined termination
		it->onTerminate();

	}

	//this is like clearing the list
	m_activeTaskList_First = NULL;
	m_activeTaskList_Last = NULL;

	m_activeTaskCount = 0;

}
