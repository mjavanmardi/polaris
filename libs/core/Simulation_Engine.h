#pragma once
///----------------------------------------------------------------------------------------------------
/// Simulation_Engine.h - Entry point for discrete event engine
///----------------------------------------------------------------------------------------------------

#include "Execution_Component_Manager.h"
#include "Event_Component_Manager.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// Simulation_Engine - Entry point for discrete event engine
	///----------------------------------------------------------------------------------------------------

	class Simulation_Engine
	{
	public:
		Simulation_Engine():_active_types(),_queued_active_types(){}

		void Initialize();
		void Activate_Type(Execution_Component_Manager_Base* type_to_activate);

		//----------------------------------------------------------------------------------------------------
		// Simulation functions
		//----------------------------------------------------------------------------------------------------

		void Step(Revision& out_next_revision);
		void Update();

		inline void Update_Schedule(const Revision& update_revision)
		{
			// Following makes EX aware, can catch EX in 2 states which are handled identically: A) Pre-Update B) Mid-Update

			LOCK(_ex_lock);

				if(update_revision < _ex_next_revision)
				{
					// This update is acceptable as it is indistinguishable from a different thread updating normally
					_ex_next_revision = update_revision;
				}

			UNLOCK(_ex_lock);
		}

		void Collect_Core_Diagnostics();
		
		//----------------------------------------------------------------------------------------------------
		// Accessor functions
		//----------------------------------------------------------------------------------------------------

		const Revision& ex_next_revision(){ return _ex_next_revision; }
		void ex_next_revision(const Revision& value){ _ex_next_revision = value; }

	private:
		_lock _ex_lock;

		boost::intrusive::list<Execution_Component_Manager_Base> _active_types;

		_lock _active_types_lock;

		boost::intrusive::list<Execution_Component_Manager_Base> _queued_active_types;

		Revision _ex_next_revision;
	};
}