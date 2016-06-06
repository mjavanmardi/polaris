///----------------------------------------------------------------------------------------------------
/// Simulation_Engine.cpp - Entry point for discrete event engine
///----------------------------------------------------------------------------------------------------

#include "World.h"

namespace polaris
{

///----------------------------------------------------------------------------------------------------
/// Initialize - Construct Simulation Engine
///----------------------------------------------------------------------------------------------------

void Simulation_Engine::Initialize()
{
	_ex_next_revision._iteration = 0;
	_ex_next_revision._sub_iteration = 0;

	UNLOCK(_ex_lock);
	UNLOCK(_active_types_lock);
}

///----------------------------------------------------------------------------------------------------
/// Activate_Type - move the clock to the next simulation event
///----------------------------------------------------------------------------------------------------

void Simulation_Engine::Activate_Type(Execution_Component_Manager_Base* type_to_activate)
{
	LOCK(_active_types_lock);

		_queued_active_types.push_back( *type_to_activate );

	UNLOCK(_active_types_lock);
}

///----------------------------------------------------------------------------------------------------
/// Step - process one iteration of the simulation
///----------------------------------------------------------------------------------------------------

void Simulation_Engine::Step(Revision& out_next_revision)
{
	// Thread logs an overly conservative standing guess for when the next event should happen
	Revision ex_proposed_next_revision = __revision_omega;

	// log the current revision as a const
	const Revision this_revision = revision();

	// loop over the types which are active this event step
	for(boost::intrusive::list< Execution_Component_Manager_Base >::iterator itr=_active_types.begin();itr!=_active_types.end();itr++)
	{
		Execution_Component_Manager_Base& execution_type=(*itr);

		// Thread logs the TEX's current guess as an initial guess to report back to the EX
		// If it is the current iteration, it must be overwritten
		Revision tex_proposed_next_revision = execution_type.tex_next_revision();

		// Visit the TEX if it has requested to go this step
		if(tex_proposed_next_revision == this_revision)
		{
			// process one slice of the TEX
			 execution_type.Step(tex_proposed_next_revision);
		}

		// Here locking is not necessary because local copies are being updated
		if(tex_proposed_next_revision < ex_proposed_next_revision)
		{
			#ifdef FORWARD_SIMULATION_MODE
				if(tex_proposed_next_revision <= revision())
				{
					THROW_EXCEPTION("Core Error allowing reschedule by TEX: " << execution_type.name() << " at step: " << iteration() << "," << sub_iteration() << " to step: " << tex_proposed_next_revision._iteration << "," << tex_proposed_next_revision._sub_iteration);
				}
			#endif

			// TEX has indicated that it wishes to go sooner than proposed, update local copy
			ex_proposed_next_revision = tex_proposed_next_revision;
		}
	}

	// Here locking is necessary because there are cases where you may overwrite the true minimum proposed revision
	// lock and get minimum estimate from each thread of next_revision over all execution types
	LOCK(_ex_lock);
		
		if(ex_proposed_next_revision < _ex_next_revision)
		{
			// EX slice has revealed that it wishes to go sooner than proposed, update EX copy
			_ex_next_revision = ex_proposed_next_revision;
		}

		out_next_revision = _ex_next_revision;

	UNLOCK(_ex_lock);
}

///----------------------------------------------------------------------------------------------------
/// Collect_Core_Diagnostics - Print User-Specified Core Diagnostics
///----------------------------------------------------------------------------------------------------

void Simulation_Engine::Collect_Core_Diagnostics()
{
	//MESSAGE("======================= " << __iteration << " =======================");

	//unsigned long long memory_allocated = 0;

	//for(std::vector<Component_Manager_Base*>::iterator itr=__all_components.begin();itr!=__all_components.end();itr++)
	//{
	//	if((*itr)->Component_Manager_Type() == EXECUTION_MANAGER)
	//	{

	//		//for(int i=0;i<__num_sim_threads;i++)
	//		//{
	//		//	
	//		//}
	//	}
	//}

	//DEBUG_MESSAGE("\tCore Memory Allocated: " << memory_allocated);
}

///----------------------------------------------------------------------------------------------------
/// Update - move the clock to the next simulation event - point at which time advances in the simulation - by updating work->revision.
///----------------------------------------------------------------------------------------------------

void Simulation_Engine::Update()
{
	// activate new types
	while( _queued_active_types.size() )
	{
		Execution_Component_Manager_Base* ptr = &_queued_active_types.front();

		_queued_active_types.pop_front();

		_active_types.push_back(*ptr);
	}

	// you should be guaranteed at this point that all simulation threads are tied up

	_world->_revision = _ex_next_revision;
	_ex_next_revision = __revision_omega;

	Collect_Core_Diagnostics();
}

}