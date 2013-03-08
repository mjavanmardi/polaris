#include "network_data.h"

//#ifdef LINUX
//// for map
//using namespace __gnu_cxx;
//#endif

using namespace std;
using namespace network_models::network_information;
void network_data_information::network_data_initialization(NetworkData& network_data)
{
	network_data.node_type_int_string_map.clear();
	network_data.node_type_int_string_map.insert(make_pair(NO_CONTROL,"NO_CONTROL"));
	network_data.node_type_int_string_map.insert(make_pair(YIELD_SIGN,"YIELD_SIGN"));
	network_data.node_type_int_string_map.insert(make_pair(ALL_WAY_STOP_SIGN,"ALL_WAY_STOP_SIGN"));
	network_data.node_type_int_string_map.insert(make_pair(TWO_WAY_STOP_SIGN,"TWO_WAY_STOP_SIGN"));
	network_data.node_type_int_string_map.insert(make_pair(PRE_TIMED_SIGNAL_CONTROL,"PRE_TIMED_SIGNAL_CONTROL"));
	network_data.node_type_int_string_map.insert(make_pair(ACTUATED_SIGNAL_CONTROL,"ACTUATED_SIGNAL_CONTROL"));
	network_data.node_type_int_string_map.insert(make_pair(ACTUATED_SIGNAL_CONTROL,"ACTUATED_SIGNAL_CONTROL"));

	network_data.node_type_string_int_map.clear();
	network_data.node_type_string_int_map.insert(make_pair("NO_CONTROL",NO_CONTROL));
	network_data.node_type_string_int_map.insert(make_pair("YIELD_SIGN",YIELD_SIGN));
	network_data.node_type_string_int_map.insert(make_pair("ALL_WAY_STOP_SIGN",ALL_WAY_STOP_SIGN));
	network_data.node_type_string_int_map.insert(make_pair("TWO_WAY_STOP_SIGN",TWO_WAY_STOP_SIGN));
	network_data.node_type_string_int_map.insert(make_pair("PRE_TIMED_SIGNAL_CONTROL",PRE_TIMED_SIGNAL_CONTROL));
	network_data.node_type_string_int_map.insert(make_pair("ACTUATED_SIGNAL_CONTROL",ACTUATED_SIGNAL_CONTROL));
	network_data.node_type_string_int_map.insert(make_pair("ACTUATED_SIGNAL_CONTROL",ACTUATED_SIGNAL_CONTROL));

	network_data.link_type_int_string_map.clear();
	network_data.link_type_int_string_map.insert(make_pair(FREEWAY,"FREEWAY"));
	network_data.link_type_int_string_map.insert(make_pair(ON_RAMP,"ON_RAMP"));
	network_data.link_type_int_string_map.insert(make_pair(OFF_RAMP,"OFF_RAMP"));
	network_data.link_type_int_string_map.insert(make_pair(EXPRESSWAY,"EXPRESSWAY"));
	network_data.link_type_int_string_map.insert(make_pair(ARTERIAL,"ARTERIAL"));

	network_data.link_type_string_int_map.clear();
	network_data.link_type_string_int_map.insert(make_pair("FREEWAY",FREEWAY));
	network_data.link_type_string_int_map.insert(make_pair("ON_RAMP",ON_RAMP));
	network_data.link_type_string_int_map.insert(make_pair("OFF_RAMP",OFF_RAMP));
	network_data.link_type_string_int_map.insert(make_pair("EXPRESSWAY",EXPRESSWAY));
	network_data.link_type_string_int_map.insert(make_pair("ARTERIAL",ARTERIAL));

	network_data.turn_movement_type_int_string_map.clear();
	network_data.turn_movement_type_int_string_map.insert(make_pair(LEFT_TURN,"LEFT_TURN"));
	network_data.turn_movement_type_int_string_map.insert(make_pair(THROUGH_TURN,"THROUGH_TURN"));
	network_data.turn_movement_type_int_string_map.insert(make_pair(RIGHT_TURN,"RIGHT_TURN"));
	network_data.turn_movement_type_int_string_map.insert(make_pair(U_TURN,"U_TURN"));
	network_data.turn_movement_type_int_string_map.insert(make_pair(MERGE_RIGHT,"MERGE_RIGHT"));
	network_data.turn_movement_type_int_string_map.insert(make_pair(MERGE_LEFT,"MERGE_LEFT"));
	network_data.turn_movement_type_int_string_map.insert(make_pair(DIVERGE_RIGHT,"DIVERGE_RIGHT"));
	network_data.turn_movement_type_int_string_map.insert(make_pair(DIVERGE_LEFT,"DIVERGE_LEFT"));

	network_data.turn_movement_type_string_int_map.clear();
	network_data.turn_movement_type_string_int_map.insert(make_pair("LEFT_TURN",LEFT_TURN));
	network_data.turn_movement_type_string_int_map.insert(make_pair("THROUGH_TURN",THROUGH_TURN));
	network_data.turn_movement_type_string_int_map.insert(make_pair("RIGHT_TURN",RIGHT_TURN));
	network_data.turn_movement_type_string_int_map.insert(make_pair("U_TURN",U_TURN));
	network_data.turn_movement_type_string_int_map.insert(make_pair("MERGE_RIGHT",MERGE_RIGHT));
	network_data.turn_movement_type_string_int_map.insert(make_pair("MERGE_LEFT",MERGE_LEFT));
	network_data.turn_movement_type_string_int_map.insert(make_pair("DIVERGE_RIGHT",DIVERGE_RIGHT));
	network_data.turn_movement_type_string_int_map.insert(make_pair("DIVERGE_LEFT",DIVERGE_LEFT));

	network_data.turn_movement_rule_int_string_map.clear();
	network_data.turn_movement_rule_int_string_map.insert(make_pair(PROHIBITED,"PROHIBITED"));
	network_data.turn_movement_rule_int_string_map.insert(make_pair(ALLOWED,"ALLOWED"));

	network_data.turn_movement_rule_string_int_map.clear();
	network_data.turn_movement_rule_string_int_map.insert(make_pair("PROHIBITED",PROHIBITED));
	network_data.turn_movement_rule_string_int_map.insert(make_pair("ALLOWED",ALLOWED));
};
void network_data_information::write_network_data(string output_dir_name, NetworkData& network_data)
{
	//node
	write_node(output_dir_name,network_data);
	//link
	write_link(output_dir_name,network_data);
	//movement
	write_turn_movement(output_dir_name,network_data);
	//zone
	write_zone(output_dir_name,network_data);
	//activity location
	write_activity_location(output_dir_name,network_data);
};
void network_data_information::write_node(string output_dir_name, NetworkData& network_data)
{
	//node
	string file_name = output_dir_name + "output_node";
	fstream file;
	file.open(file_name,fstream::out);
	if(file.is_open()) 
	{ 
		file 
			<< "node" <<  "	"
			<< "x" <<  "	"
			<< "y" << "	"
			<< "type"
			<<endl;
	}
	else
	{
		cout << "Cannot open file - "
			<< file_name
			<< endl;
	}

	for (int i=0;i<network_data.network_node_size;i++)
	{
		file
			<< network_data.node_data_array[i].uuid<< "	"
			<< network_data.node_data_array[i].x<< "	"
			<< network_data.node_data_array[i].y<< "	"
			<< network_data.node_type_int_string_map[network_data.node_data_array[i].node_type];
		
		if (i<network_data.network_node_size-1)
		{
			file << endl;
		}
			
	}

	file.close();
};
void network_data_information::write_link(string output_dir_name, NetworkData& network_data)
{
	//node
	string file_name = output_dir_name + "output_link";
	fstream file;
	file.open(file_name,fstream::out);
	if(file.is_open()) 
	{
											
		file 
			<< "link" <<  "	"
			<< "unode" <<  "	"
			<< "dnode" << "	"
			<< "length" << "	"
			<< "free_flow_speed" << "	"
			<< "capacity" << "	"
			<< "type" << "	"
			<< "num_lanes" << "	"
			<< "num_left_turn_bays" << "	"
			<< "num_right_turn_bays" << "	"
			<< "left_turn_bay_length" << "	"
			<< "right_turn_bay_length" << " "
			<< "original_free_flow_speed"
			<<endl;
	}
	else
	{
		cout << "Cannot open file - "
			<< file_name
			<< endl;
	}

	for (int i=0;i<network_data.network_link_size;i++)
	{
		file
			<< network_data.link_data_array[i].uuid<< "	"
			<< network_data.node_data_array[network_data.link_data_array[i].unode_index].uuid<< "	"
			<< network_data.node_data_array[network_data.link_data_array[i].dnode_index].uuid<< "	"
			<< network_data.link_data_array[i].length<< "	"
			<< network_data.link_data_array[i].free_flow_speed<< "	"
			<< network_data.link_data_array[i].maximum_flow_rate<< "	"
			<< network_data.link_type_int_string_map[network_data.link_data_array[i].link_type]<< "	"
			<< network_data.link_data_array[i].num_lanes<< "	"
			<< network_data.link_data_array[i].num_left_turn_bays<< "	"
			<< network_data.link_data_array[i].num_right_turn_bays<< "	"
			<< network_data.link_data_array[i].left_turn_bay_length<< "	"
			<< network_data.link_data_array[i].right_turn_bay_length;

		if (i<network_data.network_link_size-1)
		{
			file << endl;
		}
			
	}

	file.close();

};
void network_data_information::write_turn_movement(string output_dir_name, NetworkData& network_data)
{
	//turn movement
	string file_name = output_dir_name + "output_turn_movement";
	fstream file;
	file.open(file_name,fstream::out);
	if(file.is_open()) 
	{														
		file 
			<< "movement" <<  "	"
			<< "inbound_link" <<  "	"
			<< "outbound_link" << "	"
			<< "type" << "	"
			<< "rule" 
			<<endl;
	}
	else
	{
		cout << "Cannot open file - "
			<< file_name
			<< endl;
	}

	for (int i=0;i<network_data.network_turn_movement_size;i++)
	{
		file
			<< network_data.turn_movement_data_array[i].uuid<< "	"
			<< network_data.link_data_array[network_data.turn_movement_data_array[i].inbound_link_index].uuid<< "	"
			<< network_data.link_data_array[network_data.turn_movement_data_array[i].outbound_link_index].uuid<< "	"
			<< network_data.turn_movement_type_int_string_map[network_data.turn_movement_data_array[i].turn_movement_type]<< "	"
			<< network_data.turn_movement_rule_int_string_map[network_data.turn_movement_data_array[i].turn_movement_rule];

		if (i<network_data.network_turn_movement_size-1)
		{
			file<< endl;
		}
			
	}

	file.close();

};
void network_data_information::write_zone(string output_dir_name, NetworkData& network_data)
{
	//zone
	string file_name = output_dir_name + "output_zone";
	fstream file;
	file.open(file_name,fstream::out);
	if(file.is_open()) 
	{
															
		file 
			<< "zone" 
			<<endl;
	}
	else
	{
		cout << "Cannot open file - "
			<< file_name
			<< endl;
	}

	for (int i=0;i<network_data.network_zone_size;i++)
	{
		file
			<< network_data.zone_data_array[i].uuid;
		
		if (i<network_data.network_zone_size-1)
		{
			file << endl;
		}
			
	}

	file.close();

};
void network_data_information::write_activity_location(string output_dir_name, NetworkData& network_data)
{
	//activity_location
	string file_name = output_dir_name + "output_activity_location";
	fstream file;
	file.open(file_name,fstream::out);
	if(file.is_open()) 
	{
		file 
			<< "activity_location" << "	"
			<< "zone" << "	"
			<< "num_origin_links" << "	"
			<< "num_destination_links"
			<<endl;
	}
	else
	{
		cout << "Cannot open file - "
			<< file_name
			<< endl;
	}

	for (int i=0;i<network_data.network_activity_location_size;i++)
	{
		file
			<< network_data.activity_location_data_array[i].uuid << "	"
			<< network_data.zone_data_array[network_data.activity_location_data_array[i].zone_index].uuid << "	"
			<< network_data.activity_location_data_array[i].num_origin_links << "	"
			<< network_data.activity_location_data_array[i].num_destination_links
			<< endl;
		
		if (network_data.activity_location_data_array[i].num_origin_links>0)
		{
			for (int j=0;j<network_data.activity_location_data_array[i].num_origin_links;j++)
			{
				file<< network_data.link_data_array[network_data.activity_location_data_array[i].origin_link_index_array[j]].uuid << "	";
			}
			file<< endl;
		}
		else
		{
			file<< "-1" <<endl;
		}

		if (network_data.activity_location_data_array[i].num_destination_links>0)
		{
			for (int j=0;j<network_data.activity_location_data_array[i].num_destination_links;j++)
			{
				file<< network_data.link_data_array[network_data.activity_location_data_array[i].destination_link_index_array[j]].uuid << "	";
			}
			if (i<network_data.network_activity_location_size-1)
			{
				file<< endl;
			}
		}
		else
		{
			file<< "-1";
			if (i<network_data.network_activity_location_size-1)
			{
				file<< endl;
			}

		}

	}
	file.close();
};

