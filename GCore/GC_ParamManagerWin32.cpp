#include "GC_ParamManagerWin32.h"
#if GC_PLATFORM == GC_PLATFORM_WIN32

namespace gcore
{

	/**
		This parser follow those rules :
		- a parametter name start with '/' and end with ' '
		- the data of a parametter are all characters following the parametter name and ' ' and end with
		the next parametter or the end of the string.
	*/
	void ParamManagerWin32::parseParams(const String& parametters , ParamDataList &results)
	{
		results.clear();

		ParamData currentParam;

		bool registerParam = false;
		size_t length = parametters.length();
		for(size_t i = 0; i < length ; ++i)
		{

			//check if we changed mode :
			if( parametters[i] == '/')
			{
				if( ! currentParam.param.empty() )//we change of parametter
				{
					//register the current parametter
					results.push_back( currentParam );
					
					//start to get new param name
					currentParam.param = "";
					currentParam.data = "";
				}				 
				registerParam = true;

			}else if((parametters[i] == ' ') && registerParam)
			{
				//start to register parametter data
				registerParam = false;
				continue;//jump to the next character

			}

			if(registerParam)
			{
				//registering parametter name
				currentParam.param += parametters[i];

			}
			else
			{
				//registering parametter data
				if(! currentParam.param.empty())//do nothing if no parametter defined
				{
					currentParam.data += parametters[i];
				}


			}


		}

		if( (!currentParam.data.empty()) || (!currentParam.param.empty()))
		{
			//register the last param and data
			results.push_back( currentParam );

		}

	}
}

#endif