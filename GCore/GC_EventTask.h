#ifndef GCORE_EVENTTASK_H
#define GCORE_EVENTTASK_H
#pragma once

#include "GC_Common.h"

#include "GC_EventManager.h"
#include "GC_Task.h"

namespace GCore
{

	/** Task that update an EventManager by calling it's EventManager::processEvents() function each cycle.

		@see Task
		@see EventManager
		
	*/
	class GCORE_API EventTask  : public Task
	{
	public:
		/** Constructor.
			@param eventManager EventManager that this Task will update.
		*/
		EventTask(EventManager& eventManager, TaskPriority priority = 0 ,const String& name = "" );

		/** Destruction.
		*/
		~EventTask();

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