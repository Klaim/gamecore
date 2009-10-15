#include "GC_PhaseManager.h"
#include "GC_Phase.h"

namespace gcore
{

	PhaseManager::PhaseManager()
	{

	}

	PhaseManager::~PhaseManager()
	{
		unregisterAllPhases();
	}

	void PhaseManager::registerPhase( const PhasePtr& phase )
	{
		GC_ASSERT( phase.get() != nullptr, "Tried to register a null phase!" );
		GC_ASSERT( findPhase( phase->name() ).get() == nullptr, String( "Tried to register an already registered Phase! Phase Name : " ) + phase->name() );
		
		// register
		m_phaseIndex[ phase->name() ] = phase;
		phase->m_phaseManager = this;
		
		// notify
		//phase->onRegistered(); // obsolete

	}

	void PhaseManager::unregisterPhase( const PhasePtr& phase )
	{
		GC_ASSERT( phase.get() != nullptr , "Tried to unregister a phase not registered!");

		// notify
		// phase->onUnregister(); // obsolete

		if( phase->state() != Phase::UNLOADED )
		{
			// the phase have to be unloaded!
			// try to unload it now...

			if( phase->state() == Phase::ACTIVE )
			{
				requestTerminatePhase( phase );
			}

			if( phase->state() == Phase::LOADED )
			{
				requestUnloadPhase( phase );
			}
			
			GC_ASSERT( phase->state() == Phase::UNLOADED, String( "Tried to unregister phase " ) + phase->name() + String(" but is not unloaded!") );
		}

		// unregister
		phase->m_phaseManager = nullptr;
		m_phaseIndex.erase( phase->name() );
	}

	void PhaseManager::requestLoadPhase( const PhasePtr& phase )
	{
		GC_ASSERT( phase.get() != nullptr, "Requested to load a null phase..." );

		if( phase->isManaged() && phase->m_phaseManager == this )
		{
			phase->requestLoad();
		}
		else
		{
			GC_EXCEPTION << "Tried to request load to a non registered Phase! Name requested : " << phase->name();
		}

	}

	void PhaseManager::requestUnloadPhase( const PhasePtr& phase )
	{
		GC_ASSERT( phase.get() != nullptr, "Requested to unload a null phase..." );

		if( phase->isManaged() && phase->m_phaseManager == this )
		{
			phase->requestUnload();
		}
		else
		{
			GC_EXCEPTION << "Tried to request unload to a non registered Phase! Name requested : " << phase->name();
		}

	}

	void PhaseManager::requestActivatePhase( const PhasePtr& phase )
	{
		GC_ASSERT( phase.get() != nullptr, "Requested to activate a null phase..." );

		if( phase->isManaged() && phase->m_phaseManager == this )
		{
			phase->requestActivate();
		}
		else
		{
			GC_EXCEPTION << "Tried to request activation to a non registered Phase! Name requested : " << phase->name();
		}

	}

	void PhaseManager::requestTerminatePhase( const PhasePtr& phase )
	{
		GC_ASSERT( phase.get() != nullptr, "Requested to terminate a null phase..." );

		if( phase->isManaged() && phase->m_phaseManager == this )
		{
			phase->requestTerminate();
		}
		else
		{
			GC_EXCEPTION << "Tried to request termination to a non registered Phase! Name requested : " << phase->name();
		}

	}

	void PhaseManager::unregisterAllPhases()
	{
		// we use this heavy algorithm to make sure the the phases that unregister other phases will be fine.
		while( m_phaseIndex.begin() != m_phaseIndex.end() )
		{
			const String phaseName = m_phaseIndex.begin()->first;
			unregisterPhase( phaseName );
		}
	}


	PhasePtr PhaseManager::findPhase( const String& phaseName )
	{
		PhaseIndex::iterator it = m_phaseIndex.find( phaseName );

		if( it != m_phaseIndex.end() ) 
		{
			return it->second;
		}
		else
		{
			return PhasePtr();
		}
	}

	gcore::PhasePtr PhaseManager::findRegisteredPhase( const String& phaseName )
	{
		const PhasePtr phase = findPhase( phaseName );

		if( phase.get() == nullptr )
		{
			GC_EXCEPTION << "Phase \"" << phaseName << "\" not found in phase manager!" ;
		}

		return phase;
	}

	std::vector< String > PhaseManager::getRegisteredPhaseNames() const
	{
		const std::size_t nameCount = m_phaseIndex.size();
		std::vector< String > resultList( nameCount );

		std::size_t i = 0;
		PhaseIndex::const_iterator it = m_phaseIndex.begin();
		for( ; it != m_phaseIndex.end(); ++it )
		{
			resultList[ i ] = it->first;
			++i;
		}

		return resultList;
	}

}