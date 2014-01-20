#pragma once
#include "Interactive_Graph.h"

namespace polaris
{
	prototype struct A_Star_Neighbor;

	prototype struct A_Star_Edge
	{
		//tag_as_prototype;

		typedef typename ComponentType::neighbor_type neighbor_type;

		const long long id(){ return this_component()->id(); }
		
		const int num_forward_edges(){ return this_component()->num_forward_edges(); }
		A_Star_Neighbor<neighbor_type>* forward_edges(){ return (A_Star_Neighbor<neighbor_type>*)this_component()->forward_edges(); }
		const A_Star_Neighbor<neighbor_type>* const end_forward_edges(){ return (const A_Star_Neighbor<neighbor_type>* const)this_component()->end_forward_edges(); }

		float cost_origin_destination(){ return this_component()->cost_origin_destination(); }
		void cost_origin_destination(float value){ this_component()->cost_origin_destination(value); }

		float cost_from_origin(){ return this_component()->cost_from_origin(); }
		void cost_from_origin(float value){ this_component()->cost_from_origin(value); }

		float time_from_origin(){ return this_component()->time_from_origin(); }
		void time_from_origin(float value){ this_component()->time_from_origin(value); }

		float cost_between(A_Star_Edge* other){ return this_component()->cost_between(other); }
		float cost(){ return this_component()->cost(); }
		
		float time_cost(){ return this_component()->time_cost(); }
		
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

		float time_cost(){ return this_component()->time_cost(); }
		void time_cost(float value){ return this_component()->time_cost(value); }

		float cost(){ return this_component()->cost(); }
		void cost(float value){ return this_component()->cost(value); }

		inline A_Star_Edge<edge_type>* edge(){ return (A_Star_Edge<edge_type>*)this_component()->edge(); }
		inline A_Star_Neighbor* next(){ return (A_Star_Neighbor*)this_component()->next(); }
	};

	prototype struct A_Star_Graph
	{
		tag_as_prototype;

		typedef typename ComponentType::edge_type edge_type;
	
		A_Star_Edge<edge_type>* Get_Edge(long long id){ return this_component()->Get_Edge< A_Star_Edge<edge_type>* >(id); }
	};

	template<typename GraphType>
	static A_Star_Edge<typename A_Star_Graph<GraphType>::edge_type>* A_Star(A_Star_Graph<GraphType>* graph, long long start_id, long long end_id, boost::container::deque<long long>& out_path)
	{
		typedef typename A_Star_Graph<GraphType>::edge_type edge_type;
		typedef typename A_Star_Edge<edge_type>::neighbor_type neighbor_type;

		boost::container::deque< A_Star_Edge<edge_type>* > modified_edges;
		
		boost::intrusive::multiset< edge_type > open_set;

		A_Star_Edge<edge_type>* start = graph->Get_Edge(start_id);
		if(start == nullptr){ THROW_WARNING("Origin: " << start_id << " not found in graph!"); return nullptr; }

		A_Star_Edge<edge_type>* end = graph->Get_Edge(end_id);
		if(end == nullptr){ THROW_WARNING("Destination: " << end_id << " not found in graph!"); return nullptr; }

		start->cost_from_origin(0.0f);
		
		float initial_cost_origin_destination = start->cost_from_origin() + start->cost_between(end);

		start->cost_origin_destination( initial_cost_origin_destination );
		
		open_set.insert( *((edge_type*)start) );

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
			//  THIS MAY VERY WELL BE WRONG!
			//	A_Star_Edge<edge_type>* current_neighbor = neighbor_itr->edge();

			//	if(current_neighbor->in_closed_set()) continue;

			//	//float time_from_origin = current_neighbor->time_from_origin() + current_neighbor->time_cost() + neighbor_itr->time_cost();
			//	//float cost_from_origin = current_neighbor->cost_from_origin() + current_neighbor->cost() + neighbor_itr->cost();

			//	float time_from_origin = current->time_from_origin() + current_neighbor->time_cost() + neighbor_itr->time_cost();
			//	float cost_from_origin = current->cost_from_origin() + current_neighbor->cost() + neighbor_itr->cost();

			//	bool tentative_better=false;

			//	if(!current_neighbor->in_open_set()) tentative_better = true;
			//	else if(cost_from_origin < current_neighbor->cost_from_origin()) tentative_better = true;

			//	if(tentative_better)
			//	{
			//		if(!current_neighbor->marked_for_reset())
			//		{
			//			modified_edges.push_back(current_neighbor);
			//			current_neighbor->marked_for_reset(true);
			//		}

			//		current_neighbor->came_from(current);
			//		current_neighbor->cost_from_origin(cost_from_origin);

			//		current_neighbor->time_from_origin(time_from_origin);

			//		float neighbor_cost_origin_destination = cost_from_origin + current_neighbor->cost_between(end);

			//		current_neighbor->cost_origin_destination(neighbor_cost_origin_destination);

			//		if( current_neighbor->in_open_set() ) open_set.erase( open_set.iterator_to( *((edge_type*)current_neighbor) ) );

			//		open_set.insert( *((edge_type*)current_neighbor) );

			//		current_neighbor->in_open_set(true);
			//	}
			//}
		}

		for(boost::container::deque< A_Star_Edge<edge_type>* >::iterator itr = modified_edges.begin();itr!=modified_edges.end();itr++)
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

