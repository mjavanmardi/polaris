#include "Core.h"
#include "Routing.h"

using namespace polaris;

implementation struct Input_Edge_Implementation : public Polaris_Component<MasterType,INHERIT(Input_Edge_Implementation)>
{
	m_data(int,id,NONE,NONE);

	m_container(boost::container::vector<int>,forward_edge_ids,NONE,NONE);
	m_container(boost::container::vector<int>,backward_edge_ids,NONE,NONE);

	m_data(float,pos_x,NONE,NONE);
	m_data(float,pos_y,NONE,NONE);
};

implementation struct Input_Edge_Node_Implementation : public Polaris_Component<MasterType,INHERIT(Input_Edge_Implementation)>
{
	m_data(int,id,NONE,NONE);

	m_data(unsigned int,in_node_id,NONE,NONE);
	m_data(unsigned int,out_node_id,NONE,NONE);
};

implementation struct Input_Node_Implementation : public Polaris_Component<MasterType,INHERIT(Input_Edge_Implementation)>
{
	m_data(int,id,NONE,NONE);

	m_data(float,pos_x,NONE,NONE);
	m_data(float,pos_y,NONE,NONE);
};

struct A_Star_Attributes : public set_base_hook<optimize_size<false>>
{
	void reset()
	{
		_in_closed_set = false;
		_marked_for_reset = false;
		_in_open_set = false;

		_g_score = FLT_MAX;
		_f_score = FLT_MAX;
	}
	
	template<typename Input_Edge_Type,typename Input_Node_Type,typename A_Star_Edge_Type>
	static void construct_edge_node(Input_Edge_Type input_edge, Input_Node_Type in_node, Input_Node_Type out_node, A_Star_Edge_Type attributes_ptr)
	{
		attributes_ptr->_pos_x = (in_node->_pos_x + out_node->_pos_x)/2.0f;
		attributes_ptr->_pos_y = (in_node->_pos_y + out_node->_pos_y)/2.0f;

		attributes_ptr->_came_from = nullptr;
	
		attributes_ptr->_in_closed_set = false;
		attributes_ptr->_in_open_set = false;
		attributes_ptr->_marked_for_reset = false;

		attributes_ptr->_f_score = FLT_MAX;
		attributes_ptr->_g_score = FLT_MAX;

		attributes_ptr->_weight = 1.0f;
	}

	template<typename Input_Edge_Type,typename A_Star_Edge_Type>
	static void construct(Input_Edge_Type input_edge, A_Star_Edge_Type attributes_ptr)
	{
		attributes_ptr->_pos_x = input_edge->_pos_x;
		attributes_ptr->_pos_y = input_edge->_pos_y;

		attributes_ptr->_came_from = nullptr;
	
		attributes_ptr->_in_closed_set = false;
		attributes_ptr->_in_open_set = false;
		attributes_ptr->_marked_for_reset = false;

		attributes_ptr->_f_score = FLT_MAX;
		attributes_ptr->_g_score = FLT_MAX;

		attributes_ptr->_weight = 1.0f;
	}

	t_data(bool,in_closed_set);
	t_data(bool,in_open_set);
	t_data(bool,marked_for_reset);

	t_data(float,f_score);
	t_data(float,g_score);

	float weight_between(void* other)
	{
		A_Star_Attributes* _other = (A_Star_Attributes*)other;
		return sqrt((_other->pos_x()-this->pos_x())*(_other->pos_x()-this->pos_x()) + (_other->pos_y()-this->pos_y())*(_other->pos_y()-this->pos_y()));
	}

	float weight(){ return _weight; }
	void weight(float value){ _weight = value; }
	float _weight;

	t_data(float,pos_x);
	t_data(float,pos_y);

	t_data(void*,came_from);

