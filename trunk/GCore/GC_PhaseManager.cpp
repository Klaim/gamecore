#include "GC_PhaseManager.h"
#include "GC_Phase.h"

namespace gcore
{
	PhaseManager::PhaseManager()
	{

	}

	PhaseManager::~PhaseManager()
	{

	}

	void PhaseManager::registerPhase( const PhasePtr& phase )
	{
		GC_ASSERT( phase.get() != nullptr, "Tried to register a null phase!" );
		GC_ASSERT( getRegisteredPhase( phase->getName() ).get() == nullptr, String( "Tried to register an already registered Phase! Phase Name : " ) + phase->getName() );
		
		m_phaseIndex[ phase->getName() ] = phase;

	}

	void PhaseManager::unregisterPhase( const String& phaseName )
	{
		std::map< String , PhasePtr >::iterator it = m_phaseIndex.find( phaseName );
		GC_ASSERT( it != m_phaseIndex.end(), "Tried to unregister a non registered Phase!" );

		m_phaseIndex.erase( it );
	}

	void PhaseManager::requestLoadPhase( const String& phaseName )
	{
		PhasePtr phase = getRegisteredPhase( phaseName );
		
		if( phase.get() != nullptr )
		{
			phase->requestLoad();
		}
		else
		{
			GC_EXCEPTION( String("Tried to request load to a non registered Phase! Name requested : ") + phaseName );
		}

	}

	void PhaseManager::requestUnloadPhase( const String& phaseName )
	{
		PhasePtr phase = getRegisteredPhase( phaseName );

		if( phase.get() != nullptr )
		{
			phase->requestUnload();
		}
		else
		{
			GC_EXCEPTION( String("Tried to request unload to a non registered Phase! Name requested : ") + phaseName );
		}

	}

	void PhaseManager::requestActivatePhase( const String& phaseName )
	{
		PhasePtr phase = getRegisteredPhase( phaseName );

		if( phase.get() != nullptr )
		{
			phase->requestActivate();
		}
		else
		{
			GC_EXCEPTION( String("Tried to request activation to a non registered Phase! Name requested : ") + phaseName );
		}

	}

	void PhaseManager::requestTerminatePhase( const String& phaseName )
	{
		PhasePtr phase = getRegisteredPhase( phaseName );

		if( phase.get() != nullptr )
		{
			phase->requestTerminate();
		}
		else
		{
			GC_EXCEPTION( String("Tried to request termination to a non registered Phase! Name requested : ") + phaseName );
		}

	}

	void PhaseManager::unregisterAllPhases()
	{
		m_phaseIndex.clear();
	}

	PhasePtr PhaseManager::getRegisteredPhase( const String& phaseName )
	{
		std::map< String , PhasePtr >::iterator it = m_phaseIndex.find( phaseName );

		if( it != m_phaseIndex.end() ) 
		{
			return it->second;
		}
		else
		{
			return PhasePtr();
		}
	}

	std::vector< String > PhaseManager::getRegisteredPhaseNames() const
	{
		const std::size_t nameCount = m_phaseIndex.size();
		std::vector< String > resultList( nameCount );

		std::size_t i = 0;
		std::map< String , PhasePtr >::const_iterator it = m_phaseIndex.begin();
		for( ; it != m_phaseIndex.end(); ++it )
		{
			resultList[ i ] = it->first;
			++i;
		}

		return resultList;
	}
}