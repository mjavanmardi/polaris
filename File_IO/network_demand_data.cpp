#include "network_demand_data.h"

using namespace std;
using namespace network_models::network_information;

void demand_data_information::read_demand_vehicle(string input_dir_name,ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data)
{
	//node
	//string time_file_name = input_dir_name + "demand_time.csv";
	//fstream time_file;
	//time_file.open(time_file_name,fstream::out);
	//double time_in_second_open_file_start = get_current_cpu_time_in_seconds();
	string file_name = input_dir_name + "vehicle";
	string line;
	ifstream input_file(file_name);

	//initialization
	demand_data.vehilce_data_array.clear();
	demand_data.vehicle_id_index_map.clear();

	demand_data.time_dependent_vehicle_index_array.resize(scenario_data.num_simulation_intervals*scenario_data.simulation_interval_length);

	demand_data.first_vehicle_departure_time = 24*60*60;
	demand_data.last_vehicle_departure_time = 0;
	int num_vehicles = 0;
	//double time_in_second_open_file = get_current_cpu_time_in_seconds() - time_in_second_open_file_start;

	if(input_file.is_open()) 
	{ 
		int iline = 0;
		int vehicle_index = -1;
		vector<string> tokens;
		int token_size =0;
		while (input_file.good())
		{
			//double time_in_second_read_getline_start = get_current_cpu_time_in_seconds();
			getline(input_file,line);
			//double time_in_second_getline = get_current_cpu_time_in_seconds() - time_in_second_read_getline_start;

			iline = iline + 1;

			//double time_in_second_read_parse_start = get_current_cpu_time_in_seconds();

			//double time_in_second_parse_tokens = 0.0;
			//double time_in_second_parse_tokens_start;
			if (iline == 2)
			{
				//time_in_second_parse_tokens_start = get_current_cpu_time_in_seconds();
				token_size = 3;
				string_split(tokens, line, token_size, "\t");
				//time_in_second_parse_tokens = get_current_cpu_time_in_seconds() - time_in_second_parse_tokens_start;

				demand_data.demand_vehicle_size = stoi(tokens[0]);
				demand_data.vehilce_data_array.resize(demand_data.demand_vehicle_size);
			}
			
			//double time_in_second_parse_read_tokens = 0.0;
			//double time_in_second_parse_map = 0.0;
			//double time_in_second_parse_vehicle_data = 0.0;

			if(iline > 3) // skip the first line
			{
				
				//time_in_second_parse_tokens_start = get_current_cpu_time_in_seconds();
				token_size = 8;
				string_split(tokens, line, token_size, "\t");
				//time_in_second_parse_tokens = get_current_cpu_time_in_seconds() - time_in_second_parse_tokens_start;

				//double time_in_second_parse_read_tokens_start = get_current_cpu_time_in_seconds();
				int vehicle_id = stoi(tokens[0]);
				int origin_zone_id = stoi(tokens[1]);
				int destination_zone_id = stoi(tokens[2]);
				int origin_activity_location_id = stoi(tokens[3]);
				int destination_activity_location_id = stoi(tokens[4]);
				int origin_link_id = stoi(tokens[5]);
				int destination_link_id = stoi(tokens[6]);
				int departure_time = convert_hhmmss_to_seconds(tokens[7]);
				
				//time_in_second_parse_read_tokens = get_current_cpu_time_in_seconds() - time_in_second_parse_read_tokens_start;

				//double time_in_second_parse_map_start = get_current_cpu_time_in_seconds();
				int origin_zone_index = network_data.zone_id_index_map[origin_zone_id];
				int destination_zone_index = network_data.zone_id_index_map[destination_zone_id];
				int origin_activity_location_index = network_data.activity_location_id_index_map[origin_activity_location_id];
				int destination_activity_location_index = network_data.activity_location_id_index_map[destination_activity_location_id];
				int origin_link_index = network_data.link_id_index_map[origin_link_id];
				int destination_link_index = network_data.link_id_index_map[destination_link_id];
				//time_in_second_parse_map = get_current_cpu_time_in_seconds() - time_in_second_parse_map_start;

				//double time_in_second_parse_vehicle_data_start = get_current_cpu_time_in_seconds() ;
				if (departure_time < scenario_data.simulation_start_time || departure_time >= scenario_data.simulation_end_time)
				{
					continue;
				}
				else
				{
					departure_time = departure_time -scenario_data.simulation_start_time;
				}
				
				vehicle_index = vehicle_index + 1;
				VehicleData& vehicle_data = demand_data.vehilce_data_array[vehicle_index];

				num_vehicles++;
				vehicle_data.set_vehicle_index(vehicle_index);
				vehicle_data.set_vehicle_id(vehicle_id);

				vehicle_data.set_origin_zone_index(origin_zone_index);
				vehicle_data.set_destination_zone_index(destination_zone_index);
				vehicle_data.set_origin_activity_location_index(origin_activity_location_index);
				vehicle_data.set_destination_activity_location_index(destination_activity_location_index);
				vehicle_data.set_origin_link_index(origin_link_index);
				vehicle_data.set_destination_link_index(destination_link_index);
				vehicle_data.set_seed(scenario_data.iseed+vehicle_index);
				
				int simulation_interval_index;
				simulation_interval_index = departure_time/scenario_data.simulation_interval_length;
				int assignment_interval_index;
				assignment_interval_index = int((simulation_interval_index+1) *scenario_data.simulation_interval_length / scenario_data.assignment_interval_length);

				vehicle_data.set_departure_assignment_interval_index(assignment_interval_index);
				vehicle_data.set_departure_simulation_interval_index(simulation_interval_index);
				vehicle_data.set_departure_time(departure_time);
				vehicle_data.init();

				if (demand_data.first_vehicle_departure_time>=departure_time+scenario_data.simulation_start_time)
				{
					demand_data.first_vehicle_departure_time = departure_time+scenario_data.simulation_start_time;
				}

				if (demand_data.last_vehicle_departure_time<=departure_time+scenario_data.simulation_start_time)
				{
					demand_data.last_vehicle_departure_time = departure_time+scenario_data.simulation_start_time;
				}

				demand_data.time_dependent_vehicle_index_array[departure_time].push_back(vehicle_index);
				//time_in_second_parse_vehicle_data = get_current_cpu_time_in_seconds() - time_in_second_parse_vehicle_data_start;
			}
			//double time_in_second_parse = get_current_cpu_time_in_seconds() - time_in_second_read_parse_start;
			//time_file
			//	<<time_in_second_open_file << ","
			//	<<time_in_second_getline << ","
			//	<<time_in_second_parse_tokens << ","
			//	<<time_in_second_parse << ","
			//	<<time_in_second_parse_read_tokens << ","
			//	<<time_in_second_parse_map << ","
			//	<<time_in_second_parse_vehicle_data << ","
			//	<<endl;
		}
		demand_data.vehilce_data_array.shrink_to_fit();
		demand_data.demand_vehicle_size = (int)demand_data.vehilce_data_array.size();
	}
	else
	{
		cout << "Cannot open file - "
			<< file_name
			<< endl;
	}
	//time_file.close();
};
void demand_data_information::write_vehicle_path(string output_dir_name,ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data)
{
	//node
	string file_name = output_dir_name + "output_vehicle_path";
	fstream file;
	file.open(file_name,fstream::out);
	if(file.is_open()) 
	{ 
		file 
			<< "vehicle" <<  "	"
			<< "num_links" <<  "	"
			<< "links" << "	"
			<<endl;
	}
	else
	{
		cout << "Cannot open file - "
			<< file_name
			<< endl;
	}

	for (int i=0;i<demand_data.demand_vehicle_size;i++)
	{
		int departure_time = demand_data.vehilce_data_array[i].get_departure_time();
		
		file
			<< demand_data.vehilce_data_array[i].get_vehicle_id()<< "	"
			<< demand_data.vehilce_data_array[i].get_route_link_size()<< "	";
		for (int j=0;j<demand_data.vehilce_data_array[i].get_route_link_size();j++)
		{
			file
				<< demand_data.vehilce_data_array[i].get_route_link_index(j)<< "	";
		}

		if (i<demand_data.demand_vehicle_size-1)
		{
			file << endl;
		}
	}
	
	scenario_data.simulation_end_time = max((int(demand_data.first_vehicle_departure_time/60)+1)*60,0);
	scenario_data.simulation_end_time = (int(demand_data.last_vehicle_departure_time/60)+1)*60;

	file.close();
};
void demand_data_information::write_demand_vehicle(string output_dir_name,ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data)
{
	//node
	string file_name = output_dir_name + "output_vehicle";
	fstream file;
	file.open(file_name,fstream::out);
	if(file.is_open()) 
	{ 
		file 
			<< "num_vehicles" << "	"
			<< "starting_time" << "	"
			<< "ending_time" << "	"
			<<endl;
		
		file 
			<< demand_data.demand_vehicle_size << "	"
			<< convert_seconds_to_hhmmss(demand_data.first_vehicle_departure_time) << "	"
			<< convert_seconds_to_hhmmss(demand_data.last_vehicle_departure_time) << "	"
			<<endl;

		file 
			<< "vehicle" <<  "	"
			<< "origin_zone" <<  "	"
			<< "destination_zone" << "	"
			<< "origin_activity_location" <<  "	"
			<< "destination_activity_location" << "	"
			<< "origin_link" <<  "	"
			<< "destination_link" <<  "	"
			<< "departure_time"
			<<endl;
	}
	else
	{
		cout << "Cannot open file - "
			<< file_name
			<< endl;
	}

	for (int i=0;i<demand_data.demand_vehicle_size;i++)
	{
		int departure_time = demand_data.vehilce_data_array[i].get_departure_time();
	
		file
			<< demand_data.vehilce_data_array[i].get_vehicle_id()<< "	"
			<< network_data.zone_data_array[demand_data.vehilce_data_array[i].get_origin_zone_index()].uuid<< "	"
			<< network_data.zone_data_array[demand_data.vehilce_data_array[i].get_destination_zone_index()].uuid<< "	"
			<< network_data.activity_location_data_array[demand_data.vehilce_data_array[i].get_origin_activity_location_index()].uuid<< "	"
			<< network_data.activity_location_data_array[demand_data.vehilce_data_array[i].get_destination_activity_location_index()].uuid<< "	"
			<< network_data.link_data_array[demand_data.vehilce_data_array[i].get_origin_link_index()].uuid<< "	"
			<< network_data.link_data_array[demand_data.vehilce_data_array[i].get_destination_link_index()].uuid<< "	"
			<< convert_seconds_to_hhmmss(demand_data.vehilce_data_array[i].get_departure_time() + scenario_data.simulation_start_time);
		if (i<demand_data.demand_vehicle_size-1)
		{
			file << endl;
		}
	}
	
	scenario_data.simulation_end_time = max((int(demand_data.first_vehicle_departure_time/60)+1)*60,0);
	scenario_data.simulation_end_time = (int(demand_data.last_vehicle_departure_time/60)+1)*60;

	file.close();
};


