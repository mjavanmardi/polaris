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
			member_prototype(Person_Components::Prototypes::Person_Planner, Parent_Planner, typename MasterType::person_planner_type, none, none);


			// Interface definitions
			define_component_interface(_Person_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person), Prototypes::Person, ComponentType);
			define_component_interface(_Scenario_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_Network_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_Skim_Interface, typename _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan, typename type_of(Parent_Planner)::type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface_unqualified_container(Movement_Plans,Movement_Plan, typename type_of(Parent_Planner)::type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
	

			// Pass through accessors for the Activities / movements
			feature_implementation TargetType Activity_Container(requires(check_2(TargetType,Activity_Plans,is_same) && (check_as_given(TargetType,is_pointer) || check_as_given(TargetType,is_reference))))
			{
				return this->_Parent_Planner->Activity_Container<TargetType>();
			};
			feature_implementation TargetType Movement_Plans_Container(requires(check_2(TargetType,Movement_Plans,is_same) && (check_as_given(TargetType,is_pointer) || check_as_given(TargetType,is_reference))))
			{
				return this->_Parent_Planner->Movement_Plans_Container<TargetType>();
			};


			feature_implementation TargetType current_movement_plan(requires(check_as_given(TargetType,is_pointer) && check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				// Define interfaces to the container members of the class			
				Movement_Plans* movement_plans = this->template Movement_Plans_Container<ComponentType,CallerType, Movement_Plans*>();
				typename Movement_Plans::iterator itr;
				if ((itr = movement_plans->find(_iteration)) != movement_plans->end()) return (TargetType)*itr;
				else return nullptr;
			}
			tag_feature_as_available(current_movement_plan);
			feature_implementation TargetType current_activity_plan(requires(check_as_given(TargetType,is_pointer) && check(TargetType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			{
				// Define interfaces to the container members of the class
				Activity_Plans* activity_plans = this->template Activity_Plans_Container<ComponentType,CallerType,Activity_Plans*>();
				typename Activity_Plans::iterator itr;
				if ((itr = activity_plans->find(_iteration)) != activity_plans->end()) return (TargetType)*itr;
				else return nullptr;
			}
			tag_feature_as_available(current_activity_plan);

			// Adding activities and movements to the planning schedules
			feature_implementation void Add_Movement_Plan(TargetType movement_plan, requires(check_as_given(TargetType,is_pointer) && check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				// define interfaces
				typename Movement_Plans::iterator move_itr;

				// get interface to the movement plan to be added
				Movement_Plan* move = (Movement_Plan*)movement_plan;

				// print to log file if requested
				if (_write_activity_files) 
				{			
					stringstream s;
					int o_id = move->template origin<_Activity_Location_Interface*>()->template zone<_Zone_Interface*>()->template uuid<int>();
					int d_id = move->template destination<_Activity_Location_Interface*>()->template zone<_Zone_Interface*>()->template uuid<int>();
					_Skim_Interface* skim = _Parent_Person->template network_reference<_Network_Interface*>()->template skimming_faculty<_Skim_Interface*>();		
					s << _Parent_Person->template uuid<int>() << "," << move->template departed_time<Time_Hours>();
					s << "," << move->template origin<_Activity_Location_Interface*>()->uuid<int>() << "," << move->template destination<_Activity_Location_Interface*>()->uuid<int>();
					s << "," << skim->template Get_LOS<Target_Type<NULLTYPE,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys> >(o_id, d_id, Vehicle_Components::Types::SOV)<<endl;
					this->Write_To_Log<ComponentType,CallerType,stringstream&>(s);
				}

				// catch skipped movement plans
				if (move->template departed_time<Simulation_Timestep_Increment>() < _iteration)
				{
					if (_write_activity_files) this->logs[_thread_id] <<",invalid-start prior to current iteration."<<endl;
					return;
				}

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
				
			}
			feature_implementation void Add_Movement_Plan(TargetType movement_plan, requires(!check_as_given(TargetType,is_pointer) || !check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				assert_check_as_given(TargetType,is_pointer, "Error, TargetType must be passed as a pointer");
				assert_check(TargetType, Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype, "Error, Function requires TargetType to be a Movement_Plan_Prototype.");
			}
			tag_feature_as_available(Add_Movement_Plan);
			feature_implementation void Add_Activity_Plan(TargetType activity_plan, requires(check_as_given(TargetType,is_pointer) && check(TargetType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			{
				define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan,type_of(Activity_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Plan,ComponentType);	
				Activity_Plan* act = (Activity_Plan*)activity_plan;

				long t1 = act->template Activity_Plan_Horizon<Simulation_Timestep_Increment>();
				long t2 = this->template Planning_Time_Increment<ComponentType,CallerType,Simulation_Timestep_Increment>();
				long remain = (long)(t1 / t2);
				Simulation_Timestep_Increment departure_time = remain * this->template Planning_Time_Increment<ComponentType,CallerType,Simulation_Timestep_Increment>();

				// key the movement plan on the planning timestep just prior to departure
				Activity_Plans* activities = this->template Activity_Plans_Container<Activity_Plans*>();
				activities->push_back(act);
			}
		};

	}

}