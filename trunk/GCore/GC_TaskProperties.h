#ifndef GC_TASKPROPERTIES_H
#define GC_TASKPROPERTIES_H
#pragma once

namespace gcore
{
	/// Task priority type.
	typedef float TaskPriority;

	/// Task state.
	enum TaskState
	{
		/// The Task is not registered in a TaskManager.
		TS_UNREGISTERED = 0,

		/// The Task is registered but is not activated.
		TS_REGISTERED,

		/// The Task is registered and active.
		TS_ACTIVE,

		/// The Task is registered and paused.
		TS_PAUSED,

	};	

}

#endif