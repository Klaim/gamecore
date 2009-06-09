#ifndef GC_TIMER_H
#define GC_TIMER_H
#pragma once

#include <algorithm>
#include <vector>
#include <functional>

#include "GC_Common.h"
#include "GC_Time.h"
#include "GC_TimerManager.h"


namespace gcore
{

	class Timer;
	class Clock;

	/** Listener called when a timer it is registered in trigger.
		@see Timer
	*/
	class GCORE_API TimerListener
	{
	public:
		TimerListener(){}
		virtual ~TimerListener(){}
		virtual void onTimerTrigger( Timer& timer ) = 0;
	};

	typedef std::tr1::function< void ( Timer& ) > TimerListenerFunction;

	class ProxyTimerListener : public TimerListener
	{
	public:
		ProxyTimerListener( const TimerListenerFunction& timerListenerFunction  )
			: m_timerListenerFunction( timerListenerFunction )
		{ }

		inline void onTimerTrigger( Timer& timer )
		{
			m_timerListenerFunction( timer );
		}

	private:
		
		TimerListenerFunction m_timerListenerFunction;

	};


	/** Trigger listeners when a specified amount of time passed.
		Managed by TimerManager.
		@see TimerManager

	*/
	class GCORE_API Timer
	{
	public:

		/** Register a listener to be called on trigger.
		*/
		void registerListener( TimerListener* timerListener );

		/** Unregister a registered listener.
		*/
		void unregisterListener( TimerListener* timerListener );

		/** Unregister all registered listeners.
		*/
		void unregisterAllListeners();


		/** Verify passed time since last update and trigger listeners if
			the specified time passed. 
			@remark Call this method one time by clock update.
		*/
		void update();

		/** Reset the time counting data.
		*/
		void reset();

		/// Count how many time this timer triggered.
		unsigned long getTriggerCount() const { return m_triggerCount; }


		/// Time span to wait before trigger.
		TimeValue getWaitTime() const { return m_waitTime; }

		/** Time span to wait before trigger.
			@remark 0 Wait time will deactivate this timer.
		*/
		void setWaitTime( const TimeValue& waitTime ){ m_waitTime = waitTime; }

		/** Name of this timer or empty string if no name set on creation.
		*/
		const String& getName() const { return m_name; }

		/** True if this timer have to trigger only one time and then do nothing.
		*/
		bool isTriggerOnce() const { return m_triggerOnce; }

		/** True if this timer have to trigger only one time and then do nothing.
		*/
		void setTriggerOnce( bool triggerOnce = true ) { m_triggerOnce = triggerOnce; }

	protected:
		
	private:

		/// Name of this timer.
		const String m_name;

		/// Count how many time this timer triggered.
		unsigned long m_triggerCount;

		/// Time since the last time we triggered.
		TimeValue m_timeSinceLastTrigger;

		/// Time span to wait before trigger.
		TimeValue m_waitTime;

		/// True if this timer have to trigger only one time and then do nothing.
		bool m_triggerOnce;

		/// Clock used as a time reference provider.
		const Clock& m_clock;

		/// Listeners registered to this timer.
		std::vector< TimerListener* > m_timerListenerList;
		bool m_timerListenerListChanged;
		std::vector< TimerListener* > m_triggerList;

		friend class TimerManager;
		friend typedef TimerManager::TimerPool;

		/** Constructor.
		@param clock Clock used as a time reference.
		*/
		Timer( const String& name, const Clock& clock);

		/** Destructor.
		*/
		~Timer();

	
	};
	
	

}

#endif