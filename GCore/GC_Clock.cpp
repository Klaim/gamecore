#include "GC_Clock.h"

namespace gcore
{


	/** Construtor.
	@param name Clock's name.
	*/
	Clock::Clock(const String& name): 
		m_name(name),
		m_timeFlowFactor(1.0),
		m_time(0),
		m_deltaTime(0)
	{

	}

	/** Desstructor.
	*/
	Clock::~Clock()
	{

	}

	/** Clock Update (by ClockManager)
	@param	deltaTime Delta time value (time passed since last udpate, in microseconds).
	*/
	void Clock::update(TimeValue deltaTime)
	{
		
		//The factor value let us speed up, slow down, inverse or stop de time flow
		m_deltaTime = static_cast< unsigned long >(m_timeFlowFactor * static_cast<double>(deltaTime));
		m_time += m_deltaTime;//update time

	}

}