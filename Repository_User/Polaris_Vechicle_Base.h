#pragma once
#include "Repository_User_Includes.h"
#include "Repository_User\Vehicle_Interface.h"
#include "Link_Interface.h"

namespace Vehicle_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Bases
	{

		struct Polaris_Trajectory_Unit_Base
		{
			
			//========================================================================================================
			//start implementation of memeber_data(LinkInterface*, link)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType link(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_link);
			}

			tag_getter(link);
			typedef Link_Components::Interfaces::Link_Interface<Link_Components::Components::Polaris_Link_Component, NULLTYPE>* link_type;
		protected:
			link_type _link;
			//end
			//--------------------------------------------------------------------------------------------------------

			member_data_basic(int, delayed_time);
			member_data_basic(int, enter_time);
			member_data_basic(int, enter_interval_index);
		};

		struct Polaris_Vehicle_Base
		{

			typedef Link_Components::Interfaces::Link_Interface<Link_Components::Components::Polaris_Link_Component, NULLTYPE>* link_type;

			//========================================================================================================
			//start implementation of memeber_data(vector<Trajectory_Unit_Interface*>, trajectory)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType trajectory(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_trajectory);
			}

			tag_getter(trajectory);
			typedef Vehicle_Components::Interfaces::Trajectory_Unit_Interface<Vehicle_Components::Components::Polaris_Trajectory_Unit_Component, NULLTYPE>* trajectory_unit_type;
			typedef vector<trajectory_unit_type> trajectory_container_type;
		protected:
			trajectory_container_type _trajectory;
			//end
			//--------------------------------------------------------------------------------------------------------

			//========================================================================================================
			//start implementation of memeber_data(Trajectory_Unit_Interface*, trajectory_position)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType trajectory_position(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_trajectory_position);
			}

			tag_getter(trajectory);
		protected:
			trajectory_unit_type _trajectory_position;
			//end
			//--------------------------------------------------------------------------------------------------------

			member_data_basic(int, current_trajectory_unit_index);
			member_data_basic(int, departure_simulation_interval_index);
			member_data(link_type, original_link);
			member_data(link_type, destination_link);
			member_data(link_type, next_link);
			member_data_basic(Vehicle_Components::Types::Vehicle_Status_Keys, simulation_status);
			member_data_basic(int, arrival_time);
			member_data_basic(int, arrival_simulation_interval_index);
		};
	}

	namespace Components
	{
		typedef Polaris_Component<Vehicle_Components::Interfaces::Vehicle_Interface, Vehicle_Components::Bases::Polaris_Vehicle_Base> Polaris_Vehicle_Component;
		typedef Polaris_Component<Vehicle_Components::Interfaces::Trajectory_Unit_Interface, Vehicle_Components::Bases::Polaris_Trajectory_Unit_Base> Polaris_Trajectory_Unit_Component;
		
	}	
}