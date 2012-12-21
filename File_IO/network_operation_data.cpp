#include "network_operation_data.h"

using namespace std;
using namespace network_models::network_information;

void operation_data_information::opearation_data_initialization(OperationData& operation_data)
{
	operation_data.movement_priority_type_int_string_map.clear();
	operation_data.movement_priority_type_int_string_map.insert(make_pair(PROTECTED,"PROTECTED"));
	operation_data.movement_priority_type_int_string_map.insert(make_pair(PERMITTED,"PERMITTED"));

	operation_data.approach_type_string_int_map.clear();
	operation_data.approach_type_string_int_map.insert(make_pair("PROTECTED",PROTECTED));
	operation_data.approach_type_string_int_map.insert(make_pair("PERMITTED",PERMITTED));

	operation_data.approach_type_int_string_map.clear();
	operation_data.approach_type_int_string_map.insert(make_pair(MAJOR_APPROACH,"MAJOR_APPROACH"));
	operation_data.approach_type_int_string_map.insert(make_pair(MINOR_APPROACH,"MINOR_APPROACH"));

	operation_data.approach_type_string_int_map.clear();
	operation_data.approach_type_string_int_map.insert(make_pair("MAJOR_APPROACH",MAJOR_APPROACH));
	operation_data.approach_type_string_int_map.insert(make_pair("MINOR_APPROACH",MINOR_APPROACH));

};
void operation_data_information::read_operation_data(string input_dir_name,ScenarioData& scenario_data, OperationData& operation_data, NetworkData &network_data)
{
	//read node control data
	if (scenario_data.node_control_flag == 1)
	{
		read_node_control_data(input_dir_name, scenario_data, operation_data, network_data);
	}
	else
	{// no control
		//initialization
		operation_data.node_control_data_array.clear();
		operation_data.node_control_data_array.resize(network_data.network_node_size);
		operation_data.node_control_data_size = network_data.network_node_size;
		for (int node_index = 0; node_index<network_data.network_node_size;node_index++)
		{
			operation_data.node_control_data_array[node_index].num_control_plan_periods = 1;
			operation_data.node_control_data_array[node_index].node_index = node_index;
			operation_data.node_control_data_array[node_index].current_control_plan_index = 0;
			operation_data.node_control_data_array[node_index].control_plan_data_array.clear();
			operation_data.node_control_data_array[node_index].control_plan_data_array.resize(1);
			
			operation_data.node_control_data_array[node_index].control_plan_data_array[0].control_plan_index = 0;
			operation_data.node_control_data_array[node_index].control_plan_data_array[0].control_type = NO_CONTROL;
			operation_data.node_control_data_array[node_index].control_plan_data_array[0].starting_time = 0;
			operation_data.node_control_data_array[node_index].control_plan_data_array[0].ending_time = 24*60*60;

			operation_data.node_control_data_array[node_index].control_plan_data_array[0].num_approaches = 0;
			operation_data.node_control_data_array[node_index].control_plan_data_array[0].num_major_approaches = 0;
			operation_data.node_control_data_array[node_index].control_plan_data_array[0].num_minor_approaches = 0;
			operation_data.node_control_data_array[node_index].control_plan_data_array[0].num_phases = 0;
		}
	}
	//read sensor data
	//read_sensor_data(input_dir_name, operation_data, network_data);

};