	//template<typename GraphType>
	//static A_Star_Edge<typename A_Star_Graph<GraphType>::edge_type>* A_Star_Set(A_Star_Graph<GraphType>* graph, int start_id, int end_id, boost::container::deque<int>& out_path)
	//{
	//	typedef typename A_Star_Graph<GraphType>::edge_type edge_type;
	//	typedef typename A_Star_Edge<edge_type>::neighbor_type neighbor_type;
	//	

	//	boost::container::deque< A_Star_Edge<edge_type>* > modified_edges;
	//	
	//	//std::set< pair<float,edge_type*> > open_set;
	//	//std::multiset< pair<float,edge_type*> > open_set;
	//	std::multimap<float,edge_type*> open_set;

	//	A_Star_Edge<edge_type>* start = graph->Get_Edge(start_id);
	//	A_Star_Edge<edge_type>* end = graph->Get_Edge(end_id);

	//	start->cost_from_origin(0.0f);
	//	
	//	float initial_cost_origin_destination = start->cost_from_origin() + start->cost_between(end);

	//	start->cost_origin_destination( initial_cost_origin_destination );
	//	
	//	open_set.insert( pair<float,edge_type*>(start->cost_origin_destination(),((edge_type*)start)) );

	//	if( !start->marked_for_reset() )
	//	{
	//		modified_edges.push_back(start);
	//		start->marked_for_reset(true);
	//	}
	//	
	//	bool success = false;

	//	while( open_set.size() )
	//	{
	//		A_Star_Edge<edge_type>* current = (A_Star_Edge<edge_type>*)(open_set.begin()->second);

	//		if( current == end )
	//		{
	//			success = true;
	//			break;
	//		}
	//		
	//		open_set.erase( open_set.begin() );

	//		current->in_closed_set(true);
	//		
	//		if( !current->marked_for_reset() )
	//		{
	//			modified_edges.push_back(current);
	//			current->marked_for_reset(true);
	//		}

	//		const A_Star_Neighbor<neighbor_type>* const end_neighbor_itr = current->end_forward_edges();		

	//		for(A_Star_Neighbor<neighbor_type>* neighbor_itr = current->forward_edges(); neighbor_itr != end_neighbor_itr; neighbor_itr = neighbor_itr->next())
	//		{
	//			A_Star_Edge<edge_type>* current_neighbor = neighbor_itr->edge();

	//			if(current_neighbor->in_closed_set()) continue;

	//			float cost_from_origin = current_neighbor->cost_from_origin() + current_neighbor->cost();

	//			bool tentative_better=false;

	//			if(!current_neighbor->in_open_set()) tentative_better = true;
	//			else if(cost_from_origin < current_neighbor->cost_from_origin()) tentative_better = true;

	//			if(tentative_better)
	//			{
	//				if(!current_neighbor->marked_for_reset())
	//				{
	//					modified_edges.push_back(current_neighbor);
	//					current_neighbor->marked_for_reset(true);
	//				}

	//				current_neighbor->came_from(current);
	//				current_neighbor->cost_from_origin(cost_from_origin);

	//				float neighbor_cost_origin_destination = cost_from_origin + current_neighbor->cost_between(end);

	//				current_neighbor->cost_origin_destination(neighbor_cost_origin_destination);
	//				
	//				//if( current_neighbor->in_open_set() ) open_set.erase( pair<float,edge_type*>(current_neighbor->cost_origin_destination(), (edge_type*)current_neighbor ) );
	//				
	//				//if( current_neighbor->in_open_set() )
	//				//{
	//				//	std::multiset<pair<float,edge_type*>>::iterator found = open_set.find( pair<float,edge_type*>(current_neighbor->cost_origin_destination(), (edge_type*)current_neighbor ) );
	//				//	
	//				//	while( found->first == current_neighbor->cost_origin_destination() )
	//				//	{
	//				//		if( found->second == (edge_type*)current_neighbor)
	//				//		{
	//				//			open_set.erase( found );
	//				//			break;
	//				//		}
	//				//
	//				//		++found;
	//				//	}
	//				//}

	//				if( current_neighbor->in_open_set() )
	//				{
	//					std::multimap<float,edge_type*>::iterator found = open_set.find(current_neighbor->cost_origin_destination());
	//					
	//					while( found->first == current_neighbor->cost_origin_destination() )
	//					{
	//						if( found->second == (edge_type*)current_neighbor)
	//						{
	//							open_set.erase( found );
	//							break;
	//						}
	//				
	//						++found;
	//					}
	//				}

	//				open_set.insert( pair<float,edge_type*>(current_neighbor->cost_origin_destination(),(edge_type*)current_neighbor) );

	//				current_neighbor->in_open_set(true);
	//			}
	//		}
	//	}

	//	for(boost::container::deque< A_Star_Edge<edge_type>* >::iterator itr = modified_edges.begin();itr!=modified_edges.end();itr++)
	//	{
	//		(*itr)->reset();
	//	}

	//	if(success)
	//	{
	//		A_Star_Edge<edge_type>* current = end;
	//		A_Star_Edge<edge_type>* cached_current = current;

	//		while(current != nullptr)
	//		{
	//			out_path.push_back(current->id());

	//			current = current->came_from();
	//		
	//			cached_current->came_from(nullptr);

	//			cached_current = current;
	//		}

	//		std::reverse(out_path.begin(),out_path.end());

	//		return end;
	//	}
	//	else return nullptr;
	//}
}