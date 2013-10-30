#pragma once
#include "Interactive_Graph.h"

namespace polaris
{
	prototype struct Dijkstra_Neighbor;

	prototype struct Dijkstra_Edge
	{
		//tag_as_prototype;

		typedef typename ComponentType::neighbor_type neighbor_type;

		const long long id(){ return this_component()->id(); }
	
		const int num_forward_edges(){ return this_component()->num_forward_edges(); }
		Dijkstra_Neighbor<neighbor_type>* forward_edges(){ return (Dijkstra_Neighbor<neighbor_type>*)this_component()->forward_edges(); }
		const Dijkstra_Neighbor<neighbor_type>* const end_forward_edges(){ return (const Dijkstra_Neighbor<neighbor_type>* const)this_component()->end_forward_edges(); }

		float cost_from_origin(){ return this_component()->cost_from_origin(); }
		void cost_from_origin(float value){ this_component()->cost_from_origin(value); }

		float cost(){ return this_component()->cost(); }
	
	
		bool marked_for_reset(){ return this_component()->marked_for_reset(); }
		void marked_for_reset(bool value){ this_component()->marked_for_reset(value); }

		bool in_closed_set(){ return this_component()->in_closed_set(); }
		void in_closed_set(bool value){ this_component()->in_closed_set(value); }
	
		Dijkstra_Edge* came_from(){ return (Dijkstra_Edge*)this_component()->came_from(); }
		void came_from(Dijkstra_Edge* value){ this_component()->came_from(value); }


		void reset(){ this_component()->reset(); }

		/* + is compatible with intrusive multiset */
	};

	prototype struct Dijkstra_Neighbor
	{
		typedef typename ComponentType::edge_type edge_type;

		//float cost(){ return this_component()->cost(); }
		Dijkstra_Edge<edge_type>* edge(){ return (Dijkstra_Edge<edge_type>*)this_component()->edge(); }
		Dijkstra_Neighbor* next(){ return (Dijkstra_Neighbor*)this_component()->next(); }
	};

	prototype struct Dijkstra_Graph
	{
		tag_as_prototype;

		typedef typename ComponentType::edge_type edge_type;
		typedef typename ComponentType::edges_type edges_type;

		Dijkstra_Edge<edge_type>* Get_Edge(long long id){ return this_component()->Get_Edge< Dijkstra_Edge<edge_type>* >(id); }

		Prototype_Sequence<edges_type,Dijkstra_Edge>* edges()
		{
			return this_component()->edges<Prototype_Sequence<edges_type,Dijkstra_Edge>*>();
		}
	};

	template<typename GraphType>
	static Dijkstra_Edge<typename Dijkstra_Graph<GraphType>::edge_type>* Dijkstra(Dijkstra_Graph<GraphType>* graph, int start_id, int end_id, boost::container::vector<int>& out_path)
	{
		typedef typename Dijkstra_Graph<GraphType>::edge_type edge_type;
		typedef typename Dijkstra_Graph<GraphType>::edges_type edges_type;
		typedef typename Dijkstra_Edge<edge_type>::neighbor_type neighbor_type;

		boost::container::vector<Dijkstra_Edge<edge_type>*> modified_edges;
	
		Dijkstra_Edge<edge_type>* start = graph->Get_Edge(start_id);
		Dijkstra_Edge<edge_type>* end = graph->Get_Edge(end_id);

		start->cost_from_origin(0.0f);

		if(!start->marked_for_reset())
		{
			modified_edges.push_back(start);
			start->marked_for_reset(true);
		}
	
		boost::intrusive::multiset< edge_type > open_set;

		typedef typename Dijkstra_Graph<GraphType>::edges_type edges_type;

		Prototype_Sequence<edges_type,Dijkstra_Edge>* all_edges = graph->edges();

		for(Prototype_Sequence<edges_type,Dijkstra_Edge>::iterator itr = all_edges->begin();itr!=all_edges->end();itr++)
		{
			open_set.insert( *((edge_type*) (*itr)) );
		}

		bool success = false;

		while( open_set.size() )
		{
			Dijkstra_Edge<edge_type>* current = (Dijkstra_Edge<edge_type>*)(&(*open_set.begin()));

			if( current == end )
			{
				success = true;
				break;
			}

			if( current->cost_from_origin() == FLT_MAX )
			{
				break;
			}

			open_set.erase( open_set.iterator_to(*((edge_type*)current)) );
			current->in_closed_set(true);

			const Dijkstra_Neighbor<neighbor_type>* const end_neighbor_itr = current->end_forward_edges();
			
			for(Dijkstra_Neighbor<neighbor_type>* neighbor_itr = current->forward_edges(); neighbor_itr != end_neighbor_itr; neighbor_itr = neighbor_itr->next())
			{
				Dijkstra_Edge<edge_type>* current_neighbor = neighbor_itr->edge();

				if(current_neighbor->in_closed_set()) continue;

				float alt_score = current->cost_from_origin() + current_neighbor->cost();

				if(alt_score < current_neighbor->cost_from_origin())
				{
					if(!current_neighbor->marked_for_reset())
					{
						modified_edges.push_back(current_neighbor);
						current_neighbor->marked_for_reset(true);
					}

					current_neighbor->cost_from_origin(alt_score);
					current_neighbor->came_from(current);

					open_set.erase( open_set.iterator_to(*((edge_type*)current_neighbor) ) );
					open_set.insert( *((edge_type*)current_neighbor) );
				}
			}

		}

		for(boost::container::vector<Dijkstra_Edge<edge_type>*>::iterator itr = modified_edges.begin();itr!=modified_edges.end();itr++)
		{
			(*itr)->reset();
		}

		if(success)
		{
			Dijkstra_Edge<edge_type>* current = end;
			Dijkstra_Edge<edge_type>* cached_current = current;

			while(current != nullptr)
			{
				out_path.push_back(current->id());

				current = current->came_from();
			
				cached_current->came_from(nullptr);

				cached_current = current;
			}

			std::reverse(out_path.begin(),out_path.end());

			return end;
		}
		else return nullptr;
	}
}