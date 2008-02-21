
#include <boost/date_time/posix_time/posix_time.hpp>

#include "GC_Log.h"
#include "GC_LogManager.h"

namespace GCore
{
	//add a new message to the log
	void Log::logMessage(const String& message, LogMessageLevel level)
	{
		if(level + m_LogLevel >= GC_LOG_THRESHOLD)
		{
			using namespace boost::posix_time;
			using namespace boost::gregorian;
			ptime now = second_clock::local_time(); //use the clock

			m_LogManager->dispatchMessage(m_Name, message, level);

			String finalMsg("");
			finalMsg += "[" + to_simple_string(now.time_of_day()) + "] ";

			finalMsg += message;

			//display in console :
			std::cerr << "[" << m_Name << "]" << finalMsg << std::endl; 
			std::cerr.flush();
			//write in file :
			m_FileStream << finalMsg << std::endl;
			m_FileStream.flush();
		}


	}

	// Sets the level of the log detail.
	void Log::setLogDetail(LoggingLevel logLevel)
	{
		this->m_LogLevel = logLevel;
	}


	//only LogManager should create a log
	Log::Log(LogManager* logManager,const String& name, LoggingLevel level, bool isNewFile):
		m_LogLevel(level),
		m_LogManager(logManager),
		m_Name(name)
	{
		if(isNewFile)
		{
			m_FileStream.open(name.c_str(), std::ios_base::trunc);
		}
		else
		{
			m_FileStream.open(name.c_str(), std::ios_base::app);
		}
		//new session : 
		using namespace boost::posix_time;
		using namespace boost::gregorian;
		ptime now = second_clock::local_time(); //use the clock
		m_FileStream << "//////////////////////////////////////////////////////////////";
		m_FileStream << std::endl << "[LOG]["<< to_simple_string(now) <<"]Session start!"<<std::endl;
		m_FileStream.flush();

	}

	Log::~Log()
	{
		m_FileStream.close();
	}


}