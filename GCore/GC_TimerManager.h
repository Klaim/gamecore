#ifndef GC_TIMERMANAGER_H
#define GC_TIMERMANAGER_H
#pragma once

#include <boost/pool/poolfwd.hpp>
#include <vector>
#include <map>

#include "GC_Common.h"

namespace gcore
{
	class Timer;
	class Clock;

	/** Manage creation and destruction of Timers.
		@see Timer
	*/
	class GCORE_API TimerManager
	{
	public:

		/** Constructor.
		*/
		TimerManager();
	
		/** Destructor.
		*/
		~TimerManager();

		/** Create a timer.
			@remark The timer will start to trigger only after the wait time is set.
				@see Timer::setWaitTime
			@param clock Clock used as time reference provider for the timer.
			@param name Facultative name (empty by default).
		*/
		Timer* createTimer( const Clock& clock, const String& name = "" );

		/** Destroy a timer.
			@remark The timer must have been created by this manager!

			@param timer Timer to destroy.
		*/
		void destroyTimer( Timer* timer );

		/** Destroy all timers created by this manager.
		*/
		void destroyAllTimers();

		/** Get a timer by it's name if a name was set on creation (and not empty).
		*/
		Timer* getTimer( const String& name );

		/** Return the registered Timer list.
		*/
		const std::vector<Timer*>& getTimerList() const {return m_timerList;}

		/** Return the named index of all named timers registered.
		*/
		const std::map< const String , Timer*>& getNamedTimersIndex() const {return m_namedTimersIndex;}


		/** Update all created timers.
			Call this method one time by clock update to keep the timers in sync.
		*/
		void updateTimers();

	protected:
		
	private:

		/// Pool of timer.
		boost::object_pool< Timer >* m_timerPool;

		/// Created timers list.
		std::vector< Timer* > m_timerList;

		/// Named timer index.
		std::map< const String , Timer* > m_namedTimersIndex;
	
	};
	

}

#endif
