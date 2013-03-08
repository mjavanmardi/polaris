#include "network_vehicle_data.h"
#include "utilities.h"

using namespace std;
using namespace network_models::network_information::demand_data_information;

double VehicleData::get_random_number()
{
	return this->g.RandU01();

};
void VehicleData::set_simulation_status(Simulation_Vehicle_Status_Keys _simulation_status)
{
	this->simulation_status = _simulation_status;
};

Simulation_Vehicle_Status_Keys VehicleData::get_simulation_status()
{
	return this->simulation_status;
};

void VehicleData::set_vehicle_id(int _vehicle_id)
{
	this->vehicle_id = _vehicle_id;
};

int VehicleData::get_vehicle_id()
{
	return this->vehicle_id;
};

void VehicleData::set_vehicle_index(int _vehicle_index)
{
	this->vehicle_index = _vehicle_index;
};

int VehicleData::get_vehicle_index()
{
	return this->vehicle_index;
};

void VehicleData::set_origin_zone_index(int _origin_zone_index)
{
	this->origin_zone_index = _origin_zone_index;
};
int VehicleData::get_origin_zone_index()
{
	return this->origin_zone_index;
};

void VehicleData::set_destination_zone_index(int _destination_zone_index)
{
	this->destination_zone_index = _destination_zone_index;
};

int VehicleData::get_destination_zone_index()
{
	return this->destination_zone_index;
};

void VehicleData::set_origin_activity_location_index(int _origin_activity_location_index)
{
	this->origin_activity_location_index = _origin_activity_location_index;
};

int VehicleData::get_origin_activity_location_index()
{
	return this->origin_activity_location_index;
};

void VehicleData::set_destination_activity_location_index(int _destination_activity_location_index)
{
	this->destination_activity_location_index = _destination_activity_location_index;
};

int VehicleData::get_destination_activity_location_index()
{
	return this->destination_activity_location_index;
};

void VehicleData::set_origin_link_index(int _origin_link_index)
{
	this->origin_link_index = _origin_link_index;
};

int VehicleData::get_origin_link_index()
{
	return this->origin_link_index;
};

void VehicleData::set_destination_link_index(int _destination_link_index)
{
	this->destination_link_index = _destination_link_index;
};

int VehicleData::get_destination_link_index()
{
	return this->destination_link_index;
};

void VehicleData::set_departure_assignment_interval_index(int _departure_assignment_interval_index)
{
	this->departure_assignment_interval_index = _departure_assignment_interval_index;
};

int VehicleData::get_departure_assignment_interval_index()
{
	return this->departure_assignment_interval_index;
};

void VehicleData::set_departure_simulation_interval_index(int _departure_simulation_interval_index)
{
	this->departure_simulation_interval_index = _departure_simulation_interval_index;
};

int VehicleData::get_departure_simulation_interval_index()
{
	return this->departure_simulation_interval_index;
};

void VehicleData::set_departure_time(int _departure_time)
{
	this->departure_time = _departure_time;
};

int VehicleData::get_departure_time()
{
	return this->departure_time;
};


void VehicleData::set_arrival_time(int _arrival_time)
{
	this->arrival_time = _arrival_time;
};
int VehicleData::get_arrival_time()
{
	return this->arrival_time;
};

void VehicleData::set_arrival_simulation_interval_index(int _arrival_simulation_interval_index)
{
	this->arrival_simulation_interval_index = _arrival_simulation_interval_index;
};
int VehicleData::get_arrival_simulation_interval_index()
{
	return this->arrival_simulation_interval_index;
};

void VehicleData::set_arrival_assignment_interval_index(int _arrival_assignment_interval_index)
{
	this->arrival_assignment_interval_index = _arrival_assignment_interval_index;
};
int VehicleData::get_arrival_assignment_interval_index()
{
	return this->arrival_assignment_interval_index;
};

void VehicleData::init()
{
	this->current_link = -1;
	this->simulation_status = UNLOADED;
	this->vehicle_trajectory_data_array.clear();
	this->route_link_size = 0;
	this->route_index = -1;
	//this->set_seed();
};

void VehicleData::set_seed(int iseed)
{
	this->g.SetSeed(vehicle_index+iseed+1);
};

