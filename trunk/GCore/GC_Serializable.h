#ifndef GC_SERIALIZABLE_H
#define GC_SERIALIZABLE_H
#pragma once

#include "GC_Common.h"

#include "GC_Serializer.h"

namespace GCore
{

	/** Serializable interface for objects that should be serializable.
		Force the heritor class to provide a Serializer manipulation method to
		describe the data and the order in wich should be read and wrote for serialization.
	*/
	class Serializable
	{
	public:

		/** User defined serialization.
			The implementation should describe wich data to serialize
			via the serializer object, and in wich order (via the calling
			order).
			
			@par
			example :

			@code
			serializer.serialize( this.m_objectId );
			serializer.serialize( this.m_objectPosX );
			serializer.serialize( this.m_objectPosY );
			@endcode 

			@par
			In this example, when the data are read or written, its always in the same order
			making serialization more simple.

		*/
		virtual void serialization( Serializer& serializer ) = 0;

	protected:
		
	private:
	
	};
	

}

#endif