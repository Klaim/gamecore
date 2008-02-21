
#include <locale>
#include <vector>

#include "GC_UnicodeAscii.h"

namespace GCore
{

	String GCORE_API ToAscii( const UTFString& ws )
{
		std::vector<char> buffer(ws.size());
		std::locale loc("english");
		std::use_facet< std::ctype< UTF_CHAR > >(loc).narrow(ws.data(), ws.data() + ws.size(), '?', &buffer[0]);

		return String(&buffer[0], buffer.size());
	}

	UTFString GCORE_API ToUnicode( const String& s )
{
		std::vector<UTF_CHAR> buffer(s.size());
		std::locale loc("english");
		std::use_facet< std::ctype< UTF_CHAR > >(loc).widen(s.data(), s.data() + s.size(), &buffer[0]);

		return UTFString(&buffer[0], buffer.size());
	}

}