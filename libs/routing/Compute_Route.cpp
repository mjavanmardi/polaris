//#include "io\database.h"
//#include "io\Supply.h"
//#include "Core\Core.h"
//#include "Routing\Routing.h"
//
//using namespace polaris;
//
//implementation struct Input_Edge_Implementation : public Polaris_Component<MasterType,INHERIT(Input_Edge_Implementation)>
//{
//	m_data(long long,id,NONE,NONE);
//
//	m_container(std::vector<long long>,forward_edge_ids,NONE,NONE);
//	m_container(std::vector<long long>,backward_edge_ids,NONE,NONE);
//
//	m_data(float,pos_x,NONE,NONE);
//	m_data(float,pos_y,NONE,NONE);
//
//	m_data(float,cost,NONE,NONE);
//};
//
//implementation struct Input_Edge_Delay_Implementation : public Polaris_Component<MasterType,INHERIT(Input_Edge_Delay_Implementation)>
//{
//	void Copy(Input_Edge_Delay_Implementation& copy)
//	{
//		copy._id = _id;
//
//		for(std::vector<long long>::iterator itr = _forward_edge_ids.begin();itr!=_forward_edge_ids.end();itr++)
//		{
//			copy._forward_edge_ids.push_back( *itr );
//		}
//
//		for(std::vector<float>::iterator itr = _forward_edge_delays.begin();itr!=_forward_edge_delays.end();itr++)
//		{
//			copy._forward_edge_delays.push_back( *itr );
//		}
//
//		for(std::vector<long long>::iterator itr = _backward_edge_ids.begin();itr!=_backward_edge_ids.end();itr++)
//		{
//			copy._backward_edge_ids.push_back( *itr );
//		}
//
//		for(std::vector<float>::iterator itr = _backward_edge_delays.begin();itr!=_backward_edge_delays.end();itr++)
//		{
//			copy._backward_edge_delays.push_back( *itr );
//		}
//
//		copy._pos_x = _pos_x;
//		copy._pos_y = _pos_y;
//
//		copy._cost = _cost;
//	}
//
//	m_data(long long,id,NONE,NONE);
//
//	m_container(std::vector<long long>,forward_edge_ids,NONE,NONE);
//	m_container(std::vector<float>,forward_edge_delays,NONE,NONE);
//
//	m_container(std::vector<long long>,backward_edge_ids,NONE,NONE);
//	m_container(std::vector<float>,backward_edge_delays,NONE,NONE);
//
//	m_data(float,pos_x,NONE,NONE);
//	m_data(float,pos_y,NONE,NONE);
//
//	m_data(float,cost,NONE,NONE);
//};
//
//implementation struct Input_Edge_Node_Implementation : public Polaris_Component<MasterType,INHERIT(Input_Edge_Implementation)>
//{
//	m_data(long long,id,NONE,NONE);
//
//	m_data(unsigned int,in_node_id,NONE,NONE);
//	m_data(unsigned int,out_node_id,NONE,NONE);
//
//	m_data(float,cost,NONE,NONE);
//};
//
//implementation struct Input_Node_Implementation : public Polaris_Component<MasterType,INHERIT(Input_Edge_Implementation)>
//{
//	m_data(long long,id,NONE,NONE);
//
//	m_data(float,pos_x,NONE,NONE);
//	m_data(float,pos_y,NONE,NONE);
//};
//
//struct Turn_Delay_Neighbor
//{
//	t_data(float,cost);
//
//	float time_cost(){ return _cost; }
//	void time_cost(float value){ _cost = value; }
//};
//
//struct A_Star_Attributes : public set_base_hook<optimize_size<false>>
//{
//	void reset()
//	{
//		_in_closed_set = false;
//		_marked_for_reset = false;
//		_in_open_set = false;
//
//		_cost_from_origin = 0.0f;
//		_cost_origin_destination = 0.0f;
//	}
//	
//	template<typename Input_Edge_Type,typename Input_Node_Type,typename A_Star_Edge_Type>
//	static void construct_edge_node(Input_Edge_Type* input_edge, Input_Node_Type* in_node, Input_Node_Type* out_node, A_Star_Edge_Type* attributes_ptr)
//	{
//		attributes_ptr->_pos_x = (in_node->_pos_x + out_node->_pos_x)/2.0f;
//		attributes_ptr->_pos_y = (in_node->_pos_y + out_node->_pos_y)/2.0f;
//
//		attributes_ptr->_came_from = nullptr;
//	
//		attributes_ptr->_in_closed_set = false;
//		attributes_ptr->_in_open_set = false;
//		attributes_ptr->_marked_for_reset = false;
//
//		attributes_ptr->_cost_origin_destination = 0.0f;
//		attributes_ptr->_cost_from_origin = 0.0f;
//
//		attributes_ptr->_cost = input_edge->_cost;
//	}
//
//	template<typename Input_Edge_Type,typename A_Star_Edge_Type>
//	static void construct_connected_edge(Input_Edge_Type* input_edge, A_Star_Edge_Type* attributes_ptr)
//	{
//		
//		attributes_ptr->_pos_x = input_edge->_pos_x;
//		attributes_ptr->_pos_y = input_edge->_pos_y;
//
//		attributes_ptr->_came_from = nullptr;
//		
//		attributes_ptr->_in_closed_set = false;
//		attributes_ptr->_in_open_set = false;
//		attributes_ptr->_marked_for_reset = false;
//
//		attributes_ptr->_cost_origin_destination = 0.0f;
//		attributes_ptr->_cost_from_origin = 0.0f;
//
//		A_Star_Edge<A_Star_Edge_Type>* edge = (A_Star_Edge<A_Star_Edge_Type>*)attributes_ptr;
//
//		//cout << "Constructing: " << edge->id() << "," << edge->num_forward_edges() << endl;
//
//		A_Star_Neighbor<A_Star_Edge<A_Star_Edge_Type>::neighbor_type>* current_neighbor = edge->forward_edges();
//		const A_Star_Neighbor<A_Star_Edge<A_Star_Edge_Type>::neighbor_type>* const end_neighbor = edge->end_forward_edges();
//		
//		std::vector<float>::iterator itr = input_edge->_forward_edge_delays.begin();
//
//		while(current_neighbor != end_neighbor)
//		{
//			current_neighbor->cost(*itr);
//
//			++itr;
//
//			current_neighbor = current_neighbor->next();
//		}
//
//		attributes_ptr->_cost = input_edge->_cost;
//
//		//attributes_ptr->_cost = 1.0f;
//	}
//
//	t_data(bool,in_closed_set);
//	t_data(bool,in_open_set);
//	t_data(bool,marked_for_reset);
//
//	float time_from_origin(){ return _cost_from_origin; }
//	void time_from_origin(float value){ _cost_from_origin = value; }
//	
//	//t_data(float,time_from_origin);
//
//	float time_cost(){ return _cost; }
//
//	t_data(float,cost_origin_destination);
//	t_data(float,cost_from_origin);
//
//	float cost_between(void* other)
//	{
//		A_Star_Attributes* _other = (A_Star_Attributes*)other;
//		return sqrt((_other->pos_x()-this->pos_x())*(_other->pos_x()-this->pos_x()) + (_other->pos_y()-this->pos_y())*(_other->pos_y()-this->pos_y()))/36.0f;
//	}
//
//	t_data(float,cost);
//
//	t_data(float,pos_x);
//	t_data(float,pos_y);
//
//	t_pointer(void*,came_from);
//
//	friend __forceinline bool operator< (const A_Star_Attributes &lhs,const A_Star_Attributes &rhs)
//	{  return lhs._cost_origin_destination < rhs._cost_origin_destination;  }
//	friend __forceinline bool operator> (const A_Star_Attributes &lhs,const A_Star_Attributes &rhs)
//	{  return lhs._cost_origin_destination > rhs._cost_origin_destination;  }
//	friend __forceinline bool operator== (const A_Star_Attributes &lhs,const A_Star_Attributes &rhs)
//	{  return lhs._cost_origin_destination == rhs._cost_origin_destination;  }
//};
//
//struct Dijkstra_Attributes : public set_base_hook< optimize_size<false> >
//{
//	void reset()
//	{
//		_in_closed_set = false;
//		_marked_for_reset = false;
//		_cost_from_origin = FLT_MAX;
//	}
//
//	template<typename Input_Edge_Type,typename Dijkstra_Edge_Type>
//	static void construct(Input_Edge_Type input_edge, Dijkstra_Edge_Type attributes_ptr)
//	{
//		attributes_ptr->_came_from = nullptr;
//	
//		attributes_ptr->_in_closed_set = false;
//		attributes_ptr->_marked_for_reset = false;
//
//		attributes_ptr->_cost_from_origin = FLT_MAX;
//
//		attributes_ptr->_cost = 1.0f;
//	}
//
//	t_data(bool,in_closed_set);
//	t_data(bool,marked_for_reset);
//
//	t_data(float,cost_from_origin);
//
//	t_data(float,cost);
//
//	t_pointer(void*,came_from);
//	
//	friend __forceinline bool operator< (const Dijkstra_Attributes &lhs,const Dijkstra_Attributes &rhs)
//	{  return lhs._cost_from_origin < rhs._cost_from_origin;  }
//	friend __forceinline bool operator> (const Dijkstra_Attributes &lhs,const Dijkstra_Attributes &rhs)
//	{  return lhs._cost_from_origin > rhs._cost_from_origin;  }
//	friend __forceinline bool operator== (const Dijkstra_Attributes &lhs,const Dijkstra_Attributes &rhs)
//	{  return lhs._cost_from_origin == rhs._cost_from_origin;  }
//};
//
//struct MasterType
//{
//	//typedef Packed_Data_Set_Implementation<MasterType> packed_data_set_type;
//
//	//typedef Input_Edge_Implementation<MasterType> input_edge_type;
//	
//	typedef Input_Edge_Delay_Implementation<MasterType> input_edge_type;
//
//	//typedef Input_Edge_Node_Implementation<MasterType> input_edge_node_type;
//	//typedef Input_Node_Implementation<MasterType> input_node_type;
//
//	typedef Packed_Edge_Implementation<A_Star_Attributes,Turn_Delay_Neighbor> a_star_edge_type;
//	typedef Packed_Graph_Implementation<MasterType,NULLTYPELIST,a_star_edge_type> a_star_graph_type;
//	typedef Packed_Graph_Implementation<MasterType,NULLTYPELIST,a_star_edge_type> a_star_graph_assembler_type;
//
//	//typedef Packed_Edge_Implementation<Dijkstra_Attributes> dijkstra_edge_type;
//	//typedef Packed_Graph_Implementation<MasterType,NULLTYPELIST,dijkstra_edge_type> dijkstra_graph_type;
//	//typedef Packed_Graph_Implementation<MasterType,NULLTYPELIST,dijkstra_edge_type> dijkstra_graph_assembler_type;
//};
//
//
//template<typename GraphType>
//void Read_Graph(Graph_Assembler_Connected_Edge<GraphType>* graph)
//{
//	ifstream network_file("Network.txt",ifstream::binary);
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
//	MasterType::input_edge_type* input;
//
//	while(file_itr != eof)
//	{
//		if(*file_itr == ';')
//		{
//			input = new MasterType::input_edge_type();
//			
//			input->_id = stoi(token);
//			
//			//input->_cost = 1;
//			token.clear();
//		}
//		else if(*file_itr == ':')
//		{
//			input->_pos_x = stof(token);
//
//			token.clear();
//		}
//		else if(*file_itr == '=')
//		{
//			input->_pos_y = stof(token);
//
//			token.clear();
//		}
//		else if(*file_itr == ',' || *file_itr == '\n')
//		{
//			input->_forward_edge_ids.push_back(stoi(token));
//			token.clear();
//
//			if(*file_itr == '\n')
//			{
//				graph->Add_Edge<MasterType::input_edge_type>((Input_Connected_Edge<MasterType::input_edge_type>*)input);
//			}
//		}
//		else
//		{
//			token.push_back(*file_itr);
//		}
//
//		++file_itr;
//	}
//
//	delete[] file;
//}
//
//template<typename GraphType,typename Input_Edge_Type>
//void Read_Edges(Graph_Assembler_Edge_Node<GraphType>* graph)
//{
//	ifstream network_file("Edges.txt",ifstream::binary);
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
//	Input_Edge_Type* input;
//
//	while(file_itr != eof)
//	{
//		if(*file_itr == ':')
//		{
//			input = new Input_Edge_Type();
//			
//			input->_id = stoi(token);
//
//			token.clear();
//		}
//		else if(*file_itr == ',')
//		{
//			input->_in_node_id = stoi(token);
//			token.clear();
//		}
//		else if(*file_itr == '\n')
//		{
//			input->_out_node_id = stoi(token);
//			token.clear();
//			
//			graph->Add_Edge<Input_Edge_Type>((Input_Edge<Input_Edge_Type>*)input);
//		}
//		else
//		{
//			token.push_back(*file_itr);
//		}
//
//		++file_itr;
//	}
//
//	delete[] file;
//}
//
//template<typename GraphType,typename Input_Node_Type>
//void Read_Nodes(Graph_Assembler_Edge_Node<GraphType>* graph)
//{
//	ifstream network_file("Nodes.txt",ifstream::binary);
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
//	Input_Node_Type* input;
//
//	while(file_itr != eof)
//	{
//		if(*file_itr == ':')
//		{
//			input = new Input_Node_Type();
//			
//			input->_id = stoi(token);
//
//			token.clear();
//		}
//		else if(*file_itr == ',')
//		{
//			input->_pos_x = (float)stoi(token);
//			token.clear();
//		}
//		else if(*file_itr == '\n')
//		{
//			input->_pos_y = (float)stoi(token);
//			token.clear();
//			
//			graph->Add_Node<Input_Node_Type>((Input_Node<Input_Node_Type>*)input);
//		}
//		else
//		{
//			token.push_back(*file_itr);
//		}
//
//		++file_itr;
//	}
//
//	delete[] file;
//}
//
//template<typename GraphType,typename Input_Edge_Type,typename Input_Node_Type>
//void Read_Edge_Node_Graph(Graph_Assembler_Edge_Node<GraphType>* graph)
//{
//	Read_Edges<GraphType,Input_Edge_Type>(graph);
//	Read_Nodes<GraphType,Input_Node_Type>(graph);
//}
//
//static void update_function(MasterType::a_star_edge_type* edge)
//{
//	cout << edge->id() << endl;
//}
//
////struct Delay
////{
////	float delay;
////	string name;
////};
//
//struct stop_time
//{
//	int arrival_time;
//	int departure_time;
//	int stop_id;
//	int trip_index;
//};
//
//struct stop_data
//{
//	float latitude;
//	float longitude;
//
//	std::vector<stop_time*> outbound_trip_legs;
//};
//
//static std::unordered_map<unsigned int, long long> trip_index_uuid_map;
//static std::unordered_map<long long, unsigned int> trip_uuid_index_map;
//
//static std::unordered_map<long long, std::vector<stop_time*>> stop_times_map;
//
//static std::unordered_set<long long> valid_services;
//
//static std::unordered_map<long long, stop_data> stop_data_map;
//
//void Read_Calendar(string&& filename)
//{
//	ifstream network_file(filename.c_str(),ifstream::binary);
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
//	long long service_id;
//
//	bool skip_line = true;
//
//	while(file_itr != eof)
//	{
//		if(!skip_line)
//		{
//			if(*file_itr == ',')
//			{
//				if(field_counter == 0)
//				{
//					service_id = stoll(token);
//				}
//				else if(field_counter == 3)
//				{
//					int has_wednesday = stoi(token);
//
//					if(has_wednesday) 
//					{
//						valid_services.insert(service_id);
//					}
//
//					skip_line = true;
//				}
//
//				token.clear();
//
//				++field_counter;
//			}
//			else if(*file_itr == '\n')
//			{
//				field_counter = 0;
//				token.clear();
//			}
//			else
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
//void Read_Trips(string&& filename)
//{
//	ifstream network_file(filename.c_str(),ifstream::binary);
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
//	unsigned int trip_index_counter = 0;
//
//	while(file_itr != eof)
//	{
//		if(!skip_line)
//		{
//			if(*file_itr == ',')
//			{
//				if(field_counter == 1)
//				{
//					long long service_id = stoll(token);
//
//					if(!valid_services.count(service_id)) skip_line = true;
//				}
//				else if(field_counter == 2)
//				{
//					long long trip_id = stoll(token);
//
//					stop_times_map[trip_id];
//
//					// trip ids are too long for creating an efficient 8 byte composite key, index them from 0 and create a mapping
//					trip_index_uuid_map[trip_index_counter] = trip_id;
//					trip_uuid_index_map[trip_id] = trip_index_counter;
//
//					++trip_index_counter;
//					
//					skip_line = true;
//				}
//
//				token.clear();
//
//				++field_counter;
//			}
//			else if(*file_itr == '\n')
//			{
//				field_counter = 0;
//				token.clear();
//			}
//			else
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
//void Read_Stops(string&& filename)
//{
//	ifstream network_file(filename.c_str(),ifstream::binary);
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
//	while(file_itr != eof)
//	{
//		if(!skip_line)
//		{
//			if(*file_itr == '\"')
//			{
//				if(!in_quote)
//				{
//					//cout << "Entering Quote" << endl;
//					in_quote = true;
//				}
//				else
//				{
//					//cout << "Leaving Quote" << endl;
//					in_quote = false;
//				}
//			}
//			else if(*file_itr == ',' && !in_quote)
//			{
//				if(field_counter == 0)
//				{
//					long long stop_id = stoll(token);
//					
//					//cout << "Looking at stop_id " << stop_id << endl;
//
//					current_stop = &(stop_data_map[stop_id]);
//				}
//				else if(field_counter == 4)
//				{
//					//cout << "Looking at lat " << current_stop->latitude << endl;
//
//					current_stop->latitude = stof(token);
//				}
//				else if(field_counter == 5)
//				{
//					//cout << "Looking at long " << current_stop->longitude << endl;
//
//					current_stop->longitude = stof(token);
//
//					skip_line = true;
//				}
//
//				token.clear();
//
//				++field_counter;
//			}
//			else if(*file_itr == '\n')
//			{
//				//cout << current_stop->latitude << "," << current_stop->longitude << endl;
//
//				field_counter = 0;
//				token.clear();
//			}
//			else
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
//void Read_Stop_Times(string&& filename)
//{
//	ifstream network_file(filename.c_str(),ifstream::binary);
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
//	unsigned int sub_field_counter = 0;
//
//	stop_time* current_stop_time;
//
//	unsigned int hours;
//	unsigned int minutes;
//	unsigned int seconds;
//
//	bool skip_line = true;
//
//	unsigned int skipped = 0;
//	
//	std::vector<stop_time*>* previous_stop = nullptr;
//
//	unsigned int counter = 0;
//
//	while(file_itr != eof)
//	{
//		if(!skip_line)
//		{
//			if(*file_itr == ',')
//			{
//				if(field_counter == 0)
//				{
//					long long id = stoll(token);
//
//					if(!stop_times_map.count(id))
//					{
//						skip_line = true;
//						++skipped;
//					}
//					else
//					{
//						if(counter%100000 == 0){cout << '\t' << counter << endl;}
//						
//						++counter;
//
//						current_stop_time = new stop_time();
//						stop_times_map[id].push_back( current_stop_time );
//						current_stop_time->trip_index = trip_uuid_index_map[id];
//
//						if(previous_stop != nullptr) 
//						{
//							previous_stop->push_back( current_stop_time );
//						}
//					}
//				}
//				else if(field_counter == 1)
//				{
//					seconds = stoi(token);
//
//					int time = seconds + minutes*60 + hours*60*60;
//
//					//if( trip_index_uuid_map[current_stop_time->trip_index] == 42060791492 ) cout << "Arrival: " << hours << "," << minutes << "," << seconds << endl;
//
//					current_stop_time->arrival_time = time;
//				}
//				else if(field_counter == 2)
//				{
//					seconds = stoi(token);
//
//					int time = seconds + minutes*60 + hours*60*60;
//
//					//if( trip_index_uuid_map[current_stop_time->trip_index] == 42060791492 ) cout << "Departure: " << hours << "," << minutes << "," << seconds << endl;
//
//					current_stop_time->departure_time = time;
//				}
//				else if(field_counter == 3)
//				{
//					int stop_id = stoi(token);
//
//					current_stop_time->stop_id = stop_id;
//
//					previous_stop = &stop_data_map[current_stop_time->stop_id].outbound_trip_legs;
//
//					skip_line = true;
//				}
//
//				token.clear();
//
//				++field_counter;
//			}
//			else if(*file_itr == ':')
//			{
//				if(sub_field_counter == 0)
//				{
//					hours = stoi(token);
//					++sub_field_counter;
//				}
//				else if(sub_field_counter == 1)
//				{
//					minutes = stoi(token);
//					sub_field_counter = 0;
//				}
//
//				token.clear();
//			}
//			else if(*file_itr == '\n')
//			{
//				field_counter = 0;
//				token.clear();
//			}
//			else
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
//	cout << "Trips Skipped Due to Wednesday Service: " << skipped << endl;
//
//	delete[] file;
//}
//
//union transit_edge_id
//{
//	struct composite_id
//	{
//		unsigned int time;
//		unsigned int trip_index;
//	} composite_id;
//
//	long long id;
//};
//
///* Add input by connected node to graph */
//
//template<typename GraphType>
//void Build_Graph(Graph_Assembler_Connected_Edge<GraphType>* input_graph, unsigned int horizon_seconds)
//{
//	typedef MasterType::input_edge_type input_edge_type;
//
//	input_edge_type current_edge_stack;
//
//	transit_edge_id transit_id;
//
//	input_edge_type* current_edge = &current_edge_stack;
//
//	// loop over trips
//
//	int arrival_time_to_current = -1;
//	int arrival_time_to_last = -1;
//
//	int departure_time_from_current = -1;
//	int departure_time_from_last = -1;
//
//	//bool run_once = false;
//
//	unsigned int counter = 0;
//
//	for(std::unordered_map<long long,std::vector<stop_time*>>::iterator itr = stop_times_map.begin(); itr != stop_times_map.end(); itr++)
//	{
//		//if(run_once) break;
//
//		// loop over the individual stops
//
//		if(counter%1000 == 0){cout << '\t' << counter << endl;}
//
//		++counter;
//
//		for(std::vector<stop_time*>::iterator stop_itr = itr->second.begin(); stop_itr != itr->second.end(); stop_itr++)
//		{
//			std::vector<stop_time*>* outbound_legs = &(stop_data_map[(*stop_itr)->stop_id].outbound_trip_legs);
//
//			arrival_time_to_current = (*stop_itr)->arrival_time;
//			departure_time_from_current = (*stop_itr)->departure_time;
//
//			//cout << "\tVisiting stop: " << (*stop_itr)->stop_id << ", arriving: " << arrival_time_to_current << ", departing: " << departure_time_from_current << endl;
//
//			if(arrival_time_to_last != -1)
//			{
//				// we are on some subsequent stop, the current edge is the one which came from the previous stop to here
//
//				// the cost for this edge is the arrival time here - departure time from previous
//
//				current_edge->_cost = (float)(arrival_time_to_current - departure_time_from_last);
//
//				//cout << "\tWeight: " << current_edge->_cost << endl;
//
//				// next we need to create our "turn penalties" and "incident edges" from the set of stops which depart from this one
//
//				for(std::vector<stop_time*>::iterator outbound_leg_itr = outbound_legs->begin(); outbound_leg_itr != outbound_legs->end(); outbound_leg_itr++)
//				{
//					// the "turn penalty" for this edge is the departure time from this stop - the arrival time to this stop
//					int transfer_time = (*outbound_leg_itr)->departure_time - arrival_time_to_current;
//
//					//cout << "\tVisiting outbound stop: " << (*outbound_leg_itr)->stop_id << ", trip: " << trip_index_uuid_map[(*outbound_leg_itr)->trip_index] << endl;
//					//cout << "\t\tArrival time: " << arrival_time_to_current << ", departing: " << (*outbound_leg_itr)->departure_time << endl;
//					//cout << "\t\tTransfer time: " << transfer_time << endl;
//
//					// trim routes at times later than horizon_seconds or before the current time (negative)
//					if( transfer_time > horizon_seconds || transfer_time < 0) continue;
//
//					current_edge->_forward_edge_delays.push_back( transfer_time );
//
//					transit_edge_id outbound_id;
//					
//					// this edge will be identified by its' departure time and trip index
//					outbound_id.composite_id.time = (*outbound_leg_itr)->departure_time;
//					outbound_id.composite_id.trip_index = (*outbound_leg_itr)->trip_index;
//
//					current_edge->_forward_edge_ids.push_back( outbound_id.id );
//				}
//
//				// we are ready to add this edge to the graph
//
//				//cout << "Adding Edge: " << current_edge->_id << endl;
//
//				input_graph->Add_Edge((Input_Connected_Edge<input_edge_type>*)current_edge);
//
//				current_edge->_forward_edge_ids.clear();
//				current_edge->_forward_edge_delays.clear();
//			}
//
//			// this edge will be identified by its' departure time and trip index
//
//			//cout << "Visiting Edge: " << trip_index_uuid_map[(*stop_itr)->trip_index] << "," << (*stop_itr)->departure_time << endl;
//
//			transit_id.composite_id.trip_index = (*stop_itr)->trip_index;
//			transit_id.composite_id.time = (*stop_itr)->departure_time;
//
//			current_edge->_id = transit_id.id;
//
//			arrival_time_to_last = arrival_time_to_current;
//			departure_time_from_last = departure_time_from_current;
//		}
//
//		// add the end of the line which is not connected to anything
//		input_graph->Add_Edge((Input_Connected_Edge<input_edge_type>*)current_edge);
//
//		arrival_time_to_current = -1;
//		arrival_time_to_last = -1;
//
//		departure_time_from_current = -1;
//		departure_time_from_last = -1;
//
//		//run_once = true;
//	}
//}
//
//void Clean_Up()
//{
//
//}
//
//int main()
//{
//	//EPSG:4326 -t_srs EPSG:26916
//	FILE* output = _popen("ECHO -87.77410482 41.87632184 | gdaltransform.exe -s_srs EPSG:4326 -t_srs EPSG:26916","rb");
//
//	int c;
//
//	do 
//	{
//		c = fgetc (output);
//		cout << (char)c << endl;
//	} while (c != EOF);
//
//	//MasterType::a_star_graph_type a_star_graph_stack;
//	//
//	//Graph_Assembler_Connected_Edge<MasterType::a_star_graph_assembler_type>* a_star_graph_assembler = (Graph_Assembler_Connected_Edge<MasterType::a_star_graph_assembler_type>*)&a_star_graph_stack;
//
//	//Read_Calendar(string("calendar.txt"));
//
//	//cout << "Calendar Read" << endl;
//	//
//	//Read_Trips(string("trips.txt"));
//
//	//cout << "Trips Read" << endl;
//
//	//Read_Stops(string("stops.txt"));
//	//
//	//cout << "Stops Read" << endl;
//
//	//Read_Stop_Times(string("stop_times.txt"));
//	//
//	//cout << "Stop Times Read" << endl;
//	//
//	//Build_Graph(a_star_graph_assembler,30*60);
//	//
//	//cout << "Graph Built" << endl;
//	//
//	//stop_times_map.~unordered_map();
//	//stop_data_map.~unordered_map();
//	//valid_services.~unordered_set();
//
//	//Interactive_Graph<MasterType::a_star_graph_type>* a_star_graph = a_star_graph_assembler->Compile_Graph<MasterType::input_edge_type>( &A_Star_Attributes::construct_connected_edge );
//
//	//cout << "Graph Compiled: " << a_star_graph->edges()->size() << endl;
//
//
//
//	//transit_edge_id origin;
//
//	//origin.composite_id.time = 14*60*60+57*60;
//	//origin.composite_id.trip_index = trip_uuid_index_map[42060791492];
//
//	//transit_edge_id destination;
//
//	//destination.composite_id.time = 16*60*60+2*60;
//	//destination.composite_id.trip_index = trip_uuid_index_map[42060791492];
//
//	//std::deque<long long> out_path;
//
//
//	//A_Star_Edge<typename MasterType::a_star_edge_type>* end = A_Star((A_Star_Graph<typename MasterType::a_star_graph_type>*)a_star_graph,origin.id,destination.id,out_path);
//
//	//cout << "Done Routing!" << endl;
//
//	//transit_edge_id current_edge;
//
//	//for(std::deque<long long>::iterator itr = out_path.begin();itr!=out_path.end();itr++)
//	//{
//	//	current_edge.id = *itr;
//
//	//	cout << current_edge.composite_id.time << "," << trip_index_uuid_map[current_edge.composite_id.trip_index] << endl;
//	//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//	//for(std::unordered_map<long long,std::vector<stop_time>>::iterator itr = stop_times_map.begin();itr!=stop_times_map.end();itr++)
//	//{
//	//	cout << itr->first << endl;
//
//	//	for(std::vector<stop_time>::iterator record_itr = itr->second.begin();record_itr != itr->second.end();record_itr++)
//	//	{
//	//		cout << "\t" << record_itr->arrival_time << "," << record_itr->departure_time << "," << record_itr->stop_id << endl;
//	//	}
//	//}
//	//
//	//cout << stop_times_map.size() << endl;
//
//	//return 0;
//
//	//MasterType::packed_data_set_type packed_data_set_stack;
//
//
//	//Packed_Data_Set_Assembler< MasterType::packed_data_set_type >* assembler = (Packed_Data_Set_Assembler< MasterType::packed_data_set_type >*)&packed_data_set_stack;
//
//	//Delay* ed = new Delay();
//
//	//ed->delay = 5.0f;
//	//ed->name = "Ed";
//
//	//assembler->Add_New_Data_Record(3,ed);
//
//	//ed->name = "Ted";
//	
//	//assembler->Add_New_Data_Record(5,ed);
//
//	//Packed_Data_Set< MasterType::packed_data_set_type >* packed_data_set = assembler->Compile();
//
//	//cout << packed_data_set->Get<Delay>(3)->begin()->name << endl;
//
//	//return 0;
//
//
//
//	//MasterType::a_star_graph_type a_star_graph_stack;
//	
//	//Graph_Assembler_Edge_Node<MasterType::a_star_graph_assembler_type>* a_star_graph_assembler = (Graph_Assembler_Edge_Node<MasterType::a_star_graph_assembler_type>*)&a_star_graph_stack;
//	
//	//Read_Edge_Node_Graph<MasterType::a_star_graph_assembler_type,MasterType::input_edge_node_type,MasterType::input_node_type>(a_star_graph_assembler);
//
//	//Interactive_Graph<MasterType::a_star_graph_type>* a_star_graph = a_star_graph_assembler->Compile_Graph<MasterType::input_edge_node_type,MasterType::input_node_type>( &A_Star_Attributes::construct_edge_node );
//	
//	
//	//MasterType::a_star_graph_type a_star_graph_stack;
//
//	//Graph_Assembler_Edge_Node<MasterType::a_star_graph_assembler_type>* a_star_graph_assembler = (Graph_Assembler_Edge_Node<MasterType::a_star_graph_assembler_type>*)&a_star_graph_stack;
//
//	//
//	//using namespace odb;
//	//using namespace polaris::io;
//
//	//string name("C:\\Users\\hope\\Desktop\\POLARIS_v03\\Test_Data\\Chicago_City\\chicago");
//	//
//	//unique_ptr<database> db (open_sqlite_database (name));
//	//transaction t(db->begin());
//	//
//	//result<Node> node_result=db->template query<Node>(query<Node>::true_expr);
//
//	//for(result<Node>::iterator db_itr = node_result.begin (); db_itr != node_result.end (); ++db_itr)
//	//{
//	//	MasterType::input_node_type* node = new MasterType::input_node_type();
//
//	//	node->pos_x( db_itr->getX() );
//	//	node->pos_y( db_itr->getY() );
//
//	//	node->id( db_itr->getNode() );
//
//	//	a_star_graph_assembler->Add_Node((Input_Node<MasterType::input_node_type>*)node);
//	//}
//
//	//result<Link> link_result=db->template query<Link>(query<Link>::true_expr);
//
//	//for(result<Link>::iterator db_itr = link_result.begin (); db_itr != link_result.end (); ++db_itr)
//	//{
//	//	if(db_itr->getLanes_Ab() != 0)
//	//	{
//	//		MasterType::input_edge_node_type* input_edge = new MasterType::input_edge_node_type();
//
//	//		input_edge->in_node_id( db_itr->getNode_A()->getNode() );
//	//		input_edge->out_node_id( db_itr->getNode_B()->getNode() );
//
//	//		input_edge->id( db_itr->getLink()*2 );
//
//	//		input_edge->cost( db_itr->getLength() / db_itr->getFspd_Ab() );
//	//		a_star_graph_assembler->Add_Edge((Input_Edge<MasterType::input_edge_node_type>*)input_edge);
//	//	}
//
//	//	if(db_itr->getLanes_Ba() != 0)
//	//	{
//	//		MasterType::input_edge_node_type* input_edge = new MasterType::input_edge_node_type();
//
//	//		input_edge->in_node_id( db_itr->getNode_B()->getNode() );
//	//		input_edge->out_node_id( db_itr->getNode_A()->getNode() );
//
//	//		input_edge->id( db_itr->getLink()*2 + 1 );
//	//		input_edge->cost( db_itr->getLength() / db_itr->getFspd_Ba() );
//
//	//		a_star_graph_assembler->Add_Edge((Input_Edge<MasterType::input_edge_node_type>*)input_edge);
//	//	}
//	//}
//	//
//	//Interactive_Graph<MasterType::a_star_graph_type>* a_star_graph = a_star_graph_assembler->Compile_Graph<MasterType::input_edge_node_type,MasterType::input_node_type>( &A_Star_Attributes::construct_edge_node );
//	//
//	//MasterType::a_star_graph_type a_star_graph_stack_copy;
//
//	//LARGE_INTEGER start_timer;
//	//LARGE_INTEGER end_timer;
//	//LARGE_INTEGER frequency; QueryPerformanceFrequency(&frequency);
//
//
//
//	//QueryPerformanceCounter(&start_timer);
//	//a_star_graph->Copy_Graph((Interactive_Graph<MasterType::a_star_graph_type>*)&a_star_graph_stack_copy);
//	//QueryPerformanceCounter(&end_timer);
//
//	//cout << "Copy Time: " << ((float)end_timer.QuadPart - (float)start_timer.QuadPart)/((float)frequency.QuadPart) << endl;
//
//	//a_star_graph = (Interactive_Graph<MasterType::a_star_graph_type>*)&a_star_graph_stack_copy;
//
//	//std::deque<int> out_path;
//
//
//
//	//QueryPerformanceFrequency(&frequency);
//
//	//long long sum_time = 0;
//	//unsigned int success_counter = 0;
//	//unsigned int path_length_counter = 0;
//	//
//	//for(int i=0;i<1000000;i++)
//	//{
//	//	
//	//	
//	//	if(i%10000 == 0) cout << i << endl;
//
//	//	float ratio_through_origin = ((float)rand())/((float)RAND_MAX);
//	//	unsigned int origin_index = (unsigned int)(((float)a_star_graph_stack._edges.size() - 1.0f)*ratio_through_origin);
//	//	int origin = a_star_graph_stack._edges[origin_index]->id();
//
//	//	float ratio_through_destination = ((float)rand())/((float)RAND_MAX);
//	//	unsigned int destination_index = (unsigned int)(((float)a_star_graph_stack._edges.size() - 1.0f)*ratio_through_destination);
//	//	int destination = a_star_graph_stack._edges[destination_index]->id();
//
//	//	QueryPerformanceCounter(&start_timer);
//	//	A_Star_Edge<typename MasterType::a_star_edge_type>* end = A_Star((A_Star_Graph<typename MasterType::a_star_graph_type>*)a_star_graph,origin,destination,out_path);
//	//	QueryPerformanceCounter(&end_timer);
//
//	//	
//
//	//	if(end != nullptr){ sum_time += (end_timer.QuadPart - start_timer.QuadPart); ++success_counter; path_length_counter+=out_path.size(); }
//
//	//	out_path.clear();
//
//	//}
//
//	//cout << "Average Routing Time: " << ((float)sum_time/(float)success_counter)/((float)frequency.QuadPart) << endl;
//	//cout << "Successes: " << success_counter << endl;
//	//cout << "Average Path Length: " << ((float)path_length_counter)/((float)success_counter) << endl;
//
//
//
//
//
//	//Read_Graph(a_star_graph_assembler);
//
//	//Interactive_Graph<MasterType::a_star_graph_type>* a_star_graph = a_star_graph_assembler->Compile_Graph<MasterType::input_edge_type>( &A_Star_Attributes::construct );
//
//	//
//	//MasterType::a_star_graph_type a_star_graph_stack_copy;
//	//Interactive_Graph<MasterType::a_star_graph_type>* a_star_graph_copy = (Interactive_Graph<MasterType::a_star_graph_type>*)&a_star_graph_stack_copy;
//
//	//a_star_graph->Copy_Graph(a_star_graph_copy);
//
//	//std::unordered_set<int> negative_edges;
//	//
//	//negative_edges.insert(4);
//
//	//MasterType::a_star_graph_type a_star_graph_stack_sub_graph;
//	//Interactive_Graph<MasterType::a_star_graph_type>* a_star_graph_sub_graph = (Interactive_Graph<MasterType::a_star_graph_type>*)&a_star_graph_stack_sub_graph;
//
//	//a_star_graph->Create_Sub_Graph_Negative(a_star_graph_sub_graph, negative_edges);
//
//	//Graph_Pool_Implementation<MasterType,NULLTYPELIST,MasterType::a_star_graph_type> graph_pool;
//
//	//graph_pool.Add_Graph(a_star_graph);
//	//graph_pool.Add_Graph(a_star_graph_copy);
//	//graph_pool.Add_Graph(a_star_graph_sub_graph);
//
//	//std::vector<int> edge_set;
//	//edge_set.push_back(1);
//	//edge_set.push_back(4);
//
//	//graph_pool.Update_Edges(&edge_set,nullptr,&update_function);
//
//
//
//
//
//
//
//	//std::vector<int> out_path;
//
//	//A_Star_Edge<typename MasterType::a_star_edge_type>* end = A_Star((A_Star_Graph<typename MasterType::a_star_graph_type>*)a_star_graph_sub_graph,1,12,out_path);
//
//	//for(std::vector<int>::iterator itr = out_path.begin();itr!=out_path.end();itr++) cout << *itr << endl;
//
//	//out_path.clear();
//
//
//
//	//MasterType::dijkstra_graph_type dijkstra_graph_stack;
//
//
//	//Graph_Assembler<MasterType::dijkstra_graph_assembler_type>* dijkstra_graph_assembler = (Graph_Assembler<MasterType::dijkstra_graph_assembler_type>*)&dijkstra_graph_stack;
//
//	//Read_Graph(dijkstra_graph_assembler);
//
//	//Interactive_Graph<MasterType::dijkstra_graph_type>* dijkstra_graph = dijkstra_graph_assembler->Compile_Graph<MasterType::input_edge_type>( &Dijkstra_Attributes::construct );
//
//	//Dijkstra_Edge<typename MasterType::dijkstra_edge_type>* d_end = Dijkstra((Dijkstra_Graph<typename MasterType::dijkstra_graph_type>*)dijkstra_graph,2,11,out_path);
//
//	//for(std::vector<int>::iterator itr = out_path.begin();itr!=out_path.end();itr++) cout << *itr << endl;
//
//	//cout << "Done!" << endl;
//
//
//	bool done=true;
//}