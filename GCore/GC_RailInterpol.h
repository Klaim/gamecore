#ifndef GC_RAILINTERPOL_H
#define GC_RAILINTERPOL_H
#pragma once

#include "GC_Common.h"
#include "GC_SpeedInterpol.h"
#include "GC_PathInterpol.h"

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
	class RailInterpol
		: public SpeedInterpol< StateType, SpaceUnitType >
		, public PathInterpol< StateType, RailType, SpaceUnitType, RelationType >
	{
	public:

		void reset()
		{
			SpeedInterpol::reset();
			PathInterpol::reset();
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