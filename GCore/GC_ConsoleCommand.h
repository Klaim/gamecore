#ifndef GCORE_CONSOLECOMMAND_H
#define GCORE_CONSOLECOMMAND_H
#pragma once


#include <vector>
#include <functional>
#include <memory>
#include <boost/algorithm/string.hpp>
#include "GC_String.h"

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
	public:

		/** Constructor.
			@param name Name of this command, used as an identifier in command call.
		*/
		ConsoleCommand( const LocalizedString& name, const LocalizedString& paramSeparator = L" " )
			: m_name( name )
			, m_paramSeparator( paramSeparator )
		{
			using namespace boost::algorithm;
			trim( m_name );

			GC_ASSERT( !m_name.empty(), "Tried to create a console command with an empty name!" );
			GC_ASSERT( !m_paramSeparator.empty(), "Tried to create a console command with an empty param separator!" );
		}

		virtual ~ConsoleCommand(){}

		const LocalizedString& name() const { return m_name; }
		const LocalizedString& paramSeparator() const { return m_paramSeparator; }

		/**	User defined command execution.
		@param console Console object that execute this command.
		@param parametterList List of parameters provided with the command call.

		@return true if this command have to keep focus : 
		the following entries to the console will directly be sent to this command. 
		Return false to terminate immediately.
		*/
		virtual bool execute( Console& console , const std::vector< LocalizedString >& parameterList) = 0;
	
		/** User defined help text. @see ConsoleCmd_Help */
		virtual LocalizedString help() const = 0;
		
	private:

		LocalizedString m_name;
		LocalizedString m_paramSeparator;

	};

	/// Smart pointer for ConsoleCommand.
	typedef std::tr1::shared_ptr< ConsoleCommand > ConsoleCommandPtr;

	/// Type of function-like object that can be used as a console command.
	typedef std::tr1::function< bool ( Console& , const std::vector< LocalizedString >& parameterList )> ConsoleCommandFunction;

	/** Console command that simply call a provided function.
		@ see ConsoleCommandFunction
	*/
	class ProxyCommand : public ConsoleCommand
	{
	public:

		ProxyCommand( const LocalizedString& name, const ConsoleCommandFunction& commandFunction, const LocalizedString& helpText = L"Proxy command - undefined help text.", const LocalizedString& paramSeparator = L" " )
			: ConsoleCommand( name, paramSeparator )
			, m_commandFunction( commandFunction )
			, m_helpText( helpText )
		{}

		inline bool execute( Console & console , const std::vector< LocalizedString >& parameterList)
		{
			return m_commandFunction( console, parameterList );
		}

		LocalizedString help() const{ return m_helpText; }

	private:

		ConsoleCommandFunction m_commandFunction;
		const LocalizedString m_helpText;

	};

}

#endif

