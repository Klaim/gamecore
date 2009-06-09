#include <sstream>
#include "GC_ConsoleCmd_TaskControl.h"
#include "GC_Task.h"
#include "GC_Console.h"
#include "GC_UnicodeAscii.h"

namespace gcore
{
	


	ConsoleCmd_TaskControl::ConsoleCmd_TaskControl( const LocalizedString& name, Task& task )
		: ConsoleCommand( name )
		, m_task(task)
	{
		
	}

	ConsoleCmd_TaskControl::~ConsoleCmd_TaskControl()
	{
		
	}

	bool ConsoleCmd_TaskControl::execute( Console & console , const std::vector< LocalizedString >& parameterList )
	{
		if(parameterList.size() != 1)
		{
			printErrorHelpMessage(console);
			return false;
		}
		bool resultOk = false;

		TaskState taskState = m_task.getState();
		const LocalizedString& parameter( parameterList.at(0) );

		if(parameter == L"start")
		{
			if(taskState == TS_REGISTERED)
			{
				m_task.activate();
				console.printText(L"Started task " + AsciiToUTF16(m_task.getName()));
			}
			else
			{
				console.printText(LocalizedString(L"/!\\Invalid task state : ") + toText(taskState) + L" - only " + toText(TS_REGISTERED) + L" is valid!" );
			
			}
		}
		else if (parameter == L"pause")
		{
			if(taskState == TS_ACTIVE)
			{
				m_task.pause();
				console.printText(L"Paused task " + AsciiToUTF16(m_task.getName()));
			}
			else
			{
				console.printText(LocalizedString(L"/!\\Invalid task state : ") + toText(taskState) + L" - the task have to be " + toText(TS_ACTIVE) + L" to be paused!" );
				
			}
		}
		else if (parameter == L"resume")
		{
			if(taskState == TS_PAUSED)
			{
				m_task.resume();
				console.printText(L"Resumed task " + AsciiToUTF16(m_task.getName()));
			}
			else
			{
				console.printText(LocalizedString(L"/!\\Invalid task state : ") + toText(taskState) + L" - the task have to be " + toText(TS_PAUSED) + L" to be resumed!" );
				
			}
		}
		else if (parameter == L"stop")
		{
			if(taskState != TS_REGISTERED)
			{
				m_task.terminate();
				console.printText(L"Stopped task " + AsciiToUTF16(m_task.getName()));
			}
			else
			{
				console.printText(LocalizedString(L"/!\\Invalid task state : ") + toText(taskState) + L" - the task have to be " + toText(TS_ACTIVE) + L" or " + toText(TS_PAUSED) + L" to be stopped!" );
				
			}
		}
		else if (parameter == L"state")
		{
			console.printText( LocalizedString(L"Task ") + AsciiToUTF16(m_task.getName()) +  L" state : "+ toText(taskState) );
		}
		else
		{
			printErrorHelpMessage(console);
		}

		return false;
	}

	void ConsoleCmd_TaskControl::printErrorHelpMessage( Console& console )
	{
		console.printText( L"Invalid parameters ! Should be only ONE of the following : ");
		console.printText( L" start, pause, resume, stop, state" );
	}

	LocalizedString ConsoleCmd_TaskControl::toText( const TaskState taskState ) const 
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

	LocalizedString ConsoleCmd_TaskControl::help() const
	{
		return	L"Help managing a Task via a Console.\n" \
				L"It allow the user to link a command to the task in the console and " \
				L"use it via it's parameters matching the Task interface : " \
				L"\n\n" \
				L"Parameter | Call\n" \
				L"----------------------------------------------------------\n" \
				L"start     | task.activate();\n" \
				L"pause     | task.pause();\n" \
				L"resume    | task.resume();\n" \
				L"stop      | task.pause();\n" \
				L"state     | print the current state of the task\n" ;
	}
}