void operation_data_information::read_node_control_data(string input_dir_name,ScenarioData& scenario_data, OperationData& operation_data, NetworkData &network_data)
{
	string filename = input_dir_name + "node_control";
	string line;
	ifstream input_file(filename);
	
	//initialization
	operation_data.node_control_data_array.clear();
	operation_data.node_control_data_array.resize(network_data.network_node_size);

	//read file
	if (input_file.is_open())
	{
		int iline = 0;
		int node_id;
		int read_flag = -1;

		//0 - new node
		int read_period_flag = -1;
		int read_type_flag = -1;
		int read_phases_flag = -1;
		int read_movements_flag = -1;
		int read_approaches_flag = -1;

		int read_periods = -1;
		int read_type = -1;
		int read_phases = -1;
		int read_movements = -1;
		int read_approaches = -1;

		vector<string> control_periods;
		
		ControlPlanData control_plan_data;
		int node_index;

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

			if(iline > 6) // skip the first 6 lines
			{

				if (read_flag == -1 && read_period_flag == -1)
				{//a new node
					//node - 1	
					//num_periods - 6
					//perods - 00:00:00	04:00:00	08:00:00	12:00:00	16:00:00	20:00:00
					control_periods.clear();
					
					for (int j=0;j<tokens.size();j++)
					{
						stringstream sss(tokens[j]);
						if (j==0) sss>>node_id;
						if (j==1) sss>>read_periods;
						if (j>=2) 
						{
							string period_start_time = sss.str();
							period_start_time = period_start_time;
							control_periods.push_back(period_start_time);
						}
					}

					node_index = network_data.node_id_index_map[node_id];
					operation_data.node_control_data_array[node_index].node_index = node_index;
					operation_data.node_control_data_array[node_index].num_control_plan_periods = read_periods;
					operation_data.node_control_data_array[node_index].control_plan_data_array.clear();
					operation_data.node_control_data_array[node_index].control_plan_data_array.resize(read_periods);
					for (int cp=0;cp<read_periods;cp++)
					{
						operation_data.node_control_data_array[node_index].control_plan_data_array[cp].control_plan_index = cp;
						operation_data.node_control_data_array[node_index].control_plan_data_array[cp].cycle_length = 0;
						operation_data.node_control_data_array[node_index].control_plan_data_array[cp].offset = 0;

						operation_data.node_control_data_array[node_index].control_plan_data_array[cp].starting_time = convert_hhmm_to_seconds(control_periods[cp]);
						if (cp == read_periods-1)
						{
							operation_data.node_control_data_array[node_index].control_plan_data_array[cp].ending_time = 24*60*60;
						}
						else
						{
							operation_data.node_control_data_array[node_index].control_plan_data_array[cp].ending_time = convert_hhmm_to_seconds(control_periods[cp+1]);
						}
					}

					read_flag = 1;
					read_period_flag = 0;
					read_type_flag = -1;
					read_phases_flag = -1;
					read_movements_flag = -1;
					read_approaches_flag = -1;

					if (read_flag == 1 && read_period_flag == read_periods)
					{//new node
						read_flag = -1;

						//0 - new node
						read_period_flag = -1;
						read_type_flag = -1;
						read_phases_flag = -1;
						read_movements_flag = -1;

						read_periods = -1;
						read_type = -1;
						read_phases = -1;
						read_movements = -1;
					}
					
					continue;
				}

				if (read_flag == 1 && read_period_flag <read_periods && read_period_flag >=0)
				{//read a control plan line
					//period - 1	
					//control type - PRE_TIMED_SIGNAL_CONTROL	
					//number of phases (or number of major approaches or 0) - 4
					//cycle length (or number of minor approaches or 0) - 120	
					if (read_type_flag == -1 && read_phases_flag == -1 && read_movements_flag == -1)
					{//type line
						
						int control_plan_id;
						string control_type_string;
						int param2 = 0;
						int param3 = 0;
						for (int j=0;j<tokens.size();j++)
						{
							stringstream sss(tokens[j]);
							if (j==0) sss>>control_plan_id;
							if (j==1) sss>>control_type_string;
							if (j==2) sss>>param2;	//num_phases or num_major_approaches
							if (j==3) sss>>param3; //cycle length or num_minor_approaches
						}

						//approach data
						operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].approach_data_array.resize(network_data.node_data_array[node_index].inbound_link_size);
						operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_approaches = network_data.node_data_array[node_index].inbound_link_size;

						for (int inbound_link=0;inbound_link<network_data.node_data_array[node_index].inbound_link_size;inbound_link++)
						{
							int inbound_link_index = network_data.node_data_array[node_index].inbound_link_index_array[inbound_link];
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].approach_data_array[inbound_link].approach_index = inbound_link;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].approach_data_array[inbound_link].inbound_link_index = inbound_link_index;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].approach_data_array[inbound_link].green_cycle_ratio = 0.0f;

							//map
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].link_approach_map.insert(make_pair(inbound_link_index,inbound_link));
						}

						if (control_type_string == "NO_CONTROL")
						{
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type = NO_CONTROL;
								
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].cycle_leftover_time = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].cycle_length = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_phases = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array.clear();

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_major_approaches = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_minor_approaches = 0;
								
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].major_approach_data_array.clear();
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].minor_approach_data_array.clear();
								
							read_phases_flag = -1;
							read_approaches_flag = -1;
						}

						if (control_type_string == "YIELD_SIGN")
						{
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type = YIELD_SIGN;

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].cycle_leftover_time = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].cycle_length = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_phases = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array.clear();

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_major_approaches = param2;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_minor_approaches = param3;
								
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].major_approach_data_array.clear();
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].major_approach_data_array.resize(operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_major_approaches);

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].minor_approach_data_array.clear();
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].minor_approach_data_array.resize(operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_minor_approaches);

							read_phases_flag = -1;
							read_approaches_flag = 0;

						}

						if (control_type_string == "ALL_WAY_STOP_SIGN")
						{
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type = ALL_WAY_STOP_SIGN;

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].cycle_leftover_time = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].cycle_length = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_phases = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array.clear();

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_major_approaches = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_minor_approaches = 0;
								
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].major_approach_data_array.clear();
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].minor_approach_data_array.clear();

							read_phases_flag = -1;
							read_approaches_flag = -1;

						}

						if (control_type_string == "TWO_WAY_STOP_SIGN")
						{
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type = TWO_WAY_STOP_SIGN;

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].cycle_leftover_time = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].cycle_length = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_phases = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array.clear();

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_major_approaches = param2;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_minor_approaches = param3;
								
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].major_approach_data_array.clear();
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].major_approach_data_array.resize(operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_major_approaches);

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].minor_approach_data_array.clear();
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].minor_approach_data_array.resize(operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_minor_approaches);

							read_phases_flag = -1;
							read_approaches_flag = 0;

						}

						if (control_type_string == "PRE_TIMED_SIGNAL_CONTROL")
						{
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type = PRE_TIMED_SIGNAL_CONTROL;

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].cycle_leftover_time = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].cycle_length = param3;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_phases = param2;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array.clear();
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array.resize(operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_phases);

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_major_approaches = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_minor_approaches = 0;
								
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].major_approach_data_array.clear();
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].minor_approach_data_array.clear();

							read_phases_flag = 0;
							read_approaches_flag = -1;

						}

						if (control_type_string == "ACTUATED_SIGNAL_CONTROL")
						{
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type = ACTUATED_SIGNAL_CONTROL;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].cycle_leftover_time = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].cycle_length = param3;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_phases = param2;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array.clear();
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array.resize(operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_phases);

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_major_approaches = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_minor_approaches = 0;
								
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].major_approach_data_array.clear();
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].minor_approach_data_array.clear();

							read_phases_flag = 0;
							read_approaches_flag = -1;

						}
				
						if (control_type_string == "ADAPTIVE_SIGNAL_CONTROL")
						{
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type = ADAPTIVE_SIGNAL_CONTROL;

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].cycle_leftover_time = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].cycle_length = param3;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_phases = param2;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array.clear();
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array.resize(operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_phases);

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_major_approaches = 0;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_minor_approaches = 0;
								
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].major_approach_data_array.clear();
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].minor_approach_data_array.clear();
								
							read_phases_flag = 0;
							read_approaches_flag = -1;
						}



						read_type_flag = param2;
						read_movements_flag = -1;

						//ALL_WAY_STOP_SIGN and NO_CONTROL
						if ((operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type == ALL_WAY_STOP_SIGN
							|| operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type == NO_CONTROL
							))
						{
							read_approaches_flag = -1;
							read_phases_flag = -1;
							read_type_flag = -1; 
							read_period_flag = read_period_flag + 1;
						}

						if (read_flag == 1 && read_period_flag == read_periods)
						{//new node
							read_flag = -1;

							//0 - new node
							read_period_flag = -1;
							read_type_flag = -1;
							read_phases_flag = -1;
							read_movements_flag = -1;

							read_periods = -1;
							read_type = -1;
							read_phases = -1;
							read_movements = -1;
						}

						continue;
					}

					//read signal
					if (read_period_flag >=0 
						&& read_type_flag>0 
						&& read_phases_flag>=0 
						&& read_approaches_flag == -1)
					{
						if (read_movements_flag == -1 
							&& read_phases_flag< operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_phases
							&& (operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type == PRE_TIMED_SIGNAL_CONTROL || 
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type == ACTUATED_SIGNAL_CONTROL))
						{//type line
							int param0,param1,param2,param3,param4,param5,param6;
							for (int j=0;j<tokens.size();j++)
							{
								stringstream sss(tokens[j]);
								if (j==0) sss>>param0;	//phase_id
								if (j==1) sss>>param1;	//phase_num
								if (j==2) sss>>param2;	//num_movements
								if (j==3) sss>>param3;	//off_set or min_green
								if (j==4) sss>>param4;	//green_time or max_green
								if (j==5) sss>>param5;	//yellow_time
								if (j==6) sss>>param6;	//all_red_time
							}
						
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].phase_index = read_phases_flag;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].phase_sequence_number = param1;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].all_red_time = param6;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].yellow_time = param5;

							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].num_turn_movements_in_the_phase = param2;
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].turn_movements_in_the_phase_array.clear();
							operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].turn_movements_in_the_phase_array.resize(param2);

							if (operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type == PRE_TIMED_SIGNAL_CONTROL)
							{
								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].green_time = param4;
								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].offset = param3;

								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].maximum_green_time = 0;
								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].minimum_green_time = 0;

								//adjusted green and yellow time
								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].adjusted_yellow_time = 
									operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].yellow_time - int(0.5*operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].yellow_time);

								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].adjusted_green_time =
									operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].green_time + 
									(operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].yellow_time - operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].adjusted_yellow_time);

								if (read_phases_flag == 0)
								{
									operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].offset = param3;
								}
							}

							if (operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type == ACTUATED_SIGNAL_CONTROL)
							{
								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].minimum_green_time = param3;
								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].maximum_green_time = param4;

								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].green_time = 0;
								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].offset = 0;

								//adjusted minimum and maximum green time, and yellow time
								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].adjusted_yellow_time = 
									operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].yellow_time - int(0.5*operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].yellow_time);
								
								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].adjusted_maximum_green_time = 
									operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].maximum_green_time + 
									(operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].yellow_time - operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].adjusted_yellow_time);

								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].adjusted_minimum_green_time = 
									operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].minimum_green_time + 
									(operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].yellow_time - operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].adjusted_yellow_time);

							}

							
							read_movements_flag = 0;

							if (read_flag == 1 && read_period_flag == read_periods)
							{//new node
								read_flag = -1;

								//0 - new node
								read_period_flag = -1;
								read_type_flag = -1;
								read_phases_flag = -1;
								read_movements_flag = -1;

								read_periods = -1;
								read_type = -1;
								read_phases = -1;
								read_movements = -1;
							}
							continue;
						}

						//read movements
						if (read_phases_flag>=0 
							&& read_phases_flag< operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_phases)
						{
							if (read_movements_flag >= 0 
								&& read_movements_flag< operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].num_turn_movements_in_the_phase
								&& (operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type == PRE_TIMED_SIGNAL_CONTROL || 
								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type == ACTUATED_SIGNAL_CONTROL))
							{
								int inbound_link_id;
								int outbound_link_id;
								string movement_type_string;
								string movement_priority_string;

								for (int j=0;j<tokens.size();j++)
								{
									stringstream sss(tokens[j]);
									if (j==0) sss>>movement_type_string;	
									if (j==1) sss>>inbound_link_id;	
									if (j==2) sss>>outbound_link_id;
									if (j==3) sss>>movement_priority_string;
								}

								int inbound_link_index = network_data.link_id_index_map[inbound_link_id];
								int outbound_link_index = network_data.link_id_index_map[outbound_link_id];
								
								int turn_movement_index = network_data.link_turn_movement_map[make_pair(inbound_link_index,outbound_link_index)];
								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].turn_movements_in_the_phase_array[read_movements_flag].phase_movement_index = read_movements_flag + 1;
								operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].turn_movements_in_the_phase_array[read_movements_flag].turn_movement_index = turn_movement_index;
								
								if (movement_priority_string=="PROTECTED")
								{
									operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].turn_movements_in_the_phase_array[read_movements_flag].movement_priority_type = PROTECTED;
								}

								if (movement_priority_string=="PERMITTED")
								{
									operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].turn_movements_in_the_phase_array[read_movements_flag].movement_priority_type = PERMITTED;
								}

								//
								if (read_movements_flag == operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].phase_data_array[read_phases_flag].num_turn_movements_in_the_phase - 1)
								{
									read_movements_flag = -1;
									if (read_phases_flag == operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].num_phases - 1)
									{
										read_phases_flag = -1;
										read_type_flag = -1; 
										read_approaches_flag = -1;
										read_period_flag = read_period_flag + 1;
									}
									else
									{
										read_phases_flag = read_phases_flag + 1;
									}
								}
								else
								{
									read_movements_flag = read_movements_flag + 1;
								}


								if (read_flag == 1 && read_period_flag == read_periods)
								{//new node
									read_flag = -1;

									//0 - new node
									read_period_flag = -1;
									read_type_flag = -1;
									read_phases_flag = -1;
									read_movements_flag = -1;

									read_periods = -1;
									read_type = -1;
									read_phases = -1;
									read_movements = -1;
								}
								continue;
							}
						}
					}

					//read TWO_WAY_STOP_SIGN and YIELD_SIGN
					if (read_period_flag >=0 
						&& read_type_flag>0 
						&& read_phases_flag == -1 
						&& read_approaches_flag >= 0)
					{
						if (read_approaches_flag<=1
							&&(operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type == TWO_WAY_STOP_SIGN
							|| operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].control_type == YIELD_SIGN
							))
						{
							int link_id;
							int link_index;
							for (int j=0;j<tokens.size();j++)
							{
								stringstream sss(tokens[j]);
								sss>> link_id;
								link_index = network_data.link_id_index_map[link_id];

								int approach_index = operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].link_approach_map[link_index];

								if (read_approaches_flag == 0)
								{//major approach
									operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].approach_data_array[approach_index].approach_type = MAJOR_APPROACH;
									operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].major_approach_data_array[j] = approach_index;
								}
								if (read_approaches_flag == 1)
								{//minor approach
									
									operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].approach_data_array[approach_index].approach_type = MINOR_APPROACH;
									operation_data.node_control_data_array[node_index].control_plan_data_array[read_period_flag].minor_approach_data_array[j] = approach_index;
								}

							}

							if (read_approaches_flag == 1)
							{
								read_approaches_flag = -1;
								read_phases_flag = -1;
								read_type_flag = -1; 
								read_period_flag = read_period_flag + 1;
							}
							else
							{
								read_approaches_flag = read_approaches_flag + 1;
							}

							if (read_flag == 1 && read_period_flag == read_periods)
							{//new node
								read_flag = -1;

								//0 - new node
								read_period_flag = -1;
								read_type_flag = -1;
								read_phases_flag = -1;
								read_movements_flag = -1;

								read_periods = -1;
								read_type = -1;
								read_phases = -1;
								read_movements = -1;
							}
							continue;
						}
					}
				}
			}
		}
	}
	else
	{
		cout << "file " << filename << "cannot be opened!" << endl;
	}

	operation_data.node_control_data_size = (int) operation_data.node_control_data_array.size();
	
	operation_data.turn_movement_green_time_array.clear();
	operation_data.turn_movement_green_time_array.resize(network_data.network_turn_movement_size);
};

