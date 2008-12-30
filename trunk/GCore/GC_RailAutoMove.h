#ifndef GC_RAILAUTOMOVE_H
#define GC_RAILAUTOMOVE_H
#pragma once

#include "GC_Common.h"
#include "GC_RelativityControl_Speed.h"
#include "GC_TrajectoryControl_Path.h"

#if GC_PLATFORM == GC_PLATFORM_WIN32
	#pragma warning( push )
	#pragma warning( disable : 4250 ) // we want some virtual function definition from several base classes
#endif

namespace gcore
{
	/** No documentation yet.
	*/
	template < typename StateType
		, typename RailType
		, typename SpaceUnitType = float 
		, class RelationType = float >
	class RailAutoMove
		: public automove::RelativityControl_Speed< StateType, SpaceUnitType >
		, public automove::TrajectoryControl_Path< StateType, RailType, SpaceUnitType, RelationType >
	{
	public:

		RailAutoMove( Clock* clock = nullptr)
			: RelativityControl_Speed( clock )
			, TrajectoryControl_Path()
		{}

		RailAutoMove( const StateType& state, Clock* clock = nullptr ) 
			: RelativityControl_Speed( state, clock )
			, TrajectoryControl_Path( state )
		{

		}

		void setFixedDuration( const TimeValue& duration )
		{
			 const SpaceUnitType speed = getPath().length() / (duration / 1000) ; // speed is in unit/sec
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