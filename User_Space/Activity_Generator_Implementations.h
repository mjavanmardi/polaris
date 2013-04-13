#pragma once

#include "Person_Prototype.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		implementation struct General_Activity_Generator_Implementation : public Polaris_Component<APPEND_CHILD(General_Activity_Generator_Implementation),MasterType,Data_Object,ParentType>
		{
			// Pointer to the Parent class
			member_prototype(Person_Components::Prototypes::Person_Planner, Parent_Planner, typename MasterType::person_planner_type, none, none);

			// Interface definitions
			define_component_interface(_Scenario_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_Network_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_Skim_Interface, typename _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan, typename type_of(Parent_Planner)::type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface_unqualified_container(Movement_Plans,Movement_Plan, typename type_of(Parent_Planner)::type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
		
		};

		implementation struct ADAPTS_Activity_Generator_Implementation : public General_Activity_Generator_Implementation<MasterType, ParentType, APPEND_CHILD(ADAPTS_Activity_Generator_Implementation)>
		{
			typedef General_Activity_Generator_Implementation<MasterType, ParentType, APPEND_CHILD(ADAPTS_Activity_Generator_Implementation)> base_type;
			typedef Prototypes::Activity_Generator<base_type,base_type> base_itf;
			define_component_interface(person_itf,typename base_type::type_of(Parent_Planner)::type_of(Parent_Person), Prototypes::Person,ComponentType);

			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,Concepts::Is_Person)))
			{	
			}
			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,!Concepts::Is_Person)))
			{	
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Initialize_Defined, "The specified ParentType is not a valid Person type.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Properties_Defined, "The specified ParentType does not have the required Properties member defined.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Planner_Defined, "The specified ParentType does not have the required Planner member defined.");
			}
			tag_feature_as_available(Initialize);

			feature_implementation void Activity_Generation()
			{
				// Create alias for this to use in conditional

				// Generate average of 4 activities per day

				define_container_and_value_interface(Activity_Plans,Activity_Plan,typename base_type::type_of(Activity_Plans_Container),Associative_Container_Prototype,Activity_Components::Prototypes::Activity_Plan_Prototype,ComponentType);
				Activity_Plan* act = (Activity_Plan*)Allocate<typename base_type::type_of(Activity_Plans_Container)::unqualified_value_type>();
				act->template Activity_Plan_ID<long>(_iteration);
				Activity_Plans* activities = this_ptr->template Activity_Plans_Container<Activity_Plans*>();
				activities->insert(Simulation_Time.Future_Time<Time_Minutes,Simulation_Timestep_Increment>(15),act);

				define_container_and_value_interface(Movement_Plans,Movement_Plan,typename base_type::type_of(Movement_Plans_Container),Associative_Container_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
				Movement_Plan* move = (Movement_Plan*)Allocate<typename base_type::type_of(Movement_Plans_Container)::unqualified_value_type>();
				move->template Movement_Plan_ID<long>(_iteration);
				Movement_Plans* movements = this_ptr->template Movement_Plans_Container<Movement_Plans*>();
				movements->insert(Simulation_Time.Future_Time<Time_Minutes,Simulation_Timestep_Increment>(30),move);
			}
			tag_feature_as_available(Activity_Generation);
		};

		implementation struct CTRAMP_Activity_Generator_Implementation : public General_Activity_Generator_Implementation<MasterType, ParentType, APPEND_CHILD(CTRAMP_Activity_Generator_Implementation)>
		{
			// IMPLEMENTATION TYPEDEFS AND INTERFACES
			typedef General_Activity_Generator_Implementation<MasterType, ParentType, APPEND_CHILD(CTRAMP_Activity_Generator_Implementation)> base_type;
			typedef base_type base;
			typedef Prototypes::Activity_Generator<base_type,base_type> base_itf;
			
			// Interface definitions
			define_component_interface(_planner_itf,typename type_of(Parent_Planner),Prototypes::Person_Planner,ComponentType);
			define_component_interface(_Destination_Choice_Itf, typename type_of(Parent_Planner)::type_of(Destination_Chooser), Prototypes::Destination_Chooser, ComponentType);
			define_component_interface(person_itf,typename base_type::type_of(Parent_Planner)::type_of(Parent_Person), Prototypes::Person,ComponentType);
			define_component_interface(_static_properties_itf,typename base_type::type_of(Parent_Planner)::type_of(Parent_Person)::type_of(Static_Properties), Prototypes::Person_Properties,ComponentType);
			define_component_interface(_properties_itf,typename base_type::type_of(Parent_Planner)::type_of(Parent_Person)::type_of(Properties), Prototypes::Person_Properties,ComponentType);
			define_component_interface(_Scenario_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_Network_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_Skim_Interface, typename _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(Activities, Activity, typename type_of(Parent_Planner)::type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::routine_activity_plan_type,ComponentType> Routine_Activity_Plan;
			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_plan_type,ComponentType> Activity_Plan;
			define_container_and_value_interface_unqualified_container(Movement_Plans,Movement_Plan, typename type_of(Parent_Planner)::type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);

			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,Concepts::Is_Person)))
			{	
			}
			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,!Concepts::Is_Person)))
			{	
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Initialize_Defined, "The specified ParentType is not a valid Person type.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Properties_Defined, "The specified ParentType does not have the required Properties member defined.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Planner_Defined, "The specified ParentType does not have the required Planner member defined.");
			}
			tag_feature_as_available(Initialize);

			feature_implementation void Activity_Generation()
			{
				
				person_itf* _Parent_Person = base_type::_Parent_Planner->Parent_Person<person_itf*>();
				_static_properties_itf* static_properties = _Parent_Person->Static_Properties<_static_properties_itf*>();
				_properties_itf* properties = _Parent_Person->Properties<_properties_itf*>();
				_Destination_Choice_Itf* destination_chooser = base_type::_Parent_Planner->Destination_Chooser<_Destination_Choice_Itf*>();
				
				// get references to the plan containers
				Activities* activities = typename base_type::_Parent_Planner->template Activity_Container<Activities*>();
				Movement_Plans* movements = typename base_type::_Parent_Planner->template Movement_Plans_Container<Movement_Plans*>();	

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Scenario_Interface* scenario = _Parent_Person->template scenario_reference<_Scenario_Interface*>();

				int act_count = 1;
				

				//=========================================================================================================================
				// Generate work activity
				EMPLOYMENT_STATUS work_status = static_properties->Employment_Status<EMPLOYMENT_STATUS>();
				if (work_status == EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_CIVILIAN_AT_WORK || work_status == EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_ARMED_FORCES_AT_WORK)
				{
					Routine_Activity_Plan* activity = (Routine_Activity_Plan*)Allocate<typename MasterType::routine_activity_plan_type>();
					activity->Parent_Planner<_planner_itf*>(_Parent_Planner);
					
					// Activity planning time
					Simulation_Timestep_Increment plan_time = _iteration + act_count;
					activity->Initialize<Target_Type<NT,NT, ACTIVITY_TYPES, Simulation_Timestep_Increment> >(ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY, plan_time);
					activities->push_back((Activity*)activity);
					act_count++;
				}
				//-------------------------------------------------------------------------------------------------------------------------


				//=========================================================================================================================
				// Generate school activity
				Person_Components::Types::SCHOOL_ENROLLMENT sch_status = static_properties->School_Enrollment<SCHOOL_ENROLLMENT>();
				if (sch_status == SCHOOL_ENROLLMENT::ENROLLMENT_PUBLIC || sch_status == SCHOOL_ENROLLMENT::ENROLLMENT_PRIVATE)
				{
					Routine_Activity_Plan* activity = (Routine_Activity_Plan*)Allocate<typename MasterType::routine_activity_plan_type>();
					activity->Parent_Planner<_planner_itf*>(_Parent_Planner);
					
					// Activity planning time
					Simulation_Timestep_Increment plan_time = _iteration + act_count;
					activity->Initialize<Target_Type<NT,NT, ACTIVITY_TYPES, Simulation_Timestep_Increment> >(ACTIVITY_TYPES::SCHOOL_ACTIVITY, plan_time);
					activities->push_back((Activity*)activity);
					act_count++;
				}
				//-------------------------------------------------------------------------------------------------------------------------


				//=========================================================================================================================
				// Generate other activities
				int num_activities = 2;
				for (int i = act_count; i < act_count + num_activities; i++)
				{
					Activity_Plan* activity = (Activity_Plan*)Allocate<typename MasterType::activity_plan_type>();
					activity->Parent_Planner<_planner_itf*>(_Parent_Planner);
					
					// Activity planning time
					Simulation_Timestep_Increment plan_time = _iteration + i;
					activity->Initialize<Target_Type<NT,NT, ACTIVITY_TYPES, Simulation_Timestep_Increment> >(ACTIVITY_TYPES::OTHER_ACTIVITY, plan_time);
					activities->push_back((Activity*)activity);
				}
				act_count = act_count + num_activities;
			}
			tag_feature_as_available(Activity_Generation);

		};
	}
}