//void network_data_information::read_network_data_transims(NetworkData& network_data)
//{
//
//};
//void network_data_information::read_network_data_two_links(NetworkData& network_data)
//{
//	int i = 0;
//	float link_length = 5280.0;
//	float speed_limit = 60.0;
//	float speed_limit_ramp = 30.0;
//	float maximum_flow_rate = 2200.0;
//	float maximum_flow_rate_ramp = 600.0;
//	float maximum_flow_rate_arterial = 900;
//	float jam_density = 220.0;
//	float backward_wave_speed = 12.0;
//	float distance_factor = 1.5;
//	
//	network_data.max_free_flow_speed = -1;
//
//	network_data.node_data_array.clear();
//	
//	//node
//	network_information::network_data_information::NodeData node_data;
//
//	i = 0;
//	node_data.node_index = i;
//	
//	node_data.uuid = 1;
//	node_data.x = 0.0;
//	node_data.y = (float) 2.0*link_length;
//	node_data.node_type = NO_CONTROL;
//	network_data.node_data_array.push_back(node_data);
//
//	i = 1;
//	node_data.node_index = i;
//	
//	node_data.uuid = 2;
//	node_data.x = (float) i*link_length;
//	node_data.y = (float) 2.0*link_length;
//	node_data.node_type = NO_CONTROL;
//	network_data.node_data_array.push_back(node_data);
//
//	i = 2;
//	node_data.node_index = i;
//	
//	node_data.uuid = 3;
//	node_data.x = (float) i*link_length;
//	node_data.y = (float) 2.0*link_length;
//	node_data.node_type = NO_CONTROL;
//	network_data.node_data_array.push_back(node_data);
//
//	//link
//	network_data.link_data_array.clear();
//	network_information::network_data_information::LinkData link_data;
//
//	i = 0;
//	link_data.link_index = i;
//	link_data.unode_index = 0;
//	link_data.dnode_index = 1;
//
//	link_data.uuid = 1;
//	link_data.oppersite_link_index = -1;
//	link_data.num_lanes = 2;
//	link_data.link_type = FREEWAY;
//	link_data.length = link_length*distance_factor;
//	link_data.speed_limit = speed_limit;
//	link_data.maximum_flow_rate = maximum_flow_rate;
//	link_data.backward_wave_speed = backward_wave_speed;
//	link_data.free_flow_speed = float (speed_limit + 15.0);
//	link_data.jam_density = jam_density;
//	link_data.left_turn_bay_length = 0.0;
//	link_data.right_turn_bay_length = 0.0;
//	link_data.num_left_turn_bays = 0;
//	link_data.num_right_turn_bays = 0;
//	network_data.link_data_array.push_back(link_data);
//	network_data.max_free_flow_speed = max(network_data.max_free_flow_speed,link_data.free_flow_speed);
//
//	i = 1;
//	link_data.link_index = i;
//	link_data.unode_index = 1;
//	link_data.dnode_index = 2;
//
//	link_data.uuid = 2;
//	link_data.oppersite_link_index = -1;
//	link_data.num_lanes = 2;
//	link_data.link_type = FREEWAY;
//	link_data.length = link_length*distance_factor;
//	link_data.speed_limit = speed_limit;
//	link_data.maximum_flow_rate = maximum_flow_rate;
//	link_data.backward_wave_speed = backward_wave_speed;
//	link_data.free_flow_speed =  float (speed_limit + 15.0);
//	link_data.jam_density = jam_density;
//	link_data.left_turn_bay_length = 0.0;
//	link_data.right_turn_bay_length = 0.0;
//	link_data.num_left_turn_bays = 0;
//	link_data.num_right_turn_bays = 0;
//	network_data.link_data_array.push_back(link_data);
//	network_data.max_free_flow_speed = max(network_data.max_free_flow_speed,link_data.free_flow_speed);
//
//	//turn
//	network_data.turn_movement_data_array.clear();
//	network_information::network_data_information::TurnMovementData turn_movement_data;
//
//	i=0;
//	turn_movement_data.turn_movement_index = i;
//	turn_movement_data.inbound_link_index = 0;
//	turn_movement_data.outbound_link_index = 1;
//	
//	turn_movement_data.uuid = 1;
//	turn_movement_data.turn_movement_type = THROUGH_TURN;
//	turn_movement_data.turn_movement_rule = ALLOWED;
//	network_data.turn_movement_data_array.push_back(turn_movement_data);
//
//	//activity location
//	network_data.activity_location_data_array.clear();
//	network_information::network_data_information::ActivityLocationData activity_location_data;
//
//	i=0;
//	activity_location_data.activity_location_index = i;
//
//	activity_location_data.uuid = 1;
//	activity_location_data.origin_link_index_array.clear();
//	activity_location_data.destination_link_index_array.clear();
//	activity_location_data.origin_link_index_array.push_back(0);
//	activity_location_data.num_origin_links = (int) activity_location_data.origin_link_index_array.size();
//	activity_location_data.num_destination_links = (int) activity_location_data.destination_link_index_array.size();
//	activity_location_data.zone_index = i;
//	network_data.activity_location_data_array.push_back(activity_location_data);
//
//	i=1;
//	activity_location_data.activity_location_index = i;
//	
//	activity_location_data.uuid = 2;
//	activity_location_data.origin_link_index_array.clear();
//	activity_location_data.destination_link_index_array.clear();
//	activity_location_data.destination_link_index_array.push_back(1);
//	activity_location_data.num_origin_links = (int) activity_location_data.origin_link_index_array.size();
//	activity_location_data.num_destination_links = (int) activity_location_data.destination_link_index_array.size();
//	activity_location_data.zone_index = i;
//	network_data.activity_location_data_array.push_back(activity_location_data);
//
//	network_data.network_activity_location_size = (int) network_data.activity_location_data_array.size();
//	network_data.network_node_size = (int) network_data.node_data_array.size();
//	network_data.network_link_size = (int) network_data.link_data_array.size();
//	network_data.network_turn_movement_size = (int) network_data.turn_movement_data_array.size();
//
//	//turn movement
//	for (int i=0;i<network_data.network_turn_movement_size;i++)
//	{
//		//outbound turn movement of an inbound link
//		int inbound_link_index = network_data.turn_movement_data_array[i].inbound_link_index;
//		network_data.link_data_array[inbound_link_index].outbound_turn_movement_index_array.push_back(i);
//		
//		//inbound turn movement of an outbound link
//		int outbound_link_index = network_data.turn_movement_data_array[i].outbound_link_index;
//		network_data.link_data_array[outbound_link_index].inbound_turn_movement_index_array.push_back(i);
//
//		//printf("tm_index= %d il=%d ol=%d\n",i,network_data.turn_movement_data_array[i].inbound_link_index,network_data.turn_movement_data_array[i].outbound_link_index);
//	}
//
//	//link
//	for(int i=0;i<network_data.network_link_size;i++)
//	{
//		//outbound turn movement size
//		network_data.link_data_array[i].outbound_turn_movement_size = (int) network_data.link_data_array[i].outbound_turn_movement_index_array.size();
//		//outbound link
//		int unode_index = network_data.link_data_array[i].unode_index;
//		network_data.node_data_array[unode_index].outbound_link_index_array.push_back(i);
//		
//		//inbound turn movement size
//		network_data.link_data_array[i].inbound_turn_movement_size = (int) network_data.link_data_array[i].inbound_turn_movement_index_array.size();
//		//inbound link
//		int dnode_index = network_data.link_data_array[i].dnode_index;
//		network_data.node_data_array[dnode_index].inbound_link_index_array.push_back(i);
//		//printf("link_index= %d unode=%d dnode=%d outboud_tm_size= %d\n",i,network_data.link_data_array[i].unode_index,network_data.link_data_array[i].dnode_index,network_data.link_data_array[i].outbound_turn_movement_size);
//	}
//	//node
//	for (int i=0;i<network_data.network_node_size;i++)
//	{
//		//network_data.node_data_array[i].g.SetSeed(i+1234567);
//		//outbound link size
//		network_data.node_data_array[i].outbound_link_size = (int) network_data.node_data_array[i].outbound_link_index_array.size();
//
//		//inbound link size
//		network_data.node_data_array[i].inbound_link_size = (int) network_data.node_data_array[i].inbound_link_index_array.size();
//
//		//printf("node_index= %d outboud_link_size= %d\n",i,network_data.node_data_array[i].outbound_link_size);
//	}
//
//	//link - turn movement map
//	network_data.link_turn_movement_map.clear();
//	for (int i=0;i<network_data.network_turn_movement_size;i++)
//	{
//		int inbound_link_index = network_data.turn_movement_data_array[i].inbound_link_index;
//		int outbond_link_index = network_data.turn_movement_data_array[i].outbound_link_index;
//		network_data.link_turn_movement_map.insert(make_pair(make_pair(inbound_link_index,outbond_link_index),i));
//	}
//};

