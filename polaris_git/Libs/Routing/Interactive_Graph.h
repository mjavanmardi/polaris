#pragma once
#include "Dependencies.h"
#include "boost\intrusive\set.hpp"
#include <fstream>

using namespace boost::intrusive;
//using namespace polaris;

namespace polaris
{
	template<typename Input_Edge_Type = NT, typename Edge_Type = NT>
	struct edge_construction_callback
	{
		typedef void(*type)(Input_Edge_Type* input_edge_ptr,Edge_Type* edge_ptr);
	};

	template<typename Input_Edge_Type = NT, typename Input_Node_Type = NT, typename Edge_Type = NT>
	struct edge_node_construction_callback
	{
		typedef void(*type)(Input_Edge_Type* input_edge_ptr,Input_Node_Type* input_anode_ptr,Input_Node_Type* input_bnode_ptr,Edge_Type* edge_ptr);
	};
	
	template<typename Edge_Type = NT>
	struct edge_update_callback
	{
		typedef void(*type)(Edge_Type* edge_ptr);
	};

	prototype struct Input_Connected_Edge
	{
		//tag_as_prototype;

		int id() { return this_component()->id<int>(); }

		float pos_x() { return this_component()->pos_x<float>(); }
		float pos_y() { return this_component()->pos_y<float>(); }

		Sequence<typename ComponentType::forward_edge_ids_type,int>* forward_edge_ids(){ return this_component()->forward_edge_ids< Sequence<typename ComponentType::forward_edge_ids_type,int>* >(); }

		Sequence<typename ComponentType::backward_edge_ids_type,int>* backward_edge_ids(){ return this_component()->backward_edge_ids< Sequence<typename ComponentType::backward_edge_ids_type,int>* >(); }
	};

	prototype struct Input_Node
	{
		//tag_as_prototype;

		int id() { return this_component()->id<int>(); }

		float pos_x() { return this_component()->pos_x<float>(); }
		float pos_y() { return this_component()->pos_y<float>(); }
	};

	prototype struct Input_Edge
	{
		//tag_as_prototype;

		int id() { return this_component()->id<int>(); }

		unsigned int in_node_id() { return this_component()->in_node_id<int>(); }
		unsigned int out_node_id() { return this_component()->out_node_id<int>(); }
	};


	template<typename ComponentType,typename Extended_Interface = NT,typename Extended_Neighbor_Interface = NT>
	struct Packed_Edge : public Extended_Interface
	{
		typedef typename ComponentType::neighbor_type neighbor_type;
		typedef typename ComponentType::neighbor_attributes_type neighbor_attributes_type;

		struct Neighbor_Edge : public Extended_Neighbor_Interface
		{
			Neighbor_Edge* next(){ return (Neighbor_Edge*)((neighbor_type*)this)->next(); }

			Packed_Edge* edge(){ return (Packed_Edge*)((neighbor_type*)this)->edge(); }
			void edge(Packed_Edge* value){ ((neighbor_type*)this)->edge(value); }
		};

		const int id(){ return this_component()->id(); }

		const float pos_x(){ return this_component()->pos_x(); }
		const float pos_y(){ return this_component()->pos_y(); }

		const int num_forward_edges(){ return this_component()->num_forward_edges(); }
		const int num_backward_edges(){ return this_component()->num_backward_edges(); }
	
		Neighbor_Edge* forward_edges(){ return (Neighbor_Edge*)this_component()->forward_edges(); }
		const Neighbor_Edge* const end_forward_edges(){ return (const Neighbor_Edge* const)this_component()->end_forward_edges(); }

		Neighbor_Edge* backward_edges(){ return (Neighbor_Edge*)this_component()->backward_edges(); }
		const Neighbor_Edge* const end_backward_edges(){ return (const Neighbor_Edge* const)this_component()->end_backward_edges(); }

