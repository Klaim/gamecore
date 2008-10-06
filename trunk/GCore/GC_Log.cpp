
#include <boost/date_time/posix_time/posix_time.hpp>

#include "GC_Log.h"
#include "GC_LogListener.h"

namespace gcore
{
	//add a new message to the log
	void Log::logMessage( const String& message, LogMessageLevel level )
	{
		if( level + m_logLevel >= GC_LOG_THRESHOLD )
		{
			using namespace boost::posix_time;
			using namespace boost::gregorian;
			ptime now = second_clock::local_time(); //use the clock

			String finalMsg("");
			finalMsg += "[" + to_simple_string( now.time_of_day() ) + "] ";

			finalMsg += message;

			//display in console if any:
			std::cout << "[" << m_name << "]" << finalMsg << std::endl; 
			std::cout.flush();

			//write in file :
			m_fileStream << finalMsg << std::endl;
			m_fileStream.flush();

			// notify each listener registered to this log
			if( !m_registeredListeners.empty() ) // be lazy!
			{
				// Note : to manage the case when a listener manipulate our
				// current listener list, we make a copy first of the list and
				// notify the listeners in the copy list.

				const std::vector< LogListener* > notificationList( m_registeredListeners );
				const std::size_t listenerCount = notificationList.size();
				for ( std::size_t i = 0; i < listenerCount; ++ i )
				{
					GC_ASSERT( notificationList[i] != nullptr, String( "Found a null listener registered in log " ) + m_name );
					notificationList[i]->catchLogMessage( *this , message, level );
				}
			}
		}

	}

	//only LogManager should create a log
	Log::Log( const LogManager& logManager, const String& name, LoggingLevel level, bool isNewFile ):
		m_logLevel(level),
		m_logManager(logManager),
		m_name(name)
	{
		if(isNewFile)
		{
			m_fileStream.open(name.c_str(), std::ios_base::trunc);
		}
		else
		{
			m_fileStream.open(name.c_str(), std::ios_base::app);
		}
		//new session : 
		using namespace boost::posix_time;
		using namespace boost::gregorian;
		ptime now = second_clock::local_time(); //use the clock
		m_fileStream << "//////////////////////////////////////////////////////////////";
		m_fileStream << std::endl << "[LOG]["<< to_simple_string(now) <<"]Session start!"<<std::endl;
		m_fileStream.flush();

	}

	Log::~Log()
	{
		m_fileStream.close();
	}

	void Log::registerListener( LogListener* logListener )
	{
		GC_ASSERT( logListener != nullptr, String( "Tried to register a null listener in log " ) + m_name );
		GC_ASSERT( std::find( m_registeredListeners.begin(), m_registeredListeners.end(), logListener) == m_registeredListeners.end(), String( "Tried to register a log listener but already registered in log ") + m_name );

		m_registeredListeners.push_back( logListener );
	}

	void Log::unregisterListener( LogListener* logListener )
	{
		GC_ASSERT( logListener != nullptr, String( "Tried to unregister a null listener in log " ) + m_name );
		//GC_ASSERT( std::find( m_registeredListeners.begin(), m_registeredListeners.end(), logListener) != m_registeredListeners.end(), String( "Tried to unregister a log listener but not registered in log ") + m_name );

		m_registeredListeners.erase( std::remove( m_registeredListeners.begin(), m_registeredListeners.end(), logListener ), m_registeredListeners.end() );

	}
}