void operation_data_information::read_sensor_data(string input_dir_name, OperationData & operation_data, NetworkData &network_data)
{
	string filename = input_dir_name + "sensor";
	string line;
	ifstream input_file(filename);
	
	//initialization
	operation_data.sensor_data_array.clear();

	//read file
	int sensor_index = -1;
	if (input_file.is_open())
	{
		int iline = 0;
		SensorData sensor_data;

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
			if(iline >= 2) // skip the first line
			{
				//0 - sensor id
				//1 - link id
				//2 - distance to downstream node
				//3 - type

				string sensor_type_string;
				int sensor_id;
				int link_id;
				float distance_to_downstream_node;

				for (int j=0;j<tokens.size();j++)
				{
					stringstream sss(tokens[j]);
					if (j==0) sss>>sensor_id;
					if (j==1) sss>>link_id;
					if (j==2) sss>>distance_to_downstream_node;
					if (j==3) sss>>sensor_type_string;
				}
				int link_index = network_data.link_id_index_map[link_id];

				sensor_data.sensor_id = sensor_id;
				sensor_data.link_index = link_index;
				sensor_data.distance_to_downstream_node = distance_to_downstream_node;

				if (sensor_type_string == "PRESENCE")
				{
					sensor_data.sensor_type = PRESENCE;
				}
				if (sensor_type_string == "PASSEGE")
				{
					sensor_data.sensor_type = PASSEGE;
				}

				sensor_index = sensor_index + 1;
				sensor_data.sensor_index = sensor_index;

				operation_data.sensor_data_array.push_back(sensor_data);
			}
		}
		operation_data.sensor_data_size = int(operation_data.sensor_data_array.size());
	}
	else
	{
		cout << "file " << filename << "cannot be opened!" << endl;
	}

};

