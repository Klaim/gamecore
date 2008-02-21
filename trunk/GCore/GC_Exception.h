#ifndef GCORE_EXCEPTION_H
#define GCORE_EXCEPTION_H
#pragma once

#include "GC_String.h"

namespace GCore
{
	/** 
	*	GCore exception base class.
	*/
	class Exception
	{
	private:
		
		String			m_file;
		long			m_line;
		String			m_function;
		String			m_message;
		long			m_code;	

	protected:
	
	public:
	
		///Name of the file where the exception occurred
		const String& getFile() const {return m_file;}

		///Name of the function where the exception occurred 
		const String& getFunction() const {return m_function;}

		///Description of the error/exception and informations.
		const String& getMessage() const {return m_message;}

		///Number of the file in the file where the exception occurred
		long getLine() const {return m_line;}

		///Error identifier code - deprecated
		long getCode() const {return m_code;}
		
		///Copy operator
		virtual void operator = (const Exception& exp )
		{
			m_code = exp.getCode();
			m_file = exp.getFile();
			m_function = exp.getFunction();
			m_line = exp.getLine();
			m_message = exp.getMessage();
			
		}
		
		/** Exception constructor.
			@param msg		Description of the error and other informations.
			@param code		Error code. 
			@param file		Source file name where the error occured.
			@param function	Function or method name where the exception occured.
			@param line		Line number in the file where the exception occured. 
		*/
		Exception( const String& message, long code, const String& function ,  const String& file , long line   )	//Constructor
			:m_message(message),m_function(function),m_file(file),m_line(line),m_code(code){	}

		/** Exception copy constructor.
			@param other	Other Exception to copy from.
		*/
		explicit Exception(const Exception& other)
		{
			(*this) = other;
		}
		
	};




	/** 
		GCore assert exception class.
		Thrown on failed assertion.
		Error code : -1
	*/
	class AssertionException:public Exception
	{
	public:
		/** Assertion exception.
			@param file		Source file name where the error occurred.
			@param function	Function or method name where the exception occurred.
			@param line		Line number in the file where the exception occurred. 
		*/
		explicit AssertionException( const String& msg, const String& testExpression , const String& function ,  const String& file , long line )
			: Exception( "/!\\ Assertion failed! : \n " + msg + " \n Test Expression : " + testExpression , -1 , function , file , line )
			, m_testExpression( testExpression )
		{}

		/** Expression used to test the assertion.
		*/
		const String& getTestExpression() const { return m_testExpression; }

	private:

		/// Expression used to test the assertion.
		String		m_testExpression;

	};

}

/// Throw a general GCore::Exception. Use it as a critical error.
#define GC_EXCEPTION( msg ) throw GCore::Exception( msg , 0, __FUNCTION__ , __FILE__ , __LINE__ )

/// GCore assert macro that throw a GCore::AssertException on failure in debug mode.
#ifdef GC_DEBUG
#define GC_ASSERT( test , msg ) if(!(test))throw GCore::AssertionException( msg , #test , __FUNCTION__ , __FILE__ , __LINE__ )
#else
	#define GC_ASSERT(test , msg)
#endif

#endif