#pragma once
//#include "Traffic_Simulator_Includes.h"
//#include "../File_IO/utilities.h"
#include "Network_Skimming_Prototype.h"


/*------------------------------------------------*/

namespace Routing_Components
{
	namespace Types
	{
	}

	namespace Prototypes
	{
		prototype struct Routing ADD_DEBUG_INFO
		{
			tag_as_prototype;

			void Schedule_Route_Computation(Simulation_Timestep_Increment time_to_depart, Simulation_Timestep_Increment planning_time = 0)
			{
				departure_time<Simulation_Timestep_Increment>(time_to_depart);
				this_component()->Schedule_Route_Computation(time_to_depart, planning_time);
			}

			template<typename Movement_Plan_Type>
			void Attach_New_Movement_Plan(Movement_Plan<Movement_Plan_Type>* mp)
			{
				this_component()->Attach_New_Movement_Plan<Movement_Plan_Type>(mp);
			}

			template<typename T> T Get_Tree_Results_For_Destination(int destination_internal_id, requires(T,check(strip_modifiers(T),Basic_Units::Concepts::Is_Time_Value)))
			{
				// get reference to travel times
				typedef Random_Access_Sequence<typename get_type_of(travel_times_to_link_container)> travel_times_itf;

				travel_times_itf* travel_times = this->travel_times_to_link_container<travel_times_itf*>();

				// return travel time to destion in requested time units
				return Time<Basic_Time>::Convert_Value<Simulation_Timestep_Increment, T>(travel_times->at(destination_internal_id));
			}
			
			accessor(parent_skimmer, NONE, NONE);
			accessor(network, NONE, NONE);
			accessor(origin_link, NONE,NONE);
			accessor(movement_plan, NONE, NONE);
			accessor(departure_time, NONE, NONE); // the time at which routing is triggered.
			accessor(update_increment, NONE, NONE);
			accessor(start_time, NONE, NONE);
			accessor(end_time, NONE, NONE);
			accessor(travel_times_to_link_container, NONE, NONE);
		};
	}
}

using namespace Routing_Components::Prototypes;
