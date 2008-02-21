#ifndef GCORE_LOGMANAGER_H
#define GCORE_LOGMANAGER_H
#pragma once

#include "GC_String.h"
#include <map>
#include <vector>
#include "GC_Common.h"	//Use gcore System common defines
#include "GC_Singleton.h"

#include "GC_Log.h"
#include "GC_LogListener.h"

///Default log file : 
#define GC_LOG_FILE "lastSession.log"

namespace gcore
{
	
	/** Interface class for using logs.			

		With a LogManager you can create new Logs, register/unregister LogListener.
		Every registered LogListener are notified when a Log logs a message into a file.
		\par
		When deleted, every log created by the LogManager are deleted (and the opened files for logging messages are closed
		LogListener are not deleted.
			
	*/
	class GCORE_API LogManager 
	{
		friend class Log;
	
	public:

		/**
		*Create a LogManager, when created the m_LogList and m_LogCatcherPool are empty.
		*@param defaultLogName Name of the starting default log.
		**/
		LogManager(const String& defaultLogName);

		/**
		*Destructor.
		**/ 	
		~LogManager();

		/**
		*Register a LogListener to the LogManager, 
		*when registered the LogListener will be notified of logMessage events.
		*@param logCatcher The LogListener to register.
		**/
		void addLogListener(LogListener* logListener);

		/**
		*Unregister a LogListener of the LogManager, 
		*when unregistered the LogListener will no longer be notified of logMessage events.
		*@param logCatcher The log catcher to unregister
		**/
		void removeLogListener(LogListener* logListener);

		/**
		*Create a new Log with the desired name.
		*When a log is created by a LogManager,
		*the LogManager handles the notifying of LogListener when the logMessage of the Log is called through the LogManager.
		*THe Log is deleted when the LogManager is deleted.
		*TODO for the creation of two logs with the same name, or whe the log can't be created (files problems??)
		*@param name The name of the Log (also the name of the file in which the Log will write).
		*@param level The importance of the message.
		*@param isNewFile True for erasing the log file if it already exists, else append the messages at the end of the existing file.
		*@return The created Log
		**/
		Log* createLog(const String& name, LoggingLevel level=LL_NORMAL,bool isNewFile = true);

		/**
		*Return a log previously created by the LogManager.
		*@param name The name of the log to get.
		*@return The Log which name is passed has an argument, or nullptr if the log does not exist.
		**/
		Log* getLog(const String& name);

		/**
		*Make a Log write a message, and notify every registered LogListener.
		*@param logName The name of the log which has to write message, if no log with this name exist, nothing happens.
		*@param message The message to add into the file of the Log, each message is succeeded by a new line.
		*@param level The importance of the message.
		**/
		void logMessage(const String& logname, const String& message, LogMessageLevel level=LML_NORMAL);

		/**
		*Write a message to the default Log, and notify every registered LogListener.
		*@param logName The name of the log which has to write message, if no log with this name exist, nothing happens.
		*@param message The message to add into the file of the Log, each message is succeeded by a new line.
		*@param level The importance of the message.
		**/
		void logMessage(const String& message, LogMessageLevel level=LML_NORMAL);

		/** Returns a pointer to the default log.
		*/
		Log* getDefaultLog();

		/** Sets the passed in log as the default log.
		@returns The previous default log.
		*/
		Log* setDefaultLog(Log* log);

		
	
	private:

		/**
		*This map contains every Log that the LogManager created.
		**/
		std::map< String, Log* > m_LogList;


		///Default Log : 
		Log* m_DefaultLog;

		/**
		*This vector contains every LogListener that are register to the LogManager.
		**/
		std::vector< LogListener*  > m_LogCatcherPool;

		/**
		*Call the catchLogMessage of every Log created by the 
		*@param logName The name of log that log the message.
		*@param message The message to log.
		*@param level The importance of the message.
		**/
		void dispatchMessage(const String& logName, const String& message, LogMessageLevel level);


	};


}

#endif