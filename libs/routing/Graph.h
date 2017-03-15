#pragma once
#include "Edge.h"



namespace polaris
{




	prototype struct Interactive_Graph ADD_DEBUG_INFO
	{
		typedef typename ComponentType::base_edge_type base_edge_type;
		
		Interactive_Graph<ComponentType>* Create_Copy()
		{
			return this_component()->Create_Copy();
		}
		
		std::vector<base_edge_type*>* Get_Edges()
		{
			return this_component()->Get_Edges();
		}

		template<typename Edge_Type>
		Edge<Edge_Type>* Get_Edge(p_edge_id_type edge_id)
		{
			return this_component()->Get_Edge<Edge_Type>(edge_id);
		}
		
		base_edge_type* Get_Edge(p_edge_id_type edge_id)
		{
			return this_component()->Get_Edge(edge_id);
		}

		void Link_Graph()
		{
			//this is the way to call a function from implementation class
			this_component()->Link_Graph();
		}
	};
	//interface for adding edges to the graph
	prototype struct Graph_Assembler_Connected_Edge ADD_DEBUG_INFO
	{
		tag_as_prototype;

		typedef typename ComponentType::output_graph_type output_graph_type;

		p_graph_id_type graph_id(){ return this_component()->graph_id(); }
		void graph_id(p_graph_id_type value){ this_component()->graph_id(value); }
	
		void* graph_pool_reference(){ return this_component()->graph_pool_reference(); }
		void graph_pool_reference(void* value){ this_component()->graph_pool_reference(value); }

		bool compiled(){ return this_component()->compiled(); }
		void compiled(bool value){ this_component()->compiled(value); }

		template<typename Edge_Attributes_Type>
		void Add_Edge(Input_Edge<Edge_Attributes_Type>* new_edge)
		{
			this_component()->Add_Edge(new_edge);
		}

		template<typename Edge_Attributes_Type>
		Interactive_Graph< output_graph_type >* Compile_Graph()
		{
			if( !compiled() )
			{
				Interactive_Graph< output_graph_type >* return_value = nullptr;
			
				return_value = this_component()->Compile_Graph<Edge_Attributes_Type>();

				if(return_value != nullptr) compiled(true);

				return return_value;
			}
		
			return nullptr;
		}
	};

}