	friend __forceinline bool operator< (const A_Star_Attributes &lhs,const A_Star_Attributes &rhs)
	{  return lhs._f_score < rhs._f_score;  }
	friend __forceinline bool operator> (const A_Star_Attributes &lhs,const A_Star_Attributes &rhs)
	{  return lhs._f_score > rhs._f_score;  }
	friend __forceinline bool operator== (const A_Star_Attributes &lhs,const A_Star_Attributes &rhs)
	{  return lhs._f_score == rhs._f_score;  }
};





struct Dijkstra_Attributes : public set_base_hook< optimize_size<false> >
{
	void reset()
	{
		_in_closed_set = false;
		_marked_for_reset = false;
		_f_score = FLT_MAX;
	}

	template<typename Input_Edge_Type,typename Dijkstra_Edge_Type>
	static void construct(Input_Edge_Type input_edge, Dijkstra_Edge_Type attributes_ptr)
	{
		attributes_ptr->_came_from = nullptr;
	
		attributes_ptr->_in_closed_set = false;
		attributes_ptr->_marked_for_reset = false;

		attributes_ptr->_f_score = FLT_MAX;

		attributes_ptr->_weight = 1.0f;
	}

	t_data(bool,in_closed_set);
	t_data(bool,marked_for_reset);

	t_data(float,f_score);

	float weight(){ return _weight; }
	void weight(float value){ _weight = value; }
	float _weight;

	t_data(void*,came_from);
	
	friend __forceinline bool operator< (const Dijkstra_Attributes &lhs,const Dijkstra_Attributes &rhs)
	{  return lhs._f_score < rhs._f_score;  }
	friend __forceinline bool operator> (const Dijkstra_Attributes &lhs,const Dijkstra_Attributes &rhs)
	{  return lhs._f_score > rhs._f_score;  }
	friend __forceinline bool operator== (const Dijkstra_Attributes &lhs,const Dijkstra_Attributes &rhs)
	{  return lhs._f_score == rhs._f_score;  }
};

struct MasterType
{
	typedef Packed_Data_Set_Implementation<MasterType> packed_data_set_type;

	typedef Input_Edge_Implementation<MasterType> input_edge_type;
	
	typedef Input_Edge_Node_Implementation<MasterType> input_edge_node_type;
	typedef Input_Node_Implementation<MasterType> input_node_type;

	typedef Packed_Edge_Implementation<A_Star_Attributes,NT> a_star_edge_type;
	typedef Packed_Graph_Implementation<MasterType,NULLTYPELIST,a_star_edge_type> a_star_graph_type;
	typedef Packed_Graph_Implementation<MasterType,NULLTYPELIST,a_star_edge_type> a_star_graph_assembler_type;

	//typedef Packed_Edge_Implementation<Dijkstra_Attributes> dijkstra_edge_type;
	//typedef Packed_Graph_Implementation<MasterType,NULLTYPELIST,dijkstra_edge_type> dijkstra_graph_type;
	//typedef Packed_Graph_Implementation<MasterType,NULLTYPELIST,dijkstra_edge_type> dijkstra_graph_assembler_type;
};





template<typename GraphType>
void Read_Graph(Graph_Assembler_Edge<GraphType>* graph)
{
	ifstream network_file("Network.txt",ifstream::binary);

	network_file.seekg(0,ios::end);

	int length = (int)network_file.tellg();

	network_file.seekg(0,ios::beg);

	char* file = new char[length];
	
	network_file.read(file,length);

	const char* const eof = file + length;

	char* file_itr = file;

	string token;

	MasterType::input_edge_type* input;

	while(file_itr != eof)
	{
		if(*file_itr == ';')
		{
			input = new MasterType::input_edge_type();
			
			input->_id = stoi(token);
			
			//input->_weight = 1;
			token.clear();
		}
		else if(*file_itr == ':')
		{
			input->_pos_x = stof(token);

			token.clear();
		}
		else if(*file_itr == '=')
		{
			input->_pos_y = stof(token);

			token.clear();
		}
		else if(*file_itr == ',' || *file_itr == '\n')
		{
			input->_forward_edge_ids.push_back(stoi(token));
			token.clear();

			if(*file_itr == '\n')
			{
				graph->Add_Edge<MasterType::input_edge_type>((Input_Connected_Edge<MasterType::input_edge_type>*)input);
			}
		}
		else
		{
			token.push_back(*file_itr);
		}

		++file_itr;
	}

	delete[] file;
}

