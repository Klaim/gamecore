
#include "GC_ParamManager.h"

namespace gcore
{

	ParamManager::ParamManager()
	{

	}

	ParamManager::~ParamManager()
	{

	}

	/**
		Register a new ParamProcessor.
		Once registered, the ParamProcessor::process method can be called
		if the ParamProcessor::name is found in the command line when parsed.
	*/
	void ParamManager::registerProcessor(ParamProcessor* processor)
	{
		//processor should not be null
		GC_ASSERT( processor = nullptr , "Tried to register a null param processor in a param manager!" );

		//processor name should not be already registered
		if( m_registeredProcessors.find(processor->getParamName()) != m_registeredProcessors.end() )
		{
			GC_EXCEPTION( "Tried to register a param processor wich name is already registered : " + processor->getParamName() );
		}

		////////////////////////////

		m_registeredProcessors[ processor->getParamName() ] = processor;

	}

	/**
		Parse the parametters from the command line.
		If a ParamProcessor name is found in the command line, it's ParamProcessor::process method
		will be called with the param data.
		
		The parsing rules to recognize parametters and differentiate them of their data is 
		user application dependent and is defined by ParamManager::parseParams redefinition.

		'parametters' must contain only the parameter part of the command line.
	*/
	void ParamManager::processParams(const String& parametters)
	{
		//parse parametters and their data
		ParamDataList parsedParams;
		parseParams(parametters,parsedParams);

		//find associated processors and execute them
		for(ParamDataList::iterator it = parsedParams.begin(); it != parsedParams.end(); ++it)
		{
			ParamProcList::iterator itProc = m_registeredProcessors.find( it->param );
			if( itProc != m_registeredProcessors.end() )
			{
				//Processor for the current parameter found : process it with parameter data!
				itProc->second->process( it->data );
			}
			
		}


	}
}