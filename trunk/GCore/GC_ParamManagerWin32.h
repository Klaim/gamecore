#ifndef PARAM_MANAGER_WIN32_H
#define PARAM_MANAGER_WIN32_H
#pragma once

#include "GC_Common.h"

#if GC_PLATFORM == GC_PLATFORM_WIN32
#include "GC_ParamManager.h"

namespace gcore
{
	/** Default ParamManager for Win32 system.

	*/
	class ParamManagerWin32: public ParamManager
	{
	public:

		/**
			This parser follow those rules :
			- a parametter name start with '/' and end with ' '
			- the data of a parametter are all characters following the parametter name and ' ' and end with
			the next parametter or the end of the string.
		*/
		void parseParams(const String& parametters , ParamDataList &results);

	};
}
#endif

#endif