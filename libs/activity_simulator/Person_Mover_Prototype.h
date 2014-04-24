#pragma once

#include "Activity_Simulator_Includes.h"
//#include "traffic_simulator\Network_Event_Prototype.h"



namespace Person_Components
{

namespace Concepts
{
	concept struct Is_Person_Mover_Prototype
	{
		check_template_method_name(Has_Parent_Person,ComponentType::Parent_Person);
		check_template_method_name(Has_Movement, ComponentType::Movement);
		define_default_check(Has_Parent_Person && Has_Movement);
	};
	concept struct Is_Person_Mover
	{
		check_template_method_name(Has_Parent_Person,Parent_Person);
		check_template_method_name(Has_Movement, Movement);
		check_concept(is_prototype, Is_Person_Mover_Prototype, T, V);
		define_default_check(is_prototype || (Has_Parent_Person && Has_Movement) );
	};
}

namespace Types
{

}

namespace Prototypes
{
	prototype struct Person_Mover ADD_DEBUG_INFO
	{
		tag_as_prototype;

		//========================================================
		// Events
		//--------------------------------------------------------
		static void Movement_Event_Controller(ComponentType* _this,Event_Response& response)
		{

			int cur_iter = iteration();
			int cur_sub = sub_iteration();


			typedef Person_Mover<ComponentType> _Person_Mover_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Mover_Interface* pthis =(_Person_Mover_Interface*)_pthis;

			typedef Prototypes::Person< typename get_type_of(Parent_Person)> person_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename person_itf::get_type_of(scenario_reference)> scenario_itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename person_itf::get_type_of(vehicle)> vehicle_itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;

			movement_itf* movement = pthis->template Movement<movement_itf*>();
			person_itf* person = pthis->template Parent_Person<person_itf*>();
			vehicle_itf* vehicle = person->template vehicle<vehicle_itf*>();
			scenario_itf* scenario = (scenario_itf*)_global_scenario;

				
			// determine router aggregation properties - i.e. is routed called at departure or an aggregated time prior to departure
			Simulation_Timestep_Increment routing_timestep = movement->template departed_time<Simulation_Timestep_Increment>()-1;
			if (scenario->template aggregate_routing<bool>())
			{
				int minutes = (int)(movement->template departed_time<Time_Minutes>());
				Simulation_Timestep_Increment temp = Simulation_Time.Convert_Time_To_Simulation_Timestep<Time_Minutes>(minutes);
				routing_timestep = max<int>(temp,iteration());
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
			else if (sub_iteration() == Scenario_Components::Types::PRETRIP_INFORMATION_ACQUISITION)
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
				if (movement->template departed_time<Simulation_Timestep_Increment>() < iteration()+1) //THROW_EXCEPTION("Error: movement departure time is prior to current iteration.");
				{
					movement->template departed_time<Simulation_Timestep_Increment>(iteration()+1);
				}
				response.next._iteration = movement->template departed_time<Simulation_Timestep_Increment>();
				response.next._sub_iteration = Scenario_Components::Types::END_OF_ITERATION;

				pthis->Do_Pretrip_Routing<NT>();
			}
			// GO TO DEPARTURE TIMESTEP
			else if (sub_iteration() == Scenario_Components::Types::END_OF_ITERATION)
			{
				response.next._iteration = END;
				response.next._sub_iteration = END;

				pthis->Do_Movement<NT>();

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
		template<typename TimeType, typename MovementItfType> void Schedule_Movement(TimeType departure_time, MovementItfType movement, requires(MovementItfType,
			check(TimeType, Basic_Units::Concepts::Is_Time_Value) && 
			check(strip_modifiers(MovementItfType), Movement_Plan_Components::Concepts::Is_Movement_Plan) && 
			(check(MovementItfType, is_pointer) || check(MovementItfType, is_reference))))
		{
			this->Movement<MovementItfType>(movement);
			this->Movement_Scheduled<bool>(true);

			// if departure_time is greater than current iteration, load pre-trip stuff on current iteration, otherwise skip pretrip and schedule departure
			int iter = Simulation_Time.template Convert_Time_To_Simulation_Timestep<TimeType>(iteration()+1);
			if (departure_time > iteration() + 2) 
			{	
				((ComponentType*)this)->Load_Event<ComponentType>(&Movement_Event_Controller,iter,Scenario_Components::Types::PRETRIP_INFORMATION_ACQUISITION);
				//load_event(ComponentType,Movement_Conditional,Pretrip_Information_Acquisition_Event,iter,Scenario_Components::Types::PRETRIP_INFORMATION_ACQUISITION,NULLTYPE);
			}
			else 
			{
				((ComponentType*)this)->Load_Event<ComponentType>(&Movement_Event_Controller,iter,Scenario_Components::Types::PRETRIP_ROUTING_SUB_ITERATION);
				//load_event(ComponentType,Movement_Conditional,Pretrip_Routing_Event,iter,Scenario_Components::Types::END_OF_ITERATION,NULLTYPE);	
			}
		}
		template<typename TimeType, typename MovementItfType> void Schedule_Movement(TimeType departure_time, MovementItfType movement, requires(MovementItfType,
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
		template<typename T> void Artificial_Arrival_Event()
		{
			typedef Person_Mover<ComponentType> _Person_Interface;
			_Person_Interface* pthis =(_Person_Interface*)this;

			pthis->template Artificial_Movement_Scheduled<bool>(false);
			
			typedef  Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef  Vehicle_Components::Prototypes::Vehicle< typename Parent_Person_Itf::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename Vehicle_Itf::get_type_of(movement_plan)> movement_itf;
			typedef Activity_Components::Prototypes::Activity_Planner< typename movement_itf::get_type_of(destination_activity_reference)> Activity_Itf;

			Parent_Person_Itf* person = pthis->Parent_Person<Parent_Person_Itf*>();
			movement_itf* movements = pthis->Movement<movement_itf*>();
			Activity_Itf* act = movements->template destination_activity_reference<Activity_Itf*>();	

			act->template Arrive_At_Activity<NT>();
		}


		//========================================================
		// Information Acquisition Functionality
		//--------------------------------------------------------
		template<typename TargetType> void Do_Pretrip_Information_Acquisition()
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Person_Components::Prototypes::Person< typename Parent_Person_Itf::get_type_of(Household)> Household_Itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename Vehicle_Itf::get_type_of(movement_plan)> movement_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;
			
			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<typename get_component_type(links)>  link_itf;
			
			typedef Random_Access_Sequence< typename link_itf::get_type_of(activity_locations)> locations;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(locations)>  location_itf;
			
			typedef Pair_Associative_Container< typename network_itf::get_type_of(zones_container)> zones;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(zones)>  zone_itf;
			

			
			// HAR interface
			typedef  Advisory_ITS_Components::Prototypes::Advisory_ITS< typename link_itf::get_type_of(advisory_radio)> advisory_radio_itf;


			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Routing_Itf* itf= person ->template router<Routing_Itf*>();	
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			network_itf* network = person->template network_reference<network_itf*>();
			movement_itf* movements = this->Movement<movement_itf*>();
			location_itf* destination = movements->destination<location_itf*>();

			//TODO:remove
			/*if (destination == nullptr) cout << "Person id: " << person->Household<Household_Itf*>()->uuid<int>()<<"."<<person->uuid<int>() << ", pretrip destination check: null"<<endl;
			else cout << "Person id: " << person->Household<Household_Itf*>()->uuid<int>()<<"."<<person->uuid<int>() << ", pretrip destination check: passed"<<endl;
*/
			link_itf* origin_link = movements->template origin<link_itf*>();
			advisory_radio_itf* har = origin_link->template advisory_radio<advisory_radio_itf*>();
			

			typedef Network_Event<typename Component_Type::base_network_event_type> event_itf;
			typedef Network_Event<typename Component_Type::weather_network_event_type> weather_itf;
			typedef Network_Event<typename Component_Type::accident_network_event_type> accident_itf;

			if (har != nullptr)
			{	
				boost::container::vector<event_itf*> base_events;
				har->template Get_Displayed_Messages<typename Component_Type::base_network_event_type>(base_events);

				// process weather events from HAR
				boost::container::vector<weather_itf*> weather_events;
				har->template Get_Displayed_Messages<typename Component_Type::weather_network_event_type>(weather_events);
				for (typename boost::container::vector<weather_itf*>::iterator w_itr = weather_events.begin(); w_itr != weather_events.end(); ++w_itr)
				{
					this->Evaluate_Network_Event<weather_itf*>(*w_itr);
				}
			
				// process accident events from HAR
				boost::container::vector<accident_itf*> accident_events;
				har->template Get_Displayed_Messages<typename Component_Type::accident_network_event_type>(accident_events);
				for (typename boost::container::vector<accident_itf*>::iterator a_itr = accident_events.begin(); a_itr != accident_events.end(); ++a_itr)
				{
					this->Evaluate_Network_Event<accident_itf*>(*a_itr);
				}
			}
			
		}

		template<typename TargetType> void Evaluate_Network_Event(TargetType event, requires(TargetType,check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Weather_Event_Prototype)))
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Person_Components::Prototypes::Person< typename Parent_Person_Itf::get_type_of(Household)> Household_Itf;
			typedef Person_Components::Prototypes::Person_Planner< typename Parent_Person_Itf::get_type_of(Planning_Faculty)> planning_itf;
			typedef Person_Components::Prototypes::Person_Scheduler< typename Parent_Person_Itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Person_Components::Prototypes::Destination_Chooser< typename planning_itf::get_type_of(Destination_Choice_Faculty)> destination_chooser_itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename Vehicle_Itf::get_type_of(movement_plan)> movement_itf;
			//typedef Activity_Components::Prototypes::Activity_Planner< typename movement_itf::get_type_of(activity_reference)> activity_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;
			
			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<typename get_component_type(links)>  link_itf;
			
			typedef Random_Access_Sequence< typename link_itf::get_type_of(activity_locations)> locations;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(locations)>  location_itf;
			
			typedef Pair_Associative_Container< typename network_itf::get_type_of(zones_container)> zones;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(zones)>  zone_itf;

			typedef Random_Access_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> activity_container_itf;
			typedef Activity_Components::Prototypes::Activity_Planner<typename get_component_type(activity_container_itf)>  activity_itf;
			
			
			// person, router, etc. interfaces
			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			planning_itf* planner = person->template Planning_Faculty<planning_itf*>();
			destination_chooser_itf* dest_chooser = planner->template Destination_Choice_Faculty<destination_chooser_itf*>();
			Routing_Itf* itf= person ->template router<Routing_Itf*>();	
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			network_itf* network = person->template network_reference<network_itf*>();
			movement_itf* movement = this->Movement<movement_itf*>();
			activity_itf* activity = movement->template destination_activity_reference<activity_itf*>();
			Activity_Components::Types::ACTIVITY_TYPES act_type = activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();

			// event interface
			typedef Network_Event_Components::Prototypes::Network_Event<typename Component_Type::weather_network_event_type> weather_itf;
			weather_itf* weather_event = (weather_itf*)event;
	
			// determine if destination activity is flexible
			Activity_Components::Types::FLEXIBILITY_VALUES flexibility = activity->template Location_Flexibility<Activity_Components::Types::FLEXIBILITY_VALUES>();


			//TODO:remove
			//zone_itf* destination = movement->template destination<zone_itf*>();
			//if (destination == nullptr) cout << "Person id: " << person->Household<Household_Itf*>()->uuid<int>()<<"."<<person->uuid<int>() << ", evaluate event destination check: null"<<endl;
			//else cout << "Person id: " << person->Household<Household_Itf*>()->uuid<int>()<<"."<<person->uuid<int>() << ", evaluate event destination check: passed"<<endl;


			// If event affects traveler
			if (this->Is_Event_Relevant<weather_itf*>(weather_event)/* && flexibility != Activity_Components::Types::FLEXIBILITY_VALUES::LOW_FLEXIBILITY*/)
			{
				this->Replanning_Needed<bool>(true);
				person->template has_done_replanning<bool>(true);

				// get all zones affected by weather
				boost::container::vector<location_itf*> *unaffected_locations = weather_event->template unaffected_locations<boost::container::vector<location_itf*>*>();

				// replan, select new zone ouside of affected area
				location_itf* orig, * dest, *next;
				zone_itf* zone_dest = movement->template destination<zone_itf*>();
				int old_dest_id = zone_dest == nullptr ? -1 : movement->template destination<zone_itf*>()->template uuid<int>();

				if (old_dest_id == -1)
				{
					//TODO: remove
					cout <<endl<<"Why is this destination null? personid="<<person->uuid<int>() <<": type="<<activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
					//if (activity->Location<location_itf*>() != nullptr) cout <<", zone="<<activity->Location<location_itf*>()->zone<zone_itf*>()->uuid<int>();
				}

				activity_itf* prev_act = person->template previous_activity_plan<Simulation_Timestep_Increment,activity_itf*>(iteration());
				if (prev_act == nullptr) orig = person->template Home_Location<location_itf*>();
				else orig = prev_act->template Location<location_itf*>();

				activity_itf* next_act = person->template next_activity_plan<Simulation_Timestep_Increment,activity_itf*>(iteration());
				if (next_act == nullptr) next = person->template Home_Location<location_itf*>();
				else next = next_act->template Location<location_itf*>();

				dest = dest_chooser->template Choose_Destination<activity_itf*,location_itf*>(activity, unaffected_locations);

				if (dest != nullptr)
				{
					movement->template destination<location_itf*>(dest);
					movement->template destination<link_itf*>((link_itf*)(dest->template origin_links<links*>()->at(0)));
					activity_itf* act = movement->template destination_activity_reference<activity_itf*>();
					act->template Location<location_itf*>(dest);
					
					// cout << endl << "Destination Replanned for person '"<<person->Household<Household_Itf*>()->uuid<int>() <<"."<<person->uuid<int>()  <<"' due to weather, switch from zone '" << old_dest_id << "', to new zone '" << dest->zone<zone_itf*>()->uuid<int>()<<"'.";
					
					Time_Seconds ttime = network->template Get_TTime<location_itf*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Seconds,Time_Seconds>(orig, dest, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, iteration());

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
		template<typename TargetType> void Evaluate_Network_Event(TargetType event, requires(TargetType,check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Accident_Event_Prototype)))
		{
			typedef Network_Event_Components::Prototypes::Network_Event<typename Component_Type::accident_network_event_type> accident_itf;
			accident_itf* accident_event = (accident_itf*)event;
			
			//cout << endl << "EVALUATING ACCIDENT EVENT: ";

			if (this->Is_Event_Relevant<accident_itf*>(accident_event))
			{
				cout << endl << "ACCIDENT_ADVISORY: there is an accident along your route.";
			}
		}
		template<typename TargetType> void Evaluate_Network_Event(TargetType event, requires(TargetType,!check(strip_modifiers(TargetType),Network_Event_Components::Concepts::Is_Weather_Event_Prototype) && !check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Accident_Event_Prototype)))
		{
			assert_check(strip_modifiers(TargetType), Is_Prototype, "Warning: TargetType event must be a polaris prototype.");
			assert_check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Weather_Event_Prototype, "Warning: TargetType component must be a weather_network_event, or ");
			assert_check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Accident_Event_Prototype, "TargetType component must be an accident_network_event_type.");
		}

		template<typename TargetType> bool Is_Event_Relevant(TargetType event, requires(TargetType,check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Weather_Event_Prototype)))
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Person_Components::Prototypes::Person_Scheduler< typename Parent_Person_Itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;
			
