#ifndef GCORE_APPLICATION_H
#define GCORE_APPLICATION_H
#pragma once

#include "GC_Common.h"

namespace gcore
{

	/** Base Class for Application Class.
		
	*/
	class GCORE_API Application 
	{
	public:
		/** Current state of application. */
		enum APPLICATION_STATE 
		{
			/// The application have not been initialized before and has not run.
			APPSTATE_READY			,	
			/// Initialization is in progress.
			APPSTATE_INITIALIZING	,
			/// Running the main Loop (in Run() ).
			APPSTATE_RUNNING,		
			///Termination is in progress.
			APPSTATE_TERMINATING,	
			///The application has been run and terminated.
			APPSTATE_FINISHED	,	
		};

		/// Current state of the application.
		APPLICATION_STATE getState() const {return m_state;}

		/// Application name.
		const String& getName() const {return m_name;}


		/** Constructor.
			@param name Name of the application.
		*/
		Application(const String& name = "...");

		/** Destructor.
		*/
		virtual ~Application();

		//////////////////////////////////////////////////////////////////////////


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
		virtual int run();

		/** End the application by setting it's state to APPSTATE_TERMINATING.
			@remark Only if it's state is APPSTATE_RUNNING (Else will not do anything).
			The main loop will then stop, call termination functions and exit, 
			as explained in run() function.
			@see run
		*/
		virtual void exit();

	protected:

		/** User defined Initialization.
			All user data initialization should be there.
			@return Must return 0 on success, anything else on failure.
		*/
		virtual int initialize() = 0;


		/** User defined Termination.
			All user data destruction should be there.
			@return Must return 0 on success, anything else on failure.
		*/
		virtual int terminate() = 0;

		/** User defined Main Loop.
			This should define the main loop process.
			@remark Use exit() to exit the main loop.
			@return Must return 0 on success, anything else on failure.
		*/
		virtual int mainLoop() = 0;


	private:

		/// Application name (and name of the application's window).
		String m_name;

		/// Current state of the application.
		APPLICATION_STATE m_state;	

	};

}

#endif