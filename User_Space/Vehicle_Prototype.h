#pragma once
#include "User_Space_Includes.h"

/*
 * Needed for Linux compatability. Do not hurt Windows compilation.
 */
namespace Scenario_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Scenario_Prototype;
	}
};
namespace Intersection_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Intersection_Prototype;
	}
};
/*------------------------------------------------*/

namespace Vehicle_Components
{
	namespace Types
	{
		enum Vehicle_Type_Keys
		{
			SOV=0,
			HOV,
			TRUCK,
			BUS,
			RAIL,
		};

		enum Route_Choice_Use_Class_Keys
		{
			UNRESPONSIVE=0,
			ENROUTE_SWITCHING,
			USER_EQUILIBRIUM,
			SYSTEM_OPTIMAL,
		};
		
		enum Vehicle_Status_Keys
		{
			UNLOADED=0,
			IN_ENTRY_QUEUE,
			IN_NETWORK,
			OUT_NETWORK,
		};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Trajectory_Unit_Prototype
		{
			tag_as_prototype;

			feature_accessor(link, none, none);
			feature_accessor(delayed_time, none, none);
			feature_accessor(enter_time, none, none);
			feature_accessor(enter_interval_index, none, none);
		};

		prototype struct Vehicle_Prototype
		{
			tag_as_prototype;
			
			feature_accessor(trajectory_container, none, none);

			feature_accessor(current_trajectory_unit_index, none, none);
			feature_accessor(departed_simulation_interval_index, none, none);
			feature_accessor(origin_link, none, none);
			feature_accessor(destination_link, none, none);

			feature_accessor(simulation_status, none, none);
			feature_accessor(departed_time, none, none);
			feature_accessor(arrived_time, none, none);
			
			feature_accessor(arrived_simulation_interval_index, none, none);
			feature_accessor(departed_assignment_interval_index, none, none);
			feature_accessor(origin_activity_location, none, none);
			feature_accessor(destination_activity_location, none, none);
			feature_accessor(origin_zone, none, none);
			feature_accessor(destination_zone, none, none);
			feature_accessor(current_link_enter_time, none, none);
			feature_accessor(current_link_enter_interval_index, none, none);
			feature_accessor(uuid, none, none);
			feature_accessor(internal_id, none, none);

			feature_prototype void set_route_links(TargetType& path_container/*,requires(TargetType,IsReversed)*/)
			{
				int route_link_size=(int)path_container.size();
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Trajectory_Unit_Interface::get_type_of(link), Link_Components::Prototypes::Link_Prototype, ComponentType);
				_Trajectory_Container_Interface& trajectory=trajectory_container<_Trajectory_Container_Interface&>();

				for(int i=route_link_size-1;i>=0;i--)
				{
					_Trajectory_Unit_Interface* vehicle_trajectory_data=(_Trajectory_Unit_Interface*)Allocate<typename _Trajectory_Unit_Interface::Component_Type>();
					vehicle_trajectory_data->template link<_Link_Interface*>((_Link_Interface*)(path_container[i]));
					vehicle_trajectory_data->template enter_time<int>(0.0);
					vehicle_trajectory_data->template delayed_time<int>(0.0);
					trajectory.push_back(vehicle_trajectory_data);
				}
			};
			
			feature_prototype TargetType trajectory_position()
			{

				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				if(current_trajectory_unit_index<int>()!=-1) return (TargetType)(trajectory_container<_Trajectory_Container_Interface&>()[current_trajectory_unit_index<int>()]);
				else return nullptr;
			}

			feature_prototype TargetType advance_trajectory()
			{
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);

				current_trajectory_unit_index<int&>()++;

				return (TargetType)trajectory_container<_Trajectory_Container_Interface&>()[current_trajectory_unit_index<int>()];
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
			};

			
			feature_prototype void initialize_trajectory()
			{
				current_trajectory_unit_index<int>(-1);
			}

			feature_prototype void load_to_entry_queue()
			{
				simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>(Vehicle_Components::Types::Vehicle_Status_Keys::IN_ENTRY_QUEUE);
			};
			
			feature_prototype TargetType next_link()
			{

				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				if((size_t)current_trajectory_unit_index<int>() < trajectory_container<_Trajectory_Container_Interface&>().size())
				{
					int index = current_trajectory_unit_index<int>() + 1;
					size_t size = trajectory_container<_Trajectory_Container_Interface&>().size();
					int vid = this->template uuid<int>();
					_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)trajectory_container<_Trajectory_Container_Interface&>()[index];

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

				if(current_trajectory_unit_index<int>() != -1)
				{
					return trajectory_position<_Trajectory_Unit_Interface*>()->template link<TargetType>();
				}
				else
				{
					return nullptr;
				}
			}

			feature_prototype void arrive_to_destination_link(int simulation_interval_index, int simulation_interval_length)
			{
				int current_time_in_seconds = simulation_interval_index*simulation_interval_length;

				simulation_status<Types::Vehicle_Status_Keys>(Types::Vehicle_Status_Keys::OUT_NETWORK);
				
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);

				_Trajectory_Unit_Interface* current=trajectory_position<_Trajectory_Unit_Interface*>();
				
				current->template delayed_time<int>(0.0);

				arrived_time<int>(current_time_in_seconds);
				arrived_simulation_interval_index<int>(simulation_interval_index);
			}
			
			feature_prototype void load_to_origin_link(int simulation_interval_index, int simulation_interval_length)
			{
				simulation_status<Types::Vehicle_Status_Keys>(Types::Vehicle_Status_Keys::IN_NETWORK);

				initialize_trajectory<NULLTYPE>();

			}
			
			feature_prototype void transfer_to_next_link(int simulation_interval_index,int simulation_interval_length,int delayed_time)
			{
				int current_time_in_seconds = simulation_interval_index*simulation_interval_length;

				//add exit time to trajectory data
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				define_component_interface(_Link_Interface, _Trajectory_Unit_Interface::get_type_of(link), Link_Components::Prototypes::Link_Prototype, ComponentType);
				_Trajectory_Unit_Interface* current=trajectory_position<_Trajectory_Unit_Interface*>();

				if(current) current->template delayed_time<int>(delayed_time);
				
				current = advance_trajectory<_Trajectory_Unit_Interface*>();

				//add enter time to the trajectory data
				current->template enter_time<int>(current_time_in_seconds);
				current->template enter_interval_index<int>(simulation_interval_index);
			}

			feature_prototype int get_current_link_enter_time()
			{
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				return trajectory_position<_Trajectory_Unit_Interface*>()->template enter_time<int>();
			}
			
			feature_prototype int get_current_link_enter_interval_index()
			{
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				return trajectory_position<_Trajectory_Unit_Interface*>()->template enter_interval_index<int>();
			}

			feature_prototype void printStatus()
			{
				int p_uuid = uuid<int>();
				int p_current_link_enter_time = current_link_enter_time<int>();
				int p_current_link_enter_interval_index =current_link_enter_interval_index<int>();

			}


		};
	}
}

using namespace Vehicle_Components::Prototypes;