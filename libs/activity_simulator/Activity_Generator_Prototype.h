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


			template<typename T> void Activity_Generation()
			{
				this_component()->Activity_Generation<T>();
			}

			//==========================================================================
			//TODO: the rest of the code below is not used as activity generation is 
			//		still currently controlled by the person_planner - needs restructuring
		 
			// Event Controller handling and helper functions
			static void Activity_Generation_Event_Controller(ComponentType* _this,Event_Response& response)
			{
				//----------------------------------------------
				// 1.) Activity Generation (Occurs 
				typedef Activity_Generator<ComponentType> _Generator_Interface;
				_Generator_Interface* this_ptr=(_Generator_Interface*)_this;

				//------------------------------------------------------------------------------------------------------------------------------
				// ACTIVITY GENERATION SUBITERATION, swap in the activity-generation event and set up future subiteration() schedule
				if (sub_iteration() == Scenario_Components::Types::ACTIVITY_GENERATION_SUB_ITERATION)
				{
					this_ptr->Activity_Generation_Event<NT>();
					response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->Generation_Time_Increment<Simulation_Timestep_Increment>()));
					response.next._sub_iteration = Scenario_Components::Types::ACTIVITY_GENERATION_SUB_ITERATION;
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// No valid events scheduled - skip to next iteration
				else
				{
					response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->Generation_Time_Increment<Simulation_Timestep_Increment>()));
					response.next._sub_iteration = Scenario_Components::Types::ACTIVITY_GENERATION_SUB_ITERATION;
				}

				// set next planning time for other functions to use
				this_ptr->Next_Activity_Generation_Time<Simulation_Timestep_Increment>(iteration() + this_ptr->Generation_Time_Increment<Simulation_Timestep_Increment>());
			}

		

			// Event handling
			template<typename TargetType> void Activity_Generation_Event()
			{
				// Call specific implementation of the activity generation routine
				this_component()->Activity_Generation<TargetType>();

				// set next activity generation occurence
				this->template Next_Activity_Generation_Time<Simulation_Timestep_Increment>(Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this->template Generation_Time_Increment<Simulation_Timestep_Increment>())));
			}

			local_check_template_method_name(Has_Initialize,Initialize);
			template<typename TargetType> void Initialize()
			{
				assert_check(ComponentType,Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");

				typedef Prototypes::Person_Planner<typename get_type_of(Parent_Planner)> planner_itf;
				typedef Prototypes::Person<typename planner_itf::get_type_of(Parent_Person)> person_itf;
				person_itf* parent = this->template Parent_Planner<planner_itf*>()->template Parent_Person<person_itf*>();
				
				float random_number = 1.0f; //GLOBALS::Uniform_RNG<float>();
				long first_iter = parent->template First_Iteration<Simulation_Timestep_Increment>() + (Simulation_Timestep_Increment)(random_number*60.0f);
				this_component()->Initialize< TargetType>();

				//load_event(ComponentType,Generator_Conditional,Activity_Generation_Event,first_iter,Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_GENERATION,NULLTYPE);
				((ComponentType*)this)->template Load_Event<ComponentType>(&Activity_Generation_Event_Controller,first_iter,Scenario_Components::Types::ACTIVITY_GENERATION_SUB_ITERATION);
			}
			template<typename TargetType> TargetType Create_Activity(TargetType act, requires(TargetType, check(TargetType, is_pointer) && check(strip_modifiers(TargetType), Activity_Simulator::Activity_Concepts::Is_Activity_Plan_Prototype)))
			{
				return this_component()->Create_Activity<TargetType>(act);
			}
			template<typename TargetType> TargetType Create_Activity(TargetType act, requires(TargetType, !check(TargetType, is_pointer) || !check(strip_modifiers(TargetType), Activity_Simulator::Activity_Concepts::Is_Activity_Plan_Prototype)))
			{
				assert_check(TargetType, is_pointer, "target type must be a pointer");
				assert_check(strip_modifiers(TargetType), Activity_Simulator::Activity_Concepts::Is_Activity_Plan_Prototype, "must be an Activity_Plan prototype");
			}
			template<typename TargetType, typename LocationType> void Create_Activity(TargetType act_type, int start_plan_time, LocationType location)
			{
				this_component()->Create_Activity<TargetType,LocationType>(act_type,start_plan_time,location);
			}
			template<typename TargetType, typename LocationType> void Create_Home_Activity(int& activity_count)
			{
				this_component()->Create_Home_Activity<TargetType>(activity_count);
			}
			template<typename TargetType, typename LocationType, typename ModeType> void Create_Activity(TargetType act_type, int start_plan_time, LocationType location, ModeType mode)
			{
				this_component()->Create_Activity<TargetType,LocationType,ModeType>(act_type,start_plan_time,location,mode);
			}
			template<typename TargetType, typename LocationType, typename TimeType> void Create_Activity(TargetType act_type, int start_plan_time, LocationType location, TimeType start, TimeType duration)
			{
				this_component()->Create_Activity<TargetType, LocationType, TimeType>(act_type, start_plan_time, location, start, duration);
			}
			template<typename TargetType, typename LocationType, typename ModeType, typename TimeType> void Create_Activity(TargetType act_type, int start_plan_time, LocationType location, ModeType mode, TimeType start, TimeType duration)
			{
				this_component()->Create_Activity<TargetType,LocationType,ModeType,TimeType>(act_type,start_plan_time,location,mode,start,duration);
			}
			template<typename ReturnType, typename TimeType, typename ModeType> ReturnType Create_Home_Activity(TimeType departure_time, TimeType start, TimeType duration, ModeType mode)
			{
				return this_component()->Create_Home_Activity<ReturnType,TimeType,ModeType>(departure_time,start,duration,mode);
			}
			



			// indicators used to determine next iteration for generation/planning events
			accessor(Next_Activity_Generation_Time, NONE, NONE);
			accessor(Generation_Time_Increment, NONE, NONE);

			// accessor to parent class
			accessor(Parent_Planner, NONE, NONE);
		};
	}

}
