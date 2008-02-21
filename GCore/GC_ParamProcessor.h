#ifndef PARAM_PROCESSOR_H
#define PARAM_PROCESSOR_H
#pragma once

#include "GC_String.h"
#include "GC_Common.h"
namespace GCore
{

	/*

		ParamProcessor is a base class used to process a
		parametter found in the application command line.

	*/
	class ParamProcessor
	{
	public:

		/**
			A ParamProcessor should always have the name of the parametter
			it will process.
		*/
		ParamProcessor(const String& paramName):m_paramName(paramName)
		{}

		virtual ~ParamProcessor(){}


		/**
			Name of the parametter to process.
		*/
		const String& getParamName() const {return m_paramName;}
		
		/**
			User defined parametter process execution.
		*/
		virtual void process(const String& paramData) = 0;

	protected:


	private:


		///	Name of the parametter to process.
		String m_paramName;

		
	};

}

#endif