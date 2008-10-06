#include <sstream>
#include "GC_PhaseCommand.h"
#include "GC_Phase.h"
#include "GC_Console.h"
#include "GC_UnicodeAscii.h"

namespace gcore
{
	
	PhaseCommand::PhaseCommand( Phase& phase )
		: ConsoleCommand()
		, m_phase(phase)
	{
		
	}

	PhaseCommand::~PhaseCommand()
	{
		
	}

	void PhaseCommand::execute( Console & console , const std::vector< UTFString >& parameterList )
	{
		if(parameterList.size() != 1)
		{
			printErrorHelpMessage(console);
			return;
		}
		bool resultOk = false;

		PhaseState phaseState = m_phase.getState();
		const UTFString& parameter( parameterList.at(0) );

		if(parameter == L"load")
		{
			if(phaseState == PS_UNLOADED)
			{
				console.printText(L"Request loading to phase " + ToUnicode(m_phase.getName()));
				m_phase.requestLoad();
			}
			else
			{
				console.printText(UTFString(L"/!\\Invalid phase state : ") + toText(phaseState) + L" - only " + toText(PS_UNLOADED) + L" is valid!" );
				return;
			}
		}
		else if (parameter == L"activate")
		{
			if(phaseState == PS_LOADED)
			{
				console.printText(L"Request activation to phase " + ToUnicode(m_phase.getName()));
				m_phase.requestActivate();
			}
			else
			{
				console.printText(UTFString(L"/!\\Invalid phase state : ") + toText(phaseState) + L" - the phase have to be " + toText(PS_LOADED) + L" to be requested to activate!" );
				return;
			}
		}
		else if (parameter == L"terminate")
		{
			if(phaseState == PS_ACTIVE)
			{
				console.printText(L"Request termination to phase " + ToUnicode(m_phase.getName()));
				m_phase.requestTerminate();
				
			}
			else
			{
				console.printText(UTFString(L"/!\\Invalid phase state : ") + toText(phaseState) + L" - the phase have to be " + toText(PS_ACTIVE) + L" to be requested to terminate!" );
				return;
			}
		}
		else if (parameter == L"unload")
		{
			if(phaseState == PS_LOADED)
			{
				console.printText(L"Request unload to phase " + ToUnicode(m_phase.getName()));
				m_phase.requestUnload();
			}
			else
			{
				console.printText(UTFString(L"/!\\Invalid phase state : ") + toText(phaseState) + L" - the phase have to be " + toText(PS_LOADED) + L" to be requested to unload!" );
				return;
			}
		}
		else if (parameter == L"state")
		{
			console.printText( UTFString(L"Phase ") + ToUnicode(m_phase.getName()) +  L" state : "+ toText(phaseState) );
		}
		else
		{
			printErrorHelpMessage(console);
		}
	}

	void PhaseCommand::printErrorHelpMessage( Console& console )
	{
		console.printText( L"Invalid parameters ! Should be only ONE of the following : ");
		console.printText( L" load, unload, activate, terminate, state" );
	}

	UTFString PhaseCommand::toText( const PhaseState phaseState ) const 
	{
		switch( phaseState )
		{

			/// The phase is unloaded.
		case PS_UNLOADED:
			return L"UNLOADED";

			/// The phase has been requested to load.
		case 	PS_LOADING:
			return L"LOADING";

			/// The phase is loaded and ready to start.
		case 	PS_LOADED:
			return L"LOADED";

			/// The phase has been requested to unload.
		case 	PS_UNLOADING:
			return L"UNLOADING";

			/// The phase has been requested to activate.
		case 	PS_ACTIVATING:
			return L"ACTIVATING";

			/// The phase is currently active.
		case 	PS_ACTIVE:
			return L"ACTIVE";

			/// The phase has been requested to terminate.
		case 	PS_TERMINATING:
			return L"TERMINATING";

		default:
			{
				std::stringstream errorMsg;
				errorMsg << "Unknown PhaseState being translated in text! State value = " ;
				errorMsg << phaseState;
				GC_EXCEPTION( errorMsg.str() );
			}
		}

	}
}