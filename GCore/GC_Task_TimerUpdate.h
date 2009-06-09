#ifndef GC_TASK_TIMERUPDATE_H
#define GC_TASK_TIMERUPDATE_H
#pragma once

#include "GC_Common.h"
#include "GC_Task.h"
#include "GC_TimerManager.h"

namespace gcore
{
	/** Task that update all Timers in a TimerManager.

		@see Task
		@see TimerManager

	*/
	class GCORE_API Task_TimerUpdate  : public Task
	{
	public:

		/** Constructor.
			@param timerManager TimerManager to update.
		*/
		Task_TimerUpdate( TimerManager& timerManager,  TaskPriority priority = 0 ,const String& name = "" );
	
		/** Destructor.
		*/
		~Task_TimerUpdate();
		
	protected:

		void onActivate();
		void execute();
		void onTerminate();
		
	private:

		/// TimerManager to update.
		TimerManager& m_timerManager;
	
	};
	

}

#endif