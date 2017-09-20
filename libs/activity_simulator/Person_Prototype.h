#pragma once

#include "Activity_Simulator_Includes.h"
#include "Activity_Prototype.h"



//#include "Network_Event_Prototype.h"
//#include "Activity_Location_Prototype.h"
//#include "Network_Prototype.h"
//#include "Zone_Prototype.h"
//#include "Movement_Plan_Prototype.h"
//#include "Network_Skimming_Prototype.h"
//#include "Person_Properties_Prototype.h"
//#include "Person_Planner_Prototype.h"
//#include "Person_Scheduler_Prototype.h"
//#include "Person_Perception_Prototype.h"
//#include "Person_Mover_Prototype.h"
//#include "Destination_Chooser_Prototype.h"

namespace Person_Components
{

namespace Concepts
{
	concept struct Is_Person
	{
		check_accessor_name(Has_Initialize_Defined,Initialize);
		check_accessor_name(Has_Properties_Defined,Properties);
		check_accessor_name(Has_Planner_Defined,Planning_Faculty);
		define_default_check(Has_Initialize_Defined && Has_Properties_Defined && Has_Planner_Defined);
	};

	concept struct Is_Transims_Person
	{
		check_concept(Is_Person_Check, Is_Person, T, V);
		check_typedef_type(Is_Transims_Check,Is_Transims,true_type);
		define_default_check(Is_Person_Check && Is_Transims_Check);
	};
	concept struct Is_CTRAMP_Person
	{
		check_concept(Is_Person_Check, Is_Person, T, V);
		check_typedef_type(Is_CTRAMP_Check,Is_Transims,true_type);
		define_default_check(Is_Person_Check && Is_CTRAMP_Check);
	};

