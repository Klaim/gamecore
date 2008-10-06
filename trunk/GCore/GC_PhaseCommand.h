#ifndef GC_PHASECOMMAND_H
#define GC_PHASECOMMAND_H
#pragma once

#include "GC_Common.h"

#include "GC_ConsoleCommand.h"

namespace gcore
{
	class Phase;
	class Console;
	enum PhaseState;

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
	class GCORE_API PhaseCommand : public ConsoleCommand
	{
	public:

		/** Constructor.
			@param phase Phase to manage.
		*/
		PhaseCommand( Phase& phase );
	
		/** Destructor.
		*/
		~PhaseCommand();

		void execute( Console & console , const std::vector< UTFString >& parameterList);

	protected:
		
	private:

		/// Print error and help message in the console.
		void printErrorHelpMessage( Console& console );

		/** Translate phase state to text.   
		*/
		UTFString toText( const PhaseState phaseState ) const ;

		/// Phase to manage.
		Phase& m_phase;
	
	};
	

}

#endif