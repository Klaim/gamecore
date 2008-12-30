#ifndef GC_RELATIVITYCONTROL_SPEED_H
#define GC_RELATIVITYCONTROL_SPEED_H
#pragma once

#include <functional>

#include "GC_Common.h"
#include "GC_RelativityControl_TimeBased.h"

namespace gcore
{
namespace automove
{
	/** No documentation yet.
	*/
	template< typename StateType , typename SpaceUnitType >
	class RelativityControl_Speed 
		: virtual public AutoMove< StateType, SpaceUnitType >
		, public RelativityControl_TimeBased< StateType , SpaceUnitType >
	{
	public:

		typedef std::tr1::function< SpaceUnitType ( const TimeValue&, const RelativityControl_Speed& ) > AccelerationFunction;

		static inline SpaceUnitType NoAcceleration( const TimeValue&, const RelativityControl_Speed& ){ return SpaceUnitType(); }
		
		RelativityControl_Speed( Clock* clock )
			: AutoMove()
			, RelativityControl_TimeBased( clock )
			, m_accelerationFunction( &RelativityControl_Speed::NoAcceleration )
			, m_speed(0)
			, m_maxSpeed(0)
			, m_minSpeed(0)
			, m_acceleration(0)
			, m_timePassedSinceStart(0)
		{
		}

		RelativityControl_Speed(const StateType& state, Clock* clock )
			: AutoMove( state )
			, RelativityControl_TimeBased( clock )
			, m_accelerationFunction( &RelativityControl_Speed::NoAcceleration )
			, m_speed(0)
			, m_maxSpeed(0)
			, m_minSpeed(0)
			, m_acceleration(0)
			, m_timePassedSinceStart(0)
		{
		}


		/** Destructor.
		*/
		~RelativityControl_Speed()
		{}

		const AccelerationFunction& getAccelerationFunction() const { return m_accelerationFunction; }
		void setAccelerationFunction( const AccelerationFunction& accelerationFunction ){ m_accelerationFunction = accelerationFunction; }

		const SpaceUnitType& getSpeed() const { return m_speed; }
		void setSpeed( const SpaceUnitType& speed )
		{
			//GC_ASSERT( maxSpeed >= 0, "Tried to set a negative max speed!" );
			m_speed = speed;
		}

		const SpaceUnitType& getAcceleration() const { return m_acceleration; }

		const SpaceUnitType& getMaxSpeed() const { return m_maxSpeed; }
		void setMaxSpeed( const SpaceUnitType& maxSpeed )
		{
			GC_ASSERT( maxSpeed >= 0, "Tried to set a negative max speed!" );
			m_maxSpeed = maxSpeed;
		}

		const SpaceUnitType& getMinSpeed() const { return m_minSpeed; }
		void setMinSpeed( const SpaceUnitType& minSpeed )
		{
			GC_ASSERT( minSpeed >= 0, "Tried to set a negative min speed!" );
			m_minSpeed = minSpeed;
		}

	protected:

		SpaceUnitType calculateDistanceToTravel()
		{
			GC_ASSERT( m_maxSpeed == 0 || m_maxSpeed > m_minSpeed, "Invalid max/min speed in SpeedInterpolator!");
			GC_ASSERT( m_maxSpeed >= 0, "Max speed negative!" );
			GC_ASSERT( m_minSpeed >= 0, "Min speed negative!" );

			GC_ASSERT( getClock() != nullptr , "No clock set before usage!" );

			const TimeValue& deltaTime = getClock()->getDeltaTime();
			m_timePassedSinceStart += deltaTime;
			const TimeValue secondsPassed ( deltaTime / 1000 );
			const TimeValue secondsPassedSinceStart( m_timePassedSinceStart / 1000 );

			const SpaceUnitType currentAcceleration( m_accelerationFunction( secondsPassedSinceStart, *this ) );

			SpaceUnitType currentSpeed( m_speed + currentAcceleration );

			if ( currentSpeed < m_minSpeed )
			{
				currentSpeed = m_minSpeed;
			}
			else if( m_maxSpeed > 0 && currentSpeed > m_maxSpeed )
			{
				currentSpeed = m_maxSpeed;
			}

			const SpaceUnitType distanceToTravel( m_speed * secondsPassed );

			m_speed = currentSpeed;
			m_acceleration = currentAcceleration;

			return distanceToTravel;
		}

	private:
	
		AccelerationFunction			m_accelerationFunction;
		SpaceUnitType					m_speed;
		SpaceUnitType					m_acceleration;
		SpaceUnitType					m_maxSpeed;
		SpaceUnitType					m_minSpeed;

		TimeValue						m_timePassedSinceStart;
	};
}
}

#endif