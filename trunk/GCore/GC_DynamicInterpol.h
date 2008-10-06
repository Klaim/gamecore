#ifndef GC_DYNAMICINTERPOL_H
#define GC_DYNAMICINTERPOL_H
#pragma once

#include "GC_Common.h"
#include "GC_SpeedInterpol.h"
#include "GC_TargetInterpol.h"

#if GC_PLATFORM == GC_PLATFORM_WIN32
#pragma warning( push )
#pragma warning( disable : 4250 ) // we want some virtual function definition from several base classes
#endif

namespace gcore
{
	/** No documentation yet.
	*/
	template 
		< typename StateType
		, typename SpaceUnitType = float 
		>
	class DynamicInterpol
		: public SpeedInterpol< StateType, SpaceUnitType >
		, public TargetInterpol< StateType, SpaceUnitType >
	{
	public:

		void reset()
		{
			SpeedInterpol::reset();
			TargetInterpol::reset();
		}

		void setFixedDuration( const TimeValue& duration )
		{
			const SpaceStateUtil< StateType, SpaceUnitType > posUtil;
			const SpaceUnitType speed = posUtil.distance( m_state, getTargetState() ) / (duration / 1000) ; // speed is in unit/sec
			setSpeed( speed );
			// make sure no acceleration variation is set to keep the duration right
			setAccelerationFunction( &NoAcceleration );		 
		}


	protected:
		
	private:
	
	};
	

}

#if GC_PLATFORM == GC_PLATFORM_WIN32
#pragma warning( pop )
#endif

#endif