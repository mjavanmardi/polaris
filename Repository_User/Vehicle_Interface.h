#pragma once
#include "Repository_User_Includes.h"

namespace Vehicle_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Interfaces
	{
		template<typename ThisType,typename CallerType>
		struct Trajectory_Unit_Interface
		{
			facet_accessor(delayed_time);
			facet_accessor(enter_time);
			facet_accessor(enter_interval_index);
		};

		template<typename ThisType,typename CallerType>
		struct Vehicle_Interface
		{
			facet TargetType advance_trajectory()
			{

			}
			
			facet TargetType initialize_trajectory()
			{

			}

			facet_accessor(trajectory);
			facet_accessor(trajectory_position);
			facet_accessor(departure_simulation_interval_index);
			facet_accessor(destination_link);
			facet_accessor(next_link);
			facet_accessor(simulation_status);
			facet_accessor(arrival_time);
			facet_accessor(arrival_simulation_interval_index);

			facet void arrive_to_destination_link(int simulation_interval_index, int simulation_interval_length)
			{
				typedef typename ThisType::trajectory_position_type TrajectoryUnitType;
				int current_time_in_seconds = simulation_interval_index*simulation_interval_length;

				simulation_status<Simulation_Vehicle_Status_Keys>(OUT_NETWORK);
				
				Trajectory_Unit_Interface<TrajectoryUnitType,ThisType>* current=initialize_trajectory<Trajectory_Unit_Interface<TrajectoryUnitType,ThisType>*>();
				current->delayed_time<int>(0);

				arrival_time<int>(current_time_in_seconds);
				arrival_simulation_interval_index<int>(simulation_interval_index);
			}
			
			facet void load_to_origin_link(int simulation_interval_index, int simulation_interval_length)
			{
				typedef typename ThisType T;
				typedef typename T::trajectory_position_type TrajectoryUnitType;

				int current_time_in_seconds = simulation_interval_index*simulation_interval_length;

				simulation_status<Simulation_Vehicle_Status_Keys>(IN_NETWORK);

				Trajectory_Unit_Interface<TrajectoryUnitType,ThisType>* current=initialize_trajectory<Trajectory_Unit_Interface<TrajectoryUnitType,ThisType>*>();
				
				current->enter_time<int>(current_time_in_seconds);
				current->enter_interval_index<int>(simulation_interval_index);
			}
			
			facet void transfer_to_next_link(int current_simulation_interval_index,int simulation_interval_length,int delayed_time)
			{
				typedef typename ThisType T;
				typedef typename T::trajectory_position_type TrajectoryUnitType;
				int current_time_in_seconds = simulation_interval_index*simulation_interval_length;

				//add exit time to trajectory data
				Trajectory_Unit_Interface<TrajectoryUnitType,ThisType>* current = trajectory_position<Trajectory_Unit_Interface<TrajectoryUnitType,ThisType>*>();
				current->delayed_time<int>(delayed_time);
				
				current = advance_trajectory<Trajectory_Unit_Interface<TrajectoryUnitType,ThisType>*>();

				//add enter time to the trajectory data
				current->enter_time<int>(current_time_in_scecond);
				current->enter_interval_index<int>(simulation_interval_index);
			}
		};
	}
}