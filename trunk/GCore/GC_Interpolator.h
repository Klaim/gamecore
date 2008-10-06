#ifndef GC_INTERPOLATOR_H
#define GC_INTERPOLATOR_H
#pragma once


#include "GC_Common.h"

namespace gcore
{
	
	/** TODO : UPDATE THIS COMMENT
		Base class for Interpolators.
		An Interpolator will update a variable value through time or other provided variation.
		The values of the variable updated is defined by a provided curve.
		@remark For time based interpolation, see TimeInterpol class.
		@see TimeInterpol @see Curve

		@param StateType Type of the positions to process. Multiplication, addition and subtraction need to be accessible
			and behave as if the operation is done independently on each "member" of the state type.
		@param RelationType Floating type used to get a state between the begin point and the end point of the curve (or outer).
	*/
	template < typename StateType , typename SpaceUnitType = float >
	class Interpolator 
	{
	public:

		Interpolator(){ }
		
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

		virtual void reset() = 0;
		virtual bool isFinished() const = 0;

		const StateType& getState() const { return m_state; }
		void setState( const StateType& newPosition ){ m_state = newPosition; }

	protected:

		virtual SpaceUnitType calculateDistanceToTravel() = 0;
		virtual void updateState( StateType& state , const SpaceUnitType& distanceToTravel ) = 0;

		/// Variable to update while interpolating.
		StateType			m_state;	
	
	};


	

}

#endif
