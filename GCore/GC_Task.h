#ifndef GCORE_TASK_H
#define GCORE_TASK_H
#pragma once


#include "GC_Common.h"
#include "GC_String.h"

#include "GC_TaskProperties.h"


namespace gcore
{
	class TaskManager;
	
	/** A Task is an iterative process that should be executed by
		a TaskManager each cycle of a loop. The purpose is
		to dynamize the execution of the loop by allowing 
		changing the iterative processes in the loop through 
		runtime.
		@par
		The Task must be registered in a TaskManager, using
		 TaskManager::registerTask  in order to be
		 usable.
		@par
		The Task::onExecute() method of each active Task will be called
		each time TaskManager::executeTasks is called, that should be 
		each loop cycle. Those Tasks are executed in ascending priority
		order.
		@remark
		A Task can be managed only by one unique TaskManager at the same time.

		@see TaskManager
	*/
	class GCORE_API Task 
	{
	public:

		/** Constructor.
			@param name Name of the Task.
			@param priority Priority of the Task.
		*/
		Task( TaskPriority priority = 0 ,const String& name = "" );

		  /** Destructor.
		  */
		  virtual ~Task();

		/// Name of the Task (if set on construction).
		inline const String& getName() const { return m_name; }
	
		/** Activate the Task.
			The Task must be registered by a TaskManager.
			Once activated, Task::onActivate() method will be called.
			@remark This is a proxy function for TaskManager::activateTask().
			@see TaskManager::activateTask
			@see onActivate
		*/
		void activate();

		/** Pause the Task.
			The Task must be registered in a TaskManager and active.
			Once paused, Task::onPaused() method will be called.
			@remark This is a proxy function for TaskManager::pauseTask().
			@see TaskManager::pauseTask
			@see onActivated
		*/
		void pause();

		/** Resume the Task.
			The Task must be registered in a TaskManager and paused.
			Once resumed, Task::onResumed() method will be called.
			@remark This is a proxy function for TaskManager::resumeTask().
			@see TaskManager::resumeTask
			@see onResumed
		*/
		void resume();

		/** Terminate the Task.
			The Task must be registered in a TaskManager.
			Once terminated, Task::onTerminate() method will be called.
			@remark This is a proxy function for TaskManager::terminateTask().
			@see TaskManager::terminateTask
			@see onTerminate
		*/
		void terminate();

		/** Unregister the Task from the TaskManager in wich it's registered.
			@remark This is a proxy function for TaskManager::unregisterTask().
			@see TaskManager::unregisterTask
		*/
		void unregister();

		//////////////////////////

		/** Change the current priority value of this Task.
			If the Task is registered in a TaskManager and is active, 
			calling this method will change the order of this task in the active
			tasks list according to it's new priority.
			@param priority New priority value for this Task.
		*/
		void setPriority(const TaskPriority& priority);

		/** Priority : ascending order of execution for each cycle.
			@see 
		*/
		inline const TaskPriority& getPriority() const { return m_priority; }

		/// Current state of the Task.
		inline const TaskState& getState() const { return m_state; }

		
		/** The Task manager currently managing this task or null if this task is not registered.
		*/
		inline TaskManager* getTaskManager() const { return m_taskManager; };
	

		bool isActive() const { return m_state == TS_ACTIVE; }
		bool isPaused() const { return m_state == TS_PAUSED; }
		bool isRegistered() const { return m_state != TS_UNREGISTERED; }

	protected:

		/** User defined activation.
			This method will be called once the Task is registered from a TaskManager and activated.
			The Task should then initialize it's data in this method.
		*/
		virtual void onActivate() = 0;

		/** User defined pause.
			This method will be called once the Task is paused.
			The Task should then manage it's data according to it's new state if
			necessary.
		*/
		virtual void onPaused(){ ; };

		/** User defined resume.
			This method will be called once the Task is resumed.
			The Task should then manage it's data according to it's new state if
			necessary.
		*/
		virtual void onResumed(){ ; };

		/** User defined termination.
			This method will be called once the Task is unregistered from a TaskManager.
			The Task should then terminate it's data in this method.
		*/
		virtual void onTerminate() = 0;

		/** Normal execution : call the execution code provided by the user.
			This method will be called each frame.
			@see TaskManager::executeTasks
			@remark This way we allow the redefinition of the way the user
			execution code will be called in a Task type that inherit this one.
		*/
		virtual void onExecute(){ this->execute(); }

		/** User defined execution.
			While the Task is active, this method will be called each time
			TaskManager::executeTasks() through Task::onExecute() method, that should be each application main
			loop cycle. The process of the Task should then be defined in this method.
		*/
		virtual void execute() = 0;

		
	private:

		///Task name.
		const String m_name;

		///Priority : the greatest the first to be executed each cycle.
		TaskPriority m_priority;

		///Current state of the Task.
		TaskState m_state;

		/// Managed by a TaskManager.
		friend class TaskManager;

		/// Current manager of the Task or null if not registered.
		TaskManager* m_taskManager;

	};



	

}

#endif