void operation_data_information::write_operation_data(string output_dir_name,ScenarioData& scenario_data, OperationData & operation_data, NetworkData &network_data)
{
	write_node_control_data(output_dir_name,scenario_data, operation_data, network_data);
};

void operation_data_information::write_node_control_data(string output_dir_name,ScenarioData& scenario_data, OperationData & operation_data, NetworkData &network_data)
{
	//activity_location
	string file_name = output_dir_name + "output_node_control";
	fstream file;
	file.open(file_name,fstream::out);
	if(file.is_open()) 
	{
		file << "node	num_timing_plan	start_time_tp_1_hh_mm	start_time_tp_2	..." << endl;
		file << "tp_1	control_type	num_phases(or num_major_approach_links)	cycle_length(or num_minor_approach_links)"  << endl;
		file << "phase_1	phase_num	num_movements	off_set(or min_green)	green_time(or max_green)	yellow_time	all_red_time"  << endl;
		file << "movement_1	type	inbound_link	outbound_link	priority (PROTECTED|PERMITTED)"  << endl;
		file << "major_approach_links (for two_way_stop_sign and yield_sign)"  << endl;
		file << "minor_approach_links (for two_way_stop_sign and yield_sign)"  << endl;
	}
	else
	{
		cout << "Cannot open file - "
			<< file_name
			<< endl;
	}

	for (int i=0;i<operation_data.node_control_data_size;i++)
	{
		//control plans
		file << network_data.node_data_array[operation_data.node_control_data_array[i].node_index].uuid << "	";
		file << operation_data.node_control_data_array[i].num_control_plan_periods << "	";
		for (int j=0;j<operation_data.node_control_data_array[i].num_control_plan_periods;j++)
		{	
			if (j < operation_data.node_control_data_array[i].num_control_plan_periods - 1)
			{
				file << convert_seconds_to_hhmm(operation_data.node_control_data_array[i].control_plan_data_array[j].starting_time) << "	" ;
			}
			else
			{
				file << convert_seconds_to_hhmm(operation_data.node_control_data_array[i].control_plan_data_array[j].starting_time) << endl;
			}
		}

		//control plan j
		for (int j=0;j<operation_data.node_control_data_array[i].num_control_plan_periods;j++)
		{
			if (operation_data.node_control_data_array[i].control_plan_data_array[j].control_type == NO_CONTROL ||
				operation_data.node_control_data_array[i].control_plan_data_array[j].control_type == ALL_WAY_STOP_SIGN)
			{
				int tmp = 0;
				file 
					<< operation_data.node_control_data_array[i].control_plan_data_array[j].control_plan_index + 1 << "	"
					<< network_data.node_type_int_string_map[operation_data.node_control_data_array[i].control_plan_data_array[j].control_type] << "	"
					<< tmp << "	"
					<< tmp;
				
				if (i<operation_data.node_control_data_size - 1)
				{
					file << endl;
				}
			}

			if (operation_data.node_control_data_array[i].control_plan_data_array[j].control_type == TWO_WAY_STOP_SIGN ||
				operation_data.node_control_data_array[i].control_plan_data_array[j].control_type == YIELD_SIGN)
			{
				file 
					<< operation_data.node_control_data_array[i].control_plan_data_array[j].control_plan_index + 1 << "	"
					<< network_data.node_type_int_string_map[operation_data.node_control_data_array[i].control_plan_data_array[j].control_type] << "	"
					<< operation_data.node_control_data_array[i].control_plan_data_array[j].num_major_approaches << "	"
					<< operation_data.node_control_data_array[i].control_plan_data_array[j].num_minor_approaches 
					<<endl;
				
				for (int k=0;k<operation_data.node_control_data_array[i].control_plan_data_array[j].num_major_approaches;k++)
				{
					int approach_index = operation_data.node_control_data_array[i].control_plan_data_array[j].major_approach_data_array[k];
					if (k<operation_data.node_control_data_array[i].control_plan_data_array[j].num_major_approaches-1)
					{
						file
							<< network_data.link_data_array[operation_data.node_control_data_array[i].control_plan_data_array[j].approach_data_array[approach_index].inbound_link_index].uuid << "	";
					}
					else
					{
						file 
							<< network_data.link_data_array[operation_data.node_control_data_array[i].control_plan_data_array[j].approach_data_array[approach_index].inbound_link_index].uuid << endl;
					}
				}

				for (int k=0;k<operation_data.node_control_data_array[i].control_plan_data_array[j].num_minor_approaches;k++)
				{
					int approach_index = operation_data.node_control_data_array[i].control_plan_data_array[j].minor_approach_data_array[k];
					if (k<operation_data.node_control_data_array[i].control_plan_data_array[j].num_minor_approaches-1)
					{
						file
							<< network_data.link_data_array[operation_data.node_control_data_array[i].control_plan_data_array[j].approach_data_array[approach_index].inbound_link_index].uuid << "	";
					}
					else
					{
 						file 
							<< network_data.link_data_array[operation_data.node_control_data_array[i].control_plan_data_array[j].approach_data_array[approach_index].inbound_link_index].uuid;
						if (i<operation_data.node_control_data_size - 1)
						{
							file << endl;
						}
					}
				}
			}

			if (operation_data.node_control_data_array[i].control_plan_data_array[j].control_type == PRE_TIMED_SIGNAL_CONTROL ||
				operation_data.node_control_data_array[i].control_plan_data_array[j].control_type == ACTUATED_SIGNAL_CONTROL)
			{
				file 
					<< operation_data.node_control_data_array[i].control_plan_data_array[j].control_plan_index + 1 << "	"
					<< network_data.node_type_int_string_map[operation_data.node_control_data_array[i].control_plan_data_array[j].control_type] << "	"
					<< operation_data.node_control_data_array[i].control_plan_data_array[j].num_phases << "	"
					<< operation_data.node_control_data_array[i].control_plan_data_array[j].cycle_length
					<<endl;

				for (int k=0;k<operation_data.node_control_data_array[i].control_plan_data_array[j].num_phases;k++)
				{
					//phase timing data
					int off_set_min_green;
					int green_time_max_green;
					if (operation_data.node_control_data_array[i].control_plan_data_array[j].control_type == PRE_TIMED_SIGNAL_CONTROL)
					{
						off_set_min_green = operation_data.node_control_data_array[i].control_plan_data_array[j].phase_data_array[k].offset;
						green_time_max_green = operation_data.node_control_data_array[i].control_plan_data_array[j].phase_data_array[k].green_time;
					}

					if (operation_data.node_control_data_array[i].control_plan_data_array[j].control_type == ACTUATED_SIGNAL_CONTROL)
					{
						off_set_min_green = operation_data.node_control_data_array[i].control_plan_data_array[j].phase_data_array[k].minimum_green_time;
						green_time_max_green = operation_data.node_control_data_array[i].control_plan_data_array[j].phase_data_array[k].maximum_green_time;
					}

					file 
						<< operation_data.node_control_data_array[i].control_plan_data_array[j].phase_data_array[k].phase_index + 1 << "	"
						<< operation_data.node_control_data_array[i].control_plan_data_array[j].phase_data_array[k].phase_sequence_number << "	"
						<< operation_data.node_control_data_array[i].control_plan_data_array[j].phase_data_array[k].num_turn_movements_in_the_phase << "	"
						<< off_set_min_green << "	"
						<< green_time_max_green << "	"
						<< operation_data.node_control_data_array[i].control_plan_data_array[j].phase_data_array[k].yellow_time << "	"
						<< operation_data.node_control_data_array[i].control_plan_data_array[j].phase_data_array[k].all_red_time
						<<endl;
					
					//phasing data
					for (int m=0;m<operation_data.node_control_data_array[i].control_plan_data_array[j].phase_data_array[k].num_turn_movements_in_the_phase;m++)
					{
						int turn_movement_index = operation_data.node_control_data_array[i].control_plan_data_array[j].phase_data_array[k].turn_movements_in_the_phase_array[m].turn_movement_index;
						file 
							<< network_data.turn_movement_type_int_string_map[network_data.turn_movement_data_array[turn_movement_index].turn_movement_type] << "	"
							<< network_data.link_data_array[network_data.turn_movement_data_array[turn_movement_index].inbound_link_index].uuid << "	"
							<< network_data.link_data_array[network_data.turn_movement_data_array[turn_movement_index].outbound_link_index].uuid << "	"
							<< operation_data.movement_priority_type_int_string_map[operation_data.node_control_data_array[i].control_plan_data_array[j].phase_data_array[k].turn_movements_in_the_phase_array[m].movement_priority_type];

						if (i<operation_data.node_control_data_size - 1)
						{
							file<<endl;
						}
					}
				}
			}
		}
	}
	file.close();
};

