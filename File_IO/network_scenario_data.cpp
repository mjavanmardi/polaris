#include "network_scenario_data.h"

using namespace std;
using namespace network_models::network_information;

void scenario_data_information::scenario_data_initialization(ScenarioData& scenario_data)
{
	scenario_data.assignment_simulation_mode_int_string_map.clear();
	scenario_data.assignment_simulation_mode_int_string_map.insert(make_pair(ONE_SHOT_ASSIGNMENT_SIMULATION_MODE,"ONE_SHOT_ASSIGNMENT_SIMULATION_MODE"));
	scenario_data.assignment_simulation_mode_int_string_map.insert(make_pair(ITERATIVE_ASSIGNMENT_SIMULATION_MODE,"ITERATIVE_ASSIGNMENT_SIMULATION_MODE"));
	scenario_data.assignment_simulation_mode_int_string_map.insert(make_pair(CONVERT_DATA_MODE,"CONVERT_DATA_MODE"));

	scenario_data.assignment_simulation_mode_string_int_map.clear();
	scenario_data.assignment_simulation_mode_string_int_map.insert(make_pair("ONE_SHOT_ASSIGNMENT_SIMULATION_MODE",ONE_SHOT_ASSIGNMENT_SIMULATION_MODE));
	scenario_data.assignment_simulation_mode_string_int_map.insert(make_pair("ITERATIVE_ASSIGNMENT_SIMULATION_MODE",ITERATIVE_ASSIGNMENT_SIMULATION_MODE));
	scenario_data.assignment_simulation_mode_string_int_map.insert(make_pair("CONVERT_DATA_MODE",CONVERT_DATA_MODE));

	scenario_data.input_data_format_int_string_map.clear();
	scenario_data.input_data_format_int_string_map.insert(make_pair(INPUT_FROM_OD,"INPUT_FROM_OD"));
	scenario_data.input_data_format_int_string_map.insert(make_pair(INPUT_FROM_VEHICLE,"INPUT_FROM_VEHICLE"));
	scenario_data.input_data_format_int_string_map.insert(make_pair(INPUT_FROM_DSP,"INPUT_FROM_DSP"));
	scenario_data.input_data_format_int_string_map.insert(make_pair(INPUT_FROM_TSM,"INPUT_FROM_TSM"));
	scenario_data.input_data_format_int_string_map.insert(make_pair(INPUT_FROM_DB,"INPUT_FROM_DB"));

	scenario_data.input_data_format_string_int_map.clear();
	scenario_data.input_data_format_string_int_map.insert(make_pair("INPUT_FROM_OD",INPUT_FROM_OD));
	scenario_data.input_data_format_string_int_map.insert(make_pair("INPUT_FROM_VEHICLE",INPUT_FROM_VEHICLE));
	scenario_data.input_data_format_string_int_map.insert(make_pair("INPUT_FROM_DSP",INPUT_FROM_DSP));
	scenario_data.input_data_format_string_int_map.insert(make_pair("INPUT_FROM_TSM",INPUT_FROM_TSM));
	scenario_data.input_data_format_string_int_map.insert(make_pair("INPUT_FROM_DB",INPUT_FROM_DB));


	scenario_data.input_length_unit = LENGTH_IN_FOOT;
	scenario_data.input_length_unit_int_string_map.clear();
	scenario_data.input_length_unit_int_string_map.insert(make_pair(LENGTH_IN_FOOT,"LENGTH_IN_FOOT"));
	scenario_data.input_length_unit_int_string_map.insert(make_pair(LENGTH_IN_METER,"LENGTH_IN_METER"));
	scenario_data.input_length_unit_int_string_map.insert(make_pair(LENGTH_IN_MILE,"LENGTH_IN_MILE"));
	scenario_data.input_length_unit_int_string_map.insert(make_pair(LENGTH_IN_KILOMETER,"LENGTH_IN_KILOMETER"));

	scenario_data.input_length_unit_string_int_map.clear();
	scenario_data.input_length_unit_string_int_map.insert(make_pair("LENGTH_IN_FOOT",LENGTH_IN_FOOT));
	scenario_data.input_length_unit_string_int_map.insert(make_pair("LENGTH_IN_METER",LENGTH_IN_METER));
	scenario_data.input_length_unit_string_int_map.insert(make_pair("LENGTH_IN_MILE",LENGTH_IN_MILE));
	scenario_data.input_length_unit_string_int_map.insert(make_pair("LENGTH_IN_KILOMETER",LENGTH_IN_KILOMETER));

	scenario_data.input_speed_unit = SPEED_IN_MILES_PER_HOUR;
	scenario_data.input_speed_unit_int_string_map.clear();
	scenario_data.input_speed_unit_int_string_map.insert(make_pair(SPEED_IN_MILES_PER_HOUR,"SPEED_IN_MILES_PER_HOUR"));
	scenario_data.input_speed_unit_int_string_map.insert(make_pair(SPEED_IN_KILOMETERS_PER_HOUR,"SPEED_IN_KILOMETERS_PER_HOUR"));
	scenario_data.input_speed_unit_int_string_map.insert(make_pair(SPEED_IN_METERS_PER_SECOND,"SPEED_IN_METERS_PER_SECOND"));

	scenario_data.input_speed_unit_string_int_map.clear();
	scenario_data.input_speed_unit_string_int_map.insert(make_pair("SPEED_IN_MILES_PER_HOUR",SPEED_IN_MILES_PER_HOUR));
	scenario_data.input_speed_unit_string_int_map.insert(make_pair("SPEED_IN_KILOMETERS_PER_HOUR",SPEED_IN_KILOMETERS_PER_HOUR));
	scenario_data.input_speed_unit_string_int_map.insert(make_pair("SPEED_IN_METERS_PER_SECOND",SPEED_IN_METERS_PER_SECOND));

	scenario_data.input_time_unit = TIME_IN_MINUTE;
	scenario_data.input_time_unit_int_string_map.clear();
	scenario_data.input_time_unit_int_string_map.insert(make_pair(TIME_IN_MINUTE,"TIME_IN_MINUTE"));
	scenario_data.input_time_unit_int_string_map.insert(make_pair(TIME_IN_SECOND,"TIME_IN_SECOND"));
	scenario_data.input_time_unit_int_string_map.insert(make_pair(TIME_IN_HOUR,"TIME_IN_HOUR"));

	scenario_data.input_time_unit_string_int_map.clear();
	scenario_data.input_time_unit_string_int_map.insert(make_pair("TIME_IN_MINUTE",TIME_IN_MINUTE));
	scenario_data.input_time_unit_string_int_map.insert(make_pair("TIME_IN_SECOND",TIME_IN_SECOND));
	scenario_data.input_time_unit_string_int_map.insert(make_pair("TIME_IN_HOUR",TIME_IN_HOUR));

	scenario_data.input_flow_unit = FLOW_IN_VEHICLES_PER_HOUR_PER_LANE;
	scenario_data.input_flow_unit_int_string_map.clear();
	scenario_data.input_flow_unit_int_string_map.insert(make_pair(FLOW_IN_VEHICLES_PER_HOUR_PER_LANE,"FLOW_IN_VEHICLES_PER_HOUR_PER_LANE"));
	scenario_data.input_flow_unit_int_string_map.insert(make_pair(FLOW_IN_PCU_PER_HOUR_PER_LANE,"FLOW_IN_PCU_PER_HOUR_PER_LANE"));

	scenario_data.input_flow_unit_string_int_map.clear();
	scenario_data.input_flow_unit_string_int_map.insert(make_pair("FLOW_IN_VEHICLES_PER_HOUR_PER_LANE",FLOW_IN_VEHICLES_PER_HOUR_PER_LANE));
	scenario_data.input_flow_unit_string_int_map.insert(make_pair("FLOW_IN_PCU_PER_HOUR_PER_LANE",FLOW_IN_PCU_PER_HOUR_PER_LANE));

	scenario_data.input_density_unit = DENSITY_IN_VEHICLES_PER_MILE_PER_LANE;
	scenario_data.input_density_unit_int_string_map.clear();
	scenario_data.input_density_unit_int_string_map.insert(make_pair(DENSITY_IN_VEHICLES_PER_MILE_PER_LANE,"DENSITY_IN_VEHICLES_PER_MILE_PER_LANE"));
	scenario_data.input_density_unit_int_string_map.insert(make_pair(DENSITY_IN_VEHICLES_PER_KILOMETER_PER_LANE,"DENSITY_IN_VEHICLES_PER_KILOMETER_PER_LANE"));

	scenario_data.input_density_unit_string_int_map.clear();
	scenario_data.input_density_unit_string_int_map.insert(make_pair("DENSITY_IN_VEHICLES_PER_MILE_PER_LANE",DENSITY_IN_VEHICLES_PER_MILE_PER_LANE));
	scenario_data.input_density_unit_string_int_map.insert(make_pair("DENSITY_IN_VEHICLES_PER_KILOMETER_PER_LANE",DENSITY_IN_VEHICLES_PER_KILOMETER_PER_LANE));

};
void scenario_data_information::read_scenario_data_test(ScenarioData& scenario_data)
{
	scenario_data.simulation_interval_length = 6; //6 seconds
	scenario_data.assignment_interval_length = 50 * scenario_data.simulation_interval_length; // 5 minutes
	scenario_data.planning_horizon = 600 * scenario_data.simulation_interval_length; // 1 hour

	scenario_data.simulation_start_time = 0;
	scenario_data.simulation_end_time = scenario_data.simulation_start_time + scenario_data.planning_horizon;

	scenario_data.num_simulation_intervals = scenario_data.planning_horizon/scenario_data.simulation_interval_length;
	scenario_data.num_assignment_intervals = scenario_data.planning_horizon/scenario_data.assignment_interval_length;
	scenario_data.num_simulation_intervals_per_assignment_interval = scenario_data.assignment_interval_length/scenario_data.simulation_interval_length;

	scenario_data.assignment_mode = ONE_SHOT_ASSIGNMENT_SIMULATION_MODE;

	scenario_data.iseed = 1;

	scenario_data.num_threads = 1;
};


