#pragma once
#include "Scenario_Manager\cfg_reader.h"
#include "User_Space_Includes.h"


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
		prototype struct Scenario_Prototype
		{
			tag_as_prototype;

			enum Assignment_Simulation_Mode_Keys
			{
				ONE_SHOT_ASSIGNMENT_SIMULATION_MODE=0,
				ITERATIVE_ASSIGNMENT_SIMULATION_MODE,
			};

			feature_accessor(current_simulation_interval_index, none, none);
			feature_accessor(simulation_interval_length, none, none);
			feature_accessor(assignment_interval_length, none, none);
			feature_accessor(current_time, none, none);

			feature_accessor(planning_horizon, none, none);
			feature_accessor(simulation_start_time, none, none);
			feature_accessor(simulation_end_time, none, none);
			
			feature_accessor(num_simulation_intervals, none, none);
			feature_accessor(num_assignment_intervals, none, none);
			feature_accessor(num_simulation_intervals_per_assignment_interval, none, none);

			feature_accessor(assignment_mode, none, none);

			feature_accessor(iseed, none, none);

			feature_accessor(output_writer, none, none);

			feature_accessor(network_cumulative_loaded_vehicles, none, none);
			feature_accessor(network_cumulative_departed_vehicles, none, none);
			feature_accessor(network_in_network_vehicles, none, none);
			feature_accessor(network_cumulative_arrived_vehicles, none, none);

			feature_prototype void read_scenario_data()
			{
				CfgReader cfgReader;
				cfgReader.initialize("scenario.json");
				cfgReader.getParameter("simulation_interval_length", simulation_interval_length<int*>());
				//simulation_interval_length<int>(6); //6 seconds
				
				
				int assignment_intervals;
				cfgReader.getParameter("intervals_per_assignment", &assignment_intervals);
				assignment_interval_length<int>(assignment_intervals*simulation_interval_length<int>());
				//assignment_interval_length<int>(50*simulation_interval_length<int>()); // 5 minutes
				int planning_intervals;
				cfgReader.getParameter("planning_intervals", &planning_intervals);
				planning_horizon<int>(planning_intervals*simulation_interval_length<int>());
				//planning_horizon<int>(6000*simulation_interval_length<int>()); // 1 hour
				
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

			feature_prototype void output(std::string s)
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