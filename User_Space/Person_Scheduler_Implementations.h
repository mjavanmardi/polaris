#pragma once

#include "Person_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Activity_Prototype.h"


namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		implementation struct General_Person_Scheduler_Implementation : public Polaris_Component<APPEND_CHILD(General_Person_Scheduler_Implementation),MasterType,Execution_Object,ParentType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<APPEND_CHILD(General_Person_Scheduler_Implementation),MasterType,Execution_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			member_prototype(Person_Components::Prototypes::Person, Parent_Person, typename MasterType::person_type, none, none);

			//Containers for activity planning events and movement planning events
			member_container(list<typename MasterType::activity_type*>,Activity_Container,none,none);
			member_container(list<typename MasterType::movement_plan_type*>,Movement_Plans_Container,none,none);

			// Interface definitions
			define_component_interface(_Scenario_Interface, typename type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_Network_Interface, typename type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_Skim_Interface, typename _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan, type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface_unqualified_container(Movement_Plans,Movement_Plan, type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
			define_container_and_value_interface(Activity_Records,Activity_Record, typename type_of(Parent_Person)::type_of(Activity_Record_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			
			// member features
			feature_implementation void Initialize()
			{

			} tag_feature_as_available(Initialize);

			// scheduling features - move to Person_Scheduler eventually
			feature_implementation TargetType current_movement_plan(requires(check_as_given(TargetType,is_pointer) && check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				// Define interfaces to the container members of the class			
				Movement_Plans* movement_plans = this->template Movement_Plans_Container<ComponentType,CallerType, Movement_Plans*>();
				typename Movement_Plans::iterator itr;
				if ((itr = movement_plans->find(_iteration)) != movement_plans->end()) return (TargetType)*itr;
				else return NULL;
			}
			tag_feature_as_available(current_movement_plan);
			feature_implementation TargetType current_activity_plan(requires(check_as_given(TargetType,is_pointer) && check(TargetType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			{
				// Define interfaces to the container members of the class
				Activity_Plans* activity_plans = this->template Activity_Container<Activity_Plans*>();
				typename Activity_Plans::iterator itr;
				if ((itr = activity_plans->find(_iteration)) != activity_plans->end()) return (TargetType)*itr;
				else return NULL;
			}
			tag_feature_as_available(current_activity_plan);
			feature_implementation typename TargetType::ReturnType previous_activity_plan(typename TargetType::ParamType current_time)
			{
				Activity_Plans* activity_plans = this->template Activity_Container<ComponentType,CallerType,Activity_Plans*>();
				typename Activity_Plans::iterator itr;

				// convert current time to seconds
				Time_Seconds start_time = GLOBALS::Time_Converter.Convert_Value<Target_Type<NT,Time_Seconds,typename TargetType::ParamType>>(current_time);
				Time_Seconds max_previous = 0;
				Activity_Plan* act;
				Activity_Plan* previous = nullptr;
				
				// search for the closest preceeding activity
				for (itr = activity_plans->begin(); itr != activity_plans->end(); ++itr)
				{
					act = (Activity_Plan*)(*itr);
					if (act->template Start_Is_Planned<bool>() && act->template Start_Time<Time_Seconds>() < start_time && act->template Start_Time<Time_Seconds>() > max_previous)
					{
						max_previous = act->template Start_Time<Time_Seconds>();
						previous = act;
					}
				}
				return (typename TargetType::ReturnType)previous;
			}
			tag_feature_signature_as_available(previous_activity_plan,1);
			feature_implementation typename TargetType::ReturnType next_activity_plan(typename TargetType::ParamType current_time)
			{
				Activity_Plans* activity_plans = this->template Activity_Container<ComponentType,CallerType,Activity_Plans*>();
				typename Activity_Plans::iterator itr;

				// convert current time to seconds
				Time_Seconds start_time = GLOBALS::Time_Converter.Convert_Value<Target_Type<NT,Time_Seconds,typename TargetType::ParamType>>(current_time);
				Time_Seconds min_next = 0;
				Activity_Plan* act;
				Activity_Plan* next = nullptr;
				
				// search for the closest preceeding activity
				for (itr = activity_plans->begin(); itr != activity_plans->end(); ++itr)
				{
					act = (Activity_Plan*)(*itr);
					if (act->template Start_Is_Planned<bool>() && act->template Start_Time<Time_Seconds>() > start_time && act->template Start_Time<Time_Seconds>() < min_next)
					{
						min_next = act->template Start_Time<Time_Seconds>();
						next = act;
					}
				}
				return (typename TargetType::ReturnType)next;
			}
			tag_feature_signature_as_available(next_activity_plan,1);

			// Adding activities and movements to the planning schedules
			feature_implementation void Add_Movement_Plan(TargetType movement_plan, requires(check_as_given(TargetType,is_pointer) && check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				// define interfaces
				typename Movement_Plans::iterator move_itr;

				// get interface to the movement plan to be added
				Movement_Plan* move = (Movement_Plan*)movement_plan;

				// catch skipped movement plans
				if (move->template departed_time<Simulation_Timestep_Increment>() < _iteration) return;

				// Insert new movement plan at the appropriate space in plan schedule
				Movement_Plans* movements = this->template Movement_Plans_Container<ComponentType,CallerType,Movement_Plans*>();
				move_itr = movements->begin();
				while(move_itr != movements->end())
				{
					Movement_Plan* current_move = *move_itr;
					if (current_move->template departed_time<Simulation_Timestep_Increment>() > move->template departed_time<Simulation_Timestep_Increment>()) break;
					++move_itr;
				}
				movements->insert(move_itr,move);	


				// log the activity throught the global person logger
				//define_component_interface(_Logger_Interface, typename MasterType::person_data_logger_type, Person_Components::Prototypes::Person_Data_Logger, NULLTYPE);	
				Activity_Plan* act = move->template destination_activity_reference<Activity_Plan*>();
				//((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity_Plan*>(act,false);


				// cache the movement plans activity in the activity record container since all attributes have been planned at this point			
				Activity_Records* act_records = _Parent_Person->template Activity_Record_Container<Activity_Records*>();
				Activity_Record* act_record = (Activity_Record*)Allocate<typename MasterType::activity_record_type>();
				act_record->template Initialize<Activity_Plan*>(act);
				act_records->push_back(act_record);
				
			}
			feature_implementation void Add_Movement_Plan(TargetType movement_plan, requires(!check_as_given(TargetType,is_pointer) || !check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				assert_check_as_given(TargetType,is_pointer, "Error, TargetType must be passed as a pointer");
				assert_check(TargetType, Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype, "Error, Function requires TargetType to be a Movement_Plan_Prototype.");
			}
			tag_feature_as_available(Add_Movement_Plan);
			feature_implementation void Add_Activity_Plan(TargetType activity_plan, requires(check_as_given(TargetType,is_pointer)/* && check(TargetType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/))
			{
				//define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan,type_of(Activity_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Plan,ComponentType);	
				Activity_Plan* act = (Activity_Plan*)activity_plan;

				// print to log file if requested
				//if (_write_activity_files) 
				//{			
				//	stringstream s;	
				//	s << endl << "ACTIVITY GEN (PERID.ACTID.ACTTYPE)," << _Parent_Person->template uuid<int>() << "," << act->template Activity_Plan_ID<int>() << ", " << act->template Activity_Type<ACTIVITY_TYPES>();
				//	this->Write_To_Log<ComponentType,CallerType,stringstream&>(s);
				//}

				Activity_Plans* activities = this->Activity_Container<ComponentType,CallerType,Activity_Plans*>();		
				activities->push_back(act);
			}
		
		};

	}

}