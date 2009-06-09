#ifndef GC_LINE_H
#define GC_LINE_H
#pragma once

#include "GC_Common.h"
#include "GC_Curve.h"
#include "GC_SpaceStateUtil.h"

namespace gcore
{
	/** Line representation.
	*/
	template < typename StateType , typename SpaceUnitType = float, typename RelationType = float >
	class Line : public Curve< StateType, SpaceUnitType, RelationType >
	{
	public:

		Line()
		{}

		/** Constructor.
			@see Curve
		*/
		Line( const StateType& beginPoint, const StateType& endPoint ) 
			: Curve( beginPoint, endPoint )
		{

		}
		~Line(){}

		inline SpaceUnitType length( const unsigned long precision = 100 ) const 
		{ 
			const SpaceStateUtil< StateType, SpaceUnitType > posUtil;
			return posUtil.delta( m_beginPoint, m_endPoint );
		}

	private:

		/** Line equation.
		*/
		StateType calculateFromEquation( const RelationType& absRelationA , const RelationType& absRelationB ) const
		{
			return ( m_beginPoint * absRelationA ) + ( m_endPoint * absRelationB ) ;
		}
	};



}

#endif