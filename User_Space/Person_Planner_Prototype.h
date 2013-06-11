#pragma once

#include "User_Space_Includes.h"
//#include "Person_Prototype.h"


//---------------------------------------------------------
//	Person Scheduler - Handles Activity Scheduling for the Planner Class
//---------------------------------------------------------

namespace Person_Components
{
	namespace Types
	{
		enum PLANNING_ITERATION_STEP_KEYS
		{
			ACTIVITY_GENERATION=1,
			ACTIVITY_PLANNING=2,
			MOVEMENT_PLANNING=3,
		};
	}

	namespace Concepts
	{
		concept struct Is_Person_Planner_Prototype
		{
			check_getter(Has_Parent_Person,ComponentType::Parent_Person);
			check_getter(Has_Movement_Plans_Container, ComponentType::Movement_Plans_Container);
			check_getter(Has_Activity_Plans_Container, ComponentType::Activity_Container);
			define_default_check(Has_Parent_Person && Has_Movement_Plans_Container && Has_Activity_Plans_Container);
		};
		concept struct Is_Person_Planner_Component
		{
			check_getter(Has_Parent_Person,Parent_Person);
			check_getter(Has_Movement_Plans_Container, Movement_Plans_Container);
			check_getter(Has_Activity_Plans_Container, Activity_Container);
			define_default_check(Has_Parent_Person && Has_Movement_Plans_Container && Has_Activity_Plans_Container);
		};
		concept struct Is_Person_Planner
		{
			check_concept(is_prototype, Is_Person_Planner_Prototype);
			check_concept(is_component, Is_Person_Planner_Component);
			define_default_check(is_prototype || is_component);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Person_Planner ADD_DEBUG_INFO
		{
			tag_as_prototype;
		 
			// Conditonal handling and helper functions
			declare_feature_conditional(Planning_Conditional)
			{
				//----------------------------------------------
				// CONDITIONALS FOR BASIC AGENT SCHEDULING
				// 1.) Activity Generation (Occurs 
				// Create alias for this to use in conditional
				typedef Person_Planner<ComponentType, ComponentType> _Planning_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Planning_Interface* this_ptr =(_Planning_Interface*)_pthis;

				// Define interfaces to the container members of the class
				define_component_interface(person_itf,typename get_type_of(Parent_Person),Prototypes::Person,ComponentType);
				define_component_interface(scheduler_itf,typename person_itf::get_type_of(Scheduling_Faculty),Prototypes::Person_Scheduler,ComponentType);
				define_container_and_value_interface(Movement_Plans_List,Movement_Plan,typename scheduler_itf::get_type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
				
				person_itf* person = this_ptr->template Parent_Person<person_itf*>();
				scheduler_itf* scheduler = person->template Scheduling_Faculty<scheduler_itf*>();
				Movement_Plans_List* movement_plans = scheduler->template Movement_Plans_Container<Movement_Plans_List*>();

				typename Movement_Plans_List::iterator move_itr = movement_plans->begin();
				Movement_Plan* movement;			
				if (move_itr != movement_plans->end()) movement = *move_itr;

				//------------------------------------------------------------------------------------------------------------------------------
				// SETUP SUBITERATION, set up future sub_iteration schedule
				if (_sub_iteration == 0)
				{
					// If activity generation is to be performed, do that next
					if(this_ptr->template Next_Activity_Generation_Time<Time_Seconds>() == Simulation_Time.template Current_Time<Time_Seconds>()) 
					{	
						this_ptr->template Go_To_Subiteration<NT>(Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_GENERATION,false,response);
					}
					else if (move_itr != movement_plans->end())
					{
						if (movement->template departed_time<Simulation_Timestep_Increment>() < Simulation_Time.template Future_Time<Simulation_Timestep_Increment, Simulation_Timestep_Increment>(this_ptr->template Planning_Time_Increment<Simulation_Timestep_Increment>()))
							this_ptr->template Go_To_Subiteration<NT>(Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING,false,response);
						else
							this_ptr->template Go_To_Next_Iteration<NT>(false,response);
					}
					// otherwise move on to next main iteration
					else
					{
						this_ptr->template Go_To_Next_Iteration<NT>(false,response);
					}
				}

				//------------------------------------------------------------------------------------------------------------------------------
				// ACTIVITY GENERATION SUBITERATION, swap in the activity-generation event and set up future sub_iteration schedule
				else if (_sub_iteration == Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_GENERATION)
				{
					_pthis->Swap_Event((Event)&Person_Planner::Activity_Generation_Event<NULLTYPE>);
					this_ptr->template Go_To_Subiteration<NT>(Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING,true,response);
				}

				//------------------------------------------------------------------------------------------------------------------------------
				// MOVEMENT PLANNING SUBITERATION, swap in the movement planning event and set up future sub_iteration schedule
				else if (_sub_iteration == Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING)
				{
					_pthis->Swap_Event((Event)&Person_Planner::Movement_Planning_Event<NULLTYPE>);
					this_ptr->template Go_To_Next_Iteration<NT>(true,response);
				}
				//------------------------------------------------------------------------------------------------------------------------------
				// No valid events scheduled - skip to next iteration
				else
				{
					this_ptr->template Go_To_Next_Iteration<NT>(false,response);
				}

				// set next planning time for other functions to use
				this_ptr->template Next_Planning_Time<Simulation_Timestep_Increment>(_iteration + this_ptr->template Planning_Time_Increment<Simulation_Timestep_Increment>());
			}
			feature_prototype void Go_To_Subiteration(Person_Components::Types::PLANNING_ITERATION_STEP_KEYS sub_iteration, bool do_current_event, Conditional_Response& response)
			{
				response.next._iteration = _iteration;
				response.next._sub_iteration = sub_iteration;
				response.result = do_current_event;
			}
			feature_prototype void Go_To_Next_Iteration(bool do_current_event, Conditional_Response& response)
			{
				response.next._iteration = Round<long,double>(Simulation_Time.template Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this->Planning_Time_Increment<Simulation_Timestep_Increment>()));
				response.next._sub_iteration = 0;
				response.result = do_current_event;
			}
		

			// Event handling
			declare_feature_event(Activity_Generation_Event)
			{
				// Create alias for this to use in conditional
				typedef Person_Planner<ComponentType, ComponentType> _Planning_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Planning_Interface* this_ptr=(_Planning_Interface*)_pthis;

				// Call specific implementation of the activity generation routine
				define_component_interface(activity_generator_itf,typename get_type_of(Activity_Generation_Faculty),Prototypes::Activity_Generator,ComponentType);
				activity_generator_itf* generator = this_ptr->template Activity_Generation_Faculty<activity_generator_itf*>();
				generator->template Activity_Generation<TargetType>();

				// set next activity generation occurence
				this_ptr->template Next_Activity_Generation_Time<Simulation_Timestep_Increment>(Round<long,double>(Simulation_Time.template Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template Generation_Time_Increment<Simulation_Timestep_Increment>())));
			}
			declare_feature_event(Activity_Planning_Event)
			{
				//// Create alias for this to use in conditional
				//typedef Person_Planner<ComponentType, ComponentType> _Planning_Interface;
				//ComponentType* _pthis = (ComponentType*)_this;
				//_Planning_Interface* this_ptr=(_Planning_Interface*)_pthis;

				//define_container_and_value_interface(Activity_Plans,Activity_Plan,typename get_type_of(Activity_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Planner,ComponentType);
				//Activity_Plans* activities = this_ptr->Activity_Plans_Container<Activity_Plans*>();
				//typename Activity_Plans::iterator act_itr = activities->begin();

				//while (act_itr != activities->end())
				//{
				//	Activity_Plan* act = *act_itr;

				//	// if movement happens in the current planning increment, execute movement
				//	if (act->template Activity_Planning_Time<Simulation_Timestep_Increment>() >= Simulation_Time.Current_Time<Simulation_Timestep_Increment>() &&
				//		act->template Activity_Planning_Time<Simulation_Timestep_Increment>() < Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->Planning_Time_Increment<Simulation_Timestep_Increment>()))
				//	{
				//		// Do activity planning
				//		act->template Do_Activity_Planning<NULLTYPE>();	
	
				//		//TODO: CHANGE SO THAT MULTIPLE MOVES CAN BE PLANNED PER PLANNING TIMESTEP - currently we are only simulating the first planned move, then throwing out the rest
				//		typename Activity_Plans::iterator prev = act_itr++;
				//		activities->erase(prev);
				//	}

				//	// remove movements which have already been skipped
				//	else if (act->template Activity_Planning_Time<Simulation_Timestep_Increment>() < Simulation_Time.Current_Time<Simulation_Timestep_Increment>())
				//	{
				//		typename Activity_Plans::iterator prev = act_itr++;
				//		activities->erase(prev);
				//	}
				//	// exit if no movements in current timestep
				//	else
				//	{
				//		break;
				//	}
				//}
			}
			declare_feature_event(Movement_Planning_Event)
			{
				// Create alias for this to use in conditional
				typedef Person_Planner<ComponentType, ComponentType> _Planning_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Planning_Interface* this_ptr=(_Planning_Interface*)_pthis;

				// create aliases for network components from parent
				define_component_interface(parent_itf,typename get_type_of(Parent_Person),Prototypes::Person,ComponentType);
				define_component_interface(scheduler_itf,typename parent_itf::get_type_of(Scheduling_Faculty),Prototypes::Person_Scheduler,ComponentType);
				define_component_interface(_Movement_Faculty_Interface, typename parent_itf::get_type_of(Moving_Faculty), Person_Components::Prototypes::Person_Mover, ComponentType);
				define_component_interface(_Network_Interface, typename parent_itf::get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
				parent_itf* parent = this_ptr->template Parent_Person<parent_itf*>();
				_Network_Interface* network = parent->template network_reference<_Network_Interface*>();
				_Movement_Faculty_Interface* movement_faculty = parent->template Moving_Faculty<_Movement_Faculty_Interface*>();

				// Get reference to vehicle, and add current movement plan to it for routing
				define_component_interface(vehicle_itf,typename parent_itf::get_type_of(vehicle),Vehicle_Components::Prototypes::Vehicle_Prototype,ComponentType);
				vehicle_itf* vehicle = parent->template vehicle<vehicle_itf*>();

				// Get reference to movement plans
				define_container_and_value_interface(Movement_Plans,Movement_Plan,typename scheduler_itf::get_type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
				Movement_Plans* movements = parent->template Scheduling_Faculty<scheduler_itf*>()->template Movement_Plans_Container<Movement_Plans*>();
				typename Movement_Plans::iterator move_itr = movements->begin();

				while (move_itr != movements->end())
				{
					Movement_Plan* move = *move_itr;

					// if movement happens in the current planning increment, execute movement
					if (move->template departed_time<Simulation_Timestep_Increment>() >= Simulation_Time.template Current_Time<Simulation_Timestep_Increment>() &&
						move->template departed_time<Simulation_Timestep_Increment>() < Simulation_Time.template Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template Planning_Time_Increment<Simulation_Timestep_Increment>()))
					{

						if (ComponentType::_write_activity_files) ComponentType::logs[_thread_id]<<"MOVE_EVENT:," << parent->template uuid<int>() << ", PASSED."<<endl;
				
						// make sure vehicle is not already being simulated, skip movement if it is
						if (vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::UNLOADED || vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::OUT_NETWORK)
						{
							// increment the zone origin/destination counters based on movement plan
							_Activity_Location_Interface* orig = move->template origin<_Activity_Location_Interface*>();
							_Activity_Location_Interface* dest = move->template destination<_Activity_Location_Interface*>();
							_Zone_Interface* orig_zone = orig->template zone<_Zone_Interface*>();
							_Zone_Interface* dest_zone = dest->template zone<_Zone_Interface*>();

							// if origin or destination zones are unavailable, abort trip
							if (orig_zone->template zone_is_available<bool>()== false || dest_zone->template zone_is_available<bool>() == false)
							{
								cout << endl << "zone is unavailable";
								typename Movement_Plans::iterator prev = move_itr++;
								movements->erase(prev);
								return;
							}


							// increment counters for display
							orig_zone->template production_count<int&>()++;
							dest_zone->template attraction_count<int&>()++;


							// if null trip, print out and remove from schedule
							if (orig->template internal_id<int>() == dest->template internal_id<int>())
							{
								typename Movement_Plans::iterator prev = move_itr++;
								movements->erase(prev);
								return;
							}

							// add movement plan to the person's vehicle and schedule the departure
							if (movement_faculty->template Movement_Scheduled<bool>() == true)
							{
								THROW_WARNING("WARNING: movement already scheduled for current iteration for person: " << parent->uuid<int>() << ", movement ignored.");
								typename Movement_Plans::iterator prev = move_itr++;
								movements->erase(prev);
								return;
							}
							vehicle->template movement_plan<Movement_Plan*>(move);
							movement_faculty->template Schedule_Movement<Target_Type<NT,void,Simulation_Timestep_Increment,Movement_Plan*>>(move->template departed_time<Simulation_Timestep_Increment>(),move);
						}
	
						//TODO: CHANGE SO THAT MULTIPLE MOVES CAN BE PLANNED PER PLANNING TIMESTEP - currently we are only simulating the first planned move, then throwing out the rest
						typename Movement_Plans::iterator prev = move_itr++;
						movements->erase(prev);
					}

					// remove movements which have already been skipped
					else if (move->template departed_time<Simulation_Timestep_Increment>() < Simulation_Time.template Current_Time<Simulation_Timestep_Increment>())
					{
						typename Movement_Plans::iterator prev = move_itr++;
						movements->erase(prev);
					}
					// exit if no movements in current timestep
					else
					{
						return;
					}
				}
			}

			define_feature_exists_check(Initialize,Has_Initialize);
			feature_prototype void Initialize(requires(check(ComponentType, Has_Initialize)))
			{
				this_component()->template Initialize<ComponentType,CallerType, TargetType>();
				long first_iter = this->Next_Activity_Generation_Time<Simulation_Timestep_Increment>();
				load_event(ComponentType,Planning_Conditional,Activity_Generation_Event,first_iter,0,NULLTYPE);
			}
			feature_prototype void Initialize(requires(!check(ComponentType,Has_Initialize)))
			{
				assert_check(ComponentType,Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
			}
			feature_prototype void Initialize(TargetType initializer, requires(check(ComponentType,Has_Initialize)))
			{
				this_component()->template Initialize<ComponentType,CallerType, TargetType>(initializer);
				long first_iter = this->Next_Activity_Generation_Time<Simulation_Timestep_Increment>();
				load_event(ComponentType,Planning_Conditional,Movement_Planning_Event,first_iter,0,NULLTYPE);
			}
			feature_prototype void Initialize(TargetType initializer, requires(!check(ComponentType,Has_Initialize)))
			{
				assert_check(ComponentType,Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
			}

			feature_prototype void Schedule_New_Routing(int planning_time, TargetType movement_plan, requires(check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan)))
			{
				// interfaces	
				define_component_interface(Parent_Person_Itf, typename get_type_of(Parent_Person), Person_Components::Prototypes::Person, ComponentType);
				define_component_interface(Scenario_Itf, typename Parent_Person_Itf::get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype,ComponentType);
				define_component_interface(Vehicle_Itf, typename get_type_of(Parent_Person)::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(Routing_Itf, typename get_type_of(Parent_Person)::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
				define_component_interface(Movement_Itf, typename Routing_Itf::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				define_component_interface(Activity_Itf, typename Movement_Itf::get_type_of(destination_activity_reference),Activity_Components::Prototypes::Activity_Planner,ComponentType);

				// references
				Parent_Person_Itf* person_itf = this->Parent_Person<Parent_Person_Itf*>();
				Routing_Itf* itf= person_itf->template router<Routing_Itf*>();	
				Movement_Itf* move = (Movement_Itf*)movement_plan;
				Activity_Itf* act = move->template destination_activity_reference<Activity_Itf*>();

				// set movement plan as current movement in router faculty
				itf->template movement_plan<Movement_Itf*>((Movement_Itf*)movement_plan);	

				move->template planning_time<Simulation_Timestep_Increment>(planning_time);

				// whether to use snapshot or not
				Scenario_Itf* scenario = (Scenario_Itf*)_global_scenario;

				// calculate route, if the mode is auto, otherwise return
				if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
				{
					itf->template Schedule_Route_Computation<NULLTYPE>(movement_plan->template departed_time<Simulation_Timestep_Increment>(), planning_time,scenario->template read_network_snapshots<bool>());
				}
			}
			feature_prototype void Schedule_New_Routing(int planning_time, TargetType movement_plan, requires(!check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan)))
			{
				assert_check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan, "ERROR: TargetType is not a valid movement plan component.");
			}

			feature_prototype void Write_To_Log(TargetType s)
			{
				this_component()->Write_To_Log<ComponentType,CallerType,TargetType>(s);
			}
	
			// Sub faculties of the Person Planner
			feature_accessor(Activity_Generation_Faculty,none,none);
			feature_accessor(Destination_Choice_Faculty,none,none);
			feature_accessor(Timing_Chooser,none,none);

			//feature_accessor(Activity_Container,none,none);
			//feature_accessor(Activity_Record_Container,none,none);
			//feature_accessor(Movement_Plans_Container,none,none);
			//feature_accessor(Schedule_Container,none,none);	

			// Pass through members to scheduler class
			//feature_accessor(current_movement_plan,none,none);
			//feature_accessor(current_activity_plan,none,none);
			//feature_method_1_arg(next_activity_plan, current_time, check(typename TargetType::ParamType,Is_Time_Value) && check(typename TargetType::ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype));
			//feature_method_1_arg(previous_activity_plan, current_time, check(typename TargetType::ParamType,Is_Time_Value) /*&& check(typename TargetType::ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/);
			feature_prototype void Add_Movement_Plan(TargetType movement_plan)
			{
				this_component()->template Add_Movement_Plan<ComponentType,CallerType,TargetType>(movement_plan);
			}
			feature_prototype void Add_Activity_Plan(TargetType activity_plan)
			{
				this_component()->template Add_Activity_Plan<ComponentType,CallerType,TargetType>(activity_plan);
			}

			// indicators used to determine next iteration for generation/planning events
			feature_accessor(Next_Activity_Generation_Time,none,none);
			feature_accessor(Next_Planning_Time,none,none);
			feature_accessor(Planning_Time_Increment,none,none);
			feature_accessor(Generation_Time_Increment,none,none);

			// accessor to parent class
			feature_accessor(Parent_Person,none,none);
		};

	}

}