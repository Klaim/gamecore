#include "GC_Clock.h"

#include <cmath>

namespace gcore
{


	/** Construtor.
	@param name Clock's name.
	*/
	Clock::Clock(const String& name, ClockManager& clockManager)
		: m_clockManager( clockManager )
		, m_name(name)
		, m_timeFlowFactor(1.0)
		, m_time(0)
		, m_deltaTime(0)
		, m_max_deltaTime(0)
	{

	}

	/** Destructor.
	*/
	Clock::~Clock()
	{

	}

	/** Clock Update (by ClockManager)
	@param	deltaTime Delta time value (time passed since last udpate, in seconds).
	*/
	void Clock::update(TimeValue deltaTime)
	{
		//The factor value let us speed up, slow down, inverse or stop the time flow
		m_deltaTime = m_timeFlowFactor * deltaTime;

		if( m_max_deltaTime > 0)
		{
			if( std::fabs( m_deltaTime ) > m_max_deltaTime )
			{
				if( m_deltaTime >= 0 )
				{
					m_deltaTime = m_max_deltaTime;
				}
				else
				{
					m_deltaTime = -m_max_deltaTime;
				}
			}
		}

		m_time += m_deltaTime;//update time

	}

	void Clock::reset()
	{
		m_time=0;
	}

	void Clock::time( TimeValue time )
	{
		m_time=time;
	}
}