void demand_data_information::read_demand_data_transims(ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data)
{
	//char *demand_control_file_name = "C:\\POLARIS\\sourcecodes\\polaris\\TransimsData\\TestNet50\\control\\ConvertTrips.ctl";

	//Initialize_Demand_Generator(demand_control_file_name,false,"TAB_DELIMITED");
	//t_Trips(
};

void demand_data_information::read_demand_data_two_links(ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data)
{
	
	RngStream g1("Demand");
	g1.SetSeed(1);
	int vehilce_index = -1;
	//sort demand by departure time

	demand_data.time_dependent_vehicle_index_array.resize(scenario_data.num_simulation_intervals*scenario_data.simulation_interval_length);
	float freeway_demand_rate = 1800.0;
	for (int simulation_interval_index=0;simulation_interval_index<scenario_data.num_simulation_intervals;simulation_interval_index++)
	{// generate vehicles for each simulation interval
		
		if (simulation_interval_index<=300)
		{
			freeway_demand_rate += 10.0;
		}
		else
		{
			freeway_demand_rate -= 10.0;
		}
		int freeway_origin_activity_location_index = 0;
		int freeway_origin_link_index = network_data.activity_location_data_array[freeway_origin_activity_location_index].origin_link_index_array[0];
		int num_lanes_freeway = network_data.link_data_array[freeway_origin_link_index].num_lanes;
		
		float num_generated_vehicles_freeway = (float) (1.0+(num_lanes_freeway*scenario_data.simulation_interval_length * freeway_demand_rate)/3600.0);

		int destination_activity_location_index = 1;
		int destination_link_index = network_data.activity_location_data_array[destination_activity_location_index].destination_link_index_array[0];
		
		network_information::demand_data_information::VehicleData vehicle_data;
		//freeway
		float vehicle_rate_per_simulation_interval;
		int departure_time = -1;
		int origin_link_index;
		int origin_activity_location_index;
		int assignment_interval;
		assignment_interval = int((simulation_interval_index+1) *scenario_data.simulation_interval_length / scenario_data.assignment_interval_length);

		vehicle_rate_per_simulation_interval = (float)(num_generated_vehicles_freeway/((float)scenario_data.simulation_interval_length));
		origin_link_index = freeway_origin_link_index;
		origin_activity_location_index = freeway_origin_activity_location_index;
		for (int i=0;i<scenario_data.simulation_interval_length;i++)
		{
			float cur_vehicle_rate_per_simulation_interval = vehicle_rate_per_simulation_interval;
			while(cur_vehicle_rate_per_simulation_interval>0)
			{
				if (cur_vehicle_rate_per_simulation_interval>=1.0)
				{//
					vehilce_index = vehilce_index + 1;
					vehicle_data.set_vehicle_index(vehilce_index);
					vehicle_data.set_origin_link_index(origin_link_index);
					vehicle_data.set_destination_link_index(destination_link_index);
					vehicle_data.set_origin_activity_location_index(origin_activity_location_index);
					vehicle_data.set_destination_activity_location_index(destination_activity_location_index);
					vehicle_data.set_seed(scenario_data.iseed+vehilce_index);
					vehicle_data.set_departure_assignment_interval_index(assignment_interval);
					vehicle_data.set_departure_simulation_interval_index(simulation_interval_index);

					departure_time = simulation_interval_index *scenario_data.simulation_interval_length  + i;
					vehicle_data.set_departure_time(departure_time);

					demand_data.vehilce_data_array.push_back(vehicle_data);

					demand_data.time_dependent_vehicle_index_array[departure_time].push_back(vehilce_index);

					cur_vehicle_rate_per_simulation_interval = float (cur_vehicle_rate_per_simulation_interval - 1.0);
				}
				else
				{//monte carlo
					double r1 = (double) g1.RandU01();
					if (r1<=cur_vehicle_rate_per_simulation_interval)
					{
						vehilce_index = vehilce_index + 1;
						vehicle_data.set_vehicle_index(vehilce_index);
						vehicle_data.set_origin_link_index(origin_link_index);
						vehicle_data.set_destination_link_index(destination_link_index);
						vehicle_data.set_origin_activity_location_index(origin_activity_location_index);
						vehicle_data.set_destination_activity_location_index(destination_activity_location_index);
						vehicle_data.set_seed(scenario_data.iseed+vehilce_index);
						vehicle_data.set_departure_assignment_interval_index(assignment_interval);
						vehicle_data.set_departure_simulation_interval_index(simulation_interval_index);

						departure_time = simulation_interval_index *scenario_data.simulation_interval_length  + i;
						vehicle_data.set_departure_time(departure_time);

						demand_data.vehilce_data_array.push_back(vehicle_data);

						demand_data.time_dependent_vehicle_index_array[departure_time].push_back(vehilce_index);
					
					}
					cur_vehicle_rate_per_simulation_interval = float(cur_vehicle_rate_per_simulation_interval - 1.0);
				}
			}
		}
		//ramp
	}
	demand_data.demand_vehicle_size = (int) demand_data.vehilce_data_array.size();
	//for (int i=0;i<(int)demand_data.time_dependent_vehicle_index_array.size();i++)
	//{
	//	printf("t= %d s=%d",i,demand_data.time_dependent_vehicle_index_array[i].size());
	//	for (int j=0;j<(int)demand_data.time_dependent_vehicle_index_array[i].size();j++)
	//	{
	//		printf(" %d",demand_data.time_dependent_vehicle_index_array[i][j]);
	//	}
	//	printf("\n");
	//}
};
void demand_data_information::read_demand_data(ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data)
{
	float freeway_demand_rate = 1800.0;
	float ramp_demand_rate = 900.0;
	RngStream g1("Demand");
	g1.SetSeed(1);
	int vehilce_index = -1;
	//sort demand by departure time

	demand_data.time_dependent_vehicle_index_array.resize(scenario_data.num_simulation_intervals*scenario_data.simulation_interval_length);

	for (int simulation_interval_index=0;simulation_interval_index<scenario_data.num_simulation_intervals;simulation_interval_index++)
	{// generate vehicles for each simulation interval

		if (simulation_interval_index<=300)
		{
			freeway_demand_rate += 5.0;
			ramp_demand_rate +=5.0;
		}
		else
		{
			freeway_demand_rate -= 5.0;
			ramp_demand_rate -=5.0;
		}

		int freeway_origin_activity_location_index = 0;
		int freeway_origin_link_index = network_data.activity_location_data_array[freeway_origin_activity_location_index].origin_link_index_array[0];
		int num_lanes_freeway = network_data.link_data_array[freeway_origin_link_index].num_lanes;

		int ramp_origin_activity_location_index = 2;
		int ramp_origin_link_index = network_data.activity_location_data_array[ramp_origin_activity_location_index].origin_link_index_array[0];
		int num_lanes_ramp = network_data.link_data_array[ramp_origin_link_index].num_lanes;
		
		float num_generated_vehicles_freeway = (float) (1.0+(num_lanes_freeway*scenario_data.simulation_interval_length * freeway_demand_rate)/3600.0);
		float num_generated_vehicles_ramp = (float) (1.0+(num_lanes_ramp*scenario_data.simulation_interval_length * ramp_demand_rate)/3600.0);

		int destination_activity_location_index = 1;
		int destination_link_index = network_data.activity_location_data_array[destination_activity_location_index].destination_link_index_array[0];

		
		network_information::demand_data_information::VehicleData vehicle_data;
		//freeway
		float vehicle_rate_per_simulation_interval;
		int departure_time = -1;
		int origin_link_index;
		int origin_activity_location_index;
		int assignment_interval;
		assignment_interval = int((simulation_interval_index+1) *scenario_data.simulation_interval_length / scenario_data.assignment_interval_length);

		vehicle_rate_per_simulation_interval = (float)(num_generated_vehicles_freeway/((float)scenario_data.simulation_interval_length));
		origin_link_index = freeway_origin_link_index;
		origin_activity_location_index = freeway_origin_activity_location_index;
		for (int i=0;i<scenario_data.simulation_interval_length;i++)
		{
			float cur_vehicle_rate_per_simulation_interval = vehicle_rate_per_simulation_interval;
			while(cur_vehicle_rate_per_simulation_interval>0)
			{
				if (cur_vehicle_rate_per_simulation_interval>=1.0)
				{//
					vehilce_index = vehilce_index + 1;
					vehicle_data.set_vehicle_index(vehilce_index);
					vehicle_data.set_origin_link_index(origin_link_index);
					vehicle_data.set_destination_link_index(destination_link_index);
					vehicle_data.set_origin_activity_location_index(origin_activity_location_index);
					vehicle_data.set_destination_activity_location_index(destination_activity_location_index);
					vehicle_data.set_seed(scenario_data.iseed+vehilce_index);
					vehicle_data.set_departure_assignment_interval_index(assignment_interval);
					vehicle_data.set_departure_simulation_interval_index(simulation_interval_index);

					departure_time = simulation_interval_index *scenario_data.simulation_interval_length  + i;
					vehicle_data.set_departure_time(departure_time);

					demand_data.vehilce_data_array.push_back(vehicle_data);

					demand_data.time_dependent_vehicle_index_array[departure_time].push_back(vehilce_index);

					cur_vehicle_rate_per_simulation_interval = float (cur_vehicle_rate_per_simulation_interval - 1.0);
				}
				else
				{//monte carlo
					double r1 = (double) g1.RandU01();
					if (r1<=cur_vehicle_rate_per_simulation_interval)
					{
						vehilce_index = vehilce_index + 1;
						vehicle_data.set_vehicle_index(vehilce_index);
						vehicle_data.set_origin_link_index(origin_link_index);
						vehicle_data.set_destination_link_index(destination_link_index);
						vehicle_data.set_origin_activity_location_index(origin_activity_location_index);
						vehicle_data.set_destination_activity_location_index(destination_activity_location_index);
						vehicle_data.set_seed(scenario_data.iseed+vehilce_index);
						vehicle_data.set_departure_assignment_interval_index(assignment_interval);
						vehicle_data.set_departure_simulation_interval_index(simulation_interval_index);

						departure_time = simulation_interval_index *scenario_data.simulation_interval_length  + i;
						vehicle_data.set_departure_time(departure_time);

						demand_data.vehilce_data_array.push_back(vehicle_data);

						demand_data.time_dependent_vehicle_index_array[departure_time].push_back(vehilce_index);
					
					}
					cur_vehicle_rate_per_simulation_interval = float(cur_vehicle_rate_per_simulation_interval - 1.0);
				}
			}
		}

		//ramp

		vehicle_rate_per_simulation_interval = (float)(num_generated_vehicles_ramp/((float)scenario_data.simulation_interval_length));
		origin_link_index = ramp_origin_link_index;
		origin_activity_location_index = ramp_origin_activity_location_index;
		for (int i=0;i<scenario_data.simulation_interval_length;i++)
		{
			float cur_vehicle_rate_per_simulation_interval = vehicle_rate_per_simulation_interval;
			while(cur_vehicle_rate_per_simulation_interval>0)
			{
				if (cur_vehicle_rate_per_simulation_interval>=1.0)
				{//
					vehilce_index = vehilce_index + 1;
					vehicle_data.set_vehicle_index(vehilce_index);
					vehicle_data.set_origin_link_index(origin_link_index);
					vehicle_data.set_destination_link_index(destination_link_index);
					vehicle_data.set_origin_activity_location_index(origin_activity_location_index);
					vehicle_data.set_destination_activity_location_index(destination_activity_location_index);
					vehicle_data.set_seed(scenario_data.iseed+vehilce_index);
					vehicle_data.set_departure_assignment_interval_index(assignment_interval);
					vehicle_data.set_departure_simulation_interval_index(simulation_interval_index);

					departure_time = simulation_interval_index *scenario_data.simulation_interval_length  + i;
					vehicle_data.set_departure_time(departure_time);

					demand_data.vehilce_data_array.push_back(vehicle_data);

					demand_data.time_dependent_vehicle_index_array[departure_time].push_back(vehilce_index);

					cur_vehicle_rate_per_simulation_interval = float(cur_vehicle_rate_per_simulation_interval - 1.0);
				}
				else
				{//monte carlo
					double r1 = (double) g1.RandU01();
					if (r1<=cur_vehicle_rate_per_simulation_interval)
					{
						vehilce_index = vehilce_index + 1;
						vehicle_data.set_vehicle_index(vehilce_index);
						vehicle_data.set_origin_link_index(origin_link_index);
						vehicle_data.set_destination_link_index(destination_link_index);
						vehicle_data.set_origin_activity_location_index(origin_activity_location_index);
						vehicle_data.set_destination_activity_location_index(destination_activity_location_index);
						vehicle_data.set_seed(scenario_data.iseed+vehilce_index);
						vehicle_data.set_departure_assignment_interval_index(assignment_interval);
						vehicle_data.set_departure_simulation_interval_index(simulation_interval_index);

						departure_time = simulation_interval_index *scenario_data.simulation_interval_length  + i;
						vehicle_data.set_departure_time(departure_time);

						demand_data.vehilce_data_array.push_back(vehicle_data);

						demand_data.time_dependent_vehicle_index_array[departure_time].push_back(vehilce_index);
					}
					cur_vehicle_rate_per_simulation_interval = float(cur_vehicle_rate_per_simulation_interval - 1.0);
				}
			}
		}
	
	}
	demand_data.demand_vehicle_size = (int) demand_data.vehilce_data_array.size();
	//for (int i=0;i<(int)demand_data.time_dependent_vehicle_index_array.size();i++)
	//{
	//	printf("t= %d s=%d",i,demand_data.time_dependent_vehicle_index_array[i].size());
	//	for (int j=0;j<(int)demand_data.time_dependent_vehicle_index_array[i].size();j++)
	//	{
	//		printf(" %d",demand_data.time_dependent_vehicle_index_array[i][j]);
	//	}
	//	printf("\n");
	//}
};

