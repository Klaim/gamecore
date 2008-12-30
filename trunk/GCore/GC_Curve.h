#ifndef GC_CURVE_H
#define GC_CURVE_H
#pragma once

#include <cmath>

#include "GC_Common.h"

namespace gcore
{
	/** Base class for curve representation.
		@param StateType Type of the positions to process. Multiplication, addition and substraction need to be accessible
				and behave as if the operation is done independantly on each "member" of the position type.
		@param RelationType Floating type used to get a position between the begin point and the end point of the curve (or outer).
	*/
	template< typename StateType , typename SpaceUnitType = float, typename RelationType = float >
	class Curve
	{
	public:

		/** Constructor.
			@param beginPoint Point where the curve begins in it's origin period. (Pos 1.0f)
			@param endPoint Point where the curve ends in it's origin period. (Pos 0.0f)
		*/
		Curve( const StateType& beginPoint, const StateType& endPoint )
			: m_beginPoint( beginPoint )
			, m_endPoint( endPoint )
		{;}

		Curve()	{;}

		/** Destructor.
		*/
		virtual ~Curve(){;}

		/** Calculate the position of a point on the curve.
			This method will call calculateFromEquation to determine the position of the point in the origin period.
			@param curveRelativePos Define where on the curve the point is, 0 being the begin point and 1 the end point.
			If this value is over 1 or under 0, the curve will be managed as a infinitely repeating sequence, like if 
			between 0 and 1 it's a full period.
		*/
		StateType calculatePoint( const RelationType& curveRelativePos ) const
		{
			
			const RelationType positiveCurveRelativePos = fabs( curveRelativePos );
			const RelationType a = (positiveCurveRelativePos > 0 && positiveCurveRelativePos == ceil(positiveCurveRelativePos)) ? 1 : fmod( positiveCurveRelativePos , RelationType(1) );
			const RelationType b = RelationType(1) - a;

			const RelationType fullPeriodCount = (curveRelativePos == 0 ) ? 0 : (floor(curveRelativePos) - (floor(a) + floor(b))) ; // number of times we got OVER 1.0
			const StateType lineVector = m_endPoint - m_beginPoint;
			
			StateType resultPos;

			if( fullPeriodCount >= 0 )
			{
				resultPos = this->calculateFromEquation( b, a); 
			}
			else
			{
				resultPos = this->calculateFromEquation( a, b); 
			}

			resultPos = resultPos + ( fullPeriodCount * lineVector );

			return resultPos;
		}

		/** Get the begin point of the curve (in it's origin period).
		*/
		const StateType& getBeginPoint() const { return m_beginPoint; }
		void setBeginPoint( const StateType& beginPoint ) { m_beginPoint = beginPoint; }


		/** Get the end point of the curve (in it's origin period).
		*/
		const StateType& getEndPoint() const { return m_endPoint; }
		void setEndPoint( const StateType& endPoint ) { m_endPoint = endPoint; }

		/** Length of ONE period of this curve.  */
		virtual SpaceUnitType length() const = 0;

	protected:

		/// Begin point of the curve (in it's origin period).
		StateType m_beginPoint;

		/// End point of the curve (in it's origin period).
		StateType m_endPoint;

		/** Equation from witch we get the position of a point on the origin period of the curve.
			@param absRelationA Value between 0 and 1 that determine where is the value between the begin point and the end point.
			@param absRelationB Value equal to : 1 - absRelationA
		*/
		virtual StateType calculateFromEquation( const RelationType& absRelationA , const RelationType& absRelationB ) const = 0;
	};
}

#endif