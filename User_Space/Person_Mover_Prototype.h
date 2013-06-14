#pragma once

#include "User_Space_Includes.h"
#include "Network_Event_Prototype.h"



namespace Person_Components
{

namespace Concepts
{
	concept struct Is_Person_Mover_Prototype
	{
		check_getter(Has_Parent_Person,ComponentType::Parent_Person);
		check_getter(Has_Movement, ComponentType::Movement);
		define_default_check(Has_Parent_Person && Has_Movement);
	};
	concept struct Is_Person_Mover
	{
		check_getter(Has_Parent_Person,Parent_Person);
		check_getter(Has_Movement, Movement);
		check_concept(is_prototype, Is_Person_Mover_Prototype);
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
		declare_feature_conditional(Movement_Conditional)
		{
			typedef Person_Mover<ComponentType, ComponentType> _Person_Mover_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Mover_Interface* pthis =(_Person_Mover_Interface*)_pthis;

			define_component_interface(person_itf, typename get_type_of(Parent_Person),Prototypes::Person,ComponentType);
			define_component_interface(scenario_itf, typename person_itf::get_type_of(scenario_reference),Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(vehicle_itf, typename person_itf::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype,ComponentType);
			define_component_interface(movement_itf, typename get_type_of(Movement), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);

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
				routing_timestep = std::max<int>(temp,_iteration);
			}
			if (routing_timestep < _iteration) routing_timestep = _iteration;


			// Do pretrip rerouting if car has realtime info
			bool has_pretrip_info = person->template has_pretrip_information<bool>();


			// DO PRE-TRIP PLANNING, THEN SCHEDULE NEXT ITERATION FOR DEPARTURE TIME
			if (_sub_iteration == Scenario_Components::Types::PRETRIP_INFORMATION_ACQUISITION)
			{
				response.next._iteration = _iteration;
				response.next._sub_iteration = Scenario_Components::Types::PRETRIP_PLANNING_SUB_ITERATION;
				response.result = has_pretrip_info;
			}
			else if (_sub_iteration == Scenario_Components::Types::PRETRIP_PLANNING_SUB_ITERATION)
			{
				_pthis->Swap_Event((Event)&Person_Mover::Pretrip_Replanning_Event<NULLTYPE>);
				response.next._iteration = routing_timestep;
				response.next._sub_iteration = Scenario_Components::Types::PRETRIP_ROUTING_SUB_ITERATION;
				response.result = pthis->template Replanning_Needed<bool>();
			}
			else if (_sub_iteration == Scenario_Components::Types::PRETRIP_ROUTING_SUB_ITERATION)
			{
				_pthis->Swap_Event((Event)&Person_Mover::Pretrip_Routing_Event<NULLTYPE>);
				response.next._iteration = routing_timestep + 1;
				response.next._sub_iteration = Scenario_Components::Types::END_OF_ITERATION;
				response.result = true;
			}

			// GO TO DEPARTURE TIMESTEP
			else if (_sub_iteration == Scenario_Components::Types::END_OF_ITERATION)
			{
				_pthis->Swap_Event((Event)&Person_Mover::Movement_Event<NULLTYPE>);
				response.next._iteration = END;
				response.next._sub_iteration = END;
				response.result = true;
			}

			// ERROR SHOULDN"T REACH HERE
			else
			{
				THROW_EXCEPTION("ERROR: should not reach this point in conditional, improper response.revision set at some point.");
				response.next._iteration = END;
				response.next._sub_iteration = END;
				response.result = false;
			}
		}
		declare_feature_event(Pretrip_Information_Acquisition_Event)
		{
			typedef Person_Mover<ComponentType, ComponentType> _Person_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Interface* pthis =(_Person_Interface*)_pthis;
			pthis->Do_Pretrip_Information_Acquisition<NT>();
		}
		declare_feature_event(Pretrip_Replanning_Event)
		{
			typedef Person_Mover<ComponentType, ComponentType> _Person_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Interface* pthis =(_Person_Interface*)_pthis;
			pthis->Do_Pretrip_Replanning<NT>();
		}
		declare_feature_event(Pretrip_Routing_Event)
		{
			typedef Person_Mover<ComponentType, ComponentType> _Person_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Interface* pthis =(_Person_Interface*)_pthis;
			pthis->Do_Pretrip_Routing<NT>();
		}
		declare_feature_event(Movement_Event)
		{
			typedef Person_Mover<ComponentType, ComponentType> _Person_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Interface* pthis =(_Person_Interface*)_pthis;
			pthis->Do_Movement<NT>();
		}


		//========================================================
		// Main hook to person planner - called 5 minutes prior to estimated departure
		//--------------------------------------------------------
		feature_prototype void Schedule_Movement(typename TargetType::ParamType departure_time, typename TargetType::Param2Type movement, requires(
			check(typename TargetType::ParamType, Basic_Units::Concepts::Is_Time_Value) && 
			check(typename TargetType::Param2Type, Movement_Plan_Components::Concepts::Is_Movement_Plan) && 
			(check_as_given(typename TargetType::Param2Type, is_pointer) || check_as_given(typename TargetType::Param2Type, is_reference))))
		{
			this->Movement<typename TargetType::Param2Type>(movement);
			this->Movement_Scheduled<bool>(true);

			// if departure_time is greater than current iteration, load pre-trip stuff on current iteration, otherwise skip pretrip and schedule departure
			if (departure_time > _iteration + 2)
			{
				int iter = Simulation_Time.template Convert_Time_To_Simulation_Timestep<typename TargetType::ParamType>(_iteration+1);
				load_event(ComponentType,Movement_Conditional,Pretrip_Information_Acquisition_Event,Simulation_Time.template Convert_Time_To_Simulation_Timestep<typename TargetType::ParamType>(_iteration+1),Scenario_Components::Types::PRETRIP_INFORMATION_ACQUISITION,NULLTYPE);
			}
			else
			{
				int iter = Simulation_Time.template Convert_Time_To_Simulation_Timestep<typename TargetType::ParamType>(_iteration+1);
				load_event(ComponentType,Movement_Conditional,Pretrip_Routing_Event,Simulation_Time.template Convert_Time_To_Simulation_Timestep<typename TargetType::ParamType>(_iteration+1),Scenario_Components::Types::END_OF_ITERATION,NULLTYPE);			
			}
			
		}
		feature_prototype void Schedule_Movement(typename TargetType::ParamType departure_time, typename TargetType::Param2Type movement, requires(
			!check(typename TargetType::ParamType, Basic_Units::Concepts::Is_Time_Value) || 
			!check(typename TargetType::Param2Type, Movement_Plan_Components::Concepts::Is_Movement_Plan) || 
			(!check_as_given(typename TargetType::Param2Type, is_pointer) && ! check_as_given(typename TargetType::Param2Type, is_reference))))
		{
			assert_check(typename TargetType::ParamType, Basic_Units::Concepts::Is_Time_Value, "Error, must use a valid time value when scheduling departure.");
			assert_check(typename TargetType::Param2Type, Movement_Plan_Components::Concepts::Is_Movement_Plan, "Error, movement parameter is not a valid Movement_Plan interface.");
			assert_check(typename TargetType::Param2Type, is_pointer, "Error, must pass movement plan interface as a pointer or reference.");
		}
		
			

		//========================================================
		// Information Acquisition Functionality
		//--------------------------------------------------------
		feature_prototype void Do_Pretrip_Information_Acquisition()
		{
			// interfaces
			define_component_interface(Parent_Person_Itf, typename get_type_of(Parent_Person), Person_Components::Prototypes::Person, ComponentType);
			define_component_interface(Vehicle_Itf, typename get_type_of(Parent_Person)::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
			define_component_interface(movement_itf, typename Vehicle_Itf::get_type_of(movement_plan),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
			define_component_interface(Routing_Itf, typename get_type_of(Parent_Person)::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
			define_component_interface(network_itf, typename Parent_Person_Itf::get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
			define_container_and_value_interface(links, link_itf, typename network_itf::get_type_of(links_container),Containers::Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(zones, zone_itf, typename network_itf::get_type_of(zones_container),Containers::Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			
			// HAR interface
			define_component_interface(advisory_radio_itf, typename link_itf::get_type_of(advisory_radio), Advisory_ITS_Components::Prototypes::Advisory_ITS, ComponentType);


			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Routing_Itf* itf= person ->template router<Routing_Itf*>();	
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			network_itf* network = person->template network_reference<network_itf*>();
			movement_itf* movements = this->Movement<movement_itf*>();
			link_itf* origin_link = movements->template origin<link_itf*>();
			advisory_radio_itf* har = origin_link->template advisory_radio<advisory_radio_itf*>();
			

			typedef Network_Event<typename Component_Type::base_network_event_type> event_itf;
			typedef Network_Event<typename Component_Type::weather_network_event_type> weather_itf;
			typedef Network_Event<typename Component_Type::accident_network_event_type> accident_itf;

			if (har != nullptr)
			{	
				vector<event_itf*> base_events;
				har->template Get_Displayed_Messages<typename Component_Type::base_network_event_type>(base_events);

				// process weather events from HAR
				vector<weather_itf*> weather_events;
				har->template Get_Displayed_Messages<typename Component_Type::weather_network_event_type>(weather_events);
				for (typename vector<weather_itf*>::iterator w_itr = weather_events.begin(); w_itr != weather_events.end(); ++w_itr)
				{
					this->Evaluate_Network_Event<weather_itf*>(*w_itr);
				}
			
				// process accident events from HAR
				vector<accident_itf*> accident_events;
				har->template Get_Displayed_Messages<typename Component_Type::accident_network_event_type>(accident_events);
				for (typename vector<accident_itf*>::iterator a_itr = accident_events.begin(); a_itr != accident_events.end(); ++a_itr)
				{
					this->Evaluate_Network_Event<accident_itf*>(*a_itr);
				}
			}
			
		}

		feature_prototype void Evaluate_Network_Event(TargetType event, requires(check(TargetType, Network_Event_Components::Concepts::Is_Weather_Event_Prototype)))
		{
			// interfaces
			define_component_interface(Parent_Person_Itf, typename get_type_of(Parent_Person), Person_Components::Prototypes::Person, ComponentType);
			define_component_interface(planning_itf, typename Parent_Person_Itf::get_type_of(Planning_Faculty), Person_Components::Prototypes::Person_Planner, ComponentType);
			define_component_interface(scheduler_itf, typename Parent_Person_Itf::get_type_of(Scheduling_Faculty), Person_Components::Prototypes::Person_Scheduler, ComponentType);
			define_component_interface(destination_chooser_itf, typename planning_itf::get_type_of(Destination_Choice_Faculty), Person_Components::Prototypes::Destination_Chooser, ComponentType);
			define_component_interface(Vehicle_Itf, typename get_type_of(Parent_Person)::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
			define_component_interface(movement_itf, typename Vehicle_Itf::get_type_of(movement_plan),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
			//define_component_interface(activity_itf, typename movement_itf::get_type_of(activity_reference),Activity_Components::Prototypes::Activity_Planner, ComponentType);
			define_component_interface(Routing_Itf, typename get_type_of(Parent_Person)::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
			define_component_interface(network_itf, typename Parent_Person_Itf::get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
			define_container_and_value_interface(links, link_itf, typename network_itf::get_type_of(links_container),Containers::Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(locations, location_itf, typename link_itf::get_type_of(activity_locations),Containers::Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(zones, zone_itf, typename network_itf::get_type_of(zones_container),Containers::Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface(activity_container_itf, activity_itf, typename scheduler_itf::get_type_of(Activity_Container),Containers::Random_Access_Sequence_Prototype, Activity_Components::Prototypes::Activity_Planner, ComponentType);
			
			// person, router, etc. interfaces
			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			planning_itf* planner = person->template Planning_Faculty<planning_itf*>();
			destination_chooser_itf* dest_chooser = planner->template Destination_Choice_Faculty<destination_chooser_itf*>();
			Routing_Itf* itf= person ->template router<Routing_Itf*>();	
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			network_itf* network = person->template network_reference<network_itf*>();
			movement_itf* movement = this->Movement<movement_itf*>();

			// event interface
			typedef Network_Event_Components::Prototypes::Network_Event<typename Component_Type::weather_network_event_type> weather_itf;
			weather_itf* weather_event = (weather_itf*)event;
	
			// If event affects traveler
			if (this->Is_Event_Relevant<weather_itf*>(weather_event))
			{
				this->Replanning_Needed<bool>(true);
				person->template has_done_replanning<bool>(true);

				// get all zones affected by weather
				vector<location_itf*> *unaffected_locations = weather_event->template unaffected_locations<vector<location_itf*>*>();

				// replan, select new zone ouside of affected area
				location_itf* orig, * dest, *next;
				zone_itf* zone_dest = movement->template destination<zone_itf*>();
				int old_dest_id = zone_dest == nullptr ? -1 : movement->template destination<zone_itf*>()->template uuid<int>();

				activity_itf* prev_act = person->template previous_activity_plan<Target_Type<NT,activity_itf*,Simulation_Timestep_Increment>>(_iteration);
				if (prev_act == nullptr) orig = person->template Home_Location<location_itf*>();
				else orig = prev_act->template Location<location_itf*>();

				activity_itf* next_act = person->template next_activity_plan<Target_Type<NT,activity_itf*,Simulation_Timestep_Increment>>(_iteration);
				if (next_act == nullptr) next = person->template Home_Location<location_itf*>();
				else next = next_act->template Location<location_itf*>();

				dest = dest_chooser->template Choose_Destination<location_itf*>(orig,next,unaffected_locations);
				movement->template destination<location_itf*>(dest);
				movement->template destination<link_itf*>(dest->template origin_links<links*>()->at(0));
				activity_itf* act = movement->template destination_activity_reference<activity_itf*>();
				act->template Location<location_itf*>(dest);

				//cout << endl << "Destination Replanned due to weather, switch from zone '" << old_dest_id << "', to new zone '" << dest->zone<zone_itf*>()->uuid<int>()<<"'.";
			}
		}
		feature_prototype void Evaluate_Network_Event(TargetType event, requires(check(TargetType, Network_Event_Components::Concepts::Is_Accident_Event_Prototype)))
		{
			typedef Network_Event_Components::Prototypes::Network_Event<typename Component_Type::accident_network_event_type> accident_itf;
			accident_itf* accident_event = (accident_itf*)event;
			
			//cout << endl << "EVALUATING ACCIDENT EVENT: ";

			if (this->Is_Event_Relevant<accident_itf*>(accident_event))
			{
				cout << endl << "ACCIDENT_ADVISORY: there is an accident along your route.";
			}
		}
		feature_prototype void Evaluate_Network_Event(TargetType event, requires(!check(TargetType,Network_Event_Components::Concepts::Is_Weather_Event_Prototype) && !check(TargetType, Network_Event_Components::Concepts::Is_Accident_Event_Prototype)))
		{
			assert_check(TargetType, Is_Polaris_Prototype, "Warning: TargetType event must be a polaris prototype.");
			assert_check(TargetType, Network_Event_Components::Concepts::Is_Weather_Event_Prototype, "Warning: TargetType component must be a weather_network_event, or ");
			assert_check(TargetType, Network_Event_Components::Concepts::Is_Accident_Event_Prototype, "TargetType component must be an accident_network_event_type.");
		}

		feature_prototype bool Is_Event_Relevant(TargetType event, requires(check(TargetType, Network_Event_Components::Concepts::Is_Weather_Event_Prototype)))
		{
			// interfaces
			define_component_interface(Parent_Person_Itf, typename get_type_of(Parent_Person), Person_Components::Prototypes::Person, ComponentType);
			define_component_interface(Vehicle_Itf, typename get_type_of(Parent_Person)::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
			define_component_interface(movement_itf, typename get_type_of(Movement),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
			define_component_interface(Routing_Itf, typename get_type_of(Parent_Person)::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
			define_component_interface(network_itf, typename Parent_Person_Itf::get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
			define_container_and_value_interface(trajectory_interface,trajectory_unit_interface,typename movement_itf::get_type_of(trajectory_container),Containers::Random_Access_Sequence_Prototype,Movement_Plan_Components::Prototypes::Trajectory_Unit_Prototype,ComponentType);
			define_container_and_value_interface(links, link_itf, typename network_itf::get_type_of(links_container),Containers::Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(zones, zone_itf, typename network_itf::get_type_of(zones_container),Containers::Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			
			// get destination link for current movement
			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			movement_itf* movement = this->Movement<movement_itf*>();
			zone_itf* destination = movement->template destination<zone_itf*>();
			//link_itf* destination_link = movement->destination<link_itf*>();

			// interface to event
			typedef Network_Event<typename Component_Type::weather_network_event_type> weather_itf;
			weather_itf* my_event = (weather_itf*)event;

			// does event affect destination zone?
			vector<zone_itf*> *affected_zones = my_event->template affected_zones<vector<zone_itf*> *>();
			//cout << endl << "Number of affected zones = " << affected_zones->size();

			zone_itf* affected_zone;
			for (typename vector<zone_itf*>::iterator itr = affected_zones->begin(); itr != affected_zones->end(); ++itr)
			{
				affected_zone = *itr;
				//cout << endl << "Affected ID, destination ID: " << affected_zone->uuid<int>();
				//cout << ", " << destination;
				if (affected_zone == destination || destination == nullptr) return true;
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
		feature_prototype bool Is_Event_Relevant(TargetType event, requires(check(TargetType, Network_Event_Components::Concepts::Is_Accident_Event_Prototype)))
		{
			// interfaces
			define_component_interface(Parent_Person_Itf, typename get_type_of(Parent_Person), Person_Components::Prototypes::Person, ComponentType);
			define_component_interface(Vehicle_Itf, typename get_type_of(Parent_Person)::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
			define_component_interface(movement_itf, typename get_type_of(Movement),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
			define_component_interface(Routing_Itf, typename get_type_of(Parent_Person)::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
			define_component_interface(network_itf, typename Parent_Person_Itf::get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
			define_container_and_value_interface(trajectory_interface,trajectory_unit_interface,typename movement_itf::get_type_of(trajectory_container),Containers::Random_Access_Sequence_Prototype,Movement_Plan_Components::Prototypes::Trajectory_Unit_Prototype,ComponentType);
			define_container_and_value_interface(links, link_itf, typename network_itf::get_type_of(links_container),Containers::Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			
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
				affected_link = *itr;
				for (typename trajectory_interface::iterator t_itr = trajectory->begin(); t_itr != trajectory->end(); ++t_itr)
				{
					traj_unit = *t_itr;
					if (affected_link == traj_unit->template link<link_itf*>()) return true;
				}
			}
			return false;
		}

		feature_prototype void Get_Event_Extents(TargetType event, unordered_set<int>& affected_indices, requires(check(TargetType, Network_Event_Components::Concepts::Is_Weather_Event_Prototype)))
		{
			// interfaces
			define_component_interface(Parent_Person_Itf, typename get_type_of(Parent_Person), Person_Components::Prototypes::Person, ComponentType);
			define_component_interface(Vehicle_Itf, typename get_type_of(Parent_Person)::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
			define_component_interface(movement_itf, typename get_type_of(Movement),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
			define_component_interface(Routing_Itf, typename get_type_of(Parent_Person)::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
			define_component_interface(network_itf, typename Parent_Person_Itf::get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);		
			define_container_and_value_interface(trajectory_interface,trajectory_unit_interface,typename movement_itf::get_type_of(trajectory_container),Containers::Random_Access_Sequence_Prototype,Movement_Plan_Components::Prototypes::Trajectory_Unit_Prototype,ComponentType);
			define_container_and_value_interface(links, link_itf, typename network_itf::get_type_of(links_container),Containers::Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(location_container_itf, location_itf, typename link_itf::get_type_of(activity_locations),Containers::Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_component_interface(zone_itf, typename location_itf::get_type_of(zone), Zone_Components::Prototypes::Zone_Prototype, ComponentType);		
			
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
		feature_prototype void Do_Pretrip_Replanning()
		{
			// interfaces
			define_component_interface(Parent_Person_Itf, typename get_type_of(Parent_Person), Person_Components::Prototypes::Person, ComponentType);
			define_component_interface(Vehicle_Itf, typename get_type_of(Parent_Person)::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
			define_component_interface(movement_itf, typename Vehicle_Itf::get_type_of(movement_plan),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
			define_component_interface(Routing_Itf, typename get_type_of(Parent_Person)::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
			define_component_interface(network_itf, typename Parent_Person_Itf::get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
			define_container_and_value_interface(links, link_itf, typename network_itf::get_type_of(links_container),Containers::Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);

			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Routing_Itf* itf= person ->template router<Routing_Itf*>();	
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			network_itf* network = person->template network_reference<network_itf*>();
			movement_itf* movements = this->Movement<movement_itf*>();
			link_itf* origin_link = movements->template origin<link_itf*>();
		}

		//========================================================
		// Pre-trip Rerouting Functionality
		//--------------------------------------------------------
		feature_prototype void Do_Pretrip_Routing()
		{
			// interfaces
			define_component_interface(Parent_Person_Itf, typename get_type_of(Parent_Person), Person_Components::Prototypes::Person, ComponentType);
			define_component_interface(Planning_Itf, typename Parent_Person_Itf::get_type_of(Planning_Faculty),Prototypes::Person_Planner,ComponentType);
			define_component_interface(Vehicle_Itf, typename get_type_of(Parent_Person)::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
			define_component_interface(movement_itf, typename Vehicle_Itf::get_type_of(movement_plan),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
			define_component_interface(Routing_Itf, typename get_type_of(Parent_Person)::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
			define_component_interface(network_itf, typename Parent_Person_Itf::get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
			define_component_interface(location_itf, typename Parent_Person_Itf::get_type_of(current_location), Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(links, link_itf, typename network_itf::get_type_of(links_container),Containers::Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			
			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Planning_Itf* planner = person->template Planning_Faculty<Planning_Itf*>();
			/*Routing_Itf* itf= person ->template router<Routing_Itf*>();	
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			network_itf* network = person->template network_reference<network_itf*>();*/
			movement_itf* movements = this->Movement<movement_itf*>();
			
			// check if movement plan origin is aligned with persons current location, if not change it
			if (movements->template origin<location_itf*>() != person->template current_location<location_itf*>()) movements->template origin<location_itf*>(person->template current_location<location_itf*>());

			//itf->template movement_plan<movement_itf*>(movements);
			//itf->template Schedule_Route_Computation<int>(_iteration+1);
			planner->template Schedule_New_Routing<movement_itf*>(_iteration+1,movements);
		}

		//========================================================
		// Movement Functionality
		//--------------------------------------------------------
		feature_prototype void Do_Movement()
		{
			//this->Movement_Scheduled<bool>(true);
			debug("DO_Movement @,"<<_iteration);


			// interfaces
			define_component_interface(Parent_Person_Itf, typename get_type_of(Parent_Person), Person_Components::Prototypes::Person, ComponentType);
			define_component_interface(Vehicle_Itf, typename get_type_of(Parent_Person)::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
			define_component_interface(movement_itf, typename Vehicle_Itf::get_type_of(movement_plan),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
			define_component_interface(Routing_Itf, typename get_type_of(Parent_Person)::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
			define_component_interface(network_itf, typename Parent_Person_Itf::get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
			define_component_interface(location_itf, typename Parent_Person_Itf::get_type_of(current_location), Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(links, link_itf, typename network_itf::get_type_of(links_container),Containers::Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_component_interface(Activity_Itf, typename movement_itf::get_type_of(destination_activity_reference),Activity_Components::Prototypes::Activity_Planner,ComponentType);

			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			Routing_Itf* itf= person ->template router<Routing_Itf*>();	
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			network_itf* network = person->template network_reference<network_itf*>();
			movement_itf* movements = this->Movement<movement_itf*>();
			Activity_Itf* act = movements->template destination_activity_reference<Activity_Itf*>();	

			// Schedule the routing if the vehicle is not already in the network, otherwise return false
			if (movements->template valid_trajectory<bool>() && (vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::UNLOADED || vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::OUT_NETWORK))
			{
				// set the persons location to be the destination
				person->template current_location<location_itf*>(movements->template destination<location_itf*>());

				// if auto trip, push to network, if not skip (for now)
				if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
				{
					link_itf* origin_link = movements->template origin<link_itf*>();
					origin_link->push_vehicle(vehicle);
				}
				// for all other trips - do magic move and arrive at activity
				else
				{
					act->template Arrive_At_Activity<NT>();
				}
			}
		}

		//========================================================
		// Arrival Functionality - ends current movement, plans next if necessary
		//--------------------------------------------------------
		feature_prototype void Arrive_At_Destination()
		{
			// free up movement schedule
			this->Movement_Scheduled<bool>(false);

			// interfaces
			define_component_interface(Parent_Person_Itf, typename get_type_of(Parent_Person), Person_Components::Prototypes::Person, ComponentType);
			define_component_interface(Scheduler_Itf, typename Parent_Person_Itf::get_type_of(Scheduling_Faculty), Person_Components::Prototypes::Person_Scheduler, ComponentType);
			define_component_interface(Planner_Itf, typename Parent_Person_Itf::get_type_of(Planning_Faculty), Person_Components::Prototypes::Person_Planner, ComponentType);
			define_component_interface(Vehicle_Itf, typename get_type_of(Parent_Person)::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
			define_component_interface(movement_itf, typename Vehicle_Itf::get_type_of(movement_plan),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
			define_component_interface(Routing_Itf, typename get_type_of(Parent_Person)::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
			define_component_interface(network_itf, typename Parent_Person_Itf::get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
			define_component_interface(skim_itf, typename network_itf::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_component_interface(location_itf, typename Parent_Person_Itf::get_type_of(current_location), Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_component_interface(zone_itf, typename location_itf::get_type_of(zone), Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface(links_container_itf, link_itf, typename network_itf::get_type_of(links_container),Containers::Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(turns_container_itf, turn_itf, typename network_itf::get_type_of(turn_movements_container),Containers::Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
			define_component_interface(Activity_Itf, typename movement_itf::get_type_of(destination_activity_reference),Activity_Components::Prototypes::Activity_Planner,ComponentType);

			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
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
			Activity_Itf* next_act = scheduler->template next_activity_plan<Target_Type<NT,Activity_Itf*, Simulation_Timestep_Increment>>(_iteration);
			if (next_act == nullptr) return;

			// don't add additional movement if already at home
			if (act->template Location<location_itf*>() == person->template Home_Location<location_itf*>()) return;

			movement_itf* next_movement = next_act->template movement_plan<movement_itf*>();

			// Define time window after current activity is completed
			Time_Seconds end_this = act->template Start_Time<Time_Seconds>() + act->template Duration<Time_Seconds>();
			Time_Seconds begin_next = next_act->template Start_Time<Time_Seconds>();

			// O/D ids
			int o_id = act->template Location<location_itf*>()->template zone<zone_itf*>()->template uuid<int>();
			int d_id = next_act->template Location<location_itf*>()->template zone<zone_itf*>()->template uuid<int>();
			int h_id = person->template Home_Location<zone_itf*>()->template uuid<int>();

			// expected travel times
			typedef Vehicle_Components::Types::Vehicle_Type_Keys MODE;
			Time_Seconds ttime_this_to_next = network->template Get_LOS<Target_Type<NT,Time_Seconds,int,MODE,Time_Seconds>>(o_id,d_id,MODE::SOV,end_this);
			Time_Seconds ttime_this_to_home = network->template Get_LOS<Target_Type<NT,Time_Seconds,int,MODE,Time_Seconds>>(o_id,h_id,MODE::SOV,end_this);
			Time_Seconds ttime_home_to_next = network->template Get_LOS<Target_Type<NT,Time_Seconds,int,MODE,Time_Seconds>>(h_id,d_id,MODE::SOV,end_this + ttime_this_to_home);
			Time_Seconds min_home_duration = min((float)ttime_this_to_home,(float)ttime_home_to_next);
			//=====================================================================
			// Person can go home first, schedule an additional return home movement
			if (begin_next - end_this > ttime_this_to_home + ttime_home_to_next + min_home_duration)
			{
				// Create movement plan and give it an ID
				/*movement_itf* move = (movement_itf*)Allocate<typename Scheduler_Itf::get_type_of(Movement_Plans_Container)::unqualified_value_type>();
				move->template initialize_trajectory<NULLTYPE>();*/
				/*Activity_Itf* null_act = nullptr;
				move->template destination_activity_reference<Activity_Itf*>(null_act);*/

				location_itf* orig = act->template Location<location_itf*>();
				location_itf* dest = person->template Home_Location<location_itf*>();

				// check that origin and destination are valid
				if (orig == nullptr || dest == nullptr) THROW_WARNING("WARNING: movement can not happen as no origin or destination is null pointer.");
				if (orig->template origin_links<links_container_itf&>().size() == 0 || dest->template origin_links<links_container_itf&>().size() == 0) THROW_WARNING("WARNING: movement from " << orig->template uuid<int>() << " to " << dest->template uuid<int>() << ", can not happen as no origin / destination links are available for the locations.");
				if (orig->template origin_links<links_container_itf&>().at(0)->template outbound_turn_movements<turns_container_itf*>()->size() == 0 || dest->template origin_links<links_container_itf&>().at(0)->template outbound_turn_movements<turns_container_itf*>()->size() == 0) THROW_WARNING("WARNING: cannot route trip as orig or dest links do not have valid turn movements: [Perid.actid,acttype,orig_link,dest_link,orig_zone,dest_zone]: "/*<<concat(this->Parent_Person<ComponentType,CallerType,int>()) << "." << concat(this->Activity_Plan_ID<ComponentType, CallerType,int>()) <<", " << concat(this->Activity_Type<ComponentType, CallerType,ACTIVITY_TYPES>()) << ", " <<o_link->uuid<int>() << ", " << d_link->uuid<int>() << ", "  << orig->zone<_zone_itf*>()->uuid<int>() << ", " << dest->zone<_zone_itf*>()->uuid<int>()*/);

				// GENERATE A NEW AT HOME ACTIVITY
				typedef Activity_Components::Prototypes::Activity_Planner<typename ComponentType::Master_Type::at_home_activity_plan_type,ComponentType> at_home_activity_itf;
				at_home_activity_itf* new_act = (at_home_activity_itf*)Allocate<typename ComponentType::Master_Type::at_home_activity_plan_type>();
				//new_act->template movement_plan<movement_itf*>(move);
				new_act->template Parent_Planner<Planner_Itf*>(planner);
				new_act->template Initialize<Target_Type<NT,void,Time_Seconds,Vehicle_Components::Types::Vehicle_Type_Keys>>(end_this, ttime_this_to_home,act->template Mode<MODE>());

				// If the trip is valid, assign to a movement plan and add to the schedule
				//move->template origin<location_itf*>(orig);
				//move->template destination<location_itf*>(dest);
				//move->template origin<link_itf*>(orig->template origin_links<links_container_itf&>().at(0));
				//move->template destination<link_itf*>(dest->template origin_links<links_container_itf&>().at(0));
				//move->template departed_time<Simulation_Timestep_Increment>(end_this);
				//scheduler->template Add_Movement_Plan<movement_itf*>(move);

				Time_Seconds next_depart_time = next_act->template Start_Time<Time_Seconds>() - ttime_home_to_next;
				//cout << endl << "Returning home @t=" << (int)end_this << ", expected arrival @t="<<(int)end_this+(int)ttime_this_to_home<<", approximate departure from home for next activity @t="<< (int)next_depart_time;
			}

			//=====================================================================
			// otherwise either wait at current activity or depart and spend extra time at next activity
			else
			{

			}
		}

		//========================================================
		// Basic Features
		//--------------------------------------------------------
		feature_accessor(Parent_Person,none,none);
		feature_accessor(Movement,none,none);
		feature_accessor(Movement_Scheduled,none,none);
		feature_accessor(Replanning_Needed,none,none);
	};

}

}