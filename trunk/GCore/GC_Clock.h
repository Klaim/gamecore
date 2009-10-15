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

		/** @return Virtual seconds passed since the clock initialization.
		 */
		const TimeValue& time() const{ return m_time; }

		/** Virtual seconds passed since the clock initialization.
			@param time New value.
		*/
		void time(TimeValue );

		/** @return Time flow factor.
		 */
		const TimeFlowFactor& timeFlowFactor() const {return m_timeFlowFactor;}

		/** Time flow factor.
			@param	factor New flow factor value.
		*/
		void timeFlowFactor(TimeFlowFactor factor){m_timeFlowFactor=factor;}

		/** Virtual delta time (from the last update and flow factor dependent).
		*/
		const TimeValue& deltaTime() const { return m_deltaTime;}

		/** @return Maximum virtual time elapsed or 0 or negative value if no limit set (default).                                                                     
		*/
		TimeValue maxDeltaTime() const { return m_max_deltaTime; }
		
		/** Set a maximum limit to the possible virtual delta time or 0 or negative value for no limit (default).
		*/
		void maxDeltaTime( TimeValue maxDeltaTime )
		{
			GC_ASSERT( maxDeltaTime >= 0, "Max delta time have to be 0 or positive!");
			m_max_deltaTime = maxDeltaTime;
		}
		
		/** Reset Time to 0 seconds elapsed.
		 */
		void reset();

		/** @return Clock's name.
		*/
		const String& name() const {return m_name;}

		/** @copydoc m_clockManager */
		const ClockManager& clockManager() const { return m_clockManager; }
		ClockManager& clockManager() { return m_clockManager; }

	private:
		///Managed by ClockManager only.
		friend class ClockManager;

		/// Clock manager that created, manage and will destroy this Clock.
		ClockManager& m_clockManager;

		/// Clock's name.
		String m_name;

		/// Virtual seconds passed since the clock initialization.
		TimeValue m_time;

		/// Time flow factor.
		TimeFlowFactor m_timeFlowFactor;

		/// Virtual delta time (from the last update and flow factor dependent).
		TimeValue m_deltaTime;

		/// Maximum time elapsed allowed, or 0 or negative value if no limit set.
		TimeValue m_max_deltaTime;


		/** Clock Update (by ClockManager)
			@param	deltaTime Delta time value (time passed since last update, in seconds).
		 */
		void update(TimeValue deltaTime);

		/** Constructor.
			@param name Clock's name.
			@param clockmanager Clock manager that created, manage and will destroy this Clock.
		*/
		Clock(const String& name, ClockManager& clockManager );
		
		/** Destructor.
		*/
		~Clock();
	

	};

	/// Helper function to destroy a clock cleanly
	inline void destroyClock( Clock* clock )
	{ 
		GC_ASSERT( clock != nullptr, "Tried to destroy a null clock!" );
		clock->clockManager().destroyClock(clock);
	}

	inline void destroyClock( Clock& clock )
	{
		destroyClock( &clock );
	}
}

#endif