#ifndef GC_TIMEDTASK_H
#define GC_TIMEDTASK_H

#include "GC_Common.h"

#include "GC_Timer.h"
#include "GC_Task.h"
#include "GC_ProxyTask.h"

namespace gcore
{
	class Timer;

	/** Task that execute itself only for a defined time span.
		It will then pause (or terminate) itself.
	*/
	class GCORE_API TimedTask 
		: virtual public Task
		, virtual public TimerListener
	{
	public:

		/** Constructor.
			@see Task::Task
			@param terminateOnceDone Will this task terminate once it's stopped? If no, then it will just pause.
		*/
		TimedTask( bool terminateOnceDone = false , TaskPriority priority = 0 ,const String& name = "" );

		/** Destructor.
		*/
		virtual ~TimedTask();

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

		/// True if this task has to terminate itself once done. False if it will pause.
		const bool m_terminateOnceDone;

		/// True when the timer triggered but we didn't execute yet.
		bool m_timerTriggered;

	};
	
#pragma warning( push )
#pragma warning( disable : 4250 ) // we want to use ProxyTask definitions, yes..

	class TimedProxyTask 
		: public ProxyTask
		, public TimedTask
	{ 
	public:
		TimedProxyTask( const TaskFunction& executeFunction, bool terminateOnceDone = false 
			, const TaskFunction& onActivateFunction = &ProxyTask::emptyFunction, const TaskFunction& onTerminateFunction = &ProxyTask::emptyFunction
			, const TaskFunction& onPausedFunction = &ProxyTask::emptyFunction, const TaskFunction& onResumedFunction = &ProxyTask::emptyFunction
			, TaskPriority priority = 0, const String& name = "" ) 
			: TimedTask( terminateOnceDone, priority, name)
			, ProxyTask( executeFunction, onActivateFunction, onTerminateFunction, onPausedFunction, onResumedFunction, priority, name )
		{}
	};
#pragma warning( pop )

}

#endif