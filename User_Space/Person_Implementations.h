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
			feature_implementation void Initialize(TargetType id)
			{	
				// Set the initial iteration to process
				this->template First_Iteration<ComponentType,CallerType,Time_Minutes>(1.0);

				// Create and Initialize the Properties faculty
				define_component_interface(properties_itf,type_of(Properties),Prototypes::Person_Properties,ComponentType);
				properties_itf* properties = (properties_itf*)Allocate<type_of(Properties)>();
				properties->template Initialize<NULLTYPE>();
				properties->template Parent_Person<ComponentType*>(this);
				this->template Properties<ComponentType,ComponentType,properties_itf*>(properties);

				// Create and Initialize the Planner faculty and its subcomponents
				define_component_interface(planner_itf,type_of(Planning_Faculty),Prototypes::Person_Planner,ComponentType);
				define_component_interface(generator_itf,typename type_of(Planning_Faculty)::type_of(Activity_Generator),Prototypes::Activity_Generator,ComponentType);
				define_component_interface(destination_choice_itf,typename type_of(Planning_Faculty)::type_of(Destination_Chooser),Prototypes::Destination_Choice,ComponentType);
				planner_itf* planner = (planner_itf*)Allocate<type_of(Planning_Faculty)>();	
				planner->template Parent_Person<ComponentType*>(this);
				planner->template Initialize<NULLTYPE>();
				this->template Planning_Faculty<ComponentType,ComponentType,planner_itf*>(planner);
				generator_itf* generator = (generator_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Activity_Generator)>();
				generator->template Parent_Planner<planner_itf*>(planner);
				destination_choice_itf* destination_chooser = (destination_choice_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Destination_Chooser)>();
				destination_chooser->template Parent_Planner<planner_itf*>(planner);
				planner->template Activity_Generator<generator_itf*>(generator);
				planner->template Destination_Chooser<destination_choice_itf*>(destination_chooser);

				// Create and Initialize the routing faculty
				define_component_interface(_Routing_Interface, type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
				define_component_interface(_Network_Interface, type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				_Routing_Interface* router=(_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();
				router->template traveler<ComponentType*>(this);
				router->template network<_Network_Interface*>(this->template network_reference<ComponentType,CallerType,_Network_Interface*>());	

				// Moving faculty
				Moving_Faculty_interface* move_faculty = (Moving_Faculty_interface*)Allocate<Implementations::Person_Mover_Implementation<MasterType>>();
				move_faculty->Parent_Person<ComponentType*>(this);
				this->_Moving_Faculty = move_faculty;

				// Create and Initialize the vehicle
				define_component_interface(_Vehicle_Interface, type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				_Vehicle_Interface* vehicle = (_Vehicle_Interface*)Allocate<typename _Vehicle_Interface::Component_Type>(); 	
				vehicle->template uuid<int>(id);
				vehicle->template internal_id<int>(id);
				vehicle->template traveler<ComponentType*>(this);

				// Add basic traveler properties							
				this->template uuid<ComponentType,ComponentType,int>(id);
				this->template router<ComponentType,ComponentType,_Routing_Interface*>(router);
				this->template vehicle<ComponentType,ComponentType,_Vehicle_Interface*>(vehicle);
			}
			feature_implementation void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type trip)
			{	
				// Set the initial iteration to process
				this->template First_Iteration<Time_Minutes>(15.0);

				// Set the agent ID
				this->template uuid<ComponentType,CallerType,TargetType::ParamType>(id);
				this->template internal_id<ComponentType,CallerType,TargetType::ParamType>(id);

				// Create and Initialize the Properties faculty
				define_component_interface(properties_itf,type_of(Properties),Prototypes::Person_Properties,ComponentType);
				properties_itf* properties = (properties_itf*)Allocate<type_of(Properties)>();
				properties->template Initialize<NULLTYPE>();
				properties->template Parent_Person<ComponentType*>(this);
				this->template Properties<ComponentType,ComponentType,properties_itf*>(properties);

				// Create and Initialize the Planner faculty
				define_component_interface(planner_itf,type_of(Planning_Faculty),Prototypes::Person_Planner,ComponentType);
				planner_itf* planner = (planner_itf*)Allocate<type_of(Planning_Faculty)>();
				planner->template Initialize<TargetType::Param2Type>(trip);
				planner->template Parent_Person<ComponentType*>(this);
				this->template Planning_Faculty<ComponentType,ComponentType,planner_itf*>(planner);		

				// Seed the RNG with the agent ID
				define_component_interface(rng_itf,type_of(RNG),RNG_Components::Prototypes::RNG_Prototype,ComponentType);
				rng_itf* rng = this->template RNG<ComponentType,ComponentType,rng_itf*>();	
				rng->template Initialize<TargetType::ParamType>(id);
			}
			tag_feature_as_available(Initialize);	

			member_component(typename MasterType::vehicle_type,vehicle,none,none);
			member_component(typename MasterType::routing_type,router,none,none);
			member_prototype(Prototypes::Person_Mover, Moving_Faculty,Implementations::Person_Mover_Implementation<MasterType>, none, none);
		
			member_component(typename MasterType::person_planner_type,Planning_Faculty,none,check_2(ComponentType,CallerType, Is_Same_Entity));
			member_component(typename MasterType::person_properties_type,Properties,none,check_2(ComponentType,CallerType, Is_Same_Entity));
			member_prototype(PopSyn::Prototypes::Population_Unit_Prototype,Static_Properties,typename MasterType::pop_unit,none,none);

			member_component(typename MasterType::scenario_type, scenario_reference, none, none);
			member_component(typename MasterType::network_type, network_reference, none, none);
			
			// Random number facility - accessed using Next_Rand feature_accessor
			feature_implementation TargetType Next_Rand()
			{
				return GLOBALS::Uniform_RNG.Next_Rand<double>();
			} tag_getter_as_available(Next_Rand);

			// Agent ID
			member_data(long,uuid,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			// First iteration  - sets the next iteration after all planning is completed
			member_data_component(typename Basic_Units::Implementations::Time_Implementation<MasterType>,_First_Iteration,none,none);
			member_component_feature(First_Iteration, _First_Iteration, Value, Basic_Units::Prototypes::Time_Prototype);

		};


	}
}