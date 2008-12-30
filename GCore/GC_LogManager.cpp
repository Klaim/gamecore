#include "GC_LogManager.h"
#include <algorithm>

namespace gcore
{
	
	const String LogManager::DEFAULT_LOG( "lastSession.log" );

	void LogManager::addLogListener( LogListener* logListener, const String& logName )
	{
		GC_ASSERT( logListener != nullptr, String( "Tried to add a null listener to log " ) + logName );

		if( logName != "" )
		{
			LogMap::iterator logIt = m_logList.find( logName );
			if( logIt != m_logList.end() )
			{
				GC_ASSERT( logIt->second != nullptr, "Found a null log in log manager!" );
				logIt->second->registerListener( logListener );
			}
#ifdef GC_DEBUG
			else
			{
				GC_EXCEPTION( String( "Tried to add a listener in a log not found in this log manager! Log searched : " ) + logName );
			}
#endif
		}
		else
		{
			// register in all the logs
			LogMap::iterator logIt = m_logList.begin();
			for( ; logIt != m_logList.end(); ++logIt )
			{
				GC_ASSERT( logIt->second != nullptr, "Found a null log in log manager!" );
				logIt->second->registerListener( logListener ); 
			}
		}

		

	}

	//Remove a registered LogListener
	void LogManager::removeLogListener( LogListener* logListener, const String& logName )
	{
		GC_ASSERT( logListener != nullptr, String( "Tried to remove a null log listener from log " ) + logName );

		if( logName != "" )
		{
			LogMap::iterator logIt = m_logList.find( logName );
			if( logIt != m_logList.end() )
			{
				GC_ASSERT( logIt->second != nullptr, "Found a null log in log manager!" );
				logIt->second->unregisterListener( logListener );
			}
#ifdef GC_DEBUG
			else
			{
				GC_EXCEPTION( String( "Tried to unregister a listener from a log not found in this log manager! Log searched : " ) + logName );
			}
#endif
		}
		else
		{
			// unregister from all logs
			LogMap::iterator logIt = m_logList.begin();
			for( ; logIt != m_logList.end(); ++logIt )
			{
				GC_ASSERT( logIt->second != nullptr, "Found a null log in log manager!" );
				logIt->second->unregisterListener( logListener ); 
			}
		}

	}

	//Create a new Log
	Log* LogManager::createLog(const String& name,LoggingLevel level,bool isNewFile)
	{

		if(m_logList.find(name)!=m_logList.end())
		{
			//tried to create a log already created!!!
			GC_EXCEPTION("Tried to create a log already created!!!");
		}

		Log* log = new Log( *this, name, level, isNewFile );
		m_logList[name]=log;

		return log;

	}


	void LogManager::destroyLog( const String& name )
	{
		LogMap::iterator it = m_logList.find( name );
		GC_ASSERT( it != m_logList.end(), String( "Tried to destroy a log not created in the log manager! Log name : ") + name );
		delete it->second;
		m_logList.erase( it );
	}


	//Retrieves a Log by name if exists, return nullptr if it don't exits
	Log* LogManager::getLog(const String& logName)
	{
		LogMap::iterator logIt = m_logList.find(logName);
		if(logIt != m_logList.end())
		{
			return logIt->second;
		}

		return nullptr;
	}

	//Write a message to the Log
	void LogManager::logMessage( const String& logName, const String& message, LogMessageLevel level )
	{
		//Find the logIt to let it handle the message
		LogMap::iterator logIt = m_logList.find( logName );
		if( logIt != m_logList.end() )
		{
			GC_ASSERT( logIt->second != nullptr, "Found a null log in log manager!" );
			logIt->second->logMessage( message, level );
		}
#ifdef GC_DEBUG
		else
		{
			GC_EXCEPTION( String( "Logged a message in a log not found in this log manager! Log searched : " ) + logName );
		}
#endif
	}

	void LogManager::logMessage( const String& message, LogMessageLevel level )
	{
		m_defaultLog->logMessage( message, level );
	}


	/** Sets the passed in log as the default log.
	@returns The previous default log.
	*/
	Log* LogManager::setDefaultLog( Log* log )
	{
		GC_ASSERT( log != nullptr, "Tried to set a null default log!" );
		GC_ASSERT( m_logList.find( log->getName() ) != m_logList.end(), "Tried to set a default log not registered in the log manager!" );

		Log* oldlog=m_defaultLog;
		m_defaultLog=log;

		return oldlog;
	}

	LogManager::LogManager(const String& defaultLogName)
	{
		//We create a system logIt that is set as the default logIt : 
#ifdef GC_DEBUG
		m_defaultLog = createLog(defaultLogName,LL_BOREME,true);
#else
		m_defaultLog = createLog(defaultLogName,LL_NORMAL,true);
#endif
		m_defaultLog->logMessage("LogManager initialized.");

	}

	LogManager::~LogManager()
	{
		m_defaultLog->logMessage("Terminate LogManager.");
		// Destroy all logs
		LogMap::iterator it;
		for (it = m_logList.begin(); it != m_logList.end(); ++it)
		{
			GC_ASSERT( it->second != nullptr, "Found a null log in log manager!" );
			delete it->second;
		}
	}
}