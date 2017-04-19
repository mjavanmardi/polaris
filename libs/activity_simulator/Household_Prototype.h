#pragma once


#include "Activity_Simulator_Includes.h"


namespace Household_Components
{

namespace Concepts
{
	concept struct Is_Household
	{
		check_accessor_name(Has_Initialize_Defined,Initialize);
		check_accessor_name(Has_Properties_Defined,Properties);
		check_accessor_name(Has_Planner_Defined,Planning_Faculty);
		define_default_check(Has_Initialize_Defined && Has_Properties_Defined && Has_Planner_Defined);
	};

	concept struct Has_Initialize
	{
		//%%%RLW TODO: this check fails
		static const bool value = true;
		//check_accessor_name(Has_Initialize_Defined,Initialize);
		//define_default_check(Has_Initialize_Defined);
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

		// Event handling
		static void Agent_Event_Controller(ComponentType* _this, Event_Response& response);

		template<typename TargetType, requires(TargetType, check(ComponentType, Concepts::Has_Initialize))> 
		void Initialize(TargetType id)
		{
			this_component()->template Initialize< TargetType>(id);	
			//this->template Set_Home_Location<NT>();
		}
		template<typename TargetType, requires(TargetType, !check(ComponentType, Concepts::Has_Initialize))> 
		void Initialize(TargetType id)
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}

		template<typename IdType, typename NetworkRefType, typename ScenarioRefType,requires(ScenarioRefType,check(ComponentType,Concepts::Has_Initialize))>
		void Initialize(IdType id, NetworkRefType network_ref, ScenarioRefType scenario_ref)
		{
			this_component()->template Initialize< IdType, NetworkRefType, ScenarioRefType>(id, network_ref, scenario_ref);		
		}
		template<typename IdType, typename NetworkRefType, typename ScenarioRefType,requires(ScenarioRefType,!check(ComponentType,Concepts::Has_Initialize))>
		void Initialize(IdType id, NetworkRefType network_ref, ScenarioRefType scenario_ref)
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}
		template<typename IdType, typename SynthesisZoneType, typename NetworkRefType, typename ScenarioRefType, requires(ScenarioRefType, check(ComponentType, Concepts::Has_Initialize))> 
		void Initialize(IdType id, SynthesisZoneType home_zone, NetworkRefType network_ref, ScenarioRefType scenario_ref)		
		{
			// call object initialization for all subcomponents
			this_component()->template Initialize< IdType, SynthesisZoneType, NetworkRefType, ScenarioRefType>(id, home_zone, network_ref, scenario_ref);	

			// select a home activity location from the synthesis zone
			this->template Set_Home_Location<NT>();

			// Load the vehicle selection event
			((ComponentType*)this)->template Load_Event<ComponentType>(&Agent_Event_Controller, iteration() + 3, 0);
		}
		template<typename IdType, typename SynthesisZoneType, typename NetworkRefType, typename ScenarioRefType, requires(ScenarioRefType, !check(ComponentType, Concepts::Has_Initialize))> 
		void Initialize(IdType id, SynthesisZoneType home_zone, NetworkRefType network_ref, ScenarioRefType scenario_ref)
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
		accessor(home_synthesis_zone, NONE, NONE);

		// Basic property members
		accessor(uuid, NONE, NONE);
		accessor(internal_id, NONE, NONE);

		accessor(Persons_Container, NONE, NONE);
		accessor(Vehicles_Container, NONE, NONE);

		int Number_of_Children()
		{
			typedef Random_Access_Sequence< typename get_type_of(Persons_Container)> person_container_itf;
			typedef Person_Components::Prototypes::Person<get_component_type(person_container_itf)>  person_itf;
			typedef Person_Components::Prototypes::Person_Properties<typename person_itf::get_type_of(Static_Properties)> properties_itf;

			typename person_container_itf::iterator p_itr;
			person_container_itf* persons = this->Persons_Container<person_container_itf*>();

			int children = 0;

			for (p_itr = persons->begin(); p_itr != persons->end(); ++p_itr)
			{
				person_itf* p = static_cast<person_itf*>(*p_itr);
				properties_itf* props = p->Static_Properties<properties_itf*>();

				if (props->Age<int>() < 18) children += 1;
			}
			return children;
		}

