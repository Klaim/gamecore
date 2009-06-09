#ifndef GC_PROXYTASK_H
#define GC_PROXYTASK_H
#pragma once

#include <functional>

#include "GC_Common.h"
#include "GC_Task.h"

namespace gcore
{
	typedef std::tr1::function< void () > TaskFunction;

	

	/** Task that simply connect function-like objects to task events and execution methods.
	*/
	class ProxyTask : virtual public Task
	{
	public:

		static inline void emptyFunction() { ; }

		/** Constructor.
		*/
		ProxyTask( const TaskFunction& executeFunction
			, const TaskFunction& onActivateFunction = &ProxyTask::emptyFunction, const TaskFunction& onTerminateFunction = &ProxyTask::emptyFunction
			, const TaskFunction& onPausedFunction = &ProxyTask::emptyFunction, const TaskFunction& onResumedFunction = &ProxyTask::emptyFunction
			, TaskPriority priority = 0, const String& name = "" )
			: Task( priority , name )
			, m_executeFunction( executeFunction )
			, m_onActivateFunction( onActivateFunction )
			, m_onTerminateFunction( onTerminateFunction )
			, m_onPausedFunction( onPausedFunction )
			, m_onResumedFunction( onResumedFunction )
		{
			
		}
	
		/** Destructor.
		*/
		~ProxyTask(){}

		
	protected:

		inline void onActivate() { m_onActivateFunction(); }
		inline void onPaused(){ m_onPausedFunction(); };
		inline void onResumed(){ m_onResumedFunction(); };
		inline void onTerminate() { m_onTerminateFunction(); }
		inline void execute() { m_executeFunction(); }

		
	private:
	
		const TaskFunction m_executeFunction;
		const TaskFunction m_onActivateFunction;
		const TaskFunction m_onTerminateFunction;
		const TaskFunction m_onPausedFunction;
		const TaskFunction m_onResumedFunction;

	};
	

}

#endif