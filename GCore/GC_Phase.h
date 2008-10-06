#ifndef GC_PHASE_H
#define GC_PHASE_H
#pragma once

#include <list>
#include <vector>

#include "GC_Common.h"

namespace gcore
{
	/// State of a Phase
	enum PhaseState
	{
		/// The phase is unloaded.
		PS_UNLOADED,
		/// The phase has been requested to load.
		PS_LOADING,
		/// The phase is loaded and ready to start.
		PS_LOADED,
		/// The phase has been requested to unload.
		PS_UNLOADING,
		/// The phase has been requested to activate.
		PS_ACTIVATING,
		/// The phase is currently active.
		PS_ACTIVE,
		/// The phase has been requested to terminate.
		PS_TERMINATING,
	};

	class Phase;

	/** Listener class that is notified when a phase it is registered in change it's state.
	*/
	class PhaseListener
	{
	public:
		PhaseListener(){}
		virtual ~PhaseListener(){}

		/** User defined behavior called when a phase it is registered in change it's state.
		*/
		virtual void onPhaseStateChanged( Phase& phase ) = 0;
	};

	/** Base class for cumulative states : state that can be active with others at the same time.
		Useful to organize game states while managing transition between states smoothly.
		@remark
		Note that the child class have to manage it's own state. It will be requested to change state
		but have to do it itself. This is to let the user class manage how to do the transition between states.
	*/
	class GCORE_API Phase
	{
	public:

		/** Constructor.
			@param name Name of the phase
		*/
		Phase( const String& name );

		/** Destructor.
		*/
		virtual ~Phase();

		/// Current state of this phase.
		PhaseState getState() const { return m_state; }

		/// Name of this phase.
		const String& getName() const { return m_name; }

		/** Request this phase to load.
			The state of this phase will become PS_LOADING and
			the user defined behavior for loading will be called
			( onLoadRequested ).
			@see onLoadRequested
			@remark This phase have to be unloaded!
		*/
		void requestLoad();

		/** Request this phase to activate.
			The state of this phase will become PS_ACTIVATING and
			the user defined behavior for activation will be called
			( onActivateRequested ).
			@see onActivateRequested
			@remark This phase have to be loaded!
		*/
		void requestActivate();

		/** Request this phase to terminate.
			The state of this phase will become PS_TERMINATNG and
			the user defined behavior for termination will be called
			( onLoadRequested ).
			@see onTerminateRequested
			@remark This phase have to be active!
		*/
		void requestTerminate();

		/** Request this phase to unload.
			The state of this phase will become PS_UNLOADING and
			the user defined behavior for unloading will be called
			( onUnloadRequested ).
			@see onUnloadRequested
			@remark This phase have to be loaded!
		*/
		void requestUnload();

		/** Register a listener to be notified when this phase changes it's state.
		*/
		void registerListener( PhaseListener* phaseListener );

		/** Unregister a listener.
		*/
		void unregisterListener( PhaseListener* phaseListener );

		/** Unregister all listeners.
		*/
		void unregisterAllListeners();


	protected:



		/** User defined management of the load request.
			@return True if the transition to loaded state is valid and immediate. False if not : 
			the user have to manage the state change as he wish.
		*/
		virtual bool onLoadRequested() = 0;

		/** User defined management of the load request.
			@return True if the transition to unloaded state is valid and immediate. False if not : 
			the user have to manage the state change as he wish.
		*/
		virtual bool onUnloadRequested() = 0;
		
		/** User defined management of the load request.
			@return True if the transition to active state is valid and immediate. False if not : 
			the user have to manage the state change as he wish.
		*/
		virtual bool onActivateRequested() = 0;
		
		/** User defined management of the load request.
			@return True if the transition to loaded state is valid and immediate. False if not : 
			the user have to manage the state change as he wish.
		*/
		virtual bool onTerminateRequested() = 0;

		/** Change the state of this phase.
			Calling this method notify registered listeners of the changed state.
		*/
		void changeState( PhaseState newState ); 
		
	private:

		/// Current state of this phase.
		PhaseState m_state;

		/// Name of this phase.
		const String m_name;

		/// Registered listeners list.
		std::list< PhaseListener* > m_listenerList;

		/// Listener list used on notification 
		std::vector< PhaseListener* > m_notificationList;

		/// True if the listener list has changed since last notification list update
		bool m_listenerListChanged;
	
	};
	

}

#endif