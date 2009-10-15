#include "GC_Task.h"
#include "GC_TaskManager.h"

namespace gcore
{
	
	Task::Task( TaskPriority priority /*= 0 */,const String& name /*= "" */ ) : m_name( name ), 
		m_priority( priority ), 
		m_state( TS_UNREGISTERED ),
		m_taskManager( nullptr )
	{

	}

	Task::~Task()
	{
		// this protection is to make sure we never have pointer to destroyed task
		// in a task manager.
		// This test is not ideal but should warn the user about ninja task manipulations
		// occurence. 
		//GC_ASSERT ( m_taskManager == nullptr && m_state == TS_UNREGISTERED , String("Task destroyed while registered in a TaskManager! Task name :") + m_name );
		
	}

	void Task::activate()
	{
		GC_ASSERT( m_taskManager != nullptr , "Tried to activate an unregistered Task : null TaskManager !" );  
		GC_ASSERT( m_state != TS_UNREGISTERED , "Inconsistent state !" );  
		m_taskManager->activateTask( this );
	}

	void Task::pause()
	{
		GC_ASSERT( m_taskManager != nullptr , "Tried to pause an unregistered Task! : null TaskManager !" );  
		GC_ASSERT( m_state != TS_UNREGISTERED , "Inconsistent state !" );  
		m_taskManager->pauseTask( this );
	}

	void Task::resume()
	{
		GC_ASSERT( m_taskManager != nullptr , "Tried to pause an resume Task! : null TaskManager !" );  
		GC_ASSERT( m_state != TS_UNREGISTERED , "Inconsistent state !" );  
		m_taskManager->resumeTask( this );
	}

	void Task::terminate()
	{
		GC_ASSERT( m_taskManager != nullptr , "Tried to terminate an unregistered Task! : null TaskManager !" );  
		GC_ASSERT( m_state != TS_UNREGISTERED , "Inconsistent state !" );  
		m_taskManager->terminateTask( this );
	}

	void Task::unregister()
	{
		GC_ASSERT( m_taskManager != nullptr , "Tried to unregister an unregistered Task! : null TaskManager !" );  
		GC_ASSERT( m_state != TS_UNREGISTERED , "Inconsistent state !" );  
		m_taskManager->unregisterTask( this );
	}

	void Task::priority( const TaskPriority& priority )
	{
		if( m_taskManager != nullptr ) m_taskManager->changeTaskPriority( this, priority );
		else m_priority = priority;
	}
}