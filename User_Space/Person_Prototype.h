#pragma once

//#include "User_Space_Includes.h"
#include "Activity_Prototype.h"
#include "Network_Event_Prototype.h"
#include "Activity_Location_Prototype.h"
#include "Network_Prototype.h"
#include "Zone_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Network_Skimming_Prototype.h"
#include "Person_Properties_Prototype.h"
#include "Person_Planner_Prototype.h"
#include "Person_Scheduler_Prototype.h"
#include "Person_Perception_Prototype.h"

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


	concept struct Is_Person
	{
		check_feature(Has_Initialize_Defined,Initialize);
		check_getter(Has_Properties_Defined,Properties);
		check_getter(Has_Planner_Defined,Planning_Faculty);
		define_default_check(Has_Initialize_Defined && Has_Properties_Defined && Has_Planner_Defined);
	};

	concept struct Is_Transims_Person
	{
		check_concept(Is_Person_Check, Is_Person);
		check_typename_state(Is_Transims_Check,Is_Transims,true_type);
		define_default_check(Is_Person_Check && Is_Transims_Check);
	};
	concept struct Is_CTRAMP_Person
	{
		check_concept(Is_Person_Check, Is_Person);
		check_typename_state(Is_CTRAMP_Check,Is_Transims,true_type);
		define_default_check(Is_Person_Check && Is_CTRAMP_Check);
	};

	concept struct Has_Initialize
	{
		check_feature(Has_Initialize_Defined,Initialize);
		define_default_check(Has_Initialize_Defined);
	};

}

namespace Types
{

}

