#pragma once
#include "Graph_Implementation.h"

namespace polaris
{
	union transit_edge_id
	{
		struct composite_id
		{
			unsigned int time;
			unsigned int trip_id;
		} composite_id;

		long long id;
	};

	template<typename MasterType,typename AgentType,typename GraphPoolType>
	static void* A_Star(Routable_Agent<AgentType>* agent, Graph_Pool<GraphPoolType>* graph_pool, global_edge_id& start_id, global_edge_id& end_id, unsigned int start_time, boost::container::deque< global_edge_id >& out_path)
	{
		typedef typename Graph_Pool<GraphPoolType>::base_edge_type base_edge_type;

		boost::container::deque< base_edge_type* > modified_edges;
		
		boost::intrusive::multiset< base_edge_type > open_set;

		A_Star_Edge<base_edge_type>* start = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(start_id);
		if(start == nullptr){ THROW_WARNING("Origin: " << start_id.edge_id << " not found in graph pool!"); return nullptr; }

		A_Star_Edge<base_edge_type>* end = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(end_id);
		if(end == nullptr){ THROW_WARNING("Destination: " << end_id.edge_id << " not found in graph!"); return nullptr; }


		Routing_Data<base_edge_type> routing_data;

		routing_data.modified_edges = &modified_edges;
		routing_data.open_set = &open_set;
		routing_data.start_edge = (base_edge_type*)start;
		routing_data.end_edge = (base_edge_type*)end;
		routing_data.start_time = start_time;

		start->cost_from_origin(0.0f);
		start->time_label((float)start_time);

		float initial_estimated_cost_origin_destination = start->cost_from_origin() + agent->estimated_cost_between((base_edge_type*)start,(base_edge_type*)end);

		start->estimated_cost_origin_destination( initial_estimated_cost_origin_destination );
		
		open_set.insert( *((base_edge_type*)start) );

		if( !start->marked_for_reset() )
		{
			modified_edges.push_back((base_edge_type*)start);
			start->marked_for_reset(true);
		}
		
		bool success = false;

		while( open_set.size() )
		{
			A_Star_Edge<base_edge_type>* current = (A_Star_Edge<base_edge_type>*)&(*open_set.begin());

			transit_edge_id id;
			
			id.id = current->edge_id();

			if( agent->at_destination((base_edge_type*)current, (base_edge_type*)end) )
			{
				success = true;
				break;
			}
			
			open_set.erase( open_set.iterator_to( *((base_edge_type*)current) ) );

			current->in_open_set(false);
			current->in_closed_set(true);

			Anonymous_Connection_Group<MasterType,base_edge_type>* connection_set_iterator = current->begin_connection_groups();
			const Anonymous_Connection_Group<MasterType,base_edge_type>* const connection_set_end = current->end_connection_groups();

			while( connection_set_iterator != connection_set_end )
			{
				connection_set_iterator = connection_set_iterator->Visit_Neighbors(agent, current, routing_data);
			}

		}

		for(boost::container::deque< base_edge_type* >::iterator itr = modified_edges.begin();itr!=modified_edges.end();itr++)
		{
			(*itr)->reset();
		}
		
		global_edge_id global;
		global.graph_id = 0;

		if(success)
		{
			base_edge_type* current = (base_edge_type*)end;
			base_edge_type* cached_current = (base_edge_type*)current;

			while(current != nullptr)
			{
				global.edge_id = current->_edge_id;
				
				out_path.push_back(global);

				current = (base_edge_type*)current->came_from();

				cached_current->came_from(nullptr);

				cached_current = current;
			}

			std::reverse(out_path.begin(),out_path.end());

			return end;
		}
		else
		{
			cout << "Failed to create route!" << endl;

			return nullptr;
		}
	}
}