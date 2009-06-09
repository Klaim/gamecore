#ifndef GC_STRING_H
#define GC_STRING_H
#pragma once

#include <string>

#include "GC_CrossPlatform.h"

namespace gcore
{
	// change that once C++0x is here!
	typedef wchar_t UTF16_char;

	/// Unicode string used in gcore : UTF-16 encoding.
	typedef std::basic_string< UTF16_char > LocalizedString;

	/// String type used in gcore.
	typedef std::string String;

}

#endif