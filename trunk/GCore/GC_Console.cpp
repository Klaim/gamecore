#include <boost/tokenizer.hpp>
#include "GC_Console.h"

namespace gcore
{

	
	const UTFString Console::DEFAULT_PREFIX( L"/" );

	Console::Console(unsigned long maxEntries , unsigned long maxEntryLength  , unsigned long maxTexts )
		: m_Entry(  )
		, m_MaxEntries( maxEntries )
		, m_MaxEntryLength(maxEntryLength)
		, m_MaxTexts(maxTexts)
		, m_LastEntries( maxEntries )
		, m_LastTexts( maxTexts )
	{
		m_DefaultCommand.reset();
		addCommandPrefix( DEFAULT_PREFIX );

		//optimizeMemory();
		m_CommandPrefixList.reserve( 8 ); // this is arbitrary
		m_paramList.resize( 8 ); // this is arbitrary
		for( int i = 0; i < 8 ; ++i )
		{
			m_paramList[i].reserve(m_MaxEntryLength);
		}

		// memory optimization
		m_Entry.reserve( maxEntryLength );
		
		
		for( size_t i = 0; i < m_MaxEntries ; ++i )
		{
			m_LastEntries[i].reserve( m_MaxEntryLength );
		}
		for( size_t i = 0; i < m_MaxTexts ; ++i )
		{
			m_LastTexts[i].reserve( m_MaxEntryLength );
		}
	}

	
	
	Console::~Console()
	{

	}


	void Console::parseParams(const UTFString& paramString, std::vector< UTFString >& paramList,const UTFString& paramSeparators)
	{
		/*using namespace boost;

		tokenizer<char_delimiters_separator<UTF_CHAR>> tok(paramString);
		for(tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg)
		{
		paramList.push_back((*beg));
		}*/
		//clear param list first!
		/*for( size_t k=0; k < paramList.size(); ++k)
		{
			paramList[k] = L"";
		}
		*/
		paramList.clear();

		typedef boost::tokenizer< boost::char_separator<UTF_CHAR>, UTFString::const_iterator,	UTFString > my_tok;

		boost::char_separator<UTF_CHAR> sep( paramSeparators.c_str() );
		my_tok tok( paramString, sep );

		//unsigned int i = 0;
		for ( my_tok::iterator it = tok.begin();it != tok.end();++it )
		{
			/*if(i>=paramList.size())
			{
				//memory optimization
				paramList[i] = L"";
				for( unsigned int k=i; k < paramList.size(); ++k)
				{
					paramList[k].reserve(m_MaxEntryLength);
				}
			}
			paramList[i] = (*it) ;
			*/
			paramList.push_back( (*it) );
		};

	}
	
	long Console::passKeys(const UTFString& keys)
	{
		if(keys.empty())return 0;


		long result = 0;

		long l = long(keys.length() + m_Entry.length()) - m_MaxEntryLength;

		if(l>0)// we don't take all the characters
		{
			m_Entry += keys.substr(0, m_MaxEntryLength - (keys.length() + m_Entry.length()) );	

			result = l;
		}
		else m_Entry += keys;

		onEntryChanged();

		return result;
	}

	void Console::printText(const UTFString& text)
	{
		if( text.empty() ) // do nothing if empty
			return;

		UTFString textToPrint( text );

		if( textToPrint.length() > m_MaxEntryLength ) textToPrint = textToPrint.substr(0,m_MaxEntryLength);

		if( m_LastTexts.size() + 1 > m_MaxTexts )
			m_LastTexts.erase( m_LastTexts.begin() );

		m_LastTexts.push_back(textToPrint);
		
		onAddText(textToPrint);
	}
	