//void network_data_information::read_network_data_merged_highway(NetworkData& network_data)
//{
//	int i = 0;
//	float link_length = 5280.0;
//	float speed_limit = 60.0;
//	float speed_limit_ramp = 30.0;
//	float maximum_flow_rate = 2200.0;
//	float maximum_flow_rate_ramp = 600.0;
//	float maximum_flow_rate_arterial = 900.0;
//	float jam_density = 220.0;
//	float backward_wave_speed = 12.0;
//	float distance_factor = 1.5;
//	
//	network_data.max_free_flow_speed = -1;
//
//	network_data.node_data_array.clear();
//	
//	//node
//	network_information::network_data_information::NodeData node_data;
//
//	i = 0;
//	node_data.node_index = i;
//	
//	node_data.uuid = 1;
//	node_data.x = 0.0;
//	node_data.y = (float) 2.0*link_length;
//	node_data.node_type = NO_CONTROL;
//	network_data.node_data_array.push_back(node_data);
//
//	i = 1;
//	node_data.node_index = i;
//	
//	node_data.uuid = 2;
//	node_data.x = (float) i*link_length;
//	node_data.y = (float) 2.0*link_length;
//	node_data.node_type = NO_CONTROL;
//	network_data.node_data_array.push_back(node_data);
//
//	i = 2;
//	node_data.node_index = i;
//	
//	node_data.uuid = 3;
//	node_data.x = (float) i*link_length;
//	node_data.y = (float) 2.0*link_length;
//	node_data.node_type = NO_CONTROL;
//	network_data.node_data_array.push_back(node_data);
//
//	i = 3;
//	node_data.node_index = i;
//	
//	node_data.uuid = 4;
//	node_data.x = (float) i*link_length;
//	node_data.y = (float) 2.0*link_length;
//	node_data.node_type = NO_CONTROL;
//	network_data.node_data_array.push_back(node_data);
//
//	i = 4;
//	node_data.node_index = i;
//	
//	node_data.uuid = 5;
//	node_data.x = (float) i*link_length;
//	node_data.y = (float) 2.0*link_length;
//	node_data.node_type = NO_CONTROL;
//	network_data.node_data_array.push_back(node_data);
//
//	i = 5;
//	node_data.node_index = i;
//
//	node_data.uuid = 6;
//	node_data.x = (float) 1.0*link_length;
//	node_data.y = (float) 1.0*link_length;
//	node_data.node_type = NO_CONTROL;
//	network_data.node_data_array.push_back(node_data);
//
//	i = 6;
//	node_data.node_index = i;
//	
//	node_data.uuid = 7;
//	node_data.x = 0.0;
//	node_data.y = 0.0;
//	node_data.node_type = NO_CONTROL;
//	network_data.node_data_array.push_back(node_data);
//
//	//link
//	network_data.link_data_array.clear();
//	network_information::network_data_information::LinkData link_data;
//
//	i = 0;
//	link_data.link_index = i;
//	link_data.unode_index = 0;
//	link_data.dnode_index = 1;
//
//	link_data.uuid = 1;
//	link_data.oppersite_link_index = -1;
//	link_data.num_lanes = 2;
//	link_data.link_type = FREEWAY;
//	link_data.length = link_length*distance_factor;
//	link_data.speed_limit = speed_limit;
//	link_data.maximum_flow_rate = maximum_flow_rate;
//	link_data.backward_wave_speed = backward_wave_speed;
//	link_data.free_flow_speed = float (speed_limit + 15.0);
//	link_data.jam_density = jam_density;
//	link_data.left_turn_bay_length = 0.0;
//	link_data.right_turn_bay_length = 0.0;
//	link_data.num_left_turn_bays = 0;
//	link_data.num_right_turn_bays = 0;
//	network_data.link_data_array.push_back(link_data);
//	network_data.max_free_flow_speed = max(network_data.max_free_flow_speed,link_data.free_flow_speed);
//
//	i = 1;
//	link_data.link_index = i;
//	link_data.unode_index = 1;
//	link_data.dnode_index = 2;
//
//	link_data.uuid = 2;
//	link_data.oppersite_link_index = -1;
//	link_data.num_lanes = 2;
//	link_data.link_type = FREEWAY;
//	link_data.length = link_length*distance_factor;
//	link_data.speed_limit = speed_limit;
//	link_data.maximum_flow_rate = maximum_flow_rate;
//	link_data.backward_wave_speed = backward_wave_speed;
//	link_data.free_flow_speed =  float (speed_limit + 15.0);
//	link_data.jam_density = jam_density;
//	link_data.left_turn_bay_length = 0.0;
//	link_data.right_turn_bay_length = 0.0;
//	link_data.num_left_turn_bays = 0;
//	link_data.num_right_turn_bays = 0;
//	network_data.link_data_array.push_back(link_data);
//	network_data.max_free_flow_speed = max(network_data.max_free_flow_speed,link_data.free_flow_speed);
//
//	i = 2;
//	link_data.link_index = i;
//	link_data.unode_index = 2;
//	link_data.dnode_index = 3;
//
//	link_data.uuid = 3;
//	link_data.oppersite_link_index = -1;
//	link_data.num_lanes = 2;
//	link_data.link_type = FREEWAY;
//	link_data.length = link_length*distance_factor;
//	link_data.speed_limit = speed_limit;
//	link_data.maximum_flow_rate = maximum_flow_rate;
//	link_data.backward_wave_speed = backward_wave_speed;
//	link_data.free_flow_speed =  float (speed_limit + 15.0);
//	link_data.jam_density = jam_density;
//	link_data.left_turn_bay_length = 0.0;
//	link_data.right_turn_bay_length = 0.0;
//	link_data.num_left_turn_bays = 0;
//	link_data.num_right_turn_bays = 0;
//	network_data.link_data_array.push_back(link_data);
//	network_data.max_free_flow_speed = max(network_data.max_free_flow_speed,link_data.free_flow_speed);
//
//	i = 3;
//	link_data.link_index = i;
//	link_data.unode_index = 3;
//	link_data.dnode_index = 4;
//
//	link_data.uuid = 4;
//	link_data.oppersite_link_index = -1;
//	link_data.num_lanes = 2;
//	link_data.link_type = FREEWAY;
//	link_data.length = link_length*distance_factor;
//	link_data.speed_limit = speed_limit;
//	link_data.maximum_flow_rate = maximum_flow_rate;
//	link_data.backward_wave_speed = backward_wave_speed;
//	link_data.free_flow_speed =  float (speed_limit + 15.0);
//	link_data.jam_density = jam_density;
//	link_data.left_turn_bay_length = 0.0;
//	link_data.right_turn_bay_length = 0.0;
//	link_data.num_left_turn_bays = 0;
//	link_data.num_right_turn_bays = 0;
//	network_data.link_data_array.push_back(link_data);
//	network_data.max_free_flow_speed = max(network_data.max_free_flow_speed,link_data.free_flow_speed);
//
//	i = 4;
//	link_data.link_index = i;
//	link_data.unode_index = 5;
//	link_data.dnode_index = 2;
//
//	link_data.uuid = 5;
//	link_data.oppersite_link_index = -1;
//	link_data.num_lanes = 1;
//	link_data.link_type = ON_RAMP;
//	link_data.length = link_length*distance_factor;
//	link_data.speed_limit = speed_limit_ramp;
//	link_data.maximum_flow_rate = maximum_flow_rate_ramp;
//	link_data.backward_wave_speed = backward_wave_speed;
//	link_data.free_flow_speed =  float (speed_limit + 15.0);
//	link_data.jam_density = jam_density;
//	link_data.left_turn_bay_length = 0.0;
//	link_data.right_turn_bay_length = 0.0;
//	link_data.num_left_turn_bays = 0;
//	link_data.num_right_turn_bays = 0;
//	network_data.link_data_array.push_back(link_data);
//	network_data.max_free_flow_speed = max(network_data.max_free_flow_speed,link_data.free_flow_speed);
//
//	i = 5;
//	link_data.link_index = i;
//	link_data.unode_index = 6;
//	link_data.dnode_index = 5;
//
//	link_data.uuid = 6;
//	link_data.oppersite_link_index = -1;
//	link_data.num_lanes = 1;
//	link_data.link_type = ARTERIAL;
//	link_data.length = link_length*distance_factor;
//	link_data.speed_limit = speed_limit_ramp;
//	link_data.maximum_flow_rate = maximum_flow_rate_arterial;
//	link_data.backward_wave_speed = backward_wave_speed;
//	link_data.free_flow_speed =  float (link_data.speed_limit + 10.0);
//	link_data.jam_density = jam_density;
//	link_data.left_turn_bay_length = 0.0;
//	link_data.right_turn_bay_length = 0.0;
//	link_data.num_left_turn_bays = 0;
//	link_data.num_right_turn_bays = 0;
//	network_data.link_data_array.push_back(link_data);
//	network_data.max_free_flow_speed = max(network_data.max_free_flow_speed,link_data.free_flow_speed);
//
//	//turn
//	network_data.turn_movement_data_array.clear();
//	network_information::network_data_information::TurnMovementData turn_movement_data;
//
//	i=0;
//	turn_movement_data.turn_movement_index = i;
//	turn_movement_data.inbound_link_index = 0;
//	turn_movement_data.outbound_link_index = 1;
//	
//	turn_movement_data.uuid = 1;
//	turn_movement_data.turn_movement_type = THROUGH_TURN;
//	turn_movement_data.turn_movement_rule = ALLOWED;
//	network_data.turn_movement_data_array.push_back(turn_movement_data);
//
//	i=1;
//	turn_movement_data.turn_movement_index = i;
//	turn_movement_data.inbound_link_index = 1;
//	turn_movement_data.outbound_link_index = 2;
//	
//	turn_movement_data.uuid = 2;
//	turn_movement_data.turn_movement_type = THROUGH_TURN;
//	turn_movement_data.turn_movement_rule = ALLOWED;
//	network_data.turn_movement_data_array.push_back(turn_movement_data);
//
//	i=2;
//	turn_movement_data.turn_movement_index = i;
//	turn_movement_data.inbound_link_index = 2;
//	turn_movement_data.outbound_link_index = 3;
//	
//	turn_movement_data.uuid = 3;
//	turn_movement_data.turn_movement_type = THROUGH_TURN;
//	turn_movement_data.turn_movement_rule = ALLOWED;
//	network_data.turn_movement_data_array.push_back(turn_movement_data);
//
//	i=3;
//	turn_movement_data.turn_movement_index = i;
//	turn_movement_data.inbound_link_index = 4;
//	turn_movement_data.outbound_link_index = 2;
//	
//	turn_movement_data.uuid = 4;
//	turn_movement_data.turn_movement_type = RIGHT_TURN;
//	turn_movement_data.turn_movement_rule = ALLOWED;
//	network_data.turn_movement_data_array.push_back(turn_movement_data);
//
//	i=4;
//	turn_movement_data.turn_movement_index = i;
//	turn_movement_data.inbound_link_index = 5;
//	turn_movement_data.outbound_link_index = 4;
//	
//	turn_movement_data.uuid = 5;
//	turn_movement_data.turn_movement_type = THROUGH_TURN;
//	turn_movement_data.turn_movement_rule = ALLOWED;
//	network_data.turn_movement_data_array.push_back(turn_movement_data);
//
//	//activity location
//	network_data.activity_location_data_array.clear();
//	network_information::network_data_information::ActivityLocationData activity_location_data;
//
//	i=0;
//	activity_location_data.activity_location_index = i;
//
//	activity_location_data.uuid = 1;
//	activity_location_data.origin_link_index_array.clear();
//	activity_location_data.destination_link_index_array.clear();
//	activity_location_data.origin_link_index_array.push_back(0);
//	activity_location_data.num_origin_links = (int) activity_location_data.origin_link_index_array.size();
//	activity_location_data.num_destination_links = (int) activity_location_data.destination_link_index_array.size();
//	activity_location_data.zone_index = i;
//	network_data.activity_location_data_array.push_back(activity_location_data);
//
//	i=1;
//	activity_location_data.activity_location_index = i;
//	
//	activity_location_data.uuid = 2;
//	activity_location_data.origin_link_index_array.clear();
//	activity_location_data.destination_link_index_array.clear();
//	activity_location_data.destination_link_index_array.push_back(3);
//	activity_location_data.num_origin_links = (int) activity_location_data.origin_link_index_array.size();
//	activity_location_data.num_destination_links = (int) activity_location_data.destination_link_index_array.size();
//	activity_location_data.zone_index = i;
//	network_data.activity_location_data_array.push_back(activity_location_data);
//
//	i=2;
//	activity_location_data.activity_location_index = i;
//	
//	activity_location_data.uuid = 3;
//	activity_location_data.origin_link_index_array.clear();
//	activity_location_data.destination_link_index_array.clear();
//	activity_location_data.origin_link_index_array.push_back(5);
//	activity_location_data.num_origin_links = (int) activity_location_data.origin_link_index_array.size();
//	activity_location_data.num_destination_links = (int) activity_location_data.destination_link_index_array.size();
//	activity_location_data.zone_index = i;
//	network_data.activity_location_data_array.push_back(activity_location_data);
//
//	network_data.network_activity_location_size = (int) network_data.activity_location_data_array.size();
//	network_data.network_node_size = (int) network_data.node_data_array.size();
//	network_data.network_link_size = (int) network_data.link_data_array.size();
//	network_data.network_turn_movement_size = (int) network_data.turn_movement_data_array.size();
//
//	//turn movement
//	for (int i=0;i<network_data.network_turn_movement_size;i++)
//	{
//		//outbound turn movement of an inbound link
//		int inbound_link_index = network_data.turn_movement_data_array[i].inbound_link_index;
//		network_data.link_data_array[inbound_link_index].outbound_turn_movement_index_array.push_back(i);
//		
//		//inbound turn movement of an outbound link
//		int outbound_link_index = network_data.turn_movement_data_array[i].outbound_link_index;
//		network_data.link_data_array[outbound_link_index].inbound_turn_movement_index_array.push_back(i);
//
//		//printf("tm_index= %d il=%d ol=%d\n",i,network_data.turn_movement_data_array[i].inbound_link_index,network_data.turn_movement_data_array[i].outbound_link_index);
//	}
//
//	//link
//	for(int i=0;i<network_data.network_link_size;i++)
//	{
//		//outbound turn movement size
//		network_data.link_data_array[i].outbound_turn_movement_size = (int) network_data.link_data_array[i].outbound_turn_movement_index_array.size();
//		//outbound link
//		int unode_index = network_data.link_data_array[i].unode_index;
//		network_data.node_data_array[unode_index].outbound_link_index_array.push_back(i);
//		
//		//inbound turn movement size
//		network_data.link_data_array[i].inbound_turn_movement_size = (int) network_data.link_data_array[i].inbound_turn_movement_index_array.size();
//		//inbound link
//		int dnode_index = network_data.link_data_array[i].dnode_index;
//		network_data.node_data_array[dnode_index].inbound_link_index_array.push_back(i);
//		//printf("link_index= %d unode=%d dnode=%d outboud_tm_size= %d\n",i,network_data.link_data_array[i].unode_index,network_data.link_data_array[i].dnode_index,network_data.link_data_array[i].outbound_turn_movement_size);
//	}
//	//node
//	for (int i=0;i<network_data.network_node_size;i++)
//	{
//		//outbound link size
//		network_data.node_data_array[i].outbound_link_size = (int) network_data.node_data_array[i].outbound_link_index_array.size();
//
//		//inbound link size
//		network_data.node_data_array[i].inbound_link_size = (int) network_data.node_data_array[i].inbound_link_index_array.size();
//
//		//printf("node_index= %d outboud_link_size= %d\n",i,network_data.node_data_array[i].outbound_link_size);
//	}
//
//	////link - turn movement map
//	network_data.link_turn_movement_map.clear();
//	for (int i=0;i<network_data.network_turn_movement_size;i++)
//	{
//		int inbound_link_index = network_data.turn_movement_data_array[i].inbound_link_index;
//		int outbound_link_index = network_data.turn_movement_data_array[i].outbound_link_index;
//		network_data.link_turn_movement_map.insert(make_pair(make_pair(inbound_link_index,outbound_link_index),i));
//	}
//};
void network_data_information::construct_network_cost(NetworkData& network_data)
{
	//link travel time
	network_data.link_travel_time_array.clear();
	network_data.link_travel_time_array.resize(network_data.network_link_size);
	for (int i=0;i<network_data.network_link_size;i++)
	{
		float free_flow_speed = (float) ((5280.0) * network_data.link_data_array[i].free_flow_speed/3600.0); // feet per second
		float link_travel_time = float (network_data.link_data_array[i].length / free_flow_speed);
		
		network_data.max_free_flow_speed = max(network_data.max_free_flow_speed,free_flow_speed);
		link_travel_time = max((float)1.0,link_travel_time);
		network_data.link_travel_time_array[i] = link_travel_time;
		//printf ("link =%d unode = %d dnode = %d length = %f tt = %f\n",i,network_data.link_data_array[i].unode_index,network_data.link_data_array[i].dnode_index,network_data.link_data_array[i].length,link_travel_time);
	}
	
	//turn penalty
	network_data.turn_travel_penalty_array.clear();
	network_data.turn_travel_penalty_array.resize(network_data.network_turn_movement_size);
	network_data.forward_link_turn_travel_time_array.clear();
	network_data.forward_link_turn_travel_time_array.resize(network_data.network_turn_movement_size);
	for (int i=0;i<network_data.network_turn_movement_size;i++)
	{
		int inbound_link_index = network_data.turn_movement_data_array[i].inbound_link_index;
		int outbound_link_index = network_data.turn_movement_data_array[i].outbound_link_index;

		//float turn_travel_penalty = (float) ((network_data.link_travel_time_array[inbound_link_index] + network_data.link_travel_time_array[outbound_link_index])*0.10);
		float turn_travel_penalty = 0.0f;

		if (network_data.turn_movement_data_array[i].turn_movement_rule == PROHIBITED)
		{
			turn_travel_penalty = INFINITY_FLOAT;
		}
		
		float forward_link_turn_travel_time = network_data.link_travel_time_array[inbound_link_index] + turn_travel_penalty;

		network_data.turn_travel_penalty_array[i] = turn_travel_penalty;
		network_data.forward_link_turn_travel_time_array[i] = forward_link_turn_travel_time;
		//printf ("tm =%d ilink = %d olink = %d tp = %f ftm = %f\n",i,inbound_link_index,outbound_link_index,turn_travel_penalty,forward_link_turn_travel_time);
	}
};

