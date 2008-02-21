#ifndef GCORE_CONSOLECOMMAND_H
#define GCORE_CONSOLECOMMAND_H
#pragma once

#include "GC_String.h"
#include <vector>
#include <boost/shared_ptr.hpp>


#include "GC_Common.h"

namespace gcore
{

	class Console;

	/**	Base class for commands that can be used in a Console.
		
		@remark A ConsoleCommand object can be used by multiple Console objects.
		@see Console
	*/
	class GCORE_API ConsoleCommand 
	{
	private:
		

	protected:

		friend class Console;

		///String separator used to parse and separate the different parametters.
		UTFString m_paramSeparators;

	public:

		/**	User defined command execution.
			@param console Console object that execute this command.
			@param parametterList List of parametters provided with the command call.
		*/
		virtual void execute( Console & console , const std::vector< UTFString >& parameterList)=0;

		//////////////////////////////////////////////////////////////////////////

		/** Constructor.
			@param paramSeparators String separator used to parse and separate the different parameters.
		*/
		ConsoleCommand(const UTFString & paramSeparators = L" ")
			:m_paramSeparators(paramSeparators)
				{};

		/** Destructor.
		*/
		virtual ~ConsoleCommand(){};
		


	};

	///Smart pointer for ConsoleCommand
	typedef boost::shared_ptr< ConsoleCommand > ConsoleCommandPtr;

}

#endif

