#ifndef GCORE_CLOCKMANAGER_H
#define GCORE_CLOCKMANAGER_H
#pragma once

#include <map>
#include <vector>
#include "GC_String.h"

#include "GC_Common.h"
#include "GC_TimeReferenceProvider.h"

namespace gcore
{
	class Clock;
	
	/** Create, destroy and manage Clock objects.
		@remark To keep all the managed clocks updated, frequent calls of
		updateClocks should be done.
	*/
	class GCORE_API ClockManager
	{
	public:

		/** Create a Clock object.
			The name of the Clock must be unique for this ClockManager,
			if not an exception will occurs.
			@param name Name given to the Clock.
			@return A pointer to the new Clock object
		*/
		Clock* createClock(const String& name);

		/** Destroy a Clock created by this ClockManager.
			If the Clock object was not created by this ClockManager,
			an exception occurs.
			@param clock Pointer to the Clock to destroy. 
		*/
		void destroyClock(Clock* clock);

		/** Destroy all Clocks created by this ClockManager.
			This will make all pointers to those Clock objects invalid!
		*/
		void destroyAllClocks();

		/** Get a Clock by it's name.
			@param name Clock's name (given at it's creation).
			@return A pointer to the Clock or nullptr if not found.
		*/
		Clock* getClock(const String& name);

		/** Update all Clock time.
			@remark This should be called as frequently as possible, 
			each main loop cycle at best.
		*/
		void updateClocks();


		/** Reset all clocks.
		*/
		void reset();

		
		/** @return Time value of the last update, from TimeReferenceProvider (in seconds).
		*/
		TimeValue getLastUpdateTime() const {return m_lastUpdateTime;}

		/** @return Time elapsed between the last update and the previous one (in seconds).
		*/
		TimeValue getDeltaTime() const {return m_deltaTime;}

		/** @return Maximum time elapsed or 0 or negative value if no limit set (default).                                                                     
		*/
		TimeValue getMaxDeltaTime() const { return m_max_deltaTime; }

		/** Set a maximum limit to the possible delta time or 0 for no limit (default).
		*/
		void setMaxDeltaTime( TimeValue maxDeltaTime )
		{
			GC_ASSERT( maxDeltaTime >= 0, "Max delta time have to be 0 or positive!");
			m_max_deltaTime = maxDeltaTime;
		}

		/** Return the registered Clock list.
		*/
		const std::vector<Clock*>& getClockList() const {return m_clockList;}

		/** Return the named index of all named clocks registered.
		*/
		const std::map< const String , Clock*>& getNamedClocksIndex() const {return m_clockIndex;}


		/** Constructor.
			@param timeReference Provide time used as reference to update all Clocks.
		*/
		ClockManager(const TimeReferenceProvider& timeReference,  size_t reserveClockCount = 32 );

		/** Destructor.
		*/
		~ClockManager();



	protected:

	private:

		/** Provide time used as reference to update all Clocks.
		*/
		const TimeReferenceProvider& m_timeReference;

		/** Named index of all the Clocks.
		*/
		std::map< const String, Clock* > m_clockIndex;

		/** List of all Clocks created by this ClockManager.
		*/
		std::vector< Clock* > m_clockList;

		/// Time value of the last update, from TimeReferenceProvider (in seconds).
		TimeValue m_lastUpdateTime;

		/// Time elapsed between the last update and the previous one (in seconds).
		TimeValue m_deltaTime;

		/// Maximum time elapsed allowed, or 0 or negative value if no limit set.
		TimeValue m_max_deltaTime;

	};

}

#endif