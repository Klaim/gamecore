#ifndef GCORE_TASK_H
#define GCORE_TASK_H
#pragma once

#include "GC_String.h"

#include "GC_Common.h"

namespace gcore
{

	class TaskManager;

	///Task priority type.
	typedef long TaskPriority;

	///Task state.
	enum TASK_STATE
	{
		///The Task is unactive and unregistered.
		TS_UNACTIVE = 0,

		///The Task is registered and active.
		///It's execute() method will be called each main loop cycle.
		TS_ACTIVE,

		///The Task is registered and paused.
		///It's execute() method will not be called until it is resumed.
		TS_PAUSED,
		
	};

	/** A Task is a process that should be executed by
		a TaskManager each cycle of the application main loop.
		
		The Task must be registered in a TaskManager, using
		 TaskManager::activateTask()  in order to be
		 executed each cycle.
		\par
		The Task::execute() method of each active Task will be called
		each time TaskManager::executeTasks() is called, that should be
		each application main loop. Those Tasks are executed in priority
		order : the greatest the last.
		\remark
		A Task can be managed only by one unique TaskManager at the same time.

		@see TaskManager
	*/
	class GCORE_API Task 
	{

	public:

		///Name of the Task.
		const String& getName() const { return m_name; }
	
		/** Activate the Task.
			The Task must be not registered by a TaskManager.
			Once activated, Task::onActivate() method will be called.
			@remark This is a proxy function for TaskManager::activateTask().
			@see TaskManager::activateTask
			@see onActivate
			@param taskManager TaskManager that will manager this Task.
		*/
		void activate(TaskManager* taskManager);

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

		//////////////////////////

		/** Change the current priority value of this Task.
			If the Task is registered in a TaskManager and is active, 
			calling this method will change the order of this task in the active
			tasks list according to it's new priority.
			@param priority New priority value for this Task.
		*/
		void setPriority(const TaskPriority& priority);

		///Priority : the greatest the first to be executed each cycle.
		const TaskPriority& getPriority() const{return m_priority;}

		///Current state of the Task.
		const TASK_STATE& getState() const {return m_state;}

		/** Constructor.
			@param name Name of the Task.
			@param priority Priority of the Task.
		*/
		Task( TaskPriority priority = 0 ,const String& name = "" ):
		  m_name(name), 
		  m_priority(priority), 
		  m_state(TS_UNACTIVE),
		  m_taskManager(nullptr)
		{ }

		/** Destructor.
		*/
		virtual ~Task(){   }
	


	protected:

		///Managed by a TaskManager.
		friend class TaskManager;


		///Current manager of the Task.
		TaskManager* m_taskManager;

		/** Define the name of the Task.
			@remark The Task should be named only one time : 
			if the Task is already named, that it's name
			is not "", calling this method will do nothing.
			@param name New name of the Task.
		*/
		void setName(const String& name){ if(m_name!="")m_name = name; }

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

		/** User defined execution.
			While the Task is active, this method will be called each time
			TaskManager::executeTasks(), that should be each application main
			loop cycle. The process of the Task should then be defined in this method.
		*/
		virtual void execute() = 0;

	private:

		///Task name.
		String m_name;

		///Priority : the greatest the first to be executed each cycle.
		TaskPriority m_priority;

		///Current state of the Task.
		TASK_STATE m_state;

	};

	/** Simple template task class that just call the "void update();" function of the
		provided class.
		This is useful to simply define a Task class that only update a system by calling
		it's update function.
	*/
	template < class UpdatableClass >
	class UpdateTask : 
		public Task
	{
	public:

		/** Constructor.
			@param updatable Object to be updated by this Task. This object class must have a public "void update()" function.
		*/
		UpdateTask( UpdatableClass& updatable ,TaskPriority priority = 0 ,const String& name = ""  ):
		  Task( priority , name ),
		  m_updatable( updatable )
		{ }

		virtual ~UpdateTask(){ }

	protected:
		
		virtual void onActivate(){ }

		virtual void onTerminate(){ }

		virtual void execute()
		{
			m_updatable.update();
		}

	private:

		/// Object to update
		UpdatableClass& m_updatable;
	};

}

#endif