void operation_data_information::read_control_dat_dsp(string input_dir_name,ScenarioData& scenario_data, OperationData& operation_data, NetworkData &network_data)
{
	string filename = input_dir_name + "control.dat";
	string line;
	ifstream input_file(filename);
	
	operation_data.node_control_data_array.clear();
	operation_data.node_control_data_array.resize(network_data.network_node_size);

	int node_index = -1;
	int link_index = -1;
	if (input_file.is_open())
	{
		int iline = 0;
		int iline_control_start;
		int iline_control_end;
		int iline_phase_start;
		int iline_phase_end;
		int yeild_two_way_stop_sign_start;
		int yeild_two_way_stop_sign_end;

		iline_control_start = 3;
		iline_control_end = iline_control_start + network_data.network_node_size - 1;
		iline_phase_start = iline_control_end + 1;
		int num_signals = 0;
		int num_yeild_two_way_stops = 0;
		int read_phasing_data_flag = -1;
		int read_phasing_movement_flag = -1;
		int num_inbound_links = 0;
		int current_phase = -1;
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
			//first line

			//node block
			if (iline>=iline_control_start && iline <=iline_control_end)
			{
				int node_id;
				int control_type;
				int num_phases;
				int cycle_length;
				for (int j=0;j<tokens.size();j++)
				{
					stringstream sss(tokens[j]);
					if (j==0) sss>>node_id;
					if (j==1) sss>>control_type;
					if (j==2) sss>>num_phases;
					if (j==3) sss>>cycle_length;
				}
				int node_index = network_data.node_id_index_map[node_id];
				operation_data.node_control_data_array[node_index].node_index = node_index; 
				operation_data.node_control_data_array[node_index].control_plan_data_array.clear();
				operation_data.node_control_data_array[node_index].num_control_plan_periods = 1;
				operation_data.node_control_data_array[node_index].control_plan_data_array.resize(1);
				operation_data.node_control_data_array[node_index].current_control_plan_index = 0;
				operation_data.node_control_data_array[node_index].control_plan_data_array[0].starting_time = 0;
				operation_data.node_control_data_array[node_index].control_plan_data_array[0].ending_time = 24*60*60;
				operation_data.node_control_data_array[node_index].control_plan_data_array[0].control_plan_index = 0;
				operation_data.node_control_data_array[node_index].control_plan_data_array[0].num_phases = num_phases;
				operation_data.node_control_data_array[node_index].control_plan_data_array[0].cycle_length = cycle_length;
				if (control_type == 1)
				{
					operation_data.node_control_data_array[node_index].control_plan_data_array[0].control_type = NO_CONTROL;
				}
				if (control_type == 2)
				{
					operation_data.node_control_data_array[node_index].control_plan_data_array[0].control_type = YIELD_SIGN;
					num_yeild_two_way_stops++;
				}
				if (control_type == 3)
				{
					operation_data.node_control_data_array[node_index].control_plan_data_array[0].control_type = ALL_WAY_STOP_SIGN;
				}
				if (control_type == 4)
				{
					operation_data.node_control_data_array[node_index].control_plan_data_array[0].control_type = PRE_TIMED_SIGNAL_CONTROL;
					num_signals++;
				}
				if (control_type == 5)
				{
					operation_data.node_control_data_array[node_index].control_plan_data_array[0].control_type = ACTUATED_SIGNAL_CONTROL;
					num_signals++;
				}
				if (control_type == 6)
				{
					operation_data.node_control_data_array[node_index].control_plan_data_array[0].control_type = TWO_WAY_STOP_SIGN;
					num_yeild_two_way_stops++;
				}
			}

			//link block
			if(iline >= iline_phase_start) // 
			{
				if ( iline == iline_phase_start)
				{
					read_phasing_data_flag = 1;
					read_phasing_movement_flag = -1;
				}

				if (read_phasing_data_flag == 1)
				{//reading phasing data
					int node_id;
					int phase_number;
					int offset_max_green;
					int green_time_min_green;
					int amber_time;
					int unode_1;
					int unode_2;
					int unode_3;
					int unode_4;
//phasing 1				1      1     25     10      5      1     81      0      0      0
//phasing movements     81      1      1      2      2     20
//phasing 2			    1      2     25     10      5      1    116      0      0      0
//phasing movements     116      1      2      3      2     20     81
//phasing 2			    1      3     55     10      5      1      2      0      0      0
//phasing movements     2      1      3      2     20     81
 
					for (int j=0;j<tokens.size();j++)
					{
						stringstream sss(tokens[j]);
						if (j==0) sss>>node_id;
						if (j==1) sss>>phase_number;
						if (j==2) sss>>offset_max_green;
						if (j==3) sss>>green_time_min_green;
						if (j==4) sss>>amber_time;
						if (j==5) sss>>num_inbound_links;
						if (j==5) sss>>unode_1;
						if (j==5) sss>>unode_2;
						if (j==5) sss>>unode_3;
						if (j==5) sss>>unode_4;

					}
					node_index = network_data.node_id_index_map[node_id];
					operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array.clear();
					operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array.resize(operation_data.node_control_data_array[node_index].control_plan_data_array[0].num_phases);
					current_phase = 0;
					if (operation_data.node_control_data_array[node_index].control_plan_data_array[0].control_type == PRE_TIMED_SIGNAL_CONTROL)
					{
						operation_data.node_control_data_array[node_index].control_plan_data_array[0].offset = offset_max_green;
						operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array[current_phase].green_time = green_time_min_green;
						operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array[current_phase].offset = offset_max_green;
					}
					if (operation_data.node_control_data_array[node_index].control_plan_data_array[0].control_type ==ACTUATED_SIGNAL_CONTROL)
					{
						operation_data.node_control_data_array[node_index].control_plan_data_array[0].offset = 0;
						operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array[current_phase].maximum_green_time = offset_max_green;
						operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array[current_phase].minimum_green_time = green_time_min_green;
					}

					operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array[current_phase].phase_index = current_phase;
					operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array[current_phase].yellow_time = amber_time;
					operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array[current_phase].phase_sequence_number = phase_number;
					operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array[current_phase].all_red_time = 0;
					operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array[current_phase].turn_movements_in_the_phase_array.clear();
					operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array[current_phase].num_turn_movements_in_the_phase = 0;
					
					read_phasing_data_flag = -1;
					read_phasing_movement_flag = 1;
				}
				if (read_phasing_movement_flag == 1)
				{//reading phasing movement
					int unode_inbound_link_id;
					int dnode_inbound_link_id;
					int phase_number;
					int num_movements;
					vector<int> dnode_movements;
					dnode_movements.clear();
					for (int j=0;j<tokens.size();j++)
					{
						stringstream sss(tokens[j]);
						if (j==0) sss>>unode_inbound_link_id;
						if (j==1) sss>>dnode_inbound_link_id;
						if (j==2) sss>>phase_number;
						if (j==3) sss>>num_movements;
						if (j>=4)
						{
							int dnode_outbound_link_id;
							sss>>dnode_outbound_link_id;
							dnode_movements.push_back(dnode_outbound_link_id);
						}
					}
					for (int m=0;m<num_movements;m++)
					{
						PhaseMovementData phase_movement_data;
						int unode_index = network_data.node_id_index_map[unode_inbound_link_id];
						int dnode_index = network_data.node_id_index_map[dnode_inbound_link_id];
						int nnode_index = network_data.node_id_index_map[dnode_movements[m]];
						int inbound_link_index = network_data.node_link_map[make_pair(unode_index,dnode_index)];
						int outbound_link_index = network_data.node_link_map[make_pair(dnode_index,nnode_index)];
						int turn_movement_index = network_data.link_turn_movement_map[make_pair(inbound_link_index,outbound_link_index)];
						phase_movement_data.turn_movement_index = turn_movement_index;
						phase_movement_data.movement_priority_type = PERMITTED;
						phase_movement_data.phase_movement_index = operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array[current_phase].num_turn_movements_in_the_phase;
						operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array[current_phase].turn_movements_in_the_phase_array.push_back(phase_movement_data);
						operation_data.node_control_data_array[node_index].control_plan_data_array[0].phase_data_array[current_phase].num_turn_movements_in_the_phase++;
					}

					num_inbound_links--;
					
					if (num_inbound_links==0)
					{
						read_phasing_data_flag = 1;
						read_phasing_movement_flag = -1;
						current_phase++;
					}
				}
			}
		}
	}
	else
	{
		cout << "file " << filename << "cannot be opened!" << endl;
	}


	input_file.close();
};