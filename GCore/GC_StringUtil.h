#ifndef GC_STRINGUTIL_H
#define GC_STRINGUTIL_H
#pragma once

#include "GC_Common.h"
#include "GC_String.h"

namespace gcore
{
	/* String utilities 
	*/

	/** Convert a number to string using basic STL conversion (std::basic_stringstream)
		@see String @see LocalizedString @see std::basic_stringstream
	*/
	template< typename NumType >
	inline String numToString( const NumType& number )
	{
		std::stringstream converterStream;
		converterStream << number;
		return converterStream.str();
	}

	/** @copydoc numToString   
	*/
	template< typename NumType >
	inline LocalizedString numToUTFString( const NumType& number )
	{
		std::wstringstream converterStream;
		converterStream << number;
		return converterStream.str();
	}

}

#endif