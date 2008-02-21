#ifndef GC_UNICODEASCIICONVERSION_H
#define GC_UNICODEASCIICONVERSION_H
#pragma once


#include "GC_Common.h"
#include "GC_String.h"

namespace GCore
{

	/** Convert an Unicode string to an Ascii string.
		@param ws Unicode string to convert.
	*/
	String GCORE_API ToAscii(const UTFString& ws);

	/** Convert an Ascii string to an Unicode string.
		@param s Ascii string to convert.
	*/
	UTFString GCORE_API ToUnicode(const String& s);
	

}

#endif