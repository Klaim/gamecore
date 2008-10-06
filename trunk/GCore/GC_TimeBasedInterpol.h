#ifndef GC_TIMEINTERPOL_H
#define GC_TIMEINTERPOL_H
#pragma once

#include "GC_Common.h"
#include "GC_Interpolator.h"
#include "GC_Clock.h"

namespace gcore
{
	/** Base class for time base interpolators.
		This interpolator will update the object to update with time flowing. The time
		flow is totally dependent on the provided clock.
		@see Interpolator.
	*/
	template < typename StateType , typename SpaceUnitType = float >
	class TimeBasedInterpol : virtual public Interpolator< StateType , SpaceUnitType >
	{
	public:

		/** Constructor.
			@see Interpolator::Interpolator
			@param clock		Clock used as time reference.
		*/
		TimeBasedInterpol( const StateType& initialState, Clock* clock = nullptr )
			: Interpolator( initialState )
			, m_clock( clock )
		{			
		}
	
		TimeBasedInterpol( Clock* clock = nullptr )
			: Interpolator()
			, m_clock( clock )
		{
		}


		/** Destructor.
		*/
		virtual ~TimeBasedInterpol()
		{}

		/** Clock used as time reference in this Interpolator.
		*/
		Clock* getClock() const { return m_clock; }
		void setClock( Clock* clock ){ m_clock = clock; }

	private:

		/// Clock used as time reference
		Clock*			m_clock;
	};

}

#endif