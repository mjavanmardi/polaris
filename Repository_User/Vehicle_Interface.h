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
	
	namespace Interfaces
	{
		template<typename ThisType,typename CallerType>
		struct Trajectory_Unit_Interface
		{
			facet_accessor(link);
			facet_accessor(delayed_time);
			facet_accessor(enter_time);
			facet_accessor(enter_interval_index);
		};

		template<typename ThisType,typename CallerType>
		struct Vehicle_Interface
		{
			//typedef typename ThisType::trajectory_unit_type TrajectoryUnitType;
			//typedef typename ThisType::trajectory_container_type TrajectoryContainerType;

			facet_accessor(trajectory_container);
			//facet_accessor(trajectory_position);
			facet_accessor(current_trajectory_unit_index);
			facet_accessor(departed_simulation_interval_index);
			facet_accessor(origin_link);
			facet_accessor(destination_link);
			//facet_accessor(next_link);
			facet_accessor(simulation_status);
			facet_accessor(arrived_time);
			facet_accessor(arrived_simulation_interval_index);
			facet_accessor(departed_assignment_interval_index);
			facet_accessor(origin_activity_location);
			facet_accessor(destination_activity_location);
			facet_accessor(current_link_enter_time);
			facet_accessor(current_link_enter_interval_index);
			facet_accessor(uuid);

			facet void set_route_links(TargetType& path_container/*,requires(TargetType,IsReversed)*/)
			{
				typedef typename ThisType::trajectory_type TrajectoryType;
				typedef typename ThisType::trajectory_element_type TrajectoryElementType;
				typedef typename TrajectoryElementType::link_type LinkType;
				typedef Link_Interface<LinkType,ThisType> Link_Interface;

				int route_link_size=(int)path_container.size();

				TrajectoryType& trajectory=trajectory_container<TrajectoryType&>();

				for(int i=route_link_size-1;i>=0;i--)
				{
					Trajectory_Unit_Interface<TrajectoryElementType,ThisType>* vehicle_trajectory_data=(Trajectory_Unit_Interface<TrajectoryElementType,ThisType>*)Allocate<TrajectoryElementType>();
					vehicle_trajectory_data->link<LinkType*>((LinkType*)(path_container[i]));
					vehicle_trajectory_data->enter_time<int>(0);
					vehicle_trajectory_data->delayed_time<int>(0);

					//PRINT("\t\tPath Link: " << vehicle_trajectory_data->link<Link_Interface*>()->uuid<int>());
					trajectory.push_back(vehicle_trajectory_data);
				}
			};
			
			facet TargetType trajectory_position()
			{
				typedef typename ThisType::trajectory_container_type trajectory_container_type;

				if(current_trajectory_unit_index<int>()!=-1) return (TargetType)(trajectory_container<trajectory_container_type&>()[current_trajectory_unit_index<int>()]);
				else return nullptr;
			}

			facet TargetType advance_trajectory()
			{
				typedef typename ThisType::trajectory_container_type trajectory_container_type;

				current_trajectory_unit_index<int&>()++;

				return (TargetType)trajectory_container<trajectory_container_type&>()[current_trajectory_unit_index<int>()];
			}
			
			facet void initialize_trajectory()
			{
				current_trajectory_unit_index<int>(-1);
				//return (TargetType)advance_trajectory<TargetType>();
			}

			facet void init()
			{
				current_trajectory_unit_index<int>(-1);
				simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>(Vehicle_Components::Types::Vehicle_Status_Keys::UNLOADED);
				trajectory<TrajectoryUnitContainerType>().clear();
				
			};
			
			/* not implementated due to dependency on RngStream class 
			facet void VehicleData::set_seed(int iseed)
			{
				this->g.SetSeed(vehicle_index+iseed+1);
			};
			*/

			facet void clear()
			{
				current_trajectory_unit_index<int>(-1);
				trajectory<TrajectoryUnitContainerType>().clear();
			};

			facet void load_to_entry_queue()
			{
				simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>(Vehicle_Components::Types::Vehicle_Status_Keys::IN_ENTRY_QUEUE);
			};
			
			facet TargetType next_link()
			{
				typedef typename ThisType::trajectory_container_type TrajectoryContainerType;
				typedef typename ThisType::trajectory_unit_type Trajectory_Unit_Type;
				typedef Trajectory_Unit_Interface<Trajectory_Unit_Type,ThisType> Trajectory_Unit_Interface;

				if(current_trajectory_unit_index<int>() < trajectory_container<TrajectoryContainerType&>().size())
				{
					return ((Trajectory_Unit_Interface*)trajectory_container<TrajectoryContainerType&>()[current_trajectory_unit_index<int>() + 1])->link<TargetType>();
				}
				else
				{
					return nullptr;
				}
			}

			facet TargetType current_link()
			{
				typedef typename ThisType::trajectory_container_type TrajectoryContainerType;
				
				typedef typename ThisType::trajectory_unit_type Trajectory_Unit_Type;

				if(current_trajectory_unit_index<int>() != -1)
				{
					return trajectory_position<Trajectory_Unit_Interface<Trajectory_Unit_Type,ThisType>*>()->link<TargetType>();
				}
				else
				{
					return nullptr;
				}
			}

			facet void arrive_to_destination_link(int simulation_interval_index, int simulation_interval_length)
			{
				int current_time_in_seconds = simulation_interval_index*simulation_interval_length;

				simulation_status<Types::Vehicle_Status_Keys>(Types::Vehicle_Status_Keys::OUT_NETWORK);
				
				typedef typename ThisType::trajectory_unit_type Trajectory_Unit_Type;

				Trajectory_Unit_Interface<Trajectory_Unit_Type,ThisType>* current=trajectory_position<Trajectory_Unit_Interface<Trajectory_Unit_Type,ThisType>*>();
				
				current->delayed_time<int>(0);

				arrived_time<int>(current_time_in_seconds);
				arrived_simulation_interval_index<int>(simulation_interval_index);
			}
			
			facet void load_to_origin_link(int simulation_interval_index, int simulation_interval_length)
			{
				//int current_time_in_seconds = simulation_interval_index*simulation_interval_length;

				simulation_status<Types::Vehicle_Status_Keys>(Types::Vehicle_Status_Keys::IN_NETWORK);

				//typedef typename ThisType::trajectory_unit_type Trajectory_Unit_Type;

				//Trajectory_Unit_Interface<Trajectory_Unit_Type,ThisType>* current=initialize_trajectory<NULLTYPE>();
				
				initialize_trajectory<NULLTYPE>();

				//typedef Link_Interface<typename Trajectory_Unit_Type::link_type,ThisType> Link_Interface;

				//PRINT("origin link " << current->link<Link_Interface*>()->uuid<int>());

				//current->enter_time<int>(current_time_in_seconds);
				//current->enter_interval_index<int>(simulation_interval_index);
			}
			
			facet void transfer_to_next_link(int simulation_interval_index,int simulation_interval_length,int delayed_time)
			{
				int current_time_in_seconds = simulation_interval_index*simulation_interval_length;

				//add exit time to trajectory data
				typedef typename ThisType::trajectory_unit_type Trajectory_Unit_Type;

				Trajectory_Unit_Interface<Trajectory_Unit_Type,ThisType>* current=trajectory_position<Trajectory_Unit_Interface<Trajectory_Unit_Type,ThisType>*>();

				if(current) current->delayed_time<int>(delayed_time);
				
				current = advance_trajectory<Trajectory_Unit_Interface<Trajectory_Unit_Type,ThisType>*>();
				
				typedef Link_Interface<typename Trajectory_Unit_Type::link_type,ThisType> Link_Interface;

				//PRINT("transferring to next link " << current->link<Link_Interface*>()->uuid<int>());

				//add enter time to the trajectory data
				current->enter_time<int>(current_time_in_seconds);
				current->enter_interval_index<int>(simulation_interval_index);
			}

			facet int get_current_link_enter_time()
			{
				return trajectory_position<TrajectoryUnitType>().enter_time<int>();
			}
			
			facet int get_current_link_enter_interval_index()
			{
				return trajectory_position<TrajectoryUnitType>().enter_interval_index<int>();
			}
		};
	}
}

using namespace Vehicle_Components::Interfaces;