void scenario_data_information::read_scenario_data(string input_dir_name,ScenarioData& scenario_data)
{
	string filename = input_dir_name + "scenario";
	string line;
	ifstream input_file(filename);
	
	if (input_file.is_open())
	{
		int iline = 0;
		while (input_file.good())
		{
			getline(input_file,line);
			stringstream ss(line);
			string buf;
			vector<string> tokens;
			while (ss>>buf)
			{
				tokens.push_back(buf);
			}
			iline = iline + 1;
			if(iline == 2) // skip the first line
			{
				//0 - start_time	
				//1 - planning_horizon	
				//2 - assignment_interval_length	
				//3 - simulation_interval_length
				//4 - assignment_mode
				//5 - seed
				//6 - num_threads
				string assignment_mode_string;
				string simulation_start_time;
				string simulation_end_time;
				string input_format;
				int assignment_interval_length;
				for (int j=0;j<tokens.size();j++)
				{
					stringstream sss(tokens[j]);
					if (j==0) sss>>simulation_start_time;
					if (j==1) sss>>simulation_end_time;
					if (j==2) sss>>scenario_data.simulation_interval_length;
					if (j==3) sss>>assignment_interval_length;
					if (j==4) sss>>assignment_mode_string;
					if (j==5) sss>>scenario_data.iseed;
					if (j==6) sss>>scenario_data.num_threads;
					if (j==7) sss>>scenario_data.node_control_flag;
					if (j==8) sss>>scenario_data.demand_od_flag;
					if (j==9) sss>>input_format;
				}
				
				scenario_data.input_data_format = scenario_data.input_data_format_string_int_map[input_format];

				//simulation_start_time = simulation_start_time + ":00";
				//simulation_end_time = simulation_end_time + ":00";
				scenario_data.simulation_start_time = convert_hhmm_to_seconds(simulation_start_time);
				scenario_data.simulation_end_time = convert_hhmm_to_seconds(simulation_end_time);
				
				if (scenario_data.simulation_end_time <=scenario_data.simulation_start_time)
				{
					cout<< "error: the starting time is late than the ending time!" << endl;
				}

				scenario_data.assignment_interval_length = assignment_interval_length * scenario_data.simulation_interval_length;
				scenario_data.planning_horizon = scenario_data.simulation_end_time - scenario_data.simulation_start_time; //scenario_data.planning_horizon * 60;
					
				scenario_data.num_simulation_intervals = scenario_data.planning_horizon/scenario_data.simulation_interval_length;
				scenario_data.num_assignment_intervals = scenario_data.planning_horizon/scenario_data.assignment_interval_length;
				scenario_data.num_simulation_intervals_per_assignment_interval = scenario_data.assignment_interval_length/scenario_data.simulation_interval_length;

				scenario_data.assignment_mode = scenario_data.assignment_simulation_mode_string_int_map[assignment_mode_string];
			}

			if (iline == 4)
			{//unit
				string input_length_unit_string;
				string input_speed_unit_string;
				string input_time_unit_string;
				string input_flow_unit_string;
				string input_density_unit_string;
				for (int j=0;j<tokens.size();j++)
				{
					stringstream sss(tokens[j]);
					if (j==0) sss>>input_length_unit_string;
					if (j==1) sss>>input_speed_unit_string;
					if (j==2) sss>>input_time_unit_string;
					if (j==3) sss>>input_flow_unit_string;
					if (j==4) sss>>input_density_unit_string;
				}
				scenario_data.input_length_unit = scenario_data.input_length_unit_string_int_map[input_length_unit_string];
				scenario_data.input_speed_unit = scenario_data.input_speed_unit_string_int_map[input_speed_unit_string];
				scenario_data.input_time_unit = scenario_data.input_time_unit_string_int_map[input_time_unit_string];
				scenario_data.input_flow_unit = scenario_data.input_flow_unit_string_int_map[input_flow_unit_string];
				scenario_data.input_density_unit = scenario_data.input_density_unit_string_int_map[input_density_unit_string];
			}

		}
	}
	else
	{
		cout << "file " << filename << "cannot be opened!" << endl;
	}

};

