#ifndef GC_RELATIVITYCONTROL_TIMEBASED_H
#define GC_RELATIVITYCONTROL_TIMEBASED_H
#pragma once

#include "GC_Common.h"
#include "GC_Interpolator.h"
#include "GC_Clock.h"

namespace gcore
{
namespace interpolation
{
	/** Base class for time base interpolators.
		This interpolator will update the object to update with time flowing. The time
		flow is totally dependent on the provided clock.
		@see Interpolator.
	*/
	template < typename StateType , typename SpaceUnitType >
	class RelativityControl_TimeBased : virtual public Interpolator< StateType , SpaceUnitType >
	{
	public:

		/** Constructor.
			@see Interpolator::Interpolator
			@param clock		Clock used as time reference.
		*/
		RelativityControl_TimeBased( const StateType& initialState, const Clock* clock )
			: Interpolator( initialState )
			, m_clock( clock )
		{			
		}
	
		RelativityControl_TimeBased( Clock* clock )
			: Interpolator()
			, m_clock( clock )
		{
		}


		/** Destructor.
		*/
		virtual ~RelativityControl_TimeBased()
		{}

		/** Clock used as time reference in this Interpolator.
		*/
		const Clock* getClock() const { return m_clock; }
		void setClock( const Clock* clock ){ m_clock = clock; }

	private:

		/// Clock used as time reference
		const Clock*			m_clock;
	};
}
}

#endif