void VehicleData::clear()
{
	this->vehicle_trajectory_data_array.clear();
	this->current_link = -1;
};
void VehicleData::load_to_entry_queue()
{

	this->set_simulation_status(IN_ENTRY_QUEUE);
};
void VehicleData::load_to_origin_link(int simulation_interval_index, int simulation_interval_length)
{
	this->current_link = 0;
	this->simulation_status = IN_NETWORK;
	int current_time_in_second = simulation_interval_index*simulation_interval_length;
	this->vehicle_trajectory_data_array[this->current_link].enter_time = current_time_in_second;
	this->vehicle_trajectory_data_array[this->current_link].enter_interval_index = simulation_interval_index;
};
		
void VehicleData::arrive_to_destination_link(int simulation_interval_index, int simulation_interval_length)
{
	int current_time_in_second = simulation_interval_index*simulation_interval_length;
	this->simulation_status = OUT_NETWORK;
	this->vehicle_trajectory_data_array[this->current_link].delayed_time = 0;
	this->set_arrival_time (current_time_in_second);
	this->set_arrival_simulation_interval_index(simulation_interval_index);
};

void VehicleData::set_route_index(int _route_index,int _route_link_size, int * _route_link_array_ptr)
{
	this->route_index = _route_index;
	this->route_link_size = _route_link_size;
	this->vehicle_trajectory_data_array.resize(route_link_size);
	for (int i=0;i<this->route_link_size;i++)
	{
		this->vehicle_trajectory_data_array[i].link_index = _route_link_array_ptr[i];
	}
};
		
int VehicleData::get_route_index()
{
	return this->route_index;
};

int VehicleData::get_next_link_index()
{
	if (this->current_link<this->route_link_size)
	{
		return this->vehicle_trajectory_data_array[this->current_link+1].link_index;
	}
	else
	{
		return -1;
	}
};
int VehicleData::get_current_link_index()
{
	return this->vehicle_trajectory_data_array[this->current_link].link_index;
};
void VehicleData::set_route_links(int origin_index, int destination_index,vector<int> route_link_array)
{
	
	vector<int> tmp_route_link_array;
	tmp_route_link_array.push_back(destination_index);
	int next_link_index = route_link_array[destination_index];
	while(next_link_index != -1)
	{
		tmp_route_link_array.push_back(next_link_index);
		next_link_index = route_link_array[next_link_index];
	}
	this->route_link_size = (int) tmp_route_link_array.size();
	
	this->vehicle_trajectory_data_array.clear();

	for (int i=this->route_link_size-1;i>=0;i--)
	{
		VehicleTrajectoryData vehicle_trajectory_data;
		vehicle_trajectory_data.link_index = tmp_route_link_array[i];
		vehicle_trajectory_data.enter_time = 0;
		vehicle_trajectory_data.delayed_time = 0;
		this->vehicle_trajectory_data_array.push_back(vehicle_trajectory_data);
	}
};

void VehicleData::transfer_to_next_link(int simulation_interval_index, int simulation_interval_length, int delayed_time)
{
	//add exit time to trajectory data
	int current_time_in_second = simulation_interval_index*simulation_interval_length;
	this->vehicle_trajectory_data_array[this->current_link].delayed_time = delayed_time;
	
	//add enter time to the trajectory data
	this->current_link++;
	this->vehicle_trajectory_data_array[this->current_link].enter_time = current_time_in_second;
	this->vehicle_trajectory_data_array[this->current_link].enter_interval_index = simulation_interval_index;
};

int VehicleData::get_current_link_enter_time()
{
	return this->vehicle_trajectory_data_array[this->current_link].enter_time;
};

int VehicleData::get_current_link_enter_interval_index()
{
	return this->vehicle_trajectory_data_array[this->current_link].enter_interval_index;
};

int VehicleData::get_route_link_size()
{
	return this->route_link_size;
};


int VehicleData::get_route_link_index(int link)
{
	return this->vehicle_trajectory_data_array[link].link_index;
};
int VehicleData::get_route_link_enter_time(int link)
{
	return this->vehicle_trajectory_data_array[link].enter_time;
};

int VehicleData::get_route_link_delayed_time(int link)
{
	return this->vehicle_trajectory_data_array[link].delayed_time;
};

int VehicleData::get_route_link_exit_time(int link)
{
	int route_link_exit_time = 0;
	if (link<this->route_link_size-1)
	{
		route_link_exit_time = this->vehicle_trajectory_data_array[link+1].enter_time;
	}
	else
	{
		route_link_exit_time = this->vehicle_trajectory_data_array[link].enter_time + this->vehicle_trajectory_data_array[link].delayed_time;
	}
	return route_link_exit_time;
};
