#include "GC_Application.h"

namespace gcore
{
	Application::Application(const String& name)
		: m_name(name)
		, m_state(State_Ready)
	{
		
	}

	Application::~Application()
	{

	}


	int Application::run()
	{

		int result;

		//If the application is in another state than State_Ready or State_Finished, we cannot run it!
		if(m_state != State_Ready && m_state != State_Finished)
		{
			GC_EXCEPTION << "Tried to run a non ready application!";
		}

		//Start Initialization
		m_state = State_Initialisation;

		
		//User defined initialization:
		result = initialize();
		if(result != 0)
		{
			//System initialization failed!
			return result;
		}

		//Start Main loop:
		m_state = State_Running;
		
		/*
			Main loop:
			While m_State is not changed (by calling end()) and mainLoop return 0,
			we just call mainLoop each cycle.			
		*/
		while ( m_state == State_Running )
		{
			result = mainLoop();
			if( result != 0 )
				return result;	
		}

		//end() set m_State to State_Termination so we don't have to set it again

		//User defined termination:
		result = terminate();
		if(result != 0)
		{
			//User defined initialization failed!
			return result;
		}

		//All went right : return 0.
		return 0;

	}

	void Application::end()
	{
		if(m_state==State_Running)
		{
			m_state=State_Termination;
		}
	}


}