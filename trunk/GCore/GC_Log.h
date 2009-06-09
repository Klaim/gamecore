/******************************************************************

	Inspired by Ogre::Log; www.ogre3D.org.

*******************************************************************/

#ifndef GCORE_LOG_H
#define GCORE_LOG_H
#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "GC_Common.h"
#include "GC_String.h"

namespace gcore
{

	class LogManager;
	class LogListener;

	/// LogMessageLevel + LoggingLevel > NRS_LOG_THRESHOLD = message logged
	#define GC_LOG_THRESHOLD 4

	/** The level of detail to which the log will go into.
	*/
	enum LoggingLevel
	{
		LL_LOW = 1,
		LL_NORMAL = 2,
		LL_BOREME = 3
	};

	/** The importance of a logged message.
	*/
	enum LogMessageLevel
	{
		LML_TRIVIAL = 1,
		LML_NORMAL = 2,
		LML_CRITICAL = 3
	};

	/** TODO : rewrite this comment!
		The Log class act has a log file writer.
		Its instanced and used by using a LogManager.
		@par	
		When created a Log is binded to a file (the file name if the name of the log),
		and messages can be written at anytime into this file (messages are concatenated with a new line separation between them).
		@par
		In order to add a message into the binded file, the logMessage methods has to be called.
		When the logMessage method is called thought the logMessage of the LogManager that created this Log,
		The catchLogMessage method of every registered  LogListener of the LogManager is called.
		However, if the logMessage of a Log is called directly, the message is written directly into the file, and LogListener are not notified.
		@remark Managed by LogManager.
	*/
	class GCORE_API Log
	{
	public :

		/** Write a message into the file binded to the Log.
			@param message The message to add into the file, each message is succeeded by a new line.
			@param level The importance of the message.
		**/
		void logMessage( const String& message, LogMessageLevel level = LML_NORMAL );

		/// Sets the level of the log detail.
		void setLogDetail(LoggingLevel logLevel){ m_logLevel = logLevel; }

		/** The name of the log, which is also the name of the file the log writes into.
		**/
		const String& getName() const {return m_name;}

		void registerListener( LogListener* logListener );
		void unregisterListener( LogListener* logListener );
		
	private:

		/// Only LogManager should create Logs.
		friend class LogManager;

		/// The importance level of the Log, a higher log level will log messages with lower importance.
		LoggingLevel	m_logLevel;

		/// The stream writing into the file.
		std::ofstream	m_fileStream;

		/// The name of the log, which is also the name of the file the log writes into.
		const String	m_name;

		/// LogManager that manage this Log.
		const LogManager& m_logManager;

		/// List of log listeners registered to listen this log
		std::vector< LogListener* >	m_registeredListeners;

		/** Create a log with the desired name.
			@param logManager Log manager that manage this Log.
			@param name The name of the Log, which is the name of the file in which the Log writes data.
			@param isNewFile True for erasing the log file if it already exists, else append the messages at the end of the existing file.
			@param level The level of the log.
		**/
		Log( const LogManager& logManager,const String& name, LoggingLevel level,bool isNewFile = true);

		/** The log destructor close the file in which data are written.
		**/
		~Log();
		
					
	};

	
}

#endif