void network_data_information::read_node(string input_dir_name, NetworkData& network_data)
{
	string filename = input_dir_name + "node";
	string line;
	ifstream input_file(filename);
	
	network_data.node_data_array.clear();
	network_data.node_id_index_map.clear();

	int node_index = -1;
	if (input_file.is_open())
	{
		int iline = 0;
		NodeData node_data;
		vector<string> tokens;
		int token_size =0;

		while (input_file.good())
		{
			getline(input_file,line);
			iline = iline + 1;
			if(iline >= 2) // skip the first line
			{
				//0 - uuid
				//1 - x
				//2 - y
				//3 - type
				token_size = 4;
				string_split(tokens, line, token_size);

				node_data.uuid = stoi(tokens[0]);
				node_data.x = stof(tokens[1]);
				node_data.y = stof(tokens[2]);
				node_data.node_type = network_data.node_type_string_int_map[tokens[3]];

				node_index = node_index + 1;
				node_data.node_index = node_index;
				network_data.node_data_array.push_back(node_data);
				network_data.node_id_index_map.insert(make_pair(node_data.uuid,node_index));
			}
		}
		network_data.network_node_size = int(network_data.node_data_array.size());
	}
	else
	{
		cout << "file " << filename << "cannot be opened!" << endl;
	}
	input_file.close();
};

