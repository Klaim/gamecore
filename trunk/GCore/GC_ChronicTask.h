#ifndef GC_CHRONICTASK_H
#define GC_CHRONICTASK_H
#pragma once

#include "GC_Common.h"
#include "GC_Task.h"
#include "GC_Timer.h"
#include "GC_ProxyTask.h"

namespace gcore
{
	/** Task that execute itself only each fixed time span.
		It will then pause (or terminate) itself.
	*/
	class GCORE_API ChronicTask 
		: virtual public Task
		, virtual public TimerListener
	{
	public:

		/** Constructor.
			@see Task::Task
		*/
		ChronicTask( TaskPriority priority = 0 ,const String& name = "" );

		/** Destructor.
		*/
		virtual ~ChronicTask();

		/// Timer that will notify this task that the execution time passed or null if not set.
		Timer* getTimer() const { return m_timer; }

		void setTimer( Timer* timer );

	protected:

		void onTimerTrigger( Timer& timer );

		/** Execution behavior : here we execute the task only if the
		given time to pass on it is not finished.
		*/
		virtual void onExecute();

	private:

		/// Timer that will notify this task that the execution time passed or null if not set.
		Timer* m_timer;

		/// True when the timer triggered but we didn't execute yet.
		bool m_timerTriggered;

	};

#pragma warning( push )
#pragma warning( disable : 4250 ) // we want to use ProxyTask definitions, yes..

	class ChronicProxyTask 
		: public ProxyTask
		, public ChronicTask
	{ 
	public:
		ChronicProxyTask( const TaskFunction& executeFunction
			, const TaskFunction& onActivateFunction = &ProxyTask::emptyFunction, const TaskFunction& onTerminateFunction = &ProxyTask::emptyFunction
			, const TaskFunction& onPausedFunction = &ProxyTask::emptyFunction, const TaskFunction& onResumedFunction = &ProxyTask::emptyFunction
			, TaskPriority priority = 0, const String& name = "" ) 
			: ChronicTask( priority, name)
			, ProxyTask( executeFunction, onActivateFunction, onTerminateFunction, onPausedFunction, onResumedFunction, priority, name )
		{}
	};
#pragma warning( pop )

}

#endif