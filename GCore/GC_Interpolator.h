#ifndef GC_INTERPOLATOR_H
#define GC_INTERPOLATOR_H
#pragma once


#include "GC_Common.h"

namespace gcore
{
	
	/** Automatic interpolation.
	*/
	template < typename StateType , typename SpaceUnitType = float >
	class Interpolator 
	{
	public:

		/** Constructor with zero-initialization.   
		*/
		Interpolator(): m_state() { }
		
		/** Constructor with defined initialization.   
		*/
		Interpolator( const StateType& initialState )
			: m_state( initialState )
		{
		}

		virtual ~Interpolator()	{ }
	

		void update( StateType& state )
		{
			const SpaceUnitType distanceToTravel = calculateDistanceToTravel();
			GC_ASSERT( distanceToTravel >= 0, "Distances have to be positive!" );
			updateState( state, distanceToTravel );
			m_state = state; 
		}

		const StateType& update()
		{
			const SpaceUnitType distanceToTravel = calculateDistanceToTravel();
			GC_ASSERT( distanceToTravel >= 0, "Distances have to be positive!" );
			updateState( m_state , distanceToTravel );
			return m_state;
		}

		virtual bool isFinished() const = 0;

		const StateType& getState() const { return m_state; }
		void setState( const StateType& newPosition ){ m_state = newPosition; }

	protected:

		virtual SpaceUnitType calculateDistanceToTravel() = 0;
		virtual void updateState( StateType& state , const SpaceUnitType& distanceToTravel ) = 0;

		
		StateType			m_state;	
	
	};


	

}

#endif