void network_data_information::read_link(string input_dir_name, NetworkData& network_data, ScenarioData& scenario_data)
{
	string filename = input_dir_name + "link";
	string line;
	ifstream input_file(filename);
	
	//initialization
	network_data.link_data_array.clear();
	network_data.link_id_index_map.clear();

	//read file
	int link_index = -1;
	if (input_file.is_open())
	{
		int iline = 0;
		LinkData link_data;
		vector<string> tokens;
		int token_size =0;
		while (input_file.good())
		{
			getline(input_file,line);
			iline = iline + 1;
			if(iline >= 2) // skip the first line
			{
				//0 - uuid
				//1 - unode
				//2 - dnode
				//3 - length
				//4 - free_flow_speed
				//5 - maximum_flow_rate
				//6 - link type
				//7 - num_lanes
				//8 - num_left_turn_bays
				//9 - num_right_turn_bays
				//10 - left_turn_bay_length
				//11 - right_turn_bay_length
				//12 - original_free_flow_speed

				string link_type_string;
				int unode_id;
				int dnode_id;
				token_size = 13;
				string_split(tokens, line, token_size);

				link_data.uuid = stoi(tokens[0]);
				unode_id = stoi(tokens[1]);
				dnode_id = stoi(tokens[2]);

				if (scenario_data.input_length_unit == LENGTH_IN_METER)
				{
					link_data.length = convert_meter_to_foot<float>(stof(tokens[3]));
				}
				else
				{
					link_data.length = stof(tokens[3]);
				}
				
				if (scenario_data.input_speed_unit == SPEED_IN_METERS_PER_SECOND)
				{
					link_data.free_flow_speed = convert_meters_per_second_to_miles_per_hour<float>(stof(tokens[4]));
					link_data.original_free_flow_speed = convert_meters_per_second_to_miles_per_hour<float>(stof(tokens[12]));
				}
				else
				{
					link_data.free_flow_speed = stof(tokens[4]);
					link_data.original_free_flow_speed = stof(tokens[12]);
				}

				//link_data.length = stof(tokens[3]);
				//link_data.free_flow_speed = stof(tokens[4]);
				
				
				link_data.maximum_flow_rate = stof(tokens[5]);
				link_data.link_type = network_data.link_type_string_int_map[tokens[6]];
				link_data.num_lanes = stoi(tokens[7]);
				link_data.num_left_turn_bays = stoi(tokens[8]);
				link_data.num_right_turn_bays = stoi(tokens[9]);
				link_data.left_turn_bay_length = stof(tokens[10]);
				link_data.right_turn_bay_length = stof(tokens[11]);

				link_data.unode_index = network_data.node_id_index_map[unode_id];
				link_data.dnode_index = network_data.node_id_index_map[dnode_id];
				link_index = link_index + 1;
				link_data.link_index = link_index;
				
				link_data.backward_wave_speed = 12.0;
				link_data.jam_density = 220;

				network_data.link_data_array.push_back(link_data);
				if (link_data.uuid == 40884)
				{
					cout << endl;
				}
				network_data.link_id_index_map.insert(make_pair(link_data.uuid,link_index));
				int iii = network_data.link_id_index_map[link_data.uuid];
				int map_size = (int) network_data.link_id_index_map.size();
				network_data.node_data_array[link_data.unode_index].outbound_link_index_array.push_back(link_index);
				network_data.node_data_array[link_data.dnode_index].inbound_link_index_array.push_back(link_index);
			}
		}
		network_data.network_link_size = int(network_data.link_data_array.size());
	}
	else
	{
		cout << "file " << filename << "cannot be opened!" << endl;
	}
	input_file.close();

	//node
	for (int i=0;i<network_data.network_node_size;i++)
	{
		//outbound link size
		network_data.node_data_array[i].outbound_link_size = (int) network_data.node_data_array[i].outbound_link_index_array.size();

		//inbound link size
		network_data.node_data_array[i].inbound_link_size = (int) network_data.node_data_array[i].inbound_link_index_array.size();
	}
};

void network_data_information::read_turn_movement(string input_dir_name, NetworkData& network_data)
{
	string filename = input_dir_name + "turn_movement";
	string line;
	ifstream input_file(filename);
	
	//initialization

	int turn_movement_index = -1;
	network_data.turn_movement_data_array.clear();
	network_data.turn_movement_id_index_map.clear();

	for (int link_index=0;link_index<network_data.network_link_size;link_index++)
	{
		int dnode_index = network_data.link_data_array[link_index].dnode_index;
		for (int outbound_link=0;outbound_link<network_data.node_data_array[dnode_index].outbound_link_size;outbound_link++)
		{
			int outbound_link_index = network_data.node_data_array[dnode_index].outbound_link_index_array[outbound_link];
			turn_movement_index++;
			TurnMovementData turn_movement_data;
			turn_movement_data.turn_movement_index = turn_movement_index;
			turn_movement_data.uuid = turn_movement_index + 1;
			turn_movement_data.inbound_link_index = link_index;
			turn_movement_data.outbound_link_index = outbound_link_index;
			if (network_data.link_data_array[turn_movement_data.inbound_link_index].unode_index == 
				network_data.link_data_array[turn_movement_data.outbound_link_index].dnode_index)
			{
				turn_movement_data.turn_movement_type = U_TURN;
				//turn_movement_data.turn_movement_rule = PROHIBITED;
				turn_movement_data.turn_movement_rule = ALLOWED;
			}
			else
			{
				turn_movement_data.turn_movement_type = THROUGH_TURN;
				turn_movement_data.turn_movement_rule = ALLOWED;
			}
			network_data.turn_movement_data_array.push_back(turn_movement_data);
			network_data.turn_movement_id_index_map.insert(make_pair(turn_movement_data.uuid,turn_movement_index));
			network_data.link_turn_movement_map.insert(make_pair(make_pair(link_index,outbound_link_index),turn_movement_index));
			
			network_data.link_data_array[link_index].outbound_turn_movement_index_array.push_back(turn_movement_index);
			network_data.link_data_array[outbound_link_index].inbound_turn_movement_index_array.push_back(turn_movement_index);
		}
	}
	network_data.network_turn_movement_size = (int) network_data.turn_movement_data_array.size();

	//node
	for (int i=0;i<network_data.network_link_size;i++)
	{
		//outbound link size
		network_data.link_data_array[i].outbound_turn_movement_size = (int) network_data.link_data_array[i].outbound_turn_movement_index_array.size();

		//inbound link size
		network_data.link_data_array[i].inbound_turn_movement_size = (int) network_data.link_data_array[i].inbound_turn_movement_index_array.size();
	}

	//read file
	if (input_file.is_open())
	{
		int iline = 0;
		vector<string> tokens;
		int token_size =0;

		while (input_file.good())
		{
			getline(input_file,line);
			iline = iline + 1;
			if(iline >= 2) // skip the first line
			{
				token_size = 5;
				string_split(tokens, line, token_size);

				int turn_movement_uuid = stoi(tokens[0]);
				int inbound_link_id = stoi(tokens[1]);
				int outbound_link_id = stoi(tokens[2]);
				int inbound_link_index = network_data.link_id_index_map[inbound_link_id];
				int outbound_link_index = network_data.link_id_index_map[outbound_link_id];

				turn_movement_index = network_data.link_turn_movement_map[make_pair(inbound_link_index,outbound_link_index)];
				TurnMovementData& turn_movement_data = network_data.turn_movement_data_array[turn_movement_index];

				turn_movement_data.turn_movement_type = network_data.turn_movement_type_string_int_map[tokens[3]];
				turn_movement_data.turn_movement_rule = network_data.turn_movement_rule_string_int_map[tokens[4]];
			}
		}
	}
	else
	{
		cout << "file " << filename << "cannot be opened!" << endl;
	}
	input_file.close();
};

void network_data_information::read_zone(string input_dir_name, NetworkData& network_data)
{
	string filename = input_dir_name + "zone";
	string line;
	ifstream input_file(filename);
	
	//initialization
	network_data.zone_data_array.clear();
	network_data.zone_id_index_map.clear();

	//read file
	int zone_index = -1;
	if (input_file.is_open())
	{
		int iline = 0;
		ZoneData zone_data;
		vector<string> tokens;
		int token_size =0;
		while (input_file.good())
		{
			getline(input_file,line);
			iline = iline + 1;
			if(iline >= 2) // skip the first line
			{

				//0 - uuid
				token_size = 1;
				string_split(tokens, line);
				zone_data.uuid = stoi(tokens[0]);
				zone_data.x = stoi(tokens[1]);
				zone_data.y = stoi(tokens[2]);
				zone_data.population = stoi(tokens[3]);

				zone_index = zone_index + 1;
				zone_data.zone_index = zone_index;
				zone_data.num_origin_activity_locations = 0;
				zone_data.num_destination_activity_locations = 0;

				network_data.zone_data_array.push_back(zone_data);
				network_data.zone_id_index_map.insert(make_pair(zone_data.uuid,zone_index));
			}
		}
		network_data.network_zone_size = int(network_data.zone_data_array.size());
	}
	else
	{
		cout << "file " << filename << "cannot be opened!" << endl;
	}
	input_file.close();
};

