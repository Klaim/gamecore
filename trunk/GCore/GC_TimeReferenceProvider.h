#ifndef GCORE_TIMEREFERENCEPROVIDER_H
#define GCORE_TIMEREFERENCEPROVIDER_H
#pragma once

#include "GC_Common.h"
#include "GC_Time.h"

namespace gcore
{



	/** Platform-dependent system or application(or other reference)  start time provider base class.
		This is used to provide a time reference to ClockManager objects.
		@see ClockManager
	*/
	class GCORE_API TimeReferenceProvider
	{
	public:
		/** Time passed since the system started (or other base time reference, implementation-specific).
			@return Time value (in milliseconds).
		*/
		virtual TimeValue getTimeSinceStart() const = 0;


		/** Destructor.
		*/
		virtual ~TimeReferenceProvider(){}


	protected:

	private:
	};

}

#endif