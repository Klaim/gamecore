#ifndef GC_AUTOMOVE_H
#define GC_AUTOMOVE_H
#pragma once


#include "GC_Common.h"

namespace gcore
{
	
	/** 
	*/
	template < typename StateType , typename SpaceUnitType = float >
	class AutoMove 
	{
	public:

		/** Constructor with zero-initialization.   
		*/
		AutoMove(): m_state() { }
		
		/** Constructor with defined initialization.   
		*/
		AutoMove( const StateType& initialState )
			: m_state( initialState )
		{
		}

		virtual ~AutoMove()	{ }
	

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