		Packed_Edge* next(){ return (Packed_Edge*)this_component()->next(); }
	};
	
	
	prototype struct Interactive_Graph
	{
		tag_as_prototype;
		
		typedef typename ComponentType::edge_type edge_type;
		typedef typename ComponentType::edges_type edges_type;
		
		
		void Create_Sub_Graph_Negative(Interactive_Graph<ComponentType>* io_sub_graph,boost::unordered::unordered_set<int>& negative_edges, typename edge_update_callback<edge_type>::type callback = nullptr)
		{
			return this_component()->Create_Sub_Graph_Negative((ComponentType*)io_sub_graph,negative_edges,callback);
		}

		void Create_Sub_Graph_Positive(Interactive_Graph<ComponentType>* io_sub_graph,boost::unordered::unordered_set<int>& positive_edges, typename edge_update_callback<edge_type>::type callback = nullptr)
		{
			return this_component()->Create_Sub_Graph_Negative((ComponentType*)io_sub_graph,positive_edges,callback);
		}

		void Copy_Graph(Interactive_Graph<ComponentType>* io_copy, typename edge_update_callback<edge_type>::type callback = nullptr)
		{
			return this_component()->Copy_Graph((ComponentType*)io_copy,callback);
		}

		void Update_Edge(int id, typename edge_update_callback<edge_type>::type callback = nullptr)
		{
			this_component()->Update_Edge(id,callback);
		}

		void Update_Edges(boost::container::vector<int>* edge_set = nullptr, typename edge_update_callback<edge_type>::type callback = nullptr)
		{
			this_component()->Update_Edges(edge_set,callback);
		}

		template<typename ExtendedInterface>
		Packed_Edge<edge_type,ExtendedInterface>* Get_Edge(int id)
		{
			return this_component()->Get_Edge< Packed_Edge<edge_type,ExtendedInterface>* >(id);
		}

		template<typename ExtendedInterface>
		Sequence<edges_type,Packed_Edge<edge_type,ExtendedInterface>*>* edges()
		{
			return this_component()->edges< Sequence<edges_type,Packed_Edge<edge_type,InheritedPrototype>*>* >();
		}
	};
	
	prototype struct Graph_Assembler_Edge
	{
		tag_as_prototype;

		typedef typename ComponentType::edge_type edge_type;
		typedef typename ComponentType::output_graph_type output_graph_type;
	
		bool compiled(){ return this_component()->compiled<bool>(); }
		void compiled(bool value){ this_component()->compiled<bool>(value); }

		template<typename Input_Edge_Type>
		void Add_Edge(Input_Connected_Edge<Input_Edge_Type>* new_edge)
		{
			this_component()->Add_Edge(new_edge);
		}

		template<typename Input_Edge_Type>
		Interactive_Graph< output_graph_type >* Compile_Graph(typename edge_construction_callback<Input_Edge_Type, edge_type>::type construction_callback)
		{
			if(!compiled())
			{
				Interactive_Graph< output_graph_type >* return_value = this_component()->Compile_Graph<Input_Edge_Type>(construction_callback);

				if(return_value != nullptr) compiled(true);

				return return_value;
			}
			
			return nullptr;
		}
	};

	prototype struct Graph_Assembler_Edge_Node
	{
		tag_as_prototype;

		typedef typename ComponentType::edge_type edge_type;
		typedef typename ComponentType::output_graph_type output_graph_type;
		
		bool compiled(){ return this_component()->compiled<bool>(); }
		void compiled(bool value){ this_component()->compiled<bool>(value); }

		template<typename Input_Edge_Type>
		void Add_Edge(Input_Edge<Input_Edge_Type>* new_edge)
		{
			this_component()->Add_Edge(new_edge);
		}
		
		template<typename Input_Node_Type>
		void Add_Node(Input_Node<Input_Node_Type>* new_node)
		{
			this_component()->Add_Node(new_node);
		}

		template<typename Input_Edge_Type,typename Input_Node_Type>
		Interactive_Graph< output_graph_type >* Compile_Graph(typename edge_node_construction_callback<Input_Edge_Type, Input_Node_Type, edge_type>::type construction_callback)
		{
			if(!compiled())
			{
				Interactive_Graph< output_graph_type >* return_value = this_component()->Compile_Graph<Input_Edge_Type,Input_Node_Type>(construction_callback);
				
				if(return_value != nullptr) compiled(true);
				
				return return_value;
			}
			
			return nullptr;
		}
	};
}