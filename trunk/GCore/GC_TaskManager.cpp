#include "GC_TaskManager.h"
#include "GC_Exception.h"

namespace gcore
{
		

	TaskManager::TaskManager()
	: m_needSort(false)
	{

	}

	TaskManager::~TaskManager()
	{
		terminateAllTasks();
	}


	unsigned long TaskManager::countPausedTasks() const
	{
		return (unsigned long)m_pausedTaskList.size();
	}

	unsigned long TaskManager::countActiveTasks() const
	{
		return (unsigned long)m_activeTaskList.size();
	}

	unsigned long TaskManager::countNamedTasks() const
	{
		return (unsigned long)m_namedTasksIndex.size();
	}

	unsigned long TaskManager::countTotalTasks() const
	{
		return (unsigned long)(countPausedTasks() + countActiveTasks());
	}
	

	void TaskManager::activateTask(Task* task)
	{
		//task not null
		GC_ASSERT( task != nullptr, "Tried to activate a null task!" );
		//task not already registered
		if( task->m_state != TS_UNACTIVE && !task->m_taskManager )
			GC_EXCEPTION(String("Tried to activate an already registered task! Task : ")+task->getName());
		
		////////////////////////////////////

		//register name if not empty
		if(task->getName() != "" )
		{
			//task name must not be already registered
			if( m_namedTasksIndex.find( task->getName() ) != m_namedTasksIndex.end() )
				GC_EXCEPTION(String("Tried to activate a task with an already registered name! Task : ")+task->getName());
		
			m_namedTasksIndex[task->getName()] = task;
		}
		
		//register in active list
			m_activeTaskList.push_back(task);
		//we will need to sort the active list before execution
		m_needSort = true;


		//change state
		task->m_state = TS_ACTIVE;
		task->m_taskManager = this;


		//user defined initialization
		task->onActivate();
		
	}

	void TaskManager::pauseTask(Task* task)
	{
		//task not null
		GC_ASSERT( task != nullptr , "Tried to pause a null task!" );
		//task registered in active list
		if( task->m_state != TS_ACTIVE )
			GC_EXCEPTION(String("Tried to pause a non active task! Task : ")+task->getName());

		
		/////////////////////////////
		//Remove from active tasks list
		m_activeTaskList.remove(task);
		//no need to sort the active task list

		//insert task in paused list
		m_pausedTaskList.push_back(task);

		//change state
		task->m_state = TS_PAUSED;

		//user defined pause
		task->onPaused();

	}

	void TaskManager::resumeTask(Task* task)
	{
		//task not null
		GC_ASSERT( task != nullptr , "Tried to resume a null task!" );
		//task registered in paused list
		if( task->m_state != TS_PAUSED )
			GC_EXCEPTION(String("Tried to resume a non paused task! Task : ")+task->getName());
		
		//////////////////////////////
		//Remove from paused tasks list
		m_pausedTaskList.remove(task);

		//insert task in active list
		m_activeTaskList.push_back(task);
		//we will need to sort the active list before execution
		m_needSort = true;

		//change state
		task->m_state = TS_ACTIVE;

		//user defined resume
		task->onResumed();

	}

	void TaskManager::terminateTask(Task* task)
	{
		//task not null
		GC_ASSERT( task != nullptr , "Tried to terminate a null task!" );
		//task registered
		if( task->m_state == TS_UNACTIVE )
			GC_EXCEPTION(String("Tried to terminate a non registered task! Task : ")+task->getName());

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
			m_activeTaskList.remove(task);
			//no need to sort the active task list
		}
		else if(task->m_state == TS_PAUSED)
		{
			//from paused tasks list
			m_pausedTaskList.remove(task);
		}
		else
		{
			//hu?!
			GC_EXCEPTION(String("Tried to terminate a non active nor paused task! Task : ")+task->getName());
		}

		//change state
		task->m_state = TS_UNACTIVE;
		task->m_taskManager = nullptr;

