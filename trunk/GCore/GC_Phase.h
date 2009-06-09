#ifndef GC_PHASE_H
#define GC_PHASE_H
#pragma once

#include <list>
#include <vector>

#include <memory>

#include "GC_Common.h"

namespace gcore
{
	class PhaseManager;

	/** Base class for (cumulative) states : state that can be active with others at the same time.
		Useful to organize game states while managing transition between states smoothly.
		@remark
		Note that the child class have to manage it's own state. It will be requested to change state
		but have to do it itself. This is to let the user class manage how to do the transition between states.
	*/
	class GCORE_API Phase
	{
	public:
		
		/// State of a Phase
		enum State
		{
			/// The phase is unloaded.
			UNLOADED,
			/// The phase has been requested to load.
			LOADING,
			/// The phase is loaded and ready to start.
			LOADED,
			/// The phase has been requested to unload.
			UNLOADING,
			/// The phase has been requested to activate.
			ACTIVATING,
			/// The phase is currently active.
			ACTIVE,
			/// The phase has been requested to terminate.
			TERMINATING,
		};

		/** Listener class that is notified when a phase it is registered in change it's state.
		*/
		class Listener
		{
		public:
			virtual ~Listener(){}

			/** User defined behavior called when a phase it is registered in change it's state.
			*/
			virtual void onPhaseStateChanged( Phase& phase, const Phase::State previousState ) = 0;
		};

		/** Constructor.
			@param name Name of the phase
		*/
		Phase( const String& name );

		/** Destructor.
		*/
		virtual ~Phase(){}

		/// Current state of this phase.
		const Phase::State& getState() const { return m_state; }

		/// Name of this phase.
		const String& getName() const { return m_name; }


		/** Request this phase to load.
			The state of this phase will become Phase::LOADING and
			the user defined behavior for loading will be called
			( onLoadRequested ).
			@see onLoadRequested
			@remark This phase have to be unloaded!
		*/
		void requestLoad();

		/** Request this phase to activate.
			The state of this phase will become Phase::ACTIVATING and
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
			The state of this phase will become Phase::UNLOADING and
			the user defined behavior for unloading will be called
			( onUnloadRequested ).
			@see onUnloadRequested
			@remark This phase have to be loaded!
		*/
		void requestUnload();

		/** Register a listener to be notified when this phase changes it's state.
		*/
		void registerListener( Phase::Listener* phaseListener );

		/** Unregister a listener.
		*/
		void unregisterListener( Phase::Listener* phaseListener );

		/** Unregister all listeners.
		*/
		void unregisterAllListeners();

		/** True if this phase is currently managed by a scene manager. */
		bool isManaged() const { return m_phaseManager != nullptr; }

		/** Phase manager this phase is currently registered in, or null if not registered yet.
		*/
		PhaseManager& getPhaseManager() 
		{ 
			GC_ASSERT( m_phaseManager != nullptr, "Tried to get the phase manager of a non-managed phase!" ); 
			return *m_phaseManager;
		}

		const PhaseManager& getPhaseManager() const
		{ 
			GC_ASSERT( m_phaseManager != nullptr, "Tried to get the phase manager of a non-managed phase!" ); 
			return *m_phaseManager;
		}

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

		/** User defined process to perform when this phase have just been registered in a phase manager.
		*/
		// obsolete
		//virtual void onRegistered(){ GC_ASSERT( m_phaseManager != nullptr, "Registered in a null phase manager!" ); }

		/** User defined process to perform when just before this phase will be unregistered from it's phase manager.
		*/
		// obsolete
		//virtual void onUnregister(){ GC_ASSERT( m_phaseManager != nullptr, "Unregister from a null phase manager!" ); }

		/** Change the state of this phase.
			Calling this method notify registered listeners of the changed state.
		*/
		void changeState( Phase::State newState ); 
		
	private:

		friend class PhaseManager; // managed by a phase manager

		/// Phase manager this phase is currently registered in, or null if not registered yet.
		PhaseManager* m_phaseManager;

		/// Current state of this phase.
		Phase::State m_state;

		/// Name of this phase.
		const String m_name;

		/// Registered listeners list.
		std::list< Phase::Listener* > m_listenerList;

		/// Listener list used on notification 
		std::vector< Phase::Listener* > m_notificationList;

		/// True if the listener list has changed since last notification list update
		bool m_listenerListChanged;

		// non copyable
		Phase(const Phase& ohterPhase);
		void operator=(const Phase& ohterPhase); 
	
	};

	/// Managed pointer type.
	typedef  std::tr1::shared_ptr< Phase > PhasePtr;

}

#endif