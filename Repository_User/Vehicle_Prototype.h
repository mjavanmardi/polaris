#pragma once
#include "Repository_User_Includes.h"

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
		template<typename ComponentType,typename CallerType>
		struct Trajectory_Unit_Prototype
		{
			feature_accessor(link);
			feature_accessor(delayed_time);
			feature_accessor(enter_time);
			feature_accessor(enter_interval_index);
		};

		template<typename ComponentType,typename CallerType>
		struct Vehicle_Prototype
		{
			//typedef typename ComponentType::trajectory_unit_type TrajectoryUnitType;
			//typedef typename ComponentType::trajectory_container_type TrajectoryContainerType;
			typedef ComponentType Component_Type;
			feature_accessor(trajectory_container);
			//feature_accessor(trajectory_position);
			feature_accessor(current_trajectory_unit_index);
			feature_accessor(departed_simulation_interval_index);
			feature_accessor(origin_link);
			feature_accessor(destination_link);
			//feature_accessor(next_link);
			feature_accessor(simulation_status);
			feature_accessor(arrived_time);
			feature_accessor(arrived_simulation_interval_index);
			feature_accessor(departed_assignment_interval_index);
			feature_accessor(origin_activity_location);
			feature_accessor(destination_activity_location);
			feature_accessor(current_link_enter_time);
			feature_accessor(current_link_enter_interval_index);
			feature_accessor(uuid);

			feature void set_route_links(TargetType& path_container/*,requires(TargetType,IsReversed)*/)
			{
				int route_link_size=(int)path_container.size();
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, trajectory_container, _Trajectory_Container_Interface, Trajectory_Unit_Prototype, _Trajectory_Unit_Interface, ComponentType);
				define_component_interface(_Link_Interface, Link_Prototype, _Trajectory_Unit_Interface_type::link, ComponentType);
				_Trajectory_Container_Interface& trajectory=trajectory_container<_Trajectory_Container_Interface&>();

				for(int i=route_link_size-1;i>=0;i--)
				{
					_Trajectory_Unit_Interface* vehicle_trajectory_data=(_Trajectory_Unit_Interface*)Allocate<_Trajectory_Unit_Interface_type>();
					vehicle_trajectory_data->link<_Link_Interface*>((_Link_Interface*)(path_container[i]));
					vehicle_trajectory_data->enter_time<int>(0);
					vehicle_trajectory_data->delayed_time<int>(0);

					//PRINT("\t\tPath Link: " << vehicle_trajectory_data->link<Link_Interface*>()->uuid<int>());
					trajectory.push_back(vehicle_trajectory_data);
				}
			};
			
			feature TargetType trajectory_position()
			{

				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, trajectory_container, _Trajectory_Container_Interface, Trajectory_Unit_Prototype, _Trajectory_Unit_Interface, ComponentType);
				if(current_trajectory_unit_index<int>()!=-1) return (TargetType)(trajectory_container<_Trajectory_Container_Interface&>()[current_trajectory_unit_index<int>()]);
				else return nullptr;
			}

			feature TargetType advance_trajectory()
			{
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, trajectory_container, _Trajectory_Container_Interface, Trajectory_Unit_Prototype, _Trajectory_Unit_Interface, ComponentType);

				current_trajectory_unit_index<int&>()++;

				return (TargetType)trajectory_container<_Trajectory_Container_Interface&>()[current_trajectory_unit_index<int>()];
			}
			
			feature void initialize_trajectory()
			{
				current_trajectory_unit_index<int>(-1);
				//return (TargetType)advance_trajectory<TargetType>();
			}

			feature void load_to_entry_queue()
			{
				simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>(Vehicle_Components::Types::Vehicle_Status_Keys::IN_ENTRY_QUEUE);
			};
			
			feature TargetType next_link()
			{

				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, trajectory_container, _Trajectory_Container_Interface, Trajectory_Unit_Prototype, _Trajectory_Unit_Interface, ComponentType);
				if((size_t)current_trajectory_unit_index<int>() < trajectory_container<_Trajectory_Container_Interface&>().size())
				{
					int index = current_trajectory_unit_index<int>() + 1;
					size_t size = trajectory_container<_Trajectory_Container_Interface&>().size();
					int vid = this->uuid<int>();
					_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)trajectory_container<_Trajectory_Container_Interface&>()[index];

					return trajectory_unit->link<TargetType>();
				}
				else 
				{
					return nullptr;
				}
			}

			feature TargetType current_link()
			{
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, trajectory_container, _Trajectory_Container_Interface, Trajectory_Unit_Prototype, _Trajectory_Unit_Interface, ComponentType);

				if(current_trajectory_unit_index<int>() != -1)
				{
					return trajectory_position<_Trajectory_Unit_Interface*>()->link<TargetType>();
				}
				else
				{
					return nullptr;
				}
			}

			feature void arrive_to_destination_link(int simulation_interval_index, int simulation_interval_length)
			{
				int current_time_in_seconds = simulation_interval_index*simulation_interval_length;

				simulation_status<Types::Vehicle_Status_Keys>(Types::Vehicle_Status_Keys::OUT_NETWORK);
				
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, trajectory_container, _Trajectory_Container_Interface, Trajectory_Unit_Prototype, _Trajectory_Unit_Interface, ComponentType);

				_Trajectory_Unit_Interface* current=trajectory_position<_Trajectory_Unit_Interface*>();
				
				current->delayed_time<int>(0);

				arrived_time<int>(current_time_in_seconds);
				arrived_simulation_interval_index<int>(simulation_interval_index);
			}
			
			feature void load_to_origin_link(int simulation_interval_index, int simulation_interval_length)
			{
				//int current_time_in_seconds = simulation_interval_index*simulation_interval_length;

				simulation_status<Types::Vehicle_Status_Keys>(Types::Vehicle_Status_Keys::IN_NETWORK);

				//typedef typename ComponentType::trajectory_unit_type Trajectory_Unit_Type;

				//Trajectory_Unit_Interface<Trajectory_Unit_Type,ComponentType>* current=initialize_trajectory<NULLTYPE>();
				
				initialize_trajectory<NULLTYPE>();

				//typedef Link_Interface<typename Trajectory_Unit_Type::link_type,ComponentType> Link_Interface;

				//PRINT("origin link " << current->link<Link_Interface*>()->uuid<int>());

				//current->enter_time<int>(current_time_in_seconds);
				//current->enter_interval_index<int>(simulation_interval_index);
			}
			
			feature void transfer_to_next_link(int simulation_interval_index,int simulation_interval_length,int delayed_time)
			{
				int current_time_in_seconds = simulation_interval_index*simulation_interval_length;

				//add exit time to trajectory data
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, trajectory_container, _Trajectory_Container_Interface, Trajectory_Unit_Prototype, _Trajectory_Unit_Interface, ComponentType);
				define_component_interface(_Link_Interface, Link_Prototype, _Trajectory_Unit_Interface_type::link, ComponentType);
				_Trajectory_Unit_Interface* current=trajectory_position<_Trajectory_Unit_Interface*>();

				if(current) current->delayed_time<int>(delayed_time);
				
				current = advance_trajectory<_Trajectory_Unit_Interface*>();
				
				//PRINT("transferring to next link " << current->link<Link_Interface*>()->uuid<int>());

				//add enter time to the trajectory data
				current->enter_time<int>(current_time_in_seconds);
				current->enter_interval_index<int>(simulation_interval_index);
			}

			feature int get_current_link_enter_time()
			{
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, trajectory_container, _Trajectory_Container_Interface, Trajectory_Unit_Prototype, _Trajectory_Unit_Interface, ComponentType);
				return trajectory_position<_Trajectory_Unit_Interface*>()->enter_time<int>();
			}
			
			feature int get_current_link_enter_interval_index()
			{
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, trajectory_container, _Trajectory_Container_Interface, Trajectory_Unit_Prototype, _Trajectory_Unit_Interface, ComponentType);
				return trajectory_position<_Trajectory_Unit_Interface*>()->enter_interval_index<int>();
			}
		};
	}
}

using namespace Vehicle_Components::Prototypes;