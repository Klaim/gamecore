#ifndef GC_STRING_H
#define GC_STRING_H
#pragma once

#include <string>

namespace gcore
{

	/// Character type used for Unicode strings in gcore.
	typedef wchar_t UTF_CHAR;
	
	/// Unicode string used in gcore.
	typedef std::basic_string< UTF_CHAR > UTFString;

	/// String type used int gcore.
	typedef std::string String;

}

#endif