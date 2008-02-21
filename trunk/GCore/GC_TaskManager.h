#ifndef GCORE_TASKSYSTEM_H
#define GCORE_TASKSYSTEM_H
#pragma once

#include <map>
#include <list>
#include "GC_String.h"
#include "GC_Common.h"
#include "GC_Singleton.h"
#include "GC_Task.h"


namespace gcore
{

	/** The TaskManager is a tool that let you dynamize your main loop.
		
		The TaskManager can register user defined Tasks,
		then execute them in priority order when it's executeTasks()
		function is called.
		The user application should then call executeTasks each cycle
		of it's main loop to keep the Tasks updated.
		\par
		Using TaskManager to manage your main loop is powerful but
		not as fast as a simple list of functions to call. As a copy of
		the current list of active tasks is made before execution of those
		tasks (always in priority order) it let you modify the active list (by
		pausing, resuming, activating or removing tasks) while the current 
		execution list is processed. The copy of the active tasks list
		is fast because it's only a list of	pointers but then it's
		proportional to the number of task activated in the same time (O(n)).
		A good way to keep the execution fast as if it was a simple list of function to call
		is to have a Task by "manager class" that manage one module of your application
		and that update this manager class.
	
		@see Task
	*/
	class GCORE_API TaskManager 
	{
	private:
		
	
		///Index of named registered Tasks.
		std::map< String , Task*> m_namedTasksIndex;

		///Paused Tasks list.
		std::list<Task*> m_pausedTaskList;
		
		///Active Tasks list.
		std::list<Task*> m_activeTaskList;

		///Execution task list.
		std::list<Task*> m_executionTaskList;
		
		///Does the active list needs to be sorted?
		bool m_needSort;

	public:

		
		/** Register and activate a non registered Task.
			After registering, the Task::onActivate() method will be called.
			Once activated, the Task will then be executed each cycle of the main loop.
			\par
			The Task state will be set to TS_ACTIVE before the call of Task::onActivate().
			\par
			If the Task has a name that is not "", it is registered in the named index.
			The Task can then be retrieved by providing it's name in getRegisteredTask().
			@remark Calling this method modify the active Tasks list order that will then
			be sorted automatically before the active tasks are executed, or if you call
			sortTasksList before.
			@see TaskManager::getRegisteredTask
			@param task Task to activate.
		*/
		void activateTask(Task* task);

		/** Pause a registered and active Task.
			Once paused, the Task will not be executed each cycle of the main loop,
			but will remain registered. Task::onPaused() method of the paused task will
			then be called. 
			\par
			The Task state will be set to TS_PAUSED before the call of Task::onPaused().
			@param task Task to pause.
		*/
		void pauseTask(Task* task);

		/** Resume a registered and paused Task.
			Once resumed, the Task will be executed each cycle of the main loop as before.
			Task::onResumed() method of the paused task will
			then be called.
			\par
			The Task state will be set to TS_ACTIVE before the call of Task::onResumed().
			@remark Calling this method modify the active Tasks list order that will then
			be sorted automatically before the active tasks are executed, or if you call
			sortTasksList before.
			@param task Task to resume.
		*/
		void resumeTask(Task* task);

		/** Terminate and unregister a registered Task.
			After unregistering, the Task::onTerminate() method will be called.
			Once terminated, the Task will then not be executed each cycle of the main loop.
			\par
			The Task state will be set to TS_UNACTIVE before the call of Task::onTerminate().
			@param task Task to activate.
		*/
		void terminateTask(Task* task);

		/**	Pause all registered and active Tasks.*/
		void pauseAllTasks();

		/** Resume all registered and paused Tasks.	*/
		void resumeAllTasks();

		/** Terminate all registered Tasks.	*/
		void terminateAllTasks();

		/** Terminate all registered and paused Tasks.*/
		void terminateAllPausedTasks();

		/** Terminate all registered and active Tasks. */
		void terminateAllActiveTasks();


		/** Change the priority of a Task.
			If the Task is active, the active tasks list order will be sorted 
			according to the new priority of the Task before execution.
			@param task Task that have to change of priority.
			@param newPriority New priority value to set to the Task.
		*/
		void changeTaskPriority(Task* task,const TaskPriority& newPriority);


		/** Retrieve a registered Task by it's name.
			This will not work if the name was "".
			@see TaskManager::activateTask
			@param name Name of the Task to retrieve.
			@return Task with the provided name, or nullptr if not found.
		*/
		Task* getRegisteredTask(const String& name) const;

		/** Constructor. */
		TaskManager();

		/** Destructor. 
		This method will call terminateAllTasks().
		*/
		~TaskManager();

		/** Count registered paused Tasks.
			@return result count.
		*/
		unsigned long countPausedTasks() const;
		
		/** Count registered active Tasks.
			@return result count.
		*/
		unsigned long countActiveTasks() const;
		
		/** Count registered named Tasks.
			@return result count.
		*/
		unsigned long countNamedTasks() const;
		
		/** Count all registered Tasks.
			@return result count.
		*/
		unsigned long countTotalTasks() const;

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

		/** Sort the active Tasks by priority order (the greater the last) if necessary.
		This method will do nothing if no changes have been registered in active Task order.
		*/
		void sortActiveTasks();

		/** Return false if the active tasks list is not sorted.
		In this case, the active tasks list will be sort before tasks execution in executeTasks,
		or until sortActiveTasks() is called.
		*/
		bool isActiveTasksSorted() const { return !m_needSort;}

		/** Return the current active tasks list.
		*/
		const std::list<Task*>& getActiveTasksList() const {return m_activeTaskList;}

		/** Return the current paused tasks list.
		*/
		const std::list<Task*>& getPausedTasksList() const {return m_pausedTaskList;}

		/** Return the named index of all named tasks registered.
		*/
		const std::map< String , Task*>& getNamedTasksIndex() const {return m_namedTasksIndex;}

	};

}


#endif