			typedef Random_Access_Sequence<typename movement_itf::get_type_of(trajectory_container)> trajectory_interface;
			typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<typename get_component_type(trajectory_interface)> trajectory_unit_interface;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<typename get_component_type(links)>  link_itf;
			
			typedef Random_Access_Sequence< typename link_itf::get_type_of(activity_locations)> locations;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(locations)>  location_itf;
			
			typedef Pair_Associative_Container< typename network_itf::get_type_of(zones_container)> zones;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(zones)>  zone_itf;

			typedef Random_Access_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> activity_container_itf;
			typedef Activity_Components::Prototypes::Activity_Planner<typename get_component_type(activity_container_itf)>  activity_itf;

			
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
			boost::container::vector<zone_itf*> *affected_zones = my_event->template affected_zones<boost::container::vector<zone_itf*> *>();
			
			// event is not relevent if going to home/work/school as these locations are fixed.

			if (activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY ||
				activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::PRIMARY_WORK_ACTIVITY ||
				activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::SCHOOL_ACTIVITY ||
				activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::WORK_AT_HOME_ACTIVITY)
			{
				return false;
			}

			zone_itf* affected_zone;
			for (typename boost::container::vector<zone_itf*>::iterator itr = affected_zones->begin(); itr != affected_zones->end(); ++itr)
			{
				affected_zone = *itr;
				
				if (affected_zone == destination /*|| destination == nullptr*/)
				{
					//TODO: Remove
					//cout << endl << "Affected ID, destination ID: " << affected_zone->uuid<int>();
					//if (destination != nullptr) cout << ", " << destination->uuid<int>();
					//else
					//{
					//	cout <<", null";
					//	//TODO: remove
					//	cout <<endl<<"Why is this destination null? personid="<<person->uuid<int>() <<": type="<<activity->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
					//	if (activity->Location<location_itf*>() != nullptr) cout <<", zone="<<activity->Location<location_itf*>()->zone<zone_itf*>()->uuid<int>();
					//	else cout <<", zone=null.";
					//}
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
		template<typename TargetType> bool Is_Event_Relevant(TargetType event, requires(TargetType,check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Accident_Event_Prototype)))
		{
			// interfaces
			typedef  Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef  Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef  Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef  Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;
			
			typedef Random_Access_Sequence<typename movement_itf::get_type_of(trajectory_container)> trajectory_interface;
			typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<typename get_component_type(trajectory_interface)> trajectory_unit_interface;
			
			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<typename get_component_type(links)>  link_itf;

			
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

		template<typename TargetType> void Get_Event_Extents(TargetType event, boost::unordered::unordered_set<int>& affected_indices, requires(TargetType,check(strip_modifiers(TargetType), Network_Event_Components::Concepts::Is_Weather_Event_Prototype)))
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(Movement)> movement_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;
			
			typedef Random_Access_Sequence<typename movement_itf::get_type_of(trajectory_container)> trajectory_interface;
			typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<typename get_component_type(trajectory_interface)> trajectory_unit_interface;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<typename get_component_type(links)>  link_itf;
			
			typedef Random_Access_Sequence< typename link_itf::get_type_of(activity_locations)> locations;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(locations)>  location_itf;
			
			typedef Pair_Associative_Container< typename network_itf::get_type_of(zones_container)> zones;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(zones)>  zone_itf;

			
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
		template<typename TargetType> void Do_Pretrip_Replanning()
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename Vehicle_Itf::get_type_of(movement_plan)> movement_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;
			
			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<typename get_component_type(links)>  link_itf;


			typedef  Person_Components::Prototypes::Person_Data_Logger< typename ComponentType::Master_Type::person_data_logger_type> _Logger_Interface;

			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Routing_Itf* itf= person ->template router<Routing_Itf*>();	
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
		template<typename TargetType> void Do_Pretrip_Routing()
		{
			// interfaces
			typedef  Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Prototypes::Person_Planner< typename Parent_Person_Itf::get_type_of(Planning_Faculty)> Planning_Itf;
			typedef  Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename Vehicle_Itf::get_type_of(movement_plan)> movement_itf;
			typedef  Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef  Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;
			typedef  Activity_Location_Components::Prototypes::Activity_Location< typename Parent_Person_Itf::get_type_of(current_location)> location_itf;
			typedef Activity_Components::Prototypes::Activity_Planner< typename movement_itf::get_type_of(destination_activity_reference)> Activity_Itf;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<typename get_component_type(links)>  link_itf;
					
			
			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Planning_Itf* planner = person->template Planning_Faculty<Planning_Itf*>();
			movement_itf* movements = this->Movement<movement_itf*>();
			Activity_Itf* act = movements->template destination_activity_reference<Activity_Itf*>();

			// Skip routing if the mode is not SOV (remove when transit router is developed
			if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() != Vehicle_Components::Types::Vehicle_Type_Keys::SOV) return;

			// Do routing at the next timestep for the movement
			planner->template Schedule_New_Routing<movement_itf*>(iteration()+1,movements);
		}

		//========================================================
		// Movement Functionality
		//--------------------------------------------------------
		template<typename TargetType> void Do_Movement()
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Household_Components::Prototypes::Household< typename Parent_Person_Itf::get_type_of(Household)> Household_Itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename Vehicle_Itf::get_type_of(movement_plan)> movement_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location< typename Parent_Person_Itf::get_type_of(current_location)> location_itf;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links;
			typedef Link_Components::Prototypes::Link<typename get_component_type(links)>  link_itf;

			typedef Activity_Components::Prototypes::Activity_Planner< typename movement_itf::get_type_of(destination_activity_reference)> Activity_Itf;

			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Household_Itf* household = person->template Household<Household_Itf*>();
			Routing_Itf* itf= person ->template router<Routing_Itf*>();	
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			network_itf* network = person->template network_reference<network_itf*>();
			movement_itf* movements = this->Movement<movement_itf*>();
			Activity_Itf* act = movements->template destination_activity_reference<Activity_Itf*>();	


			// If no movement involved - i.e. different activity at same location, do auto arrive
			if (movements->template origin<location_itf*>() == movements->template destination<location_itf*>())
			{
				this->Schedule_Artificial_Arrival_Event<NT>();
			}
			// Schedule the routing if the vehicle is not already in the network, otherwise return false
			else if (movements->template valid_trajectory<bool>() && (vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::UNLOADED || vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::OUT_NETWORK))
			{
				// set the persons location to be the destination
				person->template current_location<location_itf*>(movements->template destination<location_itf*>());

				// if auto trip, push to network, if not skip (for now)
				if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
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
			// for all non-auto modes, jump to activity arrival (will be replaced with simulation at some point
			else if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() != Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
			{
				this->Schedule_Artificial_Arrival_Event<NT>();
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
		template<typename TargetType> void Arrive_At_Destination()
		{
			// free up movement schedule
			this->Movement_Scheduled<bool>(false);

			// interfaces
			typedef Vehicle_Components::Types::Vehicle_Type_Keys MODE;
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Household_Components::Prototypes::Household< typename Parent_Person_Itf::get_type_of(Household)> Household_Itf;
			typedef Person_Components::Prototypes::Person_Scheduler< typename Parent_Person_Itf::get_type_of(Scheduling_Faculty)> Scheduler_Itf;
			typedef Person_Components::Prototypes::Person_Planner< typename Parent_Person_Itf::get_type_of(Planning_Faculty)> Planner_Itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename get_type_of(Parent_Person)::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename Vehicle_Itf::get_type_of(movement_plan)> movement_itf;
			typedef Routing_Components::Prototypes::Routing< typename get_type_of(Parent_Person)::get_type_of(router)> Routing_Itf;
			typedef Network_Components::Prototypes::Network< typename Parent_Person_Itf::get_type_of(network_reference)> network_itf;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename network_itf::get_type_of(skimming_faculty)> skim_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location< typename Parent_Person_Itf::get_type_of(current_location)> location_itf;
			typedef Zone_Components::Prototypes::Zone< typename location_itf::get_type_of(zone)> zone_itf;
			
			typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container)> links_container_itf;
			typedef Link_Components::Prototypes::Link<typename get_component_type(links_container_itf)>  link_itf;
			
			typedef Random_Access_Sequence< typename network_itf::get_type_of(turn_movements_container)> turns_container_itf;
			typedef Turn_Movement_Components::Prototypes::Movement<typename get_component_type(turns_container_itf)>  turn_itf;
			
			typedef Activity_Components::Prototypes::Activity_Planner< typename movement_itf::get_type_of(destination_activity_reference)> Activity_Itf;

			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Household_Itf* household = person->Household<Household_Itf*>();
			Planner_Itf* planner = person->template Planning_Faculty<Planner_Itf*>();
			Routing_Itf* itf= person->template router<Routing_Itf*>();	
			Scheduler_Itf* scheduler = person->template Scheduling_Faculty<Scheduler_Itf*>();
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			network_itf* network = person->template network_reference<network_itf*>();
			skim_itf* skim = network->template skimming_faculty<skim_itf*>();
			movement_itf* movements = this->Movement<movement_itf*>();

			//=====================================================================
			// schedule departure from destination if no following activity
			Activity_Itf* act = movements->template destination_activity_reference<Activity_Itf*>();	
			movements->arrived_time<Simulation_Timestep_Increment>(iteration());
			location_itf* destination = movements->destination<location_itf*>();
			//=====================================================================
			// remove from the scheduler movement container
			scheduler->Remove_Movement_Plan<movement_itf*>(movements);


			//=====================================================================
			// Update activity start time
			// Handle late arrival
			Time_Seconds new_end = act->End_Time<Time_Seconds>();
			if (iteration() > act->Start_Time<Simulation_Timestep_Increment>())
			{
				Activity_Itf* next_act = scheduler->template next_activity_plan<Activity_Itf*,Activity_Itf*>(act);
				movement_itf* next_movement;
				if (next_act != nullptr) next_movement = next_act->template movement_plan<movement_itf*>();

				// no following act, so just shift end
				if (next_act == nullptr)
				{
					new_end = iteration() + act->template Duration<Simulation_Timestep_Increment>();
				}
				// following act does not depart before end of shifted current act, so just shift end
				else if (iteration() + act->template Duration<Simulation_Timestep_Increment>() < next_movement->template departed_time<Simulation_Timestep_Increment>())
				{
					new_end = iteration() + act->template Duration<Simulation_Timestep_Increment>();
				}

				// Very far past original arrival time, shorten activity and check next activity start
				else if (act->End_Time<Time_Seconds>() < iteration() + act->Minimum_Duration<Time_Seconds>())
				{
					// reset end of activity
					new_end = iteration() + act->Minimum_Duration<Time_Seconds>();

					// delete all activities which are completely overlapped by adjusted minimum activity due to late arrival
					bool activity_deleted=false;
					while (true)
					{
						// get next activity 
						next_act = scheduler->template next_activity_plan<Activity_Itf*,Activity_Itf*>(act);
						if (next_act == nullptr) break;

						// get associated movement plan
						next_movement = next_act->template movement_plan<movement_itf*>();
						
						// drop next activity and go to the following if completely overlapped
						if (next_act->End_Time<Time_Seconds>() - next_act->Minimum_Duration<Time_Seconds>() - next_act->Expected_Travel_Time<Time_Seconds>() < new_end)
						{
							scheduler->Remove_Activity_Plan<Activity_Itf*>(next_act);
							scheduler->Remove_Movement_Plan<movement_itf*>(next_movement);
							activity_deleted=true;
							THROW_WARNING("Activity Plan removed due to late arrival at preceding activity: ");
						}
						// no conflict with next remaining activity so break
						else if (next_movement->departed_time<Time_Seconds>() > new_end)
						{
							// update movement plan to account for deleted activities
							if (activity_deleted) next_act->Update_Movement_Plan<location_itf*>(act->Location<location_itf*>(),next_act->Location<location_itf*>(),new_end);
							break;
						}
						// resolvable conflict with next activity, so shift departure then break
						else
						{
							// update movement plan to account for deleted activities
							if (activity_deleted) next_act->Update_Movement_Plan<location_itf*>(act->Location<location_itf*>(),next_act->Location<location_itf*>(),new_end);

							Time_Seconds next_end = next_act->End_Time<Time_Seconds>();
							next_movement->departed_time<Time_Seconds>(new_end);
							next_act->Start_Time<Time_Seconds>(next_movement->departed_time<Time_Seconds>() + next_act->Expected_Travel_Time<Time_Seconds>());
							next_act->End_Time<Time_Seconds>(next_end,false);
							break;
						}

						WHILE_PRINT("person mover - arrive at destination while loop");
					}
				}
				else
				{

				}

			}

			act->Start_Time<Simulation_Timestep_Increment>(iteration());
			act->End_Time<Time_Seconds>(new_end,false);
			
			

			//=====================================================================
			// Get the next scheduled activity if exists
			Activity_Itf* next_act = scheduler->template next_activity_plan<Simulation_Timestep_Increment,Activity_Itf*>(iteration());

			typedef  Person_Components::Prototypes::Person_Data_Logger< typename MasterType::person_data_logger_type> _Logger_Interface;
			


			// Define time window after current activity is completed
			Time_Seconds end_this = act->template Start_Time<Time_Seconds>() + act->template Duration<Time_Seconds>();
			location_itf* orig = act->template Location<location_itf*>();
			location_itf* home = person->template Home_Location<location_itf*>();

			Time_Seconds ttime_this_to_home = network->template Get_TTime<location_itf*,MODE,Time_Seconds,Time_Seconds>(orig,home,MODE::SOV,end_this);


			// don't add additional movement if already at home, just update the end time of the current at home activity
			if (act->template Location<location_itf*>() == person->template Home_Location<location_itf*>() && act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY)
			{
				if (next_act != nullptr) 
				{
					movement_itf* next_movement = next_act->template movement_plan<movement_itf*>();
					act->template End_Time<Time_Seconds>(next_movement->template departed_time<Time_Seconds>(),false);
				}
				else
				{
					act->template End_Time<Time_Seconds>(END,false);
				}
				((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity_Itf*>(act,true);

				return;
			}

			// If this is the last act of the day, then return home afterward
			if (next_act == nullptr)
			{
				// GENERATE A NEW AT HOME ACTIVITY
				
				typedef Activity_Components::Prototypes::Activity_Planner<typename ComponentType::Master_Type::at_home_activity_plan_type> at_home_activity_itf;
				at_home_activity_itf* new_act = (at_home_activity_itf*)Allocate<typename ComponentType::Master_Type::at_home_activity_plan_type>();
				new_act->template Parent_Planner<Planner_Itf*>(planner);
				new_act->Activity_Plan_ID<int>(scheduler->Activity_Count<int>() + 100);
				Time_Seconds min_home_duration = 86400 - (end_this+ttime_this_to_home);
				Time_Seconds new_start = end_this+ttime_this_to_home;
				// Ignore travelers who return home after the simulation day is over
				if (new_start > END || (float)min_home_duration < 0)
				{
					Free<typename ComponentType::Master_Type::at_home_activity_plan_type>((typename ComponentType::Master_Type::at_home_activity_plan_type*)new_act);
				}
				else
				{
					new_act->template Initialize<Time_Seconds,Vehicle_Components::Types::Vehicle_Type_Keys>(end_this,new_start, min_home_duration,act->template Mode<MODE>());
					((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity_Itf*>(act,true);
				}

				return;
			}
			movement_itf* next_movement = next_act->template movement_plan<movement_itf*>();

			
			// expected travel times
			location_itf* dest = next_act->template Location<location_itf*>();
			Time_Seconds begin_next = next_act->template Start_Time<Time_Seconds>();
			Time_Seconds ttime_this_to_next = network->template Get_TTime<location_itf*,MODE,Time_Seconds,Time_Seconds>(orig,dest,MODE::SOV,end_this);
			Time_Seconds ttime_home_to_next = network->template Get_TTime<location_itf*,MODE,Time_Seconds,Time_Seconds>(home,dest,MODE::SOV,end_this + ttime_this_to_home);
			Time_Seconds min_home_duration = min((float)ttime_this_to_home,(float)ttime_home_to_next);
			min_home_duration = max((float)min_home_duration, 900.0f);

			//=====================================================================
			// Person can go home first, schedule an additional return home movement
			if (begin_next - end_this > ttime_this_to_home + ttime_home_to_next + min_home_duration)
			{
				// GENERATE A NEW AT HOME ACTIVITY
				Time_Seconds duration = (begin_next - ttime_home_to_next) - (end_this + ttime_this_to_home);
				typedef Activity_Components::Prototypes::Activity_Planner<typename ComponentType::Master_Type::at_home_activity_plan_type> at_home_activity_itf;
				at_home_activity_itf* new_act = (at_home_activity_itf*)Allocate<typename ComponentType::Master_Type::at_home_activity_plan_type>();
				new_act->template Parent_Planner<Planner_Itf*>(planner);
				new_act->Activity_Plan_ID<int>(scheduler->Activity_Count<int>() + 100);

				//TODO: remove when done testing
				if(end_this+ttime_this_to_home > END || duration > END)
				{
					cout<<endl<<"begin_next="<<begin_next<<", end_this="<<end_this<<", ttime_this_to_home="<<ttime_this_to_home<<", ttime_home_to_next="<<ttime_home_to_next<<", min_home_duration="<<min_home_duration<<", home="<<home<<", dest="<<dest<<", LOS_start="<<end_this + ttime_this_to_home<<endl;
					THROW_EXCEPTION("Invalid start/duration for at home activity. Start="<<end_this+ttime_this_to_home<<", duration="<<duration<<", departure time="<<end_this<<", travel time="<<ttime_this_to_home);
				}

				new_act->template Initialize<Time_Seconds,Vehicle_Components::Types::Vehicle_Type_Keys>(end_this, end_this+ttime_this_to_home, duration,act->template Mode<MODE>());
			}

			//=====================================================================
			// otherwise either wait at current activity or depart and spend extra time at next activity
			else
			{
				// first, make sure following activity departs from current activity
				if (next_movement->origin<location_itf*>() != destination)
				{
					next_act->Update_Movement_Plan<location_itf*>(destination,next_movement->destination<location_itf*>(),end_this);
				}
				//TODO: remove when done testing
				if (next_movement->departed_time<Time_Seconds>() > (END)*2.0)
				{
					THROW_EXCEPTION("Error, next_movement departure time is out of simulation time frame. next move depart time="<<next_movement->departed_time<Time_Seconds>()<<", next act start time="<<next_act->Start_Time<Time_Seconds>());
				}
				act->End_Time<Time_Seconds>(next_movement->departed_time<Time_Seconds>(),false);
			}

			// Finally, log the activity
			((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity_Itf*>(act,true);
			
		}

		template<typename TargetType> void Schedule_Artificial_Arrival_Event()
		{
			// interfaces
			typedef Person_Components::Prototypes::Person< typename get_type_of(Parent_Person)> Parent_Person_Itf;
			typedef Vehicle_Components::Prototypes::Vehicle< typename Parent_Person_Itf::get_type_of(vehicle)> Vehicle_Itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename Vehicle_Itf::get_type_of(movement_plan)> movement_itf;
			typedef Activity_Components::Prototypes::Activity_Planner< typename movement_itf::get_type_of(destination_activity_reference)> Activity_Itf;

			movement_itf* movements = this->Movement<movement_itf*>();
			Activity_Itf* act = movements->template destination_activity_reference<Activity_Itf*>();	

			this->Artificial_Movement_Scheduled<bool>(true);

			int arrival_time = max((int)act->Start_Time<Simulation_Timestep_Increment>(),iteration()+1);

			this->Artificial_Arrival_Time<Simulation_Timestep_Increment>(arrival_time);

			//((ComponentType*)this)->Load_Event<ComponentType>(&Movement_Event_Controller,arrival_time,0);
			//load_event(ComponentType,Movement_Conditional,Artificial_Arrival_Event,arrival_time,0,NULLTYPE);
		}

		//========================================================
		// Basic Forms
		//--------------------------------------------------------
		accessor(Parent_Person, NONE, NONE);
		accessor(Movement, NONE, NONE);
		accessor(Movement_Scheduled, NONE, NONE);
		accessor(Artificial_Movement_Scheduled, NONE, NONE);
		accessor(Artificial_Arrival_Time, NONE, NONE);
		accessor(Replanning_Needed, NONE, NONE);
	};

}

}
