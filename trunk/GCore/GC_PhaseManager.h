#ifndef GC_PHASEMANAGER_H
#define GC_PHASEMANAGER_H
#pragma once

#include <map>
#include <vector>
#include <memory>
//#include <boost/shared_ptr.hpp>

#include "GC_Common.h"
#include "GC_String.h"

#include "GC_Phase.h"

namespace gcore
{
	typedef  std::tr1::shared_ptr< Phase > PhasePtr;
	//typedef  boost::shared_ptr< Phase > PhasePtr;

	/** Utility tool for managing several Phases in one object.
		
	*/
	class GCORE_API PhaseManager
	{
	public:

		/** Constructor.
		*/
		PhaseManager();
	
		/** Destructor.
		*/
		~PhaseManager();

		/** Register a phase.
			@param phase Phase to register.
		*/
		void registerPhase( const PhasePtr& phase );

		/** Unregister a registered phase.
			@param phaseName Name of the phase to unregister.
		*/
		void unregisterPhase( const String& phaseName );

		/** Get a registered phase by it's name.
			@param phaseName Name of the registered phase to retrieve.
			@return The requested phase or null if not found.
		*/
		PhasePtr getRegisteredPhase( const String& phaseName );

		/** Get a list of registered phases' names.
		*/
		std::vector< String > getRegisteredPhaseNames() const;

		/** Request a phase to load.
			@param phaseName Name of the registered phase to load.
		*/
		void requestLoadPhase( const String& phaseName );
		
		/** Request a phase to load.
			@param phaseName Name of the registered phase to unload.
		*/
		void requestUnloadPhase( const String& phaseName );


		/** Request a phase to load.
			@param phaseName Name of the registered phase to activate.
		*/
		void requestActivatePhase( const String& phaseName );

		/** Request a phase to load.
			@param phaseName Name of the registered phase to terminate.
		*/
		void requestTerminatePhase( const String& phaseName );

		/** Unregister all registered phases.
		*/
		void unregisterAllPhases();

	protected:
		
	private:

		/// phase index
		std::map< String,  PhasePtr > m_phaseIndex;
	};
	

}

#endif