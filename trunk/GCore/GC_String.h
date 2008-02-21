#ifndef GC_STRING_H
#define GC_STRING_H
#pragma once

#include <string>

namespace GCore
{

	/// Character type used for Unicode strings in GCore.
	typedef wchar_t UTF_CHAR;

	
	/// Unicode string used in GCore.
	typedef std::basic_string< UTF_CHAR > UTFString;


	/// String type used int GCore.
	typedef std::string String;



}

#endif