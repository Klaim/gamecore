#ifndef GC_CONSOLECMD_HELP_H
#define GC_CONSOLECMD_HELP_H
#pragma once

#include "GC_StringStream.h"

#include "GC_Common.h"

#include "GC_ConsoleCommand.h"

namespace gcore
{
	/** Print help texts from all registered commands of a console.
	*/
	class GCORE_API ConsoleCmd_Help
		: public ConsoleCommand
	{
	public:

		static const LocalizedString HELP_COMMAND_NAME;
		static const LocalizedString DEFAULT_HELP_TEXT;

		ConsoleCmd_Help( const LocalizedString& helpText = DEFAULT_HELP_TEXT );
		~ConsoleCmd_Help(){}

		
	protected:
		
	private:

		bool execute( Console & console , const std::vector< LocalizedString >& parameterList);

		LocalizedString help() const { return m_helpText; }

		void printCommandHelp( LocalizedStringStream& output, const LocalizedString& commandPrefix, const ConsoleCommandPtr& command );

		const LocalizedString m_helpText;

	};
	

}

#endif