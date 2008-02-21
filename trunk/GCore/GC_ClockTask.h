#ifndef GCORE_CLOCKTASK_H
#define GCORE_CLOCKTASK_H
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
	class GCORE_API ClockTask  : public Task
	{
	public:
		/** Constructor.
			@param clockManager ClockManager that this Task will update.
			@param priority Task priority for this clock update.
			@param name Name of this task.
		*/
		ClockTask(ClockManager& clockManager, TaskPriority priority = 0 ,const String& name = "" );

		/** Destruction.
		*/
		~ClockTask();

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
		virtual void execute();

	private:

		///ClockManager to update
		ClockManager& m_clockManager;


	};

}

#endif