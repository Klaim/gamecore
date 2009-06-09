
#ifndef GCORE_CONSOLE_H
#define GCORE_CONSOLE_H
#pragma once

#include "GC_String.h"
#include <vector>
#include <map>
#include <boost/circular_buffer.hpp>

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
		- uses LocalizedString ( have to change to template character type )
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

		typedef boost::circular_buffer< LocalizedString > EntryList;
		typedef boost::circular_buffer< LocalizedString > TextList;
		typedef std::vector< LocalizedString > ParameterList;
		typedef std::map< const LocalizedString , ConsoleCommandPtr > CommandIndex;


		// default prefix (L"/"), set on console construction.
		static const LocalizedString DEFAULT_PREFIX;
		
		//////////////////////////////////////////////////////////////////////////
		//Input :


		/** @return Last entries in the console.	*/
		const EntryList& getLastEntries() const {return m_lastEntries;}

		/** @return Last texts printed.	*/
		const TextList& getLastTexts() const {return m_lastTexts;}

		/** Add one or more translated keys in the current entry.
			@param keys Translated keys to add in the current entry.
			@return The number of keys that couldn't be added (if the entry is full for example).
		*/
		long injectKeys(const LocalizedString& keys);

		/** Remove a number of keys in the entry starting at the cursor position. 
			
			@param keyCount Count of keys to remove.
			@param onTheLeft True if the removed keys are on the left of the cursor, false if they are on the right.
			
			@return The count of keys really removed.
		*/
		unsigned long removeEntryKeys( unsigned long keyCount, bool onTheLeft = true );

		/** @return Current position of the cursor in the entry. */
		unsigned long getCursorPosition() const { return m_cursorPos; }

		/** Set the current position of the cursor in the entry. 
			@param cursorPos New position of the cursor in the entry.
			@return True if the cursor have been moved correctly, false if the given position is out of bound.
				In this case the cursor will be set to the end of the entry.
		*/
		bool setCursorPosition( unsigned long cursorPos );

		/** Move the cursor in the entry text.
			
			@param steps Step count (characters) the cursor will have to go through.
			@param toTheRight True if the cursor will move to the right, false if it have to go to the left.
			
			@return The real count of character steps the cursor gone through.
		*/
		unsigned long moveCursor( unsigned long steps = 1, bool toTheright = true );

		/** Move the cursor to the end of the entry. */
		void moveCursorToEnd();

		/** Move the cursor to the beginning of the entry. */
		void moveCursorToBegin();

		void moveEntryHistoryCursor( unsigned long steps = 1, bool toPast = true );

		/** Print the text string (without parsing it).
			@param text Text to print.
		*/
		void printText(const LocalizedString& text);

		
		/** Current value of the entry.	*/
		const LocalizedString& getEntry() const {return m_entry;}

		/** Set the current text entry value.
			@param entry New entry value.
		*/
		void setEntry(const LocalizedString& entry);

		/** Entry execution.
			This will store the entry, then if it starts with "/" (by default),
			we parse it to get the parameters of the command and we execute the command.
			Then we reset the entry.
			If it is not a command, we just use the default command.
			If there is no default command, we just display the entry.
		*/
		void executeEntry();

		/** Shortcut to execute directly an entry.		*/
		void execute( const LocalizedString& entry )
		{
			setEntry( entry );
			executeEntry();
		}

		/** Add a new command to execute when asked.
			@param command ConsoleCommand object to register.
		*/
		void addCommand( const ConsoleCommandPtr& command );

		/** Remove a registered command.
			@param name Name of the ConsoleCommand to remove.
		*/
		void removeCommand(const LocalizedString& name);

		/** Get a registered ConsoleCommand object.
			@param name Name of the ConsoleCommand object to return.
			@return Return a registered command if found. Return null if not.
		*/
		ConsoleCommandPtr getCommand(const LocalizedString& name);

		const ConsoleCommandPtr& getActiveCommand() const { return m_activeCommand; }

		/** Set the default command called when the command is not recognized.
			@remark The command must have been registered.
			@param name Name of the registered command to set as default command. 
				Set "" to set the default neutral behavior of the command (just print the entry).
		*/
		void setDefaultCommand(const LocalizedString& name);

		/** Set a prefix used to identify command call.
			The prefix have to be used at the start of the entry to detect the command call.
			@param commandCallPrefix Prefix to set to identify command call. Once trimmed, it must not be empty!
		*/
		void setCommandCallPrefix(const LocalizedString& commandCallPrefix);
 
		/** Current prefix used to identify a command call.
		*/
		const LocalizedString& getCommandCallPrefix() const { return m_commandCallPrefix; }
		
		//////////////////////////////////////////////////////////////////////////
		//Configuration :

		/// @return Maximum of stored entries
		unsigned int getMaxEntries() const {return m_maxEntries;}

		/// @return Maximum of stored printed texts
		unsigned int getMaxTexts() const {return m_maxTexts;}

		/// @return Max entry length
		unsigned int getMaxEntryLength() const {return m_maxEntryLength;}

		/// @return True if the console will print an executed commmand.
		bool willPrintCommandOnExecute() const { return m_printCommandOnExecute; }
		void setPrintCommandOnExecute( bool printCommandOnExecute ){ m_printCommandOnExecute = printCommandOnExecute; }

		
		/** Constructor.
			@param  This will add a default prefix. @see DEFAULT_PREFIX
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
		void parseParameters(const LocalizedString& paramString, std::vector< LocalizedString >& paramList,const LocalizedString& paramSeparators= L" ");

		/** Called each time a text is added to the console, including entry execution result.
			@remark User should define rendering processes in this function.
			@param text Text added to the current texts.
		*/
		virtual void onAddText(const LocalizedString& text) = 0;
		
		/** Called each time the current entry is modified.
			@remark User should define rendering processes in this function if necessary.
			You can get the current entry value with getEntry
		*/
		virtual void onEntryChanged() { } 


		/** Called before current entry will be printed. */
		virtual void onEntryPrint() {}

		/** Called each time the cursor have been moved ( including when entry changed ). */
		virtual void onCursorMoved() {}
		
	private:

		//////////////////////////////////////////////////////////////////////////
		// Commands :

		friend class ConsoleCmd_Help; // command extending 

		/// Registered commands
		CommandIndex m_commandIndex;

		/// Command that is currently active.
		ConsoleCommandPtr m_activeCommand;

		/// ConsoleCommand call prefix.
		LocalizedString m_commandCallPrefix;

		//////////////////////////////////////////////////////////////////////////
		//Input :

		/// Maximum of stored entries
		const unsigned long m_maxEntries;

		/// Maximum of stored printed texts
		const unsigned long m_maxTexts;

		/// Max entry length
		const unsigned long m_maxEntryLength;

		/// Last entries in the console
		EntryList m_lastEntries;

		/// Last texts printed
		TextList m_lastTexts;

		/// Current entry 
		LocalizedString m_entry;

		/// Current cursor position in the entry.
		unsigned long m_cursorPos;

		/// Entry history cursor.
		unsigned long m_entryHistoryCursorPos;

		/// True if the console will print the entry before executing it.
		bool m_printCommandOnExecute;

		/// Default command used when no "/" is present as first char of the text
		ConsoleCommandPtr m_defaultCommand;

		/// parameter list, used on entry parsing
		ParameterList m_paramList;
		
		/// @return A command name called in the provided entry or an empty string if no command call found.
		LocalizedString extractCommandCall( const LocalizedString& entry ) const;
		
		/// Execute the active command.
		void executeActiveCommand();

		/// Print the current entry.
		void printEntry();

		/// Add text to the entry
		void insertEntryText( const LocalizedString& text );

	};

	
	

}

#endif	//GCORE_CONSOLE_H