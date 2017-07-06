#pragma once

#include "Core.h"



namespace Movement_Plan_Components
{
	namespace Concepts
	{	
		concept struct Is_Movement_Plan_Prototype
		{
			check_component_accessor_name(has_trajectory, trajectory_container);
			check_component_accessor_name(has_origin, origin);
			check_component_accessor_name(has_destination, destination);
			check_component_accessor_name(has_departed_time, departed_time);
			check_component_accessor_name(has_arrived_time, arrived_time);

			define_default_check(has_trajectory && has_origin && has_destination && has_departed_time && has_arrived_time);
		};

		concept struct Is_Movement_Plan
		{
			check_accessor_name(has_trajectory, trajectory_container);
			check_accessor_name(has_origin, origin);
			check_accessor_name(has_destination, destination);
			check_accessor_name(has_departed_time, departed_time);
			check_accessor_name(has_arrived_time, arrived_time);
			check_concept(is_prototype, Is_Movement_Plan_Prototype, T, V);
			define_default_check(is_prototype || (has_trajectory && has_origin && has_destination && has_departed_time && has_arrived_time));
		};

		concept struct Is_Movement_Record_Prototype
		{
			check_component_accessor_name(has_trajectory, trajectory_container);
			define_default_check(has_trajectory);
		};
	}
}


namespace Traveler_Components
{
	namespace Concepts
	{
		concept struct Is_Traveler_Prototype
		{
			check_component_accessor_name(has_router,router);
			check_component_accessor_name(has_vehicle,vehicle);
			define_default_check(has_router && has_vehicle);
		};
		concept struct Is_Traveler
		{
			check_accessor_name(has_router,router);
			check_accessor_name(has_vehicle,vehicle);
			check_concept(is_prototype, Is_Traveler_Prototype, T, V);
			define_default_check(is_prototype || (has_router && has_vehicle));
		};
	}
}