	void Console::executeEntry()
	{
		//do nothing if the entry is empty :
		if(m_Entry.empty())return;

		//store the entry and move the older entry if not enough place:
		if( m_LastEntries.size() + 1 > m_MaxEntries )
			m_LastEntries.erase( m_LastEntries.begin() );

		m_LastEntries.push_back(m_Entry);

		//is it a command?
		bool isCommand = false;
		unsigned int idPrefix ;
		for(idPrefix = 0; idPrefix< m_CommandPrefixList.size(); ++idPrefix)
		{
			if( m_Entry.substr( 0, m_CommandPrefixList[idPrefix].length() ) == m_CommandPrefixList[idPrefix] )
			{
				isCommand = true;
				break;
			}
		}

		if(isCommand)
		{
			//////////////////////////////////////////////////////////////////////////
			//execute command :

			//get the command
			size_t prefixLength = m_CommandPrefixList[idPrefix].length();
			UTFString commandName = m_Entry.substr(prefixLength,m_Entry.find_first_of(L" ")-prefixLength);

			if(commandName.empty())return;//do nothing if no command

			//check that the command is registered : 
			std::map< UTFString, ConsoleCommandPtr >::iterator it;
			it = m_CommandList.find(commandName);

			if(it!=m_CommandList.end())
			{
				//command found!
				//print it!
				printText(m_Entry);
				//get the parameters : 
				UTFString params = m_Entry.substr(commandName.length()+prefixLength);
				
				parseParams(params, m_paramList,it->second->m_paramSeparators);
				//execute the command :
				it->second->execute((*this),m_paramList);

			}
			else
			{
				//command not found!
				printText( L"'" + commandName + L"' command unknown.");

			}

		}else
		{
			//execute default command with the entry as parameter if defined:
			if(m_DefaultCommand)
			{
				
				parseParams(m_Entry, m_paramList, m_DefaultCommand->m_paramSeparators);

				m_DefaultCommand->execute((*this),m_paramList);

			}
			else
			{
				//no default command : we just print the entry
				printText(m_Entry);
			}
		}

		//reset the entry : 
		m_Entry.clear();
		onEntryChanged();

	}

	void Console::addCommand(const UTFString& name,ConsoleCommandPtr& command )
	{
		GC_ASSERT( !name.empty(), "Tried to add an unnamed ConsoleCommand!" );
		GC_ASSERT( &command != nullptr && command.get() != nullptr , "Tried to add a null ConsoleCommand!");
		m_CommandList[name]= command;
	}

	void Console::removeCommand(const UTFString& name)
	{
		GC_ASSERT( !name.empty(), "Tried to remove an unnamed ConsoleCommand!" );

		std::map<UTFString, ConsoleCommandPtr >::iterator it;
		it=m_CommandList.find(name);
		if(it!=m_CommandList.end())
		{
			m_CommandList.erase(it);
			return;
		}
		
		// command not found!
	}

	ConsoleCommandPtr Console::getCommand(const UTFString& name)
	{
		GC_ASSERT( !name.empty(), "Tried to get an unnamed ConsoleCommand!" );

		std::map<UTFString, ConsoleCommandPtr >::iterator it;
		it = m_CommandList.find(name);

		if(it!=m_CommandList.end())
		{
			return it->second;
		}

		return ConsoleCommandPtr();
	}

	void Console::setDefaultCommand(const UTFString& name)
	{
		//just set nullptr if the name is empty
		if(name.empty())
		{
			m_DefaultCommand.reset();
			return;
		}

		std::map<UTFString, ConsoleCommandPtr >::iterator it;
		it=m_CommandList.find(name);
		if(it!=m_CommandList.end())
		{
			m_DefaultCommand = it->second;
		}
	}

	void Console::addCommandPrefix(const UTFString& prefix)
	{
		GC_ASSERT( !prefix.empty(), "Tried to add an empty command prefix!" );
		m_CommandPrefixList.push_back(prefix);
	}

	void Console::removeCommandPrefix(const UTFString& prefix)
	{
		GC_ASSERT( !prefix.empty(), "Tried to remove an empty command prefix!" );

		std::vector< UTFString >::iterator it;
		it = std::find(m_CommandPrefixList.begin(),m_CommandPrefixList.end(),prefix);
		if(it != m_CommandPrefixList.end())
		{
			m_CommandPrefixList.erase(it);
		}

	}

	void Console::setEntry( const UTFString& entry )
	{
		m_Entry = entry;
		if(m_Entry.length()>m_MaxEntryLength)m_Entry = m_Entry.substr(0,m_MaxEntryLength);
		onEntryChanged();
	}

/*
	void Console::setMaxEntries(unsigned int val)
	{
		m_MaxEntries = val;

		if(m_LastEntries.size() < val) m_LastEntries.resize(val);

		optimizeMemory();

	}

	void Console::setMaxTexts(unsigned int val)
	{
		m_MaxTexts = val;

		if(m_LastTexts.size() < val) m_LastTexts.resize(val);

		optimizeMemory();
	}

	void Console::setMaxEntryLength(unsigned int val)
	{
		m_MaxEntryLength = val;

		optimizeMemory();
	}

	
	void Console::optimizeMemory()
	{
		// cut the entry if too long
		if(m_Entry.length()>m_MaxEntryLength)m_Entry = m_Entry.substr(0,m_MaxEntryLength);

		// reserve entry memory
		m_Entry.reserve( sizeof(UTF_CHAR) * m_MaxEntryLength );

		// reserve memory for last entries 
		m_LastEntries.reserve( m_MaxEntries );

		// reserve memory for last texts
		m_LastTexts.reserve( m_MaxTexts );

	}
*/
}