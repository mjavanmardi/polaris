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
#include "Person_Mover_Prototype.h"

namespace Person_Components
{

namespace Concepts
{
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
	prototype struct Person
	{
		tag_as_prototype;

		// Event handling
		declare_feature_conditional(Agent_Conditional)
		{
			define_component_interface(planner_itf,typename get_type_of(Planning_Faculty),Person_Planner,ComponentType);
			typedef Person<ComponentType, ComponentType> _Person_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Interface* pthis =(_Person_Interface*)_pthis;
			planner_itf* planner = pthis->template Planning_Faculty<planner_itf*>();
			

			// First do the 'Set Locations Event', 
			if (_iteration == pthis->template First_Iteration<Simulation_Timestep_Increment>())
			{
				Simulation_Timestep_Increment first_plan_time = planner->template Next_Planning_Time<Simulation_Timestep_Increment>() + planner->template Planning_Time_Increment<Simulation_Timestep_Increment>();
				response.next._iteration = first_plan_time;
				response.next._sub_iteration = 0;
				response.result = true;
			}
			// then, prior to the first planning period, dump all preplanned activities to file
			else
			{	
				_pthis->Swap_Event((Event)&Person::Print_Preplanned_Activities_Event<NULLTYPE>);
				response.next._iteration = END;
				response.next._sub_iteration = 0;
				response.result = true;
			}
		}
		declare_feature_event(Set_Locations_Event)
		{
			typedef Person<ComponentType, ComponentType> _Person_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Interface* pthis =(_Person_Interface*)_pthis;
			pthis->template Set_Home_Location<NT>();
		}
		declare_feature_event(Print_Preplanned_Activities_Event)
		{
			typedef Person<ComponentType, ComponentType> _Person_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Interface* pthis =(_Person_Interface*)_pthis;

			define_component_interface(scheduler_itf,typename get_type_of(Scheduling_Faculty),Person_Scheduler,ComponentType);
			define_container_and_value_interface_unqualified_container(Activity_Records,Activity_Record, typename scheduler_itf::get_type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_component_interface(_Logger_Interface, typename MasterType::person_data_logger_type, Person_Components::Prototypes::Person_Data_Logger, NULLTYPE);	
			scheduler_itf* scheduler = pthis->template Scheduling_Faculty<scheduler_itf*>();
			Activity_Records* activities = scheduler->template Activity_Container<Activity_Records*>();


			for (typename Activity_Records::iterator itr = activities->begin(); itr != activities->end(); ++itr)
			{
				//cout << endl <<"Person ID: " << (*itr)->Parent_ID<int>() << "Activity Type: " << (*itr)->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
				((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity_Record*>(*itr,false);
			}
		}

		// Initializers
		feature_prototype void Initialize(TargetType id, requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			this->First_Iteration<Simulation_Timestep_Increment>(_iteration+1);
			this_component()->template Initialize<ComponentType,CallerType, TargetType>(id);	
			load_event(ComponentType,Agent_Conditional,Agent_Event,this->First_Iteration<Simulation_Timestep_Increment>(),0,NULLTYPE);
		}
		feature_prototype void Initialize(TargetType id, requires(!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}
		feature_prototype void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type home_zone, typename TargetType::Param3Type network_ref, typename TargetType::Param4Type scenario_ref/*,requires(check(ComponentType,Concepts::Has_Initialize))*/)
		{
			this->First_Iteration<Simulation_Timestep_Increment>(_iteration+1);
			this_component()->template Initialize<ComponentType,CallerType, TargetType>(id, home_zone, network_ref, scenario_ref);		
			load_event(ComponentType,Agent_Conditional,Set_Locations_Event,this->First_Iteration<Simulation_Timestep_Increment>(),0,NULLTYPE);
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
		feature_accessor(Scheduling_Faculty,none,none);
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
		feature_accessor(Activity_Record_Container,none,none);

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
			if (loc_id < 0 || loc_id >= locations->size())
			{
				THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' does not have a valid work location.  Should not be requesting this.");
				return nullptr;
			}
			activity_location_itf* loc = (*locations)[loc_id];	
			TargetType z = loc->template zone<TargetType>();
			if (z == nullptr)
			{
				THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' work location is not associated with a Traffic Analysis Zone.  Location reset to home location");
				return nullptr;
			}
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
				//cout << endl << "Work location id: " << this->Work_Location<int>()<<endl;
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



		// PASS-THROUGH FEATURES OF SUB-COMPONENTS
		feature_prototype TargetType current_movement_plan()
		{
			define_component_interface(scheduler_itf,typename get_type_of(Scheduling_Faculty),Prototypes::Person_Scheduler,ComponentType);
			scheduler_itf* scheduler = this->Scheduling_Faculty<scheduler_itf*>();
			return scheduler->template current_movement_plan<TargetType>();
		}
		feature_prototype TargetType current_activity_plan()
		{
			define_component_interface(scheduler_itf,typename get_type_of(Scheduling_Faculty),Prototypes::Person_Scheduler,ComponentType);
			scheduler_itf* scheduler = this->Scheduling_Faculty<scheduler_itf*>();
			return scheduler->template current_activity_plan<TargetType>();
		}
		feature_prototype typename TargetType::ReturnType next_activity_plan(typename TargetType::ParamType current_time)
		{
			define_component_interface(scheduler_itf,typename get_type_of(Scheduling_Faculty),Prototypes::Person_Scheduler,ComponentType);
			scheduler_itf* scheduler = this->Scheduling_Faculty<scheduler_itf*>();
			return scheduler->template next_activity_plan<TargetType>(current_time);
		}
		feature_prototype typename TargetType::ReturnType previous_activity_plan(typename TargetType::ParamType current_time)
		{
			define_component_interface(scheduler_itf,typename get_type_of(Scheduling_Faculty),Prototypes::Person_Scheduler,ComponentType);
			scheduler_itf* scheduler = this->Scheduling_Faculty<scheduler_itf*>();
			return scheduler->template previous_activity_plan<TargetType>(current_time);
		}
		feature_prototype void Add_Movement_Plan(TargetType movement_plan)
		{
			this_component()->template Add_Movement_Plan<ComponentType,CallerType,TargetType>(movement_plan);
		}
		feature_prototype void Add_Activity_Plan(TargetType activity_plan)
		{
			this_component()->template Add_Activity_Plan<ComponentType,CallerType,TargetType>(activity_plan);
		}
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

		feature_prototype TargetType Choose_Destination(TargetType previous_location, TargetType next_location, vector<TargetType>* destinations_to_use=nullptr, requires(check_as_given(TargetType,is_pointer) && check(TargetType,Activity_Location_Components::Concepts::Is_Activity_Location)))
		{
			return this_component()->template Choose_Destination<ComponentType,CallerType,TargetType>(previous_location,next_location,destinations_to_use);
		}
	};


	prototype struct Destination_Choice_Option ADD_DEBUG_INFO
	{
		tag_as_prototype;

		// accessor to parent class
		feature_accessor(Parent_Planner,none,none);
		feature_accessor(previous,none,none);
		feature_accessor(destination,none,none);
		feature_accessor(next,none,none);

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