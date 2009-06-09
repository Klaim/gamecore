#ifndef GC_UNICODEASCIICONVERSION_H
#define GC_UNICODEASCIICONVERSION_H
#pragma once


#include "GC_Common.h"
#include "GC_String.h"

#include "../UTF8cpp/source/utf8.h" // add utf8 unicode tools

namespace gcore
{
	// Here we're assuming that all our unicode strings are UTF-16 or UTF-8.

	/** Convert an UTF-16 string to an Ascii string.
		@param ws Unicode string to convert.
	*/
	String GCORE_API UTF16ToAscii(const LocalizedString& ws);

	/** Convert an Ascii string to an UTF-16 string.
		@param s Ascii string to convert.
	*/
	LocalizedString GCORE_API AsciiToUTF16(const String& s);

	/** Convert an Unicode UTF-16 string to an UTF-8 string.	*/
	String GCORE_API UTF16ToUTF8( const LocalizedString& utf16String );

	/** Convert an Unicode UTF-8 string to an UTF-16 string.	*/
	LocalizedString GCORE_API UTF8ToUTF16( const String& utf8String );

}

#endif