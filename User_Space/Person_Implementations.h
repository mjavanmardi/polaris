#pragma once

#include "Person_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Network_Skimming_Prototype.h"
#include "Activity_Prototype.h"
#include "Population_Unit_Implementations.h"


namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Person Agent classes
		//----------------------------------------------------------------------------------
		implementation struct Person_Mover_Implementation : public Polaris_Component<APPEND_CHILD(Person_Mover_Implementation),MasterType,Execution_Object,ParentType>
		{
			member_prototype(Prototypes::Person,Parent_Person,typename MasterType::person_type,none,none);
			member_prototype(Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,Movement,typename MasterType::movement_plan_type,none,none);
			member_data(bool, Movement_Scheduled, check(ReturnValueType,is_integral), check(SetValueType,is_integral));
		};

		implementation struct Person_Implementation : public Polaris_Component<APPEND_CHILD(Person_Implementation),MasterType,Execution_Object,ParentType>
		{
			//=======================================================================================================================================================================
			// DATA MEMBERS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			member_prototype(Vehicle_Components::Prototypes::Vehicle_Prototype, vehicle,typename MasterType::vehicle_type,none,none);
			member_prototype(Routing_Components::Prototypes::Routing_Prototype, router,typename MasterType::routing_type,none,none);
			member_prototype(Prototypes::Person_Mover, Moving_Faculty,Implementations::Person_Mover_Implementation<MasterType>, none, none);
		
			member_prototype(Prototypes::Person_Planner, Planning_Faculty, typename MasterType::person_planner_type,none,check_2(ComponentType,CallerType, Is_Same_Entity));
			member_prototype(Prototypes::Person_Properties, Properties, typename MasterType::person_properties_type,none,check_2(ComponentType,CallerType, Is_Same_Entity));
			member_prototype(PopSyn::Prototypes::Population_Unit_Prototype,Static_Properties,typename MasterType::pop_unit,none,none);

			member_prototype(Scenario_Components::Prototypes::Scenario_Prototype, scenario_reference, typename MasterType::scenario_type, none, none);
			member_prototype(Network_Components::Prototypes::Network_Prototype, network_reference, typename MasterType::network_type, none, none);

			// Agent ID
			member_data(long,uuid,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			// First iteration  - sets the next iteration after all planning is completed
			member_component_and_feature_accessor(First_Iteration, Value, Basic_Units::Prototypes::Time_Prototype, Basic_Units::Implementations::Time_Implementation<NT>);


			//=======================================================================================================================================================================
			// INTERFACE DEFINITIONS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			define_component_interface(generator_itf,typename type_of(Planning_Faculty)::type_of(Activity_Generator),Prototypes::Activity_Generator,ComponentType);
			define_component_interface(destination_choice_itf,typename type_of(Planning_Faculty)::type_of(Destination_Chooser),Prototypes::Destination_Chooser,ComponentType);
			define_container_and_value_interface(zones_container_interface, zone_interface, typename network_reference_interface::get_type_of(zones_container),Containers::Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype,ComponentType);
			define_container_and_value_interface(locations_container_interface, location_interface, typename network_reference_interface::get_type_of(activity_locations_container),Containers::Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);

			//=======================================================================================================================================================================
			// FEATURES
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			feature_implementation void Initialize(TargetType id)
			{	
				// Set the initial iteration to process
				this->template First_Iteration<ComponentType,CallerType,Time_Minutes>(1.0);

				// Create and Initialize the Properties faculty
				_Properties = (Properties_interface*)Allocate<type_of(Properties)>();
				_Properties->template Initialize<NULLTYPE>();
				_Properties->template Parent_Person<ComponentType*>(this);

				// Create and Initialize the Planner faculty and its subcomponents
				_Planning_Faculty = (Planning_Faculty_interface*)Allocate<type_of(Planning_Faculty)>();	
				_Planning_Faculty->template Parent_Person<ComponentType*>(this);
				_Planning_Faculty->template Initialize<NULLTYPE>();
				generator_itf* generator = (generator_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Activity_Generator)>();
				generator->template Parent_Planner<Planning_Faculty_interface*>(_Planning_Faculty);
				_Planning_Faculty->template Activity_Generator<generator_itf*>(generator);
				destination_choice_itf* destination_chooser = (destination_choice_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Destination_Chooser)>();
				destination_chooser->template Parent_Planner<Planning_Faculty_interface*>(_Planning_Faculty);
				_Planning_Faculty->template Destination_Chooser<destination_choice_itf*>(destination_chooser);

				// Create and Initialize the routing faculty
				_router=(router_interface*)Allocate<type_of(router)>();
				_router->template traveler<ComponentType*>(this);
				_router->template network<network_reference_interface*>(_network_reference);	

				// Moving faculty
				_Moving_Faculty = (Moving_Faculty_interface*)Allocate<Implementations::Person_Mover_Implementation<MasterType>>();
				_Moving_Faculty->Parent_Person<ComponentType*>(this);

				// Create and Initialize the vehicle		
				_vehicle = (vehicle_interface*)Allocate<type_of(vehicle)>(); 	
				_vehicle->template uuid<int>(id);
				_vehicle->template internal_id<int>(id);
				_vehicle->template traveler<ComponentType*>(this);

				// Add basic traveler properties							
				this->template uuid<ComponentType,ComponentType,int>(id);
			}
			tag_feature_as_available(Initialize);	

			feature_implementation TargetType Choose_Work_Location()
			{
				Time_Minutes ttime = _Static_Properties->Journey_To_Work_Travel_Time<Time_Minutes>();
			}
			tag_feature_as_available(Choose_Work_Location);

		};


	}
}