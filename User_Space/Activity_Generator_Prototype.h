#pragma once

#include "User_Space_Includes.h"
//#include "Person_Prototype.h"


//---------------------------------------------------------
//	Activity Generator - Handles Activity Generation for the Planner Class
//---------------------------------------------------------

namespace Person_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Activity_Generator ADD_DEBUG_INFO
		{
			tag_as_prototype;
		 
			// Conditonal handling and helper functions
			declare_feature_conditional(Generator_Conditional)
			{
				//----------------------------------------------
				// CONDITIONALS FOR BASIC ACTIVITY GENERATION
				// 1.) Activity Generation (Occurs 
				typedef Activity_Generator<ComponentType, ComponentType> _Generator_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Generator_Interface* this_ptr=(_Generator_Interface*)_pthis;

				//------------------------------------------------------------------------------------------------------------------------------
				// ACTIVITY GENERATION SUBITERATION, swap in the activity-generation event and set up future sub_iteration schedule
				if (_sub_iteration == Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_GENERATION)
				{
					this_ptr->Go_To_Next_Iteration<NT>(true,response);
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// No valid events scheduled - skip to next iteration
				else
				{
					this_ptr->Go_To_Next_Iteration<NT>(false,response);
				}

				// set next planning time for other functions to use
				this_ptr->Next_Activity_Generation_Time<Simulation_Timestep_Increment>(_iteration + this_ptr->Generation_Time_Increment<Simulation_Timestep_Increment>());
			}
			feature_prototype void Go_To_Next_Iteration(bool do_current_event, Conditional_Response& response)
			{
				response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this->Generation_Time_Increment<Simulation_Timestep_Increment>()));
				response.next._sub_iteration = Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_GENERATION;
				response.result = do_current_event;
			}
		

			// Event handling
			declare_feature_event(Activity_Generation_Event)
			{
				// Create alias for this to use in conditional
				typedef Activity_Generator<ComponentType, ComponentType> _Generator_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Generator_Interface* this_ptr=(_Generator_Interface*)_pthis;

				// Call specific implementation of the activity generation routine
				this_ptr->template Activity_Generation<TargetType>();

				// set next activity generation occurence
				this_ptr->template Next_Activity_Generation_Time<Simulation_Timestep_Increment>(Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template Generation_Time_Increment<Simulation_Timestep_Increment>())));
			}

			define_feature_exists_check(Initialize,Has_Initialize);
			feature_prototype void Initialize(requires(check(ComponentType, Has_Initialize)))
			{
				define_component_interface(planner_itf,typename get_type_of(Parent_Planner),Prototypes::Person_Planner,ComponentType);
				define_component_interface(person_itf,typename planner_itf::get_type_of(Parent_Person),Prototypes::Person,ComponentType);
				person_itf* parent = this->Parent_Planner<planner_itf*>()->Parent_Person<person_itf*>();
				
				float random_number = 1.0f; //GLOBALS::Uniform_RNG<float>();
				long first_iter = parent->template First_Iteration<Simulation_Timestep_Increment>() + (Simulation_Timestep_Increment)(random_number*60.0f);
				this_component()->Initialize<ComponentType,CallerType, TargetType>();
				load_event(ComponentType,Generator_Conditional,Activity_Generation_Event,first_iter,Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_GENERATION,NULLTYPE);
			}
			feature_prototype void Initialize(requires(!check(ComponentType,Has_Initialize)))
			{
				assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
			}


			feature_prototype void Activity_Generation()
			{
				this_component()->Activity_Generation<ComponentType,CallerType,TargetType>();
			}

			// indicators used to determine next iteration for generation/planning events
			feature_accessor(Next_Activity_Generation_Time,none,none);
			feature_accessor(Generation_Time_Increment,none,none);

			// accessor to parent class
			feature_accessor(Parent_Planner,none,none);
		};
	}

}