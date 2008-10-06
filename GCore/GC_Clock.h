#ifndef GCORE_CLOCK_H
#define GCORE_CLOCK_H
#pragma once

#include "GC_String.h"

#include "GC_Common.h"
#include "GC_Time.h"
#include "GC_ClockManager.h"

namespace gcore
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
		const TimeValue& getTime() const{ return m_time; }

		/** @return Time flow factor.
		 */
		const TimeFlowFactor& getTimeFlowFactor() const {return m_timeFlowFactor;}

		/** Virtual delta time (from the last update and flow factor dependent).
		*/
		const TimeValue& getDeltaTime() const { return m_deltaTime;}

		/** Time flow factor.
			@param	factor New flow factor value.
		 */
		void setTimeFlowFactor(TimeFlowFactor factor){m_timeFlowFactor=factor;}

		
		/** Reset Time to 0 milliseconds elapsed.
		 */
		void reset(){m_time=0; }

		/** Virtual milliseconds passed since the clock initialization.
			@param time New value.
		*/
		void setTime(TimeValue time){m_time=time;}

		/** @return Clock's name.
		*/
		const String& getName() const {return m_name;}
		
	private:

		/** Clock's name.
		*/
		String m_name;

		/** Virtual milliseconds passed since the clock initialization.
		 */
		TimeValue m_time;

		/** Time flow factor.
		 */
		TimeFlowFactor m_timeFlowFactor;

		/** Virtual delta time (from the last update and flow factor dependant).
		*/
		TimeValue m_deltaTime;

		///Managed by ClockManager only.
		friend class ClockManager;

		/** Clock Update (by ClockManager)
			@param	deltaTime Delta time value (time passed since last udpate, in milliseconds).
		 */
		void update(TimeValue deltaTime);

		/** Construtor.
			@param name Clock's name.
		*/
		Clock(const String& name);
		
		/** Destructor.
		*/
		~Clock();
	

	};


}

#endif