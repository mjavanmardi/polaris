#pragma once
#include "User_Space.h"

namespace Movement_Plan_Components
{
	namespace Types
	{
		struct Load_To_Entry_Queue{};
		struct Load_To_Origin_Link{};
	}

	namespace Concepts
	{
		concept struct Is_Movement_Plan
		{
			check_getter(has_trajectory, trajectory_container);
			check_getter(has_origin, origin);
			check_getter(has_destination, destination);
			check_getter(has_departed_time, departed_time);
			check_getter(has_arrived_time, arrived_time);

			define_default_check(has_trajectory && has_origin && has_destination && has_departed_time && has_arrived_time);
		};
		concept struct Is_Movement_Plan_Prototype
		{
			check_getter(has_trajectory, Component_Type::trajectory_container);
			check_getter(has_origin, Component_Type::origin);
			check_getter(has_destination, Component_Type::destination);
			check_getter(has_departed_time, Component_Type::departed_time);
			check_getter(has_arrived_time, Component_Type::arrived_time);

			define_default_check(has_trajectory && has_origin && has_destination && has_departed_time && has_arrived_time);
		};
	}
	
	namespace Prototypes
	{

		prototype struct Trajectory_Unit_Prototype
		{
			tag_as_prototype;
			
			feature void Initialize(TargetType val)
			{
				this_component()->template Initialize<ComponentType,CallerType,TargetType>(val);
			}

			feature_accessor(link, none, none);
			feature_accessor(delayed_time, none, none);
			feature_accessor(enter_time, none, none);
			feature_accessor(enter_interval_index, none, none);
			feature_prototype TargetType exit_time()
			{
				return (TargetType)(enter_time<int>() + delayed_time<float>());
			}
		};


		prototype struct Movement_Plan_Prototype
		{
			tag_as_prototype;

			feature_accessor(trajectory_container, none, none);
			feature_accessor(current_trajectory_position, none, none);
			feature_accessor(origin, none, none);
			feature_accessor(destination, none, none);
			feature_accessor(departed_time, none, none);
			feature_accessor(arrived_time, none, none);
			feature_accessor(plan, none, none);

			feature_prototype void set_trajectory(TargetType& path_container)
			{
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, get_type_of(trajectory_container), Back_Insertion_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Trajectory_Unit_Interface::get_type_of(link), Link_Components::Prototypes::Link_Prototype, ComponentType);
				_Trajectory_Container_Interface& trajectory=trajectory_container<_Trajectory_Container_Interface&>();
				typename TargetType::reverse_iterator itr;
				for(itr = path_container.rbegin(); itr != path_container.rend(); itr++)
				{
					_Trajectory_Unit_Interface* vehicle_trajectory_data=(_Trajectory_Unit_Interface*)Allocate<typename _Trajectory_Unit_Interface::Component_Type>();
					vehicle_trajectory_data->template Initialize<typename TargetType::Component_Type::unqualified_value_type*>((typename TargetType::Component_Type::unqualified_value_type*)*itr);
					//vehicle_trajectory_data->Initialize<_Link_Interface*>((_Link_Interface*)*itr);
					trajectory.push_back(vehicle_trajectory_data);
				}
			}

			feature_prototype void advance_trajectory()
			{
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);

				current_trajectory_position<int&>()++;
			}

			feature_prototype void initialize_trajectory()
			{
				current_trajectory_position<int>(-1);
			}
			
			feature_prototype void load()
			{
				this_component()->template load<ComponentType,CallerType,TargetType>();
			}

			feature_prototype TargetType next_link()
			{

				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				if((size_t)current_trajectory_position<int>() < trajectory_container<_Trajectory_Container_Interface&>().size() - 1)
				{
					_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)trajectory_container<_Trajectory_Container_Interface&>()[current_trajectory_position<int>() + 1];
					return trajectory_unit->template link<TargetType>();
				}
				else 
				{
					return nullptr;
				}
			}

			feature_prototype TargetType current_link()
			{
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);

				if(current_trajectory_position<int>() != -1)
				{
					return current_trajectory_position<_Trajectory_Unit_Interface*>()->template link<TargetType>();
				}
				else
				{
					return nullptr;
				}
			}

			feature_prototype void arrive_to_destination()
			{
				this_component()->template arrive_to_destination<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void transfer_to_next_link(int delayed_time)
			{
				this_component()->template transfer_to_next_link<ComponentType,CallerType,TargetType>(delayed_time);
			}

			feature_prototype TargetType get_current_link_enter_time()
			{
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				return (TargetType)current_trajectory_position<_Trajectory_Unit_Interface*>()->template enter_time<TargetType>();
			}

			feature_prototype int get_route_link_exit_time(int trajectory_unit_index)
			{
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				int route_link_exit_time = 0;
				if (trajectory_unit_index < trajectory_container<_Trajectory_Container_Interface&>().size()-1)
				{
					route_link_exit_time = trajectory_container<_Trajectory_Container_Interface&>()[trajectory_unit_index+1]->template enter_time<int>();
				}
				else
				{
					route_link_exit_time = trajectory_container<_Trajectory_Container_Interface&>()[trajectory_unit_index]->template enter_time<int>() + trajectory_container<_Trajectory_Container_Interface&>()[trajectory_unit_index]->template delayed_time<int>();
				}
				return route_link_exit_time;
			}
		};
	}
}

using namespace Movement_Plan_Components::Prototypes;