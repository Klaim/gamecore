#ifndef GC_TASKCOMMAND_H
#define GC_TASKCOMMAND_H

#include "GC_Common.h"

#include "GC_ConsoleCommand.h"

namespace GCore
{
	class Task;
	class TaskManager;
	class Console;
	enum TASK_STATE;

	/** Console Command that just help managing a Task via a Console.
		It allow the user to link a command to the task in the console and 
		use it via it's parameters matching the Task interface :
		@par
		 Parameter | Call
		----------------------------------------------------------
		 start     | task.activate();
		 pause     | task.pause();
		 resume    | task.resume();
		 stop      | task.pause();
		 state     | print the current state of the task

		@par 
		@remark Each successful call will print the task usage in the console.
		@remark There should be only one parameter for each call.

		@see ConsoleCommand	@see Console
		@see Task @see Task::
	*/
	class GCORE_API TaskCommand : public ConsoleCommand
	{
	public:

		/** Constructor.
			@param task Task to manage.
			@param taskManager TaskManager to activate the task with.
		*/
		TaskCommand( Task& task , TaskManager& taskManager );
	
		/** Destructor.
		*/
		~TaskCommand();

		void execute( Console & console , const std::vector< UTFString >& parameterList);

	protected:
		
	private:

		/// Print error and help message in the console.
		void printErrorHelpMessage( Console& console );

		/** Translate task state to text.   
		*/
		UTFString toText( TASK_STATE taskState );

		/// Task to manage.
		Task& m_task;

		/// TaskManager to activate the task in.
		TaskManager& m_taskManager;
	
	};
	

}

#endif