void demand_data_information::read_demand_od(string input_dir_name,ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data)
{
	RngStream rng_demand("Demand");
	rng_demand.SetSeed(scenario_data.iseed);
	float r1;

	string filename = input_dir_name + "demand_od";
	string line;
	ifstream input_file(filename);
	
	//initialization
	demand_data.vehilce_data_array.clear();
	demand_data.vehicle_id_index_map.clear();
	demand_data.time_of_day_hourly_pattern.clear();
	demand_data.time_of_day_hourly_pattern.resize(24);

	demand_data.time_dependent_vehicle_index_array.resize(scenario_data.num_simulation_intervals*scenario_data.simulation_interval_length);

	demand_data.first_vehicle_departure_time = 24*60*60;
	demand_data.last_vehicle_departure_time = 0;

	//read file
	if (input_file.is_open())
	{
		int iline = 0;
		int origin_zone_id;
		int vehicle_index = -1;

		vector<string> tokens;
		int token_size =0;

		while (input_file.good())
		{
			getline(input_file,line);
			iline = iline + 1;

			if (iline == 1)
			{//time of day hourly pattern
				token_size = 24;
				string_split(tokens, line, token_size, "\t");

				for (int j=0;j<token_size;j++)
				{
					demand_data.time_of_day_hourly_pattern[j] = stof(tokens[j]);
				}
			}
		
			if(iline > 1) // skip the first line
			{
				//nested structure
				//if (iline%2 == 1)
				if (iline%2 == 0)
				{//read first line of an origin zone
					token_size = 1;
					string_split(tokens, line, token_size, "\t");
					origin_zone_id = stoi(tokens[0]);
				}

//				if (iline%2 == 0)
				if (iline%2 == 1)
				{//read third line of an activity location - destination links

					token_size = network_data.network_zone_size;
					string_split(tokens, line, token_size, "\t");

					int origin_zone_index = network_data.zone_id_index_map[origin_zone_id];
					int destination_zone_index;
					
					VehicleData vehicle_data;
					vehicle_data.set_origin_zone_index(origin_zone_index);

					int num_origin_activity_locations = network_data.zone_data_array[origin_zone_index].num_origin_activity_locations;

					for (int j=0;j<token_size;j++)
					{
						float od_flow = stof(tokens[j]);

						//vehicle generation
						if (od_flow>0.0)
						{
							destination_zone_index = j;
							vehicle_data.set_destination_zone_index(destination_zone_index);

							for (int simulation_interval_index=0;simulation_interval_index<scenario_data.num_simulation_intervals;simulation_interval_index++)
							{// generate vehicles for each simulation interval

								float vehicle_rate_per_simulation_interval;
								int departure_time = -1;
								
								int assignment_interval_index;
								assignment_interval_index = int((simulation_interval_index+1) *scenario_data.simulation_interval_length / scenario_data.assignment_interval_length);

								vehicle_rate_per_simulation_interval = (float)(od_flow/(3600.0f/(scenario_data.simulation_interval_length*1.0f)));
								
								int current_hour = (scenario_data.simulation_start_time + simulation_interval_index*scenario_data.simulation_interval_length)/(60*60);
								vehicle_rate_per_simulation_interval = vehicle_rate_per_simulation_interval * demand_data.time_of_day_hourly_pattern[current_hour];
								
								for (int i=0;i<scenario_data.simulation_interval_length;i++)
								{
									float vehicle_rate_per_second = (vehicle_rate_per_simulation_interval*1.0f)/(scenario_data.simulation_interval_length*1.0f);
									departure_time = simulation_interval_index *scenario_data.simulation_interval_length  + i;

									while(vehicle_rate_per_second>0)
									{
										if (vehicle_rate_per_second>=1.0)
										{//
											vehicle_index = vehicle_index + 1;
											vehicle_data.set_vehicle_index(vehicle_index);
											vehicle_data.set_vehicle_id(vehicle_index+1);

											//origin activity location choice
											int origin_activity_location_index = -1;
											r1 = (float) rng_demand.RandU01();
											origin_activity_location_index = origin_activity_location_choice_model(origin_zone_index,network_data,r1);

											//origin link choice
											int origin_link_index = -1;
											r1 = (float) rng_demand.RandU01();
											origin_link_index = origin_link_choice_model(origin_activity_location_index,network_data,r1);

											//destination activity location choice
											int destination_activity_location_index = -1;
											r1 = (float) rng_demand.RandU01();
											destination_activity_location_index = destination_activity_location_choice_model(destination_zone_index,network_data,r1);

											//destination link choice
											int destination_link_index = -1;
											r1 = (float) rng_demand.RandU01();
											destination_link_index = destination_link_choice_model(destination_activity_location_index,network_data,r1);

											vehicle_data.set_origin_activity_location_index(origin_activity_location_index);
											vehicle_data.set_destination_activity_location_index(destination_activity_location_index);
											vehicle_data.set_origin_link_index(origin_link_index);
											vehicle_data.set_destination_link_index(destination_link_index);

											vehicle_data.set_seed(scenario_data.iseed+vehicle_data.get_vehicle_index());

											vehicle_data.set_departure_assignment_interval_index(assignment_interval_index);
											vehicle_data.set_departure_simulation_interval_index(simulation_interval_index);
											vehicle_data.set_departure_time(departure_time);

											if (demand_data.first_vehicle_departure_time>=departure_time+scenario_data.simulation_start_time)
											{
												demand_data.first_vehicle_departure_time = departure_time+scenario_data.simulation_start_time;
											}

											if (demand_data.last_vehicle_departure_time<=departure_time+scenario_data.simulation_start_time)
											{
												demand_data.last_vehicle_departure_time = departure_time+scenario_data.simulation_start_time;
											}

											vehicle_data.init();

											demand_data.vehilce_data_array.push_back(vehicle_data);

											demand_data.time_dependent_vehicle_index_array[departure_time].push_back(vehicle_index);

											vehicle_rate_per_second = float (vehicle_rate_per_second - 1.0);
										}
										else
										{//monte carlo
											r1 = (float) rng_demand.RandU01();
											if (r1<=vehicle_rate_per_second)
											{
												vehicle_index = vehicle_index + 1;
												vehicle_data.set_vehicle_index(vehicle_index);
												vehicle_data.set_vehicle_id(vehicle_index+1);

												//origin activity location choice
												int origin_activity_location_index = -1;
												r1 = (float) rng_demand.RandU01();
												origin_activity_location_index = origin_activity_location_choice_model(origin_zone_index,network_data,r1);

												//origin link choice
												int origin_link_index = -1;
												r1 = (float) rng_demand.RandU01();
												origin_link_index = origin_link_choice_model(origin_activity_location_index,network_data,r1);

												//destination activity location choice
												int destination_activity_location_index = -1;
												r1 = (float) rng_demand.RandU01();
												destination_activity_location_index = destination_activity_location_choice_model(destination_zone_index,network_data,r1);

												//destination link choice
												int destination_link_index = -1;
												r1 = (float) rng_demand.RandU01();
												destination_link_index = destination_link_choice_model(destination_activity_location_index,network_data,r1);

												vehicle_data.set_origin_link_index(origin_link_index);
												vehicle_data.set_destination_link_index(destination_link_index);
												vehicle_data.set_origin_activity_location_index(origin_activity_location_index);
												vehicle_data.set_destination_activity_location_index(destination_activity_location_index);
												
												vehicle_data.set_seed(scenario_data.iseed+vehicle_data.get_vehicle_index());
												vehicle_data.set_departure_assignment_interval_index(assignment_interval_index);
												vehicle_data.set_departure_simulation_interval_index(simulation_interval_index);
												vehicle_data.set_departure_time(departure_time);
												if (demand_data.first_vehicle_departure_time>=departure_time+scenario_data.simulation_start_time)
												{
													demand_data.first_vehicle_departure_time = departure_time+scenario_data.simulation_start_time;
												}

												if (demand_data.last_vehicle_departure_time<=departure_time+scenario_data.simulation_start_time)
												{
													demand_data.last_vehicle_departure_time = departure_time+scenario_data.simulation_start_time;
												}

												vehicle_data.init();

												demand_data.vehilce_data_array.push_back(vehicle_data);
												demand_data.time_dependent_vehicle_index_array[departure_time].push_back(vehicle_index);
					
											}
											vehicle_rate_per_second = float(vehicle_rate_per_second - 1.0f);
										}
									}
								}

							}
						}
					}
				}
			}
		}
	}
	demand_data.demand_vehicle_size = (int) demand_data.vehilce_data_array.size();
};

