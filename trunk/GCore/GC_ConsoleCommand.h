#ifndef GCORE_CONSOLECOMMAND_H
#define GCORE_CONSOLECOMMAND_H
#pragma once

#include "GC_String.h"
#include <vector>
#include <functional>
#include <memory>
//#include <boost/function.hpp>
//#include <boost/shared_ptr.hpp>


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

	/// Smart pointer for ConsoleCommand.
	//typedef boost::shared_ptr< ConsoleCommand > ConsoleCommandPtr;
	typedef std::tr1::shared_ptr< ConsoleCommand > ConsoleCommandPtr;

	/// Type of function-like object that can be used as a console command.
	typedef std::tr1::function< void ( Console& , const std::vector< UTFString >& parameterList )> ConsoleCommandFunction;
	//typedef boost::function< void ( Console& , const std::vector< UTFString >& ) > ConsoleCommandFunction;

	/** Console command that simply call a provided function.
		@ see ConsoleCommandFunction
	*/
	class ProxyCommand : public ConsoleCommand
	{
	public:

		ProxyCommand( const ConsoleCommandFunction& commandFunction, const UTFString& paramSeparators = L" "  )
			: ConsoleCommand( paramSeparators )
			, m_commandFunction( commandFunction )
		{}

		inline void execute( Console & console , const std::vector< UTFString >& parameterList)
		{
			m_commandFunction( console, parameterList );
		}

	private:

		ConsoleCommandFunction m_commandFunction;

	};

}

#endif

