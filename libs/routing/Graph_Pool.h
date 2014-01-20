#pragma once
#include "Dependencies.h"

namespace polaris
{
	prototype struct Graph_Pool
	{
		tag_as_prototype;

		typedef typename ComponentType::graph_type graph_type;
		
		unsigned int Add_Graph(Interactive_Graph<graph_type>* graph)
		{
			return this_component()->Add_Graph(graph);
		}

		Interactive_Graph<graph_type>* Get_Graph(unsigned int graph_id)
		{
			return this_component()->Get_Graph(graph);
		}

		void Update_Edge(long long id, boost::container::vector<int>* graph_set = nullptr, typename edge_update_callback<edge_type>::type callback = nullptr)
		{
			this_component()->Update_Edge(id,graph_set,callback);
		}

		void Update_Edges(boost::container::vector<int>* edge_set = nullptr, boost::container::vector<int>* graph_set = nullptr, typename edge_update_callback<edge_type>::type callback = nullptr)
		{
			this_component()->Update_All_Edges(id,edge_set,graph_set,callback);
		}
	};
}