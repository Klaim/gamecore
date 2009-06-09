#include "GC_Console.h"

#include <cmath>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

#include "GC_ConsoleCmd_Help.h"

namespace gcore
{
	const LocalizedString Console::DEFAULT_PREFIX( L"/" );

	Console::Console(unsigned long maxEntries , unsigned long maxEntryLength  , unsigned long maxTexts )
		: m_entry()
		, m_maxEntries( maxEntries )
		, m_maxEntryLength(maxEntryLength)
		, m_maxTexts(maxTexts)
		, m_lastEntries( maxEntries )
		, m_lastTexts( maxTexts )
		, m_commandCallPrefix( DEFAULT_PREFIX )
		, m_cursorPos( 0 )
		, m_entryHistoryCursorPos( 0 )
		, m_printCommandOnExecute( true )
	{
		m_paramList.resize( 8 ); // this is arbitrary
		for( int i = 0; i < 8 ; ++i )
		{
			m_paramList[i].reserve(m_maxEntryLength);
		}

		// memory optimization
		m_entry.reserve( maxEntryLength );
		
	}

	
	
	Console::~Console()
	{

	}


	void Console::parseParameters(const LocalizedString& paramString, std::vector< LocalizedString >& paramList,const LocalizedString& paramSeparators)
	{
		paramList.clear();

		typedef boost::tokenizer< boost::char_separator<LocalizedString::value_type>, LocalizedString::const_iterator,	LocalizedString > my_tok;

		boost::char_separator<LocalizedString::value_type> sep( paramSeparators.c_str() );
		my_tok tok( paramString, sep );

		//unsigned int i = 0;
		for ( my_tok::iterator it = tok.begin();it != tok.end();++it )
		{
			paramList.push_back( (*it) );
		};

	}
	
	long Console::injectKeys(const LocalizedString& keys)
	{
		if(keys.empty())return 0;


		long result = 0;

		const unsigned long totalLength = static_cast< unsigned long >( keys.length() + m_entry.length() );
		const long overLength = totalLength - static_cast< long >( m_maxEntryLength );

		if( overLength > 0 )// we don't take all the characters
		{
			const unsigned long finalLength = static_cast<unsigned long>( keys.length() - overLength );
			if( finalLength > 0 )
			{
				const LocalizedString finalKeys = keys.substr( 0, finalLength );	
				insertEntryText( finalKeys );
			}
			
			result = overLength;
		}
		else insertEntryText( keys ); // we take it all!

		onEntryChanged();

		return result;
	}


	void Console::insertEntryText( const LocalizedString& text )
	{
		GC_ASSERT( m_cursorPos <= m_entry.length(), "Tried to insert text in an invalid location of the entry!" );

		if( m_cursorPos < m_entry.length() )
		{
			m_entry.insert( m_cursorPos, text );	
		}
		else
		{
			// cursor at the end of the entry
			m_entry += text;
		}
		
		moveCursor( static_cast<unsigned long>( text.length() ) );
	}


	void Console::printText(const LocalizedString& text)
	{
		if( text.empty() ) // do nothing if empty
			return;

		LocalizedString textToPrint( text );

		/*if( m_lastTexts.size() + 1 > m_maxTexts )
			m_lastTexts.erase( m_lastTexts.begin() );*/

		m_lastTexts.push_back(textToPrint);
		
		onAddText(textToPrint);
	}
	