	concept struct Has_Initialize
	{
		//%%RLW TODO: this check fails
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
	prototype struct Person ADD_DEBUG_INFO
	{
		tag_as_prototype;

		// Event handling
		static void Agent_Event_Controller(ComponentType* _this, Event_Response& response);
		template<typename TargetType> void Set_Locations_Event();
		
		template<typename T>
		void Print_Preplanned_Activities_Event();

		//Initializers
		template<typename TargetType, requires(TargetType, check(ComponentType, Concepts::Has_Initialize) && check_2(typename ComponentType::Object_Type, Execution_Object, is_same))>
		void Initialize(TargetType id)
		{
			this->First_Iteration<Simulation_Timestep_Increment>(iteration() + 1);

			int starting_subiteration = (int)(GLOBALS::Uniform_RNG.Next_Rand<float>()*30.0);

			this_component()->template Initialize<TargetType>(id);

			//load_event(ComponentType,Agent_Conditional,Set_Locations_Event,this->First_Iteration<Simulation_Timestep_Increment>(),starting_subiteration,NULLTYPE);
			((ComponentType*)this)->template Load_Event<ComponentType>(&Agent_Event_Controller, this->First_Iteration<Simulation_Timestep_Increment>(), starting_subiteration);
		}

		template<typename TargetType, requires(TargetType, check(ComponentType, Concepts::Has_Initialize) && check_2(typename ComponentType::Object_Type, Data_Object, is_same))>
		void Initialize(TargetType id)
		{
			this_component()->template Initialize<TargetType>(id);
		}

		template<typename TargetType, requires(TargetType, !check(ComponentType, Concepts::Has_Initialize) || (check_2(typename ComponentType::Object_Type, Execution_Object, is_same) && check_2(typename ComponentType::Object_Type, Data_Object, is_same)))>
		void Initialize(TargetType id)
		{
			assert_check(ComponentType, Concepts::Has_Initialize, "This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
			assert_check_2(typename ComponentType::Object_Type, Execution_Object, is_same, "ComponentType must be an execution object, or ");
			assert_check_2(typename ComponentType::Object_Type, Data_Object, is_same, "ComponentType must be an data object.");
		}

		template<typename IdType, typename SynthesisZoneType, typename NetworkRefType, typename ScenarioRefType, requires(IdType, check(ComponentType, Concepts::Has_Initialize) && check_2(typename ComponentType::Object_Type, Execution_Object, is_same))>
		void Initialize(IdType id, SynthesisZoneType home_zone, NetworkRefType network_ref, ScenarioRefType scenario_ref)
		{
			this->First_Iteration<Simulation_Timestep_Increment>(iteration() + 1);
			int starting_subiteration = (int)(GLOBALS::Uniform_RNG.Next_Rand<float>()*30.0);

			this_component()->template Initialize< IdType, SynthesisZoneType, NetworkRefType, ScenarioRefType>(id, home_zone, network_ref, scenario_ref);

			((ComponentType*)this)->template Load_Event<ComponentType>(&Agent_Event_Controller, this->First_Iteration<Simulation_Timestep_Increment>(), starting_subiteration);
		}

		template<typename IdType, typename SynthesisZoneType, typename NetworkRefType, typename ScenarioRefType, requires(IdType, check(ComponentType, Concepts::Has_Initialize) && check_2(typename ComponentType::Object_Type, Data_Object, is_same))>
		void Initialize(IdType id, SynthesisZoneType home_zone, NetworkRefType network_ref, ScenarioRefType scenario_ref)
		{
			this_component()->template Initialize< IdType, SynthesisZoneType, NetworkRefType, ScenarioRefType>(id, home_zone, network_ref, scenario_ref);
		}

		template<typename IdType, typename SynthesisZoneType, typename NetworkRefType, typename ScenarioRefType, requires(IdType, !check(ComponentType, Concepts::Has_Initialize) || (!check_2(typename ComponentType::Object_Type, Execution_Object, is_same) && !check_2(typename ComponentType::Object_Type, Data_Object, is_same)))>
		void Initialize(IdType id, SynthesisZoneType home_zone, NetworkRefType network_ref, ScenarioRefType scenario_ref)
		{
			assert_check(ComponentType, Concepts::Has_Initialize, "This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
			assert_check_2(typename ComponentType::Object_Type, Execution_Object, is_same, "ComponentType must be an execution object, or ");
			assert_check_2(typename ComponentType::Object_Type, Data_Object, is_same, "ComponentType must be an data object.");
		}

		// Sub-component accessors	
		accessor(Household, NONE, NONE);
		accessor(Planning_Faculty, NONE, NONE);
		accessor(Perception_Faculty, NONE, NONE);
		accessor(Scheduling_Faculty, NONE, NONE);
		accessor(router, NONE, NONE);
		accessor(Moving_Faculty, NONE, NONE);
		accessor(Properties, NONE, NONE);
		accessor(Static_Properties, NONE, NONE);
		accessor(vehicle, NONE, NONE);
		accessor(has_done_replanning, NONE, NONE);
		
		accessor(has_pretrip_information, NONE, NONE);

		// External knowledge accessor
		accessor(network_reference, NONE, NONE);
		accessor(scenario_reference, NONE, NONE);


		// Basic property members
		accessor(uuid, NONE, NONE);
		accessor(internal_id, NONE, NONE);
		accessor(Next_Rand, NONE, NONE);
		accessor(First_Iteration, NONE, NONE);
		accessor(Write_Activity_Files, NONE, NONE);
		accessor(Activity_Record_Container, NONE, NONE);
		accessor(current_location, NONE, NONE);

		// Database record for this person
		accessor(person_record, NONE, NONE);

		void Arrive_At_Destination();

		// Accessors for setting the home/work locations (stores only an index into the network_reference::activity_locations_container) - overloaded to return either th loc_index, the location interface or the zone interface
		template<typename TargetType> TargetType Home_Location();

		template<typename TargetType> TargetType Work_Location(requires(TargetType, check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType, is_pointer)));
		template<typename TargetType> TargetType Work_Location(requires(TargetType, check(strip_modifiers(TargetType), Zone_Components::Concepts::Is_Zone) && check(TargetType, is_pointer)));
		template<typename TargetType> TargetType Work_Location(requires(TargetType, check(strip_modifiers(TargetType), is_integral)));
		template<typename TargetType> TargetType Work_Location(requires(TargetType, check(strip_modifiers(TargetType), !is_integral) && ((check(strip_modifiers(TargetType), !Activity_Location_Components::Concepts::Is_Activity_Location) && check(strip_modifiers(TargetType), !Zone_Components::Concepts::Is_Zone)) || check(TargetType, !is_pointer))));
		template<typename TargetType> void Work_Location(TargetType location, requires(TargetType, check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType, is_pointer)));
		template<typename TargetType> void Work_Location(TargetType location_index, requires(TargetType, check(strip_modifiers(TargetType), is_integral)));
		template<typename TargetType> void Work_Location(TargetType location_index, requires(TargetType, !check(strip_modifiers(TargetType), is_integral) && (!check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) || !check(TargetType, is_pointer))));

		template<typename TargetType> TargetType School_Location(requires(TargetType, check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType, is_pointer)));
		template<typename TargetType> TargetType School_Location(requires(TargetType, check(strip_modifiers(TargetType), Zone_Components::Concepts::Is_Zone) && check(TargetType, is_pointer)));
		template<typename TargetType> TargetType School_Location(requires(TargetType, check(strip_modifiers(TargetType), is_integral)));
		template<typename TargetType> TargetType School_Location(requires(TargetType, check(strip_modifiers(TargetType), !is_integral) && ((check(strip_modifiers(TargetType), !Activity_Location_Components::Concepts::Is_Activity_Location) && check(strip_modifiers(TargetType), !Zone_Components::Concepts::Is_Zone)) || check(TargetType, !is_pointer))));
		template<typename TargetType> void School_Location(TargetType location_index, requires(TargetType, check(strip_modifiers(TargetType), is_integral)));
		template<typename TargetType> void School_Location(TargetType location_index, requires(TargetType, check(strip_modifiers(TargetType), !is_integral)));
		

