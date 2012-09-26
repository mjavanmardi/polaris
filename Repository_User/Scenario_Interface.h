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
	
	namespace Interfaces
	{
		template<typename ThisType,typename CallerType>
		struct Scenario_Interface
		{
			enum Assignment_Simulation_Mode_Keys
			{
				ONE_SHOT_ASSIGNMENT_SIMULATION_MODE=0,
				ITERATIVE_ASSIGNMENT_SIMULATION_MODE,
			};

			facet_accessor(current_simulation_interval_index);
			facet_accessor(simulation_interval_length);
			facet_accessor(assignment_interval_length);
			facet_accessor(current_time);

			facet_accessor(planning_horizon);
			facet_accessor(simulation_start_time);
			facet_accessor(simulation_end_time);
			
			facet_accessor(num_simulation_intervals);
			facet_accessor(num_assignment_intervals);
			facet_accessor(num_simulation_intervals_per_assignment_interval);

			facet_accessor(assignment_mode);

			facet_accessor(iseed);

			facet_accessor(output_writer);

			facet void read_scenario_data()
			{
				simulation_interval_length<int>(6); //6 seconds
				assignment_interval_length<int>(50*simulation_interval_length<int>()); // 5 minutes
				planning_horizon<int>(14400*simulation_interval_length<int>()); // 1 hour
				
				simulation_start_time<int>(0);
				simulation_end_time<int>(planning_horizon<int>()+simulation_start_time<int>());
				
				num_simulation_intervals<int>(planning_horizon<int>()/simulation_interval_length<int>());
				num_assignment_intervals<int>(planning_horizon<int>()/assignment_interval_length<int>());
				num_simulation_intervals_per_assignment_interval<int>(assignment_interval_length<int>()/simulation_interval_length<int>());
				
				assignment_mode<Assignment_Simulation_Mode_Keys>(ONE_SHOT_ASSIGNMENT_SIMULATION_MODE);
				
				iseed<int>(1);

				typedef typename ThisType::output_writer_type OutputWriterType;
				output_writer<OutputWriterType&>().open("Polaris_Output.log");
			}

			facet void output(std::string s)
			{
				typedef typename ThisType::output_writer_type OutputWriterType;
				//output_writer<OutputWriterType&>() << s;
				//cout << s;
			}
		};
	}
}

using namespace Scenario_Components::Interfaces;