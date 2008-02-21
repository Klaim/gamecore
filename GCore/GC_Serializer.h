#ifndef GCORE_SERIALIZER_H
#define GCORE_SERIALIZER_H
#pragma once

#include "GC_Common.h"

namespace GCore
{

	/** Serializer interface used by Serializable objects.
		
		Should be used as base class for different serializer 
		implementations.
		@par
		A Serializer object can only be used to read or write data
		but not read and write together.
		
	*/
	class Serializer
	{
	public:

		/** Constructor.
			@param isWriter	True if this is a writer serializer, false if it's a reader.
		*/
		Serializer( bool isWriter ):
		  m_isWriter( isWriter )
		{}

		/** Destructor.
		*/
		virtual ~Serializer(){}

		
		/** Generic serialization : write or read the value.
			You'll get a compilation error if no read or write function
			have been defined with the provided type.
		*/
		template< typename Type >
		void serialize( Type& dataValue )
		{
			if( m_isWriter ) write( dataValue );
			else read( dataValue );
		}
		

	protected:

		//@{
		/** Read and write methods for basic types.
		Those methods have to be implemented by heritor classes.
		*/
		virtual void read( bool& dataValue  ) = 0;
		virtual void read( unsigned short& dataValue  ) = 0;
		virtual void read( short& dataValue  ) = 0;
		virtual void read( unsigned char& dataValue  ) = 0;
		virtual void read( char& dataValue ) = 0;
		virtual void read( unsigned int& dataValue  ) = 0;
		virtual void read( int& dataValue ) = 0;
		virtual void read( float& dataValue ) = 0;
		virtual void read( double& dataValue ) = 0;

		virtual void write( const bool dataValue  ) = 0;
		virtual void write( const unsigned short dataValue  ) = 0;
		virtual void write( const short dataValue  ) = 0;
		virtual void write( const unsigned char dataValue  ) = 0;
		virtual void write( const char dataValue ) = 0;
		virtual void write( const unsigned int dataValue  ) = 0;
		virtual void write( const int dataValue ) = 0;
		virtual void write( const float dataValue ) = 0;
		virtual void write( const double dataValue ) = 0;
		//@}

		/// True if this is a writer serializer, false if it's a reader.
		const bool m_isWriter;

	private:

		

	};

}

#endif