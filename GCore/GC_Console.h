
#ifndef GCORE_CONSOLE_H
#define GCORE_CONSOLE_H
#pragma once

#include "GC_String.h"
#include <vector>
#include <map>

#include "GC_Common.h"
#include "GC_ConsoleCommand.h"


namespace gcore	//gcore context
{

	/** Base class for console management system.
		Features :
		- cross-platform;
		- non renderer specific;
		- accept inputs as entries;
		- command with parammetters management;
		- buffer text inputs;
		- non input-system specific;
		- uses UTFString ( have to change to template character type )
		@par
		Using the console :
		@par
		If an entry starts with "/" (default command prefix), it suggest that it is a command.
		The name of the command should be just next "/" : "/ping" for the "ping" command.
		When adding a new command to the console, the name of the command should not contain "/".
		Other command prefix can be added or removed.
		@par
		If the entry start with a command prefix but no command matches, an error message will be printed.
		If the entry do not start with a command prefix, the default command will be executed with the entry as parameter.
		If the entry do not start with a command prefix and the default command is not defined, the entry
			will just be printed in the console.
		
	*/
	class GCORE_API Console
	{

	public:

		// default prefix (L"/"), registered on console construction.
		static const UTFString DEFAULT_PREFIX;
		
		//////////////////////////////////////////////////////////////////////////
		//Input :


		/** Last entries in the console.
		*/
		const std::vector< UTFString >& getLastEntries() const {return m_LastEntries;}

		/** Last texts printed.
		*/
		const std::vector< UTFString >& getLastTexts() const {return m_LastTexts;}

		

		/** Add one or more translated keys in the current entry.
			@param keys Translated keys to add in the current entry.
			@return The number of keys that couldn't be added (if the entry is full for example).
		*/
		virtual long passKeys(const UTFString& keys);

		/** Print the text string (without parsing it).
			@param text Text to print.
		*/
		void printText(const UTFString& text);
		
		/** Current value of the entry.
		*/
		const UTFString& getEntry() const {return m_Entry;}

		/** Set the current text entry value.
			@param entry New entry value.
		*/
		void setEntry(const UTFString& entry);

		/** Entry execution.
			This will store the entry, then if it starts with "/" (by default),
			we parse it to get the parameters of the command and we execute the command.
			Then we reset the entry.
			If it is not a command, we just use the default command.
			If there is no default command, we just display the entry.
		*/
		void executeEntry();

		//////////////////////////////////////////////////////////////////////////
		//ConsoleCommand management :

		/** Add a new command to execute when asked.
			@param name Name of the command. This will be used as identifier of this command for this Console.
			@param command ConsoleCommand object to register.
		*/
		void addCommand(const UTFString& name,ConsoleCommandPtr& command );

		/** Remove a registered command.
			@param name Name of the ConsoleCommand to remove.
		*/
		void removeCommand(const UTFString& name);

		/** Get a registered ConsoleCommand object.
			@param name Name of the ConsoleCommand object to return.
			@return Return a registered command if found. Return nullptr if not.
		*/
		ConsoleCommandPtr getCommand(const UTFString& name);

		/** Set the default command called when the command is not recognized.
			@remark The command must have been registered.
			@param name Name of the registered command to set as default command. 
				Set "" to set the default neutral behavior of the command (just print the entry).
		*/
		void setDefaultCommand(const UTFString& name);

		/** Add a new command prefix.
			@param prefix If an entry starts with a command prefix, it is used as a command.	
		*/
		void addCommandPrefix(const UTFString& prefix);

		/** Remove a command prefix if exists.
			@param prefix Prefix to remove if exists.
		*/
		void removeCommandPrefix(const UTFString& prefix);

		//////////////////////////////////////////////////////////////////////////
		//Configuration :

		///@return Maximum of stored entries
		unsigned int getMaxEntries() const {return m_MaxEntries;}

		///@return Maximum of stored printed texts
		unsigned int getMaxTexts() const {return m_MaxTexts;}

		///@return Max entry length
		unsigned int getMaxEntryLength() const {return m_MaxEntryLength;}

		
		/** Constructor.
			@remark This will add a default prefix : "/" .
			@param maxEntries Maximum of stored entries.
			@param Max entry length.
			@param Maximum of stored printed texts.
		*/
		Console(unsigned long maxEntries = 8, unsigned long maxEntryLength = 256 , unsigned long maxTexts = 32);

		
		
		/**	Destructor.
		*/
		virtual ~Console();
		//////////////////////////////////////////////////////////////////////////


	protected:

		/** Parse the string as a parameter list (separated with " " by default) and fill a vector with the parameters.
			@remark The parameter vector will not be cleaned before filling.
			@param paramString String with all the parameters.
			@param paramList Vector that will receive the result of the parsing : each parameter found.
			@param paramSeparators String that will be used to find the different parameters.
		*/
		void parseParams(const UTFString& paramString, std::vector< UTFString >& paramList,const UTFString& paramSeparators= L" ");

		/** Called each time a text is added to the console, including entry execution result.
			@remark User should define rendering processes in this function.
			@param text Text added to the current texts.
		*/
		virtual void onAddText(const UTFString& text) = 0;
		
		/** Called each time the current entry is modified.
			@remark User should define rendering processes in this function if necessary.
			You can get the current entry value with getEntry
		*/
		virtual void onEntryChanged() { } 

	//private:

		//////////////////////////////////////////////////////////////////////////
		//Commands :

		///Registered commands
		std::map< UTFString , ConsoleCommandPtr > m_CommandList;

		///ConsoleCommand prefix list
		std::vector< UTFString > m_CommandPrefixList;

		//////////////////////////////////////////////////////////////////////////
		//Input :

		///Maximum of stored entries
		const unsigned int m_MaxEntries;

		///Maximum of stored printed texts
		const unsigned int m_MaxTexts;

		///Max entry length
		const unsigned int m_MaxEntryLength;

		///Last entries in the console
		std::vector< UTFString > m_LastEntries;

		///Last texts printed
		std::vector< UTFString > m_LastTexts;

		///Current entry 
		UTFString m_Entry;

		///Default command used when no "/" is present as first char of the text
		ConsoleCommandPtr m_DefaultCommand;

		/** Optimize memory usage.
			@remark Call this method after structure modification.
		*/
		//void optimizeMemory();

		/// parameter list, used on entry parsing
		std::vector<UTFString > m_paramList;

	};

	
	

}

#endif	//GCORE_CONSOLE_H