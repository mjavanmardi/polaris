#pragma once
#include "Interactive_Graph.h"

namespace polaris
{
	prototype struct A_Star_Neighbor;

	prototype struct A_Star_Edge
	{
		//tag_as_prototype;

		typedef typename ComponentType::neighbor_type neighbor_type;

		const int id(){ return this_component()->id(); }
	
		const int num_forward_edges(){ return this_component()->num_forward_edges(); }
		A_Star_Neighbor<neighbor_type>* forward_edges(){ return (A_Star_Neighbor<neighbor_type>*)this_component()->forward_edges(); }
		const A_Star_Neighbor<neighbor_type>* const end_forward_edges(){ return (const A_Star_Neighbor<neighbor_type>* const)this_component()->end_forward_edges(); }

		float f_score(){ return this_component()->f_score(); }
		void f_score(float value){ this_component()->f_score(value); }

		float g_score(){ return this_component()->g_score(); }
		void g_score(float value){ this_component()->g_score(value); }

		float weight_between(A_Star_Edge* other){ return this_component()->weight_between(other); }
		float weight(){ return this_component()->weight(); }
	
	
		bool marked_for_reset(){ return this_component()->marked_for_reset(); }
		void marked_for_reset(bool value){ this_component()->marked_for_reset(value); }

		bool in_closed_set(){ return this_component()->in_closed_set(); }
		void in_closed_set(bool value){ this_component()->in_closed_set(value); }

		bool in_open_set(){ return this_component()->in_open_set(); }
		void in_open_set(bool value){ this_component()->in_open_set(value); }
	
		A_Star_Edge* came_from(){ return (A_Star_Edge*)this_component()->came_from(); }
		void came_from(A_Star_Edge* value){ this_component()->came_from(value); }


		void reset(){ this_component()->reset(); }

		/* + is compatible with intrusive multiset */
	};

	prototype struct A_Star_Neighbor
	{
		typedef typename ComponentType::edge_type edge_type;

		//float weight(){ return this_component()->weight(); }
		A_Star_Edge<edge_type>* edge(){ return (A_Star_Edge<edge_type>*)this_component()->edge(); }
		A_Star_Neighbor* next(){ return (A_Star_Neighbor*)this_component()->next(); }
	};




	prototype struct A_Star_Graph
	{
		tag_as_prototype;

		typedef typename ComponentType::edge_type edge_type;
	
		A_Star_Edge<edge_type>* Get_Edge(int id){ return this_component()->Get_Edge< A_Star_Edge<edge_type>* >(id); }
	};



	template<typename GraphType>
	static A_Star_Edge<typename A_Star_Graph<GraphType>::edge_type>* A_Star(A_Star_Graph<GraphType>* graph, int start_id, int end_id, boost::container::vector<int>& out_path)
	{
		typedef typename A_Star_Graph<GraphType>::edge_type edge_type;
		typedef typename A_Star_Edge<edge_type>::neighbor_type neighbor_type;
		

		boost::container::vector< A_Star_Edge<edge_type>* > modified_edges;
		boost::intrusive::multiset< edge_type > open_set;

		A_Star_Edge<edge_type>* start = graph->Get_Edge(start_id);
		A_Star_Edge<edge_type>* end = graph->Get_Edge(end_id);

		
		open_set.insert( *((edge_type*)start) );

		start->g_score(0.0f);
	
		//float initial_f_score = start->g_score() + sqrt( (start->pos_x()-end->pos_x())*(start->pos_x()-end->pos_x()) + (start->pos_y()-end->pos_y())*(start->pos_y()-end->pos_y()) );
		float initial_f_score = start->g_score() + start->weight_between(end);

		start->f_score( initial_f_score );

		if( !start->marked_for_reset() )
		{
			modified_edges.push_back(start);
			start->marked_for_reset(true);
		}
	
		bool success = false;

		while( open_set.size() )
		{
			A_Star_Edge<edge_type>* current = (A_Star_Edge<edge_type>*)&(*open_set.begin());

			if( current == end )
			{
				success = true;
				break;
			}
		
			open_set.erase( open_set.iterator_to( *((edge_type*)current) ) );
		
			current->in_closed_set(true);
		
			if( !current->marked_for_reset() )
			{
				modified_edges.push_back(current);
				current->marked_for_reset(true);
			}

			const A_Star_Neighbor<neighbor_type>* const end_neighbor_itr = current->end_forward_edges();		

			for(A_Star_Neighbor<neighbor_type>* neighbor_itr = current->forward_edges(); neighbor_itr != end_neighbor_itr; neighbor_itr = neighbor_itr->next())
			{
				A_Star_Edge<edge_type>* current_neighbor = neighbor_itr->edge();

				if(current_neighbor->in_closed_set()) continue;

				float g_score = current_neighbor->g_score() + current_neighbor->weight();

				bool tentative_better=false;

				if(!current_neighbor->in_open_set()) tentative_better = true;
				else if(g_score < current_neighbor->g_score()) tentative_better = true;

				if(tentative_better)
				{
					if(!current_neighbor->marked_for_reset())
					{
						modified_edges.push_back(current_neighbor);
						current_neighbor->marked_for_reset(true);
					}

					current_neighbor->came_from(current);
					current_neighbor->g_score(g_score);

					//float neighbor_f_score = g_score + sqrt((end->pos_x()-current_neighbor->pos_x())*(end->pos_x()-current_neighbor->pos_x()) + (end->pos_y()-current_neighbor->pos_y())*(end->pos_y()-current_neighbor->pos_y()));
					float neighbor_f_score = g_score + current_neighbor->weight_between(end);

					current_neighbor->f_score(neighbor_f_score);
				
					if( current_neighbor->in_open_set() ) open_set.erase( open_set.iterator_to( *((edge_type*)current_neighbor) ) );
				
					open_set.insert( *((edge_type*)current_neighbor) );
					current_neighbor->in_open_set(true);
				}
			}
		}

		for(boost::container::vector< A_Star_Edge<edge_type>* >::iterator itr = modified_edges.begin();itr!=modified_edges.end();itr++)
		{
			(*itr)->reset();
		}

		if(success)
		{
			A_Star_Edge<edge_type>* current = end;
			A_Star_Edge<edge_type>* cached_current = current;

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