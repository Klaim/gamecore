#ifndef GCORE_CLOCK_H
#define GCORE_CLOCK_H
#pragma once

#include "GC_String.h"

#include "GC_Common.h"
#include "GC_Time.h"
#include "GC_ClockManager.h"

namespace GCore
{



	/** Virtual Clock.
		
		Own a TimeFlowFactor that allow acceleration, slow down, stop
		and inversion of time flow, for this clock only.

		@remark Should be created , destroyed and managed by a ClockManager.
		@see ClockManager
	*/
	class GCORE_API Clock
	{
	
	public:

		/** @return Virtual milliseconds passed since the clock initialization.
		 */
		TimeValue getTime() const{return TimeValue(float(m_time) * 0.0001);}

		/** @return Virtual microseconds passed since the clock initialization.
		 */
		const TimeValue& getPreciseTime() const{return m_time;}

		/** @return Time flow factor.
		 */
		const TimeFactor& getTimeFlowFactor() const {return m_timeFlowFactor;}

		/** Virtual delta time (from the last update and flow factor dependent).
		*/
		const TimeValue& getDeltaTime() const {return m_deltaTime;}

		/** Time flow factor.
			@param	factor New flow factor value.
		 */
		void setTimeFlowFactor(TimeFactor factor){m_timeFlowFactor=factor;}

		
		/** Reset Time to 0 microseconds elapsed.
		 */
		void reset(){m_time=0; }

		/** Virtual milliseconds passed since the clock initialization.
			@param time New value.
		*/
		void setTime(TimeValue time){m_time=(time*1000);}

		/** Virtual microseconds passed since the clock initialization.
			@param time New value.
		*/
		void setPreciseTime(TimeValue time){m_time=time;}

		/** @return Clock's name.
		*/
		const String& getName(){return m_name;}
		
	private:

		/** Clock's name.
		*/
		String m_name;

		/** Virtual microseconds passed since the clock initialization.
		 */
		TimeValue m_time;

		/** Time flow factor.
		 */
		TimeFactor m_timeFlowFactor;

		/** Virtual delta time (from the last update and flow factor dependant).
		*/
		TimeValue m_deltaTime;

		///Managed by ClockManager only.
		friend class ClockManager;

		/** Clock Update (by ClockManager)
			@param	deltaTime Delta time value (time passed since last udpate, in microseconds).
		 */
		void update(TimeValue deltaTime);

		/** Construtor.
			@param name Clock's name.
		*/
		Clock(const String& name);
		
		/** Desstructor.
		*/
		~Clock();
	

	};


}

#endif