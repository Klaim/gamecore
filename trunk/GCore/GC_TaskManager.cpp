#include <algorithm>
#include "GC_Exception.h"
#include "GC_Task.h"
#include "GC_TaskManager.h"


namespace gcore
{
	/*
	Functor for Task sort by Priority:
	*/
	bool TaskManager::TaskCompare_AscendingPriority::operator ()(Task* a1, Task* a2) const 
	{ 
		return a1->priority() < a2->priority(); 
	} 


	TaskManager::TaskManager()
		: m_activeListChanged( false )
	{
		
	}

	TaskManager::~TaskManager()
	{
		unregisterAllTasks();
	}

	void TaskManager::registerTask( Task* task )
	{
		//task not null
		GC_ASSERT( task != nullptr, "Tried to register a null task!" );
		//task not already registered
		if( task->m_state != TS_UNREGISTERED || task->m_taskManager != nullptr )
		{
			GC_EXCEPTION << "Tried to register an already registered task! Task : " << task->name();
		}

		GC_ASSERT( std::find( m_registeredTasksList.begin(), m_registeredTasksList.end(), task) == m_registeredTasksList.end() , "Tried to activate task already registered in this TaskManager! Task : " << task->name() )

		//////////////////////////////////////////////////////////////////////////

		//register the task name if not empty
		if( task->name() != "" )
		{
			//task name must not be already registered
			if( m_namedTasksIndex.find( task->name() ) != m_namedTasksIndex.end() )
			{
				GC_EXCEPTION << "Tried to register a task with an already registered name! Task : " << task->name();
			}

			m_namedTasksIndex[ task->name() ] = task;
		}

		// register the task
		m_registeredTasksList.push_back( task );

		// change the task state
		task->m_state = TS_REGISTERED;
		task->m_taskManager = this;

	}

	void TaskManager::unregisterTask( Task* task )
	{
		// task not null
		GC_ASSERT( task != nullptr, "Tried to register a null task!" );
		// task must be registered here
		if( task->m_state == TS_UNREGISTERED || task->m_taskManager != this )
		{
			GC_EXCEPTION << "Tried to unregister an non registered task! Task : " << task->name() ;
		}

		//////////////////////////////////////////////////////////////////////////

		// do proper deactivation if necessary
		if( task->m_state == TS_ACTIVE || task->m_state == TS_PAUSED )
		{
			// the task is currently active or paused : terminate it first
			terminateTask( task );
		}

		//remove task name from index if not empty
		if( task->name() != "" )
		{
			m_namedTasksIndex.erase( task->name() );
		}

		// now we can unregister the task
		m_registeredTasksList.remove( task );

		// change the task state
		task->m_state = TS_UNREGISTERED;
		task->m_taskManager = nullptr;

	}

	void TaskManager::activateTask( Task* task )
	{
		//task not null
		GC_ASSERT( task != nullptr , "Tried to activate a null task!" );

		// check : task registered here!
		if( task->m_taskManager != this )
		{
			GC_EXCEPTION << "Tried to activate task not registered in this TaskManager! Task : " << task->name() ;
		}
		
		// task must be registered but not active nor paused
		if( task->m_state != TS_REGISTERED )
		{
			GC_EXCEPTION << "Tried to activate task already registered or paused! Task : " << task->name() ;
		}

		GC_ASSERT( std::find( m_registeredTasksList.begin(), m_registeredTasksList.end(), task) != m_registeredTasksList.end() , "Tried to activate task not registered in this TaskManager! Task : " << task->name() )

		////////////////////////////////////

		// add in active list
		m_activeTaskList.push_back( task );
		m_activeListChanged = true; // we'll need to update the execution list before the next tasks execution

		// change task state
		task->m_state = TS_ACTIVE;

		// user defined activation
		task->onActivate();
		
	}


	void TaskManager::deactivateTask( Task* task )
	{
		// task not null
		GC_ASSERT( task != nullptr , "Tried to deactivate a null task!" );
		GC_ASSERT( task->m_state == TS_ACTIVE , "Tried to deactivate an unactive task!" );
		GC_ASSERT( task->m_taskManager == this , "Tried to deactivate a task not registered in this task manager!" );

		// remove from active tasks list - this one can be expensive? have to check
		m_activeTaskList.remove( task );

		// remove from the current execution list 
		// note : to do this we only set the task to null in the execution list
		// to manage the case where this method is called in the task execution loop
		std::replace( m_executionTaskList.begin(), m_executionTaskList.end(), task, static_cast< Task* >( nullptr ) );
		
		// we'll need to update the execution list before the next tasks execution
		m_activeListChanged = true;

	}

