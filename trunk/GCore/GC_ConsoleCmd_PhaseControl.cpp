#include "GC_ConsoleCmd_PhaseControl.h"

#include <sstream>

#include "GC_Console.h"
#include "GC_UnicodeAscii.h"

namespace gcore
{
	
	ConsoleCmd_PhaseControl::ConsoleCmd_PhaseControl( const LocalizedString& name, Phase& phase )
		: ConsoleCommand( name )
		, m_phase(phase)
	{
		
	}

	ConsoleCmd_PhaseControl::~ConsoleCmd_PhaseControl()
	{
		
	}

	bool ConsoleCmd_PhaseControl::execute( Console & console , const std::vector< LocalizedString >& parameterList )
	{
		if(parameterList.size() != 1)
		{
			printErrorHelpMessage(console);
			return false;
		}
		bool resultOk = false;

		Phase::State phaseState = m_phase.getState();
		const LocalizedString& parameter( parameterList.at(0) );

		if(parameter == L"load")
		{
			if(phaseState == Phase::UNLOADED)
			{
				console.printText(L"Request loading to phase " + AsciiToUTF16(m_phase.getName()));
				m_phase.requestLoad();
			}
			else
			{
				console.printText(LocalizedString(L"/!\\Invalid phase state : ") + toText(phaseState) + L" - only " + toText(Phase::UNLOADED) + L" is valid!" );
				
			}
		}
		else if (parameter == L"activate")
		{
			if(phaseState == Phase::LOADED)
			{
				console.printText(L"Request activation to phase " + AsciiToUTF16(m_phase.getName()));
				m_phase.requestActivate();
			}
			else
			{
				console.printText(LocalizedString(L"/!\\Invalid phase state : ") + toText(phaseState) + L" - the phase have to be " + toText(Phase::LOADED) + L" to be requested to activate!" );
				
			}
		}
		else if (parameter == L"terminate")
		{
			if(phaseState == Phase::ACTIVE)
			{
				console.printText(L"Request termination to phase " + AsciiToUTF16(m_phase.getName()));
				m_phase.requestTerminate();
				
			}
			else
			{
				console.printText(LocalizedString(L"/!\\Invalid phase state : ") + toText(phaseState) + L" - the phase have to be " + toText(Phase::ACTIVE) + L" to be requested to terminate!" );
				
			}
		}
		else if (parameter == L"unload")
		{
			if(phaseState == Phase::LOADED)
			{
				console.printText(L"Request unload to phase " + AsciiToUTF16(m_phase.getName()));
				m_phase.requestUnload();
			}
			else
			{
				console.printText(LocalizedString(L"/!\\Invalid phase state : ") + toText(phaseState) + L" - the phase have to be " + toText(Phase::LOADED) + L" to be requested to unload!" );
				
			}
		}
		else if (parameter == L"state")
		{
			console.printText( LocalizedString(L"Phase ") + AsciiToUTF16(m_phase.getName()) +  L" state : "+ toText(phaseState) );
		}
		else
		{
			printErrorHelpMessage(console);
		}

		return false;
	}

	void ConsoleCmd_PhaseControl::printErrorHelpMessage( Console& console )
	{
		console.printText( L"Invalid parameters ! Should be only ONE of the following : ");
		console.printText( L" load, unload, activate, terminate, state" );
	}

	LocalizedString ConsoleCmd_PhaseControl::toText( const Phase::State phaseState ) const 
	{
		switch( phaseState )
		{

			/// The phase is unloaded.
		case Phase::UNLOADED:
			return L"UNLOADED";

			/// The phase has been requested to load.
		case 	Phase::LOADING:
			return L"LOADING";

			/// The phase is loaded and ready to start.
		case 	Phase::LOADED:
			return L"LOADED";

			/// The phase has been requested to unload.
		case 	Phase::UNLOADING:
			return L"UNLOADING";

			/// The phase has been requested to activate.
		case 	Phase::ACTIVATING:
			return L"ACTIVATING";

			/// The phase is currently active.
		case 	Phase::ACTIVE:
			return L"ACTIVE";

			/// The phase has been requested to terminate.
		case 	Phase::TERMINATING:
			return L"TERMINATING";

		default:
			{
				std::stringstream errorMsg;
				errorMsg << "Unknown Phase::State being translated in text! State value = " ;
				errorMsg << phaseState;
				GC_EXCEPTION << errorMsg.str();
			}
		}

	}

	LocalizedString ConsoleCmd_PhaseControl::help() const
	{
		return	L"help managing a Phase via a Console. \n" \
				L"It allow the user to link a command to the phase in the console and " \
				L"use it via it's parameters matching the Phase interface :" \
				L"\n\n" \
				L"Parameter | Call \n" \
				L"----------------------------------------------------------\n" \
				L"load      | phase.requestLoad();\n" \
				L"activate  | phase.requestActivate();\n" \
				L"terminate | phase.requestTerminate();\n" \
				L"unload    | phase.requestUnload();\n" \
				L"state     | print the current state of the phase" ;
	}
}