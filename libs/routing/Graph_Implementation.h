#pragma once
#include "Edge_Implementation.h"

namespace polaris
{
	using namespace boost::intrusive;

	template<typename MasterType, typename InheritanceList = NTL, typename Stored_Edge_Type = NT>
	struct Graph_Implementation : public Polaris_Component<MasterType, typename Append<InheritanceList, Graph_Implementation<MasterType,NTL,Stored_Edge_Type>>::Result>
	{
		Graph_Implementation():_graph_id(-1),_input_edge_reference(nullptr),_ordered_input_edge_reference(nullptr),_compiled(false),_graph(nullptr),_graph_size(0){}

		typedef Polaris_Component<MasterType, typename Append<InheritanceList, Graph_Implementation<MasterType,NTL,Stored_Edge_Type>>::Result> Base_t;

		typedef typename MasterType::graph_pool_type graph_pool_type;
		
		typedef Graph_Implementation output_graph_type;

		typedef Stored_Edge_Type stored_edge_type;
		typedef typename stored_edge_type::base_edge_type base_edge_type;
		typedef typename Base_t::ComponentType ComponentType;
		typedef typename Base_t::Master_Type Master_Type;

		t_data(p_graph_id_type,graph_id);
		t_data(bool,compiled);

		
		std::vector< base_edge_type* >* Get_Edges()
		{
			return (std::vector< base_edge_type* >*)_ordered_edge_reference;
		}


		template<typename Edge_Type>
		Edge<Edge_Type>* Get_Edge(p_edge_id_type edge_id)
		{
			if( _edge_reference->count(edge_id) )
			{
				return (Edge<Edge_Type>*)(*_edge_reference)[edge_id];
			}
			else
			{
				THROW_WARNING("Edge: " << edge_id << " not found in graph " << (int)_graph_id << endl);
				return nullptr;
			}
		}

		base_edge_type* Get_Edge(p_edge_id_type edge_id)
		{
			if( _edge_reference->count(edge_id) )
			{
				return (base_edge_type*)(*_edge_reference)[edge_id];
			}
			else
			{
				THROW_WARNING("Edge: " << edge_id << " not found in graph " << (int)_graph_id << endl);
				return nullptr;
			}
		}

		template<typename Edge_Attributes_Type>
		void Add_Edge( Input_Edge<Edge_Attributes_Type>* new_edge )
		{
			if(_input_edge_reference == nullptr) _input_edge_reference = new std::unordered_map<p_edge_id_type,void*>();
			if(_ordered_input_edge_reference == nullptr) _ordered_input_edge_reference = new std::vector<void*>();

			if(_input_edge_reference->count(new_edge->edge_id())){ THROW_EXCEPTION("Duplicate Edge Added: " << new_edge->edge_id()); }

			Input_Edge<Edge_Attributes_Type>* edge_copy = new Input_Edge<Edge_Attributes_Type>( *new_edge );

			(*_input_edge_reference)[new_edge->edge_id()] = edge_copy;
			_ordered_input_edge_reference->push_back(edge_copy);
		}

		template<typename Edge_Attributes_Type>
		size_t Compute_Graph_Size()
		{
			size_t current_graph_size = 0;
			size_t current_edge_size = 0;

			for(auto edges_itr = _ordered_input_edge_reference->begin(); edges_itr != _ordered_input_edge_reference->end(); edges_itr++)
			{
				Input_Edge<Edge_Attributes_Type>* current_edge = (Input_Edge<Edge_Attributes_Type>*) *edges_itr;

				current_edge_size += sizeof(stored_edge_type);

				for(auto groups_itr = current_edge->connection_groups().begin(); groups_itr != current_edge->connection_groups().end(); groups_itr++)
				{
					Input_Connection_Group* current_group = *groups_itr;

					current_edge_size += current_group->Connection_Group_Size();
				}

				current_edge_size = (current_edge_size/__cache_line_size + 1)*__cache_line_size;

				current_graph_size += current_edge_size;

				current_edge_size = 0;
			}

			return current_graph_size;
		}

