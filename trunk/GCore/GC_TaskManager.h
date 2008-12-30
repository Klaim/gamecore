#ifndef GCORE_TASKSYSTEM_H
#define GCORE_TASKSYSTEM_H
#pragma once

#include <map>
#include <list>
#include <vector>
#include "GC_Common.h"
#include "GC_String.h"
#include "GC_Singleton.h"

#include "GC_TaskProperties.h"

namespace gcore
{
	class Task;

	/** The TaskManager is a tool that let you make your a (main) loop dynamic.
		The purpose is to allow iterative processes to change through runtime.
		@par
		The TaskManager can register user defined Tasks,
		then execute them in ascending priority order when it's executeTasks
		function is called.
		The user application should then call executeTasks each cycle
		of it's (main) loop to keep the tasks updated.
		
		@see Task
	*/
	class GCORE_API TaskManager 
	{
		struct TaskCompare_AscendingPriority;

	public:
		typedef std::list< Task* > TaskList;


		/** Constructor. */
		TaskManager();

		/** Destructor. 
		This method will call terminateAllTasks and unregisterAllTasks .
		*/
		~TaskManager();

		/** Register a non registered Task.
			After registering, the task can be activated with activateTask or Task::activate
			@par
			The Task state will be set to TS_REGISTERED.
			@par
			If the Task has a name that is not "", it is registered in the named index.
			The Task can then be retrieved by providing it's name in getRegisteredTask().
			@see TaskManager::getRegisteredTask
			@param task Task to activate.
		*/
		void registerTask( Task* task );
		
		/** Activate a registered Task.
			After activation, the Task::onActivation() method will be called.
			Once activated, the Task will then be executed each executeTasks call.
			@par
			The Task state will be set to TS_ACTIVE before the call of Task::onActivate().
			@remark Calling this method modify the active Tasks list order that will then
			be sorted automatically before the active tasks are executed, or if you call
			sortTasksList before.
			@see TaskManager::getRegisteredTask
			@param task Task to activate.
		*/
		void activateTask( Task* task );

		/** Pause a registered and active Task.
			Once paused, the Task will not be executed each cycle of the main loop,
			but will remain "loaded". Task::onPaused() method of the paused task will
			then be called. 
			@par
			The Task state will be set to TS_PAUSED before the call of Task::onPaused().
			@param task Task to pause.
		*/
		void pauseTask( Task* task );

		/** Resume a registered and paused Task.
			Once resumed, the Task will be executed each cycle of the main loop as before.
			Task::onResumed() method of the paused task will
			then be called.
			@par
			The Task state will be set to TS_ACTIVE before the call of Task::onResumed().
			@remark Calling this method modify the active Tasks list order that will then
			be sorted automatically before the active tasks are executed, or if you call
			sortTasksList before.
			@param task Task to resume.
		*/
		void resumeTask( Task* task );

		/** Terminate a registered Task.
			After terminating, the Task::onTerminated() method will be called.
			Once terminated, the Task will then not be executed each cycle of the main loop.
			@par
			The Task state will be set to TS_REGISTERED before the call of Task::onTerminate().
			@param task Task to terminate.
		*/
		void terminateTask( Task* task );

		/** Unregister a registered Task.    
			ADD MISSING COMMENTS HERE
		*/
		void unregisterTask( Task* task );

		/** Terminate all registered Tasks.	*/
		void terminateAllTasks();

		/** Terminate all registered Tasks.	*/
		void unregisterAllTasks();

		/** Change the priority of a Task.
			If the Task is active, the active tasks list order will be sorted 
			according to the new priority of the Task before execution.
			@param task Task that have to change of priority.
			@param newPriority New priority value to set to the Task.
		*/
		void changeTaskPriority( Task* task, const TaskPriority& newPriority );


		/** Retrieve a registered Task by it's name.
			This will return null if the name was "".
			@see TaskManager::activateTask
			@param name Name of the Task to retrieve.
			@return Task with the provided name, or null if not found.
		*/
		Task* getRegisteredTask( const String& name ) const;


		/** Execute all Tasks in priority order.
			You should call this method once each
			cycle of your main loop. 
			@remark
			As the active Task list is copied before execution,
			if Tasks execution modifies the active Task list while an execution cycle,
			the changes on task order will 
			be taken account only after the current cycle for the order. The paused and terminated
			tasks will be not be executed in the same execution list.
		*/
		void executeTasks();

		/** Return the current active tasks list.
		*/
		const TaskList& getActiveTasksList() const { return m_activeTaskList; }

		/** Return the current paused tasks list.
		*/
		const TaskList& getPausedTaskList() const { return m_pausedTasksList; }

		/** Return the current registered tasks list.
		*/
		const TaskList& getRegisteredTasksList() const { return m_registeredTasksList; }

		/** Return the named index of all named tasks registered.
		*/
		const std::map< const String , Task*>& getNamedTasksIndex() const {return m_namedTasksIndex;}

	private:

		/** Utility method to deactivate an active Task.
			@remark Private use only!
		*/
		void deactivateTask( Task* task );

		/** Comparison functor used to sort Tasks.
		*/
		struct TaskCompare_AscendingPriority 
		{
			bool operator ()( Task* a1, Task* a2 ) const ;
		};

		/// Index of named registered Tasks.
		std::map< const String , Task* > m_namedTasksIndex;
		
		/// Registered Tasks list.
		TaskList m_registeredTasksList;

		/// Paused Tasks list.
		TaskList m_pausedTasksList;

		/// Active Tasks list.
		TaskList m_activeTaskList;

		/// Execution task list.
		std::vector< Task* > m_executionTaskList;

		/// True if we modified the active list since last execution
		bool m_activeListChanged;
	};

}


#endif