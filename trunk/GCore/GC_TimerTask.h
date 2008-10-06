#ifndef GC_TIMERTASK_H
#define GC_TIMERTASK_H
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
	class GCORE_API TimerTask  : public Task
	{
	public:

		/** Constructor.
			@param timerManager TimerManager to update.
		*/
		TimerTask( TimerManager& timerManager,  TaskPriority priority = 0 ,const String& name = "" );
	
		/** Destructor.
		*/
		~TimerTask();
		
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