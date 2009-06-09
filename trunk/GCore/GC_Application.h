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
		enum State
		{
			/// The application have not been initialized before and has not run.
			State_Ready			,	
			/// Initialization is in progress.
			State_Initialisation	,
			/// Running the main Loop (in Run() ).
			State_Running,		
			///Termination is in progress.
			State_Termination,	
			///The application has been run and terminated.
			State_Finished	,	
		};

		/// Current state of the application.
		State getState() const {return m_state;}

		bool isRunning() const { return m_state == State_Running; }
		
		/// Application name.
		const String& getName() const {return m_name;}

		/// Provide text version informations.
		virtual String getVersionName() const { return "Undefined"; }

		/// Provide text version build infos.
		virtual String getBuildInfos() const { return "Undefined";}

		/** Constructor.
			@param name Name of the application.
		*/
		Application(const String& name = "...");

		/** Destructor.
		*/
		virtual ~Application();

		//////////////////////////////////////////////////////////////////////////


		/** Start the application and go through the main loop until end() is called.
			This method will : 
			- call initialize() for user defined initialization;
			- start a loop that will call mainLoop() each cycle;
			- end the loop when State == State_Termination, by calling end() for example;
			- call terminate() for user defined application termination;
			@remark If initialize(), terminate() or mainLoop() fail by returning anything else than 0, 
			this function will just return the failed function return value. No termination() will be
			called anymore.
			@return Non-zero value if a problem occured while initialization, mainLoop or termination.
		*/
		virtual int run();

		/** End the application by setting it's state to State_Termination.
			@remark Only if it's state is State_Running (Else will not do anything).
			The main loop will then stop, call termination functions and end, 
			as explained in run() function.
			@see run
		*/
		virtual void end();

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
			@remark Use end() to end the main loop.
			@return Must return 0 on success, anything else on failure.
		*/
		virtual int mainLoop() = 0;


	private:

		/// Application name (and name of the application's window).
		const String m_name;

		/// Current state of the application.
		State m_state;	

	};

}

#endif