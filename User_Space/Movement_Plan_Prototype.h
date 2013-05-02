#pragma once
#include "User_Space_Includes.h"
#include "Activity_Location_Prototype.h"
#include "Link_Prototype.h"
#include "Zone_Prototype.h"

namespace Movement_Plan_Components
{
	namespace Types
	{
		struct Load_To_Entry_Queue{};
		struct Load_To_Origin_Link{};
	}

	namespace Concepts
	{	
		concept struct Is_Movement_Plan_Prototype
		{
			check_getter(has_trajectory, Component_Type::trajectory_container);
			check_getter(has_origin, Component_Type::origin);
			check_getter(has_destination, Component_Type::destination);
			check_getter(has_departed_time, Component_Type::departed_time);
			check_getter(has_arrived_time, Component_Type::arrived_time);

			define_default_check(has_trajectory && has_origin && has_destination && has_departed_time && has_arrived_time);
		};
		concept struct Is_Movement_Plan
		{
			check_getter(has_trajectory, trajectory_container);
			check_getter(has_origin, origin);
			check_getter(has_destination, destination);
			check_getter(has_departed_time, departed_time);
			check_getter(has_arrived_time, arrived_time);
			check_concept(is_prototype, Is_Movement_Plan_Prototype);
			define_default_check(is_prototype || (has_trajectory && has_origin && has_destination && has_departed_time && has_arrived_time));
		};

		concept struct Is_Movement_Record_Prototype
		{
			check_getter(has_trajectory, Component_Type::trajectory_container);
			define_default_check(has_trajectory);
		};
	}
	
	namespace Prototypes
	{

		prototype struct Trajectory_Unit_Prototype ADD_DEBUG_INFO
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