void scenario_data_information::write_scenario_data(ScenarioData& scenario_data)
{
	//scenario
	string file_name = scenario_data.output_dir_name + "output_scenario";
	fstream file;
	file.open(file_name,fstream::out);
	if(file.is_open()) 
	{
		file 
			<< "starting_time_hh_mm" << "	"
			<< "ending_time_hh_mm" << "	"
			<< "simulation_interval_length_in_second" << "	"
			<< "num_simulation_intervals_per_assignment_interval" << "	"
			<< "assignment_mode" << "	"
			<< "seed" << "	"
			<< "num_threads" << "	"
			<< "node_control_flag" << "	"
			<< "demand_od_flag" << "	"
			<< "input_format"
			<<endl;
	}
	else
	{
		cout << "Cannot open file - "
			<< file_name
			<< endl;
	}

	file
		<< convert_seconds_to_hhmm(scenario_data.simulation_start_time) << "	"
		<< convert_seconds_to_hhmm(scenario_data.simulation_end_time) << "	"
		<< scenario_data.simulation_interval_length << "	"
		<< scenario_data.num_simulation_intervals_per_assignment_interval << "	"
		<< scenario_data.assignment_simulation_mode_int_string_map[ONE_SHOT_ASSIGNMENT_SIMULATION_MODE] << "	"
		<< scenario_data.iseed << "	"
		<< scenario_data.num_threads << "	"
		<< scenario_data.node_control_flag << "	"
		<< 0 <<"	"
		<< scenario_data.input_data_format_int_string_map[INPUT_FROM_VEHICLE];

	file.close();

};

void scenario_data_information::read_scenario_data_dsp(string input_dir_name,ScenarioData& scenario_data)
{
	scenario_data.simulation_interval_length = 6; //6 seconds
	scenario_data.assignment_interval_length = 50 * scenario_data.simulation_interval_length; // 5 minutes
	scenario_data.planning_horizon = 5*600 * scenario_data.simulation_interval_length; // 1 hour

	scenario_data.simulation_start_time = 0;
	scenario_data.simulation_end_time = scenario_data.simulation_start_time + scenario_data.planning_horizon;

	scenario_data.num_simulation_intervals = scenario_data.planning_horizon/scenario_data.simulation_interval_length;
	scenario_data.num_assignment_intervals = scenario_data.planning_horizon/scenario_data.assignment_interval_length;
	scenario_data.num_simulation_intervals_per_assignment_interval = scenario_data.assignment_interval_length/scenario_data.simulation_interval_length;

	scenario_data.assignment_mode = ONE_SHOT_ASSIGNMENT_SIMULATION_MODE;

	scenario_data.iseed = 1;

	scenario_data.num_threads = 1;

	scenario_data.node_control_flag = 0;
	scenario_data.demand_od_flag = 0;
};