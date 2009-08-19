/******************************************************************

	Inspired by Ogre::Log; www.ogre3D.org.

*******************************************************************/
#ifndef GCORE_LOGCATCHER_H
#define GCORE_LOGCATCHER_H
#pragma once

#include "GC_String.h"
#include <map>
#include <vector>
#include <functional>
#include "GC_Common.h"	



namespace gcore
{
	class LogManager;

	/**	TODO : review this comment!
		Listener class for logMessage event of the Log class of a LogManager.
			
		This class possess the abstract method catchLogMessage,
		which is called when the method logMessage of a log is called.
		
		An instance of the class is notified of (e.g. have the OnLogMessage being called),
		when it's registered to a LogManager.
		
		So in order to have use this class, you have to:
			-inherit from it and provide your own implementation of OnLogMessage
			-instanciate it
			-register it to a LogManager
		Once those three step are done,
		whenever a Log of the LogManager have its logMessage method called, the OnLogMessge is called.
			
		@remark Managed by LogManager.
	*/

	class GCORE_API LogListener	
	{
	public:
	
		/** TODO : review this comment!
			The methods is called when a the method logMessage of a log is called
			(e.g This method is an event called when a log writes data to a file).
			Inherit and provide an implementation of this method for managing logMessage events.
			@param logName The name of the Log which method logMessage has been called.
			@param message The message the log has to write to the file.
		**/
		virtual void catchLogMessage(Log& log ,const String& message)=0;

		LogListener(){};

		/**
		*The destructor is virtual in order to avoid memory problems.
		**/
		virtual ~LogListener(){};

	private:

	};

	/// Function-like object that can catch log messages.
	typedef std::tr1::function< void ( Log&, const String& ) > LogListenerFunction;

	/** Proxy event listener that only redirect event catches to a provided function-like object.
		note : seems obsolete ... should be replaced by boost::signal
		*/
	class ProxyLogListener : public LogListener
	{
	public:
		ProxyLogListener( const LogListenerFunction& logListenerFunction )
			: m_logListenerFunction( logListenerFunction )
		{}

		inline void catchLogMessage(Log& log ,const String& message)
		{
			m_logListenerFunction( log, message );
		}

	private:

		LogListenerFunction m_logListenerFunction;
	};



}

#endif