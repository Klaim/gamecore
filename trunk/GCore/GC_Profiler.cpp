#include <algorithm>

#include "GC_Profiler.h"


namespace gcore
{
	

	Profiler::Profiler( const TimeReferenceProvider& timeReference  , unsigned int maxRecordCount) 
		: m_lastRecordTime( 0 )
		, m_timeReference( timeReference )
		, m_maxRecordCount( maxRecordCount )
	{
		if( maxRecordCount > 0)
		{
			// optimize memory manipulation
			m_timeSpans.reserve( maxRecordCount );
		}
	}

	Profiler::~Profiler()
	{

	}

	void Profiler::startRecordTimeSpan( void )
	{
		m_lastRecordTime = m_timeReference.getTimeSinceStart();
	}

	gcore::TimeValue Profiler::stopRecordTimeSpan( void )
	{
		if( m_lastRecordTime != 0 )
		{
			// record the time span since last record
			const TimeValue currentTime( m_timeReference.getTimeSinceStart() );
			const TimeValue spanTime( currentTime - m_lastRecordTime );

			if( m_maxRecordCount == 0 ) // we have no record limit
			{
				m_timeSpans.push_back( spanTime );
			}
			else
			{
				// we have a record limit : register only if we have enough records
				if( m_timeSpans.empty() || m_timeSpans.size() < m_maxRecordCount )
				{
					m_timeSpans.push_back( spanTime );
				}
			}

			// update the last record time
			m_lastRecordTime = currentTime; 

			return spanTime;
		}
		else 
		{
			return 0;
		}
	}

	TimeValue Profiler::recordTimeSpan( void )
	{
		if( m_lastRecordTime != 0 )
		{
			return stopRecordTimeSpan();
		}
		else
		{
			// first record : just get the current time for the next time we track
			startRecordTimeSpan();
			return 0;
		}
	}

	void Profiler::textReport( std::ostream& outputStream , bool isFullReport /*= false */ ) const
	{
		// gather data...
		const TimeValue biggestTimeSpan( getBiggestTimeSpan() );
		const TimeValue shortestTimeSpan( getShortestTimeSpan() );
		const TimeValue averageTimeSpan( getAverageTimeSpan() );

		outputStream << std::endl << "Profiler recorded "<< m_timeSpans.size() << " time spans : "<< std::endl;
		outputStream << "\tBiggest time span : \t" << biggestTimeSpan << " millisecs" << std::endl;
		outputStream << "\tShortest time span : \t" << shortestTimeSpan << " millisecs" << std::endl;
		outputStream << "\tAverage time span : \t"<< averageTimeSpan << " millisecs" << std::endl;

		if( isFullReport )
		{
			outputStream << "Time Spans : " << std::endl;
			const std::size_t recordCount( m_timeSpans.size() );
			for(std::size_t i = 0; i < recordCount; ++i)
			{
				outputStream << "[" << i << "] = "<< m_timeSpans[i] << " millisecs" << std::endl;
			}
		}
		outputStream.flush();
	}

	TimeValue Profiler::getBiggestTimeSpan() const
	{
		TimeSpanList::const_iterator it( std::max_element( m_timeSpans.begin(), m_timeSpans.end() ));
		if( it != m_timeSpans.end())
		{
			return (*it);
		}
		else return 0;
	}

	TimeValue Profiler::getShortestTimeSpan() const
	{
		TimeSpanList::const_iterator it( std::min_element( m_timeSpans.begin(), m_timeSpans.end() ));
		if( it != m_timeSpans.end())
		{
			return (*it);
		}
		else return 0;
	}

	TimeValue Profiler::getAverageTimeSpan() const
	{
		if( m_timeSpans.empty() ) return 0;

		TimeValue timeSpanSum = 0;

		const std::size_t recordCount( m_timeSpans.size() );
		for(std::size_t i = 0; i < recordCount; ++i)
		{
			timeSpanSum += m_timeSpans[i];
		}

		return timeSpanSum / recordCount;
	}

	void Profiler::clear()
	{
		m_lastRecordTime = 0;
		m_timeSpans.clear();
	}

	
}