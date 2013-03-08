#ifndef NETWORK_MODELS_H
#define NETWORK_MODELS_H

//data
#include "network_information.h"


namespace network_models
{

	void initialization
			(ScenarioData& scenario_data, NetworkData& network_data, DemandData& demand_data, OperationData& operation_data)
	{
		network_models::network_information::scenario_data_information::scenario_data_initialization(scenario_data);
		network_models::network_information::network_data_information::network_data_initialization(network_data);
		network_models::network_information::operation_data_information::opearation_data_initialization(operation_data);
	};
	void read_data
		(ScenarioData& scenario_data, NetworkData& network_data, DemandData& demand_data, OperationData& operation_data)
	{
		string input_dir_name = scenario_data.input_dir_name;
	
		//read data from files
		cout<<"reading scenario..."<<endl;
		read_scenario_data(input_dir_name,scenario_data);
	
		if (scenario_data.input_data_format == INPUT_FROM_DSP)
		{
			read_scenario_data_dsp(input_dir_name,scenario_data);
			cout<<"reading network..."<<endl;
			read_network_data_dsp(input_dir_name,network_data);
			cout<<"reading vehicle..."<<endl;
			read_demand_vehicle_dsp(input_dir_name,scenario_data,demand_data,network_data);
			cout<<"reading operation..."<<endl;
			read_operation_data(input_dir_name,scenario_data,operation_data,network_data);

			return;
		}

		if (scenario_data.input_data_format == INPUT_FROM_OD ||
			scenario_data.input_data_format == INPUT_FROM_VEHICLE)
		{
			cout<<"reading network..."<<endl;
			read_network_data(input_dir_name,network_data,scenario_data);
	
			//if (scenario_data.demand_od_flag == 1)
			if (scenario_data.input_data_format == INPUT_FROM_OD)
			{
				cout<<"reading od..."<<endl;
				read_demand_od(input_dir_name,scenario_data,demand_data,network_data);
			}
			else
			{
				cout<<"reading vehicle..."<<endl;
				read_demand_vehicle(input_dir_name,scenario_data,demand_data,network_data);
			}
			cout<<"reading operation..."<<endl;
			read_operation_data(input_dir_name,scenario_data,operation_data,network_data);
			return;
		}
	};

	void write_data(string output_dir_name,ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data,OperationData &operation_data)
	{
		//network
		cout<<"writing network..."<<endl;
		::write_network_data(output_dir_name,network_data);
		cout<<"writing vehicle..."<<endl;
		::write_demand_vehicle(output_dir_name,scenario_data,demand_data,network_data);
		cout<<"writing operation..."<<endl;
		::write_operation_data(output_dir_name,scenario_data,operation_data,network_data);
		cout<<"writing scenario..."<<endl;
		::write_scenario_data(scenario_data);		
	};

}

#endif