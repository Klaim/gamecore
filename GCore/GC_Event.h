#ifndef GCORE_EVENT_H
#define GCORE_EVENT_H
#pragma once

//#include <boost/shared_ptr.hpp>
#include <memory>

#include "GC_Common.h"

namespace gcore
{

	///Event type type .
	typedef int EventType;

	/** Event object base class.
		
		An Event is sent in an EventManager that will reroute
		it to it's registered EventListeners that have the same EventType.
		@par
		This class alone is enough to manage simple events. In the case
		of an event that should provide data, you should use this class as
		base class.
		@remark
		An event can sent to the appropriate EventListeners in an immediate way 
		or in a buffered way. See EventManager class for more details.
		@see EventManager

	*/
	class GCORE_API Event
	{
	private:

	protected:

		///Event type value. It is not constant to make the Event class serializable if necessary.
		EventType m_Type;

	public:

		/** Constructor.
			@param Type id of this event.
		*/
		Event(EventType type):m_Type(type)
		{}
		
		virtual ~Event(){}

		/** Event type id value.
			@return Type id of this event.
		*/
		const EventType& getType() const {return m_Type;}
	};

	/** Smart pointer for Event objects.
	*/
	//typedef  boost::shared_ptr<Event> EventPtr;
	typedef  std::tr1::shared_ptr<Event> EventPtr;

}

#endif