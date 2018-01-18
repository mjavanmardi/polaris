#pragma once

#include "Activity_Simulator_Includes.h"
#include "Activity_Generator_Prototype.h"
#include "Person_Prototype.h"
#include "Household_Prototype.h"

//---------------------------------------------------------
//	Person Scheduler - Handles Activity Scheduling for the Planner Class
//---------------------------------------------------------

namespace Person_Components
{
	namespace Types
	{
		//enum PLANNING_ITERATION_STEP_KEYS
		//{
		//	ACTIVITY_GENERATION=19,
		//	ACTIVITY_PLANNING=20,
		//	MOVEMENT_PLANNING=21,
		//};
	}

	namespace Concepts
	{
		concept struct Is_Person_Planner_Prototype
		{
			check_component_accessor_name(Has_Parent_Person, Parent_Person);
			check_component_accessor_name(Has_Movement_Plans_Container, Movement_Plans_Container);
			check_component_accessor_name(Has_Activity_Plans_Container, Activity_Container);
			define_default_check(Has_Parent_Person && Has_Movement_Plans_Container && Has_Activity_Plans_Container);
		};
		concept struct Is_Person_Planner_Component
		{
			check_accessor_name(Has_Parent_Person,Parent_Person);
			check_accessor_name(Has_Movement_Plans_Container, Movement_Plans_Container);
			check_accessor_name(Has_Activity_Plans_Container, Activity_Container);
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
		 
			//=========================================================================================================
			// Event handling and helper functions
			static void Planning_Event_Controller(ComponentType* _this, Event_Response& response);
			template<typename TargetType> void Go_To_Subiteration(Scenario_Components::Types::Demand_Sub_Iteration_keys subiteration, Event_Response& response);
			template<typename TargetType> void Go_To_Next_Iteration(Event_Response& response);
		
			//=========================================================================================================
			// Planning events
			template<typename T> void Activity_Generation_Event();
			template<typename T> void Movement_Planning_Event();


			//=========================================================================================================
			// Member functions
			local_check_template_method_name(Has_Initialize,Initialize);
			template<typename TargetType> void Initialize();
			template<typename TargetType> void Initialize(TargetType initializer);
			template<typename TargetType> void Schedule_New_Routing(int planning_time, TargetType movement_plan, requires(TargetType, check(strip_modifiers(TargetType), Movement_Plan_Components::Concepts::Is_Movement_Plan)));
			template<typename TargetType> void Schedule_New_Routing(int planning_time, TargetType movement_plan, requires(TargetType, !check(strip_modifiers(TargetType), Movement_Plan_Components::Concepts::Is_Movement_Plan)));

			template<typename TargetType> void Write_To_Log(TargetType s);
	
			// Sub faculties of the Person Planner
			accessor(Activity_Generation_Faculty, NONE, NONE);
			accessor(Destination_Choice_Faculty, NONE, NONE);
			accessor(Mode_Choice_Faculty, NONE, NONE);
			accessor(Timing_Chooser, NONE, NONE);
			accessor(Telecommuting_Choice_Faculty, NONE, NONE);

			template<typename TargetType> void Add_Movement_Plan(TargetType movement_plan);
			template<typename TargetType> void Add_Activity_Plan(TargetType activity_plan);

			// indicators used to determine next iteration for generation/planning events
			accessor(Next_Activity_Generation_Time, NONE, NONE);
			accessor(Next_Planning_Time, NONE, NONE);
			accessor(Planning_Time_Increment, NONE, NONE);
			accessor(Generation_Time_Increment, NONE, NONE);

			// accessor to parent class
			accessor(Parent_Person, NONE, NONE);
		};

