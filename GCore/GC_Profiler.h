#ifndef GC_PROFILER_H
#define GC_PROFILER_H

#include "GC_Common.h"

namespace GCore
{
	/** No documentation yet.
	*/
	class Profiler
	{
	public:

		/** Constructor.
		*/
		Profiler();
	
		/** Destructor.
		*/
		~Profiler();

		int toValue( const std::string& text, bool val = false );

	protected:
		
	private:
	
	};
	

}

#endif