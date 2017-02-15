#pragma once
#include "Graph.h"

namespace polaris
{
	//the highest lavel object in the library hierarchy. It contains all of the graph types, and each thread has a Graph_Pool 
	//see NetworkModel.cpp for usage example
	prototype struct Graph_Pool ADD_DEBUG_INFO
	{
		tag_as_prototype;
		//ComponentType is an implementation
		typedef typename ComponentType::base_edge_type base_edge_type;
		
		template<typename Graph_Type>
		Edge<typename Graph_Type::stored_edge_type>* Get_Edge(global_edge_id& gid)
		{
			return this_component()->Get_Edge<Graph_Type>(gid);
		}

		//template<typename Connection_Type>
		//Connection<Connection_Type>* Get_Connection(global_edge_id& inbound_edge_id,global_edge_id& outbound_edge_id)
		//{
		//	return this_component()->Get_Connection<Connection_Type>(inbound_edge_id,outbound_edge_id);
		//}

		std::vector<base_edge_type*>* Get_Edges(p_graph_id_type gid)
		{
			return this_component()->Get_Edges(gid);
		}

		base_edge_type* Get_Edge(global_edge_id& gid)
		{
			return this_component()->Get_Edge(gid);
		}

		void Link_Graphs()
		{
			this_component()->Link_Graphs();
		}

		template<typename Graph_Type>
		Graph_Assembler_Connected_Edge<Graph_Type>* Create_New_Graph()
		{
			return this_component()->Create_New_Graph<Graph_Type>();
		}

		Graph_Pool<ComponentType>* Create_Copy()
		{
			return this_component()->Create_Copy();
		}
	};
}