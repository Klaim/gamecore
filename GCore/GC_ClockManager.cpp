#include "GC_ClockManager.h"
#include "GC_Clock.h"

namespace gcore
{

	/** Constructor.
	@param timeReference Provide time used as reference to update all Clocks.
	*/
	ClockManager::ClockManager(const TimeReferenceProvider& timeReference)
		: m_timeReference(timeReference),
		m_deltaTime(0),
		m_lastUpdateTime(0)
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
			GC_EXCEPTION("Tried to create a Clock with a name already registered!");
		}

		//create the clock
		Clock* clock = new Clock(name);

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
		for(std::list<Clock*>::iterator it= m_clockList.begin(); it != m_clockList.end(); ++it)
		{
			if( (*it) == clock )
			{
				//Found!
				//unregister
				m_clockList.erase(it);
				m_clockIndex.erase( m_clockIndex.find(clock->getName()) );
				
				//destroy
				delete clock;

				return;
			}
		}

		
		//Name already registered
		GC_EXCEPTION("Tried to create a Clock with a name already registered!");

	}


	/** Destroy all Clocks created by this ClockManager.
		This will make all pointers to those Clock objects invalid!
	*/
	void ClockManager::destroyAllClocks()
	{
		//delete all clocks
		for(std::list<Clock*>::iterator it = m_clockList.begin(); it != m_clockList.end(); ++it)
		{
			delete (*it);
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
		
		std::map< String, Clock* >::iterator it = m_clockIndex.find(name);
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
		unsigned long timeValue = m_timeReference.getTimeSinceStart();
		m_deltaTime = timeValue - m_lastUpdateTime ;
		//update clocks
		for (std::list<Clock*>::iterator it = m_clockList.begin(); it != m_clockList.end(); ++it )
		{
			(*it)->update(m_deltaTime);
						
		}
		//save the last update time
		m_lastUpdateTime = timeValue;
	}

}