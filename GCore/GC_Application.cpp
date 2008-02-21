#include "GC_Application.h"

namespace gcore
{

	/** Constructor.
		@param name Name of the application.
	*/
	Application::Application(const String& name):
		m_name(name),m_state(APPSTATE_READY)
	{
		
	}


	/** Destructor.
	*/
	Application::~Application()
	{

	}


	/** Start the application and go through the main loop until exit() is called.
		This method will : 
		- call initialize() for user defined initialization;
		- start a loop that will call mainLoop() each cycle;
		- exit the loop when State == APPSTATE_TERMINATING, by calling exit() for example;
		- call terminate() for user defined application termination;
		@remark If initialize(), terminate() or mainLoop() fail by returning anything else than 0, 
		this function will just return the failed function return value. No termination() will be
		called anymore.
		@return Non-zero value if a problem occured while initialization, mainLoop or termination.
	*/
	int Application::run()
	{

		int result;

		//If the application is in another state than APPSTATE_READY or APPSTATE_FINISHED, we cannot run it!
		if(m_state != APPSTATE_READY && m_state != APPSTATE_FINISHED)
		{
			GC_EXCEPTION("Tried to run a non ready application!");
		}

		//Start Initialization
		m_state = APPSTATE_INITIALIZING;

		
		//User defined initialization:
		result = initialize();
		if(result != 0)
		{
			//System initialization failed!
			return result;
		}

		//Start Main loop:
		m_state = APPSTATE_RUNNING;
		
		/*
			Main loop:
			While m_State is not changed (by calling exit()) and mainLoop return 0,
			we just call mainLoop each cycle.			
		*/
		while ( m_state == APPSTATE_RUNNING )
		{
			result = mainLoop();
			if( result != 0 )
				return result;	
		}

		//exit() set m_State to APPSTATE_TERMINATING so we don't have to set it again

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

	/** End the application by setting it's state to APPSTATE_TERMINATING.
		@remark Only if it's state is APPSTATE_RUNNING (Else will not do anything).
		The main loop will then stop, call termination functions and exit, 
		as explained in run() function.
		@see run
	*/
	void Application::exit()
	{
		if(m_state==APPSTATE_RUNNING)
		{
			m_state=APPSTATE_TERMINATING;
		}
	}


}