int demand_data_information::origin_activity_location_choice_model(int origin_zone_index, NetworkData& network_data, float r1)
{
	int origin_activity_location_index;
	int num_origin_activity_locations = network_data.zone_data_array[origin_zone_index].num_origin_activity_locations;
	if (num_origin_activity_locations>1)
	{
		for (int ia=0;ia<num_origin_activity_locations;ia++)
		{
			if (r1<=network_data.zone_data_array[origin_zone_index].origin_activity_location_choice_cdf_array[ia])
			{
				origin_activity_location_index = network_data.zone_data_array[origin_zone_index].origin_activity_location_index_array[ia];
				break;
			}
		}
	}
	else
	{
		origin_activity_location_index = network_data.zone_data_array[origin_zone_index].origin_activity_location_index_array[0];
	}
	return origin_activity_location_index;
};
int demand_data_information::destination_activity_location_choice_model(int destination_zone_index, NetworkData& network_data, float r1)
{
	int destination_activity_location_index;
	int num_destination_activity_locations = network_data.zone_data_array[destination_zone_index].num_destination_activity_locations;
	if (num_destination_activity_locations>1)
	{
		for (int ia=0;ia<num_destination_activity_locations;ia++)
		{
			if (r1<=network_data.zone_data_array[destination_zone_index].destination_activity_location_choice_cdf_array[ia])
			{
				destination_activity_location_index = network_data.zone_data_array[destination_zone_index].destination_activity_location_index_array[ia];
				break;
			}
		}
	}
	else
	{
		destination_activity_location_index = network_data.zone_data_array[destination_zone_index].destination_activity_location_index_array[0];
											
	}
	return destination_activity_location_index;
};

