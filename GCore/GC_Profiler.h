#ifndef GC_PROFILER_H
#define GC_PROFILER_H

#include <vector>
#include <ostream>
#include "GC_Common.h"
#include "GC_TimeReferenceProvider.h"

namespace gcore
{
	
	/** Simple time profiling tool.
	*/
	class GCORE_API Profiler
	{
	public:

		/// Contain a list of time spans.
		typedef std::vector< TimeValue > TimeSpanList;

		/** Constructor.
			@param timeReference	Time reference provider used to record time spans.
			@parama maxRecordCount	Maximum number of records allowed for this profiler or 0 if no limit is set.
		*/
		Profiler( const TimeReferenceProvider& timeReference , unsigned int maxRecordCount = 0);
	
		/** Destructor.
		*/
		~Profiler();

		/** Record the time span between the last call to this method to the current one, 
			or record the current call time to register the time span the next time if it's the 
			first call to this method.
			@remark Use this method to check the time spent between two same function call.

			@return Recorded time span between the last call and the current one, or 0 if it's the first call.
		*/
		TimeValue recordTimeSpan( void );

		/** Start to record the time span until the call of stopRecordTimeSpan.
			@see stopRecordTimeSpan
		*/
		void startRecordTimeSpan( void );

		/** Stop to record the current time span and store it.
			@remark Call startRecordTimeSpan before this one. @see startRecordTimeSpan
			@return The stored time span or 0 if startRecordTimeSpan was not called before.
		*/
		TimeValue stopRecordTimeSpan( void );

		/** Clear the records.
			@remark Calling this method will not deallocate memory used by the profiler.
		*/
		void clear();
		
		/** Append an ANSI text report about the current records to the provided stream.
			@param outputStream		Stream to append the text report to.
			@param isFullReport		Should be true if the report have to provide all the time span records.
		*/
		void textReport( std::ostream& outputStream , bool isFullReport = false ) const;
		
		/** Recorded time spans.
		*/
		const TimeSpanList & getTimeSpans() const { return m_timeSpans; }

		/** Returns the biggest time span recorded until here or 0 if no records.
		*/
		TimeValue getBiggestTimeSpan() const;

		/** Returns the shortest time span recorded until here or 0 if no records.
		*/
		TimeValue getShortestTimeSpan() const;

		/** Returns the average time span of the recorded time spans.
		*/
		TimeValue getAverageTimeSpan() const;

		/** Returns the number of time spans recorded.
		*/
		unsigned int getRecordCount() const { return static_cast< unsigned int>( m_timeSpans.size() ); }

		/** Returns the max number of records allowed or 0 if no limit set.
		*/
		unsigned int getMaxRecordCount() const { return m_maxRecordCount; }


	protected:
		
	private:

		/// Recorded time spans.
		TimeSpanList m_timeSpans;

		/// Time value on the last time span record.
		TimeValue m_lastRecordTime;

		/// Time reference provider used to record time spans.
		const TimeReferenceProvider& m_timeReference;

		/// Maximum number of records allowed for this profiler or 0 if no limit is set.
		const unsigned int m_maxRecordCount;
	};
	

}

#endif