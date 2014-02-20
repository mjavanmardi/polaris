#pragma once

#include "Activity_Simulator_Includes.h"
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
			static void Generator_Conditional(ComponentType* _this,Event_Response& response)
			{
				//----------------------------------------------
				// CONDITIONALS FOR BASIC ACTIVITY GENERATION
				// 1.) Activity Generation (Occurs 
				typedef Activity_Generator<ComponentType> _Generator_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Generator_Interface* this_ptr=(_Generator_Interface*)_pthis;

				//------------------------------------------------------------------------------------------------------------------------------
				// ACTIVITY GENERATION SUBITERATION, swap in the activity-generation event and set up future subiteration() schedule
				if (sub_iteration() == Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_GENERATION)
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
				this_ptr->Next_Activity_Generation_Time<Simulation_Timestep_Increment>(iteration() + this_ptr->Generation_Time_Increment<Simulation_Timestep_Increment>());
			}
			template<typename TargetType> void Go_To_Next_Iteration(bool do_current_event, Event_Response& response)
			{
				response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this->Generation_Time_Increment<Simulation_Timestep_Increment>()));
				response.next._sub_iteration = Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_GENERATION;
				response.result = do_current_event;
			}
		

			// Event handling
			declare_event(Activity_Generation_Event)
			{
				// Create alias for this to use in conditional
				typedef Activity_Generator<ComponentType> _Generator_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Generator_Interface* this_ptr=(_Generator_Interface*)_pthis;

				// Call specific implementation of the activity generation routine
				this_ptr->template Activity_Generation<TargetType>();

				// set next activity generation occurence
				this_ptr->template Next_Activity_Generation_Time<Simulation_Timestep_Increment>(Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template Generation_Time_Increment<Simulation_Timestep_Increment>())));
			}

			local_check_template_method_name(Has_Initialize,Initialize);
			template<typename TargetType> void Initialize(requires(TargetType,check(ComponentType, Has_Initialize)))
			{
				typedef Prototypes::Person_Planner<typename get_type_of(Parent_Planner)> planner_itf;
				typedef Prototypes::Person<typename planner_itf::get_type_of(Parent_Person)> person_itf;
				person_itf* parent = this->Parent_Planner<planner_itf*>()->Parent_Person<person_itf*>();
				
				float random_number = 1.0f; //GLOBALS::Uniform_RNG<float>();
				long first_iter = parent->template First_Iteration<Simulation_Timestep_Increment>() + (Simulation_Timestep_Increment)(random_number*60.0f);
				this_component()->Initialize< TargetType>();
				//TODO
//load_event(ComponentType,Generator_Conditional,Activity_Generation_Event,first_iter,Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_GENERATION,NULLTYPE);
			}
			template<typename TargetType> void Initialize(requires(TargetType,!check(ComponentType,Has_Initialize)))
			{
				assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
			}


			template<typename TargetType> void Activity_Generation()
			{
				this_component()->Activity_Generation<TargetType>();
			}

			// indicators used to determine next iteration for generation/planning events
			accessor(Next_Activity_Generation_Time, NONE, NONE);
			accessor(Generation_Time_Increment, NONE, NONE);

			// accessor to parent class
			accessor(Parent_Planner, NONE, NONE);
		};
	}

}