	void TaskManager::terminateTask( Task* task )
	{
		// task not null
		GC_ASSERT( task != nullptr , "Tried to terminate a null task!" );
		
		// task must be registered
		if( task->m_state == TS_UNREGISTERED )
		{
			GC_EXCEPTION << "Tried to terminate a not registered task! Task : "  << task->name() ;
		}

		// task registered here
		if( task->m_taskManager != this )
		{
			GC_EXCEPTION << "Tried to terminate a task not registered in this TaskManager! Task : " << task->name() ;
		}
		
		GC_ASSERT( std::find( m_registeredTasksList.begin(), m_registeredTasksList.end(), task) != m_registeredTasksList.end() , "Tried to activate task not registered in this TaskManager! Task : " << task->name() )
		/////////////////////////////
		

		// remove from the current list
		switch( task->m_state )
		{
		case( TS_ACTIVE ):
			{
				// deactivate the task
				deactivateTask( task );
				break;
			}
		case( TS_PAUSED ):
			{
				// remove from paused tasks list
				m_pausedTasksList.remove( task );
				break;
			}
		default:
			{
				GC_EXCEPTION << "Tried to terminate a nor active nor paused task! Task : " << task->name();
			}
		};

		// change state
		task->m_state = TS_REGISTERED;
		
		// user defined termination
		task->onTerminate();
		
	}

	void TaskManager::pauseTask( Task* task )
	{
		// task not null
		GC_ASSERT( task != nullptr , "Tried to pause a null task!" );

		// task registered here
		if( task->m_taskManager != this )
		{
			GC_EXCEPTION << "Tried to terminate a task not registered in this TaskManager! Task : " << task->name() ;
		}

		// task registered must be active
		if( task->m_state != TS_ACTIVE )
		{
			GC_EXCEPTION << "Tried to pause a non active task! Task : " << task->name() ;
		}
		
		GC_ASSERT( std::find( m_registeredTasksList.begin(), m_registeredTasksList.end(), task) != m_registeredTasksList.end() , "Tried to activate task not registered in this TaskManager! Task : " << task->name() )
		/////////////////////////////
		// deactivate the task
		deactivateTask( task );

		// add task in paused list
		m_pausedTasksList.push_back( task );

		// change state
		task->m_state = TS_PAUSED;

		// user defined pause
		task->onPaused();

	}

	void TaskManager::resumeTask(Task* task)
	{
		// task not null
		GC_ASSERT( task != nullptr , "Tried to resume a null task!" );

		// task registered here
		if( task->m_taskManager != this )
		{
			GC_EXCEPTION << "Tried to terminate a task not registered in this TaskManager! Task : " << task->name();
		}

		// task registered in paused list
		if( task->m_state != TS_PAUSED )
		{
			GC_EXCEPTION << "Tried to resume a non paused task! Task : " << task->name() ;
		}

		GC_ASSERT( std::find( m_registeredTasksList.begin(), m_registeredTasksList.end(), task) != m_registeredTasksList.end() , "Tried to activate task not registered in this TaskManager! Task : " << task->name() );
		//////////////////////////////
		// remove from paused tasks list
		m_pausedTasksList.remove( task );

		// insert task in active list
		m_activeTaskList.push_back(task);
		m_activeListChanged = true;	// we'll need to update the execution list before the next tasks execution
		
		//change state
		task->m_state = TS_ACTIVE;

		//user defined resume
		task->onResumed();

	}

	
	void TaskManager::changeTaskPriority( Task* task, const TaskPriority& newPriority)
	{
		//task not null
		GC_ASSERT( task != nullptr , "Tried to change the priority of a null task!" );
		GC_ASSERT( task->m_taskManager == this , "Tried to change the priority of a task not registered in this task manager!" );
		GC_ASSERT( std::find( m_registeredTasksList.begin(), m_registeredTasksList.end(), task) != m_registeredTasksList.end() , "Tried to activate task not registered in this TaskManager! Task : " << task->name() );
		// if current priority is the same, don"t change anything
		if( task->m_priority == newPriority ) return;
		
		//////////////////////////
		//change priority 
		task->m_priority = newPriority;
		
		//if task is in active list
		if( task->m_state ==  TS_ACTIVE)
		{
			// we'll need to update the execution list before the next tasks execution
			m_activeListChanged = true;
		}

	}


