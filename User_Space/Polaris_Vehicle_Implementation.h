#pragma once
#include "User_Space_Includes.h"
#include "Vehicle_Prototype.h"
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
		implementation struct Polaris_Trajectory_Unit_Implementation
		{
			member_data(int, delayed_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, enter_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, enter_interval_index, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			//========================================================================================================
			//start implementation of member_data(LinkInterface*, link)
			//--------------------------------------------------------------------------------------------------------
			
			member_component(typename MasterType::link_type, link, none, none);

			//end
			//--------------------------------------------------------------------------------------------------------


		};

		implementation struct Polaris_Vehicle_Implementation
		{
			member_data(int, current_trajectory_unit_index, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, departed_simulation_interval_index, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, departed_assignment_interval_index, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_component(typename MasterType::link_type, origin_link, none, none);
			member_component(typename MasterType::link_type, destination_link, none, none);
			member_component(typename MasterType::link_type, next_link, none, none);
			member_data(Vehicle_Components::Types::Vehicle_Status_Keys, simulation_status, none, none);
			member_data(int, departed_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, arrived_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, arrived_simulation_interval_index, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, current_link_enter_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, current_link_enter_interval_index, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);

			typedef Polaris_Component<Implementations::Polaris_Trajectory_Unit_Implementation, MasterType> trajectory_unit_type;
			member_container(vector<trajectory_unit_type*>, trajectory_container, none, none);

			member_component(typename MasterType::activity_location_type, origin_activity_location, none, none);

			member_component(typename MasterType::activity_location_type, destination_activity_location, none, none);

			member_component(typename MasterType::zone_type, origin_zone, none, none);

			member_component(typename MasterType::zone_type, destination_zone, none, none);
		};
	}

}