namespace Prototypes
{
	prototype struct Person ADD_DEBUG_INFO
	{
		tag_as_prototype;

		// Event handling
		declare_feature_conditional(Agent_Conditional)
		{
			response.next._iteration = END;
			response.next._sub_iteration = 0;
			response.result = true;
		}
		declare_feature_event(Agent_Event)
		{
			typedef Person<ComponentType, ComponentType> _Person_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Interface* pthis =(_Person_Interface*)_pthis;
			pthis->template Set_Home_Location<NT>();
		}

		// Initializers
		feature_prototype void Initialize(TargetType id, requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			this_component()->template Initialize<ComponentType,CallerType, TargetType>(id);
			load_event(ComponentType,Agent_Conditional,Agent_Event,_iteration+1,0,NULLTYPE);
		}
		feature_prototype void Initialize(TargetType id, requires(!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}
		feature_prototype void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type home_zone, typename TargetType::Param3Type network_ref, typename TargetType::Param4Type scenario_ref/*,requires(check(ComponentType,Concepts::Has_Initialize))*/)
		{
			this_component()->template Initialize<ComponentType,CallerType, TargetType>(id, home_zone, network_ref, scenario_ref);
			load_event(ComponentType,Agent_Conditional,Agent_Event,_iteration+1,0,NULLTYPE);
		}
		//feature_prototype void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type home_zone, typename TargetType::Param3Type network_ref, typename TargetType::Param4Type scenario_ref,requires(!check(ComponentType,Concepts::Has_Initialize)))
		//{
		//	assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		//}
		//feature_prototype void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type trip, requires(check(ComponentType,Concepts::Has_Initialize)))
		//{
		//	this_component()->Initialize<ComponentType,CallerType, TargetType>(id, trip);
		//	load_event(ComponentType,Agent_Conditional,Agent_Event,this->First_Iteration<Simulation_Timestep_Increment>(),0,NULLTYPE);
		//}
		//feature_prototype void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type trip, requires(!check(ComponentType,Concepts::Has_Initialize)))
		//{
		//	assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		//}

		// Sub-component accessors	

		feature_prototype void Set_Home_Location()
		{
			this_component()->template Set_Home_Location<ComponentType,CallerType, TargetType>();
		}
		feature_accessor(Planning_Faculty,none,none);
		feature_accessor(Perception_Faculty,none,none);
		feature_accessor(router,none,none);
		feature_accessor(Moving_Faculty,none,none);
		feature_accessor(Properties,none,none);
		feature_accessor(Static_Properties,none,none);
		feature_accessor(vehicle,none,none);
		feature_accessor(has_done_replanning,none,none);
		
		feature_accessor(has_pretrip_information,none,none);

		// External knowledge accessor
		feature_accessor(network_reference,none,none);
		feature_accessor(scenario_reference,none,none);

		// Basic property members
		feature_accessor(uuid,none,none);
		feature_accessor(internal_id,none,none);
		feature_accessor(Next_Rand,none,none);
		feature_accessor(First_Iteration,none,none);
		feature_accessor(Write_Activity_Files,none,none);

		// Accessors for setting the home/work locations (stores only an index into the network_reference::activity_locations_container) - overloaded to return either th loc_index, the location interface or the zone interface
		feature_prototype TargetType Home_Location(requires(check(TargetType, Activity_Location_Components::Concepts::Is_Activity_Location) && check_as_given(TargetType,is_pointer)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(perception_itf,typename get_type_of(Perception_Faculty),Person_Perception,ComponentType);
			define_component_interface(network_itf, typename perception_itf::get_type_of(Network),Network_Components::Prototypes::Network_Prototype,ComponentType);

			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->Perception_Faculty<perception_itf*>()->template Network<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();
			
			int loc_id = properties->template home_location_id<int>();
			return (TargetType)(*locations)[loc_id];						
		}
		feature_prototype TargetType Home_Location(requires(check(TargetType, Zone_Components::Concepts::Is_Zone) && check_as_given(TargetType,is_pointer)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			int loc_id = properties->template home_location_id<int>();
			activity_location_itf* loc = (*locations)[loc_id];	
			return loc->template zone<TargetType>();
		}
		feature_prototype TargetType Home_Location(requires(check(TargetType, is_integral)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			return properties->template home_location_id<TargetType>();
		}
		feature_prototype TargetType Home_Location(requires(check(TargetType, !is_integral) && ( (check(TargetType,!Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType,!Zone_Components::Concepts::Is_Zone) ) || check_as_given(TargetType,!is_pointer) ) ) )
		{
			assert_check(TargetType,is_integral,"Error, Home_Location can only be requested as an Integral type - which returns location index, or as an Activity_Location refernence type, which returns the actual location.");
		}
		feature_prototype void Home_Location(TargetType location_index, requires(check(TargetType, is_integral)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			if (location_index < 0 || location_index >= locations->size()) THROW_EXCEPTION("Error: location index "<<location_index<<" does not exist in network locations container.  Index out of range (0,"<<locations->size()<<").");
			properties->template home_location_id<TargetType>(location_index);
		}
		feature_prototype void Home_Location(TargetType location_index, requires(check(TargetType, !is_integral)))
		{
			assert_check(TargetType, is_integral, "Error, Home_Location can only be set by passing an integral index from network::activity_locations_container");
		}
		
		feature_prototype TargetType Work_Location(requires(check(TargetType, Activity_Location_Components::Concepts::Is_Activity_Location) && check_as_given(TargetType,is_pointer)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			int loc_id = properties->template work_location_id<int>();
			if (loc_id < 0)
			{
				THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' does not have a valid work location.  Should not be requesting this.");
				return nullptr;
			}
			return (TargetType)(*locations)[loc_id];						
		}
		feature_prototype TargetType Work_Location(requires(check(TargetType, Zone_Components::Concepts::Is_Zone) && check_as_given(TargetType,is_pointer)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			int loc_id = properties->template work_location_id<int>();
			if (loc_id < 0)
			{
				THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' does not have a valid work location.  Should not be requesting this.");
				return nullptr;
			}
			activity_location_itf* loc = (*locations)[loc_id];	
			return loc->template zone<TargetType>();
		}
		feature_prototype TargetType Work_Location(requires(check(TargetType, is_integral)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			return properties->template work_location_id<TargetType>();
		}
		feature_prototype TargetType Work_Location(requires(check(TargetType, !is_integral) && ( (check(TargetType,!Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType,!Zone_Components::Concepts::Is_Zone) ) || check_as_given(TargetType,!is_pointer) ) ) )
		{
			assert_check(TargetType,is_integral,"Error, Home_Location can only be requested as an Integral type - which returns location index, or as an Activity_Location refernence type, which returns the actual location.");
		}
		feature_prototype void Work_Location(TargetType location_index, requires(check(TargetType, is_integral)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			if (location_index < -1 || location_index >= (signed int)locations->size()) THROW_EXCEPTION("Error: location index "<<location_index<<" does not exist in network locations container.");
			properties->template work_location_id<TargetType>(location_index);
		}
		feature_prototype void Work_Location(TargetType location_index, requires(check(TargetType, !is_integral)))
		{
			assert_check(TargetType, is_integral, "Error, work location can only be set by passing an integral index from network::activity_locations_container");
		}

		feature_prototype TargetType School_Location(requires(check(TargetType, Activity_Location_Components::Concepts::Is_Activity_Location) && check_as_given(TargetType,is_pointer)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			int loc_id = properties->template school_location_id<int>();
			if (loc_id < 0)
			{
				THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' does not have a valid school location.  Should not be requesting this.");
				return nullptr;
			}
			return (TargetType)(*locations)[loc_id];						
		}
		feature_prototype TargetType School_Location(requires(check(TargetType, Zone_Components::Concepts::Is_Zone) && check_as_given(TargetType,is_pointer)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			int loc_id = properties->template school_location_id<int>();
			if (loc_id < 0)
			{
				THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' does not have a valid school location.  Should not be requesting this.");
				return nullptr;
			}
			activity_location_itf* loc = (*locations)[loc_id];	
			return loc->template zone<TargetType>();
		}
		feature_prototype TargetType School_Location(requires(check(TargetType, is_integral)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			return properties->template school_location_id<TargetType>();
		}
		feature_prototype TargetType School_Location(requires(check(TargetType, !is_integral) && ( (check(TargetType,!Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType,!Zone_Components::Concepts::Is_Zone) ) || check_as_given(TargetType,!is_pointer) ) ) )
		{
			assert_check(TargetType,is_integral,"Error, School_Location can only be requested as an Integral type - which returns location index, or as an Activity_Location refernence type, which returns the actual location.");
		}
		feature_prototype void School_Location(TargetType location_index, requires(check(TargetType, is_integral)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			if (location_index < -1 || location_index >= (signed int)locations->size()) THROW_EXCEPTION("Error: location index "<<location_index<<" does not exist in network locations container.");
			properties->template school_location_id<TargetType>(location_index);
		}
		feature_prototype void School_Location(TargetType location_index, requires(check(TargetType, !is_integral)))
		{
			assert_check(TargetType, is_integral, "Error, work location can only be set by passing an integral index from network::activity_locations_container");
		}
		
		define_feature_exists_check(Choose_Work_Location, has_Choose_Work_Location);
		feature_prototype void Choose_Work_Location(requires(check(ComponentType,has_Choose_Work_Location)))
		{
			this_component()->template Choose_Work_Location<ComponentType,CallerType,TargetType>();
		}
		feature_prototype void Choose_Work_Location(requires(!check(ComponentType,has_Choose_Work_Location)))
		{
			assert_check(ComponentType,has_Choose_Work_Location,"ComponentType does not have Choose_Work_Location feature.");
		}

		define_feature_exists_check(Choose_School_Location, has_Choose_School_Location);
		feature_prototype void Choose_School_Location(requires(check(ComponentType,has_Choose_School_Location)))
		{
			this_component()->template Choose_School_Location<ComponentType,CallerType,TargetType>();
		}
		feature_prototype void Choose_School_Location(requires(!check(ComponentType,has_Choose_School_Location)))
		{
			assert_check(ComponentType,has_Choose_School_Location,"ComponentType does not have Choose_School_Location feature.");
		}

		feature_prototype string To_String()
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(static_properties_itf,typename get_type_of(Static_Properties), Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(zone_container_itf, zone_itf, typename network_itf::get_type_of(zones_container), Containers::Random_Access_Sequence_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
			
			properties_itf* props = this->Properties<properties_itf*>();
			static_properties_itf* static_props = this->Static_Properties<static_properties_itf*>();

			stringstream s;
			s << this->uuid<int>() << ", ";
			s << this->Home_Location<zone_itf*>()->uuid<int>();
			if (static_props->template Is_Employed<bool>())
			{
				s << ", WORK_ZONE," << this->Work_Location<zone_itf*>()->template uuid<int>();
			}
			else
			{
				s << ", DOESNT_WORK,,";
			}
			if (static_props->template Is_Student<bool>())
			{
				s << ", SCHOOL_ZONE," << this->School_Location<zone_itf*>()->template uuid<int>();
			}
			else
			{
				s << ", NOT_IN_SCHOOL,,";
			}
			return s.str();
		}
	};

	
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
			define_component_interface(vehicle_itf, typename person_itf::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype,ComponentType);
			define_component_interface(movement_itf, typename get_type_of(Movement), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);

			movement_itf* movement = pthis->template Movement<movement_itf*>();
			person_itf* person = pthis->template Parent_Person<person_itf*>();
			vehicle_itf* vehicle = person->template vehicle<vehicle_itf*>();

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
				response.next._iteration = movement->template departed_time<Simulation_Timestep_Increment>();
				response.next._sub_iteration = Scenario_Components::Types::PRETRIP_ROUTING_SUB_ITERATION;
				response.result = pthis->template Replanning_Needed<bool>();
			}
			else if (_sub_iteration == Scenario_Components::Types::PRETRIP_ROUTING_SUB_ITERATION)
			{
				_pthis->Swap_Event((Event)&Person_Mover::Pretrip_Routing_Event<NULLTYPE>);
				response.next._iteration = movement->template departed_time<Simulation_Timestep_Increment>();
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
				load_event(ComponentType,Movement_Conditional,Pretrip_Information_Acquisition_Event,Simulation_Time.template Convert_Time_To_Simulation_Timestep<typename TargetType::ParamType>(_iteration+1),Scenario_Components::Types::PRETRIP_INFORMATION_ACQUISITION,NULLTYPE);
			}
			else
			{
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
			

			typedef Network_Event<typename Component_Type::MasterType::base_network_event_type> event_itf;
			typedef Network_Event<typename Component_Type::MasterType::weather_network_event_type> weather_itf;
			typedef Network_Event<typename Component_Type::MasterType::accident_network_event_type> accident_itf;

			if (har != nullptr)
			{	
				vector<event_itf*> base_events;
				har->template Get_Displayed_Messages<typename Component_Type::MasterType::base_network_event_type>(base_events);

				// process weather events from HAR
				vector<weather_itf*> weather_events;
				har->template Get_Displayed_Messages<typename Component_Type::MasterType::weather_network_event_type>(weather_events);
				for (typename vector<weather_itf*>::iterator w_itr = weather_events.begin(); w_itr != weather_events.end(); ++w_itr)
				{
					this->Evaluate_Network_Event<weather_itf*>(*w_itr);
				}
			
				// process accident events from HAR
				vector<accident_itf*> accident_events;
				har->template Get_Displayed_Messages<typename Component_Type::MasterType::accident_network_event_type>(accident_events);
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
			define_component_interface(destination_chooser_itf, typename planning_itf::get_type_of(Destination_Choice_Faculty), Person_Components::Prototypes::Destination_Chooser, ComponentType);
			define_component_interface(Vehicle_Itf, typename get_type_of(Parent_Person)::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
			define_component_interface(movement_itf, typename Vehicle_Itf::get_type_of(movement_plan),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
			//define_component_interface(activity_itf, typename movement_itf::get_type_of(activity_reference),Activity_Components::Prototypes::Activity_Planner, ComponentType);
			define_component_interface(Routing_Itf, typename get_type_of(Parent_Person)::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
			define_component_interface(network_itf, typename Parent_Person_Itf::get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
			define_container_and_value_interface(links, link_itf, typename network_itf::get_type_of(links_container),Containers::Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(locations, location_itf, typename link_itf::get_type_of(activity_locations),Containers::Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(zones, zone_itf, typename network_itf::get_type_of(zones_container),Containers::Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface(activity_container_itf, activity_itf, typename planning_itf::get_type_of(Activity_Container),Containers::Random_Access_Sequence_Prototype, Activity_Components::Prototypes::Activity_Planner, ComponentType);
			
			// person, router, etc. interfaces
			Parent_Person_Itf* person = this->Parent_Person<Parent_Person_Itf*>();
			planning_itf* planner = person->template Planning_Faculty<planning_itf*>();
			destination_chooser_itf* dest_chooser = planner->template Destination_Choice_Faculty<destination_chooser_itf*>();
			Routing_Itf* itf= person ->template router<Routing_Itf*>();	
			Vehicle_Itf* vehicle = person->template vehicle<Vehicle_Itf*>();
			network_itf* network = person->template network_reference<network_itf*>();
			movement_itf* movement = this->Movement<movement_itf*>();

			// event interface
			typedef Network_Event_Components::Prototypes::Network_Event<typename Component_Type::MasterType::weather_network_event_type> weather_itf;
			weather_itf* weather_event = (weather_itf*)event;
	
			// If event affects traveler
			if (this->Is_Event_Relevant<weather_itf*>(weather_event))
			{
				this->Replanning_Needed<bool>(true);
				person->template has_done_replanning<bool>(true);

				// get all zones affected by weather
				vector<location_itf*> *unaffected_locations = weather_event->template unaffected_locations<vector<location_itf*>*>();

				// replan, select new zone ouside of affected area
				location_itf* orig, * dest;
				zone_itf* zone_dest = movement->template destination<zone_itf*>();
				int old_dest_id = zone_dest == nullptr ? -1 : movement->template destination<zone_itf*>()->template uuid<int>();
				activity_itf* prev_act = planner->template previous_activity_plan<Target_Type<NT,activity_itf*,Simulation_Timestep_Increment>>(_iteration);
				if (prev_act == nullptr) orig = person->template Home_Location<location_itf*>();
				else orig = prev_act->template Location<location_itf*>();
				dest = dest_chooser->template Choose_Destination<location_itf*>(orig,unaffected_locations);
				movement->template destination<location_itf*>(dest);
				movement->template destination<link_itf*>(dest->template origin_links<links*>()->at(0));
				activity_itf* act = movement->template activity_reference<activity_itf*>();
				act->template Location<location_itf*>(dest);

				//cout << endl << "Destination Replanned due to weather, switch from zone '" << old_dest_id << "', to new zone '" << dest->zone<zone_itf*>()->uuid<int>()<<"'.";
			}
		}
		feature_prototype void Evaluate_Network_Event(TargetType event, requires(check(TargetType, Network_Event_Components::Concepts::Is_Accident_Event_Prototype)))
		{
			typedef Network_Event_Components::Prototypes::Network_Event<typename Component_Type::MasterType::accident_network_event_type> accident_itf;
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
			typedef Network_Event<typename Component_Type::MasterType::weather_network_event_type> weather_itf;
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
			typedef Network_Event<typename Component_Type::MasterType::weather_network_event_type> weather_itf;
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

		feature_prototype void Get_Event_Extents(TargetType event, hash_set<int>& affected_indices, requires(check(TargetType, Network_Event_Components::Concepts::Is_Weather_Event_Prototype)))
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
			typedef Network_Event<typename Component_Type::MasterType::weather_network_event_type> weather_itf;
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

			itf->template movement_plan<movement_itf*>(movements);
			itf->template Schedule_Route_Computation<int>(_iteration+1);
		}

		//========================================================
		// Movement Functionality
		//--------------------------------------------------------
		feature_prototype void Do_Movement()
		{
			// free up movement schedule
			this->Movement_Scheduled<bool>(false);

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

			// Schedule the routing if the vehicle is not already in the network, otherwise return false
			if (movements->template valid_trajectory<bool>() && (vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::UNLOADED || vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::OUT_NETWORK))
			{
				//if (typename ComponentType::_write_activity_files) typename ComponentType::logs[_thread_id]<<"MOVEMENT:," << person->uuid<int>() << ","<<departed_time<<endl;
				origin_link->push_vehicle(vehicle);
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


	prototype struct Activity_Generator ADD_DEBUG_INFO
	{
		tag_as_prototype;

		// accessor to parent class
		feature_accessor(Parent_Planner,none,none);

		feature_prototype void Activity_Generation()
		{
			this_component()->template Activity_Generation<ComponentType,CallerType,TargetType>();
		}
	};


	prototype struct Destination_Chooser ADD_DEBUG_INFO
	{
		tag_as_prototype;

		// accessor to parent class
		feature_accessor(Parent_Planner,none,none);
		feature_accessor(choice_set_size,none,none);
		feature_accessor(choice_model,none,none);	

		feature_prototype TargetType Choose_Destination(TargetType origin, vector<TargetType>* destinations_to_use=nullptr, requires(check_as_given(TargetType,is_pointer) && check(TargetType,Activity_Location_Components::Concepts::Is_Activity_Location)))
		{
			return this_component()->template Choose_Destination<ComponentType,CallerType,TargetType>(origin,destinations_to_use);
		}
	};


	prototype struct Destination_Choice_Option ADD_DEBUG_INFO
	{
		tag_as_prototype;

		// accessor to parent class
		feature_accessor(Parent_Planner,none,none);
		feature_accessor(origin,none,none);
		feature_accessor(destination,none,none);

		feature_prototype TargetType Calculate_Utility()
		{
			return this_component()->template Calculate_Utility<ComponentType,CallerType,TargetType>();
		}
		feature_prototype TargetType Print_Utility()
		{
			return this_component()->template Print_Utility<ComponentType,CallerType,TargetType>();
		}
	};
}

}