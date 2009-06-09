
#include <locale>
#include <vector>

#include "GC_UnicodeAscii.h"

namespace gcore
{

	String GCORE_API UTF16ToAscii( const LocalizedString& ws )
	{
		std::vector<char> buffer(ws.size());
		std::locale loc("english");
		std::use_facet< std::ctype< LocalizedString::value_type > >(loc).narrow(ws.data(), ws.data() + ws.size(), '?', &buffer[0]);

		return String(&buffer[0], buffer.size());
	}

	LocalizedString GCORE_API AsciiToUTF16( const String& s )
	{
		std::vector< LocalizedString::value_type > buffer(s.size());
		std::locale loc("english");
		std::use_facet< std::ctype< LocalizedString::value_type > >(loc).widen(s.data(), s.data() + s.size(), &buffer[0]);

		return LocalizedString(&buffer[0], buffer.size());
	}

	String GCORE_API UTF16ToUTF8( const LocalizedString& utf16String )
	{
		String resultString;
		resultString.reserve( utf16String.length() );
		utf8::utf16to8( utf16String.begin(), utf16String.end(), std::back_inserter( resultString ) );

		return resultString;
	}

	LocalizedString GCORE_API UTF8ToUTF16( const String& utf8String )
	{
		LocalizedString resultString;
		resultString.reserve( utf8String.length() );
		utf8::utf8to16( utf8String.begin(), utf8String.end(), std::back_inserter( resultString ) );

		return resultString; 
	}

}