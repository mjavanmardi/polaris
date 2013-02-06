#ifndef NETWORK_NODE_CONTROL_DATA_H
#define NETWORK_NODE_CONTROL_DATA_H
#include "network_data.h"
#include "network_scenario_data.h"

#include "utilities.h"

using namespace std;
using namespace network_models::network_information::network_data_information;
using namespace network_models::network_information::scenario_data_information;

namespace network_models
{

	namespace network_information
	{
		namespace operation_data_information
		{
			enum Movement_Priority_Type_Keys
			{
				PROTECTED=1,  //high priority
				PERMITTED,    //yield the right-of-way to conflicting movements
			};
			enum Approach_Type_Keys
			{
				MAJOR_APPROACH=1,
				MINOR_APPROACH,
			};

			struct PhaseMovementData
			{
				int phase_movement_index;
				int turn_movement_index;	//from turn_movement_data

				Movement_Priority_Type_Keys movement_priority_type;	//
			};

			struct PhaseData
			{
				int phase_index;	// the index counter on all phases in the network
				int phase_sequence_number;	// 
				
				//starting time of green, yellow, and red
				int green_starting_time;
				int yellow_starting_time;
				int red_start_time;

				//actuated signal control
				int maximum_green_time;
				int minimum_green_time;

				//pre-timed signal control
				int offset;
				int green_time;

				//clearence time
				int yellow_time;
				int all_red_time;

				//adjusted green and yellow time
				int adjusted_green_time;
				int adjusted_yellow_time;

				//adjusted max and min green
				int adjusted_maximum_green_time;
				int adjusted_minimum_green_time;

				//movements
				int num_turn_movements_in_the_phase;
				vector<PhaseMovementData> turn_movements_in_the_phase_array;
			};

			struct ApproachData
			{
				int approach_index;
				Approach_Type_Keys approach_type;
				int inbound_link_index;
				float green_cycle_ratio;
			};

			struct ControlPlanData
			{
				int control_plan_index;

				//duration
				int starting_time;
				int ending_time;
				
				//type
				Node_Type_Keys control_type;

				//timing plan for signal control nodes
				int offset;
				int cycle_length;

				int cycle_index;
				int cycle_starting_time;
				int cycle_ending_time;
				int cycle_leftover_time;

				int num_phases;
				vector<PhaseData> phase_data_array;

				//approach data
				int num_approaches;
				vector<ApproachData> approach_data_array;

				map<int,int> link_approach_map;
				map<int,int>::iterator link_approach_map_itr;

				//two-way stop and yield sign
				int num_major_approaches;
				vector<int> major_approach_data_array;

				int num_minor_approaches;
				vector<int> minor_approach_data_array;
			};

			struct NodeControlData
			{
				int node_index;
				int num_control_plan_periods;

				int current_control_plan_index;

				vector<ControlPlanData> control_plan_data_array;
			};
		}
	}
}
#endif