		template<typename PersonItfType, typename TimeType> PersonItfType Get_Free_Member(TimeType start_time, TimeType end_time, requires(PersonItfType,check(PersonItfType,is_pointer) && check_stripped_type(PersonItfType,Activity_Simulator::Person_Concepts::Is_Person)))
		{
			typedef Household_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;

			typedef Random_Access_Sequence< typename get_type_of(Persons_Container)> person_container_itf;
			typedef Person_Components::Prototypes::Person<get_component_type(person_container_itf)>  person_itf;

			typename person_container_itf::iterator p_itr;
			person_container_itf* persons = this->Persons_Container<person_container_itf*>();

			for (p_itr = persons->begin(); p_itr != persons->end(); ++p_itr)
			{
				person_itf* p = (person_itf*)(*p_itr);
				if (p->Is_Free(start_time,end_time)) return (PersonItfType)p;
			}
			return nullptr;
		}
		
		//%%%RLW - fix requires part
		//template<typename PersonItfType, typename TimeType, requires(PersonItfType, check(PersonItfType, std::is_pointer) && check_stripped_type(PersonItfType, Activity_Simulator::Person_Concepts::Is_Person))> PersonItfType Get_Free_Escort(TimeType start_time, TimeType end_time)
		template<typename PersonItfType, typename TimeType> PersonItfType Get_Free_Escort(TimeType start_time, TimeType end_time)
		{
			typedef Household_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;

			typedef Random_Access_Sequence< typename get_type_of(Persons_Container)> person_container_itf;
			typedef Person_Components::Prototypes::Person<get_component_type(person_container_itf)>  person_itf;

			typedef Person_Components::Prototypes::Person_Properties<typename person_itf::get_type_of(Static_Properties)>  person_properties_itf;

			typename person_container_itf::iterator p_itr;
			person_container_itf* persons = this->Persons_Container<person_container_itf*>();

			for (p_itr = persons->begin(); p_itr != persons->end(); ++p_itr)
			{
				person_itf* p = (person_itf*)(*p_itr);
				if (p->Is_Free(start_time,end_time) && p->template Static_Properties<person_properties_itf*>()->template Age<int>()>=16) return (PersonItfType)p;
			}
			return nullptr;
		}
		
		template<typename PersonItfType> PersonItfType Get_Primary_Driver(){ return this_component()->Get_Primary_Driver<PersonItfType>(); }

		template<typename VehicleItfType> VehicleItfType Get_Free_Vehicle(requires(VehicleItfType, check(VehicleItfType, is_pointer)))// && check_stripped_type(PersonItfType, Activity_Simulator::Person_Concepts::Is_Person)))
		{
			return this_component()->Get_Free_Vehicle<VehicleItfType>();
		}

		template<typename TimeType> TimeType Get_Next_Available_Vehicle_Time()
		{
			return this_component()->Get_Next_Available_Vehicle_Time<TimeType>();
		}

		// Accessors for setting the home/work locations (stores only an index into the network_reference::activity_locations_container) - overloaded to return either th loc_index, the location interface or the zone interface
		template<typename TargetType> TargetType Home_Location(requires(TargetType,check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType,is_pointer)))
		{
			typedef Household_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(activity_locations_container_itf)>  activity_location_itf;
			
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
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(activity_locations_container_itf)>  activity_location_itf;

			
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			int loc_id = properties->template home_location_id<int>();
			activity_location_itf* loc = (activity_location_itf*)((*locations)[loc_id]);	
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
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(activity_locations_container_itf)>  activity_location_itf;

			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			if (location_index < 0 || location_index >= (TargetType)locations->size()) THROW_EXCEPTION("Error: location index "<<location_index<<" does not exist in network locations container.  Index out of range (0,"<<locations->size()<<").");
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
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zone_container_itf)>  zone_itf;
			
			typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> locations_container_interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(locations_container_interface)>  location_interface;


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

	// Event handling - Currently only does vehicle selection
	template<typename ComponentType>
	void Household<ComponentType>::Agent_Event_Controller(ComponentType* _this, Event_Response& response)
	{
		typedef Household<ComponentType> _Household_Interface;
		typedef Household_Components::Prototypes::Vehicle_Chooser<get_type_of(Vehicle_Chooser)> vehicle_chooser_interface;
		typedef PopSyn::Prototypes::Synthesis_Zone<get_type_of(home_synthesis_zone)> zone_interface;

		// Do choose vehicle routine - must occur after persons are intialized and all locations set
		_Household_Interface* pthis = (_Household_Interface*)_this;
		pthis->Vehicle_Chooser<vehicle_chooser_interface*>()->Select_Vehicles(pthis->home_synthesis_zone<zone_interface*>());

		response.next._iteration = END;
		response.next._sub_iteration = 0;
	}

}

}
