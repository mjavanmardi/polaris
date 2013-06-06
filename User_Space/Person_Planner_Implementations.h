#pragma once

//#include "Person_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Network_Skimming_Prototype.h"
#include "Activity_Prototype.h"
//#include "Person_Implementations.h"


namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		implementation struct General_Person_Planner_Implementation : public Polaris_Component<APPEND_CHILD(General_Person_Planner_Implementation),MasterType,Execution_Object,ParentType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<APPEND_CHILD(General_Person_Planner_Implementation),MasterType,Execution_Object>::Component_Type ComponentType;

			static ofstream logs[_num_threads];
			static member_data(bool, write_activity_files,none,none);
			feature_implementation void Write_To_Log(TargetType s, requires(!check_2(TargetType,string, is_same)))
			{
				if (_write_activity_files) this->logs[_thread_id] << s.str();
			}
			feature_implementation void Write_To_Log(TargetType s, requires(check_2(TargetType,string, is_same)))
			{
				if (_write_activity_files) this->logs[_thread_id] << s;
			}

			// Pointer to the Parent class
			member_prototype(Person_Components::Prototypes::Person, Parent_Person, typename MasterType::person_type, none, none);

			// Pointer to the child classses
			//member_prototype(Prototypes::Person_Scheduler, Person_Scheduler, typename MasterType::person_scheduler_type,none,none);
			member_prototype(Prototypes::Activity_Generator, Activity_Generation_Faculty, typename MasterType::activity_generator_type,none,none);
			member_prototype(Prototypes::Destination_Chooser, Destination_Choice_Faculty, typename MasterType::person_destination_chooser_type,none,none);
			member_prototype(Prototypes::Activity_Timing_Chooser, Timing_Chooser, typename MasterType::activity_timing_chooser_type,none,none);

			// Next Activity Generation Time member - used to schedule the next activity generation
			member_component_and_feature_accessor(Next_Activity_Generation_Time, Value, Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>);
			// Planning Time Increment member - sets the next iteration after all planning is completed
			member_component_and_feature_accessor(Planning_Time_Increment, Value, Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>);
			// Generation Time Increment member - sets the next Generation iteration 
			member_component_and_feature_accessor(Generation_Time_Increment, Value, Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(Next_Planning_Time, Value, Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>);

			//Containers for activity planning events and movement planning events
			//member_container(list<typename MasterType::activity_type*>,Activity_Container,none,none);
			//member_container(list<typename MasterType::activity_record_type*>,Activity_Record_Container,none,none);
			//member_container(list<typename MasterType::movement_plan_type*>,Movement_Plans_Container,none,none);


			// Interface definitions
			define_component_interface(_Scheduler_Interface, typename Parent_Person_interface::get_type_of(Scheduling_Faculty), Prototypes::Person_Scheduler, ComponentType);
			define_component_interface(_Scenario_Interface, typename Parent_Person_interface::get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_Network_Interface, typename Parent_Person_interface::get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_Skim_Interface, typename _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			///*define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan, type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			//define_container_and_value_interface_unqualified_container(Activity_Records,Activity_Record, type_of(Activity_Record_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			//define_container_and_value_interface_unqualified_container(Movement_Plans,Movement_Plan, type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);*/
		

			// Adding activities and movements to the planning schedules
			feature_implementation void Add_Movement_Plan(TargetType movement_plan)
			{
				_Scheduler_Interface* scheduler = this->_Parent_Person->template Scheduling_Faculty<_Scheduler_Interface*>();
				scheduler->Add_Movement_Plan<TargetType>(movement_plan);
			} tag_feature_as_available(Add_Movement_Plan);
			feature_implementation void Add_Activity_Plan(TargetType activity_plan)
			{
				_Scheduler_Interface* scheduler = this->_Parent_Person->template Scheduling_Faculty<_Scheduler_Interface*>();
				scheduler->Add_Activity_Plan<TargetType>(activity_plan);

			} tag_feature_as_available(Add_Activity_Plan);
		};
		// static member definition
		template<typename MasterType, typename ParentType, typename InheritanceList> ofstream General_Person_Planner_Implementation<MasterType, ParentType, InheritanceList>::logs[_num_threads];
		static_member_definition(General_Person_Planner_Implementation,write_activity_files);




		implementation struct POLARIS_Person_Planner_Implementation : public General_Person_Planner_Implementation<MasterType, ParentType, APPEND_CHILD(POLARIS_Person_Planner_Implementation)>
		{
			// IMPLEMENTATION TYPEDEFS AND INTERFACES
			typedef General_Person_Planner_Implementation<MasterType, ParentType, APPEND_CHILD(POLARIS_Person_Planner_Implementation)> base_type;
			typedef typename base_type::Component_Type ComponentType;

			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,Concepts::Is_Person)))
			{	
				base_type::template Generation_Time_Increment<ComponentType,CallerType,Time_Minutes>(END);
				base_type::template Planning_Time_Increment<ComponentType,CallerType,Time_Minutes>(5);
				base_type::template Next_Activity_Generation_Time<ComponentType,CallerType,Simulation_Timestep_Increment>(60);	
			}
			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,!Concepts::Is_Person)))
			{	
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Initialize_Defined, "The specified ParentType is not a valid Person type.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Properties_Defined, "The specified ParentType does not have the required Properties member defined.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Planner_Defined, "The specified ParentType does not have the required Planner member defined.");
			}
			tag_feature_as_available(Initialize);

		};
		
	}
}