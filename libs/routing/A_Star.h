#pragma once
#include "Graph_Implementation.h"

namespace polaris
{
	//this is not a descriptive name, this is used for any ID
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
	static float A_Star(Routable_Agent<AgentType>* agent, Graph_Pool<GraphPoolType>* graph_pool, std::vector<global_edge_id>& start_ids, std::vector<global_edge_id>& end_ids, unsigned int start_time, std::deque< global_edge_id >& out_path, std::deque< float >& out_cost)
	{
		typedef typename Graph_Pool<GraphPoolType>::base_edge_type base_edge_type;

		std::deque< base_edge_type* > modified_edges;
		
		boost::intrusive::multiset< base_edge_type > open_set;

		std::vector<base_edge_type*> starts;
		A_Star_Edge<base_edge_type>* start;
		for (auto itr = start_ids.begin(); itr != start_ids.end(); ++itr)
		{
			start = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(*itr);
			if(start == nullptr){ THROW_WARNING("Origin: " << (*itr).edge_id << " not found in graph pool!"); return 0.0f; }
			starts.push_back((base_edge_type*)start);
		}
		base_edge_type* start_base = (base_edge_type*)start;

		std::vector<base_edge_type*> ends;
		A_Star_Edge<base_edge_type>* end;
		for (auto itr = end_ids.begin(); itr != end_ids.end(); ++itr)
		{
			end = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(*itr);
			if(end == nullptr){ THROW_WARNING("Destination: " << (*itr).edge_id << " not found in graph!"); return 0.0f; }
			ends.push_back((base_edge_type*)end);
		}
		base_edge_type* end_base = (base_edge_type*)end;

		Routing_Data<base_edge_type> routing_data;

		routing_data.modified_edges = &modified_edges;
		routing_data.open_set = &open_set;
		routing_data.start_edge = (base_edge_type*)starts.front();
		routing_data.end_edge = (base_edge_type*)ends.front();
		routing_data.start_time = start_time;

		for (auto itr = starts.begin(); itr != starts.end(); ++itr)
		{
			start = (A_Star_Edge<base_edge_type>*)(*itr);
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
		}
		
		bool success = false;

		while( open_set.size() )
		{
			A_Star_Edge<base_edge_type>* current = (A_Star_Edge<base_edge_type>*)&(*open_set.begin());

			transit_edge_id id;
			
			id.id = current->edge_id();

			if( agent->at_destination((base_edge_type*)current, ends, &end_base) )
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

		
		global_edge_id global;
		global.graph_id = 0;

		float total_cost = FLT_MAX;

		if(success)
		{
			base_edge_type* current = end_base;//(base_edge_type*)end;
			base_edge_type* cached_current = (base_edge_type*)current;

			while(current != nullptr)
			{
				global.edge_id = current->_edge_id;
				
				out_path.push_back(global);
				out_cost.push_back(current->_cost_from_origin);

				current = (base_edge_type*)current->came_from();

				cached_current->came_from(nullptr);

				cached_current = current;
			}
			
			std::reverse(out_path.begin(),out_path.end());
			std::reverse(out_cost.begin(),out_cost.end());

			total_cost = out_cost.back();

			// update start_ids/end_ids to includ final routed start/end
			start_ids.clear();
			start_ids.push_back(out_path.front());
			end_ids.clear();
			end_ids.push_back(out_path.back());
		}
		
		//since we used the graph stracture to store algorithm instance specific information, we need to reset the graph to te initial state
		for(auto itr = modified_edges.begin();itr!=modified_edges.end();itr++)
		{
			(*itr)->reset();
		}

		return total_cost;
	}

	template<typename MasterType,typename AgentType,typename GraphPoolType>
	static float A_Star_Tree(Routable_Agent<AgentType>* agent, Graph_Pool<GraphPoolType>* graph_pool, global_edge_id& start_id, unsigned int start_time, std::vector<float>& out_edge_costs)
	{
		typedef typename Graph_Pool<GraphPoolType>::base_edge_type base_edge_type;

		std::deque< base_edge_type* > modified_edges;
		
		boost::intrusive::multiset< base_edge_type > open_set;

		A_Star_Edge<base_edge_type>* start = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(start_id);
		if(start == nullptr){ THROW_WARNING("Origin: " << start_id.edge_id << " not found in graph pool!"); return 0.0f; }

		A_Star_Edge<base_edge_type>* end = nullptr;

		Routing_Data<base_edge_type> routing_data;

		routing_data.modified_edges = &modified_edges;
		routing_data.open_set = &open_set;
		routing_data.start_edge = (base_edge_type*)start;
		routing_data.end_edge = (base_edge_type*)end;
		routing_data.start_time = start_time;

		start->cost_from_origin(0.0f);
		start->time_label((float)start_time);

		float initial_estimated_cost_origin_destination = start->cost_from_origin();

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

		std::vector<base_edge_type*>* edges = graph_pool->Get_Edges(start_id.graph_id);

		for(auto itr = edges->begin();itr!=edges->end();itr++)
		{
			A_Star_Edge<base_edge_type>* current = (A_Star_Edge<base_edge_type>*)*itr;

			//current->
			out_edge_costs.push_back( (*itr)->estimated_cost_origin_destination() );
		}
		
		
		float total_cost = 0.0f;

		for(auto itr = modified_edges.begin();itr!=modified_edges.end();itr++)
		{
			(*itr)->reset();
		}

		return total_cost;
	}


	template<typename MasterType,typename AgentType,typename GraphPoolType>
	static float Time_Dependent_A_Star(Routable_Agent<AgentType>* agent, Graph_Pool<GraphPoolType>* graph_pool, std::vector<global_edge_id>& start_ids, std::vector<global_edge_id>& end_ids, unsigned int start_time, std::deque< global_edge_id >& out_path, std::deque< float >& out_cost, bool debug_route=false)
	{
		typedef typename Graph_Pool<GraphPoolType>::base_edge_type base_edge_type;

		std::deque< base_edge_type* > modified_edges;
		
		boost::intrusive::multiset< base_edge_type > open_set;

		std::vector<base_edge_type*> starts;
		A_Star_Edge<base_edge_type>* start;
		for (auto itr = start_ids.begin(); itr != start_ids.end(); ++itr)
		{
			start = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(*itr);
			if(start == nullptr){ THROW_WARNING("Origin: " << (*itr).edge_id << " not found in graph pool!"); return 0.0f; }
			starts.push_back((base_edge_type*)start);
		}
		base_edge_type* start_base = (base_edge_type*)start;

		std::vector<base_edge_type*> ends;
		A_Star_Edge<base_edge_type>* end;
		for (auto itr = end_ids.begin(); itr != end_ids.end(); ++itr)
		{
			end = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(*itr);
			if(end == nullptr){ THROW_WARNING("Destination: " << (*itr).edge_id << " not found in graph!"); return 0.0f; }
			ends.push_back((base_edge_type*)end);
		}
		base_edge_type* end_base = (base_edge_type*)end;

		Routing_Data<base_edge_type> routing_data;

		routing_data.modified_edges = &modified_edges;
		routing_data.open_set = &open_set;
		routing_data.start_edge = (base_edge_type*)starts.front();
		routing_data.end_edge = (base_edge_type*)ends.front();
		routing_data.start_time = start_time;

		for (auto itr = starts.begin(); itr != starts.end(); ++itr)
		{
			start = (A_Star_Edge<base_edge_type>*)(*itr);
			start->cost_from_origin(0.0f);
			start->time_from_origin(0.0f);
			start->time_label((float)start_time);

			float initial_estimated_cost_origin_destination = start->cost_from_origin() + agent->estimated_cost_between((base_edge_type*)start,(base_edge_type*)ends.front());

			start->estimated_cost_origin_destination( initial_estimated_cost_origin_destination );
		
			open_set.insert( *((base_edge_type*)start) );

			if( !start->marked_for_reset() )
			{
				modified_edges.push_back((base_edge_type*)start);
				start->marked_for_reset(true);
			}
		}
		
		bool success = false;

		while( open_set.size() )
		{
			A_Star_Edge<base_edge_type>* current = (A_Star_Edge<base_edge_type>*)&(*open_set.begin());
			
			//TODO: remove when done testing
			if (debug_route)
			{
				current->Display();
			}

			transit_edge_id id;
			
			id.id = current->edge_id();

			if( agent->at_destination((base_edge_type*)current, ends, &end_base) )
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

		//TODO: remove when done testing
		if (debug_route)
		{
			int test = 1;
		}

		
		global_edge_id global;
		global.graph_id = 0;

		float total_cost = 0.0f;

		if(success)
		{
			base_edge_type* current = end_base;//(base_edge_type*)end;
			base_edge_type* cached_current = (base_edge_type*)current;

			while(current != nullptr)
			{
				global.edge_id = current->_edge_id;
				
				out_path.push_back(global);
				out_cost.push_back(current->_cost_from_origin);

				current = (base_edge_type*)current->came_from();

				cached_current->came_from(nullptr);

				cached_current = current;
			}
			
			std::reverse(out_path.begin(),out_path.end());
			std::reverse(out_cost.begin(),out_cost.end());

			total_cost = out_cost.back();

			// update start_ids/end_ids to includ final routed start/end
			start_ids.clear();
			start_ids.push_back(out_path.front());
			end_ids.clear();
			end_ids.push_back(out_path.back());
		}
			
		for(auto itr = modified_edges.begin();itr!=modified_edges.end();itr++)
		{
			(*itr)->reset();
		}

		return total_cost;
	}

	template<typename MasterType, typename AgentType, typename GraphPoolType>
	static float Transit_A_Star(Routable_Agent<AgentType>* agent, Graph_Pool<GraphPoolType>* graph_pool, std::vector<global_edge_id>& start_ids, std::vector<global_edge_id>& end_ids, unsigned int start_time, std::deque< global_edge_id >& out_path, std::deque< float >& out_cost, bool debug_route = false)
	{
		typedef typename Graph_Pool<GraphPoolType>::base_edge_type base_edge_type;
		typedef Edge_Implementation<Routing_Components::Types::transit_attributes<MasterType>> transit_edge_type;

		typedef Network<typename MasterType::network_type> Network_Interface;
		typedef  Transit_Vehicle_Trip_Components::Prototypes::Transit_Vehicle_Trip<typename remove_pointer< typename Network_Interface::get_type_of(transit_vehicle_trips_container)::value_type>::type>  _Transit_Vehicle_Trip_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(transit_vehicle_trips_container), _Transit_Vehicle_Trip_Interface*> _Transit_Vehicle_Trips_Container_Interface;

		std::deque< base_edge_type* > modified_edges;

		std::deque<Link_Components::Types::Link_Type_Keys> out_type;
		std::deque<int> out_trip;
		std::deque<int> out_seq;

		boost::intrusive::multiset< base_edge_type > open_set;

		std::vector<base_edge_type*> starts;
		A_Star_Edge<base_edge_type>* start;
		for (auto itr = start_ids.begin(); itr != start_ids.end(); ++itr)
		{
			start = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(*itr);
			if (start == nullptr) { THROW_WARNING("Origin: " << (*itr).edge_id << " not found in graph pool!"); return 0.0f; }
			starts.push_back((base_edge_type*)start);
		}
		base_edge_type* start_base = (base_edge_type*)start;

		std::vector<base_edge_type*> ends;
		A_Star_Edge<base_edge_type>* end;
		for (auto itr = end_ids.begin(); itr != end_ids.end(); ++itr)
		{
			end = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(*itr);
			if (end == nullptr) { THROW_WARNING("Destination: " << (*itr).edge_id << " not found in graph!"); return 0.0f; }
			ends.push_back((base_edge_type*)end);
		}
		base_edge_type* end_base = (base_edge_type*)end;

		Routing_Data<base_edge_type> routing_data;

		routing_data.modified_edges = &modified_edges;
		routing_data.open_set = &open_set;
		routing_data.start_edge = (base_edge_type*)starts.front();
		routing_data.end_edge = (base_edge_type*)ends.front();
		routing_data.start_time = start_time;

		for (auto itr = starts.begin(); itr != starts.end(); ++itr)
		{
			start = (A_Star_Edge<base_edge_type>*)(*itr);
			start->cost_from_origin(0.0f);
			start->time_from_origin(0.0f);
			start->time_label((float)start_time);

			float initial_estimated_cost_origin_destination = start->cost_from_origin() + agent->estimated_cost_between((base_edge_type*)start, (base_edge_type*)ends.front());

			start->estimated_cost_origin_destination(initial_estimated_cost_origin_destination);

			open_set.insert(*((base_edge_type*)start));

			if (!start->marked_for_reset())
			{
				modified_edges.push_back((base_edge_type*)start);
				start->marked_for_reset(true);
			}
		}

		bool success = false;

		while (open_set.size())
		{
			A_Star_Edge<base_edge_type>* current = (A_Star_Edge<base_edge_type>*)&(*open_set.begin());

			//TODO: remove when done testing
			if (debug_route)
			{
				current->Display();
			}

			transit_edge_id id;

			id.id = current->edge_id();

			if (agent->at_destination((base_edge_type*)current, ends, &end_base))
			{
				success = true;
				break;
			}

			open_set.erase(open_set.iterator_to(*((base_edge_type*)current)));

			current->in_open_set(false);
			current->in_closed_set(true);

			Anonymous_Connection_Group<MasterType, base_edge_type>* connection_set_iterator = current->begin_connection_groups();
			const Anonymous_Connection_Group<MasterType, base_edge_type>* const connection_set_end = current->end_connection_groups();

			while (connection_set_iterator != connection_set_end)
			{
				connection_set_iterator = connection_set_iterator->Visit_Transit_Neighbors(agent, current, routing_data, graph_pool);
			}

		}

		//TODO: remove when done testing
		if (debug_route)
		{
			int test = 1;
		}


		global_edge_id global;
		global.graph_id = 1;
		_Transit_Vehicle_Trip_Interface* current_trip;

		float total_cost = 0.0f;		

		if (success)
		{
			//base_edge_type* current = end_base;//(base_edge_type*)end;

			global.edge_id = end_base->_edge_id;
			global.graph_id = 1;

			transit_edge_type* current = (transit_edge_type*)graph_pool->Get_Edge(global);
			transit_edge_type* cached_current = (transit_edge_type*)current;
			
			while (current != nullptr)
			{
				global.edge_id = current->_edge_id;

				out_path.push_back(global);
				out_cost.push_back(current->_cost_from_origin);
				out_type.push_back(current->_edge_type);
				out_seq.push_back(current->_came_on_seq_index);

				Link_Components::Types::Link_Type_Keys current_type = current->_edge_type;
				if (current_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
				{
					current_trip = (_Transit_Vehicle_Trip_Interface*)current->_came_on_trip;
					out_trip.push_back(current_trip->_uuid);
				}
				else
				{
					out_trip.push_back(-1);
				}				

				current = (transit_edge_type*)current->came_from();
				cached_current->came_from(nullptr);
				cached_current = current;
			}

			std::reverse(out_path.begin(), out_path.end());
			std::reverse(out_cost.begin(), out_cost.end());
			std::reverse(out_type.begin(), out_type.end());
			std::reverse(out_trip.begin(), out_trip.end());
			std::reverse(out_seq.begin(), out_seq.end());

			total_cost = out_cost.back();

			// update start_ids/end_ids to includ final routed start/end
			start_ids.clear();
			start_ids.push_back(out_path.front());
			end_ids.clear();
			end_ids.push_back(out_path.back());
		}

		for (auto itr = modified_edges.begin(); itr != modified_edges.end(); itr++)
		{
			(*itr)->reset();
		}

		return total_cost;
	}
}