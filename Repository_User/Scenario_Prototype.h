#pragma once
#include "Repository_User_Includes.h"

namespace Scenario_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		template<typename ComponentType,typename CallerType>
		struct Scenario_Prototype
		{
			enum Assignment_Simulation_Mode_Keys
			{
				ONE_SHOT_ASSIGNMENT_SIMULATION_MODE=0,
				ITERATIVE_ASSIGNMENT_SIMULATION_MODE,
			};

			feature_accessor(current_simulation_interval_index);
			feature_accessor(simulation_interval_length);
			feature_accessor(assignment_interval_length);
			feature_accessor(current_time);

			feature_accessor(planning_horizon);
			feature_accessor(simulation_start_time);
			feature_accessor(simulation_end_time);
			
			feature_accessor(num_simulation_intervals);
			feature_accessor(num_assignment_intervals);
			feature_accessor(num_simulation_intervals_per_assignment_interval);

			feature_accessor(assignment_mode);

			feature_accessor(iseed);

			feature_accessor(output_writer);

			feature_accessor(network_cumulative_loaded_vehicles);
			feature_accessor(network_cumulative_departed_vehicles);
			feature_accessor(network_in_network_vehicles);
			feature_accessor(network_cumulative_arrived_vehicles);

			feature void read_scenario_data()
			{
				simulation_interval_length<int>(6); //6 seconds
				assignment_interval_length<int>(50*simulation_interval_length<int>()); // 5 minutes
				planning_horizon<int>(600*simulation_interval_length<int>()); // 1 hour
				
				simulation_start_time<int>(0);
				simulation_end_time<int>(planning_horizon<int>()+simulation_start_time<int>());
				
				num_simulation_intervals<int>(planning_horizon<int>()/simulation_interval_length<int>());
				num_assignment_intervals<int>(planning_horizon<int>()/assignment_interval_length<int>());
				num_simulation_intervals_per_assignment_interval<int>(assignment_interval_length<int>()/simulation_interval_length<int>());
				
				assignment_mode<Assignment_Simulation_Mode_Keys>(ONE_SHOT_ASSIGNMENT_SIMULATION_MODE);
				
				iseed<int>(1);

				typedef typename ComponentType::output_writer_type OutputWriterType;
				output_writer<OutputWriterType&>().open("Polaris_Output.log");

				network_cumulative_loaded_vehicles<int>(0);
				network_cumulative_departed_vehicles<int>(0);
				network_in_network_vehicles<int>(0);
				network_cumulative_arrived_vehicles<int>(0);
			}

			feature void output(std::string s)
			{
				typedef typename ComponentType::output_writer_type OutputWriterType;
				//output_writer<OutputWriterType&>() << s;
				cout << s;
				cout.flush();
			}
		};
	}
}

using namespace Scenario_Components::Prototypes;