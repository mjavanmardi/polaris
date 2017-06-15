#pragma once
#include "Graph_Implementation.h"

namespace polaris
{
	//this is not a descriptive name, this is used for any ID
	union multimodal_edge_id
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

			multimodal_edge_id id;
			
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

			multimodal_edge_id id;
			
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
	static float Multimodal_A_Star(Routable_Agent<AgentType>* agent, Graph_Pool<GraphPoolType>* graph_pool, std::vector<global_edge_id>& start_ids, std::vector<global_edge_id>& end_ids, unsigned int start_time, std::deque< global_edge_id >& out_path, std::deque< float >& out_cost, bool debug_route = false)
	{
		typedef typename Graph_Pool<GraphPoolType>::base_edge_type base_edge_type;
		typedef Edge_Implementation<Routing_Components::Types::multimodal_attributes<MasterType>> multimodal_edge_type;

		typedef Network_Components::Prototypes::Network<typename MasterType::network_type> Network_Interface;
		Network_Interface* net = (Network_Interface*)_global_network;
		//Network_Interface* net = (Network_Interface*)Allocate<typename MasterType::network_type>();

		typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface; 
		
		typedef  Transit_Vehicle_Trip_Components::Prototypes::Transit_Vehicle_Trip<typename remove_pointer< typename Network_Interface::get_type_of(transit_vehicle_trips_container)::value_type>::type>  _Transit_Vehicle_Trip_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(transit_vehicle_trips_container), _Transit_Vehicle_Trip_Interface*> _Transit_Vehicle_Trips_Container_Interface;

		typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
		_Scenario_Interface*_scenario_reference = net->scenario_reference<_Scenario_Interface*>();
		
		std::ofstream sp_file;
		char myLine[2000];
		std::string myParagraph;

		if (debug_route)
		{
			// Initialize executed activities file
			stringstream sp_filename("");
			sp_filename << _scenario_reference->template output_dir_name<string>();
			sp_filename << "sp_output.dat";
			sp_file.open(sp_filename.str(), std::ofstream::out | std::ofstream::app);
			/*if (!this->sp_file.is_open())THROW_EXCEPTION("ERROR: executed activity distribution file could not be created.");*/
			//sp_file.open("sp_output.dat", std::ofstream::out | std::ofstream::app);
		}

		std::deque< base_edge_type* > modified_edges;

		std::deque<Link_Components::Types::Link_Type_Keys> out_type;
		std::deque<int> out_trip;
		std::deque<int> out_seq;
		std::deque<float> out_time;
		std::deque<float> out_arr_time;
		std::deque<float> out_wait_time;
		std::deque<float> out_walk_time;
		std::deque<float> out_ivt_time;
		std::deque<float> out_car_time;
		std::deque<int> out_wait_count;
		std::deque<float> out_transfer_pen;
		std::deque<float> out_est_cost;

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

			global_edge_id start_g;
			start_g.graph_id = 1;
			start_g.edge_id = start->_edge_id;

			multimodal_edge_type* start_t = (multimodal_edge_type*)graph_pool->Get_Edge(start_g);

			start_t->_came_on_seq_index = 0;
			start_t->_came_on_trip = nullptr();
			start_t->_wait_count_from_origin = 0;
			start_t->_wait_time_from_origin = 0;
			start_t->_walk_time_from_origin = 0;
			start_t->_ivt_time_from_origin = 0;
			start_t->_car_time_from_origin = 0;
			start_t->_transfer_pen_from_origin = 0;

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
			/*if (debug_route)
			{
				current->Display();
			}*/

			multimodal_edge_id id;

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
				connection_set_iterator = connection_set_iterator->Visit_Multimodal_Neighbors(agent, current, routing_data, graph_pool);
			}

		}

		//TODO: remove when done testing
		/*if (debug_route)
		{
			int test = 1;
		}*/


		global_edge_id global;
		global.graph_id = 1;
		_Transit_Vehicle_Trip_Interface* current_trip;

		float total_cost = 0.0f;
		float travel_time = 0.0f;
				
		if (success)
		{
			//base_edge_type* current = end_base;//(base_edge_type*)end;

			global.edge_id = end_base->_edge_id;
			global.graph_id = 1;

			multimodal_edge_type* current = (multimodal_edge_type*)graph_pool->Get_Edge(global);
			multimodal_edge_type* cached_current = (multimodal_edge_type*)current;

			multimodal_edge_type* intermediate_current;
			_Link_Interface* intermediate_link;

			multimodal_edge_type* target_current;
			_Link_Interface* target_link;
			
			if (debug_route)
			{
				sp_file << "\nsuccess";
			}

			while (current != nullptr)
			{
				global.edge_id = current->_edge_id;

				out_path.push_back(global);
				out_cost.push_back(current->_cost_from_origin);
				out_type.push_back(current->_edge_type);
				out_seq.push_back(current->_came_on_seq_index);

				_Link_Interface* current_link = net->template get_link_ptr<typename MasterType::link_type>(global.edge_id);
				
				out_time.push_back(current->_time_from_origin); 
				out_arr_time.push_back(current->_time_label);
				out_wait_count.push_back(current->_wait_count_from_origin);
				out_wait_time.push_back(current->_wait_time_from_origin);
				out_walk_time.push_back(current->_walk_time_from_origin);
				out_ivt_time.push_back(current->_ivt_time_from_origin);
				out_car_time.push_back(current->_car_time_from_origin);
				out_transfer_pen.push_back(current->_transfer_pen_from_origin);
				out_est_cost.push_back(current->_estimated_cost_origin_destination);

				Link_Components::Types::Link_Type_Keys current_type = current->_edge_type;
				if (current_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
				{
					current_trip = (_Transit_Vehicle_Trip_Interface*)current->_came_on_trip;
					out_trip.push_back(current_trip->_uuid);

					if (debug_route)
					{
						sprintf_s(myLine, "\n%s\t%s\t%f\t%s\t%d\t%s\t%f\t%f\t%d\t%f\t%f\t%f\t%f\t%f\t%f",
							current_link->_upstream_intersection->_dbid.c_str(),
							current_link->_downstream_intersection->_dbid.c_str(),
							current->_cost_from_origin,
							current_trip->_dbid.c_str(),
							current->_came_on_seq_index,
							"TRANSIT",
							current->_time_from_origin,
							current->_time_label,
							current->_wait_count_from_origin,
							current->_wait_time_from_origin,
							current->_walk_time_from_origin,
							current->_ivt_time_from_origin,
							current->_car_time_from_origin,
							current->_transfer_pen_from_origin,
							current->_estimated_cost_origin_destination);
						myParagraph.insert(0, myLine);
					}

					target_current = (multimodal_edge_type*)current->came_from();
					global.edge_id = target_current->_edge_id;
					target_link = net->template get_link_ptr<typename MasterType::link_type>(global.edge_id);

					int mySeq = current->_came_on_seq_index - 1;
					if (mySeq >= 0 && current_link->_upstream_intersection->_dbid != target_link->_downstream_intersection->_dbid && mySeq >= 0)
					{						
						intermediate_link = (_Link_Interface*)current_trip->_pattern->_pattern_links.at(mySeq);
						global.edge_id = intermediate_link->_uuid;
						intermediate_current = (A_Star_Edge<multimodal_edge_type>*)graph_pool->Get_Edge(global);

						while (intermediate_link->_downstream_intersection->_dbid != target_link->_downstream_intersection->_dbid && mySeq >= 0)
						{
							if (debug_route)
							{
								sprintf_s(myLine, "\n%s\t%s\t%f\t%s\t%d\t%s\t%f\t%f\t%d\t%f\t%f\t%f\t%f\t%f\t%f",
									intermediate_link->_upstream_intersection->_dbid.c_str(),
									intermediate_link->_downstream_intersection->_dbid.c_str(),
									0, //intermediate_current->_cost_from_origin,
									current_trip->_dbid.c_str(),
									mySeq,
									"TRANSIT",
									0, //intermediate_current->_time_from_origin,
									0, //intermediate_current->_time_label,
									current->_wait_count_from_origin,
									0, //intermediate_current->_wait_time_from_origin,
									0, //intermediate_current->_walk_time_from_origin,
									0, //intermediate_current->_ivt_time_from_origin,
									0, //intermediate_current->_car_time_from_origin,
									0, //intermediate_current->_transfer_pen_from_origin,
									0); //intermediate_current->_estimated_cost_origin_destination);
								myParagraph.insert(0, myLine);
							}

							mySeq = mySeq - 1;
							if (mySeq >= 0)
							{
								intermediate_link = (_Link_Interface*)current_trip->_pattern->_pattern_links.at(mySeq);
								global.edge_id = intermediate_link->_uuid;
								intermediate_current = (A_Star_Edge<multimodal_edge_type>*)graph_pool->Get_Edge(global);
							}
						}
					}

					
				}
				
				else if (current_type == Link_Components::Types::Link_Type_Keys::WALK)
				{
					out_trip.push_back(-1);

					if (debug_route)
					{
						sprintf_s(myLine, "\n%s\t%s\t%f\t%s\t%d\t%s\t%f\t%f\t%d\t%f\t%f\t%f\t%f\t%f\t%f",
							current_link->_upstream_intersection->_dbid.c_str(),
							current_link->_downstream_intersection->_dbid.c_str(),
							current->_cost_from_origin,
							"0",
							current->_came_on_seq_index,
							"WALK",
							current->_time_from_origin,
							current->_time_label,
							current->_wait_count_from_origin,
							current->_wait_time_from_origin,
							current->_walk_time_from_origin,
							current->_ivt_time_from_origin,
							current->_car_time_from_origin,
							current->_transfer_pen_from_origin,
							current->_estimated_cost_origin_destination);
						myParagraph.insert(0, myLine);
					}

				}	

				else
				{
					out_trip.push_back(-1);

					if (debug_route)
					{
						sprintf_s(myLine, "\n%s\t%s\t%f\t%s\t%d\t%s\t%f\t%f\t%d\t%f\t%f\t%f\t%f\t%f\t%f",
							current_link->_upstream_intersection->_dbid.c_str(),
							current_link->_downstream_intersection->_dbid.c_str(),
							current->_cost_from_origin,
							"0",
							current->_came_on_seq_index,
							"DRIVE",
							current->_time_from_origin,
							current->_time_label,
							current->_wait_count_from_origin,
							current->_wait_time_from_origin,
							current->_walk_time_from_origin,
							current->_ivt_time_from_origin,
							current->_car_time_from_origin,
							current->_transfer_pen_from_origin,
							current->_estimated_cost_origin_destination);
						myParagraph.insert(0, myLine);
					}

				}

				current = (multimodal_edge_type*)current->came_from();
				cached_current->came_from(nullptr);
				cached_current = current;

			}

			if (debug_route)
			{
				sprintf_s(myLine, "\nNode_A\tNode_B\tGen_Cost\tTrip_ID\tSequence\tType\tTime\tArr_Time\tWait_Count\tWait_Time\tWalk_Time\tIVTT\tCar_Time\tTransfer_Pen\tEst_Cost");
				myParagraph.insert(0, myLine);
				//sprintf_s(myLine, "TOD:\t%d-%d-%d", start, end, start_time);
				//sp_file << "Origin:\t" << start << "\tDestination:\t" << end << "Departure:\t" << start_time << endl;
				//Our_Line.insert(0, myLine);
				sp_file << myParagraph << endl;
			}

			std::reverse(out_path.begin(), out_path.end());
			std::reverse(out_cost.begin(), out_cost.end());
			std::reverse(out_type.begin(), out_type.end());
			std::reverse(out_trip.begin(), out_trip.end());
			std::reverse(out_seq.begin(), out_seq.end());
			std::reverse(out_time.begin(), out_time.end());
			std::reverse(out_arr_time.begin(), out_arr_time.end());
			std::reverse(out_wait_count.begin(), out_wait_count.end());
			std::reverse(out_wait_time.begin(), out_wait_time.end());
			std::reverse(out_walk_time.begin(), out_walk_time.end());
			std::reverse(out_ivt_time.begin(), out_ivt_time.end());
			std::reverse(out_car_time.begin(), out_car_time.end());
			std::reverse(out_transfer_pen.begin(), out_transfer_pen.end());
			std::reverse(out_est_cost.begin(), out_est_cost.end());
			
			total_cost = out_cost.back();
			travel_time = out_time.back();

			// update start_ids/end_ids to includ final routed start/end
			start_ids.clear();
			start_ids.push_back(out_path.front());
			end_ids.clear();
			end_ids.push_back(out_path.back());
		}
		else
		{
			sp_file << "fail" << endl;
		}
		sp_file.close();

		for (auto itr = modified_edges.begin(); itr != modified_edges.end(); itr++)
		{
			(*itr)->reset();
		}

		//return total_cost;
		return travel_time;
	}
}