#ifndef GC_TRAJECTORYCONTROL_TARGET_H
#define GC_TRAJECTORYCONTROL_TARGET_H
#pragma once

#include "GC_Common.h"
#include "GC_SpaceStateUtil.h"
#include "GC_Interpolator.h"

namespace gcore
{
namespace interpolation
{
	/** No documentation yet.
	*/
	template< typename StateType, typename SpaceUnitType >
	class TrajectoryControl_Target : public virtual Interpolator< StateType, SpaceUnitType >
	{
	public:

		/** Constructor.
		*/
		TrajectoryControl_Target()
			: m_isRepulsion( false )
			, m_range( 0 )
		{
		}

		TrajectoryControl_Target( const StateType& state )
			: Interpolator( state )
			, m_isRepulsion( false )
			, m_range( 0 )
		{
		}
	
		/** Destructor.
		*/
		virtual ~TrajectoryControl_Target(){}

		
		void reset()
		{ 

		}

		bool isFinished() const
		{
			GC_ASSERT( m_range >= 0, "Contact range have to be positive!");
			const SpaceStateUtil< StateType, SpaceUnitType > posUtil;
			const SpaceUnitType distance = posUtil.delta( m_state, m_targetState );
			GC_ASSERT( distance >= 0 , "Distance have to be positive!");
			return ( distance <= m_range ) ;
		}

		const StateType& getTargetState() const { return m_targetState; }
		void setTargetState( const StateType& relativePosition )
		{ m_targetState = relativePosition; }

		bool isRepulsion() const { return m_isRepulsion }
		void setRepulsion( bool repulsion = true )
		{ m_isRepulsion= repulsion; }

		const SpaceUnitType& getRange() const { return m_range; }
		void setRange( const SpaceUnitType& range ) 
		{
			GC_ASSERT( range >= 0, "Contact range have to be positive!");
			m_range = range;
		}

	protected:

		void updateState( StateType& state , const SpaceUnitType& distanceToTravel )
		{
			GC_ASSERT( m_range >= 0, "Contact range have to be positive!");

			const SpaceStateUtil< StateType, SpaceUnitType > posUtil;

			// get the direction to go to
			const StateType relationVector = posUtil.deltaState( m_targetState , state);
			StateType direction = posUtil.normalize( relationVector );
			SpaceUnitType finalDistance( distanceToTravel );

			const SpaceUnitType distance = posUtil.delta( state, m_targetState );
			GC_ASSERT( distance >= 0 , "Distance have to be positive!");
			if( !m_isRepulsion )
			{
				if(		distance < m_range			// in range
					||	distance < finalDistance 	// going farther 
				)
				{
					// we are already in range, we only have to set the position
					state = m_targetState;
					return;
				}
								
			}
			else
			{
				// inverse on repulsion
				direction = -direction;

				// on repulsion, range mean maximum distance from the relative point
				if( m_range > 0
				&&	distance > m_range )
				{
					// we are out of range, so no move is needed
					return;
				}
			}

			// deduce the move from direction and delta
			const StateType finalMove = posUtil.moveForward( direction , finalDistance );

			// apply the move
			state = posUtil.move( state , finalMove);
		}
		
	private:

		StateType m_targetState;

		SpaceUnitType m_range;

		bool m_isRepulsion;
	
	};
	
}
}

#endif