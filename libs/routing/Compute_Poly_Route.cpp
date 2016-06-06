//this is the transit routing. This file does not compile at this point, but it was checked against google's transit routing service at some point

//#include "io\database.h"
//#include "io\Supply.h"
//#include "core\Core.h"
//
//#include "A_Star.h"
//#include "Routable_Agent_Implementation.h"
//
//using namespace polaris;
//
//
//
//template<typename MasterType>
//struct Custom_Connection_Group<MasterType,typename MasterType::stop_graph_type,typename MasterType::transit_graph_type,typename MasterType::stop_to_transit_type> : public Connection_Group_Base<MasterType, typename MasterType::stop_graph_type, typename MasterType::transit_graph_type, typename MasterType::stop_to_transit_type >
//{
//	int binary_search(Connection_Implementation* A, float key, int imin, int imax)
//	{
//		// continue searching while [imin,imax] is not empty
//		while (imax >= imin)
//		{
//			// calculate the midpoint for roughly equal partition
//			int imid = (imin + imax)/2;
//
//			//cout << "\tbinary searching: " << (A+imid)->_departure_time << endl;
//
//			// determine which subarray to search
//			if ((A+imid)->_departure_time < key)
//			{
//				// change min index to search upper subarray
//				imin = imid + 1;
//			}
//			else if ((A+imid)->_departure_time > key)
//			{
//				// change max index to search lower subarray
//				imax = imid - 1;
//			}
//			else/* if((A+imid)->_departure_time == key)*/
//			{
//				// key found at index imid
//				return imid;
//			}
//		}
//		// key not found, return the index to the value which is just higher than key
//		return imin;
//	}
//
//	template<typename AgentType>
//	Anonymous_Connection_Group* Visit_Neighbors(Routable_Agent<AgentType>* agent, current_edge_type* current, Routing_Data<base_edge_type>& routing_data)
//	{
//		const Connection_Implementation* const end_connection_itr = end_forward_edges();
//
//		if( agent->process_connection_set<current_edge_type, stop_to_transit>(current) )
//		{
//			//cout << "\tvisiting neighbors of connection type: " << typeid(stop_to_transit).name() << endl;
//
//			const float current_time = current->_time_label;
//			const float horizon_time = current_time + ((AgentType*)agent)->_wait_horizon;
//			
//			Connection_Implementation* connection_itr = forward_edges();
//
//			// for smaller sets, a pure linear search will probably be faster
//			if(num_forward_edges() > 24)
//			{
//				connection_itr = connection_itr + binary_search( connection_itr, current_time, 0, (num_forward_edges() - 1) );
//
//				if(connection_itr > end_connection_itr)
//				{
//					THROW_EXCEPTION("Binary search went awry!");
//				}
//			}
//
//			for(; connection_itr != end_connection_itr; ++connection_itr)
//			{
//				if( connection_itr->_departure_time < current_time )
//				{
//					//cout << "\tskipping: " << connection_itr->_departure_time << endl;
//					continue;
//				}
//				else if( connection_itr->_departure_time < horizon_time )
//				{
//					Evaluate_Neighbor<AgentType>(agent,current,connection_itr,routing_data);
//				}
//				else
//				{
//					//cout << "\tbreaking: " << connection_itr->_departure_time << endl;
//					break;
//				}
//			}
//		}
//
//		return (Anonymous_Connection_Group<MasterType,base_edge_type>*)end_connection_itr;
//	}
//
//	template<typename AgentType>
//	void Evaluate_Neighbor(Routable_Agent<AgentType>* agent, current_edge_type* current, connection_type* connection, Routing_Data<base_edge_type>& routing_data)
//	{
//		A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection->neighbor();
//
//		if(current_neighbor->in_closed_set()) return;
//
//		transit_edge_id id;
//		
//		id.id = current_neighbor->edge_id();
//
//		//cout << "\t\tNeighbor ID: " << id.composite_id.time << ", " << id.composite_id.trip_id << endl;
//
//		A_Star_Edge<current_edge_type>* current_edge = (A_Star_Edge<current_edge_type>*)current;
//
//		float cost_from_origin = current->cost_from_origin() + agent->cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
//
//		if( cost_from_origin < current_neighbor->cost_from_origin() )
//		{
//			if( current_neighbor->in_open_set() ) routing_data.open_set->erase( routing_data.open_set->iterator_to( *((base_edge_type*)current_neighbor)  ) );
//
//			float time_cost_between = agent->time_cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
//			//float time_from_origin = current->time_from_origin() + time_cost_between;
//
//			if(!current_neighbor->marked_for_reset())
//			{
//				routing_data.modified_edges->push_back((base_edge_type*)current_neighbor);
//				current_neighbor->marked_for_reset(true);
//			}
//				
//			current_neighbor->came_from( current );
//
//			current_neighbor->cost_from_origin( cost_from_origin );
//			//current_neighbor->time_from_origin( time_from_origin );
//				
//			current_neighbor->time_label( current_edge->time_label() + time_cost_between );
//
//			float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, routing_data.end_edge);
//
//			//cout << "\t\t\tcurrent time: " << current_neighbor->time_label() << ", current cost: " << cost_from_origin << ", estimated cost OD: " << neighbor_estimated_cost_origin_destination << endl;
//
//			current_neighbor->estimated_cost_origin_destination( neighbor_estimated_cost_origin_destination );
//				
//			routing_data.open_set->insert( *((base_edge_type*)current_neighbor) );
//
//			current_neighbor->in_open_set(true);
//
//			// update the label
//			agent->update_label(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
//		}
//	}
//};
//
//template<typename MasterType>
//struct transit_attributes : public Base_Edge_A_Star<MasterType>
//{
//	transit_attributes():_transfers_made(0),_walk_transfers_made(0){}
//
//	virtual void reset()
//	{
//		Base_Edge_A_Star<MasterType>::reset();
//
//		_transfers_made = 0;
//		_walk_transfers_made = 0;
//	}
//
//	t_data(unsigned int,transfers_made);
//	t_data(unsigned int,walk_transfers_made);
//};
//
//template<typename MasterType>
//struct stop_attributes : public Base_Edge_A_Star<MasterType>
//{
//	stop_attributes():_transfers_made(0),_walk_transfers_made(0){}
//
//	virtual void reset()
//	{
//		Base_Edge_A_Star<MasterType>::reset();
//
//		_transfers_made = 0;
//		_walk_transfers_made = 0;
//	}
//
//	t_data(unsigned int,transfers_made);
//	t_data(unsigned int,walk_transfers_made);
//
//	t_data(float,cost);
//	t_data(float,time_cost);
//};
//
//struct ride_on
//{
//	t_data(float,cost);
//	t_data(float,time_cost);
//};
//
//struct transfer_at_stop
//{
//	t_data(float,cost);
//	t_data(float,time_cost);
//};
//
//struct walk_transfer
//{
//	t_data(float,cost);
//	t_data(float,time_cost);
//};
//
//struct transit_to_stop
//{
//	t_data(float,cost);
//	t_data(float,time_cost);
//};
//
//struct stop_to_transit
//{
//	t_data(float,cost);
//	t_data(float,time_cost);
//	t_data(unsigned int,departure_time);
//};
//
//struct stop_to_stop
//{
//	t_data(float,cost);
//	t_data(float,time_cost);
//};
//
//
//
//
//struct walk_data
//{
//	long long dest_stop_id;
//	float distance;
//};
//
//struct stop_time_data
//{
//	long long stop_id;
//
//	unsigned int trip_id;
//	unsigned int time;
//};
//
//struct stop_data
//{
//	float x;
//	float y;
//	std::vector<walk_data> nearby_stops;
//	std::list<stop_time_data*> outbound_transit_legs;
//	//std::list<stop_time_data*> outbound_walk_transit_legs;
//};
//
//std::unordered_map< long long, unsigned int > stop_id_to_index;
//
//std::unordered_map< string, unsigned int > string_to_stop_index;
//std::unordered_map< string, unsigned int > string_to_trip_index;
//
//std::unordered_map< unsigned int, string > stop_index_to_string;
//std::unordered_map< unsigned int, string > trip_index_to_string;
//
//std::unordered_map< unsigned int, stop_data > stops;
//std::unordered_map< unsigned int, std::vector<stop_time_data> > stop_times;
//
//void Read_Stops()
//{
//	ifstream network_file("PolarisStops.csv",ifstream::binary);
//
//	network_file.seekg(0,ios::end);
//
//	int length = (int)network_file.tellg();
//
//	network_file.seekg(0,ios::beg);
//
//	char* file = new char[length];
//	
//	network_file.read(file,length);
//
//	const char* const eof = file + length;
//
//	char* file_itr = file;
//	
//	string token;
//	
//	unsigned int field_counter = 0;
//
//	bool skip_line = true;
//
//	bool in_quote = false;
//	stop_data* current_stop = nullptr;
//
//	unsigned int stop_counter = 0;
//
//	boost::hash<string> stop_id_hash;
//
//	while(file_itr != eof)
//	{
//		if(!skip_line)
//		{
//			if(*file_itr == '\"')
//			{
//				if(!in_quote)
//				{
//					in_quote = true;
//				}
//				else
//				{
//					in_quote = false;
//				}
//			}
//			else if(*file_itr == ',' && !in_quote)
//			{
//				if(field_counter == 0)
//				{
//					//long long stop_id = stoll(token);
//					long long stop_id = stop_id_hash(token);
//
//					unsigned int stop_index = stop_counter;
//
//					stop_index_to_string[stop_index] = token;
//					string_to_stop_index[token] = stop_index;
//
//					if(stop_id_to_index.count(stop_id))
//					{
//						THROW_EXCEPTION("Hash collision converting stop string id to long long");
//					}
//
//					stop_id_to_index[stop_id] = stop_index;
//
//					current_stop = &(stops[stop_index]);
//
//					++stop_counter;
//				}
//				else if(field_counter == 4)
//				{
//					current_stop->x = stof(token);
//				}
//				else if(field_counter == 5)
//				{
//					current_stop->y = stof(token);
//
//					skip_line = true;
//				}
//
//				token.clear();
//
//				++field_counter;
//			}
//			else if(*file_itr == '\n' && !in_quote)
//			{
//				field_counter = 0;
//				token.clear();
//			}
//			else if(*file_itr != '\r')
//			{
//				token.push_back(*file_itr);
//			}
//		}
//		else
//		{
//			if(*file_itr == '\n')
//			{
//				skip_line = false;
//				field_counter = 0;
//			}
//		}
//
//		++file_itr;
//	}
//
//	delete[] file;
//}
//
//void Compute_Nearby_Stops()
//{
//	// .25 miles
//	const float nearby_distance = 402.336f;
//
//	float sum_nearby_stops = 0.0f;	
//	float stop_counter = 0.0f;
//
//	for(std::unordered_map<unsigned int,stop_data>::iterator itr = stops.begin(); itr != stops.end(); itr++)
//	{
//		const long long current_id = itr->first;
//		const float current_x = itr->second.x;
//		const float current_y = itr->second.y;
//
//		for(std::unordered_map<unsigned int,stop_data>::iterator neighbor_itr = stops.begin(); neighbor_itr != stops.end(); neighbor_itr++)
//		{
//			float neighbor_x_delta = abs(neighbor_itr->second.x - current_x);
//			//neighbor_x_delta *= neighbor_x_delta;
//			
//			float neighbor_y_delta = abs(neighbor_itr->second.y - current_y);
//			//neighbor_y_delta *= neighbor_y_delta;
//			
//			const float dist = neighbor_x_delta + neighbor_y_delta;
//			//const float dist = sqrt(neighbor_x_delta + neighbor_y_delta);
//
//			if(dist < nearby_distance)
//			{
//				if(neighbor_itr->first == current_id) continue;
//
//				++sum_nearby_stops;
//
//				itr->second.nearby_stops.push_back( walk_data() );
//
//				itr->second.nearby_stops.back().distance = dist;
//				itr->second.nearby_stops.back().dest_stop_id = neighbor_itr->first;
//			}
//		}
//
//		++stop_counter;
//	}
//
//	//cout << "Average nearby stops: " << sum_nearby_stops / stop_counter << endl;
//}
//
//void Read_Stop_Times()
//{
//	ifstream network_file("PolarisStopTimes.csv",ifstream::binary);
//
//	network_file.seekg(0,ios::end);
//
//	int length = (int)network_file.tellg();
//
//	network_file.seekg(0,ios::beg);
//
//	char* file = new char[length];
//	
//	network_file.read(file,length);
//
//	const char* const eof = file + length;
//
//	char* file_itr = file;
//	
//	string token;
//
//	unsigned int field_counter = 0;
//
//	bool skip_line = true;
//
//	std::vector<stop_time_data>* current_stop_time = nullptr;
//
//	std::vector<stop_time_data> template_stops;
//	std::vector<stop_time_data>::iterator template_stops_itr;
//
//	enum record_mode { NULL_MODE, STOP_MODE, ARRIVAL_MODE };
//
//	record_mode current_record_mode = NULL_MODE;
//
//	unsigned int line_counter = 0;
//	unsigned int trip_counter = 0;
//	unsigned int current_trip_id = 0;
//
//	boost::hash<string> stop_id_hash;
//
//	unsigned int previous_time = 0;
//	bool check_time_collision = false;
//
//	while(file_itr != eof)
//	{
//		if(!skip_line)
//		{
//			if(*file_itr == ',')
//			{
//				if(field_counter == 1)
//				{
//					trip_index_to_string[trip_counter] = token;
//					string_to_trip_index[token] = trip_counter;
//				}
//				else if(field_counter == 4)
//				{
//					if(token == "STOP")
//					{
//						current_record_mode = STOP_MODE;
//
//						template_stops.clear();
//					}
//					else if(token == "ARRIVAL")
//					{
//						current_trip_id = trip_counter;
//						
//						current_stop_time = &(stop_times[trip_counter]);
//
//						++trip_counter;
//
//						current_record_mode = ARRIVAL_MODE;
//
//						template_stops_itr = template_stops.begin();
//					}
//					else
//					{
//						THROW_EXCEPTION("Mode not recognized!: " << token << "," << line_counter);
//					}
//				}
//				else if(field_counter > 4)
//				{
//					if(current_record_mode == STOP_MODE)
//					{
//						long long stop_id = stop_id_hash(token);
//
//						unsigned int stop_index = stop_id_to_index[stop_id];
//
//						template_stops.push_back(stop_time_data());
//
//						template_stops.back().stop_id = stop_index;
//					}
//					else if(current_record_mode == ARRIVAL_MODE)
//					{
//						template_stops_itr->time = (unsigned int)stoul(token);
//						
//						if(check_time_collision)
//						{
//							if(previous_time >= template_stops_itr->time)
//							{
//								THROW_EXCEPTION("Time collision, cannot resolve multiple stops at the same start time!");
//							}
//
//							check_time_collision = false;
//						}
//
//						if(template_stops_itr->time == previous_time)
//						{
//							template_stops_itr->time += 1;
//							check_time_collision = true;
//						}
//
//						previous_time = template_stops_itr->time;
//
//						template_stops_itr->trip_id = current_trip_id;
//
//						++template_stops_itr;
//					}
//				}
//
//				token.clear();
//
//				++field_counter;
//			}
//			else if(*file_itr == '\n')
//			{
//				if(current_record_mode == STOP_MODE)
//				{
//					long long stop_id = stop_id_hash(token);
//
//					unsigned int stop_index = stop_id_to_index[stop_id];
//
//					template_stops.push_back(stop_time_data());
//
//					template_stops.back().stop_id = stop_index;
//				}
//				else if(current_record_mode == ARRIVAL_MODE)
//				{
//					template_stops_itr->time = (unsigned int)stoul(token);
//
//					if(check_time_collision)
//					{
//						if(previous_time >= template_stops_itr->time)
//						{
//							THROW_EXCEPTION("Time collision, cannot resolve multiple stops at the same start time!");
//						}
//
//						check_time_collision = false;
//					}
//
//					if(template_stops_itr->time == previous_time)
//					{
//						template_stops_itr->time += 1;
//						check_time_collision = true;
//					}
//
//					previous_time = template_stops_itr->time;
//
//					template_stops_itr->trip_id = current_trip_id;
//
//					for(template_stops_itr = template_stops.begin();template_stops_itr != template_stops.end();template_stops_itr++)
//					{
//						current_stop_time->push_back(stop_time_data());
//						current_stop_time->back().stop_id = template_stops_itr->stop_id;
//						current_stop_time->back().time = template_stops_itr->time;
//						current_stop_time->back().trip_id = template_stops_itr->trip_id;
//					}
//				}
//
//				++line_counter;
//				field_counter = 0;
//
//				token.clear();
//
//				check_time_collision = false;
//			}
//			else if(*file_itr != '\r')
//			{
//				token.push_back(*file_itr);
//			}
//		}
//		else
//		{
//			if(*file_itr == '\n')
//			{
//				++line_counter;
//				skip_line = false;
//				
//				field_counter = 0;
//
//				token.clear();
//
//				check_time_collision = false;
//			}
//		}
//
//		++file_itr;
//	}
//
//	delete[] file;
//}
//
//void Compute_Outbound_Trip_Options()
//{
//	for(std::unordered_map<unsigned int,std::vector<stop_time_data>>::iterator itr = stop_times.begin(); itr != stop_times.end(); itr++)
//	{
//		stop_time_data* current_stop_time = nullptr;
//		stop_time_data* cached_stop_time = nullptr;
//		
//		for(std::vector<stop_time_data>::iterator stop_times_itr = itr->second.begin(); stop_times_itr != itr->second.end(); stop_times_itr++)
//		{
//			// the last stop is not really an outbound connection, more of an endpoint
//
//			++stop_times_itr;
//
//			if(stop_times_itr == itr->second.end()) break;
//
//			--stop_times_itr;
//
//			current_stop_time = &(*stop_times_itr);
//
//			stops[current_stop_time->stop_id].outbound_transit_legs.push_back(current_stop_time);
//		}
//	}
//}
//
//template <>
//struct less<stop_time_data*>
//{
//	bool operator() (const stop_time_data* x, const stop_time_data* y) const
//	{
//		return x->time < y->time;
//	}
//
//	typedef stop_time_data* first_argument_type;
//	typedef stop_time_data* second_argument_type;
//	typedef bool result_type;
//};
//
//void Sort_Outbound_Trip_Options()
//{
//	less<stop_time_data*> comparator;
//
//	for(std::unordered_map< unsigned int, stop_data >::iterator itr = stops.begin(); itr != stops.end(); itr++)
//	{
//		itr->second.outbound_transit_legs.sort(comparator);
//	}
//}
//
//template<typename MasterType>
//void Construct_Stop_Graph(Graph_Assembler_Connected_Edge<typename MasterType::stop_graph_type>* stop_graph, graph_id_type neighbor_graph_id)
//{
//	Input_Edge<stop_attributes<MT>> input_stop_edge;
//
//	// Get on transit from this stop
//	Input_Connection_Group_Implementation<typename MasterType::stop_to_transit_connection>::_neighbor_graph_id = neighbor_graph_id;
//	Input_Connection_Group_Implementation<typename MasterType::stop_to_transit_connection>* stop_to_transit_connection_group = new Input_Connection_Group_Implementation<typename MasterType::stop_to_transit_connection>();
//	stop_to_transit connection_attributes;
//
//	// Move between stops
//	Input_Connection_Group_Implementation<typename MasterType::stop_to_stop_connection>::_neighbor_graph_id = stop_graph->graph_id();
//	Input_Connection_Group_Implementation<typename MasterType::stop_to_stop_connection>* stop_to_stop_connection_group = new Input_Connection_Group_Implementation<typename MasterType::stop_to_stop_connection>();
//	stop_to_stop walk_attributes;
//
//
//
//	// scan all stops, create an edge for each one which has outbound trips
//	for(std::unordered_map<unsigned int,stop_data>::iterator itr = stops.begin(); itr != stops.end(); itr++)
//	{
//		// Fill input stop edge attributes
//		input_stop_edge._x = itr->second.x;
//		input_stop_edge._y = itr->second.y;
//		input_stop_edge._edge_id = (edge_id_type)itr->first;
//
//		transit_edge_id linked_transit_edge;
//
//		// find the oubound transit legs corresponding to this stop
//		for(std::list<stop_time_data*>::iterator outbound_itr = itr->second.outbound_transit_legs.begin(); outbound_itr != itr->second.outbound_transit_legs.end(); outbound_itr++)
//		{
//			// id for transit is a composite of which trip and when it departs
//			linked_transit_edge.composite_id.trip_id = (*outbound_itr)->trip_id;
//			linked_transit_edge.composite_id.time = (*outbound_itr)->time;
//
//			// add this id, it will be linked upon graph compilation
//			stop_to_transit_connection_group->_neighbors.push_back(linked_transit_edge.id);
//
//			connection_attributes._departure_time = linked_transit_edge.composite_id.time;
//
//			// the costs will be largely determined by the agent and how fast they alight
//			connection_attributes._cost = 0.0f;
//			connection_attributes._time_cost = 0.0f;
//
//			stop_to_transit_connection_group->_neighbor_attributes.push_back(connection_attributes);
//
//			// make sure connection_attributes is sufficiently cleaned up here
//		}
//		
//		// Add the connection group to the input stop, it will be copied, then cleared and re-used similar to the input stop edge		
//		input_stop_edge._connection_groups.push_back(stop_to_transit_connection_group);
//
//
//		// find the nearby transit legs corresponding to this stop
//
//		for(std::vector<walk_data>::iterator walk_itr = itr->second.nearby_stops.begin();walk_itr != itr->second.nearby_stops.end(); walk_itr++)
//		{
//			// add this id, it will be linked upon graph compilation
//			stop_to_stop_connection_group->_neighbors.push_back(walk_itr->dest_stop_id);
//
//			// the costs will be largely determined by the agent and how fast they walk (assume 2mph)
//			walk_attributes._cost = walk_itr->distance / 0.89408f;
//			walk_attributes._time_cost = walk_itr->distance / 0.89408f;
//
//			stop_to_stop_connection_group->_neighbor_attributes.push_back(walk_attributes);
//
//			// make sure walk_attributes is sufficiently cleaned up here
//		}
//
//		// add only if there are nearby stops
//		if(itr->second.nearby_stops.size()) input_stop_edge._connection_groups.push_back(stop_to_stop_connection_group);
//
//		
//		
//		stop_graph->Add_Edge<stop_attributes<MT>>( &input_stop_edge );
//
//		// make sure input_stop_edge is sufficiently cleaned up here
//
//		// make sure stop_to_transit_connection_group is sufficiently cleaned up here
//		stop_to_transit_connection_group->_neighbors.clear();
//		stop_to_transit_connection_group->_neighbor_attributes.clear();
//
//		// make sure stop_to_stop_connection_group is sufficiently cleaned up here
//		stop_to_stop_connection_group->_neighbors.clear();
//		stop_to_stop_connection_group->_neighbor_attributes.clear();
//
//		// clean up input edge
//		input_stop_edge._connection_groups.clear();
//	}
//
//	stop_graph->Compile_Graph<stop_attributes<MT>>();
//
//	cout << ((typename MasterType::stop_graph_type*)stop_graph)->_ordered_edge_reference->size() << endl;
//}
//
//template<typename MasterType>
//void Construct_Transit_Graph(Graph_Assembler_Connected_Edge<typename MasterType::transit_graph_type>* transit_graph, graph_id_type neighbor_graph_id)
//{
//	float sum_nearby_stops = 0.0f;	
//	float stop_counter = 0.0f;
//
//	// should probably ultimately be a fully agent-based factor, dependent on agent and bus
//	const int avg_alighting_delay = 15;
//	const int avg_boarding_delay = 30;
//	const int avg_transfer_delay = 30;
//
//	// people will currently wait 10 minutes for a bus
//	const int avg_transfer_horizon = 30*60;
//
//	Input_Edge<transit_attributes<MT>> input_transit_edge;
//	
//	// Continue on the same trip
//	Input_Connection_Group_Implementation<typename MasterType::ride_on_connection>::_neighbor_graph_id = transit_graph->graph_id();
//	Input_Connection_Group_Implementation<typename MasterType::ride_on_connection>* ride_on_connection_group = new Input_Connection_Group_Implementation<typename MasterType::ride_on_connection>();
//	ride_on ride_on_attributes;
//
//	// Get off at the stop and get on a different trip at the same stop
//	Input_Connection_Group_Implementation<typename MasterType::transfer_at_stop_connection>::_neighbor_graph_id = transit_graph->graph_id();
//	Input_Connection_Group_Implementation<typename MasterType::transfer_at_stop_connection>* transfer_at_stop_connection_group = new Input_Connection_Group_Implementation<typename MasterType::transfer_at_stop_connection>();
//	transfer_at_stop transfer_at_stop_attributes;
//
//	// Get off at the stop, walk to another stop and get on a different trip - not being added for right now
//	Input_Connection_Group_Implementation<typename MasterType::walk_transfer_connection>::_neighbor_graph_id = transit_graph->graph_id();
//	Input_Connection_Group_Implementation<typename MasterType::walk_transfer_connection>* walk_transfer_connection_group = new Input_Connection_Group_Implementation<typename MasterType::walk_transfer_connection>();
//	walk_transfer walk_transfer_attributes;
//
//	// Get off at a stop and switch graphs
//	Input_Connection_Group_Implementation<typename MasterType::transit_to_stop_connection>::_neighbor_graph_id = neighbor_graph_id;
//	Input_Connection_Group_Implementation<typename MasterType::transit_to_stop_connection>* transit_to_stop_connection_group = new Input_Connection_Group_Implementation<typename MasterType::transit_to_stop_connection>();
//	transit_to_stop transit_to_stop_attributes;
//
//	transit_edge_id current_id;
//
//	transit_edge_id transit_connection_id;
//	edge_id_type stop_connection_id;
//
//	unsigned int arrival_time_to_next;
//	unsigned int next_stop_id;
//
//	// scan stop times
//	for(std::unordered_map< unsigned int, std::vector<stop_time_data> >::iterator itr = stop_times.begin(); itr != stop_times.end(); itr++)
//	{
//		// build one edge for each trip leg which represents the segment from one departure to another
//		for(std::vector<stop_time_data>::iterator stop_time_itr = itr->second.begin(); stop_time_itr != itr->second.end(); stop_time_itr++)
//		{
//			// peek ahead and, if available, log the "arrival" time for the purposes of adding the time cost
//
//			++stop_time_itr;
//
//			if(stop_time_itr != itr->second.end())
//			{
//				arrival_time_to_next = stop_time_itr->time;
//				next_stop_id = stop_time_itr->stop_id;
//			}
//			else
//			{
//				// don't add the final edge, all of the connection options have already been accounted for in the previous edge
//				break;
//			}
//
//			--stop_time_itr;
//
//
//
//			// id for transit is a composite of which trip and when it departs
//
//			current_id.composite_id.time = stop_time_itr->time;
//			current_id.composite_id.trip_id = stop_time_itr->trip_id;
//
//			// the departure information indicates the id
//			input_transit_edge._edge_id = (edge_id_type)current_id.id;
//			
//			// the coordinates of this edge are based off of the departure stop
//			input_transit_edge._x = stops[stop_time_itr->stop_id].x;
//			input_transit_edge._y = stops[stop_time_itr->stop_id].y;
//
//			// time cost is the how long it takes between departure and arrival
//			input_transit_edge._time_cost = (float)(arrival_time_to_next - current_id.composite_id.time);
//			
//			//input_transit_edge._cost = FLT_MAX / 2.0f;
//
//
//			// while we know that there is a forward leg, add the "ride on" connection
//			// there is no "ride on" connection if the next stop is the last stop
//
//			// peek twice ahead to see whether there is a valid departure for this trip from the next stop
//
//			++stop_time_itr;
//			++stop_time_itr;
//
//			if(stop_time_itr != itr->second.end())
//			{
//				// fill in the forward id, since arrival time == departure time, we know the departure time and hence the id
//				transit_connection_id.composite_id.time = arrival_time_to_next;
//				transit_connection_id.composite_id.trip_id = stop_time_itr->trip_id;
//
//				ride_on_connection_group->_neighbors.push_back((edge_id_type)transit_connection_id.id);
//
//				// fill in attributes corresponding to this "ride on" connection
//
//				// cost would be the wait time at the next stop if there were any
//				ride_on_attributes._time_cost = 0;
//				ride_on_attributes._cost = FLT_MAX / 2.0f;
//
//				ride_on_connection_group->_neighbor_attributes.push_back(ride_on_attributes);
//
//				input_transit_edge._connection_groups.push_back(ride_on_connection_group);
//			}
//
//			--stop_time_itr;
//			--stop_time_itr;
//
//			
//
//
//			// add the option to let someone off at the next stop
//
//			// at one point I thought this conditional made sense, leave it for now
//			//if(stop_time_itr != itr->second.begin())
//			//{
//				// stop id to link to in the other graph
//				transit_to_stop_connection_group->_neighbors.push_back(next_stop_id);
//			
//				// fill in attributes corresponding to this "transit to stop" connection
//
//				// cost is the average time it takes for everyone to get off plus some agent-based factor
//				transit_to_stop_attributes._time_cost = (float)(avg_alighting_delay);
//				ride_on_attributes._cost = FLT_MAX / 2.0f;
//
//				transit_to_stop_connection_group->_neighbor_attributes.push_back(transit_to_stop_attributes);
//
//				input_transit_edge._connection_groups.push_back(transit_to_stop_connection_group);
//			//}
//
//
//			// add the option to get off the bus and transfer to another trip at the same stop
//
//			//std::list<stop_time_data*>* outbound_transit_trips = &stops[stop_time_itr->stop_id].outbound_transit_legs;
//			std::list<stop_time_data*>* outbound_transit_trips = &stops[next_stop_id].outbound_transit_legs;
//
//			for(std::list<stop_time_data*>::iterator stop_transfer_itr = outbound_transit_trips->begin(); stop_transfer_itr != outbound_transit_trips->end(); stop_transfer_itr++)
//			{
//				// skip the "ride on" connection, it has already been handled
//
//				if( (*stop_transfer_itr)->trip_id == stop_time_itr->trip_id ) continue;
//
//				// only allow transfers that start after the current time and within a reasonable time horizon - accounting for alighting and boarding delay
//
//				// trip occurs before the agent can make the transfer
//				if( (*stop_transfer_itr)->time < (avg_alighting_delay + avg_boarding_delay + avg_transfer_delay + arrival_time_to_next) ) continue;
//					
//				// trip occurs farther in the future than the agent is willing to wait - these are time-sorted so we can break at this point
//				if( (*stop_transfer_itr)->time > (arrival_time_to_next + avg_alighting_delay + avg_transfer_horizon) ) break;
//
//				transit_connection_id.composite_id.time = (*stop_transfer_itr)->time;
//				transit_connection_id.composite_id.trip_id = (*stop_transfer_itr)->trip_id;
//
//				transfer_at_stop_connection_group->_neighbors.push_back((edge_id_type)transit_connection_id.id);
//				
//				// time cost is the amount of time required to alight, transfer, board, and depart
//				// or, in other terms, if it is a valid transfer then the departure time - arrival time
//
//				transfer_at_stop_attributes._time_cost = (float)((*stop_transfer_itr)->time - arrival_time_to_next);
//				transfer_at_stop_attributes._cost = FLT_MAX / 2.0f;
//
//				transfer_at_stop_connection_group->_neighbor_attributes.push_back(transfer_at_stop_attributes);
//
//					
//
//				sum_nearby_stops++;
//			}
//
//			input_transit_edge._connection_groups.push_back(transfer_at_stop_connection_group);
//
//			// add the option to get off the bus, walk to another stop, and transfer to another trip
//
//			//std::vector<walk_data>* nearby_stops = &stops[stop_time_itr->stop_id].nearby_stops;
//			std::vector<walk_data>* nearby_stops = &stops[next_stop_id].nearby_stops;
//
//			for(std::vector<walk_data>::iterator nearby_itr = nearby_stops->begin(); nearby_itr != nearby_stops->end(); nearby_itr++)
//			{
//				stop_data* nearby_stop = &stops[nearby_itr->dest_stop_id];
//				
//				// 2 mph in m/s - eventually this should probably be more agent-determined
//				const float walk_time = nearby_itr->distance / 0.89408f;
//
//				for(std::list<stop_time_data*>::iterator stop_transfer_itr = nearby_stop->outbound_transit_legs.begin(); stop_transfer_itr != nearby_stop->outbound_transit_legs.end(); stop_transfer_itr++)
//				{
//					//if(stop_time_itr->trip_id == 25980 /*&& arrival_time_to_next == 70283*/ && ((*stop_transfer_itr)->trip_id == 26370 || (*stop_transfer_itr)->trip_id == 26371)) cout << walk_time << "," << (*stop_transfer_itr)->time << "," << (*stop_transfer_itr)->trip_id << "," << arrival_time_to_next << endl;
//					
//					// skip the distant "ride on" connection, it will be handled later this trip
//
//					if( (*stop_transfer_itr)->trip_id == stop_time_itr->trip_id ) continue;
//
//					// only allow transfers that start after the current time and within a reasonable time horizon - accounting for alighting, walking, and boarding delay
//
//					// trip occurs before the agent can make the transfer
//					if( (*stop_transfer_itr)->time < (avg_alighting_delay + avg_boarding_delay + avg_transfer_delay + arrival_time_to_next + walk_time) ) continue;
//					
//					// trip occurs farther in the future than the agent is willing to wait - these are time-sorted so we can break at this point
//					if( (*stop_transfer_itr)->time > (arrival_time_to_next + avg_alighting_delay + avg_transfer_horizon + walk_time) ) break;
//
//					transit_connection_id.composite_id.time = (*stop_transfer_itr)->time;
//					transit_connection_id.composite_id.trip_id = (*stop_transfer_itr)->trip_id;
//
//					walk_transfer_connection_group->_neighbors.push_back( (edge_id_type)transit_connection_id.id );
//					
//					// time cost is the amount of time required to alight, transfer, walk, board, and depart
//					// or, in other terms, if it is a valid transfer then the departure time - arrival time
//
//					walk_transfer_attributes._time_cost = (float)( (*stop_transfer_itr)->time - arrival_time_to_next );
//					walk_transfer_attributes._cost = FLT_MAX / 2.0f;
//
//					walk_transfer_connection_group->_neighbor_attributes.push_back(walk_transfer_attributes);
//
//					sum_nearby_stops++;
//				}
//			}
//
//			input_transit_edge._connection_groups.push_back(walk_transfer_connection_group);
//
//			stop_counter++;
//
//			// add the edge
//
//			transit_graph->Add_Edge<transit_attributes<MT>>( &input_transit_edge );
//
//			// clean up ride-on connection
//
//			ride_on_connection_group->_neighbors.clear();
//			ride_on_connection_group->_neighbor_attributes.clear();
//
//			// clean up "transit to stop" connection
//
//			transit_to_stop_connection_group->_neighbors.clear();
//			transit_to_stop_connection_group->_neighbor_attributes.clear();
//
//			// clean up "transfer at stop" connection
//
//			transfer_at_stop_connection_group->_neighbors.clear();
//			transfer_at_stop_connection_group->_neighbor_attributes.clear();
//
//			// clean up "walk transfer" connection
//
//			walk_transfer_connection_group->_neighbors.clear();
//			walk_transfer_connection_group->_neighbor_attributes.clear();
//
//			// clean up input edge
//
//			input_transit_edge._connection_groups.clear();
//		}
//	}
//	
//	//cout << "Average walk transfers: " << sum_nearby_stops / stop_counter << endl;
//
//	transit_graph->Compile_Graph<transit_attributes<MT>>();
//
//	cout << ((typename MasterType::transit_graph_type*)transit_graph)->_ordered_edge_reference->size() << endl;
//}
//
//
//
//
//
//struct MasterType
//{
//	typedef Graph_Implementation<MasterType, NTL, Base_Edge_A_Star<MasterType>> base_graph_type;
//
//	typedef Edge_Implementation<transit_attributes<MasterType>> transit_edge;
//	typedef Edge_Implementation<stop_attributes<MasterType>> stop_edge;
//
//	typedef Graph_Implementation<MasterType, NTL, transit_edge> transit_graph_type;
//	typedef Graph_Implementation<MasterType, NTL, stop_edge> stop_graph_type;
//
//	typedef Custom_Connection_Group<MasterType, transit_graph_type, transit_graph_type, ride_on> ride_on_connection;
//	typedef Custom_Connection_Group<MasterType, transit_graph_type, transit_graph_type, transfer_at_stop> transfer_at_stop_connection;
//	typedef Custom_Connection_Group<MasterType, transit_graph_type, transit_graph_type, walk_transfer> walk_transfer_connection;
//	typedef Custom_Connection_Group<MasterType, transit_graph_type, stop_graph_type, transit_to_stop> transit_to_stop_connection;
//	
//	typedef Custom_Connection_Group<MasterType, stop_graph_type, transit_graph_type, stop_to_transit> stop_to_transit_connection;
//	typedef Custom_Connection_Group<MasterType, stop_graph_type, stop_graph_type, stop_to_stop> stop_to_stop_connection;
//
//	typedef Graph_Pool_Implementation<MasterType, NTL, base_graph_type> graph_pool_type;
//
//	typedef Routable_Agent_Implementation<MasterType> routable_agent_type;
//
//	typedef ride_on ride_on_type;
//	typedef transfer_at_stop transfer_at_stop_type;
//	typedef walk_transfer walk_transfer_type;
//	typedef transit_to_stop transit_to_stop_type;
//	typedef stop_to_transit stop_to_transit_type;
//	typedef stop_to_stop stop_to_stop_type;
//};
//
//
////4754616
//
//
//
////21537
//void main()
//{
//	cout << "Beginning Transit Router Construction" << endl;
//
//	Read_Stops();
//
//	cout << "Done Reading Stops" << endl;
//
//	Compute_Nearby_Stops();
//
//	cout << "Done Computing Nearby Stops" << endl;
//
//	Read_Stop_Times();
//
//	cout << "Done Reading Stop Times" << endl;
//
//
//
//	//std::unordered_map< unsigned int, string > stop_index_to_string;
//	//std::unordered_map< unsigned int, string > trip_index_to_string;
//
//	//cout << trip_index_to_string[26369] << "," << trip_index_to_string[26369] << endl;
//
//	//cout << string_to_stop_index[string("41400")] << endl;
//
//	//cout << stop_index_to_string[9740] << endl;
//
//	//cout << string_to_trip_index[string("43067053926")] << endl;
//
//
//
//	Compute_Outbound_Trip_Options();
//
//	cout << "Done Computing Outbound Trip Options" << endl;
//
//	Sort_Outbound_Trip_Options();
//
//	cout << "Done Sorting Outbound Trip Options" << endl;
//
//	Graph_Pool<MasterType::graph_pool_type>* graph_pool = (Graph_Pool<MasterType::graph_pool_type>*)new MasterType::graph_pool_type();
//	
//	Graph_Assembler_Connected_Edge<MasterType::stop_graph_type>* stop_graph = graph_pool->Create_New_Graph<MasterType::stop_graph_type>();
//
//	Graph_Assembler_Connected_Edge<MasterType::transit_graph_type>* transit_graph = graph_pool->Create_New_Graph<MasterType::transit_graph_type>();
//
//	cout << "Done Creating Graphs" << endl;
//
//	Construct_Stop_Graph<MasterType>( stop_graph, transit_graph->graph_id() );
//
//	cout << "Done Constructing Stop Graph" << endl;
//
//	Construct_Transit_Graph<MasterType>( transit_graph, stop_graph->graph_id() );
//
//	cout << "Done Constructing Transit Graph" << endl;
//
//	graph_pool->Link_Graphs();
//
//	cout << "Done Linking Graphs" << endl;
//
//	
//	
//	std::deque<global_edge_id> path_container;
//
//	Routable_Agent_Implementation<MasterType> bob;
//
//	bob._wait_horizon = 30*60;
//
//	global_edge_id start_id;
//	transit_edge_id edge_id;
//
//	start_id.graph_id = stop_graph->graph_id();
//
//	global_edge_id end_id;
//
//	end_id.graph_id = stop_graph->graph_id();
//
//	// Validation 1
//	start_id.edge_id = string_to_stop_index[string("15204")];
//	end_id.edge_id = string_to_stop_index[string("30154")];
//	unsigned int start_time = 60*60*12 + 60*60*3 + 10*60;
//
//	// Validation 2
//	//start_id.edge_id = string_to_stop_index[string("17361")];
//	//end_id.edge_id = string_to_stop_index[string("30080")];
//	//unsigned int start_time = 60*60*12 + 60*60*2 + 60*10;
//
//	// Validation 3
//	//start_id.edge_id = string_to_stop_index[string("834s0485")];
//	//end_id.edge_id = string_to_stop_index[string("30269")];
//	//unsigned int start_time = 60*60*12 + 60*60*5;
//
//	LARGE_INTEGER frequency;
//	LARGE_INTEGER start;
//	LARGE_INTEGER end;
//
//	QueryPerformanceFrequency(&frequency);
//
//	QueryPerformanceCounter(&start);
//	A_Star<MasterType,Routable_Agent_Implementation<MasterType>,MasterType::graph_pool_type>((Routable_Agent<Routable_Agent_Implementation<MasterType>>*) &bob, graph_pool, start_id, end_id, start_time, path_container);
//	QueryPerformanceCounter(&end);
//
//
//	cout << "Run Time: " << ((float)(end.QuadPart - start.QuadPart)) / ((float)frequency.QuadPart) << endl;
//}