		template<typename TargetType> string To_String();
		template<typename TargetType> void Sort_Activity_Records();

		// Determine if the person is free at a given time
		template<typename TimeType> bool Is_Free(TimeType start, TimeType end);

		// Print current state of activity schedule
		void Display_Activities(ostream& out);

		// PASS-THROUGH FEATURES OF SUB-COMPONENTS
		template<typename TargetType> TargetType current_movement_plan();
		template<typename TargetType> TargetType current_activity_plan();
		template<typename ParamType, typename ReturnType> ReturnType next_activity_plan(ParamType current_time_or_activity);
		template<typename ParamType, typename ReturnType> ReturnType previous_activity_plan(ParamType current_time);
		template<typename TargetType> void Add_Movement_Plan(TargetType movement_plan);
		template<typename TargetType> void Add_Activity_Plan(TargetType activity_plan);

		template<typename TimeType> TimeType Get_Estimated_Return_Home_Time();
		bool Is_Moving();
		void Leave_Vehicle();
	};

	// Event handling
	template<typename ComponentType>
	/*static*/ void Person<ComponentType>::Agent_Event_Controller(ComponentType* _this, Event_Response& response)
	{
		typedef Person_Planner<typename get_type_of(Planning_Faculty)> planner_itf;
		typedef Person<ComponentType> _Person_Interface;
		ComponentType* _pthis = (ComponentType*)_this;
		_Person_Interface* pthis = (_Person_Interface*)_pthis;
		planner_itf* planner = pthis->template Planning_Faculty<planner_itf*>();


		// First do the 'Set Locations Event', 
		if (iteration() == pthis->template First_Iteration<Simulation_Timestep_Increment>())
		{
			Simulation_Timestep_Increment first_plan_time = planner->template Next_Planning_Time<Simulation_Timestep_Increment>() + planner->template Planning_Time_Increment<Simulation_Timestep_Increment>();
			response.next._iteration = Round<int, Basic_Units::Time_Value_Type>(first_plan_time);
			response.next._sub_iteration = 0;
			pthis->Set_Locations_Event<NT>();
		}
		// then, prior to the first planning period, dump all preplanned activities to file
		else
		{
			//_pthis->Swap_Event((Event)&Person::Print_Preplanned_Activities_Event<NULLTYPE>);
			response.next._iteration = END;
			response.next._sub_iteration = 0;
			pthis->Print_Preplanned_Activities_Event<NT>();
		}
	}

