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

	SpaceUnitType delta(const StateType& coordA, const StateType& coordB) const
	{
		GC_FORCE_IMPLEMENTATION;
		return SpaceUnitType;
	}

	StateType deltaState(const StateType& coordA, const StateType& coordB) const
	{
		GC_FORCE_IMPLEMENTATION;
		return StateType;
	}


	StateType move( const StateType& state, const StateType& difference ) const
	{
		GC_FORCE_IMPLEMENTATION;
		return state;
	}

	StateType moveForward( const StateType& state, const SpaceUnitType& difference ) const
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

	float delta(const float& coordA, const float& coordB) const
	{
		return fabs(coordA - coordB);
	}

	float deltaState(const float& coordA, const float& coordB) const
	{
		return fabs(coordA - coordB);
	}

	float moveForward( const float& state, const float& difference ) const
	{
		return state * difference;
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

	double delta(const double& coordA, const double& coordB) const
	{
		return fabs(coordA - coordB);
	}

	double deltaState(const double& coordA, const double& coordB) const
	{
		return fabs(coordA - coordB);
	}

	double move( const double& state, const double& difference ) const
	{
		return state + difference;
	}

	double moveForward( const double& state, const double& difference ) const
	{
		return state * difference;
	}
}; 



#endif