		template<typename ComponentType>
		/*static*/ void Person_Planner<ComponentType>::Planning_Event_Controller(ComponentType* _this, Event_Response& response)
		{
			ComponentType::Planning_Event_Controller(_this, response);
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Planner<ComponentType>::Go_To_Subiteration(Scenario_Components::Types::Demand_Sub_Iteration_keys subiteration, Event_Response& response)
		{
			response.next._iteration = iteration();
			response.next._sub_iteration = subiteration;
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Planner<ComponentType>::Go_To_Next_Iteration(Event_Response& response)
		{
			response.next._iteration = Round<long, double>(Simulation_Time.template Future_Time<Simulation_Timestep_Increment, Simulation_Timestep_Increment>(this->Planning_Time_Increment<Simulation_Timestep_Increment>()));
			response.next._sub_iteration = 0;
		}

		//=========================================================================================================
		// Planning events
		template<typename ComponentType>
		template<typename T>
		void Person_Planner<ComponentType>::Activity_Generation_Event()
		{
			// Create alias
			typedef Person_Planner<ComponentType> _Planning_Interface;
			_Planning_Interface* this_ptr = (_Planning_Interface*)this;

			// Call specific implementation of the activity generation routine
			typedef Prototypes::Activity_Generator<typename get_type_of(Activity_Generation_Faculty)> activity_generator_itf;
			activity_generator_itf* generator = this_ptr->template Activity_Generation_Faculty<activity_generator_itf*>();

			generator->template Activity_Generation<T>();

			// set next activity generation occurence
			this_ptr->template Next_Activity_Generation_Time<Simulation_Timestep_Increment>(Round<long, double>(Simulation_Time.template Future_Time<Simulation_Timestep_Increment, Simulation_Timestep_Increment>(this_ptr->template Generation_Time_Increment<Simulation_Timestep_Increment>())));
		}

		template<typename ComponentType>
		template<typename T> 
		void Person_Planner<ComponentType>::Movement_Planning_Event()
		{
			// Create alias
			typedef Person_Planner<ComponentType> _Planning_Interface;
			_Planning_Interface* this_ptr = (_Planning_Interface*)this;

			// create aliases for network components from parent
			typedef Person<typename get_type_of(Parent_Person)> parent_itf;
			typedef Household_Components::Prototypes::Household<typename parent_itf::get_type_of(Household)> household_itf;
			typedef Prototypes::Person_Scheduler<typename parent_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Person_Components::Prototypes::Person_Mover< typename parent_itf::get_type_of(Moving_Faculty)> _Movement_Faculty_Interface;
			typedef Network_Components::Prototypes::Network< typename parent_itf::get_type_of(network_reference)> _Network_Interface;

			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;

			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			typedef Link_Components::Prototypes::Link<get_component_type(_Links_Container_Interface)>  _Link_Interface;

			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;

			typedef Random_Access_Sequence< typename household_itf::get_type_of(Vehicles_Container)> _Vehicles_Container_Interface;
			typedef Vehicle_Components::Prototypes::Vehicle<typename parent_itf::get_type_of(vehicle)> _Vehicle_Interface;

			typedef Back_Insertion_Sequence<typename scheduler_itf::get_type_of(Movement_Plans_Container)> Movement_Plans;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<get_component_type(Movement_Plans)> Movement_Plan;

			typedef Activity_Components::Prototypes::Activity_Planner< typename Movement_Plan::get_type_of(destination_activity_reference)> Activity_Plan;

			parent_itf* parent = this_ptr->template Parent_Person<parent_itf*>();
			//%%%RLW - check this out: parent->parent_itf::template Household<int>();
			//parent->parent_itf::template Household<household_itf*>();
			household_itf* household = parent->Household<household_itf*>();

			_Network_Interface* network = parent->template network_reference<_Network_Interface*>();
			_Movement_Faculty_Interface* movement_faculty = parent->template Moving_Faculty<_Movement_Faculty_Interface*>();


			// Get reference to movement plans
			Movement_Plans* movements = parent->template Scheduling_Faculty<scheduler_itf*>()->template Movement_Plans_Container<Movement_Plans*>();
			typename Movement_Plans::iterator move_itr = movements->begin();

			while (move_itr != movements->end())
			{
				Movement_Plan* move = *move_itr;
				Activity_Plan* act = move->destination_activity_reference<Activity_Plan*>();

				//=====================================================================================================================================================
				// if movement happens in the current planning increment, execute movement
				//=====================================================================================================================================================
				if (move->template departed_time<Simulation_Timestep_Increment>() >= Simulation_Time.template Current_Time<Simulation_Timestep_Increment>() &&
					move->template departed_time<Simulation_Timestep_Increment>() < Simulation_Time.template Future_Time<Simulation_Timestep_Increment, Simulation_Timestep_Increment>(this_ptr->template Planning_Time_Increment<Simulation_Timestep_Increment>()))
				{

					// not moving - reassign to walk mode
					if (move->origin<_Activity_Location_Interface*>() == move->destination<_Activity_Location_Interface*>())
					{
						act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::WALK);
					}

					//===============================================================================
					// IF IT IS AN SOV MOVEMENT - do vehicle selection
					if (act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
					{
						// get current vehicle (s.b. nullptr if starting at home
						_Vehicle_Interface* vehicle = parent->vehicle<_Vehicle_Interface*>();			

						// if no current assigned vehicle, get a new one from the household vehicle list
						if (vehicle == nullptr) vehicle = household->Get_Free_Vehicle<_Vehicle_Interface*>();	

						// vehicle found....
						if (vehicle != nullptr)
						{
							// make sure vehicle is not already being simulated, skip movement if it is
							if (vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::UNLOADED || vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::OUT_NETWORK)
							{
								vehicle->Assign_To_Person(parent);
								vehicle->template movement_plan<Movement_Plan*>(move);
							}
							else
							{
								act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::HOV);
							}
						}
						// no vehicle found, can we wait for a short time until a vehicle is available?
						else
						{
							//act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::HOV);

							// get next vehicle availability time, make sure it is at least in the next movement checking period (so we can ensure that the simulation movements are completed)
							int next_avail_veh_iter = std::max((float)household->Get_Next_Available_Vehicle_Time<Simulation_Timestep_Increment>(), (float)Simulation_Time.template Future_Time<Simulation_Timestep_Increment, Simulation_Timestep_Increment>(this_ptr->template Planning_Time_Increment<Simulation_Timestep_Increment>()+1));

							// vehicle available soon, delay the movement departure time until the estimated availability - **** make sure the new departure time is not in the same movement planning horizon
							if (next_avail_veh_iter + act->Expected_Travel_Time<Simulation_Timestep_Increment>() < act->Start_Time<Simulation_Timestep_Increment>() + act->Duration<Simulation_Timestep_Increment>()*0.5)
							{
								act->Start_Time<Simulation_Timestep_Increment>(next_avail_veh_iter + act->Expected_Travel_Time<Simulation_Timestep_Increment>());
								move->departed_time(next_avail_veh_iter); //THROW_EXCEPTION("NO VEHICLES AVAILABLE");
								typename Movement_Plans::iterator prev = move_itr++;
								continue;
							}
							// no vehicles available, make it an HOV
							else
							{
								act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::HOV); 

							}		
						}

					}

					// increment the zone origin/destination counters based on movement plan
					_Activity_Location_Interface* orig = move->template origin<_Activity_Location_Interface*>();
					_Activity_Location_Interface* dest = move->template destination<_Activity_Location_Interface*>();
					_Zone_Interface* orig_zone = orig->template zone<_Zone_Interface*>();
					_Zone_Interface* dest_zone = dest->template zone<_Zone_Interface*>();

					// if origin or destination zones are unavailable, abort trip
					if (orig_zone->template zone_is_available<bool>() == false || dest_zone->template zone_is_available<bool>() == false)
					{
						cout << endl << "zone is unavailable";
						typename Movement_Plans::iterator prev = move_itr++;
						//movements->erase(prev);
						return;
					}


					// increment counters for display
					orig_zone->template production_count<int&>()++;
					dest_zone->template attraction_count<int&>()++;

					// if already moving, skip movement
					if (movement_faculty->template Movement_Scheduled<bool>() == true)
					{
						typename Movement_Plans::iterator prev = move_itr++;
						//movements->erase(prev);
						return;
					}

					movement_faculty->template Schedule_Movement<Simulation_Timestep_Increment, Movement_Plan*>(move->template departed_time<Simulation_Timestep_Increment>(), move);

					//TODO: CHANGE SO THAT MULTIPLE MOVES CAN BE PLANNED PER PLANNING TIMESTEP - currently we are only simulating the first planned move, then throwing out the rest
					typename Movement_Plans::iterator prev = move_itr++;
					//movements->erase(prev);
				}

				// remove movements which have already been skipped
				else if (move->template departed_time<Simulation_Timestep_Increment>() < Simulation_Time.template Current_Time<Simulation_Timestep_Increment>())
				{
					typename Movement_Plans::iterator prev = move_itr++;
				}
				// exit if no movements in current timestep
				else
				{
					return;
				}

				WHILE_PRINT("movement planning event while loop");
			}
		}


