#include "GC_Event.h"

namespace gcore
{
	EventPtr makeEvent( Event::TypeId type )
	{
		return EventPtr( new Event( type ) );
	}


}