void network_data_information::read_activity_location(string input_dir_name, NetworkData& network_data)
{
	string filename = input_dir_name + "activity_location";
	string line;
	ifstream input_file(filename);
	
	//initialization
	network_data.activity_location_data_array.clear();
	network_data.activity_location_id_index_map.clear();

	//read file
	int activity_location_index = -1;
	if (input_file.is_open())
	{
		int iline = 0;
		ActivityLocationData activity_location_data;
		int zone_id;
		vector<string> tokens;
		int token_size =0;

		while (input_file.good())
		{
			getline(input_file,line);
			iline = iline + 1;

			if(iline >= 2) // skip the first line
			{
				//nested structure
				if ((iline-1)%3 == 1)
				{//read first line of an activity location
					token_size = 4;
					string_split(tokens, line, token_size);
					activity_location_data.uuid = stoi(tokens[0]);
					zone_id = stoi(tokens[1]);
					activity_location_data.num_origin_links = stoi(tokens[2]);
					activity_location_data.num_destination_links = stoi(tokens[3]);

					activity_location_data.zone_index = zone_id;//network_data.zone_id_index_map[zone_id];
				}
				
				if ((iline-1)%3 == 2)
				{//read second line of an activity location - origin links
					activity_location_data.origin_link_index_array.clear();
					activity_location_data.origin_link_index_array.resize(activity_location_data.num_origin_links);
					token_size = activity_location_data.num_origin_links;
					string_split(tokens, line, token_size);

					for (int j=0;j<token_size;j++)
					{
						int link_id = stoi(tokens[j]);
						activity_location_data.origin_link_index_array[j] = network_data.link_id_index_map[link_id];
					}
				}

				if ((iline-1)%3 == 0)
				{//read third line of an activity location - destination links
					activity_location_data.destination_link_index_array.clear();
					activity_location_data.destination_link_index_array.resize(activity_location_data.num_destination_links);
					token_size = activity_location_data.num_destination_links;
					string_split(tokens, line, token_size);
					
					for (int j=0;j<token_size;j++)
					{
						int link_id = stoi(tokens[j]);
						activity_location_data.destination_link_index_array[j] = network_data.link_id_index_map[link_id];
					}

					activity_location_index = activity_location_index + 1;
					activity_location_data.activity_location_index = activity_location_index;

					network_data.activity_location_data_array.push_back(activity_location_data);
					network_data.activity_location_id_index_map.insert(make_pair(activity_location_data.uuid,activity_location_index));
				}
			}
		}
		network_data.network_activity_location_size = int(network_data.activity_location_data_array.size());
	}
	else
	{
		cout << "file " << filename << "cannot be opened!" << endl;
	}
	input_file.close();
};

void network_data_information::read_network_data(string input_dir_name, NetworkData& network_data, ScenarioData& scenario_data)
{
	//read node
	read_node(input_dir_name, network_data);

	//read link
	read_link(input_dir_name, network_data, scenario_data);

	//read turn movement
	read_turn_movement(input_dir_name, network_data);

	//read zone
	read_zone(input_dir_name, network_data);

	//read activity location
	read_activity_location(input_dir_name, network_data);

	//activity location
	for (int i=0;i<network_data.network_activity_location_size;i++)
	{
		int zone_id = network_data.activity_location_data_array[i].zone_index;
		int zone_index = network_data.zone_id_index_map.find(zone_id)->second;

		//origin activity location
		if (network_data.activity_location_data_array[i].num_origin_links>0)
		{
			network_data.zone_data_array[zone_index].origin_activity_location_index_array.push_back(zone_index);
			network_data.zone_data_array[zone_index].num_origin_activity_locations++;
		}

		//origin link choice probability
		network_data.activity_location_data_array[i].origin_link_choice_cdf_array.resize(network_data.activity_location_data_array[i].num_origin_links);
		for (int il=0;il<network_data.activity_location_data_array[i].num_origin_links;il++)
		{
			if (il==0)
			{
				network_data.activity_location_data_array[i].origin_link_choice_cdf_array[il] = 1.0f/(network_data.activity_location_data_array[i].num_origin_links*1.0f);
			}
			else
			{
				if (il == network_data.activity_location_data_array[i].num_origin_links-1)
				{
					network_data.activity_location_data_array[i].origin_link_choice_cdf_array[il]= 1.0f;
				}
				else
				{
					network_data.activity_location_data_array[i].origin_link_choice_cdf_array[il] = network_data.activity_location_data_array[i].origin_link_choice_cdf_array[il-1] + 1.0f/(network_data.activity_location_data_array[i].num_origin_links*1.0f);
				}
			}
		}

		//destination activity location
		if (network_data.activity_location_data_array[i].num_destination_links>0)
		{
			network_data.zone_data_array[zone_index].destination_activity_location_index_array.push_back(zone_index);
			network_data.zone_data_array[zone_index].num_destination_activity_locations++;
		}

		//destination link choice probability
		network_data.activity_location_data_array[i].destination_link_choice_cdf_array.resize(network_data.activity_location_data_array[i].num_destination_links);
		for (int il=0;il<network_data.activity_location_data_array[i].num_destination_links;il++)
		{
			if (il==0)
			{
				network_data.activity_location_data_array[i].destination_link_choice_cdf_array[il] = 1.0f/(network_data.activity_location_data_array[i].num_destination_links*1.0f);
			}
			else
			{
				if (il == network_data.activity_location_data_array[i].num_destination_links-1)
				{
					network_data.activity_location_data_array[i].destination_link_choice_cdf_array[il]= 1.0f;
				}
				else
				{
					network_data.activity_location_data_array[i].destination_link_choice_cdf_array[il] = network_data.activity_location_data_array[i].destination_link_choice_cdf_array[il-1] + 1.0f/(network_data.activity_location_data_array[i].num_destination_links*1.0f);
				}
			}
		}

	}
	
	//zone
	for (int i=0;i<network_data.network_zone_size;i++)
	{
		//origin activity location choice probability
		network_data.zone_data_array[i].origin_activity_location_choice_cdf_array.resize(network_data.zone_data_array[i].num_origin_activity_locations);
		for (int ia=0;ia<network_data.zone_data_array[i].num_origin_activity_locations;ia++)
		{
			if (ia==0)
			{
				network_data.zone_data_array[i].origin_activity_location_choice_cdf_array[ia] = 1.0f/(network_data.zone_data_array[i].num_origin_activity_locations*1.0f);
			}
			else
			{
				if (ia == network_data.zone_data_array[i].num_origin_activity_locations-1)
				{
					network_data.zone_data_array[i].origin_activity_location_choice_cdf_array[ia] = 1.0f;
				}
				else
				{
					network_data.zone_data_array[i].origin_activity_location_choice_cdf_array[ia] = network_data.zone_data_array[i].origin_activity_location_choice_cdf_array[ia-1] + 1.0f/(network_data.zone_data_array[i].num_origin_activity_locations*1.0f);
				}
			}
		}

		//destination activity location choice probability
		network_data.zone_data_array[i].destination_activity_location_choice_cdf_array.resize(network_data.zone_data_array[i].num_destination_activity_locations);
		for (int ia=0;ia<network_data.zone_data_array[i].num_destination_activity_locations;ia++)
		{
			if (ia==0)
			{
				network_data.zone_data_array[i].destination_activity_location_choice_cdf_array[ia] = 1.0f/(network_data.zone_data_array[i].num_destination_activity_locations*1.0f);
			}
			else
			{
				if (ia == network_data.zone_data_array[i].num_destination_activity_locations-1)
				{
					network_data.zone_data_array[i].destination_activity_location_choice_cdf_array[ia] = 1.0f;
				}
				else
				{
					network_data.zone_data_array[i].destination_activity_location_choice_cdf_array[ia] = network_data.zone_data_array[i].destination_activity_location_choice_cdf_array[ia-1] + 1.0f/(network_data.zone_data_array[i].num_destination_activity_locations*1.0f);
				}
			}
		}

	}
};

void network_data_information::read_network_data_dsp(string input_dir_name, NetworkData& network_data)
{
	//read network.dat
	read_network_dat_data_dsp(input_dir_name,network_data);

	//link
	for(int i=0;i<network_data.network_link_size;i++)
	{
		//outbound link
		int unode_index = network_data.link_data_array[i].unode_index;
		network_data.node_data_array[unode_index].outbound_link_index_array.push_back(i);
		
		//inbound link
		int dnode_index = network_data.link_data_array[i].dnode_index;
		network_data.node_data_array[dnode_index].inbound_link_index_array.push_back(i);
	}

	//node
	for (int i=0;i<network_data.network_node_size;i++)
	{
		//outbound link size
		network_data.node_data_array[i].outbound_link_size = (int) network_data.node_data_array[i].outbound_link_index_array.size();

		//inbound link size
		network_data.node_data_array[i].inbound_link_size = (int) network_data.node_data_array[i].inbound_link_index_array.size();
	}


	//read xy.dat
	read_xy_dat_data_dsp(input_dir_name,network_data);

	//read turn movement
	read_movement_dat_data_dsp(input_dir_name, network_data);

	//read zone.dat
	read_zone_dat_data_dsp(input_dir_name, network_data);

	//read origin.dat
	read_origin_dat_data_dsp(input_dir_name, network_data);

	//read destination.dat
	read_destination_dat_data_dsp(input_dir_name, network_data);

	//activity location
	for (int i=0;i<network_data.network_activity_location_size;i++)
	{
		int zone_index = network_data.activity_location_data_array[i].zone_index;

		//origin link choice probability
		network_data.activity_location_data_array[i].origin_link_choice_cdf_array.resize(network_data.activity_location_data_array[i].num_origin_links);
		for (int il=0;il<network_data.activity_location_data_array[i].num_origin_links;il++)
		{
			if (il==0)
			{
				network_data.activity_location_data_array[i].origin_link_choice_cdf_array[il] = 1.0f/(network_data.activity_location_data_array[i].num_origin_links*1.0f);
			}
			else
			{
				if (il == network_data.activity_location_data_array[i].num_origin_links-1)
				{
					network_data.activity_location_data_array[i].origin_link_choice_cdf_array[il]= 1.0f;
				}
				else
				{
					network_data.activity_location_data_array[i].origin_link_choice_cdf_array[il] = network_data.activity_location_data_array[i].origin_link_choice_cdf_array[il-1] + 1.0f/(network_data.activity_location_data_array[i].num_origin_links*1.0f);
				}
			}
		}

		//destination link choice probability
		network_data.activity_location_data_array[i].destination_link_choice_cdf_array.resize(network_data.activity_location_data_array[i].num_destination_links);
		for (int il=0;il<network_data.activity_location_data_array[i].num_destination_links;il++)
		{
			if (il==0)
			{
				network_data.activity_location_data_array[i].destination_link_choice_cdf_array[il] = 1.0f/(network_data.activity_location_data_array[i].num_destination_links*1.0f);
			}
			else
			{
				if (il == network_data.activity_location_data_array[i].num_destination_links-1)
				{
					network_data.activity_location_data_array[i].destination_link_choice_cdf_array[il]= 1.0f;
				}
				else
				{
					network_data.activity_location_data_array[i].destination_link_choice_cdf_array[il] = network_data.activity_location_data_array[i].destination_link_choice_cdf_array[il-1] + 1.0f/(network_data.activity_location_data_array[i].num_destination_links*1.0f);
				}
			}
		}

	}
	
	//zone
	for (int i=0;i<network_data.network_zone_size;i++)
	{
		//origin activity location choice probability
		network_data.zone_data_array[i].origin_activity_location_choice_cdf_array.resize(network_data.zone_data_array[i].num_origin_activity_locations);
		for (int ia=0;ia<network_data.zone_data_array[i].num_origin_activity_locations;ia++)
		{
			if (ia==0)
			{
				network_data.zone_data_array[i].origin_activity_location_choice_cdf_array[ia] = 1.0f/(network_data.zone_data_array[i].num_origin_activity_locations*1.0f);
			}
			else
			{
				if (ia == network_data.zone_data_array[i].num_origin_activity_locations-1)
				{
					network_data.zone_data_array[i].origin_activity_location_choice_cdf_array[ia] = 1.0f;
				}
				else
				{
					network_data.zone_data_array[i].origin_activity_location_choice_cdf_array[ia] = network_data.zone_data_array[i].origin_activity_location_choice_cdf_array[ia-1] + 1.0f/(network_data.zone_data_array[i].num_origin_activity_locations*1.0f);
				}
			}
		}

		//destination activity location choice probability
		network_data.zone_data_array[i].destination_activity_location_choice_cdf_array.resize(network_data.zone_data_array[i].num_destination_activity_locations);
		for (int ia=0;ia<network_data.zone_data_array[i].num_destination_activity_locations;ia++)
		{
			if (ia==0)
			{
				network_data.zone_data_array[i].destination_activity_location_choice_cdf_array[ia] = 1.0f/(network_data.zone_data_array[i].num_destination_activity_locations*1.0f);
			}
			else
			{
				if (ia == network_data.zone_data_array[i].num_destination_activity_locations-1)
				{
					network_data.zone_data_array[i].destination_activity_location_choice_cdf_array[ia] = 1.0f;
				}
				else
				{
					network_data.zone_data_array[i].destination_activity_location_choice_cdf_array[ia] = network_data.zone_data_array[i].destination_activity_location_choice_cdf_array[ia-1] + 1.0f/(network_data.zone_data_array[i].num_destination_activity_locations*1.0f);
				}
			}
		}

	}
};

