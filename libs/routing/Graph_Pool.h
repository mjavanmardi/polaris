#pragma once
#include "Graph.h"

namespace polaris
{
	prototype struct Graph_Pool
	{
		tag_as_prototype;

		typedef typename ComponentType::base_edge_type base_edge_type;
		
		template<typename Graph_Type>
		Edge<typename Graph_Type::stored_edge_type>* Get_Edge(global_edge_id& gid)
		{
			return this_component()->Get_Edge<Graph_Type>(gid);
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