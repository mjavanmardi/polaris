#pragma once
#include "Movement_Plan_Prototype.h"


namespace Movement_Plan_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Polaris_Trajectory_Unit_Implementation:public Polaris_Component_Class<Polaris_Trajectory_Unit_Implementation,MasterType,Data_Object,ParentType>
		{
			member_data(int, delayed_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, enter_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, enter_interval_index, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_component(typename MasterType::link_type, link, none, none);

			feature_implementation void Initialize(TargetType link_val)
			{
				_link = (type_of(link)*)link_val;
				_enter_time = 0.0;
				_delayed_time=0.0;
			}

		};

		implementation struct Polaris_Movement_Plan_Implementation:public Polaris_Component_Class<Polaris_Movement_Plan_Implementation,MasterType,Data_Object,ParentType>
		{
			typedef Implementations::Polaris_Trajectory_Unit_Implementation<MasterType> trajectory_unit_type;
			member_container(vector<trajectory_unit_type*>, trajectory_container, none, none);
			
			//==================================================================================================================
			/// current_trajectory_position
			//------------------------------------------------------------------------------------------------------------------

			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType current_trajectory_position(requires(check_2(TargetType,int,is_same) || check_2(TargetType,int&,is_same)))
			{
				return (TargetType)_current_trajectory_index;
			}

			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType current_trajectory_position(requires(!check_2(TargetType,int,is_same) && !check_2(TargetType,int&,is_same)))
			{
				return (TargetType)_trajectory_container[_current_trajectory_index];
			}
			
			tag_getter_as_available(current_trajectory_position);

			template<typename ComponentType, typename CallerType, typename TargetType>
			void current_trajectory_position(TargetType val,requires(check_2(TargetType,int,is_same) || check_2(TargetType,int&,is_same)))
			{
				_current_trajectory_index=val;
			}

			tag_setter_as_available(current_trajectory_position);

			int _current_trajectory_index;
			//------------------------------------------------------------------------------------------------------------------

			member_component(typename MasterType::link_type, origin, none, none);
			member_component(typename MasterType::link_type, destination, none, none);
			member_data(int, departed_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, arrived_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_component(typename MasterType::plan_type, plan, none, none);


			feature_implementation void arrive_to_destination()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;
				_trajectory_container[_current_trajectory_index]->_delayed_time = 0.0;
				_arrived_time = ((_Scenario_Interface*)_global_scenario)->template current_simulation_time<int>();
			}

			feature_implementation void transfer_to_next_link(int delayed_time)
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;
				typedef Movement_Plan_Components::Prototypes::Trajectory_Unit_Prototype<typename MasterType::trajectory_unit_type,ComponentType> _Trajectory_Unit_Interface;

				if (_current_trajectory_index != -1)
				{
					_trajectory_container[_current_trajectory_index]->_delayed_time = delayed_time;
				}
				
				((Movement_Plan_Prototype<ComponentType,ComponentType>*)this)->advance_trajectory<_Trajectory_Unit_Interface*>();
				_trajectory_container[_current_trajectory_index]->_enter_time = ((_Scenario_Interface*)_global_scenario)->template current_simulation_time<int>();
			}
		};
	}
}