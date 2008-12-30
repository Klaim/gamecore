#ifndef GC_TRAJECTORYCONTROL_PATH_H
#define GC_TRAJECTORYCONTROL_PATH_H
#pragma once

#include "GC_Common.h"
#include "GC_Curve.h"
#include "GC_AutoMove.h"

namespace gcore
{
namespace automove
{
	/** No documentation yet.
	*/
	template < typename StateType, typename PathType, typename SpaceUnitType , typename RelationType = float >
	class TrajectoryControl_Path : virtual public AutoMove< StateType, SpaceUnitType >
	{
	public:

		//typedef Curve< StateType, SpaceUnitType, RelationType > PathType;

		TrajectoryControl_Path()
			: m_startState( 0 )
			, m_finalState( 1 )
			, m_currentState( 0 )
			, m_lengthState( 1 )
		{
		}

		TrajectoryControl_Path( const StateType& state )
			: AutoMove( state )
			, m_startState( 0 )
			, m_finalState( 1 )
			, m_currentState( 0 )
			, m_lengthState( 1 )
		{
		}

		/** Destructor.
		*/
		virtual ~TrajectoryControl_Path(){}


		bool isFinished() const { return m_currentState >= m_finalState; }
		
		const RelationType& getStartState() const { return m_startState; }
		void setStartState( const RelationType& startState )
		{
			m_startState = startState;
			calculateLengthState();
		}

		const RelationType& getFinalState() const { return m_finalState; }
		void setFinalState( const RelationType& finalState )
		{
			m_finalState = finalState;
			calculateLengthState();
		}

		const RelationType& getCurrentState() const { return m_currentState; }
		const RelationType& getLengthState() const { return m_lengthState; }

		const PathType& getPath() const { return m_path; }
		void setPath( const PathType& path )
		{
			m_path = path;
		}

	protected:

		void calculateLengthState()
		{
			m_lengthState = abs(m_finalState - m_startState);
		}

		void updateState( StateType& position , const SpaceUnitType& distanceToTravel )
		{ 
			const SpaceUnitType totalLength = ( m_path.length() * m_lengthState );
			
			if( totalLength != 0 )
			{
				RelationType distanceState = static_cast< RelationType >( distanceToTravel / totalLength );

				if( distanceState > 1 ) m_currentState = 1;
				else if( distanceState < 0 ) m_currentState = 0;

				m_currentState += distanceState;
			}
			else
			{
				m_currentState = 1;
			}
			
			
			// make sure we don't go out of bounds...
			if( m_finalState > m_startState )
			{
				m_currentState = m_startState + (m_currentState * m_lengthState) ;

				if( m_currentState > m_finalState )
				{	
					m_currentState = m_finalState; 
				}
			}
			else if( m_startState > m_finalState ) //...in all senses...
			{
				m_currentState = m_startState - (m_currentState * m_lengthState) ;

				if( m_currentState < m_finalState )
				{	
					m_currentState = m_finalState; 
				}
			}
			else
			{
				// no difference between start and final states? then do nothing.
				m_currentState = m_finalState;
			}

			position = m_path.calculatePoint( m_currentState );
		}


	private:

		/// Curve describing the values that can take the object to update
		PathType		m_path;		

		/// Start state of this interpolation.
		RelationType	m_startState;

		/// Final state of this interpolation.
		RelationType	m_finalState;

		/// "Length" between the start state and the final state.
		RelationType	m_lengthState;

		/// State on last update
		RelationType	m_currentState;

	
	};
}

}

#endif