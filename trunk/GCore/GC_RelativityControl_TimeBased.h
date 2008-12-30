#ifndef GC_RELATIVITYCONTROL_TIMEBASED_H
#define GC_RELATIVITYCONTROL_TIMEBASED_H
#pragma once

#include "GC_Common.h"
#include "GC_AutoMove.h"
#include "GC_Clock.h"

namespace gcore
{
namespace automove
{
	/** Base class for time base interpolators.
		This interpolator will update the object to update with time flowing. The time
		flow is totally dependent on the provided clock.
		@see AutoMove.
	*/
	template < typename StateType , typename SpaceUnitType >
	class RelativityControl_TimeBased : virtual public AutoMove< StateType , SpaceUnitType >
	{
	public:

		/** Constructor.
			@see AutoMove::AutoMove
			@param clock		Clock used as time reference.
		*/
		RelativityControl_TimeBased( const StateType& initialState, Clock* clock )
			: AutoMove( initialState )
			, m_clock( clock )
		{			
		}
	
		RelativityControl_TimeBased( Clock* clock )
			: AutoMove()
			, m_clock( clock )
		{
		}


		/** Destructor.
		*/
		virtual ~RelativityControl_TimeBased()
		{}

		/** Clock used as time reference in this AutoMove.
		*/
		Clock* getClock() const { return m_clock; }
		void setClock( Clock* clock ){ m_clock = clock; }

	private:

		/// Clock used as time reference
		Clock*			m_clock;
	};
}
}

#endif