int demand_data_information::origin_link_choice_model(int origin_activity_location_index, NetworkData& network_data, float r1)
{
	int origin_link_index;
	int num_origin_links = network_data.activity_location_data_array[origin_activity_location_index].num_origin_links;
	if (num_origin_links>1)
	{
		for (int il=0;il<num_origin_links;il++)
		{
			if (r1<=network_data.activity_location_data_array[origin_activity_location_index].origin_link_choice_cdf_array[il])
			{
				origin_link_index = network_data.activity_location_data_array[origin_activity_location_index].origin_link_index_array[il];
				break;
			}
		}
	}
	else
	{
		origin_link_index = network_data.activity_location_data_array[origin_activity_location_index].origin_link_index_array[0];
	}
	return origin_link_index;
};

int demand_data_information::destination_link_choice_model(int destination_activity_location_index, NetworkData& network_data, float r1)
{
	int destination_link_index;
	int num_destination_links = network_data.activity_location_data_array[destination_activity_location_index].num_destination_links;
	if (num_destination_links>1)
	{
		for (int il=0;il<num_destination_links;il++)
		{
			if (r1<=network_data.activity_location_data_array[destination_activity_location_index].destination_link_choice_cdf_array[il])
			{
				destination_link_index = network_data.activity_location_data_array[destination_activity_location_index].destination_link_index_array[il];
				break;
			}
		}
	}
	else
	{
		destination_link_index = network_data.activity_location_data_array[destination_activity_location_index].destination_link_index_array[0];
	}
	return destination_link_index;
};

