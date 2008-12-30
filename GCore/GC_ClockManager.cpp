#include <boost/pool/object_pool.hpp>
#include "GC_ClockManager.h"
#include "GC_Clock.h"

namespace gcore
{

	/** Constructor.
		@param timeReference Provide time used as reference to update all Clocks.
	*/
	ClockManager::ClockManager(const TimeReferenceProvider& timeReference, size_t reserveClockCount )
		: m_timeReference(timeReference)
		, m_deltaTime(0)
		, m_lastUpdateTime(0)
		, m_clockPool( new ClockPool( reserveClockCount ) )
	{

	}

	/** Destructor.
	*/
	ClockManager::~ClockManager()
	{
		if(!m_clockList.empty())
			destroyAllClocks();

		delete m_clockPool;
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
			GC_EXCEPTION("Tried to create a Clock with a name already registered!");
		}

		//create the clock
		Clock* clock = m_clockPool->construct(name);

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
		GC_ASSERT( m_clockPool->is_from( clock ) , String( "Tried to destroy a clock that was not created by this manager! Clock name : ") + clock->getName() );

		for(std::vector<Clock*>::iterator it= m_clockList.begin(); it != m_clockList.end(); ++it)
		{
			if( (*it) == clock )
			{
				//Found!
				//unregister
				m_clockList.erase(it);
				m_clockIndex.erase( m_clockIndex.find(clock->getName()) );
				
				//destroy
				m_clockPool->destroy(clock);

				return;
			}
		}

	}


	/** Destroy all Clocks created by this ClockManager.
		This will make all pointers to those Clock objects invalid!
	*/
	void ClockManager::destroyAllClocks()
	{
		//delete all clocks
		for(std::vector<Clock*>::iterator it = m_clockList.begin(); it != m_clockList.end(); ++it)
		{
			m_clockPool->destroy(*it);
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
		
		std::map< const String, Clock* >::iterator it = m_clockIndex.find(name);
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
		m_deltaTime = timeValue - m_lastUpdateTime ;
		//update clocks
		for (std::vector<Clock*>::iterator it = m_clockList.begin(); it != m_clockList.end(); ++it )
		{
			(*it)->update(m_deltaTime);
						
		}
		//save the last update time
		m_lastUpdateTime = timeValue;
	}

}