#ifndef GC_TARGETINTERPOL_H
#define GC_TARGETINTERPOL_H
#pragma once

#include "GC_Common.h"
#include "GC_SpaceStateUtil.h"
#include "GC_Interpolator.h"

namespace gcore
{
	/** No documentation yet.
	*/
	template< typename StateType
		, typename SpaceUnitType = float 
		>
	class TargetInterpol : public virtual Interpolator< StateType, SpaceUnitType >
	{
	public:

		/** Constructor.
		*/
		TargetInterpol()
			: m_isRepulsion( false )
			, m_range( 0 )
		{
		}
	
		/** Destructor.
		*/
		virtual ~TargetInterpol(){}

		
		void reset()
		{ 

		}

		bool isFinished() const
		{
			GC_ASSERT( m_range >= 0, "Contact range have to be positive!");
			const SpaceStateUtil< StateType, SpaceUnitType > posUtil;
			const SpaceUnitType distance = posUtil.distance( m_state, m_targetState );
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
			const StateType relationVector = m_targetState - state;
			StateType direction = posUtil.normalize( relationVector );
			SpaceUnitType finalDistance( distanceToTravel );

			const SpaceUnitType distance = posUtil.distance( state, m_targetState );
			GC_ASSERT( distance >= 0 , "Distance have to be positive!");
			if( !m_isRepulsion )
			{
				if( m_range > distance )
				{
					// we are already in range, we don't have to move
					return;
				}
				
				// check that we don't go farther than the target point or it's range
				const SpaceUnitType maxDistance = distance - m_range;
				if( finalDistance > maxDistance )
				{
					finalDistance = maxDistance;
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

			// deduce the move from direction and distance
			const StateType finalMove = direction * finalDistance;

			// apply the move
			state = posUtil.move( state , finalMove);
		}
		
	private:

		StateType m_targetState;

		SpaceUnitType m_range;

		bool m_isRepulsion;
	
	};
	

}

#endif