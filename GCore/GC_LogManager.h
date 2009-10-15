#ifndef GCORE_LOGMANAGER_H
#define GCORE_LOGMANAGER_H
#pragma once

#include <unordered_map>
#include <vector>
#include "GC_Common.h"	//Use gcore System common defines
#include "GC_Singleton.h"
#include "GC_String.h"
#include "GC_Log.h"
#include "GC_LogListener.h"


namespace gcore
{
	/** Manage logs.			

		With a LogManager you can create new Logs, register/unregister LogListener.
		Every registered LogListener are notified when a Log logs a message into a file.
		@par
		When deleted, every log created by the LogManager are deleted (and the opened files for logging messages are closed
		LogListener are not deleted.
			
	*/
	class GCORE_API LogManager 
	{
	public:

		/// Default log file if not defined : 
		static const String DEFAULT_LOG;

		/** Create a LogManager, when created the m_logList and m_LogCatcherPool are empty.
			@param defaultLogName Name of the starting default log.
		**/
		explicit LogManager( const String& defaultLogName = DEFAULT_LOG );

		/** Destructor.
		**/ 	
		~LogManager();

		/** Register a LogListener to the LogManager, 
			when registered the LogListener will be notified of logMessage events.
			@param logCatcher	The LogListener to register.
			@param logName		Name of the log to listen to. If empty (default value), the listener will
			be registered for all the currently created logs.
		**/
		void addLogListener( LogListener* logListener, const String& logName = "" );

		/** Unregister a LogListener of the LogManager, 
			when unregistered the LogListener will no longer be notified of logMessage events.
			@param logCatcher	The LogListener to unregister.
			@param logName		Name of the log to listen to. If empty (default value), the listener will
			be unregistered from all the logs it is currently registered in.
		**/
		void removeLogListener( LogListener* logListener, const String& logName = "" );

		/** Create a new Log with the desired name.
			When a log is created by a LogManager,
			the LogManager handles the notifying of LogListener when the logMessage of the Log is called through the LogManager.
			THe Log is deleted when the LogManager is deleted.
			@param name The name of the Log (also the name of the file in which the Log will write).
			@param isNewFile True for erasing the log file if it already exists, else append the messages at the end of the existing file.
			@return The created Log
		**/
		Log* createLog( const String& name, bool isNewFile = true );

		/** TODO : add some comments here!   
		*/
		void destroyLog( const String& name );

		/** Return a log previously created by the LogManager.
			@param name The name of the log to get.
			@return The Log which name is passed has an argument, or nullptr if the log does not exist.
		**/
		Log* getLog( const String& name );

		/** Make a Log write a message, and notify every registered LogListener.
			@param logName The name of the log which has to write message, if no log with this name exist, nothing happens.
			@param message The message to add into the file of the Log, each message is succeeded by a new line.
		**/
		void logMessage( const String& logname, const String& message );

		/** Write a message to the default Log, and notify every registered LogListener.
			@param logName The name of the log which has to write message, if no log with this name exist, nothing happens.
			@param message The message to add into the file of the Log, each message is succeeded by a new line.
		**/
		void logMessage( const String& message );

		/** Returns a pointer to the default log.
		*/
		Log* getDefaultLog() { return m_defaultLog; }

		/** Sets the passed in log as the default log.
		@returns The previous default log.
		*/
		Log* setDefaultLog( Log* log );

	
	private:

		typedef std::tr1::unordered_map< String, Log* > LogIndex;

		/// Index of all a logs created.
		LogIndex m_logList;

		/// Default Log : 
		Log* m_defaultLog;
		
	};


}

#endif