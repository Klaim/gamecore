#ifndef GC_CONSOLECMD_PHASECONTROL_H
#define GC_CONSOLECMD_PHASECONTROL_H
#pragma once

#include "GC_Common.h"

#include "GC_ConsoleCommand.h"
#include "GC_Phase.h"

namespace gcore
{
	class Console;
	

	/** Console Command that just help managing a Phase via a Console.
		It allow the user to link a command to the phase in the console and 
		use it via it's parameters matching the Phase interface :
		@par
		 Parameter | Call
		----------------------------------------------------------
		 load      | phase.requestLoad();
		 activate  | phase.requestActivate();
		 terminate | phase.requestTerminate();
		 unload    | phase.requestUnload();
		 state     | print the current state of the phase

		@par 
		@remark Each successful call will print the phase usage in the console.
		@remark There should be only one parameter for each call.

		@see ConsoleCommand	@see Console
		@see Phase @see Phase::
	*/
	class GCORE_API ConsoleCmd_PhaseControl : public ConsoleCommand
	{
	public:

		/** Constructor.
			@param phase Phase to manage.
		*/
		ConsoleCmd_PhaseControl( const LocalizedString& name, Phase& phase );
	
		/** Destructor.
		*/
		~ConsoleCmd_PhaseControl();

		bool execute( Console & console , const std::vector< LocalizedString >& parameterList);

		LocalizedString help() const;

	protected:
		
	private:

		/// Print error and help message in the console.
		void printErrorHelpMessage( Console& console );

		/** Translate phase state to text.   
		*/
		LocalizedString toText( const Phase::State phaseState ) const ;

		/// Phase to manage.
		Phase& m_phase;
	
	};
	

}

#endif