template<typename GraphType,typename Input_Edge_Type>
void Read_Edges(Graph_Assembler_Edge_Node<GraphType>* graph)
{
	ifstream network_file("Edges.txt",ifstream::binary);

	network_file.seekg(0,ios::end);

	int length = (int)network_file.tellg();

	network_file.seekg(0,ios::beg);

	char* file = new char[length];
	
	network_file.read(file,length);

	const char* const eof = file + length;

	char* file_itr = file;

	string token;

	Input_Edge_Type* input;

	while(file_itr != eof)
	{
		if(*file_itr == ':')
		{
			input = new Input_Edge_Type();
			
			input->_id = stoi(token);

			token.clear();
		}
		else if(*file_itr == ',')
		{
			input->_in_node_id = stoi(token);
			token.clear();
		}
		else if(*file_itr == '\n')
		{
			input->_out_node_id = stoi(token);
			token.clear();
			
			graph->Add_Edge<Input_Edge_Type>((Input_Edge<Input_Edge_Type>*)input);
		}
		else
		{
			token.push_back(*file_itr);
		}

		++file_itr;
	}

	delete[] file;
}

template<typename GraphType,typename Input_Node_Type>
void Read_Nodes(Graph_Assembler_Edge_Node<GraphType>* graph)
{
	ifstream network_file("Nodes.txt",ifstream::binary);

	network_file.seekg(0,ios::end);

	int length = (int)network_file.tellg();

	network_file.seekg(0,ios::beg);

	char* file = new char[length];
	
	network_file.read(file,length);

	const char* const eof = file + length;

	char* file_itr = file;

	string token;

	Input_Node_Type* input;

	while(file_itr != eof)
	{
		if(*file_itr == ':')
		{
			input = new Input_Node_Type();
			
			input->_id = stoi(token);

			token.clear();
		}
		else if(*file_itr == ',')
		{
			input->_pos_x = (float)stoi(token);
			token.clear();
		}
		else if(*file_itr == '\n')
		{
			input->_pos_y = (float)stoi(token);
			token.clear();
			
			graph->Add_Node<Input_Node_Type>((Input_Node<Input_Node_Type>*)input);
		}
		else
		{
			token.push_back(*file_itr);
		}

		++file_itr;
	}

	delete[] file;
}

template<typename GraphType,typename Input_Edge_Type,typename Input_Node_Type>
void Read_Edge_Node_Graph(Graph_Assembler_Edge_Node<GraphType>* graph)
{
	Read_Edges<GraphType,Input_Edge_Type>(graph);
	Read_Nodes<GraphType,Input_Node_Type>(graph);
}

static void update_function(MasterType::a_star_edge_type* edge)
{
	cout << edge->id() << endl;
}

struct Delay
{
	float delay;
	string name;
};

