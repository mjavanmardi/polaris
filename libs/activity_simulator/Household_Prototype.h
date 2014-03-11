#pragma once


#include "Activity_Simulator_Includes.h"


namespace Household_Components
{

namespace Concepts
{
	concept struct Is_Household
	{
		check_template_method_name(Has_Initialize_Defined,Initialize);
		check_template_method_name(Has_Properties_Defined,Properties);
		check_template_method_name(Has_Planner_Defined,Planning_Faculty);
		define_default_check(Has_Initialize_Defined && Has_Properties_Defined && Has_Planner_Defined);
	};

	concept struct Has_Initialize
	{
		check_template_method_name(Has_Initialize_Defined,Initialize);
		define_default_check(Has_Initialize_Defined);
	};

}

namespace Types
{

}

namespace Prototypes
{
	prototype struct Household ADD_DEBUG_INFO
	{
		tag_as_prototype;


		template<typename TargetType> void Initialize(TargetType id, requires(TargetType,check(ComponentType,Concepts::Has_Initialize)))
		{
			this_component()->template Initialize< TargetType>(id);	
			this->template Set_Home_Location<NT>();
		}
		template<typename TargetType> void Initialize(TargetType id, requires(TargetType,!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}

		template<typename IdType, typename SynthesisZoneType, typename NetworkRefType, typename ScenarioRefType> void Initialize(IdType id, SynthesisZoneType home_zone, NetworkRefType network_ref, ScenarioRefType scenario_ref,requires(ScenarioRefType,check(ComponentType,Concepts::Has_Initialize)))
		{
			this_component()->template Initialize< IdType, SynthesisZoneType, NetworkRefType, ScenarioRefType>(id, home_zone, network_ref, scenario_ref);		
			this->template Set_Home_Location<NT>();
		}
		template<typename IdType, typename SynthesisZoneType, typename NetworkRefType, typename ScenarioRefType> void Initialize(IdType id, SynthesisZoneType home_zone, NetworkRefType network_ref, ScenarioRefType scenario_ref,requires(ScenarioRefType,!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}

		// Sub-component accessors	
		template<typename TargetType> void Set_Home_Location()
		{
			// set the home locations on event
			this_component()->template Set_Home_Location< TargetType>();
		}
		accessor(Properties, NONE, NONE);
		accessor(Static_Properties, NONE, NONE);
		
		// External knowledge accessor
		accessor(network_reference, NONE, NONE);
		accessor(scenario_reference, NONE, NONE);

		// Basic property members
		accessor(uuid, NONE, NONE);
		accessor(internal_id, NONE, NONE);

		accessor(Persons_Container, NONE, NONE);
		

		// Accessors for setting the home/work locations (stores only an index into the network_reference::activity_locations_container) - overloaded to return either th loc_index, the location interface or the zone interface
		template<typename TargetType> TargetType Home_Location(requires(TargetType,check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType,is_pointer)))
		{
			typedef Household_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_container_itf)>  activity_location_itf;
			
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();
			
			int loc_id = properties->template home_location_id<int>();
			return (TargetType)(*locations)[loc_id];						
		}
		template<typename TargetType> TargetType Home_Location(requires(TargetType,check(strip_modifiers(TargetType), Zone_Components::Concepts::Is_Zone) && check(TargetType,is_pointer)))
		{
			typedef Household_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;
			typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_container_itf)>  activity_location_itf;

			
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			int loc_id = properties->template home_location_id<int>();
			activity_location_itf* loc = (*locations)[loc_id];	
			return loc->template zone<TargetType>();
		}
		template<typename TargetType> TargetType Home_Location(requires(TargetType,check(strip_modifiers(TargetType), is_integral)))
		{
			typedef Household_Properties<typename get_type_of(Properties)> properties_itf;
			properties_itf* properties = this->Properties<properties_itf*>();
			return properties->template home_location_id<TargetType>();
		}
		template<typename TargetType> TargetType Home_Location(requires(TargetType,check(strip_modifiers(TargetType), !is_integral) && ( (check(strip_modifiers(TargetType),!Activity_Location_Components::Concepts::Is_Activity_Location) && check(strip_modifiers(TargetType),!Zone_Components::Concepts::Is_Zone) ) || check(TargetType,!is_pointer) ) ) )
		{
			assert_check(strip_modifiers(TargetType),is_integral,"Error, Home_Location can only be requested as an Integral type - which returns location index, or as an Activity_Location refernence type, which returns the actual location.");
		}
		template<typename TargetType> void Home_Location(TargetType location_index, requires(TargetType,check(strip_modifiers(TargetType), is_integral)))
		{
			typedef Household_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;
			typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_container_itf)>  activity_location_itf;

			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			if (location_index < 0 || location_index >= locations->size()) THROW_EXCEPTION("Error: location index "<<location_index<<" does not exist in network locations container.  Index out of range (0,"<<locations->size()<<").");
			properties->template home_location_id<TargetType>(location_index);
		}
		template<typename TargetType> void Home_Location(TargetType location_index, requires(TargetType,check(strip_modifiers(TargetType), !is_integral)))
		{
			assert_check(strip_modifiers(TargetType), is_integral, "Error, Home_Location can only be set by passing an integral index from network::activity_locations_container");
		}
	
		template<typename TargetType> string To_String()
		{
			typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Person_Properties<typename get_type_of(Static_Properties)> static_properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;

			typedef Pair_Associative_Container< typename network_itf::get_type_of(zones_container)> zone_container_itf;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(zone_container_itf)>  zone_itf;
			
			typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> locations_container_interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(locations_container_interface)>  location_interface;


			properties_itf* props = this->Properties<properties_itf*>();
			static_properties_itf* static_props = this->Static_Properties<static_properties_itf*>();

			stringstream s;
			s << this->uuid<int>() << ", HOME_ZONE,";
			s << this->Home_Location<location_interface*>()->template zone<zone_itf*>()->template uuid<int>();
			s << ", HOME_LOC,";
			s << this->Home_Location<location_interface*>()->template uuid<int>();
			
			return s.str();
		}

	};

}

}