		//user defined termination
		task->onTerminate();

	}


	void TaskManager::changeTaskPriority(Task* task,const TaskPriority& newPriority)
	{
		//task not null
		GC_ASSERT( task != nullptr , "Tried to change the priority of a null task!" );
		//if current priority is the same, don"t change anything
		if(task->m_priority == newPriority) return;
		
		//////////////////////////
		//change priority 
		task->m_priority = newPriority;
		
		//if task is in active list
		if(task->m_state ==  TS_ACTIVE)
		{
			//we will need to sort the active list before execution
			m_needSort = true;
		}

	}


	Task* TaskManager::getRegisteredTask(const String& name) const
	{
		std::map< String , Task*>::const_iterator findIt = m_namedTasksIndex.find(name);
		if( findIt == m_namedTasksIndex.end() )
			return nullptr; //Not found

		else return (findIt->second);
	}

	/*
		Functor for Task sort by Priority:
	*/
	struct SortTaskByPriority 
	{ 
		bool operator ()(Task* a1, Task* a2) const 
		{ 
			return a1->getPriority() < a2->getPriority(); 
		} 
	};
	
	void TaskManager::sortActiveTasks()
	{
		if(m_needSort)
		{
			m_activeTaskList.sort(SortTaskByPriority());

			m_needSort = false;
		}

	}

	void TaskManager::executeTasks()
	{
		//Sort tasks if necessary.
		sortActiveTasks();

		/*
			To prevent all the issues from modifying the active tasks lists,
			we register the current active tasks list in another list to execute each
			task without taking account of the modifications on the original list
			until the next call of this method.
		*/

		//Register the tasks to execute :
		m_executionTaskList = m_activeTaskList;

		//Execute the tasks
		for(std::list<Task*>::reverse_iterator it = m_executionTaskList.rbegin(); it != m_executionTaskList.rend() ; ++it)
		{
			GC_ASSERT( (*it) != nullptr , "Found a null task in the task manager !" );

			if((*it)->getState()== TS_ACTIVE ) (*it)->execute();
		}

	}

	void TaskManager::pauseAllTasks()
	{
		//register tasks to pause without modifying current active tasks list
		std::list<Task*> tasksToPause (m_activeTaskList);
		//pause all active tasks
		for(std::list<Task*>::iterator itTask = tasksToPause.begin(); itTask != tasksToPause.end() ; ++itTask)
		{
			GC_ASSERT( (*itTask) != nullptr , "Found a null task in the task manager!" );

			//insert task in paused list
			m_pausedTaskList.push_back( (*itTask) );

			//change state
			(*itTask)->m_state = TS_PAUSED;

			//user defined pause
			(*itTask)->onPaused();
		}

		//clear the active list
		m_activeTaskList.clear();
	}

	void TaskManager::resumeAllTasks()
	{
		//resume all paused tasks
		
		//we use a seperate list to keep track of tasks if the paused list is modified
		std::list<Task*> resumingTaskList( m_pausedTaskList );

		std::list<Task*>::iterator it = resumingTaskList.begin();
		Task* lastTask =nullptr;
		for(it; it!=resumingTaskList.end(); ++it)
		{
			Task* task = (*it);

			GC_ASSERT( (task) != nullptr , "Found a null task in the task manager!" );

			//insert task in active list
			m_activeTaskList.push_back(task);
		
			//change state
			task->m_state = TS_ACTIVE;

			//user defined resume
			task->onResumed();
		}

		//will need to sort tasks before execution
		m_needSort=true;

		//clear paused tasks list
		m_pausedTaskList.clear();

	}

	void TaskManager::terminateAllTasks()
	{
		std::list<Task*> tasksToTerminate;

		///////////////////////////////////
		//terminate active tasks :
		tasksToTerminate = m_activeTaskList;

		for(std::list<Task*>::iterator itTask = tasksToTerminate.begin(); itTask != tasksToTerminate.end() ; ++itTask)
		{
			GC_ASSERT( (*itTask) != nullptr , "Found a null task in the task manager!" );

			//change state
			(*itTask)->m_state = TS_UNACTIVE;
			//user defined termination
			(*itTask)->onTerminate();
		}

		//clear active tasks list
		m_activeTaskList.clear();

		///////////////////////////////
		//terminate paused tasks :

		tasksToTerminate = m_pausedTaskList;

		for(std::list<Task*>::iterator itTask = tasksToTerminate.begin(); itTask != tasksToTerminate.end() ; ++itTask)
		{
			GC_ASSERT( (*itTask) != nullptr , "Found a null task in the task manager!" );

			//change state
			(*itTask)->m_state = TS_UNACTIVE;
			//user defined termination
			(*itTask)->onTerminate();
		}

		//clear active tasks list
		m_pausedTaskList.clear();

		///////////////////////////////
		//clear name index:
		m_namedTasksIndex.clear();

	}


	void TaskManager::terminateAllPausedTasks()
	{
		//we use a seperate list to keep track of tasks if the paused list is modified
		std::list<Task*> deadTaskList(m_pausedTaskList);
		std::list<Task*>::iterator it;

		for(it=deadTaskList.begin(); it!=deadTaskList.end(); ++it)
		{
			Task* task = (*it);
			
			GC_ASSERT( (task) != nullptr , "Found a null task in the task manager!" );

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

	void TaskManager::terminateAllActiveTasks()
	{
		//we use a seperate list to keep track of tasks if the active list is modified
		std::list<Task*> deadTaskList(m_activeTaskList);
		std::list<Task*>::iterator it;

		for(it=deadTaskList.begin(); it!=deadTaskList.end(); ++it)
		{
			Task* task = (*it);
			
			GC_ASSERT( (task) != nullptr , "Found a null task in the task manager!" );

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
		m_activeTaskList.clear();

		//deadTaskList should be automatically cleared


	}


}