int main()
{
	MasterType::packed_data_set_type packed_data_set_stack;


	Packed_Data_Set_Assembler< MasterType::packed_data_set_type >* assembler = (Packed_Data_Set_Assembler< MasterType::packed_data_set_type >*)&packed_data_set_stack;

	Delay* ed = new Delay();

	ed->delay = 5.0f;
	ed->name = "Ed";

	assembler->Add_New_Data_Record(3,ed);

	ed->name = "Ted";
	
	assembler->Add_New_Data_Record(5,ed);

	Packed_Data_Set< MasterType::packed_data_set_type >* packed_data_set = assembler->Compile();

	cout << packed_data_set->Get<Delay>(3)->begin()->name << endl;

	return 0;



	MasterType::a_star_graph_type a_star_graph_stack;
	
	
	
	
	
	//Graph_Assembler_Edge_Node<MasterType::a_star_graph_assembler_type>* a_star_graph_assembler = (Graph_Assembler_Edge_Node<MasterType::a_star_graph_assembler_type>*)&a_star_graph_stack;
	//
	//Read_Edge_Node_Graph<MasterType::a_star_graph_assembler_type,MasterType::input_edge_node_type,MasterType::input_node_type>(a_star_graph_assembler);

	//Interactive_Graph<MasterType::a_star_graph_type>* a_star_graph = a_star_graph_assembler->Compile_Graph<MasterType::input_edge_node_type,MasterType::input_node_type>( &A_Star_Attributes::construct_edge_node );
	
	

	Graph_Assembler_Edge<MasterType::a_star_graph_assembler_type>* a_star_graph_assembler = (Graph_Assembler_Edge<MasterType::a_star_graph_assembler_type>*)&a_star_graph_stack;

	Read_Graph(a_star_graph_assembler);

	Interactive_Graph<MasterType::a_star_graph_type>* a_star_graph = a_star_graph_assembler->Compile_Graph<MasterType::input_edge_type>( &A_Star_Attributes::construct );

	
	MasterType::a_star_graph_type a_star_graph_stack_copy;
	Interactive_Graph<MasterType::a_star_graph_type>* a_star_graph_copy = (Interactive_Graph<MasterType::a_star_graph_type>*)&a_star_graph_stack_copy;

	a_star_graph->Copy_Graph(a_star_graph_copy);

	boost::unordered::unordered_set<int> negative_edges;
	
	negative_edges.insert(4);

	MasterType::a_star_graph_type a_star_graph_stack_sub_graph;
	Interactive_Graph<MasterType::a_star_graph_type>* a_star_graph_sub_graph = (Interactive_Graph<MasterType::a_star_graph_type>*)&a_star_graph_stack_sub_graph;

	a_star_graph->Create_Sub_Graph_Negative(a_star_graph_sub_graph, negative_edges);

	Graph_Pool_Implementation<MasterType,NULLTYPELIST,MasterType::a_star_graph_type> graph_pool;

	graph_pool.Add_Graph(a_star_graph);
	graph_pool.Add_Graph(a_star_graph_copy);
	graph_pool.Add_Graph(a_star_graph_sub_graph);

	boost::container::vector<int> edge_set;
	edge_set.push_back(1);
	edge_set.push_back(4);

	graph_pool.Update_Edges(&edge_set,nullptr,&update_function);

	//boost::container::vector<int> out_path;

	//A_Star_Edge<typename MasterType::a_star_edge_type>* end = A_Star((A_Star_Graph<typename MasterType::a_star_graph_type>*)a_star_graph_sub_graph,1,12,out_path);

	//for(boost::container::vector<int>::iterator itr = out_path.begin();itr!=out_path.end();itr++) cout << *itr << endl;

	//out_path.clear();



	//MasterType::dijkstra_graph_type dijkstra_graph_stack;


	//Graph_Assembler<MasterType::dijkstra_graph_assembler_type>* dijkstra_graph_assembler = (Graph_Assembler<MasterType::dijkstra_graph_assembler_type>*)&dijkstra_graph_stack;

	//Read_Graph(dijkstra_graph_assembler);

	//Interactive_Graph<MasterType::dijkstra_graph_type>* dijkstra_graph = dijkstra_graph_assembler->Compile_Graph<MasterType::input_edge_type>( &Dijkstra_Attributes::construct );

	//Dijkstra_Edge<typename MasterType::dijkstra_edge_type>* d_end = Dijkstra((Dijkstra_Graph<typename MasterType::dijkstra_graph_type>*)dijkstra_graph,2,11,out_path);

	//for(boost::container::vector<int>::iterator itr = out_path.begin();itr!=out_path.end();itr++) cout << *itr << endl;

	//cout << "Done!" << endl;


	bool done=true;
}