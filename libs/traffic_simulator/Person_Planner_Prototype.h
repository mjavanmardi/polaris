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
			ACTIVITY_GENERATION=19,
			ACTIVITY_PLANNING,
			MOVEMENT_PLANNING,
		};
	}

	namespace Concepts
	{
		concept struct Is_Person_Planner_Prototype
		{
			check_template_method_name(Has_Parent_Person,ComponentType::Parent_Person);
			check_template_method_name(Has_Movement_Plans_Container, ComponentType::Movement_Plans_Container);
			check_template_method_name(Has_Activity_Plans_Container, ComponentType::Activity_Container);
			define_default_check(Has_Parent_Person && Has_Movement_Plans_Container && Has_Activity_Plans_Container);
		};
		concept struct Is_Person_Planner_Component
		{
			check_template_method_name(Has_Parent_Person,Parent_Person);
			check_template_method_name(Has_Movement_Plans_Container, Movement_Plans_Container);
			check_template_method_name(Has_Activity_Plans_Container, Activity_Container);
			define_default_check(Has_Parent_Person && Has_Movement_Plans_Container && Has_Activity_Plans_Container);
		};
		concept struct Is_Person_Planner
		{
			check_concept(is_prototype, Is_Person_Planner_Prototype, T, V);
			check_concept(is_component, Is_Person_Planner_Component, T, V);
			define_default_check(is_prototype || is_component);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Person_Planner ADD_DEBUG_INFO
		{
			tag_as_prototype;
		 
			// Conditonal handling and helper functions
			static void Planning_Conditional(ComponentType* _this,Event_Response& response)
			{
				//----------------------------------------------
				// CONDITIONALS FOR BASIC AGENT SCHEDULING
				// 1.) Activity Generation (Occurs 
				// Create alias for this to use in conditional
				typedef Person_Planner<ComponentType> _Planning_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Planning_Interface* this_ptr =(_Planning_Interface*)_pthis;

				// Define interfaces to the container members of the class
				typedef Prototypes::Person<typename get_type_of(Parent_Person)> person_itf;
				typedef Household_Components::Prototypes::Household<typename person_itf::get_type_of(Household)> household_itf;
				typedef Prototypes::Person_Scheduler<typename person_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
				typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename remove_pointer<typename scheduler_itf::get_type_of(Movement_Plans_Container)::value_type>::type> Movement_Plan;
				typedef Back_Insertion_Sequence<typename scheduler_itf::get_type_of(Movement_Plans_Container),Movement_Plan*> Movement_Plans_List;

				
				person_itf* person = this_ptr->template Parent_Person<person_itf*>();
				household_itf* household = person->template Household<household_itf*>();
				scheduler_itf* scheduler = person->template Scheduling_Faculty<scheduler_itf*>();
				Movement_Plans_List* movement_plans = scheduler->template Movement_Plans_Container<Movement_Plans_List*>();

				typename Movement_Plans_List::iterator move_itr = movement_plans->begin();
				Movement_Plan* movement;			
				if (move_itr != movement_plans->end()) movement = *move_itr;


				//------------------------------------------------------------------------------------------------------------------------------
				// SETUP SUBITERATION, set up future subiteration() schedule
				if (sub_iteration() == 0)
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
				// ACTIVITY GENERATION SUBITERATION, swap in the activity-generation event and set up future subiteration() schedule
				else if (sub_iteration() == Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_GENERATION)
				{
					_pthis->Swap_Event((Event)&Person_Planner::Activity_Generation_Event<NULLTYPE>);
					this_ptr->template Go_To_Subiteration<NT>(Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING,true,response);
				}

				//------------------------------------------------------------------------------------------------------------------------------
				// MOVEMENT PLANNING SUBITERATION, swap in the movement planning event and set up future subiteration() schedule
				else if (sub_iteration() == Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING)
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
				this_ptr->template Next_Planning_Time<Simulation_Timestep_Increment>(iteration() + this_ptr->template Planning_Time_Increment<Simulation_Timestep_Increment>());

				CHECK_CONDITIONAL
			}
			template<typename TargetType> void Go_To_Subiteration(Person_Components::Types::PLANNING_ITERATION_STEP_KEYS subiteration(), bool do_current_event, Event_Response& response)
			{
				response.next._iteration = _iteration;
				response.next._sub_iteration = sub_iteration;
				response.result = do_current_event;
			}
			template<typename TargetType> void Go_To_Next_Iteration(bool do_current_event, Event_Response& response)
			{
				response.next._iteration = Round<long,double>(Simulation_Time.template Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this->Planning_Time_Increment<Simulation_Timestep_Increment>()));
				response.next._sub_iteration = 0;
				response.result = do_current_event;
			}
		

			// Event handling
			declare_event(Activity_Generation_Event)
			{
				// Create alias for this to use in conditional
				typedef Person_Planner<ComponentType> _Planning_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Planning_Interface* this_ptr=(_Planning_Interface*)_pthis;

				// Call specific implementation of the activity generation routine
				typedef Prototypes::Activity_Generator<typename get_type_of(Activity_Generation_Faculty)> activity_generator_itf;
				activity_generator_itf* generator = this_ptr->template Activity_Generation_Faculty<activity_generator_itf*>();
				generator->template Activity_Generation<TargetType>();

				// set next activity generation occurence
				this_ptr->template Next_Activity_Generation_Time<Simulation_Timestep_Increment>(Round<long,double>(Simulation_Time.template Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template Generation_Time_Increment<Simulation_Timestep_Increment>())));
			}
			declare_event(Activity_Planning_Event)
			{
				//// Create alias for this to use in conditional
				//typedef Person_Planner<ComponentType> _Planning_Interface;
				//ComponentType* _pthis = (ComponentType*)_this;
				//_Planning_Interface* this_ptr=(_Planning_Interface*)_pthis;

				//typedef Activity_Planner<typename remove_pointer<typename get_type_of(Activity_Plans_Container)::value_type>::type> Activity_Plan;
				//typedef Back_Insertion_Sequence<typename get_type_of(Activity_Plans_Container),Activity_Plan*> Activity_Plans;

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
			declare_event(Movement_Planning_Event)
			{
				// Create alias for this to use in conditional
				typedef Person_Planner<ComponentType> _Planning_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Planning_Interface* this_ptr=(_Planning_Interface*)_pthis;

				// create aliases for network components from parent
				typedef Prototypes::Person<typename get_type_of(Parent_Person)> parent_itf;
				typedef Household_Components::Prototypes::Household<typename parent_itf::get_type_of(Household)> household_itf;
				typedef Prototypes::Person_Scheduler<typename parent_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
				typedef  Person_Components::Prototypes::Person_Mover< typename parent_itf::get_type_of(Moving_Faculty)> _Movement_Faculty_Interface;
				typedef  Network_Components::Prototypes::Network< typename parent_itf::get_type_of(network_reference)> _Network_Interface;
				typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename _Network_Interface::get_type_of(activity_locations_container)::value_type>::type>  _Activity_Location_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container), _Activity_Location_Interface*> _Activity_Locations_Container_Interface;

				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Activity_Location_Interface::get_type_of(origin_links)::value_type>::type>  _Link_Interface;
				typedef  Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links), _Link_Interface*> _Links_Container_Interface;

				typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename _Network_Interface::get_type_of(zones_container)::value_type>::type>  _Zone_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(zones_container), _Zone_Interface*> _Zones_Container_Interface;

				
				parent_itf* parent = this_ptr->template Parent_Person<parent_itf*>();
				household_itf* household = parent->template Household<household_itf*>();

				_Network_Interface* network = parent->template network_reference<_Network_Interface*>();
				_Movement_Faculty_Interface* movement_faculty = parent->template Moving_Faculty<_Movement_Faculty_Interface*>();

				
				// Get reference to vehicle, and add current movement plan to it for routing
				typedef Vehicle_Components::Prototypes::Vehicle<typename parent_itf::get_type_of(vehicle)> vehicle_itf;
				vehicle_itf* vehicle = parent->template vehicle<vehicle_itf*>();

				// Get reference to movement plans
				typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename remove_pointer<typename scheduler_itf::get_type_of(Movement_Plans_Container)::value_type>::type> Movement_Plan;
				typedef Back_Insertion_Sequence<typename scheduler_itf::get_type_of(Movement_Plans_Container),Movement_Plan*> Movement_Plans;

				typedef Activity_Components::Prototypes::Activity_Planner< typename Movement_Plan::get_type_of(destination_activity_reference)> Activity_Plan;
				Movement_Plans* movements = parent->template Scheduling_Faculty<scheduler_itf*>()->template Movement_Plans_Container<Movement_Plans*>();
				typename Movement_Plans::iterator move_itr = movements->begin();

				while (move_itr != movements->end())
				{
					Movement_Plan* move = *move_itr;

					// if movement happens in the current planning increment, execute movement
					if (move->template departed_time<Simulation_Timestep_Increment>() >= Simulation_Time.template Current_Time<Simulation_Timestep_Increment>() &&
						move->template departed_time<Simulation_Timestep_Increment>() < Simulation_Time.template Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template Planning_Time_Increment<Simulation_Timestep_Increment>()))
					{

						//if (ComponentType::_write_activity_files) ComponentType::logs[__thread_id]<<"MOVE_EVENT:," << parent->template uuid<int>() << ", PASSED."<<endl;
				
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
							//if (orig->template internal_id<int>() == dest->template internal_id<int>())
							//{
							//	typename Movement_Plans::iterator prev = move_itr++;
							//	movements->erase(prev);
							//	return;
							//}

							// if already moving, skip movement
							if (movement_faculty->template Movement_Scheduled<bool>() == true)
							{
								//Movement_Plan* cur_move = movement_faculty->template Movement<Movement_Plan*>();
								//Activity_Plan* cur_act = cur_move->template destination_activity_reference<Activity_Plan*>();
								//THROW_WARNING("WARNING: movement already scheduled for current iteration for person: " << parent->template uuid<int>() << ", movement ignored.   " << "Act in progress: "<<cur_act->template Activity_Plan_ID<int>()<<", departure time="<<cur_move->template departed_time<Time_Seconds>()<<", expected start time=" << cur_act->template Start_Time<Time_Seconds>()<<", cur trajactory position: " << cur_move->template current_trajectory_position<int>() <<" of " << cur_move->template trajectory_size<int>()<<", mode="<<cur_act->template Mode<int>()<<endl);
								typename Movement_Plans::iterator prev = move_itr++;
								//movements->erase(prev);
								return;
							}
							vehicle->template movement_plan<Movement_Plan*>(move);
//TODO
//							movement_faculty->template Schedule_Movement<Target_Type<NT,void,Simulation_Timestep_Increment,Movement_Plan*>>(move->template departed_time<Simulation_Timestep_Increment>(),move);
						}
	
						//TODO: CHANGE SO THAT MULTIPLE MOVES CAN BE PLANNED PER PLANNING TIMESTEP - currently we are only simulating the first planned move, then throwing out the rest
						typename Movement_Plans::iterator prev = move_itr++;
						//movements->erase(prev);
					}

					// remove movements which have already been skipped
					else if (move->template departed_time<Simulation_Timestep_Increment>() < Simulation_Time.template Current_Time<Simulation_Timestep_Increment>())
					{
						typename Movement_Plans::iterator prev = move_itr++;
						//movements->erase(prev);
						//
						//// increment the deleted activities counter
						//typedef  Person_Components::Prototypes::Person_Data_Logger< typename ComponentType::Master_Type::person_data_logger_type> _Logger_Interface;
						//_Logger_Interface* logger = (_Logger_Interface*)_global_person_logger;
						//logger->template Increment_Cancelled_Activities<NT>();
					}
					// exit if no movements in current timestep
					else
					{
						return;
					}

					WHILE_PRINT("movement planning event while loop");
				}
			}

			define_feature_exists_check(Initialize,Has_Initialize);
			template<typename TargetType> void Initialize(requires(TargetType,check(ComponentType, Has_Initialize)))
			{
				this_component()->template Initialize< TargetType>();
				long first_iter = this->Next_Activity_Generation_Time<Simulation_Timestep_Increment>();
				//TODO
//load_event(ComponentType,Planning_Conditional,Activity_Generation_Event,first_iter,0,NULLTYPE);
			}
			template<typename TargetType> void Initialize(requires(TargetType,!check(ComponentType,Has_Initialize)))
			{
				assert_check(ComponentType,Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
			}
			template<typename TargetType> void Initialize(TargetType initializer, requires(TargetType,check(ComponentType,Has_Initialize)))
			{
				this_component()->template Initialize< TargetType>(initializer);
				long first_iter = this->Next_Activity_Generation_Time<Simulation_Timestep_Increment>();
				//TODO
//load_event(ComponentType,Planning_Conditional,Movement_Planning_Event,first_iter,0,NULLTYPE);
			}
			template<typename TargetType> void Initialize(TargetType initializer, requires(TargetType,!check(ComponentType,Has_Initialize)))
			{
				assert_check(ComponentType,Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
			}

			template<typename TargetType> void Schedule_New_Routing(int planning_time, TargetType movement_plan, requires(TargetType,check(strip_modifiers(TargetType),Movement_Plan_Components::Concepts::Is_Movement_Plan)))
			{
				// interfaces	
				typedef  Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
				typedef  Scenario_Components::Prototypes::Scenario< typename Parent_Person_Itf::get_type_of(scenario_reference)> Scenario_Itf;
				typedef  Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
				typedef  Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
				typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename Routing_Itf::get_type_of(movement_plan)> Movement_Itf;
				typedef Activity_Components::Prototypes::Activity_Planner< typename Movement_Itf::get_type_of(destination_activity_reference)> Activity_Itf;

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
				if (act == nullptr)
				{
					itf->template Schedule_Route_Computation<NULLTYPE>(movement_plan->template departed_time<Simulation_Timestep_Increment>(), planning_time,scenario->template read_network_snapshots<bool>());
					return;
				}
				else if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
				{
					itf->template Schedule_Route_Computation<NULLTYPE>(movement_plan->template departed_time<Simulation_Timestep_Increment>(), planning_time,scenario->template read_network_snapshots<bool>());
				}

			}
			template<typename TargetType> void Schedule_New_Routing(int planning_time, TargetType movement_plan, requires(TargetType,!check(strip_modifiers(TargetType),Movement_Plan_Components::Concepts::Is_Movement_Plan)))
			{
				assert_check(strip_modifiers(TargetType),Movement_Plan_Components::Concepts::Is_Movement_Plan, "ERROR: TargetType is not a valid movement plan component.");
			}

			template<typename TargetType> void Write_To_Log(TargetType s)
			{
				this_component()->Write_To_Log<TargetType>(s);
			}
	
			// Sub faculties of the Person Planner
			accessor(Activity_Generation_Faculty, NONE, NONE);
			accessor(Destination_Choice_Faculty, NONE, NONE);
			accessor(Mode_Choice_Faculty, NONE, NONE);
			accessor(Timing_Chooser, NONE, NONE);

			//accessor(Activity_Container, NONE, NONE);
			//accessor(Activity_Record_Container, NONE, NONE);
			//accessor(Movement_Plans_Container, NONE, NONE);
			//accessor(Schedule_Container, NONE, NONE);	

			// Pass through members to scheduler class
			//accessor(current_movement_plan, NONE, NONE);
			//accessor(current_activity_plan, NONE, NONE);
			//feature_method_1_arg(next_activity_plan, current_time, check(typename TargetType::ParamType,Is_Time_Value) && check(typename TargetType::ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype));
			//feature_method_1_arg(previous_activity_plan, current_time, check(typename TargetType::ParamType,Is_Time_Value) /*&& check(typename TargetType::ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/);
			template<typename TargetType> void Add_Movement_Plan(TargetType movement_plan)
			{
				this_component()->template Add_Movement_Plan<TargetType>(movement_plan);
			}
			template<typename TargetType> void Add_Activity_Plan(TargetType activity_plan)
			{
				this_component()->template Add_Activity_Plan<TargetType>(activity_plan);
			}

			// indicators used to determine next iteration for generation/planning events
			accessor(Next_Activity_Generation_Time, NONE, NONE);
			accessor(Next_Planning_Time, NONE, NONE);
			accessor(Planning_Time_Increment, NONE, NONE);
			accessor(Generation_Time_Increment, NONE, NONE);

			// accessor to parent class
			accessor(Parent_Person, NONE, NONE);
		};

	}

}
