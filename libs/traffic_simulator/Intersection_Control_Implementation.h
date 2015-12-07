#pragma once
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
		implementation struct Phase_Movement_Implementation:public Polaris_Component<MasterType,INHERIT(Phase_Movement_Implementation),Data_Object>
		{
			m_prototype(Null_Prototype,typename MasterType::movement_type, movement, NONE, NONE);
			m_data(Intersection_Control_Components::Types::Movement_Priority_Type_Keys, movement_priority_type, NONE, NONE);
		};

		implementation struct Phase_Implementation:public Polaris_Component<MasterType,INHERIT(Phase_Implementation),Data_Object>
		{
			// index counter on all phases in the network
			m_data(int, phase_index, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));	
			m_data(int, phase_sequence_number, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));	 
				
			//starting time of green, yellow, and red
			m_data(int, green_starting_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, yellow_starting_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, red_start_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			//actuated signal control
			m_data(int, maximum_green_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, minimum_green_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			//pre-timed signal control
			m_data(int, offset, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, green_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			//clearence time
			m_data(int, yellow_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, all_red_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			//adjusted green and yellow time
			m_data(int, adjusted_green_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, adjusted_yellow_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			//adjusted max and min green
			m_data(int, adjusted_maximum_green_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, adjusted_minimum_green_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			//movements
			m_container(std::vector<typename MasterType::phase_movement_type*>, turn_movements_in_the_phase_array, NONE, NONE);
		};

		implementation struct Approach_Implementation:public Polaris_Component<MasterType,INHERIT(Approach_Implementation),Data_Object>
		{
			m_data(int, approach_index, NONE, NONE);
			m_data(Intersection_Control_Components::Types::Approach_Type_Keys, approach_type, NONE, NONE);
			m_prototype(Null_Prototype,typename MasterType::link_type, inbound_link, NONE, NONE);
			m_data(float, green_cycle_ratio, NONE, NONE);
		};

		implementation struct Control_Plan_Implementation:public Polaris_Component<MasterType,INHERIT(Control_Plan_Implementation),Data_Object>
		{
			m_data(int, control_plan_index, NONE, NONE);

			//duration
			m_data(int, starting_time, NONE, NONE);
			m_data(int, ending_time, NONE, NONE);
			
			//type
			m_data(Intersection_Components::Types::Intersection_Type_Keys, control_type, NONE, NONE);

			//timing plan for signal control nodes
			m_data(int, offset, NONE, NONE);
			m_data(int, cycle_length, NONE, NONE);

			m_data(int, cycle_index, NONE, NONE);
			m_data(int, cycle_starting_time, NONE, NONE);
			m_data(int, cycle_ending_time, NONE, NONE);
			m_data(int, cycle_leftover_time, NONE, NONE);

			m_container(std::vector<typename MasterType::phase_type*>, phase_data_array, NONE, NONE);

			//approach data
			m_container(std::vector<typename MasterType::approach_type*>, approach_data_array, NONE, NONE);

			//two-way stop and yield sign
			m_container(std::vector<typename MasterType::approach_type*>, major_approach_data_array, NONE, NONE);

			m_container(std::vector<typename MasterType::approach_type*>, minor_approach_data_array, NONE, NONE);
		};

		implementation struct Intersection_Control_Implementation:public Polaris_Component<MasterType,INHERIT(Intersection_Control_Implementation),Execution_Object>
		{
			m_prototype(Null_Prototype,typename MasterType::intersection_type, intersection, NONE, NONE);
			m_prototype(Null_Prototype,typename MasterType::control_plan_type, current_control_plan, NONE, NONE);
			m_container(std::vector<typename MasterType::control_plan_type*>, control_plan_data_array, NONE, NONE);
			m_prototype(Null_Prototype,typename MasterType::network_type, network_reference, NONE, NONE);
		};
	}
}
