#ifndef GC_DYNAMICAUTOMOVE_H
#define GC_DYNAMICAUTOMOVE_H
#pragma once

#include "GC_Common.h"
#include "GC_RelativityControl_Speed.h"
#include "GC_TrajectoryControl_Target.h"

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
	class DynamicInterpolator
		: public interpolation::RelativityControl_Speed< StateType, SpaceUnitType >
		, public interpolation::TrajectoryControl_Target< StateType, SpaceUnitType >
	{
	public:

		DynamicInterpolator( Clock* clock = nullptr)
			: RelativityControl_Speed( clock )
			, TrajectoryControl_Target()
		{ }

		DynamicInterpolator( const StateType& state, Clock* clock = nullptr) 
			: RelativityControl_Speed( state, clock )
			, TrajectoryControl_Target( state )
		{

		}

		void setFixedDuration( const TimeValue& duration )
		{
			const SpaceStateUtil< StateType, SpaceUnitType > posUtil;
			const SpaceUnitType speed = posUtil.delta( m_state, getTargetState() ) / (duration / 1000) ; // speed is in unit/sec
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