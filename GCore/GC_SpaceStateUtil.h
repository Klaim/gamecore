#ifndef GC_SPACEPOSUTIL_H
#define GC_SPACEPOSUTIL_H
#pragma once
#include <cmath>
#include "GC_Common.h"

/** No documentation yet.
*/
template< typename StateType, typename SpaceUnitType = float >
class SpaceStateUtil
{
public:

	StateType normalize(const StateType& coord) const
	{
		GC_FORCE_IMPLEMENTATION;
		return StateType;
	}

	SpaceUnitType distance(const StateType& coordA, const StateType& coordB) const
	{
		GC_FORCE_IMPLEMENTATION;
		return SpaceUnitType;
	}

	StateType move( const StateType& state, const StateType& difference ) const
	{
		GC_FORCE_IMPLEMENTATION;
		return state;
	}

}; 

template<>
class SpaceStateUtil< float, float >
{
public:

	float normalize(const float& coord) const
	{
		return (coord / fabs(coord));
	}

	float distance(const float& coordA, const float& coordB) const
	{
		return fabs(coordA - coordB);
	}

	float move( const float& state, const float& difference ) const
	{
		return state + difference;
	}


}; 

template<>
class SpaceStateUtil< double, double >
{
public:

	double normalize(const double& coord) const
	{
		return (coord / fabs(coord));
	}

	double distance(const double& coordA, const double& coordB) const
	{
		return fabs(coordA - coordB);
	}

	double move( const double& state, const double& difference ) const
	{
		return state + difference;
	}


}; 



#endif