		prototype struct Movement_Plan_Prototype ADD_DEBUG_INFO
		{
			tag_as_prototype;

			feature_accessor(trajectory_container, none, none);

			feature_accessor(current_trajectory_position, none, none);
			feature_accessor(departed_time, none, none);
			feature_accessor(arrived_time, none, none);
			feature_accessor(planning_time, none, none);
			feature_accessor(plan, none, none);
			feature_accessor(valid_trajectory,none,none);
			feature_accessor(activity_reference,none,none);
			feature_accessor(routed_travel_time,none,none);
			feature_accessor(estimated_time_of_arrival,none,none);
			
			feature_accessor(absolute_departure_time, none, none);

			// overloaded origin and destination, depending on targetType
			feature_prototype void origin(TargetType activity_location, requires(check(TargetType,Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				this_component()->origin_location<ComponentType,CallerType,TargetType>(activity_location);
			}
			feature_prototype TargetType origin(requires(check(TargetType,Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				return this_component()->origin_location<ComponentType,CallerType,TargetType>();
			}
			feature_prototype void destination(TargetType activity_location, requires(check(TargetType,Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				this_component()->destination_location<ComponentType,CallerType,TargetType>(activity_location);
			}
			feature_prototype TargetType destination(requires(check(TargetType,Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				return this_component()->destination_location<ComponentType,CallerType,TargetType>();
			}
			feature_prototype void origin(TargetType link, requires(check(TargetType,Link_Components::Concepts::Is_Basic_Link)))
			{
				this_component()->origin<ComponentType,CallerType,TargetType>(link);
			}
			feature_prototype TargetType origin(requires(check(TargetType,Link_Components::Concepts::Is_Basic_Link)))
			{
				return this_component()->origin<ComponentType,CallerType,TargetType>();
			}
			feature_prototype void destination(TargetType link, requires(check(TargetType,Link_Components::Concepts::Is_Basic_Link)))
			{
				this_component()->destination<ComponentType,CallerType,TargetType>(link);
			}
			feature_prototype TargetType destination(requires(check(TargetType,Link_Components::Concepts::Is_Basic_Link)))
			{
				return this_component()->destination<ComponentType,CallerType,TargetType>();
			}
			feature_prototype void origin(TargetType zone, requires(check(TargetType,Zone_Components::Concepts::Is_Zone)))
			{
				this_component()->origin_zone<ComponentType,CallerType,TargetType>(zone);
			}
			feature_prototype TargetType origin(requires(check(TargetType,Zone_Components::Concepts::Is_Zone)))
			{
				return this_component()->origin_zone<ComponentType,CallerType,TargetType>();
			}
			feature_prototype void destination(TargetType link, requires(check(TargetType,Zone_Components::Concepts::Is_Zone)))
			{
				this_component()->destination_zone<ComponentType,CallerType,TargetType>(link);
			}
			feature_prototype TargetType destination(requires(check(TargetType,Zone_Components::Concepts::Is_Zone)))
			{
				return this_component()->destination_zone<ComponentType,CallerType,TargetType>();
			}

			feature_method_void(Initialize,none);
			feature_method_1_arg(Initialize,movement_to_copy, none);

			feature_prototype void set_trajectory(TargetType& path_container)
			{
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, typename get_type_of(trajectory_container), Back_Insertion_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				define_component_interface(_Link_Interface, typename _Trajectory_Unit_Interface::get_type_of(link), Link_Components::Prototypes::Link_Prototype, ComponentType);
				_Trajectory_Container_Interface& trajectory=trajectory_container<_Trajectory_Container_Interface&>();
				trajectory.clear();

				typename TargetType::reverse_iterator itr;
				for(itr = path_container.rbegin(); itr != path_container.rend(); itr++)
				{
					_Trajectory_Unit_Interface* vehicle_trajectory_data=(_Trajectory_Unit_Interface*)Allocate<typename _Trajectory_Unit_Interface::Component_Type>();
					vehicle_trajectory_data->template Initialize<typename TargetType::Component_Type::unqualified_value_type*>((typename TargetType::Component_Type::unqualified_value_type*)*itr);
					//vehicle_trajectory_data->Initialize<_Link_Interface*>((_Link_Interface*)*itr);
					trajectory.push_back(vehicle_trajectory_data);
				}
			}

			feature_prototype void update_trajectory(TargetType& path_container)
			{
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, typename get_type_of(trajectory_container), Back_Insertion_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				define_component_interface(_Link_Interface, typename _Trajectory_Unit_Interface::get_type_of(link), Link_Components::Prototypes::Link_Prototype, ComponentType);
				
				_Trajectory_Container_Interface& trajectory=trajectory_container<_Trajectory_Container_Interface&>();
				
				//erase 
				trajectory.erase(trajectory.begin() + current_trajectory_position<int&>() + 1,trajectory.end());
				
				typename TargetType::reverse_iterator itr;
				for(itr = path_container.rbegin() + 1; itr != path_container.rend(); itr++)
				{
					_Trajectory_Unit_Interface* vehicle_trajectory_data=(_Trajectory_Unit_Interface*)Allocate<typename _Trajectory_Unit_Interface::Component_Type>();
					vehicle_trajectory_data->template Initialize<typename TargetType::Component_Type::unqualified_value_type*>((typename TargetType::Component_Type::unqualified_value_type*)*itr);
					trajectory.push_back(vehicle_trajectory_data);
				}
			}

			feature_prototype void advance_trajectory()
			{
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, typename get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);

				current_trajectory_position<int&>()++;
			}

			feature_prototype void initialize_trajectory()
			{
				current_trajectory_position<int>(-1);
				valid_trajectory<bool>(false);
			}
			
			feature_prototype void load()
			{
				this_component()->template load<ComponentType,CallerType,TargetType>();
			}

			feature_prototype TargetType next_link()
			{

				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, typename get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
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
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, typename get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);

				if(current_trajectory_position<int>() != -1)
				{
					return current_trajectory_position<_Trajectory_Unit_Interface*>()->template link<TargetType>();
				}
				else
				{
					return nullptr;
				}
			}
			
			feature_prototype TargetType trajectory_size()
			{
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, typename get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				_Trajectory_Container_Interface* trajectory = this->trajectory_container<_Trajectory_Container_Interface*>();
				return (TargetType)(trajectory->size());
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
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, typename get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				return (TargetType)current_trajectory_position<_Trajectory_Unit_Interface*>()->template enter_time<TargetType>();
			}

			feature_prototype int get_route_link_exit_time(int trajectory_unit_index)
			{
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, typename get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
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