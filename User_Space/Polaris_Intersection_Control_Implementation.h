#pragma once
#include "Intersection_Prototype.h"
#include "Intersection_Control_Prototype.h"


namespace Intersection_Control_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Polaris_Phase_Movement_Implementation
		{
			member_component(typename MasterType::movement_type, movement, none, none);
			member_data(Intersection_Control_Components::Types::Movement_Priority_Type_Keys, movement_priority_type, none, none);
		};

		implementation struct Polaris_Phase_Implementation
		{
			member_data(int, phase_index, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));	// the index counter on all phases in the network
			member_data(int, phase_sequence_number, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));	// 
				
			//starting time of green, yellow, and red
			member_data(int, green_starting_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, yellow_starting_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, red_start_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			//actuated signal control
			member_data(int, maximum_green_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, minimum_green_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			//pre-timed signal control
			member_data(int, offset, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, green_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			//clearence time
			member_data(int, yellow_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, all_red_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			//adjusted green and yellow time
			member_data(int, adjusted_green_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, adjusted_yellow_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			//adjusted max and min green
			member_data(int, adjusted_maximum_green_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, adjusted_minimum_green_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			//movements
			member_container(vector<typename MasterType::phase_movement_type*>, turn_movements_in_the_phase_array, none, none);
		};

		implementation struct Polaris_Approach_Implementation
		{
			member_data(int, approach_index, none, none);
			member_data(Intersection_Control_Components::Types::Approach_Type_Keys, approach_type, none, none);
			member_component(typename MasterType::link_type, inbound_link, none, none);
			member_data(float, green_cycle_ratio, none, none);
		};

		implementation struct Polaris_Control_Plan_Implementation
		{
			member_data(int, control_plan_index, none, none);

			//duration
			member_data(int, starting_time, none, none);
			member_data(int, ending_time, none, none);
			
			//type
			member_data(Intersection_Components::Types::Intersection_Type_Keys, control_type, none, none);

			//timing plan for signal control nodes
			member_data(int, offset, none, none);
			member_data(int, cycle_length, none, none);

			member_data(int, cycle_index, none, none);
			member_data(int, cycle_starting_time, none, none);
			member_data(int, cycle_ending_time, none, none);
			member_data(int, cycle_leftover_time, none, none);

			member_container(vector<typename MasterType::phase_type*>, phase_data_array, none, none);

			//approach data

			member_container(vector<typename MasterType::approach_type*>, approach_data_array, none, none);

			//two-way stop and yield sign
			member_container(vector<typename MasterType::approach_type*>, major_approach_data_array, none, none);

			member_container(vector<typename MasterType::approach_type*>, minor_approach_data_array, none, none);
		};

		implementation struct Polaris_Intersection_Control_Implementation
		{
			member_component(typename MasterType::intersection_type, intersection, none, none);
			member_component(typename MasterType::control_plan_type, current_control_plan, none, none);
			member_container(vector<typename MasterType::control_plan_type*>, control_plan_data_array, none, none);
			member_component(typename MasterType::scenario_type, scenario_reference, none, none);
		};
	}
}