void network_data_information::read_network_dat_data_dsp(string input_dir_name, NetworkData& network_data)
{
	string filename = input_dir_name + "network.dat";
	string line;
	ifstream input_file(filename);
	
	network_data.node_data_array.clear();
	network_data.node_id_index_map.clear();

	network_data.link_data_array.clear();
	network_data.link_id_index_map.clear();

	network_data.node_link_map.clear();

	int node_index = -1;
	int link_index = -1;
	if (input_file.is_open())
	{
		int iline = 0;
		int iline_node = 0;
		int iline_link = 0;
		NodeData node_data;
		LinkData link_data;

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
			if (iline == 1)
			{//
				for (int j=0;j<tokens.size();j++)
				{
					stringstream sss(tokens[j]);
					if (j==0) sss>>network_data.network_zone_size;
					if (j==1) sss>>network_data.network_node_size;
					if (j==2) sss>>network_data.network_link_size;
				}
			
				iline_node = iline + 1;
				iline_link = iline + network_data.network_node_size;
			}

			//node block
			if (iline>=iline_node && iline <=iline_link)
			{
				for (int j=0;j<tokens.size();j++)
				{
					stringstream sss(tokens[j]);
					if (j==0) sss>>node_data.uuid;
				}

				node_index = node_index + 1;
				node_data.node_index = node_index;
				node_data.node_type = NO_CONTROL;
				network_data.node_data_array.push_back(node_data);
				network_data.node_id_index_map.insert(make_pair(node_data.uuid,node_index));
			}

			//link block
			if(iline > iline_link) // skip the first line
			{
				string link_type_string;
				int unode_id;
				int dnode_id;
				int traffic_flow_model;
				int posted_speed_limit_adjustment_margin;
				int maximum_service_flow_rate;
				int link_type_integer;
				int link_grade;
				for (int j=0;j<tokens.size();j++)
				{
					stringstream sss(tokens[j]);
					
					if (j==0) sss>>unode_id;
					if (j==1) sss>>dnode_id;
					if (j==2) sss>>link_data.num_left_turn_bays;
					if (j==3) sss>>link_data.num_right_turn_bays;
					if (j==4) sss>>link_data.length;
					if (j==5) sss>>link_data.num_lanes;
					if (j==6) sss>>traffic_flow_model;
					if (j==7) sss>>posted_speed_limit_adjustment_margin;
					if (j==8) sss>>link_data.speed_limit;
					if (j==9) sss>>maximum_service_flow_rate;
					if (j==10) sss>>link_data.maximum_flow_rate;
					if (j==11) sss>>link_type_integer;
					if (j==12) sss>>link_grade;
				}

				link_data.unode_index = network_data.node_id_index_map[unode_id];
				link_data.dnode_index = network_data.node_id_index_map[dnode_id];

				if (link_type_integer == 1 || link_type_integer == 2 || link_type_integer == 6 || link_type_integer == 8 || link_type_integer == 9 || link_type_integer == 10)
				{
					link_data.link_type = FREEWAY;
				}

				if (link_type_integer == 3)
				{
					link_data.link_type = ON_RAMP;
				}

				if (link_type_integer == 4)
				{
					link_data.link_type = OFF_RAMP;
				}

				if (link_type_integer == 7)
				{
					link_data.link_type = EXPRESSWAY;
				}

				if (link_type_integer == 5)
				{
					link_data.link_type = ARTERIAL;
				}

				link_index = link_index + 1;
				link_data.link_index = link_index;
				link_data.uuid = link_index + 1;
				link_data.backward_wave_speed = 12.0;
				link_data.jam_density = 220;
				link_data.free_flow_speed = link_data.speed_limit + posted_speed_limit_adjustment_margin;
				link_data.maximum_flow_rate = float(maximum_service_flow_rate);
				link_data.left_turn_bay_length = 0.0f;
				link_data.right_turn_bay_length = 0.0f;

				if (link_data.num_left_turn_bays > 0)
				{
					link_data.left_turn_bay_length = 0.2f*link_data.length;
				}
				if (link_data.num_right_turn_bays > 0)
				{
					link_data.right_turn_bay_length = 0.2f*link_data.length;
				}
				
				network_data.link_data_array.push_back(link_data);
				network_data.link_id_index_map.insert(make_pair(link_data.uuid,link_index));
				network_data.node_link_map.insert(make_pair(make_pair(link_data.unode_index,link_data.dnode_index),link_data.link_index));
			}
		}
	}
	else
	{
		cout << "file " << filename << "cannot be opened!" << endl;
	}

	network_data.network_node_size = (int) network_data.node_data_array.size();
	network_data.network_link_size = (int) network_data.link_data_array.size();

	input_file.close();
};

void network_data_information::read_xy_dat_data_dsp(string input_dir_name, NetworkData& network_data)
{
	string filename = input_dir_name + "xy.dat";
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
			int node_id;
			for (int j=0;j<tokens.size();j++)
			{
				stringstream sss(tokens[j]);
				if (j==0) sss>>node_id ;
				if (j==1) sss>>network_data.node_data_array[iline-1].x ;
				if (j==2) sss>>network_data.node_data_array[iline-1].y ;
			}
		}
	}
	else
	{
		cout << "file " << filename << "cannot be opened!" << endl;
	}

	input_file.close();
};

