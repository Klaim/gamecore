#ifndef GC_PHASEMANAGER_H
#define GC_PHASEMANAGER_H
#pragma once

#include <map>
#include <vector>

#include "GC_Common.h"
#include "GC_String.h"

#include "GC_Phase.h"

namespace gcore
{
		
	/** Utility tool for managing several Phases in one object.
		
	*/
	class GCORE_API PhaseManager 
	{
	public:
		PhaseManager();

		~PhaseManager();

		/** Register a phase.
			@param phase Phase to register.
		*/
		void registerPhase( const PhasePtr& phase );

		/** Unregister a registered phase.
			@param phaseName Name of the phase to unregister.
		*/
		void unregisterPhase( const String& phaseName ) { unregisterPhase( findRegisteredPhase( phaseName ) ); }
		void unregisterPhase( const PhasePtr& phase );


		/** Request a phase to load.
			@param phaseName Name of the registered phase to load.
		*/
		void requestLoadPhase( const String& phaseName ){ requestLoadPhase( findRegisteredPhase( phaseName ) ); }
		void requestLoadPhase( const PhasePtr& phase );
		
		/** Request a phase to load.
			@param phaseName Name of the registered phase to unload.
		*/
		void requestUnloadPhase( const String& phaseName ){ requestUnloadPhase( findRegisteredPhase( phaseName ) ); }
		void requestUnloadPhase( const PhasePtr& phase );


		/** Request a phase to load.
			@param phaseName Name of the registered phase to activate.
		*/
		void requestActivatePhase( const String& phaseName ){ requestActivatePhase( findRegisteredPhase( phaseName ) ); }
		void requestActivatePhase( const PhasePtr& phase );

		/** Request a phase to load.
			@param phaseName Name of the registered phase to terminate.
		*/
		void requestTerminatePhase( const String& phaseName ){ requestTerminatePhase( findRegisteredPhase( phaseName ) ); }
		void requestTerminatePhase( const PhasePtr& phase );

		/** Unregister all registered phases.
		*/
		void unregisterAllPhases();


		/** Find a registered phase by it's name.
		@param phaseName Name of the registered phase to retrieve.
		@return The requested phase or null if not found.
		*/
		PhasePtr findPhase( const String& phaseName );

		/** Find a registered phase by it's name and throw an exception if not found!
		@return The requested phase. Will throw an exception if the phase is not found.
		*/
		PhasePtr findRegisteredPhase( const String& phaseName );


		/** Get a list of registered phases' names.
		*/
		std::vector< String > getRegisteredPhaseNames() const;

	protected:
		
	private:

		/// phase index
		std::map< String,  PhasePtr > m_phaseIndex;

		// non copyable
		PhaseManager(const Phase& ohterPhase);
		void operator=(const Phase& ohterPhase); 

		
	};
	

}

#endif