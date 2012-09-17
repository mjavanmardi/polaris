#pragma once
#include "Repository_User_Includes.h"
#include "Repository_User\Vehicle_Interface.h"
#include "Polaris_Link_Base.h"

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
		template<typename MasterType>
		struct Polaris_Trajectory_Unit_Base
		{
			//========================================================================================================
			//start implementation of member_data(LinkInterface*, link)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType link(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_link);
			}

			tag_getter(link);
			
			typedef typename MasterType::link_type link_type;

		protected:
			void* _link;
			//end
			//--------------------------------------------------------------------------------------------------------

			member_data_basic(int, delayed_time);
			member_data_basic(int, enter_time);
			member_data_basic(int, enter_interval_index);
		};

		template<typename MasterType>
		struct Polaris_Vehicle_Base
		{
			member_data_basic(int, current_trajectory_unit_index);
			member_data_basic(int, departure_simulation_interval_index);
			member_data_basic(void*, origin_link);
			member_data_basic(void*, destination_link);
			member_data_basic(void*, next_link);
			member_data_basic(Vehicle_Components::Types::Vehicle_Status_Keys, simulation_status);
			member_data_basic(int, arrival_time);
			member_data_basic(int, arrival_simulation_interval_index);
			member_data_basic(int, current_link_enter_time);
			member_data_basic(int, current_link_enter_interval_index);

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType trajectory_container(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_trajectory_container);} tag_getter(trajectory_container);

			typedef Polaris_Component<Interfaces::Trajectory_Unit_Interface, Bases::Polaris_Trajectory_Unit_Base<MasterType>,NULLTYPE,MasterType> trajectory_unit_type;

			typedef vector<void*> trajectory_container_type;

			trajectory_container_type _trajectory_container;
		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Vehicle_Component
		{
			typedef Polaris_Component<Interfaces::Vehicle_Interface, Bases::Polaris_Vehicle_Base<MasterType>,NULLTYPE,MasterType> type;
		};

		template<typename MasterType>
		struct Polaris_Trajectory_Unit_Component
		{
			typedef Polaris_Component<Interfaces::Trajectory_Unit_Interface, Bases::Polaris_Trajectory_Unit_Base<MasterType>,NULLTYPE,MasterType> type;
		};
	}	
}