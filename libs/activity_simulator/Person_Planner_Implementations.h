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
		implementation struct General_Person_Planner_Implementation : public Polaris_Component<MasterType,INHERIT(General_Person_Planner_Implementation),Execution_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(General_Person_Planner_Implementation),Execution_Object>::Component_Type ComponentType;

			/*static ofstream logs[num_sim_threads()];
			static m_data(bool, write_activity_files, NONE, NONE);*/
			template<typename TargetType> void Write_To_Log(TargetType s, requires(TargetType,!check_2(TargetType,string, is_same)))
			{
				//if (_write_activity_files) this->logs[__thread_id] << s.str();
			}
			template<typename TargetType> void Write_To_Log(TargetType s, requires(TargetType,check_2(TargetType,string, is_same)))
			{
				//if (_write_activity_files) this->logs[__thread_id] << s;
			}

			// Pointer to the Parent class
			m_prototype(Null_Prototype< typename MasterType::person_type>, Parent_Person, NONE, NONE);

			// Pointer to the child classses
			//m_prototype(Prototypes::Person_Scheduler< typename MasterType::person_scheduler_type>, Person_Scheduler, NONE, NONE);
			m_prototype(Null_Prototype< typename MasterType::activity_generator_type>, Activity_Generation_Faculty, NONE, NONE);
			m_prototype(Null_Prototype< typename MasterType::person_destination_chooser_type>, Destination_Choice_Faculty, NONE, NONE);
			m_prototype(Null_Prototype< typename MasterType::person_mode_chooser_type>, Mode_Choice_Faculty, NONE, NONE);
			m_prototype(Null_Prototype< typename MasterType::activity_timing_chooser_type>, Timing_Chooser, NONE, NONE);

			// Next Activity Generation Time member - used to schedule the next activity generation
			member_component_and_feature_accessor(Next_Activity_Generation_Time, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);
			// Planning Time Increment member - sets the next iteration after all planning is completed
			member_component_and_feature_accessor(Planning_Time_Increment, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);
			// Generation Time Increment member - sets the next Generation iteration 
			member_component_and_feature_accessor(Generation_Time_Increment, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(Next_Planning_Time, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);

			//Containers for activity planning events and movement planning events
			//m_container(boost::container::list<typename MasterType::activity_type*>,Activity_Container, NONE, NONE);
			//m_container(boost::container::list<typename MasterType::activity_record_type*>,Activity_Record_Container, NONE, NONE);
			//m_container(boost::container::list<typename MasterType::movement_plan_type*>,Movement_Plans_Container, NONE, NONE);


			// Interface definitions
			typedef  Prototypes::Person_Scheduler< typename Parent_Person_interface_type::get_type_of(Scheduling_Faculty)> _Scheduler_Interface;
			typedef  Scenario_Components::Prototypes::Scenario< typename Parent_Person_interface_type::get_type_of(scenario_reference)> _Scenario_Interface;
			typedef  Network_Components::Prototypes::Network< typename Parent_Person_interface_type::get_type_of(network_reference)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;
			typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename _Network_Interface::get_type_of(activity_locations_container)::value_type>::type>  _Activity_Location_Interface;
			typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container), _Activity_Location_Interface*> _Activity_Locations_Container_Interface;

			typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Activity_Location_Interface::get_type_of(origin_links)::value_type>::type>  _Link_Interface;
			typedef  Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links), _Link_Interface*> _Links_Container_Interface;

			typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename _Network_Interface::get_type_of(zones_container)::value_type>::type>  _Zone_Interface;
			typedef  Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container), _Zone_Interface*> _Zones_Container_Interface;

			///*typedef Activity_Components::Prototypes::Activity_Planner<typename remove_pointer<typename  type_of(Activity_Container)::value_type>::type> Activity_Plan;
			///*typedef Back_Insertion_Sequence< type_of(Activity_Container),Activity_Plan*> Activity_Plans;

			//typedef Activity_Components::Prototypes::Activity_Planner<typename remove_pointer<typename  type_of(Activity_Record_Container)::value_type>::type> Activity_Record;
			//typedef Back_Insertion_Sequence< type_of(Activity_Record_Container),Activity_Record*> Activity_Records;

			//typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename remove_pointer<typename  type_of(Movement_Plans_Container)::value_type>::type> Movement_Plan;
			//typedef Back_Insertion_Sequence< type_of(Movement_Plans_Container),Movement_Plan*> Movement_Plans;

		

			// Adding activities and movements to the planning schedules
			template<typename TargetType> void Add_Movement_Plan(TargetType movement_plan)
			{
				_Scheduler_Interface* scheduler = this->_Parent_Person->template Scheduling_Faculty<_Scheduler_Interface*>();
				scheduler->Add_Movement_Plan<TargetType>(movement_plan);
			} tag_feature_as_available(Add_Movement_Plan);
			template<typename TargetType> void Add_Activity_Plan(TargetType activity_plan)
			{
				_Scheduler_Interface* scheduler = this->_Parent_Person->template Scheduling_Faculty<_Scheduler_Interface*>();
				scheduler->Add_Activity_Plan<TargetType>(activity_plan);

			} tag_feature_as_available(Add_Activity_Plan);

			template<typename TargetType> void Initialize(/*requires(TargetType,check(typename ComponentType::Parent_Type,Concepts::Is_Person))*/)
			{	
				base_type::template Generation_Time_Increment<Time_Minutes>(END);
				base_type::template Planning_Time_Increment<Time_Minutes>(5);
				base_type::template Next_Activity_Generation_Time<Simulation_Timestep_Increment>(60);	
			}
		};
		// static member definition
		/*template<typename MasterType,  typename InheritanceList> ofstream General_Person_Planner_Implementation<MasterType,  InheritanceList>::logs[num_sim_threads()];
		static_member_definition(General_Person_Planner_Implementation,write_activity_files);*/




		implementation struct POLARIS_Person_Planner_Implementation : public General_Person_Planner_Implementation<MasterType,  INHERIT(POLARIS_Person_Planner_Implementation)>
		{
			// IMPLEMENTATION TYPEDEFS AND INTERFACES
			typedef General_Person_Planner_Implementation<MasterType,  INHERIT(POLARIS_Person_Planner_Implementation)> base_type;
			typedef typename base_type::Component_Type ComponentType;

			template<typename TargetType> void Initialize(/*requires(TargetType,check(typename ComponentType::Parent_Type,Concepts::Is_Person))*/)
			{	
				base_type::template Generation_Time_Increment<Time_Minutes>(END);
				base_type::template Planning_Time_Increment<Time_Minutes>(5);
				base_type::template Next_Activity_Generation_Time<Simulation_Timestep_Increment>(60);	
			}
			/*template<typename TargetType> void Initialize(requires(TargetType,check(typename ComponentType::Parent_Type,!Concepts::Is_Person)))
			{	
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Initialize_Defined, "The specified ParentType is not a valid Person type.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Properties_Defined, "The specified ParentType does not have the required Properties member defined.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Planner_Defined, "The specified ParentType does not have the required Planner member defined.");
			}
			tag_feature_as_available(Initialize);*/

		};
		
	}
}