	void Console::executeEntry()
	{
		moveCursorToBegin(); // reset cursor
		m_entryHistoryCursorPos = 0; // reset entry history cursor

		//do nothing if the entry is empty :
		boost::algorithm::trim( m_entry );
		if(m_entry.empty())return;

		//store the entry and move the older entry if not enough place:
		/*if( m_lastEntries.size() + 1 > m_maxEntries )
			m_lastEntries.erase( m_lastEntries.begin() );*/

		m_lastEntries.push_back(m_entry);

		if( m_activeCommand.get() != nullptr )
		{
			// there is already an active command : just send it the parameters!
			parseParameters( m_entry, m_paramList, m_activeCommand->getParamSeparator() );
			
			executeActiveCommand();
		}
		else
		{
			const LocalizedString commandName = extractCommandCall( m_entry );

			// is there a command call?
			if(!commandName.empty())
			{
				// yes! 
				//check that the command is registered : 
				CommandIndex::iterator it;
				it = m_commandIndex.find(commandName);

				if(it!=m_commandIndex.end())
				{
					//command found!
					m_activeCommand = it->second;

					//print the entry!
					if( m_printCommandOnExecute ) printEntry();

					//get the parameters : 
					const LocalizedString params = m_entry.substr( commandName.length() + m_commandCallPrefix.length() );
					parseParameters( params, m_paramList, m_activeCommand->getParamSeparator() );

					// then execute it!
					executeActiveCommand();

				}
				else
				{
					//command not found!
					printText( L"'" + commandName + L"' command unknown.");

				}

			}else
			{
				//execute default command with the entry as parameter if defined:
				if(m_defaultCommand)
				{
					parseParameters( m_entry, m_paramList, m_defaultCommand->getParamSeparator() );
					m_defaultCommand->execute((*this),m_paramList);

				}
				else
				{
					//no default command : we just print the entry
					printEntry();
				}
			}
		}

		//reset the entry : 
		m_entry.clear();
		onEntryChanged(); // notify
	}

	void Console::addCommand( const ConsoleCommandPtr& command )
	{
		GC_ASSERT( !command->getName().empty(), "Tried to add an unnamed ConsoleCommand!" );
		GC_ASSERT( &command != nullptr && command.get() != nullptr , "Tried to add a null ConsoleCommand!");
		m_commandIndex[ command->getName() ] = command;
	}

	void Console::removeCommand(const LocalizedString& name)
	{
		GC_ASSERT( !name.empty(), "Tried to remove an unnamed ConsoleCommand!" );
		GC_ASSERT( name != ConsoleCmd_Help::HELP_COMMAND_NAME, "Tried to remove the help command!" );

		CommandIndex::iterator it;
		it=m_commandIndex.find(name);
		if(it!=m_commandIndex.end())
		{
			m_commandIndex.erase(it);
			return;
		}
		
		// command not found!
	}

	ConsoleCommandPtr Console::getCommand(const LocalizedString& name)
	{
		GC_ASSERT( !name.empty(), "Tried to get an unnamed ConsoleCommand!" );

		CommandIndex::iterator it;
		it = m_commandIndex.find(name);

		if(it!=m_commandIndex.end())
		{
			return it->second;
		}

		return ConsoleCommandPtr();
	}

	void Console::setDefaultCommand(const LocalizedString& name)
	{
		//just set nullptr if the name is empty
		if(name.empty())
		{
			m_defaultCommand.reset();
			return;
		}

		CommandIndex::iterator it;
		it=m_commandIndex.find(name);
		if(it!=m_commandIndex.end())
		{
			m_defaultCommand = it->second;
		}
	}
	
	void Console::setCommandCallPrefix( const LocalizedString& commandCallPrefix )
	{
		m_commandCallPrefix = boost::algorithm::trim_copy( commandCallPrefix );
		
		GC_ASSERT( !m_commandCallPrefix.empty(), "Tried to set an empty command call prefix!" );
	}

	void Console::setEntry( const LocalizedString& entry )
	{
		m_entry = entry;
		
		if(m_entry.length()>m_maxEntryLength)m_entry = m_entry.substr(0,m_maxEntryLength);

		moveCursorToEnd(); 
		onEntryChanged();
	}

	LocalizedString Console::extractCommandCall( const LocalizedString& entry ) const
	{
		const LocalizedString trimmedEntry = boost::algorithm::trim_copy( entry );
		if(trimmedEntry.empty()) return L""; // be lazy!
		
		// is there a command call?
		if( boost::algorithm::starts_with( trimmedEntry , m_commandCallPrefix ) ) 
		{
			// command call found!
			const size_t prefixLength = m_commandCallPrefix.length();
			const LocalizedString commandName = m_entry.substr( prefixLength, m_entry.find_first_of(L" ") - prefixLength);

			return commandName;
		}
		else return L"";
	}

