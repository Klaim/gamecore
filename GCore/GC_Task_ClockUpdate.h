#ifndef GCORE_TASK_CLOCK_UPDATE_H
#define GCORE_TASK_CLOCK_UPDATE_H
#pragma once

#include "GC_Common.h"

#include "GC_ClockManager.h"
#include "GC_Task.h"

namespace gcore
{

	/** Task that update an ClockManager by calling it's ClockManager::updateClocks() function each cycle.

	@see Task
	@see ClockManager

	*/
	class GCORE_API Task_ClockUpdate  : public Task
	{
	public:
		/** Constructor.
			@param clockManager ClockManager that this Task will update.
			@param priority Task priority for this clock update.
			@param name Name of this task.
		*/
		Task_ClockUpdate(ClockManager& clockManager, TaskPriority priority = 0 ,const String& name = "" );

		/** Destruction.
		*/
		~Task_ClockUpdate();

	protected:

		/** Not used.
		*/
		virtual void onActivate(){};

		/** Not used.
		*/
		virtual void onTerminate(){};

		/** Update the EventManager by calling ClockManager::updateClocks().		
			@see EventManager
		*/
		void execute();

	private:

		///ClockManager to update
		ClockManager& m_clockManager;


	};

}

#endif