void network_data_information::read_movement_dat_data_dsp(string input_dir_name, NetworkData& network_data)
{
	//construct turn_movement
	network_data.turn_movement_data_array.clear();
	int turn_movement_index = -1;
	for (int node_index=0;node_index<network_data.network_node_size;node_index++)
	{
		for (int inbound_link=0;inbound_link<network_data.node_data_array[node_index].inbound_link_size;inbound_link++)
		{
			int inbound_link_index = network_data.node_data_array[node_index].inbound_link_index_array[inbound_link];
			for (int outbound_link=0;outbound_link<network_data.node_data_array[node_index].outbound_link_size;outbound_link++)
			{
				int outbound_link_index = network_data.node_data_array[node_index].outbound_link_index_array[outbound_link];

				turn_movement_index = turn_movement_index + 1;
				TurnMovementData turn_movement_data;
				turn_movement_data.turn_movement_index = turn_movement_index;
				turn_movement_data.uuid = turn_movement_index + 1;
				turn_movement_data.inbound_link_index = inbound_link_index;
				turn_movement_data.outbound_link_index = outbound_link_index;
				turn_movement_data.turn_movement_rule = ALLOWED;
				turn_movement_data.turn_movement_type = THROUGH_TURN;
				network_data.turn_movement_data_array.push_back(turn_movement_data);
			}
		}
	}
	network_data.network_turn_movement_size = int (network_data.turn_movement_data_array.size());
	//turn movement
	for (int i=0;i<network_data.network_turn_movement_size;i++)
	{
		//outbound turn movement of an inbound link
		int inbound_link_index = network_data.turn_movement_data_array[i].inbound_link_index;
		network_data.link_data_array[inbound_link_index].outbound_turn_movement_index_array.push_back(i);
		
		//inbound turn movement of an outbound link
		int outbound_link_index = network_data.turn_movement_data_array[i].outbound_link_index;
		network_data.link_data_array[outbound_link_index].inbound_turn_movement_index_array.push_back(i);
	}

	//link
	for(int i=0;i<network_data.network_link_size;i++)
	{
		//outbound turn movement size
		network_data.link_data_array[i].outbound_turn_movement_size = (int) network_data.link_data_array[i].outbound_turn_movement_index_array.size();
		
		//inbound turn movement size
		network_data.link_data_array[i].inbound_turn_movement_size = (int) network_data.link_data_array[i].inbound_turn_movement_index_array.size();
	}

	//link - turn movement map
	network_data.link_turn_movement_map.clear();
	for (int i=0;i<network_data.network_turn_movement_size;i++)
	{
		int inbound_link_index = network_data.turn_movement_data_array[i].inbound_link_index;
		int outbond_link_index = network_data.turn_movement_data_array[i].outbound_link_index;
		network_data.link_turn_movement_map.insert(make_pair(make_pair(inbound_link_index,outbond_link_index),i));
	}

	string filename = input_dir_name + "movement.dat";
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
			int unode_id;
			int dnode_id;
			int node_id_left_turn;
			int node_id_straight_turn;
			int node_id_right_turn;
			int node_id_other_turn1;
			int node_id_other_turn2;
			int node_id_u_turn;

			for (int j=0;j<tokens.size();j++)
			{
				stringstream sss(tokens[j]);
				if (j==0) sss>>unode_id;
				if (j==1) sss>>dnode_id;
				if (j==2) sss>>node_id_left_turn;
				if (j==3) sss>>node_id_straight_turn;
				if (j==4) sss>>node_id_right_turn;
				if (j==5) sss>>node_id_other_turn1;
				if (j==6) sss>>node_id_other_turn2;
				if (j==7) sss>>node_id_u_turn;
			}
			int unode_index = network_data.node_id_index_map[unode_id];
			int dnode_index = network_data.node_id_index_map[dnode_id];
			int inbound_link_index = network_data.node_link_map[make_pair(unode_index,dnode_index)];
			
			int next_node_index=-1;
			int outbound_link_index = -1;
			int turn_movement_index = -1;
			//left turn
			if (node_id_left_turn>0)
			{
				next_node_index = network_data.node_id_index_map[node_id_left_turn];
				outbound_link_index = network_data.node_link_map[make_pair(dnode_index,next_node_index)];
				turn_movement_index = network_data.link_turn_movement_map[make_pair(inbound_link_index,outbound_link_index)];
				network_data.turn_movement_data_array[turn_movement_index].turn_movement_type = LEFT_TURN;
			}
			//straight turn
			if (node_id_straight_turn>0)
			{
				next_node_index = network_data.node_id_index_map[node_id_straight_turn];
				outbound_link_index = network_data.node_link_map[make_pair(dnode_index,next_node_index)];
				turn_movement_index = network_data.link_turn_movement_map[make_pair(inbound_link_index,outbound_link_index)];
				network_data.turn_movement_data_array[turn_movement_index].turn_movement_type = THROUGH_TURN;
			}

			//right turn
			if (node_id_right_turn>0)
			{
				next_node_index = network_data.node_id_index_map[node_id_right_turn];
				outbound_link_index = network_data.node_link_map[make_pair(dnode_index,next_node_index)];
				turn_movement_index = network_data.link_turn_movement_map[make_pair(inbound_link_index,outbound_link_index)];
				network_data.turn_movement_data_array[turn_movement_index].turn_movement_type = RIGHT_TURN;
			}

			//other 1
			if (node_id_other_turn1>0)
			{
				next_node_index = network_data.node_id_index_map[node_id_other_turn1];
				outbound_link_index = network_data.node_link_map[make_pair(dnode_index,next_node_index)];
				turn_movement_index = network_data.link_turn_movement_map[make_pair(inbound_link_index,outbound_link_index)];
				network_data.turn_movement_data_array[turn_movement_index].turn_movement_type = RIGHT_TURN;
			}

			//other 2
			if (node_id_other_turn2>0)
			{
				next_node_index = network_data.node_id_index_map[node_id_other_turn2];
				outbound_link_index = network_data.node_link_map[make_pair(dnode_index,next_node_index)];
				turn_movement_index = network_data.link_turn_movement_map[make_pair(inbound_link_index,outbound_link_index)];
				network_data.turn_movement_data_array[turn_movement_index].turn_movement_type = LEFT_TURN;
			}

			//u-turn
			if (node_id_u_turn>0)
			{
				next_node_index = unode_index;
				outbound_link_index = network_data.node_link_map[make_pair(dnode_index,next_node_index)];
				turn_movement_index = network_data.link_turn_movement_map[make_pair(inbound_link_index,outbound_link_index)];
				network_data.turn_movement_data_array[turn_movement_index].turn_movement_type = U_TURN;
			}

		}
	}
	else
	{
		cout << "file " << filename << "cannot be opened!" << endl;
	}

	input_file.close();
};

void network_data_information::read_zone_dat_data_dsp(string input_dir_name, NetworkData& network_data)
{
	string filename = input_dir_name + "zone.dat";
	string line;
	ifstream input_file(filename);
	
	//initialization
	network_data.zone_data_array.clear();
	network_data.zone_id_index_map.clear();

	ZoneData zone_data;
	for (int zone_index=0;zone_index<network_data.network_zone_size;zone_index++)
	{
		zone_data.uuid = zone_index + 1;
		zone_data.zone_index = zone_index;
		zone_data.num_origin_activity_locations = 0;
		zone_data.num_destination_activity_locations = 0;

		network_data.zone_data_array.push_back(zone_data);
		network_data.zone_id_index_map.insert(make_pair(zone_data.uuid,zone_index));
	}
};

void network_data_information::read_origin_dat_data_dsp(string input_dir_name, NetworkData& network_data)
{
	string filename = input_dir_name + "origin.dat";
	string line;
	ifstream input_file(filename);
	
	network_data.activity_location_data_array.clear();
	network_data.activity_location_id_index_map.clear();

	ActivityLocationData activity_location_data;

	int activity_location_index = -1;
	if (input_file.is_open())
	{
		for (int zone_index=0;zone_index<network_data.network_zone_size;zone_index++)
		{
			//
			activity_location_index = zone_index;
			activity_location_data.activity_location_index = zone_index;
			activity_location_data.uuid = network_data.zone_data_array[zone_index].uuid;
			activity_location_data.destination_link_index_array.clear();
			activity_location_data.origin_link_index_array.clear();
			activity_location_data.num_destination_links = 0;
			activity_location_data.num_origin_links = 0;
			activity_location_data.zone_index = zone_index;

			network_data.zone_data_array[zone_index].origin_activity_location_index_array.clear();
			network_data.zone_data_array[zone_index].origin_activity_location_index_array.push_back(activity_location_data.activity_location_index);
			network_data.zone_data_array[zone_index].num_origin_activity_locations = (int)network_data.zone_data_array[zone_index].origin_activity_location_index_array.size();

			//
			getline(input_file,line);
			stringstream ss(line);
			string buf;
			vector<string> tokens;
			while (ss>>buf)
			{
				tokens.push_back(buf);
			}
			int zone_id;
			int num_origin_links;
			int loading_mode;
			for (int j=0;j<tokens.size();j++)
			{
				stringstream sss(tokens[j]);
				if (j==0) sss>>zone_id;
				if (j==1) sss>>num_origin_links;
				if (j==2) sss>>loading_mode;
			}
			activity_location_data.num_origin_links = num_origin_links;

			for (int origin_link=0;origin_link<num_origin_links;origin_link++)
			{
				getline(input_file,line);
				stringstream ss(line);
				string buf;
				vector<string> tokens;
				while (ss>>buf)
				{
					tokens.push_back(buf);
				}

				int unode_id;
				int dnode_id;
				int loading_weight;
				for (int j=0;j<tokens.size();j++)
				{
					stringstream sss(tokens[j]);
					if (j==0) sss>>unode_id;
					if (j==1) sss>>dnode_id;
					if (j==2) sss>>loading_weight;
				}
				
				int unode_index = network_data.node_id_index_map[unode_id];
				int dnode_index = network_data.node_id_index_map[dnode_id];
				int origin_link_index = network_data.node_link_map[make_pair(unode_index,dnode_index)];
				
				activity_location_data.origin_link_index_array.push_back(origin_link_index);
			}

			network_data.activity_location_data_array.push_back(activity_location_data);
			network_data.activity_location_id_index_map.insert(make_pair(activity_location_data.uuid,activity_location_data.activity_location_index));
		}
		network_data.network_activity_location_size = (int) network_data.activity_location_data_array.size();
	}
	else
	{
		cout << "file " << filename << "cannot be opened!" << endl;
	}

	input_file.close();
};

void network_data_information::read_destination_dat_data_dsp(string input_dir_name, NetworkData& network_data)
{
	string filename = input_dir_name + "destination.dat";
	string line;
	ifstream input_file(filename);
	
	int activity_location_index = -1;
	if (input_file.is_open())
	{
		for (int zone_index=0;zone_index<network_data.network_zone_size;zone_index++)
		{
			//
			activity_location_index = zone_index;
			network_data.activity_location_data_array[activity_location_index].destination_link_index_array.clear();

			network_data.zone_data_array[zone_index].destination_activity_location_index_array.clear();
			network_data.zone_data_array[zone_index].destination_activity_location_index_array.push_back(activity_location_index);
			network_data.zone_data_array[zone_index].num_destination_activity_locations = (int) network_data.zone_data_array[zone_index].destination_activity_location_index_array.size();

			//
			getline(input_file,line);
			stringstream ss(line);
			string buf;
			vector<string> tokens;
			while (ss>>buf)
			{
				tokens.push_back(buf);
			}
			int zone_id;
			int num_destination_nodes;
			vector<int> destination_nodes;
			for (int j=0;j<tokens.size();j++)
			{
				stringstream sss(tokens[j]);
				if (j==0) sss>>zone_id;
				if (j==1) sss>>num_destination_nodes;
				if (j>=2) 
				{
					int destination_node;
					sss>>destination_node;
					destination_nodes.push_back(destination_node);
				}
			}

			for (int node=0;node<num_destination_nodes;node++)
			{
				int node_index = network_data.node_id_index_map[destination_nodes[node]];
				for (int inbound_link=0;inbound_link<network_data.node_data_array[node_index].inbound_link_size;inbound_link++)
				{
					int inbound_link_index = network_data.node_data_array[node_index].inbound_link_index_array[inbound_link];
					network_data.activity_location_data_array[activity_location_index].destination_link_index_array.push_back(inbound_link_index);
				}
			}
			network_data.activity_location_data_array[activity_location_index].num_destination_links = (int) network_data.activity_location_data_array[activity_location_index].destination_link_index_array.size();
		}
	}
	else
	{
		cout << "file " << filename << "cannot be opened!" << endl;
	}

	input_file.close();
};