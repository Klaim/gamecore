#include "GC_LogManager.h"
#include <algorithm>

namespace gcore
{
	void LogManager::dispatchMessage(const String& logName, const String& message, LogMessageLevel level)
	{
		//Send the message to all registered LogCatchers
		for(	std::vector< LogListener*  >::iterator iter = this->m_LogCatcherPool.begin();
			iter != this->m_LogCatcherPool.end();
			iter++)
		{
			(*iter)->catchLogMessage(logName, message, level);
		}
	}

	//Register a LogListener 
	void LogManager::addLogListener(LogListener* logListener)
	{
		if(!logListener)
		{
			//!!! tried to add a nullptr LogListener!
			GC_EXCEPTION("Tried to add a nullptr LogListener!");
		}
		if(logListener->isRegistered())
		{
			//!!! tried to add an already registered LogListener!
			GC_EXCEPTION("Tried to add an already registered LogListener!");
		}

		logListener->m_Registered=true;
		m_LogCatcherPool.push_back(logListener);

	}

	//Remove a registered LogListener
	void LogManager::removeLogListener(LogListener* logListener)
	{
		if(!logListener)
		{
			//!!! tried to remove a nullptr LogListener!
			GC_EXCEPTION("Tried to remove a nullptr LogListener!");
		}

		if(!logListener->isRegistered())
		{
			//!!! tried to remove an unregistered LogListener!
			GC_EXCEPTION("Tried to remove an unregistered LogListener!");
		}


		std::vector< LogListener*  >::iterator it = std::find(m_LogCatcherPool.begin(),m_LogCatcherPool.end(),logListener);
		m_LogCatcherPool.erase(it);


	}

	//Create a new Log
	Log* LogManager::createLog(const String& name,LoggingLevel level,bool isNewFile)
	{

		if(m_LogList.find(name)!=m_LogList.end())
		{
			//tried to create a log already created!!!
			GC_EXCEPTION("Tried to create a log already created!!!");

		}

		Log* log = new Log(this,name, level, isNewFile);

		m_LogList[name]=log;

		return log;

	}

	//Retrieves a Log by name if exists, return nullptr if it don't exits
	Log* LogManager::getLog(const String& logName)
	{
		std::map<String, Log* >::iterator log = m_LogList.find(logName);
		if(log != m_LogList.end())
		{
			return log->second;
		}

		return nullptr;
	}

	//Write a message to the Log
	void LogManager::logMessage(const String& logName, const String& message, LogMessageLevel level)
	{
		//Find the log to let it handle the message
		std::map<String, Log* >::iterator log = m_LogList.find(logName);
		if(log != m_LogList.end())
		{
			log->second->logMessage(message, level);
		}
	}

	void LogManager::logMessage(const String& message, LogMessageLevel level)
	{
		m_DefaultLog->logMessage(message,level);
	}


	/** Returns a pointer to the default log.
	*/
	Log* LogManager::getDefaultLog()
	{
		return m_DefaultLog;
	}

	/** Sets the passed in log as the default log.
	@returns The previous default log.
	*/
	Log* LogManager::setDefaultLog(Log* log)
	{
		Log* oldlog=m_DefaultLog;
		m_DefaultLog=log;
		return oldlog;
	}

	LogManager::LogManager(const String& defaultLogName)
	{
		//We create a system log that is set as the default log : 
#ifdef _DEBUG
		m_DefaultLog = createLog(defaultLogName,LL_BOREME,true);
#else
		m_DefaultLog = createLog(defaultLogName,LL_NORMAL,true);
#endif
		m_DefaultLog->logMessage("LogManager initialized.");

	}

	LogManager::~LogManager()
	{
		m_DefaultLog->logMessage("Terminate LogManager.");
		// Destroy all logs
		std::map<String, Log* >::iterator i;
		for (i = m_LogList.begin(); i != m_LogList.end(); ++i)
		{
			delete i->second;
		}
	}

}