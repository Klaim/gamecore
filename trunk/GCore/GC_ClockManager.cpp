#include "GC_ClockManager.h"

#include <algorithm>

#include "GC_Clock.h"

namespace gcore
{

	/** Constructor.
		@param timeReference Provide time used as reference to update all Clocks.
	*/
	ClockManager::ClockManager(const TimeReferenceProvider& timeReference, size_t reserveClockCount )
		: m_timeReference(timeReference)
		, m_deltaTime( 0 )
		, m_lastUpdateTime( timeReference.getTimeSinceStart() )
		, m_max_deltaTime( 0 )
	{
		
	}

	/** Destructor.
	*/
	ClockManager::~ClockManager()
	{
		if(!m_clockList.empty())
			destroyAllClocks();
	}


	/** Create a Clock object.
		The name of the Clock must be unique for this ClockManager,
		if not an exception will occurs. 
		@param name Name given to the Clock object.
		@return A pointer to the new Clock object.
	*/
	Clock* ClockManager::createClock(const String& name)
	{
		if( m_clockIndex.find(name)!=m_clockIndex.end())
		{
			//Name already registered
			GC_EXCEPTION << "Tried to create a Clock with a name already registered!";
		}

		//create the clock
		Clock* clock = new Clock(name, (*this) );

		//register it's name if necessary
		if(name != "")
			m_clockIndex[name] = clock;

		//register the clock for future updates
		m_clockList.push_back(clock);

		return clock;
	}

	/** Destroy a Clock created by this ClockManager.
		If the Clock object was not created by this ClockManager,
		an exception occurs.
		@param clock Pointer to the Clock to destroy. 
	*/
	void ClockManager::destroyClock(Clock* clock)
	{
		GC_ASSERT( clock != nullptr, "Tried to destroy a null clock!" );
		GC_ASSERT( std::count( m_clockList.begin(), m_clockList.end(), clock ) == 1 , String( "Tried to destroy a clock that was not created by this manager! Clock name : ") + clock->name() );

		for(ClockList::iterator it= m_clockList.begin(); it != m_clockList.end(); ++it)
		{
			Clock* registeredClock = (*it);
			GC_ASSERT_NOT_NULL( registeredClock );

			if( registeredClock == clock )
			{
				//Found!
				//unregister
				m_clockList.erase(it);
				m_clockIndex.erase( clock->name() );
				
				//destroy
				delete clock;

				return;
			}
		}

		GC_ASSERT( false, "Clock not found! - Previous checks failed!" );
 
	}


	/** Destroy all Clocks created by this ClockManager.
		This will make all pointers to those Clock objects invalid!
	*/
	void ClockManager::destroyAllClocks()
	{
		//delete all clocks
		for(ClockList::iterator it = m_clockList.begin(); it != m_clockList.end(); ++it)
		{
			Clock* clock = *it;
			GC_ASSERT( clock != nullptr, "Found a null clock in the clock list!" );
			delete clock;
		}

		//unregister all clocks
		m_clockList.clear();
		m_clockIndex.clear();

	}

	/** Get a Clock by it's name.
	@param name Clock's name (given at it's creation).
	@return A pointer to the Clock or nullptr if not found.
	*/
	Clock* ClockManager::getClock(const String& name)
	{
		Clock* result = nullptr;
		
		ClockIndex::iterator it = m_clockIndex.find(name);
		if( it != m_clockIndex.end() )
			result = it->second;
		
		return result;
	}

	/** Update all Clock time.
		@remark This should be called as frequently as possible, 
		each main loop cycle at best.
	*/
	void ClockManager::updateClocks()
	{
		//get the new delta
		TimeValue timeValue = m_timeReference.getTimeSinceStart();
		GC_ASSERT( timeValue >= 0, "Time since start cannot be negative!" );

		m_deltaTime = timeValue - m_lastUpdateTime ;
		GC_ASSERT( m_deltaTime >= 0, "Delta time cannot be negative!" );
		
		if( m_max_deltaTime > 0 )
		{
			m_deltaTime = std::min< TimeValue >( m_deltaTime, m_max_deltaTime ); // take the lowest delta time 
		}

		//update clocks
		for (ClockList::iterator it = m_clockList.begin(); it != m_clockList.end(); ++it )
		{
			Clock* clock = (*it);
			GC_ASSERT_NOT_NULL( clock );

			clock->update(m_deltaTime);
						
		}
		//save the last update time
		m_lastUpdateTime = timeValue;
	}

	void ClockManager::reset()
	{
		for (ClockList::iterator it = m_clockList.begin(); it != m_clockList.end(); ++it )
		{
			Clock* clock = (*it);
			GC_ASSERT_NOT_NULL( clock );

			clock->reset();
		}

		m_lastUpdateTime = m_timeReference.getTimeSinceStart();
		m_deltaTime = 0;

	}
}