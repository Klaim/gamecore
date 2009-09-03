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
			@param maxRecordCount	Maximum number of records allowed for this profiler or 0 if no limit is set.
		*/
		Profiler( const TimeReferenceProvider& timeReference , unsigned long maxRecordCount = 0);
	
		/** Destructor.
		*/
		~Profiler();

		/** Start to record the time span until the call of stop.
			@see stop
		*/
		void start();

		/** Stop to record the current time span and store it.
			@remark Call start before this one. @see start
			@return The stored time span or 0 if start was not called before.
		*/
		TimeValue stop();

		/** Record the time span between the last call to this method to the current one, 
			or record the current call time to register the time span the next time if it's the 
			first call to this method.
			@remark Use this method to check the time spent between two same function call.
			
			@return Recorded time span between the last call and the current one, or 0 if it's the first call.
		*/
		TimeValue record();

		/** Clear the records.
			@remark Calling this method will not deallocate memory used by the profiler.
		*/
		void clear();
		
		/** Append an ANSI text report about the current records to the provided stream.
			@param outputStream		Stream to append the text report to.
			@param isFullReport		Should be true if the report have to provide all the time span records.
		*/
		void report( std::ostream& outputStream , bool isFullReport = false ) const;

		/** Return an ANSI text report about the current records.
			@param isFullReport		Should be true if the report have to provide all the time span records.
		*/
		String report( bool isFullReport = false ) const;
		
		/** Recorded time spans.
		*/
		const TimeSpanList & timeSpans() const { return m_timeSpans; }

		/** @return The biggest time span recorded until here or 0 if no records.
		*/
		TimeValue biggest() const;

		/** @return The shortest time span recorded until here or 0 if no records.
		*/
		TimeValue shortest() const;

		/** @return The average time span of the recorded time spans.
		*/
		TimeValue average() const;

		/** @return The number of time spans recorded.
		*/
		unsigned long count() const { return static_cast< unsigned long>( m_timeSpans.size() ); }

		/** @return The max number of records allowed or 0 if no limit set.
		*/
		unsigned long maxCount() const { return m_maxRecordCount; }

		/** Reserve the memory for the given estimated count time span to limit record performance impact.
		*/
		void reserve( unsigned long estimatedCount );

	protected:
		
	private:

		/// Recorded time spans.
		TimeSpanList m_timeSpans;

		/// Time value on the last time span record.
		TimeValue m_lastRecordTime;

		/// Time reference provider used to record time spans.
		const TimeReferenceProvider& m_timeReference;

		/// Maximum number of records allowed for this profiler or 0 if no limit is set.
		const unsigned long m_maxRecordCount;
	};
	

}

#endif