	void Console::executeActiveCommand()
	{
		GC_ASSERT( m_activeCommand.get() != nullptr, "Tried to call a null active command!" );

		//execute the command :
		const bool keepActive = m_activeCommand->execute((*this),m_paramList);

		if( !keepActive )
		{
			// no more active command
			m_activeCommand.reset(); 
		}
	}

	void Console::printEntry()
	{
		if(m_entry.empty()) return; // be lazy!

		onEntryPrint(); // notify
		printText(m_entry); // display
	}

	bool Console::setCursorPosition( unsigned long cursorPos )
	{
		bool rightMove = false;

		if( m_entry.empty() )
		{
			m_cursorPos = 0;
		}
		else if( cursorPos <= m_entry.size() )
		{
			m_cursorPos = cursorPos;
			rightMove = true;
		}
		else
		{
			// out of entry bound!
			moveCursorToEnd();
		}

		onCursorMoved();

		return rightMove;
	}

	unsigned long Console::moveCursor( unsigned long steps, bool toTheright )
	{
		if( m_entry.empty() )
		{
			setCursorPosition( 0 );
			return 0;
		}

		const int stepUnit = toTheright ? 1 : -1;
		const long wantedCursorPos = long(m_cursorPos) + ( steps * stepUnit );
		
		if( wantedCursorPos < 0 )
		{
			// we want to move before the first character...
			const unsigned long oldPos = m_cursorPos;
			moveCursorToBegin();
			return oldPos  ; // we moved from the old cursor position to the beginning of the entry
		}	
		else if( wantedCursorPos < static_cast< long >( m_entry.length() ) )
		{
			// the new position is right
			const unsigned long finalMove = std::abs( static_cast<long>(m_cursorPos - wantedCursorPos) );
			setCursorPosition( wantedCursorPos );
			return finalMove;
		}
		else
		{
			// we want to move after the last character...
			const unsigned long oldPos = m_cursorPos;
			moveCursorToEnd();
			return static_cast<unsigned long>( m_entry.length() - oldPos); // we moved from the old cursor position to the end of the entry
		}
		
	}

	void Console::moveCursorToEnd()
	{
		setCursorPosition( static_cast<unsigned long>( m_entry.size() ) );
	}

	void Console::moveCursorToBegin()
	{
		setCursorPosition( 0 );
	}

	unsigned long Console::removeEntryKeys( unsigned long keyCount, bool onTheLeft )
	{
		if( m_entry.empty() || keyCount == 0 )
		{
			return 0;
		}

		const std::size_t previousLength = m_entry.length();

		if( onTheLeft )
		{
			if( m_cursorPos > 0 )
			{
				const unsigned long oldPos = m_cursorPos;
				moveCursor( keyCount, false );

				m_entry.erase( oldPos - 1, keyCount );
				
			}
		}
		else
		{
			if( m_cursorPos < m_entry.length() )
			{
				const unsigned long oldPos = m_cursorPos;
				
				moveCursor( keyCount - 1, true );
				m_entry.erase( oldPos + 1 - keyCount, keyCount );
				
			}

		}

		const std::size_t currentLength = m_entry.length();
		const std::size_t removedCharCount = previousLength - currentLength;

		return static_cast< unsigned long >( removedCharCount );
	}

	void Console::moveEntryHistoryCursor( unsigned long steps /*= 1*/, bool toPast /*= true */ )
	{
		if(m_lastEntries.empty()) return; // be lazy!

		const long stepUnit = toPast ? 1 : -1;
		const long wantedEntryIdx = (static_cast<long>(m_entryHistoryCursorPos) - 1) + (static_cast<long>(steps) * stepUnit);

		if( wantedEntryIdx >= 0 && static_cast<std::size_t>( wantedEntryIdx ) < m_lastEntries.size() ) 
		{
			m_entryHistoryCursorPos += (steps * stepUnit);
			setEntry( m_lastEntries[ m_lastEntries.size() - 1 - wantedEntryIdx  ] );
		}
	}

}