
#include <boost/date_time/posix_time/posix_time.hpp>

#include "GC_Log.h"
#include "GC_LogListener.h"
#include "GC_LogManager.h"

namespace gcore
{
	//only LogManager should create a log
	Log::Log( const LogManager& logManager, const String& name, bool isNewFile )
		: m_logManager(logManager)
		, m_name(name)
	{
		const String filepath = logManager.getRootDirectory() + "/" + name;
		if(isNewFile)
		{
			m_fileStream.open(filepath.c_str(), std::ios_base::trunc);
		}
		else
		{
			m_fileStream.open(filepath.c_str(), std::ios_base::app);
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
		GC_ASSERT( logListener != nullptr, "Tried to register a null listener in log " << m_name );
		GC_ASSERT( std::find( m_registeredListeners.begin(), m_registeredListeners.end(), logListener) == m_registeredListeners.end(), "Tried to register a log listener but already registered in log " << m_name );

		m_registeredListeners.push_back( logListener );
	}

	void Log::unregisterListener( LogListener* logListener )
	{
		GC_ASSERT( logListener != nullptr, "Tried to unregister a null listener in log " << m_name );
		//GC_ASSERT( std::find( m_registeredListeners.begin(), m_registeredListeners.end(), logListener) != m_registeredListeners.end(), "Tried to unregister a log listener but not registered in log " << m_name );

		m_registeredListeners.erase( std::remove( m_registeredListeners.begin(), m_registeredListeners.end(), logListener ), m_registeredListeners.end() );

	}

	void Log::logMessage( const String& message )
	{
		logText(); // to be sure we flush the current text if any (when "composing" a message)
		addText( message ); 
		logText(); // really log the message
	}

	void Log::addText( const String& text )
	{
		m_message << text;
	}

	void Log::logText()
	{
		const String& message = m_message.str();

		if( message.empty() ) return; // be lazy

		// be ready for the next message
		m_message.str( "" );

		// now we can log:

		using namespace boost::posix_time;
		using namespace boost::gregorian;
		ptime now = second_clock::local_time(); //use the clock

		std::stringstream finalMsgStream;
		finalMsgStream << "[" << to_simple_string( now.time_of_day() ) << "] " << message;
		const String& finalMsg = finalMsgStream.str();

		//display in console if any:
		std::cerr << "[" << m_name << "]" << finalMsg << std::endl; 
		std::cerr.flush();
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
				GC_ASSERT( notificationList[i] != nullptr, "Found a null listener registered in log " << m_name );
				notificationList[i]->catchLogMessage( *this , message );
				
			}
		}
	}

	LogStreamer operator<<( Log& log, const String& message )
	{
		return LogStreamer( log, message );
	}


}