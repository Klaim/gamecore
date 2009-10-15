#include "GC_ConsoleCmd_Help.h"

#include "GC_Console.h"

namespace gcore
{

	const LocalizedString ConsoleCmd_Help::HELP_COMMAND_NAME( L"help" );
	
	const LocalizedString ConsoleCmd_Help::DEFAULT_HELP_TEXT(L"Print the help text of all the registered commands or the command as second parametter.");


	ConsoleCmd_Help::ConsoleCmd_Help( const LocalizedString& helpText )
		: ConsoleCommand( HELP_COMMAND_NAME )
		, m_helpText( helpText )
	{
	}

	bool ConsoleCmd_Help::execute( Console & console , const std::vector< LocalizedString >& parameterList )
	{
		LocalizedStringStream helpText;

		if( parameterList.empty() )
		{
			// print all the command's help texts

			for( Console::CommandIndex::iterator it = console.m_commandIndex.begin(); it != console.m_commandIndex.end(); ++it )
			{
				const ConsoleCommandPtr command = it->second;
				GC_ASSERT_NOT_NULL( command.get() );

				printCommandHelp( helpText, console.commandCallPrefix(), command );
			}

		}
		else
		{
			// take the first parameter as the command name we want to print the help text of
			const ConsoleCommandPtr command = console.command( parameterList[0] );

			if( command )
			{
				// command found!
				printCommandHelp( helpText, console.commandCallPrefix(), command );
			}
			else
			{
				// command not found!
				helpText << L"Command not found : " <<  parameterList[0];
			}
		}

		console.printText( helpText.str() );

		return false;
	}


	void ConsoleCmd_Help::printCommandHelp( LocalizedStringStream& output, const LocalizedString& commandPrefix, const ConsoleCommandPtr& command )
	{
		output << commandPrefix << command->name() << L"\t : \t" << command->help() << L"\n\n";
	}
}