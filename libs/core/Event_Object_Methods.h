#pragma once
///----------------------------------------------------------------------------------------------------
/// Event_Object_Methods.h - Top level execution object which records type_id, revision, and memory information
///----------------------------------------------------------------------------------------------------

#include "Event_Component_Manager_Methods.h"

namespace polaris
{
	/*
		Only important fact for rescheduling is whether you are pre-update or post-update
		if you are pre-update, then you will not update because all update statements are enclosed in locks
		if you are pre-update, then you are treated identically to a thread updating you in parallel
		if you are post-update, then you must replace the _next_revision and _last_update_revision if you beat them
			I do not believe (but am not 100% confident) that you need to cache the thread's _next_revsion as _next_next_revision
	*/

	///----------------------------------------------------------------------------------------------------
	/// Load_Event - Load an event callback and schedule to an event step
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Event_Object::Load_Event(void (*p_event)(DataType*,Event_Response&),int start_iteration,int start_sub_iteration)
	{
		const Revision update_revision(start_iteration,start_sub_iteration);
		
		#ifdef FORWARD_SIMULATION_MODE
			if(update_revision <= revision() && update_revision != FREE && update_revision._revision != 0)
			{
				THROW_EXCEPTION("Simulation specified as Forward Simulation, error loading object: " << typeid(DataType).name() << " at step: " << iteration() << "," << sub_iteration() << " to step: " << update_revision._iteration << "," << update_revision._sub_iteration);
			}
		#endif

		if( _world->Is_Running() )
		{
			// Update the EX
			_world->simulation_engine()->Update_Schedule(update_revision);
			
			// Update the TEX
			DataType::component_manager->Update_Schedule(update_revision);

			// Update the PTEX
			_execution_block->Update_Schedule(update_revision);

			// If safeties are enabled, mitigate an unsafe rescheduling (object which is going this event step is being rescheduled by an external thread)
			#ifdef SAFE_MODE
				LOCK(_optex_lock);
			#elif defined ENABLE_WARNINGS
				if( Visiting() && _execution_block->thread_processing()!=__thread_id )
				{
					THROW_WARNING("Unreliable Loading of object: " << typeid(DataType).name() << " at step: " << iteration () << "," << sub_iteration());
				}
			#endif

			_next_revision = update_revision;
			_event_callback = (Event)p_event;
			
			#ifdef SAFE_MODE
				UNLOCK(_optex_lock);
			#endif
		}
		else
		{
			// Update the EX
			if(update_revision < _world->simulation_engine()->ex_next_revision())
			{
				_world->simulation_engine()->ex_next_revision(update_revision);
			}

			// Update the TEX
			if(update_revision < DataType::component_manager->tex_next_revision())
			{
				DataType::component_manager->tex_next_revision(update_revision);
			}

			// Update the PTEX
			if(update_revision < _execution_block->ptex_next_revision())
			{
				_execution_block->ptex_next_revision(update_revision);
			}
			
			// Update the OPTEX
			_next_revision = update_revision;
			_event_callback = (Event)p_event;
		}

		_execution_block->Update_Object_Schedule(this);
	}

	///----------------------------------------------------------------------------------------------------
	/// Reschedule - Reschedule the agent
	///----------------------------------------------------------------------------------------------------

//	template<typename DataType>
//	void Event_Object::Reschedule(int start_iteration,int start_sub_iteration)
//	{
//		const Revision starting_revision(start_iteration,start_sub_iteration);
//
//		#ifdef FORWARD_SIMULATION_MODE
//			if(starting_revision <= revision() && starting_revision != FREE)
//			{
//				THROW_EXCEPTION("Simulation specified as Forward Simulation, error rescheduling object: " << typeid(DataType).name() << " at step: " << iteration() << "," << sub_iteration() << " to step: " << starting_revision._iteration << "," << starting_revision._sub_iteration);
//			}
//		#endif
//
//		if( _world->Is_Running() )
//		{
//			// Update the EX
//			_world->simulation_engine()->Update_Schedule(starting_revision);
//
//			// Update the TEX
//			DataType::component_manager->Update_Schedule(starting_revision);
//
//			// Update the PTEX
//			_execution_block->Update_Schedule(starting_revision);
//
//			// If safeties are enabled, mitigate an unsafe rescheduling (object which is going this event step is being rescheduled by an external thread)
//			#ifdef SAFE_MODE
//				LOCK(_optex_lock);
//			#elif defined ENABLE_WARNINGS
//				if( Visiting() && _execution_block->_thread_processing!=__thread_id )
//				{
//					THROW_WARNING("Unreliable Rescheduling of object: " << typeid(DataType).name() << " at step: " << iteration () << "," << sub_iteration());
//				}
//			#endif
//
//			next_revision = update_revision;
//
//			#ifdef SAFE_MODE
//				UNLOCK(_optex_lock);
//			#endif
//		}
//		else
//		{
//			// Update the EX
//			if(update_revision < _world->simulation_engine()->_ex_next_revision)
//			{
//				_world->simulation_engine()->ex_next_revision(update_revision);
//			}
//
//			// Update the TEX
//			if(update_revision < DataType::component_manager->_tex_next_revision)
//			{
//				DataType::component_manager->tex_next_revision(update_revision);
//			}
//
//			// Update the PTEX
//			if(update_revision < _execution_block->_ptex_next_revision)
//			{
//				_execution_block->ptex_next_revision(update_revision);
//			}
//
//			_next_revision = update_revision;
//		}
//
//		_execution_block->Update_Object_Schedule(this);
//	}
}