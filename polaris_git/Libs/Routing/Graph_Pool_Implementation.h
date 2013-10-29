#pragma once
#include "Graph_Pool.h"

namespace polaris
{
	template<typename MasterType, typename InheritanceList = NULLTYPELIST, typename Graph_Type = NT>
	struct Graph_Pool_Implementation : public Polaris_Component<MasterType,INHERIT(Graph_Pool_Implementation)>
	{
		Graph_Pool_Implementation():_stored_graphs(0){}

		typedef typename Graph_Type graph_type;
		typedef typename graph_type::edge_type edge_type;

		boost::unordered::unordered_map<int,Interactive_Graph<graph_type>*> _graph_pool;
		unsigned int _stored_graphs;

		unsigned int Add_Graph(Interactive_Graph<graph_type>* graph)
		{
			_graph_pool[_stored_graphs] = graph;

			return ++_stored_graphs;
		}
		
		Interactive_Graph<graph_type>* Get_Graph(unsigned int graph_id)
		{
			if(_graph_pool.count(graph_id))
			{
				return _graph_pool[graph_id];
			}
			else
			{
				THROW_WARNING("Graph: " << graph_id << " does not exist in pool!");
				return nullptr;
			}
		}

		void Update_Edge(long long id, boost::container::vector<int>* graph_set = nullptr, typename edge_update_callback<edge_type>::type callback = nullptr)
		{
			if(graph_set == nullptr)
			{
				for(boost::unordered::unordered_map<int,Interactive_Graph<graph_type>*>::iterator itr = _graph_pool.begin();itr!=_graph_pool.end();itr++)
				{
					itr->second->Update_Edge(id,callback);
				}
			}
			else
			{
				for( boost::container::vector<int>::iterator itr = graph_set->begin();itr!=graph_set->end();itr++)
				{
					if(_graph_pool.count( *itr ))
					{
						_graph_pool[*itr]->Update_Edge(id,callback);
					}
					else
					{
						THROW_WARNING("Graph: " << *itr << " does not exist in pool!");
					}
				}
			}	
		}

		void Update_Edges(boost::container::vector<long long>* edge_set = nullptr, boost::container::vector<int>* graph_set = nullptr, typename edge_update_callback<edge_type>::type callback = nullptr)
		{
			if(graph_set == nullptr)
			{
				for(boost::unordered::unordered_map<int,Interactive_Graph<graph_type>*>::iterator itr = _graph_pool.begin();itr!=_graph_pool.end();itr++)
				{
					itr->second->Update_Edges(edge_set,callback);
				}
			}
			else
			{
				for( boost::container::vector<int>::iterator itr = graph_set->begin();itr!=graph_set->end();itr++)
				{
					if(_graph_pool.count(*itr))
					{
						_graph_pool[*itr]->Update_Edges(edge_set,callback);
					}
					else
					{
						THROW_WARNING("Graph: " << *itr << " does not exist in pool!");
					}
				}
			}			
		}
	};
}