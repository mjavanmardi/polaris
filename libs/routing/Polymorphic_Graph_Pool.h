#pragma once
#include "Interactive_Graph.h"

namespace polaris
{
	struct global_edge_id
	{
		unsigned char graph_id;
		unsigned long long edge_id : 56;
	};

	prototype struct Polymorphic_Graph_Pool
	{
		tag_as_prototype;

		typedef typename ComponentType::base_edge_type base_edge_type;
		
		unsigned char Add_Graph(Polymorphic_Graph<graph_type>* graph)
		{
			return this_component()->Add_Graph(graph);
		}

		Polymorphic_Graph<graph_type>* Get_Graph(unsigned char graph_id)
		{
			return this_component()->Get_Graph(graph);
		}

		Edge<base_edge_type>* Get_Edge(global_edge_id& edge_id)
		{
			return this_component()->Get_Edge(edge_id);
		}
	};
}