	template<typename ComponentType>
	template<typename TargetType>
	void Person<ComponentType>::Set_Locations_Event()
	{
		typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_type_of(current_location)> location_itf;

		// set the home/workplace/school locations on event
		this_component()->template Set_Locations< TargetType>();

		// start the agent off at home
		this->current_location<location_itf*>(this->Home_Location<location_itf*>());
	}

	template<typename ComponentType>
	template<typename T>
	void Person<ComponentType>::Print_Preplanned_Activities_Event()
	{
		//%%%RLW
		//this_component()->template Print_Preplanned_Activities_Event();
	}

	template<typename ComponentType>
	void Person<ComponentType>::Arrive_At_Destination()
	{
		typedef Person_Mover< typename get_type_of(Moving_Faculty)> mover_itf;
		mover_itf* mover = this->Moving_Faculty<mover_itf*>();
		mover->Arrive_At_Destination();
	}

	// Accessors for setting the home/work locations (stores only an index into the network_reference::activity_locations_container) - overloaded to return either th loc_index, the location interface or the zone interface
	template<typename ComponentType>
	template<typename TargetType>
	TargetType Person<ComponentType>::Home_Location()
	{
		typedef Household_Components::Prototypes::Household<typename get_type_of(Household)> household_itf;
		household_itf* household = this->Household<household_itf*>();
		return household->template Home_Location<TargetType>();
	}