		template<typename Edge_Attributes_Type>
		void Construct_Graph()
		{
			char* graph_itr = _graph;

			for(auto input_itr = _ordered_input_edge_reference->begin(); input_itr != _ordered_input_edge_reference->end(); input_itr++)
			{
				size_t current_edge_size = 0;

				current_edge_size += sizeof(stored_edge_type);

				Input_Edge<Edge_Attributes_Type>* current_input_edge = (Input_Edge<Edge_Attributes_Type>*)(*input_itr);
				//placement new operation, creats the object at the given memory location
				new (graph_itr) stored_edge_type();

				Edge<stored_edge_type>* current_edge = (Edge<stored_edge_type>*)graph_itr;

				current_input_edge->Construct_Edge_Attributes((Edge_Attributes_Type*)current_edge);

				_ordered_edge_reference->push_back( current_edge );
				(*_edge_reference)[current_edge->edge_id()] = current_edge;

				current_edge->begin_connection_groups( (Anonymous_Connection_Group<MasterType,base_edge_type>*)(((char*)graph_itr) + current_edge_size) );

				Anonymous_Connection_Group<MasterType,base_edge_type>* current_connection_group = current_edge->begin_connection_groups();

				for(auto connection_group_itr = current_input_edge->connection_groups().begin(); connection_group_itr!=current_input_edge->connection_groups().end(); connection_group_itr++)
				{
					Input_Connection_Group* current_input_connection_group = *connection_group_itr;

					current_edge_size += current_input_connection_group->Connection_Group_Size();

					current_input_connection_group->Construct_Connection_Group(current_connection_group);

					current_connection_group = current_connection_group->Next_Connection_Group();
				}
				//need to know when to stop iteratigh throug the connection groups when explore thr graph
				current_edge->end_connection_groups(current_connection_group);
				//round it to the __cashe_line_size, to speed up the access
				current_edge_size = (current_edge_size/__cache_line_size + 1)*__cache_line_size;

				//current_edge->edge_size(current_edge_size);

				graph_itr += current_edge_size;
			}
		}

		template<typename Edge_Attributes_Type>
		Interactive_Graph< output_graph_type >* Compile_Graph()
		{
			_edge_reference = new std::unordered_map<p_edge_id_type,Edge<stored_edge_type>*>();
			_ordered_edge_reference = new std::vector<Edge<stored_edge_type>*>();

			_graph_size = Compute_Graph_Size<Edge_Attributes_Type>();

			_graph = new char[_graph_size];

			Construct_Graph<Edge_Attributes_Type>();

			delete _input_edge_reference;
			delete _ordered_input_edge_reference;

			return (Interactive_Graph< output_graph_type >*)this;
		}

		void Link_Graph()
		{
			for(auto edge_itr = _ordered_edge_reference->begin(); edge_itr!=_ordered_edge_reference->end(); edge_itr++)
			{
				Edge<stored_edge_type>* current_edge = *edge_itr;

				Anonymous_Connection_Group<MasterType,base_edge_type>* connection_itr = current_edge->begin_connection_groups();
				const Anonymous_Connection_Group<MasterType,base_edge_type>* const end_connections = current_edge->end_connection_groups();

				while( connection_itr != end_connections )
				{
					connection_itr = connection_itr->Link_Edges(_graph_pool_reference);
				}
			}
		}

		virtual Interactive_Graph<ComponentType>* Create_Copy()
		{
			Graph_Implementation* copy = (Graph_Implementation*)new ComponentType();
			
			copy->_edge_reference = new std::unordered_map<p_edge_id_type,Edge<stored_edge_type>*>();
			copy->_ordered_edge_reference = new std::vector<Edge<stored_edge_type>*>();

			copy->_graph_id = _graph_id;

			copy->_compiled = true;

			copy->_graph_size = _graph_size;

			copy->_graph = new char[copy->_graph_size];

			memcpy(copy->_graph, _graph, copy->_graph_size);

			const long long address_deviation = (long long) ((long long*)copy->_graph - (long long*)_graph);
			
			for(auto itr = _ordered_edge_reference->begin();itr != _ordered_edge_reference->end(); itr++)
			{
				Edge<stored_edge_type>* current_edge = (Edge<stored_edge_type>*) *itr;
				Edge<stored_edge_type>* copy_edge = (Edge<stored_edge_type>*)(((long long*) *itr) + address_deviation);

				copy->_ordered_edge_reference->push_back(copy_edge);

				copy_edge->begin_connection_groups( (Anonymous_Connection_Group<Master_Type,base_edge_type>*)(((long long*)current_edge->begin_connection_groups())+address_deviation) );
				copy_edge->end_connection_groups( (Anonymous_Connection_Group<Master_Type,base_edge_type>*)(((long long*)current_edge->end_connection_groups())+address_deviation) );
				
				/*if (_graph_id == 1)
				{
					
					if (((MasterType::multimodal_edge_type*)copy_edge)->_unique_pattern_scanned.size())
					{
						((MasterType::multimodal_edge_type*)copy_edge)->_unique_pattern_scanned.clear();
						((MasterType::multimodal_edge_type*)copy_edge)->_unique_pattern_scanned[0] = true;
					}
				}*/

				copy_edge->begin_connection_groups()->Unlink_Edges();
			}

			for(auto itr = _edge_reference->begin();itr != _edge_reference->end(); itr++)
			{
				(*copy->_edge_reference)[itr->first] = (Edge<stored_edge_type>*)(((long long*)itr->second) + address_deviation);
			}

			return (Interactive_Graph<ComponentType>*)copy;
		}

		std::unordered_map<p_edge_id_type,void*>* _input_edge_reference;
		std::vector<void*>* _ordered_input_edge_reference;
		
		std::unordered_map< p_edge_id_type, Edge<stored_edge_type>* >* _edge_reference;
		std::vector< Edge<stored_edge_type>* >* _ordered_edge_reference;

		size_t _graph_size;
		char* _graph;

		t_data(void*, graph_pool_reference);
	};
}