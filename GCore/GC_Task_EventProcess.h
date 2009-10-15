#ifndef GCORE_TASK_EVENTPROCESS_H
#define GCORE_TASK_EVENTPROCESS_H
#pragma once

#include "GC_Common.h"

#include "GC_Task.h"

namespace gcore
{
	class EventManager;

	/** Task that update an EventManager by calling it's EventManager::processEvents() function each cycle.

		@see Task
		@see EventManager
		
	*/
	class GCORE_API Task_EventProcess  : public Task
	{
	public:
		/** Constructor.
			@param eventManager EventManager that this Task will update.
		*/
		Task_EventProcess(EventManager& eventManager, TaskPriority priority = 0 ,const String& name = "" );

		/** Destruction.
		*/
		~Task_EventProcess();


		EventManager& getEventManager() { return m_eventManager; }

	protected:

		/** Not used.
		*/
		virtual void onActivate(){};

		/** Not used.
		*/
		virtual void onTerminate(){};

		/** Update the EventManager by calling EventManager::processEvents().		
			@see EventManager
		*/
		virtual void execute();

	private:

		///EventManager to update
		EventManager& m_eventManager;


	};

}

#endif