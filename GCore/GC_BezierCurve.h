#ifndef GC_BEZIERCURVE_H
#define GC_BEZIERCURVE_H
#pragma once

#include "GC_Common.h"
#include "GC_Curve.h"

namespace gcore
{
	template < typename StateType , typename SpaceUnitType = float, typename RelationType = float >
	class BezierCurve : public Curve< StateType, SpaceUnitType, RelationType >
	{
	public:
		
		BezierCurve()
			: m_length( -1 )
		{}

		BezierCurve( const StateType& beginPoint, const StateType& endPoint ) 
			: Curve( beginPoint, endPoint )
			, m_length( -1 ) // invalid value to force length calculation on first call of length()
		{}

		virtual ~BezierCurve(){}

		SpaceUnitType length() const 
		{
			if( m_length == -1 )
			{
				const_cast< BezierCurve* >( this )->calculateLength();
			}
			GC_ASSERT( m_length >= 0 , "Bezier curve with negative length!" );
			return m_length;
		}

		/** Calculate the length of ONE period of this Bezier curve, using approximation.
			@param lengthPrecition Number of segments of the curve used to calculate the length aproximation.
		*/
		void calculateLength( unsigned int lengthPrecision = 100 );

	protected:

		inline void invalidateLength(){ m_length = -1; }

	private:

		SpaceUnitType m_length;

	};
	
	/** Quadratic Bezier curve representation.
		@see Curve
	*/
	template < typename StateType , typename SpaceUnitType = float, typename RelationType = float >
	class BezierCurveQuadratic : public BezierCurve< StateType, SpaceUnitType, RelationType >
	{
	public:

		BezierCurveQuadratic(){}

		/** Constructor.
			@see Curve
			@param directorPoint Curve middle director point.
		*/
		BezierCurveQuadratic( const StateType& beginPoint, const StateType& directorPoint , const StateType& endPoint ) 
			: BezierCurve( beginPoint, endPoint )
			, m_directorPoint( directorPoint )
		{}
		
		~BezierCurveQuadratic(){}

		/// Curve middle director point.
		const StateType& getDirectorPoint() const { return m_directorPoint;}

		void setDirectorPoint( const StateType& directorPoint )  
		{	
			if( directorPoint != m_directorPoint )
			{
				m_directorPoint = directorPoint;
				invalidateLength();
			}
		}

	protected:

		/// Curve middle director point.
		StateType m_directorPoint;

		/** Quadratic bezier curve equation.
		*/
		StateType calculateFromEquation( const RelationType& absRelationA , const RelationType& absRelationB ) const
		{
			return ( m_beginPoint * (absRelationA * absRelationA) ) + ( m_directorPoint * ( 2 * absRelationA * absRelationB ) ) + ( m_endPoint * ( absRelationB * absRelationB) ) ;
		}
	};


	/** Cubic Bezier curve representation.
	*/
	template < typename StateType , typename SpaceUnitType = float, typename RelationType = float >
	class BezierCurveCubic : public BezierCurve< StateType, SpaceUnitType, RelationType >
	{
	public:

		BezierCurveCubic(){}

		/** Constructor.
			@see Curve
			@param firstDirectorPoint First curve middle director point.
			@param secondDirectorPoint Second curve middle director point.
		*/
		BezierCurveCubic( const StateType& beginPoint, const StateType& firstDirectorPoint , const StateType& secondDirectorPoint , const StateType& endPoint
				, unsigned int lengthPrecision = 100 ) 
			: BezierCurve( beginPoint, endPoint )
			, m_firstDirectorPoint( firstDirectorPoint )
			, m_secondDirectorPoint( secondDirectorPoint )
		{
		}

		~BezierCurveCubic(){}

		/// First curve middle director point.
		const StateType& getFirstDirectorPoint() const { return m_firstDirectorPoint;}
		void setFirstDirectorPoint( const StateType& firstDirectorPoint ) 
		{ 
			if( firstDirectorPoint != m_firstDirectorPoint )
			{
				m_firstDirectorPoint = firstDirectorPoint;
				invalidateLength();
			}
		}

		/// Second curve middle director point.
		const StateType& getSecondDirectorPoint() const { return m_secondDirectorPoint;}
		void setSecondDirectorPoint( const StateType& secondDirectorPoint ) 
		{ 
			if( secondDirectorPoint != m_secondDirectorPoint )
			{
				m_secondDirectorPoint = secondDirectorPoint; 
				invalidateLength();
			}
			
		}

	private:

		/// Begin point of the curve (in it's origin period).
		StateType m_firstDirectorPoint;
		
		/// Begin point of the curve (in it's origin period).
		StateType m_secondDirectorPoint;

		/** Cubic bezier curve equation.
		*/
		StateType calculateFromEquation( const RelationType& absRelationA , const RelationType& absRelationB ) const
		{
			return ( m_beginPoint * ( absRelationA * absRelationA * absRelationA) ) + ( m_firstDirectorPoint * ( 3 * absRelationA * absRelationA * absRelationB ) ) + ( m_secondDirectorPoint * ( 3 * absRelationA * absRelationB * absRelationB ) ) + ( m_endPoint * ( absRelationB * absRelationB * absRelationB ) ) ;
		}
	};


	template < typename StateType , typename SpaceUnitType, typename RelationType >
	void gcore::BezierCurve<StateType, SpaceUnitType, RelationType>::calculateLength( unsigned int lengthPrecision /*= 100 */ )
	{
		GC_ASSERT( lengthPrecision > 0, "Tried to calculate length of Bezier curve by subdividing it in 0 segments!" );

		
		// TODO : Optimize this!

		// this is a raw version of the algorithm 
		/*	We need to calculate the length of a full period of the curve.
			To do this we will slice the curve in segments and get the distance
			of each part then make the sum to have an aproximation.
		*/

		// calculate the length of the full period of this curve
		SpaceUnitType resultLength = 0;
		const RelationType segmentPart = 1 / static_cast<RelationType>( lengthPrecision );

		for( unsigned int k = 0; k < lengthPrecision; ++k )
		{
			const RelationType segmentStart = k * segmentPart;
			const RelationType segmentEnd = (k + 1) * segmentPart;

			const StateType startPoint ( calculatePoint( segmentStart ) );
			const StateType endPoint ( calculatePoint( segmentEnd ) );

			const SpaceStateUtil< StateType, SpaceUnitType > posUtil;
			const SpaceUnitType segmentLength = posUtil.distance( startPoint, endPoint );
			GC_ASSERT( segmentLength >= 0, "Segment length is negative!" );
			resultLength += segmentLength;
		}

		// store the final result
		m_length = resultLength;
	}

}

#endif