	Task* TaskManager::getRegisteredTask(const String& name) const
	{
		TaskIndex::const_iterator findIt = m_namedTasksIndex.find(name);

		if( findIt == m_namedTasksIndex.end() ) return nullptr; //Not found
		else return (findIt->second);	// found!
	}

	void TaskManager::executeTasks()
	{
		if( m_activeTaskList.empty() ) return ; // be lazy!
		
		/*
			To prevent all the issues from modifying the active tasks lists,
			we register the current active tasks list in another list to execute each
			task without taking account of the modifications on the original list
			until the next call of this method.

			If the execution list is modified during the execution loop, like if
			a task is paused, we only set the task to null in the execution list
			and it will be removed from that list on the next call to this method
			on sort.
		*/

		// Update execution list if active list have been modified since last update.
		if( m_activeListChanged )
		{
			// as active task has changed, re register the execution task list as necessary :
			m_activeTaskList.sort( TaskCompare_AscendingPriority() ); // sort first
			
			// update the execution list
			m_executionTaskList = std::vector< Task* >( m_activeTaskList.begin(), m_activeTaskList.end() );
			GC_ASSERT( !m_executionTaskList.empty(), "Invalid state : execution task list is empty while active task list is not!?" );

			m_activeListChanged = false;
		}

		// Execute the tasks
		for( std::vector< Task* >::iterator taskCursor = m_executionTaskList.begin(); taskCursor != m_executionTaskList.end() ; ++taskCursor )
		{
			Task* task = (*taskCursor);

			if( task != nullptr ) // ignore removed tasks
			{
				GC_ASSERT( task->m_state == TS_ACTIVE, String( "Found an non active task in execution list! Task :" ) + task->name() );
				GC_ASSERT( task->m_taskManager == this, "Found an active task in execution list that is not managed here! Task :" << task->name() );
				task->onExecute();
			}

		}

	}

	void TaskManager::terminateAllTasks()
	{
		// terminate active & paused tasks :
		// gather the tasks
		std::list<Task*> tasksToTerminate( m_activeTaskList.begin(), m_activeTaskList.end() );
		tasksToTerminate.insert( tasksToTerminate.end(), m_pausedTasksList.begin(), m_pausedTasksList.end() );

		// clear active tasks list
		m_activeTaskList.clear();
		m_pausedTasksList.clear();

		// terminate tasks 
		for(std::list<Task*>::iterator itTask = tasksToTerminate.begin(); itTask != tasksToTerminate.end() ; ++itTask)
		{
			Task* task = (*itTask);

			GC_ASSERT( task != nullptr , "Found a null task in the task manager!" );
			GC_ASSERT( std::find( m_registeredTasksList.begin(), m_registeredTasksList.end(), task) != m_registeredTasksList.end() , "Tried to activate task not registered in this TaskManager! Task : " << task->name() )
			//change state
			task->m_state = TS_REGISTERED;
			//user defined termination
			task->onTerminate();
		}

		// clear name index:
		m_namedTasksIndex.clear();

		// clear execution list
		m_executionTaskList.clear();
		m_activeListChanged = false;

	}


	void TaskManager::unregisterAllTasks()
	{
		// first, clear all lists if necessary
		terminateAllTasks();

		// now unregister properly each registered task
		for ( TaskList::iterator it =  m_registeredTasksList.begin(); it != m_registeredTasksList.end(); ++it )
		{
			Task* task = (*it);

			GC_ASSERT( task != nullptr , "Found a null task in the task manager!" );
			GC_ASSERT( std::find( m_registeredTasksList.begin(), m_registeredTasksList.end(), task) != m_registeredTasksList.end() , "Tried to activate task not registered in this TaskManager! Task : " << task->name() )
			task->m_state = TS_UNREGISTERED;
			task->m_taskManager = nullptr;
		}

		// then unregister them all from here
		m_registeredTasksList.clear();
	}

	
}