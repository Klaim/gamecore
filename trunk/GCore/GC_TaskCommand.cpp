#include <sstream>
#include "GC_TaskCommand.h"
#include "GC_Task.h"
#include "GC_Console.h"
#include "GC_UnicodeAscii.h"

namespace gcore
{
	


	TaskCommand::TaskCommand( Task& task )
		: ConsoleCommand()
		, m_task(task)
	{
		
	}

	TaskCommand::~TaskCommand()
	{
		
	}

	void TaskCommand::execute( Console & console , const std::vector< UTFString >& parameterList )
	{
		if(parameterList.size() != 1)
		{
			printErrorHelpMessage(console);
			return;
		}
		bool resultOk = false;

		TaskState taskState = m_task.getState();
		const UTFString& parameter( parameterList.at(0) );

		if(parameter == L"start")
		{
			if(taskState == TS_REGISTERED)
			{
				m_task.activate();
				console.printText(L"Started task " + ToUnicode(m_task.getName()));
			}
			else
			{
				console.printText(UTFString(L"/!\\Invalid task state : ") + toText(taskState) + L" - only " + toText(TS_REGISTERED) + L" is valid!" );
				return;
			}
		}
		else if (parameter == L"pause")
		{
			if(taskState == TS_ACTIVE)
			{
				m_task.pause();
				console.printText(L"Paused task " + ToUnicode(m_task.getName()));
			}
			else
			{
				console.printText(UTFString(L"/!\\Invalid task state : ") + toText(taskState) + L" - the task have to be " + toText(TS_ACTIVE) + L" to be paused!" );
				return;
			}
		}
		else if (parameter == L"resume")
		{
			if(taskState == TS_PAUSED)
			{
				m_task.resume();
				console.printText(L"Resumed task " + ToUnicode(m_task.getName()));
			}
			else
			{
				console.printText(UTFString(L"/!\\Invalid task state : ") + toText(taskState) + L" - the task have to be " + toText(TS_PAUSED) + L" to be resumed!" );
				return;
			}
		}
		else if (parameter == L"stop")
		{
			if(taskState != TS_REGISTERED)
			{
				m_task.terminate();
				console.printText(L"Stopped task " + ToUnicode(m_task.getName()));
			}
			else
			{
				console.printText(UTFString(L"/!\\Invalid task state : ") + toText(taskState) + L" - the task have to be " + toText(TS_ACTIVE) + L" or " + toText(TS_PAUSED) + L" to be stopped!" );
				return;
			}
		}
		else if (parameter == L"state")
		{
			console.printText( UTFString(L"Task ") + ToUnicode(m_task.getName()) +  L" state : "+ toText(taskState) );
		}
		else
		{
			printErrorHelpMessage(console);
		}
	}

	void TaskCommand::printErrorHelpMessage( Console& console )
	{
		console.printText( L"Invalid parameters ! Should be only ONE of the following : ");
		console.printText( L" start, pause, resume, stop, state" );
	}

	UTFString TaskCommand::toText( const TaskState taskState ) const 
	{
		switch( taskState )
		{

		case( TS_ACTIVE ):
			{
				return L"ACTIVE";
			}
		case( TS_PAUSED ):
			{
				return L"PAUSED";
			}
		case( TS_REGISTERED ):
			{
				return L"REGISTERED";
			}
		case( TS_UNREGISTERED ):
			{
				return L"UNREGISTERED";
			}

		default:
			{
				std::stringstream errorMsg;
				errorMsg << "Unknown TaskState being translated in text! State value = " ;
				errorMsg << taskState;
				GC_EXCEPTION( errorMsg.str() );
			}
		}

	}
}