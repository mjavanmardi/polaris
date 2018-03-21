#pragma once

#include "Activity_Simulator_Includes.h"
//#include "traffic_simulator\Network_Event_Prototype.h"

namespace Person_Components
{
	namespace Prototypes
	{
		prototype struct Person_Mover ADD_DEBUG_INFO
		{
			tag_as_prototype;
			typedef typename ComponentType::Master_Type MasterType;

			//========================================================
			// Events
			//--------------------------------------------------------
			static void Movement_Event_Controller(ComponentType* _this, Event_Response& response);

			//========================================================
			// Main hook to person planner - called 5 minutes prior to estimated departure
			//--------------------------------------------------------
			template<typename TimeType, typename MovementItfType> void Schedule_Movement(TimeType departure_time, MovementItfType movement, bool do_routing=true, requires(MovementItfType,
				check(TimeType, Basic_Units::Concepts::Is_Time_Value) &&
				check(strip_modifiers(MovementItfType), Movement_Plan_Components::Concepts::Is_Movement_Plan) &&
				(check(MovementItfType, is_pointer) || check(MovementItfType, is_reference))));
			template<typename TimeType, typename MovementItfType> void Schedule_Movement(TimeType departure_time, MovementItfType movement, bool do_routing=true, requires(MovementItfType,
				!check(strip_modifiers(TimeType), Basic_Units::Concepts::Is_Time_Value) ||
				!check(strip_modifiers(MovementItfType), Movement_Plan_Components::Concepts::Is_Movement_Plan) ||
				(!check(MovementItfType, is_pointer) && !check(MovementItfType, is_reference))));
		
			//========================================================
			// Artificial arrival for non-simulated trips
			//--------------------------------------------------------	
			template<typename T> void Artificial_Arrival_Event();


			//========================================================
			// Information Acquisition Functionality
			//--------------------------------------------------------
			template<typename TargetType> void Do_Pretrip_Information_Acquisition();

			template<typename TargetType> void Evaluate_Network_Event(TargetType event, requires(TargetType, check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Weather_Event_Prototype)));
			template<typename TargetType> void Evaluate_Network_Event(TargetType event, requires(TargetType, check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Accident_Event_Prototype)));
			template<typename TargetType> void Evaluate_Network_Event(TargetType event, requires(TargetType, !check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Weather_Event_Prototype) && !check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Accident_Event_Prototype)));

			template<typename TargetType> bool Is_Event_Relevant(TargetType event, requires(TargetType, check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Weather_Event_Prototype)));
			template<typename TargetType> bool Is_Event_Relevant(TargetType event, requires(TargetType, check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Accident_Event_Prototype)));

	// TODO: does not compile (location_itf definition not found)
			template<typename TargetType> void Get_Event_Extents(TargetType event, std::unordered_set<int>& affected_indices, requires(TargetType, check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Weather_Event_Prototype)));
		
			//========================================================
			// Pre-trip Replanning Functionality
			//--------------------------------------------------------
			template<typename TargetType> void Do_Pretrip_Replanning();

			//========================================================
			// Pre-trip Rerouting Functionality
			//--------------------------------------------------------
			template<typename TargetType> void Do_Pretrip_Routing();

			//========================================================
			// Movement Functionality
			//--------------------------------------------------------
			template<typename TargetType> void Do_Movement();

			//========================================================
			// Arrival Functionality - ends current movement, plans next if necessary
			//--------------------------------------------------------
			void Arrive_At_Destination();

			template<typename TargetType> void Schedule_Artificial_Arrival_Event();

		
			//TODO: Omer Multimodal Movement
			accessor(Next_Sub_Iteration, NONE, NONE);
			template<typename TargetType> void Do_Multimodal_Movement();
			template<typename TargetType> void schdeule_person_movements_in_multimodal_network();
			static void move_persons_in_multimodal_network_conditional(ComponentType* _this, Event_Response& response);
			template<typename TargetType> void teleport_multimodal_person_to_next_link();
			template<typename TargetType> void person_action_at_beginning_of_link();
			template<typename TargetType> void person_waiting_at_beginning_of_link();
			//template<typename TargetType> void person_boarding_transit_vehicle();
			template<typename TargetType> void person_boarded_transit_vehicle(int next_simulation_time);
			template<typename TargetType> void person_alighting_transit_vehicle();
			//--------------------------------------------------------

			//========================================================
			// Basic Forms
			//--------------------------------------------------------
			accessor(Parent_Person, NONE, NONE);
			accessor(Movement, NONE, NONE);
			typed_accessor(bool, Is_Moving);
			accessor(Movement_Scheduled, NONE, NONE);
			accessor(Artificial_Movement_Scheduled, NONE, NONE);
			accessor(Artificial_Arrival_Time, NONE, NONE);
			accessor(Replanning_Needed, NONE, NONE);
		};

		template<typename ComponentType>
		void Person_Mover<ComponentType>::Movement_Event_Controller(ComponentType* _this, Event_Response& response)
		{

			int cur_iter = iteration();
			int cur_sub = sub_iteration();


			typedef Person_Mover<ComponentType> _Person_Mover_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Mover_Interface* pthis = (_Person_Mover_Interface*)_pthis;

			typedef Prototypes::Person< typename get_type_of(Parent_Person)> person_itf;
			typedef Household_Components::Prototypes::Household< typename person_itf::get_type_of(Household)> household_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename person_itf::get_type_of(scenario_reference)> scenario_itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename person_itf::get_type_of(vehicle)> vehicle_itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef Activity_Components::Prototypes::Activity_Planner< typename movement_itf::get_type_of(destination_activity_reference)> activity_itf;

			movement_itf* movement = pthis->template Movement<movement_itf*>();
			person_itf* person = pthis->template Parent_Person<person_itf*>();
			vehicle_itf* vehicle = person->template vehicle<vehicle_itf*>();
			scenario_itf* scenario = (scenario_itf*)_global_scenario;
				activity_itf* act = movement->destination_activity_reference<activity_itf*>();

			// determine router aggregation properties - i.e. is routed called at departure or an aggregated time prior to departure
			Simulation_Timestep_Increment routing_timestep = movement->template departed_time<Simulation_Timestep_Increment>() - 1;
			if (scenario->template aggregate_routing<bool>())
			{
				int minutes = (int)(movement->template departed_time<Time_Minutes>());
				Simulation_Timestep_Increment temp = Simulation_Time.Convert_Time_To_Simulation_Timestep<Time_Minutes>(minutes);
				routing_timestep = max<int>(temp, iteration());
			}
			if (routing_timestep < iteration()) routing_timestep = iteration();


			// Do pretrip replanning if car has realtime info
			bool has_pretrip_info = person->template has_pretrip_information<bool>();


			// GO TO ARTIFICIAL ARRIVAL EVENT - S.B. only time that this is true
			if (pthis->template Artificial_Movement_Scheduled<bool>() == true)
			{
				response.next._iteration = END;
				response.next._sub_iteration = END;

				pthis->Artificial_Arrival_Event<NT>();
			}

			// DO PRE-TRIP PLANNING, THEN SCHEDULE NEXT ITERATION FOR DEPARTURE TIME
			else if (sub_iteration() == Scenario_Components::Types::PRETRIP_INFORMATION_ACQUISITION_SUB_ITERATION)
			{
				response.next._iteration = iteration();
				response.next._sub_iteration = Scenario_Components::Types::PRETRIP_PLANNING_SUB_ITERATION;

				if (has_pretrip_info) pthis->Do_Pretrip_Information_Acquisition<NT>();
			}
			else if (sub_iteration() == Scenario_Components::Types::PRETRIP_PLANNING_SUB_ITERATION)
			{
				response.next._iteration = routing_timestep;
				response.next._sub_iteration = Scenario_Components::Types::PRETRIP_ROUTING_SUB_ITERATION;

				if (pthis->template Replanning_Needed<bool>()) pthis->Do_Pretrip_Replanning<NT>();
			}
			else if (sub_iteration() == Scenario_Components::Types::PRETRIP_ROUTING_SUB_ITERATION)
			{
				if (movement->template departed_time<Simulation_Timestep_Increment>() < iteration() + 1)
				{
					movement->template departed_time<Simulation_Timestep_Increment>(iteration() + 1);
				}
				response.next._iteration = movement->template departed_time<Simulation_Timestep_Increment>();
				response.next._sub_iteration = Scenario_Components::Types::MOVEMENT_SUB_ITERATION;

				pthis->Do_Pretrip_Routing<NT>();
			}
			// GO TO DEPARTURE TIMESTEP
			else if (sub_iteration() == Scenario_Components::Types::MOVEMENT_SUB_ITERATION)
			{
				response.next._iteration = END;
				response.next._sub_iteration = END;

			

				//TODO: Omer: Multimodal
				Vehicle_Components::Types::Vehicle_Type_Keys mode = movement->template mode<Vehicle_Components::Types::Vehicle_Type_Keys>();

				if (((scenario_itf*)_global_scenario)->template multimodal_routing<bool>() && (mode == Vehicle_Components::Types::Vehicle_Type_Keys::BUS || mode == Vehicle_Components::Types::Vehicle_Type_Keys::RAIL || mode == Vehicle_Components::Types::Vehicle_Type_Keys::WALK || mode == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE || mode == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE || mode == Vehicle_Components::Types::Vehicle_Type_Keys::KISS_AND_RIDE))
				{
					pthis->Do_Multimodal_Movement<NT>();
					response.next._iteration = pthis->template Next_Simulation_Time<Simulation_Timestep_Increment>();
					response.next._sub_iteration = pthis->template Next_Sub_Iteration<int>();
				}
				else
				{
					pthis->Do_Movement<NT>();
				}

				if (pthis->Artificial_Movement_Scheduled<bool>() == true)
				{
					response.next._iteration = pthis->Artificial_Arrival_Time<int>();
					response.next._sub_iteration = 0;
				}
			}


			// ERROR SHOULDN"T REACH HERE
			else
			{
				THROW_EXCEPTION("ERROR: should not reach this point in conditional, improper response.revision set at some point.");
				response.next._iteration = END;
				response.next._sub_iteration = END;
				//response.result = false;
			}
		}

		//========================================================
		// Main hook to person planner - called 5 minutes prior to estimated departure
		//--------------------------------------------------------
		template<typename ComponentType>
		template<typename TimeType, typename MovementItfType> 
		void Person_Mover<ComponentType>::Schedule_Movement(TimeType departure_time, MovementItfType movement, bool do_routing=true, requires(MovementItfType,
			check(TimeType, Basic_Units::Concepts::Is_Time_Value) &&
			check(strip_modifiers(MovementItfType), Movement_Plan_Components::Concepts::Is_Movement_Plan) &&
			(check(MovementItfType, is_pointer) || check(MovementItfType, is_reference))))
		{
			this->Movement<MovementItfType>(movement);
			this->Movement_Scheduled<bool>(true);

			int iter = Simulation_Time.template Convert_Time_To_Simulation_Timestep<TimeType>(iteration() + 1);

			if (do_routing)
			{
				// if departure_time is greater than current iteration, load pre-trip stuff on current iteration, otherwise skip pretrip and schedule departure
			
				if (departure_time > iteration() + 2)
				{
					static_cast<ComponentType*>(this)->template Load_Event<ComponentType>(&Movement_Event_Controller, iter, Scenario_Components::Types::PRETRIP_INFORMATION_ACQUISITION_SUB_ITERATION);
					//load_event(ComponentType,Movement_Conditional,Pretrip_Information_Acquisition_Event,iter,Scenario_Components::Types::PRETRIP_INFORMATION_ACQUISITION,NULLTYPE);
				}
				else
				{
					static_cast<ComponentType*>(this)->template Load_Event<ComponentType>(&Movement_Event_Controller, iter, Scenario_Components::Types::PRETRIP_ROUTING_SUB_ITERATION);
					//load_event(ComponentType,Movement_Conditional,Pretrip_Routing_Event,iter,Scenario_Components::Types::END_OF_ITERATION,NULLTYPE);	
				}
			}
			else
			{
				static_cast<ComponentType*>(this)->template Load_Event<ComponentType>(&Movement_Event_Controller, departure_time, Scenario_Components::Types::MOVEMENT_SUB_ITERATION);
			}
		}

		template<typename ComponentType>
		template<typename TimeType, typename MovementItfType>
		void Person_Mover<ComponentType>::Schedule_Movement(TimeType departure_time, MovementItfType movement, bool do_routing = true, requires(MovementItfType,
			!check(strip_modifiers(TimeType), Basic_Units::Concepts::Is_Time_Value) ||
			!check(strip_modifiers(MovementItfType), Movement_Plan_Components::Concepts::Is_Movement_Plan) ||
			(!check(MovementItfType, is_pointer) && !check(MovementItfType, is_reference))))
		{
			assert_check(strip_modifiers(TimeType), Basic_Units::Concepts::Is_Time_Value, "Error, must use a valid time value when scheduling departure.");
			assert_check(strip_modifiers(MovementItfType), Movement_Plan_Components::Concepts::Is_Movement_Plan, "Error, movement parameter is not a valid Movement_Plan interface.");
			assert_check(MovementItfType, is_pointer, "Error, must pass movement plan interface as a pointer or reference.");
		}

		//========================================================
		// Artificial arrival for non-simulated trips
		//--------------------------------------------------------	
		template<typename ComponentType>
		template<typename T>
		void Person_Mover<ComponentType>::Artificial_Arrival_Event()
		{
			typedef Person_Mover<ComponentType> _Person_Interface;
			_Person_Interface* pthis = (_Person_Interface*)this;

			pthis->template Artificial_Movement_Scheduled<bool>(false);

			typedef  Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef  Vehicle_Components::Prototypes::Vehicle< typename Parent_Person_Itf::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef Activity_Components::Prototypes::Activity_Planner< typename movement_itf::get_type_of(destination_activity_reference)> Activity_Itf;

			Parent_Person_Itf* person = pthis->Parent_Person<Parent_Person_Itf*>();
			movement_itf* movements = pthis->Movement<movement_itf*>();
			Activity_Itf* act = movements->template destination_activity_reference<Activity_Itf*>();

			act->Arrive_At_Activity();
		}


		//========================================================
		// Information Acquisition Functionality
		//--------------------------------------------------------
		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::Do_Pretrip_Information_Acquisition()
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Person_Components::Prototypes::Person< typename Parent_Person_Itf::get_type_of(Household)> Household_Itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<get_component_type(links)>  link_itf;

			typedef Random_Access_Sequence< typename link_itf::get_type_of(activity_locations)> locations;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(locations)>  location_itf;

			typedef Pair_Associative_Container< typename network_itf::get_type_of(zones_container)> zones;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones)>  zone_itf;



			// HAR interface
			typedef  Advisory_ITS_Components::Prototypes::Advisory_ITS< typename link_itf::get_type_of(advisory_radio)> advisory_radio_itf;


			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Routing_Itf* itf = person ->template router<Routing_Itf*>();
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			network_itf* network = person->template network_reference<network_itf*>();
			movement_itf* movements = this->Movement<movement_itf*>();
			location_itf* destination = movements->template destination<location_itf*>();

			link_itf* origin_link = movements->template origin<link_itf*>();
			advisory_radio_itf* har = origin_link->template advisory_radio<advisory_radio_itf*>();


			typedef Network_Event<typename Component_Type::base_network_event_type> event_itf;
			typedef Network_Event<typename Component_Type::weather_network_event_type> weather_itf;
			typedef Network_Event<typename Component_Type::accident_network_event_type> accident_itf;

			if (har != nullptr)
			{
				std::vector<event_itf*> base_events;
				har->template Get_Displayed_Messages<typename Component_Type::base_network_event_type>(base_events);

				// process weather events from HAR
				std::vector<weather_itf*> weather_events;
				har->template Get_Displayed_Messages<typename Component_Type::weather_network_event_type>(weather_events);
				for (typename std::vector<weather_itf*>::iterator w_itr = weather_events.begin(); w_itr != weather_events.end(); ++w_itr)
				{
					this->Evaluate_Network_Event<weather_itf*>(*w_itr);
				}

				// process accident events from HAR
				std::vector<accident_itf*> accident_events;
				har->template Get_Displayed_Messages<typename Component_Type::accident_network_event_type>(accident_events);
				for (typename std::vector<accident_itf*>::iterator a_itr = accident_events.begin(); a_itr != accident_events.end(); ++a_itr)
				{
					this->Evaluate_Network_Event<accident_itf*>(*a_itr);
				}
			}

		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::Evaluate_Network_Event(TargetType event, requires(TargetType, check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Weather_Event_Prototype)))
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Person_Components::Prototypes::Person< typename Parent_Person_Itf::get_type_of(Household)> Household_Itf;
			typedef Person_Components::Prototypes::Person_Planner< typename Parent_Person_Itf::get_type_of(Planning_Faculty)> planning_itf;
			typedef Person_Components::Prototypes::Person_Scheduler< typename Parent_Person_Itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Person_Components::Prototypes::Destination_Chooser< typename planning_itf::get_type_of(Destination_Choice_Faculty)> destination_chooser_itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<get_component_type(links)>  link_itf;

			typedef Random_Access_Sequence< typename link_itf::get_type_of(activity_locations)> locations;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(locations)>  location_itf;

			typedef Pair_Associative_Container< typename network_itf::get_type_of(zones_container)> zones;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones)>  zone_itf;

			typedef Random_Access_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> activity_container_itf;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(activity_container_itf)>  activity_itf;


			// person, router, etc. interfaces
			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			planning_itf* planner = person->template Planning_Faculty<planning_itf*>();
			destination_chooser_itf* dest_chooser = planner->template Destination_Choice_Faculty<destination_chooser_itf*>();
			Routing_Itf* itf = person ->template router<Routing_Itf*>();
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			network_itf* network = person->template network_reference<network_itf*>();
			movement_itf* movement = this->Movement<movement_itf*>();
			activity_itf* activity = movement->template destination_activity_reference<activity_itf*>();
			Activity_Components::Types::ACTIVITY_TYPES act_type = activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();

			// event interface
			typedef Network_Event_Components::Prototypes::Network_Event<typename Component_Type::weather_network_event_type> weather_itf;
			weather_itf* weather_event = (weather_itf*)event;

			// determine if destination activity is flexible
			Activity_Components::Types::FLEXIBILITY_VALUES flexibility = activity->template Location_Flexibility<Activity_Components::Types::FLEXIBILITY_VALUES>();


			// If event affects traveler
			if (this->Is_Event_Relevant<weather_itf*>(weather_event)/* && flexibility != Activity_Components::Types::FLEXIBILITY_VALUES::LOW_FLEXIBILITY*/)
			{
				this->Replanning_Needed<bool>(true);
				person->template has_done_replanning<bool>(true);

				// get all zones affected by weather
				std::vector<location_itf*> *unaffected_locations = weather_event->template unaffected_locations<std::vector<location_itf*>*>();

				// replan, select new zone ouside of affected area
				location_itf* orig, *dest, *next;
				zone_itf* zone_dest = movement->template destination<zone_itf*>();
				int old_dest_id = zone_dest == nullptr ? -1 : movement->template destination<zone_itf*>()->template uuid<int>();

				if (old_dest_id == -1)
				{
					cout << endl << "Why is this destination null? personid=" << person->template uuid<int>() << ": type=" << activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
				}

				activity_itf* prev_act = person->template previous_activity_plan<Simulation_Timestep_Increment, activity_itf*>(iteration());
				if (prev_act == nullptr) orig = person->template Home_Location<location_itf*>();
				else orig = prev_act->template Location<location_itf*>();

				activity_itf* next_act = person->template next_activity_plan<Simulation_Timestep_Increment, activity_itf*>(iteration());
				if (next_act == nullptr) next = person->template Home_Location<location_itf*>();
				else next = next_act->template Location<location_itf*>();

				dest = dest_chooser->template Choose_Destination<activity_itf*, location_itf*>(activity, unaffected_locations);

				if (dest != nullptr)
				{
					movement->template destination<location_itf*>(dest);
					movement->template destination<link_itf*>((link_itf*)(dest->template origin_links<links*>()->at(0)));
					activity_itf* act = movement->template destination_activity_reference<activity_itf*>();
					act->template Location<location_itf*>(dest);

					// cout << endl << "Destination Replanned for person '"<<person->Household<Household_Itf*>()->uuid<int>() <<"."<<person->uuid<int>()  <<"' due to weather, switch from zone '" << old_dest_id << "', to new zone '" << dest->zone<zone_itf*>()->uuid<int>()<<"'.";

					Time_Seconds ttime = network->template Get_TTime<location_itf*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Seconds, Time_Seconds>(orig, dest, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, iteration());

					// update start time
					Time_Seconds old_start = act->template Start_Time<Time_Seconds>();
					act->template Start_Time<Time_Seconds>(iteration() + ttime);
					//cout << "Destination replanned due to weather, start time updated from " << old_start << " to " << act->template Start_Time<Time_Seconds>();
				}
				else
				{
					if (movement->template destination<location_itf*>() == nullptr) THROW_WARNING("ERROR: valid destination not set for activity.");
				}
			}
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::Evaluate_Network_Event(TargetType event, requires(TargetType, check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Accident_Event_Prototype)))
		{
			typedef Network_Event_Components::Prototypes::Network_Event<typename Component_Type::accident_network_event_type> accident_itf;
			accident_itf* accident_event = (accident_itf*)event;

			//cout << endl << "EVALUATING ACCIDENT EVENT: ";

			if (this->Is_Event_Relevant<accident_itf*>(accident_event))
			{
				cout << endl << "ACCIDENT_ADVISORY: there is an accident along your route.";
			}
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::Evaluate_Network_Event(TargetType event, requires(TargetType, !check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Weather_Event_Prototype) && !check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Accident_Event_Prototype)))
		{
			// TODO: evaluatable before function overloading
			//assert_check(strip_modifiers(TargetType), Is_Prototype, "Warning: TargetType event must be a polaris prototype.");
			assert_check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Weather_Event_Prototype, "Warning: TargetType component must be a weather_network_event, or ");
			assert_check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Accident_Event_Prototype, "TargetType component must be an accident_network_event_type.");
		}

		template<typename ComponentType>
		template<typename TargetType>
		bool Person_Mover<ComponentType>::Is_Event_Relevant(TargetType event, requires(TargetType, check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Weather_Event_Prototype)))
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Person_Components::Prototypes::Person_Scheduler< typename Parent_Person_Itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;

			typedef Random_Access_Sequence<typename movement_itf::get_type_of(trajectory_container)> trajectory_interface;
			typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<get_component_type(trajectory_interface)> trajectory_unit_interface;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<get_component_type(links)>  link_itf;

			typedef Random_Access_Sequence< typename link_itf::get_type_of(activity_locations)> locations;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(locations)>  location_itf;

			typedef Pair_Associative_Container< typename network_itf::get_type_of(zones_container)> zones;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones)>  zone_itf;

			typedef Random_Access_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> activity_container_itf;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(activity_container_itf)>  activity_itf;


			// get destination link for current movement
			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			movement_itf* movement = this->Movement<movement_itf*>();
			zone_itf* destination = movement->template destination<zone_itf*>();
			activity_itf* activity = movement->template destination_activity_reference<activity_itf*>();
			//link_itf* destination_link = movement->destination<link_itf*>();

			// interface to event
			typedef Network_Event<typename Component_Type::weather_network_event_type> weather_itf;
			weather_itf* my_event = (weather_itf*)event;

			// does event affect destination zone?
			std::vector<zone_itf*> *affected_zones = my_event->template affected_zones<std::vector<zone_itf*> *>();

			// event is not relevent if going to home/work/school as these locations are fixed.

			if (activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY ||
				activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::PRIMARY_WORK_ACTIVITY ||
				activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::PART_TIME_WORK_ACTIVITY ||
				activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::SCHOOL_ACTIVITY ||
				activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::WORK_AT_HOME_ACTIVITY)
			{
				return false;
			}

			zone_itf* affected_zone;
			for (auto itr = affected_zones->begin(); itr != affected_zones->end(); ++itr)
			{
				affected_zone = *itr;

				if (affected_zone == destination /*|| destination == nullptr*/)
				{
					return true;
				}
			}
			return false;

			//// does event affect destination link?
			//links* affected_links = my_event->affected_links<links*>();
			//link_itf* affected_link;
			//for (links::iterator itr = affected_links->begin(); itr != affected_links->end(); ++itr)
			//{
			//	affected_link = *itr;
			//	if (affected_link == destination_link) return true;
			//}
			//return false;
		}

		template<typename ComponentType>
		template<typename TargetType>
		bool Person_Mover<ComponentType>::Is_Event_Relevant(TargetType event, requires(TargetType, check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Accident_Event_Prototype)))
		{
			// interfaces
			typedef  Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef  Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef  Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef  Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;

			typedef Random_Access_Sequence<typename movement_itf::get_type_of(trajectory_container)> trajectory_interface;
			typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<get_component_type(trajectory_interface)> trajectory_unit_interface;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<get_component_type(links)>  link_itf;


			// get destination link for current movement
			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			movement_itf* movement = this->Movement<movement_itf*>();
			trajectory_interface* trajectory = movement->template trajectory_container<trajectory_interface*>();
			trajectory_unit_interface* traj_unit;

			// interface to event
			typedef Network_Event<typename Component_Type::weather_network_event_type> weather_itf;
			weather_itf* my_event = (weather_itf*)event;

			// does event affect destination link?
			links* affected_links = my_event->template affected_links<links*>();
			link_itf* affected_link;
			for (typename links::iterator itr = affected_links->begin(); itr != affected_links->end(); ++itr)
			{
				affected_link = (link_itf*)(*itr);
				for (typename trajectory_interface::iterator t_itr = trajectory->begin(); t_itr != trajectory->end(); ++t_itr)
				{
					traj_unit = (trajectory_unit_interface*)(*t_itr);
					if (affected_link == traj_unit->template link<link_itf*>()) return true;
				}
			}
			return false;
		}

		// TODO: does not compile (location_itf definition not found)
		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::Get_Event_Extents(TargetType event, std::unordered_set<int>& affected_indices, requires(TargetType, check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Weather_Event_Prototype)))
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;

			typedef Random_Access_Sequence<typename movement_itf::get_type_of(trajectory_container)> trajectory_interface;
			typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<get_component_type(trajectory_interface)> trajectory_unit_interface;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<get_component_type(links)>  link_itf;

			typedef Random_Access_Sequence< typename link_itf::get_type_of(activity_locations)> locations;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(locations)>  location_itf;

			typedef Pair_Associative_Container< typename network_itf::get_type_of(zones_container)> zones;
			typedef Zone_Components::Prototypes::Zone<get_component_type(zones)>  zone_itf;


			// interface to event
			typedef Network_Event<typename Component_Type::weather_network_event_type> weather_itf;
			weather_itf* my_event = (weather_itf*)event;

			// does event affect destination link?
			links* affected_links = my_event->template affected_links<links*>();
			link_itf* affected_link;
			for (typename links::iterator itr = affected_links->begin(); itr != affected_links->end(); ++itr)
			{
				affected_link = *itr;
				location_container_itf* locations = affected_link->template activity_locations<location_container_itf*>();
				if (locations->size() > 0)
				{
					location_itf* loc = locations->at(0);
					affected_indices.insert(loc->template zone<zone_itf*>()->template uuid<int>());
				}
			}
		}

		//========================================================
		// Pre-trip Replanning Functionality
		//--------------------------------------------------------
		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::Do_Pretrip_Replanning()
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<get_component_type(links)>  link_itf;


			typedef  Person_Components::Prototypes::Person_Data_Logger< typename ComponentType::Master_Type::person_data_logger_type> _Logger_Interface;

			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Routing_Itf* itf = person ->template router<Routing_Itf*>();
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			network_itf* network = person->template network_reference<network_itf*>();
			movement_itf* movements = this->Movement<movement_itf*>();
			link_itf* origin_link = movements->template origin<link_itf*>();

			// increment the replanned activities counter
			_Logger_Interface* logger = (_Logger_Interface*)_global_person_logger;
			logger->template Increment_Replanned_Activities<NT>();
		}

		//========================================================
		// Pre-trip Rerouting Functionality
		//--------------------------------------------------------
		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::Do_Pretrip_Routing()
		{
			// interfaces
			typedef  Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Household_Components::Prototypes::Household<typename Parent_Person_Itf::get_type_of(Household)> Household_Itf;
			typedef Prototypes::Person_Planner< typename Parent_Person_Itf::get_type_of(Planning_Faculty)> Planning_Itf;
			typedef  Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef  Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef  Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;
			typedef  Activity_Location_Components::Prototypes::Activity_Location< typename Parent_Person_Itf::get_type_of(current_location)> location_itf;
			typedef Activity_Components::Prototypes::Activity_Planner< typename movement_itf::get_type_of(destination_activity_reference)> Activity_Itf;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<get_component_type(links)>  link_itf;


			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Household_Itf* household = person->Parent_Person_Itf::template Household<Household_Itf*>();
			Planning_Itf* planner = person->template Planning_Faculty<Planning_Itf*>();
			movement_itf* movements = this->Movement<movement_itf*>();
			Activity_Itf* act = movements->template destination_activity_reference<Activity_Itf*>();


			//TODO: Omer - Everthing is put under if condition to enable routing from demand table
			if (act != nullptr && planner != nullptr)
			{
				// Skip routing if the mode is not SOV (remove when transit router is developed - 6/14/17 - Testing multimodal router developmen so commenting out this line below
				movements->mode(act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>());
				//if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() != Vehicle_Components::Types::Vehicle_Type_Keys::SOV) return;

				//=================================================
				// Do routing at the next timestep for the movement
				planner->template Schedule_New_Routing<movement_itf*>(iteration() + 1, movements);
			}
			else
			{
				Routing_Itf* router = person->template router<Routing_Itf*>();
				router->Schedule_Route_Computation(movements->template departed_time<Simulation_Timestep_Increment>());
			}
		}

		//========================================================
		// Movement Functionality
		//--------------------------------------------------------
		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::Do_Movement()
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Household_Components::Prototypes::Household< typename Parent_Person_Itf::get_type_of(Household)> Household_Itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location< typename Parent_Person_Itf::get_type_of(current_location)> location_itf;
			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<get_component_type(links)>  link_itf;
			typedef Activity_Components::Prototypes::Activity_Planner< typename movement_itf::get_type_of(destination_activity_reference)> Activity_Itf;
			typedef Pair_Associative_Container< typename network_itf::get_type_of(zones_container)> zones;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones)>  zone_itf;

			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Household_Itf* household = person->Parent_Person_Itf::template Household<Household_Itf*>();
			Routing_Itf* itf = person ->template router<Routing_Itf*>();
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			//network_itf* network = person->template network_reference<network_itf*>();
			movement_itf* movements = this->Movement<movement_itf*>();
			Activity_Itf* act = movements->template destination_activity_reference<Activity_Itf*>();
			Vehicle_Components::Types::Vehicle_Type_Keys mode = movements->mode<Vehicle_Components::Types::Vehicle_Type_Keys>();

			this->Is_Moving(true);

			/// CHECK IF THE TRIP CAN SUBSTITUTE WALK MODE FOR SOV BASED ON DISTANCE
			//location_itf* orig = movements->template origin<location_itf*>();
			//location_itf* dest = movements->template destination<location_itf*>();
			//zone_itf* dest_zone = dest->template zone<zone_itf*>();
			//Miles dist = dest->template distance<location_itf*,Miles>(orig);
			//Miles max_walk_distance = 0;
			//if (dest_zone->template areatype<int>() == 1) max_walk_distance = 1.0;			// CBD
			//else if (dest_zone->template areatype<int>() == 2) max_walk_distance = 0.75;	// Downtown
			//else if (dest_zone->template areatype<int>() == 3) max_walk_distance = 0.33;		// Rest of chicago
			//else if (dest_zone->template areatype<int>() >= 4 && dest_zone->template areatype<int>() >= 6) max_walk_distance = 0.33; // Suburban
			//else if (dest_zone->template areatype<int>() == 7) max_walk_distance = 0.2;	// Exurb
			//else max_walk_distance = 0.0;	
			//if (dist <max_walk_distance) act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::WALK);

			// If no movement involved - i.e. different activity at same location, do automatic arrival
			if (movements->template origin<location_itf*>() == movements->template destination<location_itf*>())
			{
				this->Schedule_Artificial_Arrival_Event<NT>();
			}
			else if (movements->template origin<link_itf*>() == movements->template destination<link_itf*>())
			{
				this->Schedule_Artificial_Arrival_Event<NT>();
			}
			// for all non-auto modes, jump to activity arrival (will be replaced with simulation at some point.....)
			else if (mode != Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
			{
				this->Schedule_Artificial_Arrival_Event<NT>();
			}
			// Schedule the routing if the vehicle is not already in the network, otherwise return false
			else if (movements->template valid_trajectory<bool>() && (vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::UNLOADED || vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::OUT_NETWORK))
			{
				// set the persons location to be the destination
				person->template current_location<location_itf*>(movements->template destination<location_itf*>());

				// if auto trip, push to network, if not skip (for now)
				if (mode == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
				{
					link_itf* origin_link = movements->template origin<link_itf*>();
					origin_link->push_vehicle_from_origin(vehicle);
				}
				// for all other trips - do magic move and arrive at activity
				else
				{
					this->Schedule_Artificial_Arrival_Event<NT>();
				}
			}
			// else, if no valid trajectory, unschedule movement
			else
			{
				THROW_WARNING("Warning: invalid movement trajectory specified.  Trip advanced to end-point without simulation.");
				this->Schedule_Artificial_Arrival_Event<NT>();
			}
		}

		//========================================================
		// Arrival Functionality - ends current movement, plans next if necessary
		//--------------------------------------------------------
		template<typename ComponentType>
		void Person_Mover<ComponentType>::Arrive_At_Destination()
		{

			// free up movement schedule
			this->Is_Moving(false);
			this->Movement_Scheduled<bool>(false);

			// interfaces
			typedef Vehicle_Components::Types::Vehicle_Type_Keys MODE;
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Household_Components::Prototypes::Household< typename Parent_Person_Itf::get_type_of(Household)> Household_Itf;
			typedef Person_Components::Prototypes::Person_Scheduler< typename Parent_Person_Itf::get_type_of(Scheduling_Faculty)> Scheduler_Itf;
			typedef Person_Components::Prototypes::Person_Scheduler< typename Parent_Person_Itf::get_type_of(Scheduling_Faculty)> Scheduler_Itf;
			typedef Person_Components::Prototypes::Person_Planner< typename Parent_Person_Itf::get_type_of(Planning_Faculty)> Planner_Itf;
			typedef Person_Components::Prototypes::Activity_Generator< typename Planner_Itf::get_type_of(Activity_Generation_Faculty)> Generator_Itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename network_itf::get_type_of(skimming_faculty)> skim_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location< typename Parent_Person_Itf::get_type_of(current_location)> location_itf;
			typedef Zone_Components::Prototypes::Zone< typename location_itf::get_type_of(zone)> zone_itf;
			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links_container_itf;
			typedef Link_Components::Prototypes::Link<get_component_type(links_container_itf)>  link_itf;
			typedef Random_Access_Sequence< typename network_itf::get_type_of(turn_movements_container)> turns_container_itf;
			typedef Turn_Movement_Components::Prototypes::Movement<get_component_type(turns_container_itf)>  turn_itf;
			typedef Activity_Components::Prototypes::Activity_Planner< typename movement_itf::get_type_of(destination_activity_reference)> Activity_Itf;
			typedef Activity_Components::Prototypes::Activity_Planner<typename ComponentType::Master_Type::at_home_activity_plan_type> at_home_activity_itf;
			typedef  Person_Components::Prototypes::template Person_Data_Logger< typename MasterType::person_data_logger_type> _Logger_Interface;


			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Household_Itf* household = person->Parent_Person_Itf::template Household<Household_Itf*>();
			Planner_Itf* planner = person->template Planning_Faculty<Planner_Itf*>();
			Routing_Itf* itf = person->template router<Routing_Itf*>();
			Generator_Itf* generator = planner->template Activity_Generation_Faculty<Generator_Itf*>();
			Scheduler_Itf* scheduler = person->template Scheduling_Faculty<Scheduler_Itf*>();
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			network_itf* network = person->template network_reference<network_itf*>();
			skim_itf* skim = network->template skimming_faculty<skim_itf*>();
			movement_itf* movements = this->Movement<movement_itf*>();



			//=====================================================================
			// schedule departure from destination if no following activity
			Activity_Itf* act = movements->template destination_activity_reference<Activity_Itf*>();
			movements->template arrived_time<Simulation_Timestep_Increment>(iteration());
			location_itf* destination = movements->template destination<location_itf*>();


			//=========================================================================================================================
			// Update activity start time to handle late arrival
			//-------------------------------------------------------------------------------------------------------------------------
			Time_Seconds new_end = act->template End_Time<Time_Seconds>();
			if (iteration() > act->template Start_Time<Simulation_Timestep_Increment>())
			{
				Activity_Itf* next_act = scheduler->template next_activity_plan<Activity_Itf*, Activity_Itf*>(act);
				movement_itf* next_movement;
				if (next_act != nullptr)
				{
					next_movement = next_act->template movement_plan<movement_itf*>();
					if (!next_act->Route_Is_Planned()) next_movement = nullptr;
				}

				// no following act, so just shift end
				if (next_act == nullptr)
				{
					new_end = iteration() + act->template Duration<Simulation_Timestep_Increment>();
				}
				else if (next_movement == nullptr && iteration() + act->template Duration<Simulation_Timestep_Increment>() < next_act->template Start_Time<Simulation_Timestep_Increment>())
				{
					new_end = iteration() + act->template Duration<Simulation_Timestep_Increment>();
				}
				// following act does not depart before end of shifted current act, so just shift end
				else if (next_movement != nullptr && iteration() + act->template Duration<Simulation_Timestep_Increment>() < next_movement->template departed_time<Simulation_Timestep_Increment>())
				{
					new_end = iteration() + act->template Duration<Simulation_Timestep_Increment>();
				}

				// Very far past original arrival time, shorten activity and check next activity start
				else if (act->template End_Time<Time_Seconds>() < iteration() + act->template Minimum_Duration<Time_Seconds>())
				{
					// reset end of activity
					new_end = iteration() + act->template Minimum_Duration<Time_Seconds>();

					// delete all activities which are completely overlapped by adjusted minimum activity due to late arrival
					bool activity_deleted = false;
					while (true)
					{
						// get next activity 
						next_act = scheduler->template next_activity_plan<Activity_Itf*, Activity_Itf*>(act);
						if (next_act == nullptr) break;

						// get associated movement plan
						next_movement = next_act->template movement_plan<movement_itf*>();

						// drop next activity and go to the following if completely overlapped
						if (next_act->template End_Time<Time_Seconds>() - next_act->template Minimum_Duration<Time_Seconds>() - next_act->template Expected_Travel_Time<Time_Seconds>() < new_end)
						{
							scheduler->template Remove_Activity_Plan<Activity_Itf*>(next_act);
							//scheduler->Remove_Movement_Plan<movement_itf*>(next_movement);
							activity_deleted = true;
							THROW_WARNING("Activity Plan removed due to late arrival at preceding activity: ");
						}
						// no conflict with next remaining activity so break
						else if (next_movement->template departed_time<Time_Seconds>() > new_end)
						{
							// update movement plan to account for deleted activities
							if (activity_deleted) next_act->template Update_Movement_Plan<location_itf*>(act->template Location<location_itf*>(), next_act->template Location<location_itf*>(), new_end);
							break;
						}
						// resolvable conflict with next activity, so shift departure then break
						else
						{
							// update movement plan to account for deleted activities
							if (activity_deleted) next_act->template Update_Movement_Plan<location_itf*>(act->template Location<location_itf*>(), next_act->template Location<location_itf*>(), new_end);

							Time_Seconds next_end = next_act->template End_Time<Time_Seconds>();
							next_movement->template departed_time<Time_Seconds>(new_end);
							next_act->template Start_Time<Time_Seconds>(next_movement->template departed_time<Time_Seconds>() + next_act->template Expected_Travel_Time<Time_Seconds>());
							next_act->template End_Time<Time_Seconds>(next_end, false);
							break;
						}

						WHILE_PRINT("person mover - arrive at destination while loop");
					}
				}
				else
				{

				}

			}
			act->template Start_Time<Simulation_Timestep_Increment>(iteration());
			act->template End_Time<Time_Seconds>(new_end, false);



			// Define time window after current activity is completed
			Activity_Itf* next_act = scheduler->template next_activity_plan<Simulation_Timestep_Increment, Activity_Itf*>(iteration());
			Time_Seconds end_this = act->template Start_Time<Time_Seconds>() + act->template Duration<Time_Seconds>();
			location_itf* orig = act->template Location<location_itf*>();
			location_itf* home = person->template Home_Location<location_itf*>();
			Time_Seconds ttime_this_to_home = network->template Get_TTime<location_itf*, MODE, Time_Seconds, Time_Seconds>(orig, home, MODE::SOV, end_this);


			//=========================================================================================================================
			// Don't add additional movement if already at home, just update the end time of the current at home activity
			//-------------------------------------------------------------------------------------------------------------------------
			if (act->template Location<location_itf*>() == person->template Home_Location<location_itf*>() && act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY)
			{
				if (next_act != nullptr)
				{
					movement_itf* next_movement = next_act->template movement_plan<movement_itf*>();
					act->template End_Time<Time_Seconds>(next_movement->template departed_time<Time_Seconds>(), false);
				}
				else
				{
					act->template End_Time<Time_Seconds>(END, false);
				}
				((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity_Itf*>(act, true);

				//=====================================================================
				// updated the person scheduler to make this activity the current activity for future planning 
				// then remove from the scheduler activity and movement containers
				scheduler->template Update_Current_Activity<Activity_Itf*>(act);
				scheduler->template Remove_Activity_Plan<Activity_Itf*>(act);
			}

			//=========================================================================================================================
			// Else if this is the last act of the day, then return home afterward
			//-------------------------------------------------------------------------------------------------------------------------
			else if (next_act == nullptr)
			{
				// GENERATE A NEW AT HOME ACTIVITY
				Time_Seconds min_home_duration = 86400 - (end_this + ttime_this_to_home);
				Time_Seconds new_start = end_this + ttime_this_to_home;
				// Ignore travelers who return home after the simulation day is over
				if (new_start < END && (float)min_home_duration > 0)
				{
					at_home_activity_itf* new_act = generator->template Create_Home_Activity<at_home_activity_itf*, Time_Seconds, Vehicle_Components::Types::Vehicle_Type_Keys>(end_this, new_start, min_home_duration, act->template Mode<MODE>());
				}
				((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity_Itf*>(act, true);
				//=====================================================================
				// updated the person scheduler to make this activity the current activity for future planning 
				// then remove from the scheduler activity and movement containers
				scheduler->template Update_Current_Activity<Activity_Itf*>(act);
				scheduler->template Remove_Activity_Plan<Activity_Itf*>(act);
			}

			//=========================================================================================================================
			// Else, determine if can go home before next activity and add return home act if needed
			//-------------------------------------------------------------------------------------------------------------------------
			else
			{
				movement_itf* next_movement = next_act->template movement_plan<movement_itf*>();


				// expected travel times
				location_itf* dest = next_act->template Location<location_itf*>();
				Time_Seconds begin_next = next_act->template Start_Time<Time_Seconds>();
				Time_Seconds ttime_this_to_next = network->template Get_TTime<location_itf*, MODE, Time_Seconds, Time_Seconds>(orig, dest, MODE::SOV, end_this);
				Time_Seconds ttime_home_to_next = network->template Get_TTime<location_itf*, MODE, Time_Seconds, Time_Seconds>(home, dest, MODE::SOV, end_this + ttime_this_to_home);
				Time_Seconds min_home_duration = min((float)ttime_this_to_home, (float)ttime_home_to_next);
				min_home_duration = max((float)min_home_duration, 900.0f);

				//=====================================================================
				// Person can go home first, schedule an additional return home movement
				if (begin_next - end_this > ttime_this_to_home + ttime_home_to_next + min_home_duration)
				{
					// GENERATE A NEW AT HOME ACTIVITY
					Time_Seconds duration = (begin_next - ttime_home_to_next) - (end_this + ttime_this_to_home);
					at_home_activity_itf* new_act = generator->template Create_Home_Activity<at_home_activity_itf*, Time_Seconds, Vehicle_Components::Types::Vehicle_Type_Keys>(end_this, end_this + ttime_this_to_home, duration, act->template Mode<MODE>());
				}

				//=====================================================================
				// otherwise either wait at current activity or depart and spend extra time at next activity
				else
				{
					// first, make sure following activity departs from current activity
					if (next_movement->template origin<location_itf*>() != destination)
					{
						next_act->template Update_Movement_Plan<location_itf*>(destination, next_movement->template destination<location_itf*>(), end_this);
					}
					act->template End_Time<Time_Seconds>(next_movement->template departed_time<Time_Seconds>(), false);
				}

				// Finally, log the activity
				((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity_Itf*>(act, true);


				//=====================================================================
				// updated the person scheduler to make this activity the current activity for future planning 
				// then remove from the scheduler activity and movement containers
				scheduler->template Update_Current_Activity<Activity_Itf*>(act);
				scheduler->template Remove_Activity_Plan<Activity_Itf*>(act);
			}


			//====================================================================
			// Give up ownership of the vehicle after arriving at home, so other agents can use
			if (destination == household->Home_Location<location_itf*>() && vehicle != nullptr) person->Leave_Vehicle();
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::Schedule_Artificial_Arrival_Event()
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename Parent_Person_Itf::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef Activity_Components::Prototypes::Activity_Planner< typename movement_itf::get_type_of(destination_activity_reference)> Activity_Itf;

			movement_itf* movements = this->Movement<movement_itf*>();
			Activity_Itf* act = movements->template destination_activity_reference<Activity_Itf*>();

			this->Artificial_Movement_Scheduled<bool>(true);

			// get adjustment to the routed travel time based on mode type...
			Vehicle_Components::Types::Vehicle_Type_Keys mode = movements->mode<Vehicle_Components::Types::Vehicle_Type_Keys>();
			float ttime_adjust = 1.0;
			if (mode == Vehicle_Components::Types::Vehicle_Type_Keys::WALK) ttime_adjust = 12.0f;
			else if (mode == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE) ttime_adjust = 3.5f;
			else if (mode == Vehicle_Components::Types::Vehicle_Type_Keys::BUS) ttime_adjust = 2.0f; //TODO: replace this with LOS lookup



			int arrival_time = iteration() + movements->routed_travel_time<Simulation_Timestep_Increment>() * ttime_adjust;
			//int arrival_time = max((int)act->template Start_Time<Simulation_Timestep_Increment>(), iteration() + 1);

			this->Artificial_Arrival_Time<Simulation_Timestep_Increment>(arrival_time);

			//((ComponentType*)this)->Load_Event<ComponentType>(&Movement_Event_Controller,arrival_time,0);
			//load_event(ComponentType,Movement_Conditional,Artificial_Arrival_Event,arrival_time,0,NULLTYPE);
		}

		//========================================================
		// Movement Functionality
		//--------------------------------------------------------
		//TODO: Omer Multimodal Movement
		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::Do_Multimodal_Movement()
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Household_Components::Prototypes::Household< typename Parent_Person_Itf::get_type_of(Household)> Household_Itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location< typename Parent_Person_Itf::get_type_of(current_location)> location_itf;
			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<get_component_type(links)>  link_itf;
			typedef Activity_Components::Prototypes::Activity_Planner< typename movement_itf::get_type_of(destination_activity_reference)> Activity_Itf;
			typedef Pair_Associative_Container< typename network_itf::get_type_of(zones_container)> zones;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones)>  zone_itf;

			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Household_Itf* household = person->Parent_Person_Itf::template Household<Household_Itf*>();
			Routing_Itf* itf = person ->template router<Routing_Itf*>();
			//network_itf* network = person->template network_reference<network_itf*>();
			movement_itf* movements = this->Movement<movement_itf*>();
			Activity_Itf* act = movements->template destination_activity_reference<Activity_Itf*>();

			this->Is_Moving(true);

			// If no movement involved - i.e. different activity at same location, do automatic arrival
			if (movements->template origin<location_itf*>() == movements->template destination<location_itf*>())
			{
				this->Schedule_Artificial_Arrival_Event<NT>();
			}
			else if (movements->template origin<link_itf*>() == movements->template destination<link_itf*>())
			{
				this->Schedule_Artificial_Arrival_Event<NT>();
			}
			// Schedule the routing if the vehicle is not already in the network, otherwise return false
			else if (movements->template valid_trajectory<bool>() && (person->template simulation_status<Person_Components::Types::Movement_Status_Keys>() == Person_Components::Types::Movement_Status_Keys::UNLOADED || person->template simulation_status<Person_Components::Types::Movement_Status_Keys>() == Person_Components::Types::Movement_Status_Keys::OUT_NETWORK))
			{
				this->schdeule_person_movements_in_multimodal_network<NT>();
			
			}
			// else, if no valid trajectory, unschedule movement
			else
			{
				THROW_WARNING("Warning: invalid movement trajectory specified.  Trip advanced to end-point without simulation.");
				this->Schedule_Artificial_Arrival_Event<NT>();
			}
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::schdeule_person_movements_in_multimodal_network()
		{
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef  Movement_Plan_Components::Prototypes::Multimodal_Trajectory_Unit<typename remove_pointer< typename movement_itf::get_type_of(multimodal_trajectory_container)::value_type>::type>  _Multimodal_Trajectory_Unit_Interface;
			typedef  Random_Access_Sequence< typename movement_itf::get_type_of(multimodal_trajectory_container), _Multimodal_Trajectory_Unit_Interface*> _Multimodal_Trajectory_Container_Interface;

			movement_itf* movement = this->Movement<movement_itf*>();
			_Multimodal_Trajectory_Container_Interface& trajectory = movement->template multimodal_trajectory_container<_Multimodal_Trajectory_Container_Interface&>();

			movement->template advance_multimodal_trajectory<_Multimodal_Trajectory_Unit_Interface*>();

			int position = movement->template current_multimodal_trajectory_position<int>();

			_Multimodal_Trajectory_Unit_Interface* trajectory_unit = (_Multimodal_Trajectory_Unit_Interface*) trajectory[position];
		
			int next_simulation_time = trajectory_unit->estimated_arrival_time<int>();
			int next_sub = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_ARRIVING_SUBITERATION;

			this->template Next_Simulation_Time<Simulation_Timestep_Increment>(next_simulation_time);
			this->template Next_Sub_Iteration<int>(next_sub);

			static_cast<ComponentType*>(this)->template Load_Event<ComponentType>(&move_persons_in_multimodal_network_conditional, next_simulation_time, next_sub);
		}

		template<typename ComponentType>
		void Person_Mover<ComponentType>::move_persons_in_multimodal_network_conditional(ComponentType* _this, Event_Response& response)
		{
			int cur_iter = iteration();
			int cur_sub = sub_iteration();

			typedef Person_Mover<ComponentType> _Person_Mover_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Mover_Interface* pthis = (_Person_Mover_Interface*)_pthis;

			typedef Prototypes::Person< typename get_type_of(Parent_Person)> person_itf;
			typedef Household_Components::Prototypes::Household< typename person_itf::get_type_of(Household)> household_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename person_itf::get_type_of(scenario_reference)> scenario_itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename person_itf::get_type_of(vehicle)> vehicle_itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef Activity_Components::Prototypes::Activity_Planner< typename movement_itf::get_type_of(destination_activity_reference)> activity_itf;
		
			if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_ARRIVING_SUBITERATION)
			{
				pthis->template person_action_at_beginning_of_link<NT>();
				response.next._iteration = pthis->template Next_Simulation_Time<Simulation_Timestep_Increment>();
				response.next._sub_iteration = pthis->template Next_Sub_Iteration<int>();
			}
			else if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_WAITING_SUBITERATION)
			{
				pthis->template person_waiting_at_beginning_of_link<NT>();
				response.next._iteration = pthis->template Next_Simulation_Time<Simulation_Timestep_Increment>();
				response.next._sub_iteration = pthis->template Next_Sub_Iteration<int>();
			}
			else if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_REROUTING_SUBITERATION)
			{
				//TODO: Omer
				stringstream trajectory_stream;
				trajectory_stream << "help this poor soul" << endl;
				if ((int)num_sim_threads() == 1)
				{
					fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
				}
				response.next._iteration = END;
				response.next._sub_iteration = END;
				//response.next._iteration = pthis->template Next_Simulation_Time<Simulation_Timestep_Increment>();
				//response.next._sub_iteration = pthis->template Next_Sub_Iteration<int>();
			}
			else if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_MOVING_SUBITERATION)
			{
				pthis->template teleport_multimodal_person_to_next_link<NT>();
				response.next._iteration = pthis->template Next_Simulation_Time<Simulation_Timestep_Increment>();
				response.next._sub_iteration = pthis->template Next_Sub_Iteration<int>();
			}
			else
			{
				assert(false);
				cout << "Should never reach here in transit multimodal person mover conditional!" << endl;
			}
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::person_action_at_beginning_of_link()
		{
			//typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf; 
			typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename MasterType::movement_plan_type> movement_itf;
			typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename MasterType::integrated_movement_plan_type> integrated_movement_itf;
			typedef  Movement_Plan_Components::Prototypes::Multimodal_Trajectory_Unit<typename remove_pointer< typename movement_itf::get_type_of(multimodal_trajectory_container)::value_type>::type>  _Multimodal_Trajectory_Unit_Interface;
			typedef  Random_Access_Sequence< typename movement_itf::get_type_of(multimodal_trajectory_container), _Multimodal_Trajectory_Unit_Interface*> _Multimodal_Trajectory_Container_Interface;
			typedef Link_Components::Prototypes::Link< typename _Multimodal_Trajectory_Unit_Interface::get_type_of(link)> _Link_Interface;
			typedef Intersection_Components::Prototypes::Intersection<typename _Link_Interface::get_type_of(upstream_intersection)>  _Intersection_Interface;
			typedef Prototypes::Person< typename get_type_of(Parent_Person)> person_itf;
			typedef typename _Multimodal_Trajectory_Unit_Interface::get_type_of(transit_vehicle_trip) _Transit_Vehicle_Trip_Interface;
			typedef Transit_Pattern_Components::Prototypes::Transit_Pattern<typename MasterType::transit_pattern_type> _Transit_Pattern_Interface;
			typedef  typename _Transit_Pattern_Interface::get_type_of(pattern_links) _Pattern_Links_Container_Interface;
			typedef Scenario_Components::Prototypes::Scenario< typename person_itf::get_type_of(scenario_reference)> scenario_itf;
			typedef Demand_Components::Prototypes::Demand<typename MasterType::demand_type> _Demand_Interface;
			stringstream trajectory_stream;

			int cur_iter = iteration();
			int cur_sub = sub_iteration();

			//Get the person
			person_itf* person = this->Parent_Person<person_itf*>();
			//Get their status
			Person_Components::Types::Movement_Status_Keys current_status = person->template simulation_status<Person_Components::Types::Movement_Status_Keys>();			
			//Get the movement plan
			movement_itf* movement = this->Movement<movement_itf*>();
			//Get the planned trajectory
			_Multimodal_Trajectory_Container_Interface& trajectory = movement->template multimodal_trajectory_container<_Multimodal_Trajectory_Container_Interface&>();
			//Get the person's position along the trajectory
			int current_position = movement->template current_multimodal_trajectory_position<int>();
			//Get the trajectory members at that position
			_Multimodal_Trajectory_Unit_Interface* trajectory_unit = (_Multimodal_Trajectory_Unit_Interface*)trajectory[current_position];
			//Get the link at that position
			_Link_Interface* link = trajectory_unit->template link<_Link_Interface*>();
			//Get the destination link
			_Link_Interface* destination_link = movement->template destination<_Link_Interface*>();
			//Get the link mode from two different sources
			Link_Components::Types::Link_Type_Keys link_type = link->template link_type<Link_Components::Types::Link_Type_Keys>();
			Link_Components::Types::Link_Type_Keys link_mode = trajectory_unit->template link_mode<Link_Components::Types::Link_Type_Keys>();
			//Sanity check
			if (link_type != link_mode)
			{
				assert(false);
				cout << "Link and trajectory data are inconsistent in terms of mode!" << endl;
			}

			trajectory_stream << "I am person:\t" << person->template uuid<int>() << "\t";
			trajectory_stream << "At position:\t" << current_position << "\t";
			trajectory_stream << "Time is:\t" << trajectory_unit->estimated_arrival_time<int>() << "\t";
			trajectory_stream << "Simulation time is:\t" << cur_iter << "\t";
			trajectory_stream << "Subiteration is:\t" << cur_sub << "\t";
			trajectory_stream << "Mode is:\t" << link_type << "\t";
			trajectory_stream << "Node is:\t" << link->template upstream_intersection<_Intersection_Interface*>()->template dbid<std::string>() << "\t";
			trajectory_stream << "I just arrived here ";

			trajectory_unit->template actual_arrival_time<float>(cur_iter);

			//To set the actual experienced values
			if (current_position > 0)
			{
				//Get the previous trajectory position
				int previous_position = current_position - 1;
				//Get the previous trajectory unit
				_Multimodal_Trajectory_Unit_Interface* previous_trajectory_unit = (_Multimodal_Trajectory_Unit_Interface*)trajectory[previous_position];
				//Get the previous link mode
				Link_Components::Types::Link_Type_Keys previous_link_mode = previous_trajectory_unit->template link_mode<Link_Components::Types::Link_Type_Keys>();
				
				//Calculate the actual travel time based on arrival times
				float travel_time = trajectory_unit->template actual_arrival_time<float>() - previous_trajectory_unit->template actual_arrival_time<float>();
				//Set the actual travel time
				previous_trajectory_unit->template actual_travel_time<float>(travel_time);
				
				//Get the waiting time
				float waiting_time = previous_trajectory_unit->template actual_wait_time<float>();

				//Get the previous wait count
				int previous_wait_count = previous_trajectory_unit->template actual_wait_count<int>();
				//Set the current wait count
				trajectory_unit->template actual_wait_count<int>(previous_wait_count);

				//Get the previous transfer penalty
				int previous_transfer_penalty = previous_trajectory_unit->template actual_transfer_penalty<float>();
				//Set the current transfer penalty
				trajectory_unit->template actual_transfer_penalty<float>(previous_transfer_penalty);

				//Set values based on mode
				if (previous_link_mode == Link_Components::Types::Link_Type_Keys::TRANSIT)
				{
					//ivt_time is travel_time minus waiting_time
					float ivt_time = travel_time - waiting_time;
					//Set the ivt_time
					previous_trajectory_unit->template actual_ivt_time<float>(ivt_time);					
					//Get the ivt weight
					float ivtWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::ivtWeight<float>();
					//Augment the generalized cost
					float gen_cost = previous_trajectory_unit->template actual_gen_cost<float>() + ivtWeight * ivt_time;
					//Set the generalized cost
					previous_trajectory_unit->template actual_gen_cost<float>(gen_cost);
				}
				else if (previous_link_mode == Link_Components::Types::Link_Type_Keys::WALK && movement->template mode<Vehicle_Components::Types::Vehicle_Type_Keys>() != Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE)
				{
					//Walk time is travel time
					previous_trajectory_unit->template actual_walk_time<float>(travel_time);
					//Get the ivt weight
					float walkWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::walkWeight<float>();					
					//Augment the generalized cost
					float gen_cost = previous_trajectory_unit->template actual_gen_cost<float>() + walkWeight * travel_time;
					//Set the generalized cost
					previous_trajectory_unit->template actual_gen_cost<float>(gen_cost);
				}
				else if (previous_link_mode == Link_Components::Types::Link_Type_Keys::WALK && movement->template mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE)
				{
					//Bike time is travel time
					previous_trajectory_unit->template actual_bike_time<float>(travel_time);
					//Get the ivt weight
					float bikeWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::bikeWeight<float>();
					//Augment the generalized cost
					float gen_cost = previous_trajectory_unit->template actual_gen_cost<float>() + bikeWeight * travel_time;
					//Set the generalized cost
					previous_trajectory_unit->template actual_gen_cost<float>(gen_cost);
				}
				else
				{
					//Car time is travel time
					previous_trajectory_unit->template actual_car_time<float>(travel_time);
					//Get the car weight
					float carWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::carWeight<float>();
					//Augment the generalized cost
					float gen_cost = previous_trajectory_unit->template actual_gen_cost<float>() + carWeight * travel_time;
					//Set the generalized cost
					previous_trajectory_unit->template actual_gen_cost<float>(gen_cost);
				}
			}

			int next_simulation_time;
			int next_sub;
			Person_Components::Types::Movement_Status_Keys next_status;
			//Check if the person has arrived at their destination
			if (link == destination_link)
			{
				next_simulation_time = END;
				//Next subiteration is moving
				next_sub = END;
				//Traveler status is walking
				next_status = Person_Components::Types::Movement_Status_Keys::OUT_NETWORK;

				//Set the values
				this->template Next_Simulation_Time<Simulation_Timestep_Increment>(next_simulation_time);
				this->template Next_Sub_Iteration<int>(next_sub);

				//If the person is on board they it means they are alighting
				if (current_status == ON_BOARD_SEATED || current_status == ON_BOARD_STANDING)
				{							
					trajectory_stream << ", will alight my trip, and this is my destination!" << endl;
					if ((int)num_sim_threads() == 1)
					{
						fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
					}
					this->template person_alighting_transit_vehicle<NT>();							
				}
				else
				{
					trajectory_stream << "and this is my destination!" << endl;
					if ((int)num_sim_threads() == 1)
					{
						fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
					}
				}

				person->template simulation_status<Person_Components::Types::Movement_Status_Keys>(next_status);

				bool write_trajectory = true;
				//Check if the movement is basic or integrated
				if (movement->template is_integrated<bool>())
				{
					((integrated_movement_itf*) movement)->arrive_to_mm_destination(write_trajectory);
				}
				else
				{
					//movement<basic_movement_itf*>()->arrive_to_mm_destination(write_trajectory);
					movement->arrive_to_mm_destination(write_trajectory);
				}
			}
			//If the mode is non-transit person can walk/bike/drive freely
			else if (link_type != Link_Components::Types::Link_Type_Keys::TRANSIT)
			{
				//If the person is on board they it means they are alighting
				if (current_status == ON_BOARD_SEATED || current_status == ON_BOARD_STANDING)
				{
					trajectory_stream << "and will alight my trip to start walking/driving." << endl;
					if ((int)num_sim_threads() == 1)
					{
						fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
					}
					this->template person_alighting_transit_vehicle<NT>();
					//Next simulation time is the current time
					next_simulation_time = iteration();
					//Next subiteration is moving
					next_sub = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_MOVING_SUBITERATION;
					//Traveler status is walking
					next_status = Person_Components::Types::Movement_Status_Keys::WALKING;
				}
				else
				{
					trajectory_stream << "and will continue moving." << endl; 
					if ((int)num_sim_threads() == 1)
					{
						fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
					}
					//Next simulation time is the current time
					next_simulation_time = iteration();
					//Next subiteration is moving
					next_sub = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_MOVING_SUBITERATION;
					//Traveler status is walking
					next_status = Person_Components::Types::Movement_Status_Keys::WALKING;
				}

				//Set the values
				this->template Next_Simulation_Time<Simulation_Timestep_Increment>(next_simulation_time);
				this->template Next_Sub_Iteration<int>(next_sub);
				person->template simulation_status<Person_Components::Types::Movement_Status_Keys>(next_status);
			}
			//Else means the mode is transit.
			else
			{
				//Get the transit vehicle trip
				_Transit_Vehicle_Trip_Interface* transit_vehicle_trip = trajectory_unit->template transit_vehicle_trip<_Transit_Vehicle_Trip_Interface*>();
				//Get the pattern of that trip
				_Transit_Pattern_Interface* trip_pattern = transit_vehicle_trip->template pattern<_Transit_Pattern_Interface*>();
				//Get the links of that pattern
				_Pattern_Links_Container_Interface& pattern_links = trip_pattern->template pattern_links<_Pattern_Links_Container_Interface&>();
				//Get the current position of the transit vehicle
				int transit_vehicle_position = transit_vehicle_trip->template current_position<int>();
						
				//If they are already on board 
				if (current_status == ON_BOARD_SEATED || current_status == ON_BOARD_STANDING)
				{
					//Get the person's current trip
					_Transit_Vehicle_Trip_Interface* current_transit_vehicle = (_Transit_Vehicle_Trip_Interface*)(person->template current_transit_vehicle<_Transit_Vehicle_Trip_Interface*>());

					//If the person will stay onboard they just move freely in the transit vehicle
					if (current_transit_vehicle == transit_vehicle_trip)
					{
						trajectory_stream << "and will move in my vehicle with trip ID:\t" << transit_vehicle_trip->template dbid<std::string>() << endl;
						if ((int)num_sim_threads() == 1)
						{
							fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
						}
						//Next simulation time is the vehicle departure time
						next_simulation_time = transit_vehicle_trip->template departure_seconds<std::vector<int>>()[transit_vehicle_position];
						//Next subiteration is moving
						next_sub = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_MOVING_SUBITERATION;
						//Traveler status will be current_status
						next_status = current_status;
						//Set the values
						this->template Next_Simulation_Time<Simulation_Timestep_Increment>(next_simulation_time);
						this->template Next_Sub_Iteration<int>(next_sub);
						person->template simulation_status<Person_Components::Types::Movement_Status_Keys>(next_status);
					}
					//Otherwise they are alighting and will wait for their next vehicle
					else
					{
						trajectory_stream << "and will alight the following trip:\t" << current_transit_vehicle->template dbid<std::string>() << "\t to wait for another trip:\t" << transit_vehicle_trip->template dbid<std::string>() << endl;
						if ((int)num_sim_threads() == 1)
						{
							fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
						}
						this->template person_alighting_transit_vehicle<NT>();
						this->template person_waiting_at_beginning_of_link<NT>();
					}
				}
				//Else means they have to wait for the current vehicle
				else
				{			
					trajectory_stream << "and will wait for my trip:\t" << transit_vehicle_trip->template dbid<std::string>() << endl;
					if ((int)num_sim_threads() == 1)
					{
						fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
					}
					this->template person_waiting_at_beginning_of_link<NT>();
				}
			}		
			
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::teleport_multimodal_person_to_next_link()
		{
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef  Movement_Plan_Components::Prototypes::Multimodal_Trajectory_Unit<typename remove_pointer< typename movement_itf::get_type_of(multimodal_trajectory_container)::value_type>::type>  _Multimodal_Trajectory_Unit_Interface;
			typedef  Random_Access_Sequence< typename movement_itf::get_type_of(multimodal_trajectory_container), _Multimodal_Trajectory_Unit_Interface*> _Multimodal_Trajectory_Container_Interface;
			typedef Link_Components::Prototypes::Link< typename _Multimodal_Trajectory_Unit_Interface::get_type_of(link)> _Link_Interface;
			typedef Intersection_Components::Prototypes::Intersection<typename _Link_Interface::get_type_of(upstream_intersection)>  _Intersection_Interface;
			typedef Prototypes::Person< typename get_type_of(Parent_Person)> person_itf;
			stringstream trajectory_stream;

			int cur_iter = iteration();
			int cur_sub = sub_iteration();

			//Get the person
			person_itf* person = this->Parent_Person<person_itf*>();
			//Get the movement plan
			movement_itf* movement = this->Movement<movement_itf*>();
			//Get the planned trajectory
			_Multimodal_Trajectory_Container_Interface& trajectory = movement->template multimodal_trajectory_container<_Multimodal_Trajectory_Container_Interface&>();
			//Advance trajectory position by 1 using the function below
			movement->template advance_multimodal_trajectory<_Multimodal_Trajectory_Unit_Interface*>();
			//Retrieve the new position
			int position = movement->template current_multimodal_trajectory_position<int>();
			//Get the trajectory members at that position
			_Multimodal_Trajectory_Unit_Interface* trajectory_unit = (_Multimodal_Trajectory_Unit_Interface*)trajectory[position];
			//Get the link at that position
			_Link_Interface* link = trajectory_unit->template link<_Link_Interface*>();
			//Get the link mode
			Link_Components::Types::Link_Type_Keys link_type = link->template link_type<Link_Components::Types::Link_Type_Keys>();
			//Next simulation time is the arrival time at that new position
			int next_simulation_time = trajectory_unit->estimated_arrival_time<int>();
			//Next subiteration is arriving at the next link
			int next_sub = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_ARRIVING_SUBITERATION;

			trajectory_stream << "I am person:\t" << person->template uuid<int>() << "\t";
			trajectory_stream << "Will move to position:\t" << position << "\t";
			trajectory_stream << "Will arive there at:\t" << trajectory_unit->estimated_arrival_time<int>() << "\t";
			trajectory_stream << "Simulation time is:\t" << cur_iter << "\t";
			trajectory_stream << "Subiteration is:\t" << cur_sub << "\t";
			trajectory_stream << "Next mode is:\t" << link_type << "\t";
			trajectory_stream << "Next node is:\t" << link->template upstream_intersection<_Intersection_Interface*>()->template dbid<std::string>() << endl;

			//Set the values
			this->template Next_Simulation_Time<Simulation_Timestep_Increment>(next_simulation_time);
			this->template Next_Sub_Iteration<int>(next_sub);

			if ((int)num_sim_threads() == 1)
			{
				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}
		}
		
		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::person_waiting_at_beginning_of_link()
		{
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef  Movement_Plan_Components::Prototypes::Multimodal_Trajectory_Unit<typename remove_pointer< typename movement_itf::get_type_of(multimodal_trajectory_container)::value_type>::type>  _Multimodal_Trajectory_Unit_Interface;
			typedef  Random_Access_Sequence< typename movement_itf::get_type_of(multimodal_trajectory_container), _Multimodal_Trajectory_Unit_Interface*> _Multimodal_Trajectory_Container_Interface;
			typedef Link_Components::Prototypes::Link< typename _Multimodal_Trajectory_Unit_Interface::get_type_of(link)> _Link_Interface;
			typedef Intersection_Components::Prototypes::Intersection<typename _Link_Interface::get_type_of(upstream_intersection)>  _Intersection_Interface;
			typedef Prototypes::Person< typename get_type_of(Parent_Person)> person_itf;
			typedef typename _Multimodal_Trajectory_Unit_Interface::get_type_of(transit_vehicle_trip) _Transit_Vehicle_Trip_Interface;
			typedef Transit_Pattern_Components::Prototypes::Transit_Pattern<typename MasterType::transit_pattern_type> _Transit_Pattern_Interface;
			typedef  typename _Transit_Pattern_Interface::get_type_of(pattern_links) _Pattern_Links_Container_Interface;
			typedef Scenario_Components::Prototypes::Scenario< typename person_itf::get_type_of(scenario_reference)> scenario_itf;
			typedef typename std::list<person_itf*>::iterator queue_iterator_type;
			stringstream trajectory_stream;

			int cur_iter = iteration();
			int cur_sub = sub_iteration();

			//Get the person
			person_itf* person = this->Parent_Person<person_itf*>();
			//Get the movement plan
			movement_itf* movement = this->Movement<movement_itf*>();
			//Get the planned trajectory
			_Multimodal_Trajectory_Container_Interface& trajectory = movement->template multimodal_trajectory_container<_Multimodal_Trajectory_Container_Interface&>();
			//Get the person's position along the trajectory
			int current_position = movement->template current_multimodal_trajectory_position<int>();
			//Get the trajectory members at that position
			_Multimodal_Trajectory_Unit_Interface* trajectory_unit = (_Multimodal_Trajectory_Unit_Interface*)trajectory[current_position];
			//Get the link at that position
			_Link_Interface* link = trajectory_unit->template link<_Link_Interface*>();
			//Get the link mode
			Link_Components::Types::Link_Type_Keys link_type = link->template link_type<Link_Components::Types::Link_Type_Keys>();
			//Get the transit vehicle trip
			_Transit_Vehicle_Trip_Interface* transit_vehicle_trip = trajectory_unit->template transit_vehicle_trip<_Transit_Vehicle_Trip_Interface*>();
			//Get the pattern of that trip
			_Transit_Pattern_Interface* trip_pattern = transit_vehicle_trip->template pattern<_Transit_Pattern_Interface*>();
			//Get the links of that pattern
			_Pattern_Links_Container_Interface& pattern_links = trip_pattern->template pattern_links<_Pattern_Links_Container_Interface&>();
			//Get the current position of the transit vehicle
			int transit_vehicle_position = transit_vehicle_trip->template current_position<int>();
			//Get the unique patterns on the traveler's link
			std::vector<_Transit_Pattern_Interface*>& unique_patterns = link->template unique_patterns<std::vector<_Transit_Pattern_Interface*>&>();
					
			int pattern_ctr = 0;
			int index_along_pattern_at_upstream_node = -1;
			int anticipated_vehicle_arrival_time;
			//Get this link's position along the transit pattern
			for (auto pattern_itr = unique_patterns.begin(); pattern_itr != unique_patterns.end(); pattern_itr++)
			{
				if (trip_pattern == *pattern_itr)
				{
					index_along_pattern_at_upstream_node = link->template index_along_pattern_at_upstream_node<std::vector<int>&>()[pattern_ctr];
					break;
				}
				pattern_ctr++;
			}

			//Get the anticipated_vehicle_arrival_time at that link
			if (index_along_pattern_at_upstream_node >= 0)
			{
				anticipated_vehicle_arrival_time = transit_vehicle_trip->template arrival_seconds<std::vector<int>&>()[index_along_pattern_at_upstream_node];
			}
			else
			{
				assert(false);
				cout << "The traveler is supposed to find a valid pattern!" << endl;
			}

			//Get the list of people waiting at the beginning of that link
			std::list<person_itf*>* people_waiting = link->template people_waiting<std::list<person_itf*>*>();
			//push the person into the standing people list
			people_waiting->push_back(person);
			//Get the position in the queue
			queue_iterator_type position_in_link_waiting_queue = --people_waiting->end();
			//set the person's position as a person property
			person->position_in_link_waiting_queue(position_in_link_waiting_queue);

			trajectory_stream << "I am person:\t" << person->template uuid<int>() << "\t";
			trajectory_stream << "At position:\t" << current_position << "\t";
			trajectory_stream << "Time is:\t" << trajectory_unit->estimated_arrival_time<int>() << "\t";
			trajectory_stream << "Simulation time is:\t" << cur_iter << "\t";
			trajectory_stream << "Subiteration is:\t" << cur_sub << "\t";
			trajectory_stream << "Mode is:\t" << link_type << "\t";
			trajectory_stream << "Node is:\t" << link->template upstream_intersection<_Intersection_Interface*>()->template dbid<std::string>() << "\t";
			trajectory_stream << "I am waiting for trip:\t" << transit_vehicle_trip->template dbid<std::string>() << "\t";
			trajectory_stream << "Hoping to board it at:\t" << anticipated_vehicle_arrival_time << endl;

			//Next simulation time is anticipated_vehicle_arrival_time plus ten minutes
			int next_simulation_time = anticipated_vehicle_arrival_time + 600;
			//Next subiteration is rerouting in case the vehicle does not arrive within 10 minutes of schedule
			int next_sub = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_REROUTING_SUBITERATION;
			//Traveler status will be waiting
			Person_Components::Types::Movement_Status_Keys next_status = Person_Components::Types::Movement_Status_Keys::WAITING;

			//Set the values
			this->template Next_Simulation_Time<Simulation_Timestep_Increment>(next_simulation_time);
			this->template Next_Sub_Iteration<int>(next_sub);
			person->template simulation_status<Person_Components::Types::Movement_Status_Keys>(next_status);

			if ((int)num_sim_threads() == 1)
			{
				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}
		}	

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::person_boarded_transit_vehicle(int next_simulation_time)
		{
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef  Movement_Plan_Components::Prototypes::Multimodal_Trajectory_Unit<typename remove_pointer< typename movement_itf::get_type_of(multimodal_trajectory_container)::value_type>::type>  _Multimodal_Trajectory_Unit_Interface;
			typedef  Random_Access_Sequence< typename movement_itf::get_type_of(multimodal_trajectory_container), _Multimodal_Trajectory_Unit_Interface*> _Multimodal_Trajectory_Container_Interface;
			typedef Link_Components::Prototypes::Link< typename _Multimodal_Trajectory_Unit_Interface::get_type_of(link)> _Link_Interface;
			typedef Intersection_Components::Prototypes::Intersection<typename _Link_Interface::get_type_of(upstream_intersection)>  _Intersection_Interface;
			typedef Prototypes::Person< typename get_type_of(Parent_Person)> person_itf;
			typedef typename _Multimodal_Trajectory_Unit_Interface::get_type_of(transit_vehicle_trip) _Transit_Vehicle_Trip_Interface;
			typedef Transit_Pattern_Components::Prototypes::Transit_Pattern<typename MasterType::transit_pattern_type> _Transit_Pattern_Interface;
			typedef  typename _Transit_Pattern_Interface::get_type_of(pattern_links) _Pattern_Links_Container_Interface;
			typedef Scenario_Components::Prototypes::Scenario< typename person_itf::get_type_of(scenario_reference)> scenario_itf;
			stringstream trajectory_stream;

			int cur_iter = iteration();
			int cur_sub = sub_iteration();

			//Get the person
			person_itf* person = this->Parent_Person<person_itf*>();
			//Get their status
			Person_Components::Types::Movement_Status_Keys current_status = person->template simulation_status<Person_Components::Types::Movement_Status_Keys>();
			//Get the movement plan
			movement_itf* movement = this->Movement<movement_itf*>();
			//Get the planned trajectory
			_Multimodal_Trajectory_Container_Interface& trajectory = movement->template multimodal_trajectory_container<_Multimodal_Trajectory_Container_Interface&>();
			//Get the person's position along the trajectory
			int current_position = movement->template current_multimodal_trajectory_position<int>();
			//Get the trajectory members at that position
			_Multimodal_Trajectory_Unit_Interface* trajectory_unit = (_Multimodal_Trajectory_Unit_Interface*)trajectory[current_position];
			// Get the transit vehicle trip
			_Transit_Vehicle_Trip_Interface* transit_vehicle_trip = trajectory_unit->template transit_vehicle_trip<_Transit_Vehicle_Trip_Interface*>();		
			//Get the link at that position
			_Link_Interface* link = trajectory_unit->template link<_Link_Interface*>();
			//Get the link mode
			Link_Components::Types::Link_Type_Keys link_type = link->template link_type<Link_Components::Types::Link_Type_Keys>();

			//Set the person's current vehicle to the boarded vehicle
			person->template current_transit_vehicle<_Transit_Vehicle_Trip_Interface*>(transit_vehicle_trip);
			//int next_simulation_time = iteration();
			int next_sub = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_MOVING_SUBITERATION;

			//Set the values
			this->template Next_Simulation_Time<Simulation_Timestep_Increment>(next_simulation_time);
			this->template Next_Sub_Iteration<int>(next_sub);

			//Calculate the waiting time as departure time minus arrival time
			float wait_time = next_simulation_time - trajectory_unit->template actual_arrival_time<float>();
			//Set the waiting time
			trajectory_unit->template actual_wait_time<float>(wait_time);

			//To set the actual experienced values
			if (current_position > 0)
			{
				//Get the previous trajectory position
				int previous_position = current_position - 1;
				//Get the previous trajectory unit
				_Multimodal_Trajectory_Unit_Interface* previous_trajectory_unit = (_Multimodal_Trajectory_Unit_Interface*)trajectory[previous_position];
				//Get the previous waiting count
				int wait_count = previous_trajectory_unit->template actual_wait_count<int>();
				//Augment the waiting count
				wait_count++;
				//Set the current waiting_count;
				trajectory_unit->template actual_wait_count<int>(wait_count);
				//Get the base transfer penalty
				float transferPenalty = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::transferPenalty<float>();
				//Get the previous transfer penalty
				float previousTransferPen = previous_trajectory_unit->template actual_transfer_penalty<float>();
				//Calculate the transfer penalty
				float effectiveTransferPen = std::max(wait_count - 1, 0) * transferPenalty;
				//Set the actual transfer penalty
				trajectory_unit->template actual_transfer_penalty<float>(previousTransferPen + effectiveTransferPen);
				//Get the waiting weight
				float waitWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::waitWeight<float>();
				//Augment the generalized cost
				float gen_cost = trajectory_unit->template actual_gen_cost<float>() + waitWeight * wait_time + effectiveTransferPen;
				//Set the generalized cost
				trajectory_unit->template actual_gen_cost<float>(gen_cost);
			}

			trajectory_stream << "I am person:\t" << person->template uuid<int>() << "\t";
			trajectory_stream << "At position:\t" << current_position << "\t";
			trajectory_stream << "Time is:\t" << trajectory_unit->estimated_arrival_time<int>() << "\t";
			trajectory_stream << "Simulation time is:\t" << cur_iter << "\t";
			trajectory_stream << "Subiteration is:\t" << cur_sub << "\t";
			trajectory_stream << "Mode is:\t" << link_type << "\t";
			trajectory_stream << "Node is:\t" << link->template upstream_intersection<_Intersection_Interface*>()->template dbid<std::string>() << "\t";
			trajectory_stream << "I have boarded trip:\t" << transit_vehicle_trip->template dbid<std::string>() << endl;

			//Schedule departure event for person
			static_cast<ComponentType*>(this)->template Load_Event<ComponentType>(&move_persons_in_multimodal_network_conditional, next_simulation_time, next_sub);		

			if ((int)num_sim_threads() == 1)
			{
				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Mover<ComponentType>::person_alighting_transit_vehicle()
		{
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef  Movement_Plan_Components::Prototypes::Multimodal_Trajectory_Unit<typename remove_pointer< typename movement_itf::get_type_of(multimodal_trajectory_container)::value_type>::type>  _Multimodal_Trajectory_Unit_Interface;
			typedef  Random_Access_Sequence< typename movement_itf::get_type_of(multimodal_trajectory_container), _Multimodal_Trajectory_Unit_Interface*> _Multimodal_Trajectory_Container_Interface;
			typedef Link_Components::Prototypes::Link< typename _Multimodal_Trajectory_Unit_Interface::get_type_of(link)> _Link_Interface;
			typedef Intersection_Components::Prototypes::Intersection<typename _Link_Interface::get_type_of(upstream_intersection)>  _Intersection_Interface;
			typedef Prototypes::Person< typename get_type_of(Parent_Person)> person_itf;
			typedef typename _Multimodal_Trajectory_Unit_Interface::get_type_of(transit_vehicle_trip) _Transit_Vehicle_Trip_Interface;
			typedef Transit_Pattern_Components::Prototypes::Transit_Pattern<typename MasterType::transit_pattern_type> _Transit_Pattern_Interface;
			typedef  typename _Transit_Pattern_Interface::get_type_of(pattern_links) _Pattern_Links_Container_Interface;
			typedef Scenario_Components::Prototypes::Scenario< typename person_itf::get_type_of(scenario_reference)> scenario_itf;
			typedef typename std::list<person_itf*>::iterator queue_iterator_type;
			stringstream trajectory_stream;
			
			int cur_iter = iteration();
			int cur_sub = sub_iteration();

			//Get the person
			person_itf* person = this->Parent_Person<person_itf*>();
			//Get their status
			Person_Components::Types::Movement_Status_Keys current_status = person->template simulation_status<Person_Components::Types::Movement_Status_Keys>();
			//Get the movement plan
			movement_itf* movement = this->Movement<movement_itf*>();
			//Get the planned trajectory
			_Multimodal_Trajectory_Container_Interface& trajectory = movement->template multimodal_trajectory_container<_Multimodal_Trajectory_Container_Interface&>();
			//Get the person's position along the trajectory
			int current_position = movement->template current_multimodal_trajectory_position<int>();
			//Get the trajectory members at the previous position
			_Multimodal_Trajectory_Unit_Interface* trajectory_unit = (_Multimodal_Trajectory_Unit_Interface*)trajectory[current_position-1];
			//Get the transit vehicle trip to be alighted
			_Transit_Vehicle_Trip_Interface* transit_vehicle_trip = trajectory_unit->template transit_vehicle_trip<_Transit_Vehicle_Trip_Interface*>();
			//Get the link at that position
			_Link_Interface* link = trajectory_unit->template link<_Link_Interface*>();
			//Get the link mode
			Link_Components::Types::Link_Type_Keys link_type = link->template link_type<Link_Components::Types::Link_Type_Keys>();

			trajectory_stream << "I am person:\t" << person->template uuid<int>() << "\t";
			trajectory_stream << "At position:\t" << current_position << "\t";
			trajectory_stream << "Time is:\t" << trajectory_unit->estimated_arrival_time<int>() << "\t";
			trajectory_stream << "Simulation time is:\t" << cur_iter << "\t";
			trajectory_stream << "Subiteration is:\t" << cur_sub << "\t";
			trajectory_stream << "Mode is:\t" << link_type << "\t";
			trajectory_stream << "Node is:\t" << link->template downstream_intersection<_Intersection_Interface*>()->template dbid<std::string>() << "\t";
			trajectory_stream << "I alighted from trip\t" << transit_vehicle_trip->template dbid<std::string>();

			if (current_status == ON_BOARD_SEATED)
			{//Get the standing people list in the transit vehicle
				std::list<person_itf*>* people_seated = transit_vehicle_trip->template people_seated<std::list<person_itf*>*>();
				//Get the person's position in the standin queue
				queue_iterator_type position_in_vehicle_seated_queue = person->template position_in_vehicle_seated_queue<queue_iterator_type>();
				//Erase the person from the queue
				people_seated->erase(position_in_vehicle_seated_queue);
				//Set the waiting standing position of the person to null
				person->position_in_vehicle_seated_queue(people_seated->end());
				//Check the size
				int queue_size = people_seated->size();
				trajectory_stream << "\t and I was seated" << endl;
			}
			else if (current_status == ON_BOARD_STANDING)
			{//Get the standing people list in the transit vehicle
				std::list<person_itf*>* people_standing = transit_vehicle_trip->template people_standing<std::list<person_itf*>*>();
				//Get the person's position in the standin queue
				queue_iterator_type position_in_vehicle_standing_queue = person->template position_in_vehicle_standing_queue<queue_iterator_type>();
				//Erase the person from the queue
				people_standing->erase(position_in_vehicle_standing_queue);
				//Set the waiting standing position of the person to null
				person->position_in_vehicle_standing_queue(people_standing->end());
				//Check the size
				int queue_size = people_standing->size();
				trajectory_stream << "\t and I was standing" << endl;
			}

			if ((int)num_sim_threads() == 1)
			{
				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}
		}
		//TODO: Omer END
	}
}
