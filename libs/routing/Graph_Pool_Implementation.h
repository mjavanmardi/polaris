#pragma once
#include "Graph_Pool.h"

namespace polaris
{
	template<typename MasterType, typename InheritanceList = NULLTYPELIST, typename Base_Graph_Type = NT>
	struct Graph_Pool_Implementation : public Polaris_Component<MasterType, typename Append<InheritanceList,Graph_Pool_Implementation<MasterType,NTL,Base_Graph_Type>>::Result>
	{
		typedef Polaris_Component<MasterType, typename Append<InheritanceList,Graph_Pool_Implementation<MasterType,NTL,Base_Graph_Type>>::Result> Base_t;
		typedef typename Base_t::ComponentType ComponentType;

		Graph_Pool_Implementation():_num_graphs(0){}

		typedef Base_Graph_Type base_graph_type;
		typedef typename base_graph_type::base_edge_type base_edge_type;

		std::vector<base_edge_type*>* Get_Edges(p_graph_id_type gid)
		{
			if( gid/*.graph_id*/ < _graphs.size() && gid/*.graph_id*/ >= 0 )
			{
				Interactive_Graph<base_graph_type>* current_graph = (Interactive_Graph<base_graph_type>*)_graphs[ gid/*.graph_id*/ ];

				return current_graph->Get_Edges();
			}
			else
			{
				THROW_WARNING("Graph id: " << gid/*.graph_id*/ << " not found");

				return nullptr;
			}
		}



		base_edge_type* Get_Edge(global_edge_id& gid)
		{
			if( gid.graph_id < _graphs.size() && gid.graph_id >= 0 )
			{
				Interactive_Graph<base_graph_type>* current_graph = (Interactive_Graph<base_graph_type>*)_graphs[ gid.graph_id ];

				return current_graph->Get_Edge(gid.edge_id);
			}
			else
			{
				THROW_WARNING("Graph id: " << gid.graph_id << " not found");

				return nullptr;
			}
		}

		template<typename Graph_Type>
		Edge<typename Graph_Type::stored_edge_type>* Get_Edge(global_edge_id& gid)
		{
			if( gid.graph_id < _graphs.size() && gid.graph_id >= 0 )
			{
				Interactive_Graph<Graph_Type>* current_graph = (Interactive_Graph<Graph_Type>*)_graphs[ gid.graph_id ];

				return current_graph->Get_Edge<typename Graph_Type::stored_edge_type>(gid.edge_id);
			}
			else
			{
				THROW_WARNING("Graph id: " << gid.graph_id << " not found");

				return nullptr;
			}
		}
		//liks all of the graphs in the pool
		void Link_Graphs()
		{
			for(auto itr = _graphs.begin(); itr != _graphs.end(); itr++)
			{
				Interactive_Graph<base_graph_type>* current_graph = (Interactive_Graph<base_graph_type>*) *itr;

				current_graph->Link_Graph();
			}
		}

		template<typename Graph_Type>
		Graph_Assembler_Connected_Edge<Graph_Type>* Create_New_Graph()
		{
			//Graph_Assembler_Connected_Edge<Graph_Type>* graph = (Graph_Assembler_Connected_Edge<Graph_Type>*)Allocate<Graph_Type>();
			Graph_Assembler_Connected_Edge<Graph_Type>* graph = (Graph_Assembler_Connected_Edge<Graph_Type>*)(new Graph_Type());

			_graphs.push_back(graph);

			graph->graph_pool_reference(this);
			graph->graph_id( _num_graphs );

			++_num_graphs;

			return graph;
		}

		Graph_Pool<ComponentType>* Create_Copy()
		{
			//cout << typeid(ComponentType).name() << endl;

			Graph_Pool_Implementation* copy = (Graph_Pool_Implementation*) new ComponentType();

			copy->_num_graphs = _num_graphs;

			for(auto itr = _graphs.begin(); itr != _graphs.end(); itr++)
			{
				Interactive_Graph<base_graph_type>* current_graph = (Interactive_Graph<base_graph_type>*) *itr;

				Graph_Assembler_Connected_Edge<base_graph_type>* copy_graph = (Graph_Assembler_Connected_Edge<base_graph_type>*)current_graph->Create_Copy();

				copy->_graphs.push_back(copy_graph);

				copy_graph->graph_pool_reference(copy);
			}

			copy->Link_Graphs();

			return (Graph_Pool<ComponentType>*)copy;
		}


		std::vector<void*> _graphs;

		p_graph_id_type _num_graphs;
	};
}