		//=========================================================================================================
		// Member functions
		template<typename ComponentType>
		template<typename TargetType>
		void Person_Planner<ComponentType>::Initialize()
		{
			assert_check(ComponentType, Has_Initialize, "This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");

			this_component()->template Initialize< TargetType>();
			long first_iter = this->Next_Activity_Generation_Time<Simulation_Timestep_Increment>();

			((ComponentType*)this)->template Load_Event<ComponentType>(&Planning_Event_Controller, first_iter, 0);
			//load_event(ComponentType,Planning_Conditional,Activity_Generation_Event,first_iter,0,NULLTYPE);
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Planner<ComponentType>::Initialize(TargetType initializer)
		{
			assert_check(ComponentType, Has_Initialize, "This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");

			this_component()->template Initialize< TargetType>(initializer);
			long first_iter = this->Next_Activity_Generation_Time<Simulation_Timestep_Increment>();

			((ComponentType*)this)->template Load_Event<ComponentType>(&Planning_Event_Controller, first_iter, 0);
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Planner<ComponentType>::Schedule_New_Routing(int planning_time, TargetType movement_plan, requires(TargetType, check(strip_modifiers(TargetType), Movement_Plan_Components::Concepts::Is_Movement_Plan)))
		{
			// interfaces	
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Scenario_Components::Prototypes::Scenario< typename Parent_Person_Itf::get_type_of(scenario_reference)> Scenario_Itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename Routing_Itf::get_type_of(movement_plan)> Movement_Itf;
			typedef Activity_Components::Prototypes::Activity_Planner< typename Movement_Itf::get_type_of(destination_activity_reference)> Activity_Itf;

			// references
			Parent_Person_Itf* person_itf = this->Parent_Person<Parent_Person_Itf*>();
			Routing_Itf* itf = person_itf->template router<Routing_Itf*>();
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
				itf->Schedule_Route_Computation(movement_plan->template departed_time<Simulation_Timestep_Increment>(), planning_time/*,scenario->template read_network_snapshots<bool>()*/);
				return;
			}
			else //if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::SOV) // 6/14/17-JA-Testing multimodal router so removing the restriction to SOV only
			{
				itf->Schedule_Route_Computation(movement_plan->template departed_time<Simulation_Timestep_Increment>()/*, planning_time,scenario->template read_network_snapshots<bool>()*/);
			}

		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Planner<ComponentType>::Schedule_New_Routing(int planning_time, TargetType movement_plan, requires(TargetType, !check(strip_modifiers(TargetType), Movement_Plan_Components::Concepts::Is_Movement_Plan)))
		{
			assert_check(strip_modifiers(TargetType), Movement_Plan_Components::Concepts::Is_Movement_Plan, "ERROR: TargetType is not a valid movement plan component.");
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Planner<ComponentType>::Write_To_Log(TargetType s)
		{
			this_component()->Write_To_Log<TargetType>(s);
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Planner<ComponentType>::Add_Movement_Plan(TargetType movement_plan)
		{
			this_component()->template Add_Movement_Plan<TargetType>(movement_plan);
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Planner<ComponentType>::Add_Activity_Plan(TargetType activity_plan)
		{
			this_component()->template Add_Activity_Plan<TargetType>(activity_plan);
		}
	}

}
