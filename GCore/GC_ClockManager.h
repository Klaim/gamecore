#ifndef GCORE_CLOCKMANAGER_H
#define GCORE_CLOCKMANAGER_H
#pragma once

#include <map>
#include "GC_String.h"
#include <vector>

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

		
		/** @return Time value of the last update, from TimeReferenceProvider (in milliseconds).
		*/
		TimeValue getLastUpdateTime(){return m_lastUpdateTime;}

		/** @return Time elapsed between the last update and the previous one (in milliseconds).
		*/
		TimeValue getDeltaTime(){return m_deltaTime;}

		/** Return the registered Clock list.
		*/
		const std::vector<Clock*>& getClockList() const {return m_clockList;}

		/** Return the named index of all named clocks registered.
		*/
		const std::map< const String , Clock*>& getNamedClocksIndex() const {return m_clockIndex;}


		/** Constructor.
			@param timeReference Provide time used as reference to update all Clocks.
		*/
		ClockManager(const TimeReferenceProvider& timeReference);

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


		/** Time value of the last update, from TimeReferenceProvider (in milliseconds).
		*/
		TimeValue m_lastUpdateTime;

		/** Time elapsed between the last update and the previous one (in milliseconds).
		*/
		TimeValue m_deltaTime;

	};

}

#endif