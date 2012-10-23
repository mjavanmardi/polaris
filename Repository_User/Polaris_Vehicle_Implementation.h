#pragma once
#include "Repository_User_Includes.h"
#include "Repository_User\Vehicle_Prototype.h"
#include "Polaris_Link_Implementation.h"

namespace Vehicle_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		template<typename MasterType>
		struct Polaris_Trajectory_Unit_Implementation
		{
			member_data_basic(int, delayed_time);
			member_data_basic(int, enter_time);
			member_data_basic(int, enter_interval_index);
			
			//========================================================================================================
			//start implementation of member_data(LinkInterface*, link)
			//--------------------------------------------------------------------------------------------------------
			
			member_component_basic(typename MasterType::link_type, link);

			typedef typename MasterType::link_type link_type;

			//end
			//--------------------------------------------------------------------------------------------------------


		};

		template<typename MasterType>
		struct Polaris_Vehicle_Implementation
		{
			member_data_basic(int, current_trajectory_unit_index);
			member_data_basic(int, departed_simulation_interval_index);
			member_data_basic(int, departed_assignment_interval_index);
			member_component_basic(typename MasterType::link_type, origin_link);
			member_component_basic(typename MasterType::link_type, destination_link);
			member_component_basic(typename MasterType::link_type, next_link);
			member_data_basic(Vehicle_Components::Types::Vehicle_Status_Keys, simulation_status);
			member_data_basic(int, arrived_time);
			member_data_basic(int, arrived_simulation_interval_index);
			member_data_basic(int, current_link_enter_time);
			member_data_basic(int, current_link_enter_interval_index);
			member_data_basic(int, uuid);
			typedef Polaris_Component<Implementations::Polaris_Trajectory_Unit_Implementation, MasterType> trajectory_unit_type;
			member_container_basic(vector<trajectory_unit_type*>, trajectory_container);

			member_component_basic(typename MasterType::activity_location_type, origin_activity_location);

			member_component_basic(typename MasterType::activity_location_type, destination_activity_location);
		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Vehicle_Component{typedef Polaris_Component<Implementations::Polaris_Vehicle_Implementation, MasterType> type; };

		template<typename MasterType>
		struct Polaris_Trajectory_Unit_Component{typedef Polaris_Component<Implementations::Polaris_Trajectory_Unit_Implementation, MasterType> type; };
	}	
}