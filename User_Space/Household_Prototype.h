#pragma once


//#include "Activity_Prototype.h"
//#include "Network_Event_Prototype.h"
//#include "Activity_Location_Prototype.h"
#include "Network_Prototype.h"
#include "Zone_Prototype.h"
#include "Household_Properties_Prototype.h"


namespace Household_Components
{

namespace Concepts
{
	concept struct Is_Household
	{
		check_feature(Has_Initialize_Defined,Initialize);
		check_getter(Has_Properties_Defined,Properties);
		check_getter(Has_Planner_Defined,Planning_Faculty);
		define_default_check(Has_Initialize_Defined && Has_Properties_Defined && Has_Planner_Defined);
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
	prototype struct Household ADD_DEBUG_INFO
	{
		tag_as_prototype;


		feature_prototype void Initialize(TargetType id, requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			this_component()->template Initialize<ComponentType,CallerType, TargetType>(id);	
			this->template Set_Home_Location<NT>();
		}
		feature_prototype void Initialize(TargetType id, requires(!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}

		feature_prototype void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type home_zone, typename TargetType::Param3Type network_ref, typename TargetType::Param4Type scenario_ref,requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			this_component()->template Initialize<ComponentType,CallerType, TargetType>(id, home_zone, network_ref, scenario_ref);		
			this->template Set_Home_Location<NT>();
		}

		// Sub-component accessors	
		feature_prototype void Set_Home_Location()
		{
			// set the home locations on event
			this_component()->template Set_Home_Location<ComponentType,CallerType, TargetType>();
		}
		feature_accessor(Properties,none,none);
		feature_accessor(Static_Properties,none,none);
		
		// External knowledge accessor
		feature_accessor(network_reference,none,none);
		feature_accessor(scenario_reference,none,none);

		// Basic property members
		feature_accessor(uuid,none,none);
		feature_accessor(internal_id,none,none);

		feature_accessor(Persons_Container,none,none);
		

		// Accessors for setting the home/work locations (stores only an index into the network_reference::activity_locations_container) - overloaded to return either th loc_index, the location interface or the zone interface
		feature_prototype TargetType Home_Location(requires(check(TargetType, Activity_Location_Components::Concepts::Is_Activity_Location) && check_as_given(TargetType,is_pointer)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Household_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);

			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();
			
			int loc_id = properties->template home_location_id<int>();
			return (TargetType)(*locations)[loc_id];						
		}
		feature_prototype TargetType Home_Location(requires(check(TargetType, Zone_Components::Concepts::Is_Zone) && check_as_given(TargetType,is_pointer)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Household_Properties,ComponentType);
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
			define_component_interface(properties_itf,typename get_type_of(Properties),Household_Properties,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			return properties->template home_location_id<TargetType>();
		}
		feature_prototype TargetType Home_Location(requires(check(TargetType, !is_integral) && ( (check(TargetType,!Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType,!Zone_Components::Concepts::Is_Zone) ) || check_as_given(TargetType,!is_pointer) ) ) )
		{
			assert_check(TargetType,is_integral,"Error, Home_Location can only be requested as an Integral type - which returns location index, or as an Activity_Location refernence type, which returns the actual location.");
		}
		feature_prototype void Home_Location(TargetType location_index, requires(check(TargetType, is_integral)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Household_Properties,ComponentType);
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
	
		feature_prototype string To_String()
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(static_properties_itf,typename get_type_of(Static_Properties), Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(zone_container_itf, zone_itf, typename network_itf::get_type_of(zones_container), Containers::Random_Access_Sequence_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
			define_container_and_value_interface(locations_container_interface, location_interface, typename network_itf::get_type_of(activity_locations_container),Containers::Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);

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