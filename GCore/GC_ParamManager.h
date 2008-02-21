#ifndef PARAM_MANAGER_H
#define PARAM_MANAGER_H
#pragma once

#include "GC_String.h"
#include <map>
#include <vector>
#include "GC_Common.h"
#include "GC_ParamProcessor.h"

namespace GCore
{
	/** The key is the name of the processor and the value is a pointer to the processor.
	*/
	typedef std::map<String, ParamProcessor*> ParamProcList;

	/** A parametter and its data.
	*/
	struct ParamData
	{ 
		String param; 
		String data;

		ParamData(const String& paramName ="", const String& paramData ="")
			:param(paramName), data(paramData)
		{}

		ParamData(const ParamData& a):param(a.param),data(a.data)
		{}


	};

	typedef std::vector<ParamData> ParamDataList;

	/** Base class for parsing the parametters of the command line and execute the processes according to marametters and their data.
		It register the diferrent ParamProcessors assossiated to
		parametters and call them if found in the command line.
	*/
	class ParamManager 
	{

	public:

		ParamManager();
		virtual ~ParamManager();


		
		/** Register a new ParamProcessor.
			Once registered, the ParamProcessor::process method can be called
			if the ParamProcessor::name is found in the command line when parsed.
		*/
		void registerProcessor(ParamProcessor* processor);

		/** Parse the parametters from the command line.
			If a ParamProcessor name is found in the command line, it's ParamProcessor::process method
			will pe called with the param data.
			
			The parsing rules to recognize parametters and differentiate them of their data is 
			user application dependant and is defined by ParamManager::parseParams redefinition.

			'parametters' must contain only the parametter part of the command line.
		*/
		void processParams(const String& parametters);


		/** Parse the parametters from 'parametters' string and put into 'results' map the parametter names
			as keys and their data as values.

			This should be defined for each application type as parameter syntax should be user application
			dependant.
		*/
		virtual void parseParams(const String& parametters , ParamDataList &results) = 0;


	protected:

		
	private:

		///Registered ParamProcessors. 
		ParamProcList m_registeredProcessors;



	};
}

#endif