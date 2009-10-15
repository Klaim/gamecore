#ifndef GC_STRINGSTREAM_H
#define GC_STRINGSTREAM_H
#pragma once

#include <sstream>
#include "GC_String.h"

namespace gcore
{
	typedef std::basic_stringstream< char > StringStream;
	typedef std::basic_stringstream< UTF16_char > LocalizedStringStream;

}

#endif