void demand_data_information::read_vehicle_dat_dsp(string input_dir_name,ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data)
{
	RngStream rng_demand("Demand");
	rng_demand.SetSeed(scenario_data.iseed);
	float r1;

	//node
	string file_name = input_dir_name + "vehicle.dat";
	string line;
	ifstream input_file(file_name);
	int departure_time_of_last_vehicle = 0;

	//initialization
	demand_data.vehilce_data_array.clear();
	demand_data.vehicle_id_index_map.clear();

	demand_data.time_dependent_vehicle_index_array.resize(scenario_data.num_simulation_intervals*scenario_data.simulation_interval_length);

	demand_data.first_vehicle_departure_time = 24*60*60;
	demand_data.last_vehicle_departure_time = 0;

	VehicleData vehicle_data;
	int num_vehicles;
	int max_num_of_stops;
	if(input_file.is_open()) 
	{ 
		int iline = 0;
		int iline_start;
		int iline_end;
		int vehicle_index = -1;
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
			if (iline == 1)
			{
				for (int j=0;j<tokens.size();j++)
				{
					stringstream sss(tokens[j]);
					if (j==0) sss>>num_vehicles;
					if (j==1) sss>>max_num_of_stops;
				}
				iline_start = iline + 2;
				iline_end = iline_start - 1 + num_vehicles*2;
			}
			if(iline >= iline_start && iline <= iline_end)
			{
//      #   usec   dsec   stime usrcls vehtype ioc #ONode #IntDe info ribf comp OZ
//      1    199    116    0.00     1     1     1    15     1     0  0.0000  0.0000    1
//           2   0.00
				if (iline%2 == 1)
				{//first line
					int vehicle_id;
					int unode_id_origin_link;
					int dnode_id_origin_link;
					float starting_time_in_minute;
					int user_class;
					int vehilce_type;
					int occupancy_level;
					int num_nodes_in_path;
					int num_destinations;
					int en_route_inforamtion_indicator;
					float indifference_band_in_minute;
					float compliance_rate;
					int origin_zone_id;

					for (int j=0;j<tokens.size();j++)
					{
						stringstream sss(tokens[j]);
						if (j==0) sss>>vehicle_id;
						if (j==1) sss>>unode_id_origin_link;
						if (j==2) sss>>dnode_id_origin_link;
						if (j==3) sss>>starting_time_in_minute;
						if (j==4) sss>>user_class;
						if (j==5) sss>>vehilce_type;
						if (j==6) sss>>occupancy_level;
						if (j==7) sss>>num_nodes_in_path;
						if (j==8) sss>>num_destinations;
						if (j==9) sss>>en_route_inforamtion_indicator;
						if (j==10) sss>>indifference_band_in_minute;
						if (j==11) sss>>compliance_rate;
						if (j==12) sss>>origin_zone_id;
					}

					int origin_zone_index = network_data.zone_id_index_map[origin_zone_id];
					int origin_activity_location_index = origin_zone_index;
					int unode_index_origin_link = network_data.node_id_index_map[unode_id_origin_link];
					int dnode_index_origin_link = network_data.node_id_index_map[dnode_id_origin_link];
					int origin_link_index = network_data.node_link_map[make_pair(unode_index_origin_link,dnode_index_origin_link)];
					int departure_time = int(starting_time_in_minute * 60);
					
					if (departure_time_of_last_vehicle<=departure_time)
					{
						departure_time_of_last_vehicle = departure_time;
					}

					vehicle_index = vehicle_index + 1;
					vehicle_data.set_vehicle_index(vehicle_index);
					vehicle_data.set_vehicle_id(vehicle_id);

					vehicle_data.set_origin_zone_index(origin_zone_index);
					vehicle_data.set_origin_activity_location_index(origin_activity_location_index);
					vehicle_data.set_origin_link_index(origin_link_index);
					vehicle_data.set_seed(scenario_data.iseed+vehicle_index);
				
					int simulation_interval_index;
					simulation_interval_index = departure_time/scenario_data.simulation_interval_length;
					int assignment_interval_index;
					assignment_interval_index = int((simulation_interval_index+1) *scenario_data.simulation_interval_length / scenario_data.assignment_interval_length);

					vehicle_data.set_departure_assignment_interval_index(assignment_interval_index);
					vehicle_data.set_departure_simulation_interval_index(simulation_interval_index);
					vehicle_data.set_departure_time(departure_time);

					if (demand_data.first_vehicle_departure_time>=departure_time+scenario_data.simulation_start_time)
					{
						demand_data.first_vehicle_departure_time = departure_time+scenario_data.simulation_start_time;
					}

					if (demand_data.last_vehicle_departure_time<=departure_time+scenario_data.simulation_start_time)
					{
						demand_data.last_vehicle_departure_time = departure_time+scenario_data.simulation_start_time;
					}

				}
				else
				{//second line

					int destination_zone_id;
					float destination_duration;

					for (int j=0;j<tokens.size();j++)
					{
						stringstream sss(tokens[j]);
						if (j==0) sss>>destination_zone_id;
						if (j==1) sss>>destination_duration;
					}

					int destination_zone_index = network_data.zone_id_index_map[destination_zone_id];
					int destination_activity_location_index = destination_zone_index;

					//destination link choice
					int destination_link_index = -1;
					r1 = (float) rng_demand.RandU01();
					destination_link_index = destination_link_choice_model(destination_activity_location_index,network_data,r1);

					vehicle_data.set_destination_zone_index(destination_zone_index);
					vehicle_data.set_destination_activity_location_index(destination_activity_location_index);
					vehicle_data.set_destination_link_index(destination_link_index);

					vehicle_data.init();				
					demand_data.vehilce_data_array.push_back(vehicle_data);
					demand_data.time_dependent_vehicle_index_array[vehicle_data.get_departure_time()].push_back(vehicle_index);
				}
			}
		}
		demand_data.demand_vehicle_size = (int)demand_data.vehilce_data_array.size();
	}
	else
	{
		cout << "Cannot open file - "
			<< file_name
			<< endl;
	}

};

void demand_data_information::read_demand_vehicle_dsp(string input_dir_name,ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data)
{
	//
	read_vehicle_dat_dsp(input_dir_name,scenario_data,demand_data,network_data);
};