	template<typename ComponentType>
	template<typename TargetType>
	TargetType Person<ComponentType>::Work_Location(requires(TargetType, check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType, is_pointer)))
	{
		typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
		typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;

		typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
		typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(activity_locations_container_itf)>  activity_location_itf;

		properties_itf* properties = this->Properties<properties_itf*>();
		network_itf* network = this->network_reference<network_itf*>();
		activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

		int loc_id = properties->template work_location_id<int>();
		if (loc_id < 0)
		{
			//THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' does not have a valid work location.  Should not be requesting this.");
			return nullptr;
		}
		return (TargetType)(*locations)[loc_id];
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	TargetType Person<ComponentType>::Work_Location(requires(TargetType, check(strip_modifiers(TargetType), Zone_Components::Concepts::Is_Zone) && check(TargetType, is_pointer)))
	{
		typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
		typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;

		typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
		typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(activity_locations_container_itf)>  activity_location_itf;


		properties_itf* properties = this->Properties<properties_itf*>();
		network_itf* network = this->network_reference<network_itf*>();
		activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

		int loc_id = properties->template work_location_id<int>();
		if (loc_id < 0 || loc_id >= locations->size())
		{
			//THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' does not have a valid work location.  Should not be requesting this.");
			return nullptr;
		}
		activity_location_itf* loc = (activity_location_itf*)((*locations)[loc_id]);
		TargetType z = loc->template zone<TargetType>();
		if (z == nullptr)
		{
			THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' work location is not associated with a Traffic Analysis Zone.  Location reset to home location");
			return nullptr;
		}
		return loc->template zone<TargetType>();
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	TargetType Person<ComponentType>::Work_Location(requires(TargetType, check(strip_modifiers(TargetType), is_integral)))
	{
		typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
		properties_itf* properties = this->Properties<properties_itf*>();
		return properties->template work_location_id<TargetType>();
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	TargetType Person<ComponentType>::Work_Location(requires(TargetType, check(strip_modifiers(TargetType), !is_integral) && ((check(strip_modifiers(TargetType), !Activity_Location_Components::Concepts::Is_Activity_Location) && check(strip_modifiers(TargetType), !Zone_Components::Concepts::Is_Zone)) || check(TargetType, !is_pointer))))
	{
		assert_check(strip_modifiers(TargetType), is_integral, "Error, Home_Location can only be requested as an Integral type - which returns location index, or as an Activity_Location refernence type, which returns the actual location.");
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	void Person<ComponentType>::Work_Location(TargetType location, requires(TargetType, check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType, is_pointer)))
	{
		typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
		typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;

		typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
		typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(activity_locations_container_itf)>  activity_location_itf;

		properties_itf* properties = this->Properties<properties_itf*>();
		network_itf* network = this->network_reference<network_itf*>();
		activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();
		int location_index = location->internal_id<int>();
		properties->template work_location_id<int>(location_index);
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	void Person<ComponentType>::Work_Location(TargetType location_index, requires(TargetType, check(strip_modifiers(TargetType), is_integral)))
	{
		typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
		typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;

		typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
		typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(activity_locations_container_itf)>  activity_location_itf;

		properties_itf* properties = this->Properties<properties_itf*>();
		network_itf* network = this->network_reference<network_itf*>();
		activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

		if (location_index < -1 || location_index >= (signed int)locations->size()) THROW_EXCEPTION("Error: location index " << location_index << " does not exist in network locations container.");
		properties->template work_location_id<TargetType>(location_index);
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	void Person<ComponentType>::Work_Location(TargetType location_index, requires(TargetType, !check(strip_modifiers(TargetType), is_integral) && (!check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) || !check(TargetType, is_pointer))))
	{
		assert_check(strip_modifiers(TargetType), is_integral, "Error, work location can only be set by passing an integral index from network::activity_locations_container");
		assert_check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location, "Error, work location can only be set by passing an activity_location prototype");
		assert_check(TargetType, is_pointer, "Error, work location can only be set by passing a pointer to an activity location prototype, target type is not a pointer type.");
	}

	template<typename ComponentType>
	template<typename TargetType>
	TargetType Person<ComponentType>::School_Location(requires(TargetType, check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType, is_pointer)))
	{
		typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
		typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;

		typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
		typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(activity_locations_container_itf)>  activity_location_itf;

		properties_itf* properties = this->Properties<properties_itf*>();
		network_itf* network = this->network_reference<network_itf*>();
		activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

		int loc_id = properties->template school_location_id<int>();
		if (loc_id < 0)
		{
			//THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' does not have a valid school location.  Should not be requesting this.");
			return nullptr;
		}
		return (TargetType)(*locations)[loc_id];
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	TargetType Person<ComponentType>::School_Location(requires(TargetType, check(strip_modifiers(TargetType), Zone_Components::Concepts::Is_Zone) && check(TargetType, is_pointer)))
	{
		typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
		typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;
		typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
		typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(activity_locations_container_itf)>  activity_location_itf;


		properties_itf* properties = this->Properties<properties_itf*>();
		network_itf* network = this->network_reference<network_itf*>();
		activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

		int loc_id = properties->template school_location_id<int>();
		if (loc_id < 0)
		{
			//THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' does not have a valid school location.  Should not be requesting this.");
			return nullptr;
		}
		activity_location_itf* loc = (*locations)[loc_id];
		return loc->template zone<TargetType>();
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	TargetType Person<ComponentType>::School_Location(requires(TargetType, check(strip_modifiers(TargetType), is_integral)))
	{
		typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
		properties_itf* properties = this->Properties<properties_itf*>();
		return properties->template school_location_id<TargetType>();
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	TargetType Person<ComponentType>::School_Location(requires(TargetType, check(strip_modifiers(TargetType), !is_integral) && ((check(strip_modifiers(TargetType), !Activity_Location_Components::Concepts::Is_Activity_Location) && check(strip_modifiers(TargetType), !Zone_Components::Concepts::Is_Zone)) || check(TargetType, !is_pointer))))
	{
		assert_check(strip_modifiers(TargetType), is_integral, "Error, School_Location can only be requested as an Integral type - which returns location index, or as an Activity_Location refernence type, which returns the actual location.");
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	void Person<ComponentType>::School_Location(TargetType location_index, requires(TargetType, check(strip_modifiers(TargetType), is_integral)))
	{
		typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
		typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;
		typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
		typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(activity_locations_container_itf)>  activity_location_itf;

		properties_itf* properties = this->Properties<properties_itf*>();
		network_itf* network = this->network_reference<network_itf*>();
		activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

		if (location_index < -1 || location_index >= (signed int)locations->size()) THROW_EXCEPTION("Error: location index " << location_index << " does not exist in network locations container.");
		properties->template school_location_id<TargetType>(location_index);
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	void Person<ComponentType>::School_Location(TargetType location_index, requires(TargetType, check(strip_modifiers(TargetType), !is_integral)))
	{
		assert_check(strip_modifiers(TargetType), is_integral, "Error, work location can only be set by passing an integral index from network::activity_locations_container");
	}

	template<typename ComponentType>
	template<typename TargetType>
	string Person<ComponentType>::To_String()
	{
		typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
		typedef Person_Properties<typename get_type_of(Static_Properties)> static_properties_itf;
		typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;

		typedef Pair_Associative_Container< typename network_itf::get_type_of(zones_container)> zone_container_itf;
		typedef Zone_Components::Prototypes::Zone<get_component_type(zone_container_itf)>  zone_itf;

		typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> locations_container_interface;
		typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(locations_container_interface)>  location_interface;


		properties_itf* props = this->Properties<properties_itf*>();
		static_properties_itf* static_props = this->Static_Properties<static_properties_itf*>();

		stringstream s;
		s << this->uuid<int>() << ", HOME_ZONE,";
		s << this->Home_Location<location_interface*>()->template zone<zone_itf*>()->template uuid<int>();
		s << ", HOME_LOC,";
		s << this->Home_Location<location_interface*>()->template uuid<int>();
		if (static_props->template Is_Employed<bool>())
		{
			//cout << endl << "Work location id: " << this->Work_Location<int>()<<endl;
			s << ", WORK_ZONE," << this->Work_Location<zone_itf*>()->template uuid<int>() << ", WORK_LOC," << this->Work_Location<location_interface*>()->template uuid<int>();
		}
		else
		{
			s << ", DOESNT_WORK,,,";
		}
		if (static_props->template Is_Student<bool>())
		{
			s << ", SCHOOL_ZONE," << this->School_Location<zone_itf*>()->template uuid<int>() << ", SCHOOL_LOC," << this->School_Location<location_interface*>()->template uuid<int>();
		}
		else
		{
			s << ", NOT_IN_SCHOOL,,,";
		}
		return s.str();
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	void Person<ComponentType>::Sort_Activity_Records()
	{
		this_component()->Sort_Activity_Records<TargetType>();
	}

	// Determine if the person is free at a given time
	template<typename ComponentType>
	template<typename TimeType>
	bool Person<ComponentType>::Is_Free(TimeType start, TimeType end)
	{
		typedef Prototypes::Person_Scheduler<typename get_type_of(Scheduling_Faculty)> scheduler_itf;
		typedef Back_Insertion_Sequence<typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
		typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Activity_Plan;
		//typedef std::list<Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type>*> Activity_Plans;
		//typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> Activity_Plan;

		Activity_Plan* prev_act = this->previous_activity_plan<TimeType, Activity_Plan*>(start);
		Activity_Plan* next_act = this->next_activity_plan<TimeType, Activity_Plan*>(start);

		TimeType prev_end = 0;
		TimeType next_start = END;
		if (prev_act != nullptr) prev_end = prev_act->template End_Time<TimeType>();
		if (next_act != nullptr) next_start = next_act->template Start_Time<TimeType>();

		if (prev_end < start && next_start > end) return true;
		else if (prev_end > start && next_start > end)
		{
			if (prev_act != nullptr)
			{
				if (prev_act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY) return true;
			}
		}
		else if (prev_end < start && next_start < end)
		{
			if (next_act != nullptr)
			{
				if (next_act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY) return true;
			}
		}
		return false;
	}

	// Print current state of activity schedule
	template<typename ComponentType>
	void Person<ComponentType>::Display_Activities(ostream& out)
	{
		typedef Prototypes::Person_Scheduler<typename get_type_of(Scheduling_Faculty)> scheduler_itf;
		typedef Back_Insertion_Sequence<typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
		typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Activity_Plan;
		//typedef std::list<Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type>*> Activity_Plans;
		//typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> Activity_Plan;
		typedef Back_Insertion_Sequence<typename scheduler_itf::get_type_of(Movement_Plans_Container)> Movement_Plans;
		typedef Movement_Plan_Components::Prototypes::Movement_Plan<get_component_type(Movement_Plans)> Movement_Plan;
		typedef Household_Components::Prototypes::Household<typename get_type_of(Household)> _household_itf;

		cout << endl << "**************************************************************************************************" << endl;
		cout << "Printing activities for (Household,person): " << this->Household<_household_itf*>()->uuid<int>() << "," << this->uuid<int>() << endl;

		scheduler_itf* scheduler = this->Scheduling_Faculty<scheduler_itf*>();
		scheduler->template Current_Activity<Activity_Plan*>()->Display_Activity();

		Activity_Plans& activities = this->Scheduling_Faculty<scheduler_itf*>()->template Activity_Container<Activity_Plans&>();
		for (auto itr = activities.begin(); itr != activities.end(); ++itr)
		{
			//Activity_Plan* act = (Activity_Plan*)(*itr);
			Activity_Plan* act = (*itr);
			act->Display_Activity();
		}

		Movement_Plans& moves = this->Scheduling_Faculty<scheduler_itf*>()->template Movement_Plans_Container<Movement_Plans&>();
		for (auto itr = moves.begin(); itr != moves.end(); ++itr)
		{
			Movement_Plan* move = (Movement_Plan*)(*itr);
			move->Display_Movement();
		}

	}

	// PASS-THROUGH FEATURES OF SUB-COMPONENTS
	template<typename ComponentType>
	template<typename TargetType>
	TargetType Person<ComponentType>::current_movement_plan()
	{
		typedef Prototypes::Person_Scheduler<typename get_type_of(Scheduling_Faculty)> scheduler_itf;
		scheduler_itf* scheduler = this->Scheduling_Faculty<scheduler_itf*>();
		return scheduler->template current_movement_plan<TargetType>();
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	TargetType Person<ComponentType>::current_activity_plan()
	{
		typedef Prototypes::Person_Scheduler<typename get_type_of(Scheduling_Faculty)> scheduler_itf;
		scheduler_itf* scheduler = this->Scheduling_Faculty<scheduler_itf*>();
		return scheduler->template current_activity_plan<TargetType>();
	}
	
	template<typename ComponentType>
	template<typename ParamType, typename ReturnType>
	ReturnType Person<ComponentType>::next_activity_plan(ParamType current_time_or_activity)
	{
		typedef Prototypes::Person_Scheduler<typename get_type_of(Scheduling_Faculty)> scheduler_itf;
		scheduler_itf* scheduler = this->Scheduling_Faculty<scheduler_itf*>();
		return scheduler->template next_activity_plan<ParamType, ReturnType>(current_time_or_activity);
	}
	
	template<typename ComponentType>
	template<typename ParamType, typename ReturnType>
	ReturnType Person<ComponentType>::previous_activity_plan(ParamType current_time)
	{
		typedef Prototypes::Person_Scheduler<typename get_type_of(Scheduling_Faculty)> scheduler_itf;
		scheduler_itf* scheduler = this->Scheduling_Faculty<scheduler_itf*>();
		return scheduler->template previous_activity_plan<ParamType, ReturnType>(current_time);
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	void Person<ComponentType>::Add_Movement_Plan(TargetType movement_plan)
	{
		this_component()->template Add_Movement_Plan<TargetType>(movement_plan);
	}
	
	template<typename ComponentType>
	template<typename TargetType>
	void Person<ComponentType>::Add_Activity_Plan(TargetType activity_plan)
	{
		this_component()->template Add_Activity_Plan<TargetType>(activity_plan);
	}

	template<typename ComponentType>
	template<typename TimeType>
	TimeType Person<ComponentType>::Get_Estimated_Return_Home_Time()
	{
		return this_component()->Get_Estimated_Return_Home_Time<TimeType>();
	}

	template<typename ComponentType>
	bool Person<ComponentType>::Is_Moving()
	{
		typedef Person_Components::Prototypes::Person_Mover<typename get_type_of(Moving_Faculty)> mover_itf;
		return this->Moving_Faculty<mover_itf*>()->Is_Moving();
	}
	template<typename ComponentType>
	void Person<ComponentType>::Leave_Vehicle()
	{
		this_component()->Leave_Vehicle();
	}
}

}
