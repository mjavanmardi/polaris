#pragma once
#include "Graph_Implementation.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

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
	static float A_Star(Routable_Agent<AgentType>* agent, Graph_Pool<GraphPoolType>* graph_pool, std::vector<global_edge_id>& start_ids, std::vector<global_edge_id>& end_ids, unsigned int start_time, std::deque< global_edge_id >& out_path, std::deque< float >& out_cost, unsigned int origin_loc_id, unsigned int destination_loc_id, bool debug_route, std::string& summary_paragraph )
	{
		typedef typename Graph_Pool<GraphPoolType>::base_edge_type base_edge_type;
		
		int graph_id = start_ids.front().graph_id;

		char myLine[2000];
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
			end->_cost = 0;
			end->_time_cost = 0;
			if(end == nullptr){ THROW_WARNING("Destination: " << (*itr).edge_id << " not found in graph!"); return 0.0f; }
			ends.push_back((base_edge_type*)end);
		}
		base_edge_type* end_base = (base_edge_type*)end;

		Routing_Data<base_edge_type> routing_data;

		routing_data.modified_edges = &modified_edges;
		routing_data.open_set = &open_set;
		routing_data.start_edge = (base_edge_type*)starts.front();
		routing_data.end_edge = (base_edge_type*)ends.front();
		routing_data.ends = &ends;
		routing_data.start_time = start_time;

		for (auto itr = starts.begin(); itr != starts.end(); ++itr)
		{
			start = (A_Star_Edge<base_edge_type>*)(*itr);
			//start->cost_from_origin(0.0f);
			//start->time_from_origin(0.0f);
			//start->time_label((float)start_time);
			start->cost_from_origin(start->_cost);
			start->time_from_origin(start->_time_cost);
			start->time_label((float)start_time + start->_time_cost);
			

			float initial_estimated_cost_origin_destination = start->cost_from_origin() + agent->estimated_cost_between((base_edge_type*)start, ends);

			start->estimated_cost_origin_destination( initial_estimated_cost_origin_destination );
		
			open_set.insert( *((base_edge_type*)start) );

			if( !start->marked_for_reset() )
			{
				modified_edges.push_back((base_edge_type*)start);
				start->marked_for_reset(true);
			}
		}
		
		bool success = false;
		int scanCount = 0;
		while( open_set.size() )
		{
			A_Star_Edge<base_edge_type>* current = (A_Star_Edge<base_edge_type>*)&(*open_set.begin());
			++scanCount;
			
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
		global.graph_id = graph_id;

		float total_cost = FLT_MAX;

		if(success)
		{
			base_edge_type* current = end_base;//(base_edge_type*)end;
			base_edge_type* cached_current = (base_edge_type*)current;

			if (debug_route)
			{
				sprintf_s(myLine, "%d\t%d\t%d\t%f\t%f\t%f\t%f\t%d\n",
					origin_loc_id,
					destination_loc_id,
					start_time,
					current->_time_label,
					current->_cost_from_origin,
					current->_time_from_origin,
					current->_estimated_cost_origin_destination,
					scanCount);
				summary_paragraph.insert(0, myLine);
			}

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

			for (auto itr = end_ids.begin(); itr != end_ids.end(); ++itr)
			{
				end = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(*itr);
				end->_cost = end->_cost_backup;
				end->_time_cost = end->_time_cost_backup;
			}

			// update start_ids/end_ids to includ final routed start/end
			start_ids.clear();
			start_ids.push_back(out_path.front());
			end_ids.clear();
			end_ids.push_back(out_path.back());
		}
		else
		{
			for (auto itr = end_ids.begin(); itr != end_ids.end(); ++itr)
			{
				end = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(*itr);
				end->_cost = end->_cost_backup;
				end->_time_cost = end->_time_cost_backup;
			}

			if (debug_route)
			{
				sprintf_s(myLine, "%d\t%d\t%d\t%f\t%f\t%f\t%f\t%d\n",
					origin_loc_id,
					destination_loc_id,
					start_time,
					864000.0,
					864000.0,
					864000.0,
					864000.0,
					scanCount);
				summary_paragraph.insert(0, myLine);
			}
		}
		
		//since we used the graph stracture to store algorithm instance specific information, we need to reset the graph to te initial state
		for(auto itr = modified_edges.begin();itr!=modified_edges.end();itr++)
		{
			(*itr)->reset();
		}

		return total_cost;
	}

	template<typename MasterType, typename AgentType, typename GraphPoolType>
	static float A_Star_Tree(Routable_Agent<AgentType>* agent, Graph_Pool<GraphPoolType>* graph_pool, global_edge_id& start_id, unsigned int start_time, std::vector<float>& out_edge_costs)
	{
		typedef typename Graph_Pool<GraphPoolType>::base_edge_type base_edge_type;

		std::deque< base_edge_type* > modified_edges;

		boost::intrusive::multiset< base_edge_type > open_set;

		int graph_id = start_id.graph_id;

		A_Star_Edge<base_edge_type>* start = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(start_id);
		if (start == nullptr) { THROW_WARNING("Origin: " << start_id.edge_id << " not found in graph pool!"); return 0.0f; }

		A_Star_Edge<base_edge_type>* end = nullptr;

		Routing_Data<base_edge_type> routing_data;

		routing_data.modified_edges = &modified_edges;
		routing_data.open_set = &open_set;
		routing_data.start_edge = (base_edge_type*)start;
		routing_data.end_edge = (base_edge_type*)end;
		routing_data.start_time = start_time;

		//start->cost_from_origin(0.0f);
		//start->time_label((float)start_time);
		start->cost_from_origin(start->_cost);
		start->time_from_origin(start->_time_cost);
		start->time_label((float)start_time + start->_time_cost);

		float initial_estimated_cost_origin_destination = start->cost_from_origin();

		start->estimated_cost_origin_destination(initial_estimated_cost_origin_destination);

		open_set.insert(*((base_edge_type*)start));

		if (!start->marked_for_reset())
		{
			modified_edges.push_back((base_edge_type*)start);
			start->marked_for_reset(true);
		}

		bool success = false;

		while (open_set.size())
		{
			A_Star_Edge<base_edge_type>* current = (A_Star_Edge<base_edge_type>*)&(*open_set.begin());

			if (agent->at_destination((base_edge_type*)current, (base_edge_type*)end))
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
				connection_set_iterator = connection_set_iterator->Visit_Neighbors(agent, current, routing_data);
			}
		}

		std::vector<base_edge_type*>* edges = graph_pool->Get_Edges(graph_id);

		for (auto itr = edges->begin(); itr != edges->end(); itr++)
		{
			A_Star_Edge<base_edge_type>* current = (A_Star_Edge<base_edge_type>*)*itr;

			//current->
			out_edge_costs.push_back((*itr)->estimated_cost_origin_destination());
		}


		float total_cost = 0.0f;

		for (auto itr = modified_edges.begin(); itr != modified_edges.end(); itr++)
		{
			(*itr)->reset();
		}

		return total_cost;
	}
	
	template<typename MasterType, typename AgentType, typename GraphPoolType>
	static float Dijkstra_Tree(Routable_Agent<AgentType>* agent, Graph_Pool<GraphPoolType>* graph_pool, std::vector<global_edge_id>& start_ids, int zone_index, bool debug_route, std::string& summary_paragraph)
	{
		typedef typename Graph_Pool<GraphPoolType>::base_edge_type base_edge_type;
		
		typedef Network_Components::Prototypes::Network<typename MasterType::network_type> Network_Interface;
		Network_Interface* net = (Network_Interface*)_global_network;
		
		typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
		_Scenario_Interface*_scenario_reference = net->scenario_reference<_Scenario_Interface*>();

		typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface;

		typedef Random_Access_Sequence<typename _Link_Interface::get_type_of(heur_cost_to_dest)> _Heuristic_Cost_Container_Interface;
		
		int graph_id = start_ids.front().graph_id;

		//char myLine[2000];
		/*Counter A_Star_Time;
		if (debug_route)
		{
			A_Star_Time.Start();
		}*/

		std::deque< base_edge_type* > modified_edges;

		boost::intrusive::multiset< base_edge_type > open_set;

		std::vector<base_edge_type*> starts;
		A_Star_Edge<base_edge_type>* start;
		for (auto itr = start_ids.begin(); itr != start_ids.end(); ++itr)
		{
			start = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(*itr);
			if (start == nullptr) { THROW_WARNING("Origin: " << (*itr).edge_id << " not found in graph pool!"); return 0.0f; }
			starts.push_back((base_edge_type*)start);
		}

		Routing_Data<base_edge_type> routing_data;

		routing_data.modified_edges = &modified_edges;
		routing_data.open_set = &open_set;
		routing_data.start_edge = (base_edge_type*)starts.front();
		
		for (auto itr = starts.begin(); itr != starts.end(); ++itr)
		{
			start = (A_Star_Edge<base_edge_type>*)(*itr);

			_Link_Interface* start_link = (_Link_Interface*)start->_source_link;
			start->cost_from_origin(start_link->min_multi_modal_cost<float>());
			//start->cost_from_origin(start->_min_multi_modal_cost);
			
			float initial_estimated_cost_origin_destination = start->cost_from_origin();

			start->estimated_cost_origin_destination(initial_estimated_cost_origin_destination);

			open_set.insert(*((base_edge_type*)start));

			if (!start->marked_for_reset())
			{
				modified_edges.push_back((base_edge_type*)start);
				start->marked_for_reset(true);
			}
		}

		bool success = false;
		int scanCount = 0;
		while (open_set.size())
		{
			A_Star_Edge<base_edge_type>* current = (A_Star_Edge<base_edge_type>*)&(*open_set.begin());
			++scanCount;

			open_set.erase(open_set.iterator_to(*((base_edge_type*)current)));

			current->in_open_set(false);
			current->in_closed_set(true);

			Anonymous_Connection_Group<MasterType, base_edge_type>* connection_set_iterator = current->begin_connection_groups();
			const Anonymous_Connection_Group<MasterType, base_edge_type>* const connection_set_end = current->end_connection_groups();

			while (connection_set_iterator != connection_set_end)
			{
				connection_set_iterator = connection_set_iterator->Visit_Neighbors(agent, current, routing_data);
			}

		}			

		float total_cost = 0;

		/*if (debug_route)
		{
			float perf_time = A_Star_Time.Stop();
			sprintf_s(myLine, "\n%s\t%s\t%d\t%s\t%d\t%s\t%f",
				"success",
				"zone:",
				zone,
				"scanScount:",
				scanCount,
				"Router run-time (ms):",
				perf_time
				);
			summary_paragraph.insert(0, myLine);			
		}*/

		global_edge_id current_g;
		current_g.graph_id = graph_id;
		std::vector<base_edge_type*>* edges = graph_pool->Get_Edges(graph_id);
		for (auto itr = edges->begin(); itr != edges->end(); itr++)
		{
			A_Star_Edge<base_edge_type>* current = (A_Star_Edge<base_edge_type>*)*itr;	
			_Link_Interface* current_link = (_Link_Interface*)current->_source_link;	
			current_link->heur_cost_to_dest<_Heuristic_Cost_Container_Interface&>()[zone_index] = current->_cost_from_origin;
		}

		for (auto itr = modified_edges.begin(); itr != modified_edges.end(); itr++)
		{
			(*itr)->reset();
		}

		return total_cost;
	}
	
	template<typename MasterType, typename AgentType, typename GraphPoolType>
	static float Dijkstra_Walk(Routable_Agent<AgentType>* agent, Graph_Pool<GraphPoolType>* graph_pool, global_edge_id& start_id, bool debug_route = false)
	{
		typedef typename Graph_Pool<GraphPoolType>::base_edge_type base_edge_type;

		typedef Network_Components::Prototypes::Network<typename MasterType::network_type> Network_Interface;
		Network_Interface* net = (Network_Interface*)_global_network;

		typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
		_Scenario_Interface*_scenario_reference = net->scenario_reference<_Scenario_Interface*>();

		typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface;

		std::ofstream perf_file;
		std::string myParagraph;
		Counter A_Star_Time;
		if (debug_route)
		{
			stringstream perf_filename("");
			perf_filename << _scenario_reference->template output_dir_name<string>();
			perf_filename << "walk_perf_output.dat";
			perf_file.open(perf_filename.str(), std::ofstream::out | std::ofstream::app);

			A_Star_Time.Start();
		}

		std::deque< base_edge_type* > modified_edges;
		boost::intrusive::multiset< base_edge_type > open_set;

		A_Star_Edge<base_edge_type>* start;		
		start = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(start_id);
				
		_Link_Interface* start_link = (_Link_Interface*)start->_source_link;
		start->cost_from_origin(start_link->walk_length<float>());
		//start->cost_from_origin(start->_walk_length);

		float initial_estimated_cost_origin_destination = start->cost_from_origin();

		start->estimated_cost_origin_destination(initial_estimated_cost_origin_destination);

		open_set.insert(*((base_edge_type*)start));
		if (!start->marked_for_reset())
		{
			modified_edges.push_back((base_edge_type*)start);
			start->marked_for_reset(true);
		}		

		Routing_Data<base_edge_type> routing_data;
		routing_data.modified_edges = &modified_edges;
		routing_data.open_set = &open_set;
		routing_data.start_edge = start;

		bool success = false;
		int scanCount = 0;
		A_Star_Edge<base_edge_type>* current;
		while (open_set.size())
		{
			current = (A_Star_Edge<base_edge_type>*)&(*open_set.begin());
			++scanCount;

			open_set.erase(open_set.iterator_to(*((base_edge_type*)current)));
			_Link_Interface* current_link = (_Link_Interface*)current->_source_link;
			if (current_link->template touch_transit<bool>())
			{
				success = true;
				break;
			}

			current->in_open_set(false);
			current->in_closed_set(true);

			Anonymous_Connection_Group<MasterType, base_edge_type>* connection_set_iterator = current->begin_connection_groups();
			const Anonymous_Connection_Group<MasterType, base_edge_type>* const connection_set_end = current->end_connection_groups();

			while (connection_set_iterator != connection_set_end)
			{
				connection_set_iterator = connection_set_iterator->Visit_Neighbors(agent, current, routing_data);
			}

		}

		float total_cost = 0;
		
		if (success)
		{
			start_link->template walk_distance_to_transit<float>(current->_cost_from_origin);
			if (debug_route)
			{
				perf_file << "Link_ID:\t" << start_link->dbid<int>();
				perf_file << "\tDistance:\t" << start_link->walk_distance_to_transit<float>();
				perf_file << "\tsuccess\tscanScount:\t" << scanCount;
				perf_file << "\tRouter run-time (ms):\t" << A_Star_Time.Stop() << endl;
			}
		}
		else
		{
			if (debug_route)
			{
				perf_file << "Link_ID:\t" << start_link->dbid<int>();
				perf_file << "\tDistance:\t" << start_link->walk_distance_to_transit<float>(); 
				perf_file << "fail\tscanScount:\t" << scanCount;
				perf_file << "\tRouter run-time (ms):\t" << A_Star_Time.Stop() << endl;
			}
		}		

		for (auto itr = modified_edges.begin(); itr != modified_edges.end(); itr++)
		{
			(*itr)->reset();
		}

		return total_cost;
	}

	template<typename MasterType, typename AgentType, typename GraphPoolType>
	static float Dijkstra_Drive(Routable_Agent<AgentType>* agent, Graph_Pool<GraphPoolType>* graph_pool, global_edge_id& start_id, bool debug_route = false)
	{
		typedef typename Graph_Pool<GraphPoolType>::base_edge_type base_edge_type;

		typedef Network_Components::Prototypes::Network<typename MasterType::network_type> Network_Interface;
		Network_Interface* net = (Network_Interface*)_global_network;

		typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
		_Scenario_Interface*_scenario_reference = net->scenario_reference<_Scenario_Interface*>();

		typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
		typedef Intersection_Components::Prototypes::Intersection<typename _Link_Interface::get_type_of(upstream_intersection)> _Intersection_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface;

		std::ofstream perf_file;
		std::string myParagraph;
		Counter A_Star_Time;
		if (debug_route)
		{
			stringstream perf_filename("");
			perf_filename << _scenario_reference->template output_dir_name<string>();
			perf_filename << "drive_perf_output.dat";
			perf_file.open(perf_filename.str(), std::ofstream::out | std::ofstream::app);

			A_Star_Time.Start();
		}

		std::deque< base_edge_type* > modified_edges;
		boost::intrusive::multiset< base_edge_type > open_set;

		A_Star_Edge<base_edge_type>* start;
		start = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(start_id);

		_Link_Interface* start_link = (_Link_Interface*)start->_source_link;
		start->cost_from_origin(start_link->template drive_time<float>() );
		//start->cost_from_origin(start->_drive_time);

		float initial_estimated_cost_origin_destination = start->cost_from_origin();

		start->estimated_cost_origin_destination(initial_estimated_cost_origin_destination);

		open_set.insert(*((base_edge_type*)start));
		if (!start->marked_for_reset())
		{
			modified_edges.push_back((base_edge_type*)start);
			start->marked_for_reset(true);
		}

		Routing_Data<base_edge_type> routing_data;
		routing_data.modified_edges = &modified_edges;
		routing_data.open_set = &open_set;
		routing_data.start_edge = start;

		bool success = false;
		int scanCount = 0;
		A_Star_Edge<base_edge_type>* current;
		while (open_set.size())
		{
			current = (A_Star_Edge<base_edge_type>*)&(*open_set.begin());
			++scanCount;

			open_set.erase(open_set.iterator_to(*((base_edge_type*)current)));
			_Link_Interface* current_link = (_Link_Interface*)current->_source_link;
			if (current_link->template touch_transit<bool>())
			{
				success = true;
				break;
			}

			current->in_open_set(false);
			current->in_closed_set(true);

			Anonymous_Connection_Group<MasterType, base_edge_type>* connection_set_iterator = current->begin_connection_groups();
			const Anonymous_Connection_Group<MasterType, base_edge_type>* const connection_set_end = current->end_connection_groups();

			while (connection_set_iterator != connection_set_end)
			{
				connection_set_iterator = connection_set_iterator->Visit_Neighbors(agent, current, routing_data);
			}

		}

		float total_cost = 0;

		if (success)
		{
			start_link-> template drive_fft_to_transit<float>(current->_cost_from_origin);
			if (debug_route)
			{
				perf_file << "Link_ID:\t" << start_link->dbid<int>();
				perf_file << "\tDistance:\t" << start_link->drive_fft_to_transit<float>();
				perf_file << "success\tscanScount:\t" << scanCount;
				perf_file << "\tRouter run-time (ms):\t" << A_Star_Time.Stop() << endl;
			}
		}
		else
		{
			if (debug_route)
			{
				perf_file << "Link_ID:\t" << start_link->dbid<int>();
				perf_file << "\tDistance:\t" << start_link->drive_fft_to_transit<float>();
				perf_file << "fail\tscanScount:\t" << scanCount;
				perf_file << "\tRouter run-time (ms):\t" << A_Star_Time.Stop() << endl;
			}
		}

		for (auto itr = modified_edges.begin(); itr != modified_edges.end(); itr++)
		{
			(*itr)->reset();
		}

		return total_cost;
	}

	template<typename MasterType, typename AgentType, typename GraphPoolType>
	static float Multimodal_A_Star(
		Routable_Agent<AgentType>* agent, 
		Graph_Pool<GraphPoolType>* graph_pool, 
		std::vector<global_edge_id>& start_ids, 
		std::vector<global_edge_id>& end_ids,
		/*std::vector<global_edge_id>& tr_end_ids,*/ 
		unsigned int start_time, 
		std::deque< global_edge_id >& out_path, 
		std::deque< float >& out_cost, 
		std::deque<Link_Components::Types::Link_Type_Keys>& out_type,
		std::deque<int>& out_trip,
		std::deque<int>& out_seq,
		std::deque<float>& out_time,
		std::deque<float>& out_arr_time,
		std::deque<float>& out_wait_time,
		std::deque<float>& out_walk_time,
		std::deque<float>& out_ivt_time,
		std::deque<float>& out_car_time,
		std::deque<int>& out_wait_count,
		std::deque<float>& out_transfer_pen,
		std::deque<float>& out_heur_cost,	
		__int64& astar_time, 
		unsigned int origin_loc_id,
		unsigned int destination_loc_id,
		bool debug_route,
		std::string& summary_paragraph,
		std::string& detail_paragraph,
		Vehicle_Components::Types::Vehicle_Type_Keys sub_mode)
	{
		typedef typename Graph_Pool<GraphPoolType>::base_edge_type base_edge_type;
		typedef Edge_Implementation<Routing_Components::Types::multimodal_attributes<MasterType>> multimodal_edge_type;

		typedef Network_Components::Prototypes::Network<typename MasterType::network_type> Network_Interface;
		Network_Interface* net = (Network_Interface*)_global_network;
		//Network_Interface* net = (Network_Interface*)Allocate<typename MasterType::network_type>();

		typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface; 
		typedef Intersection_Components::Prototypes::Intersection<typename _Link_Interface::get_type_of(upstream_intersection)> _Intersection_Interface;
		
		typedef  Transit_Vehicle_Trip_Components::Prototypes::Transit_Vehicle_Trip<typename remove_pointer< typename Network_Interface::get_type_of(transit_vehicle_trips_container)::value_type>::type>  _Transit_Vehicle_Trip_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(transit_vehicle_trips_container), _Transit_Vehicle_Trip_Interface*> _Transit_Vehicle_Trips_Container_Interface;

		typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
		_Scenario_Interface*_scenario_reference = net->scenario_reference<_Scenario_Interface*>();
		
		//Individualizable Parameters
		//---------------------------------------------------------------------------------------------------------------------------------------------------
		float transferPenalty = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::transferPenalty<float>();
		float waitWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::waitWeight<float>();
		float walkWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::walkWeight<float>();
		float bikeWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::bikeWeight<float>();
		float ivtWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::ivtWeight<float>();
		float carWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::carWeight<float>();
		float scanThreshold = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::scanThreshold<float>();
		float costThreshold = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::costThreshold<float>();
		float waitThreshold_Time = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::waitThreshold<float>();
		Meters walkThreshold = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::walkThreshold<float>();
		Kilometers_Per_Hour walkSpeed_kph = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::walkSpeed<float>();
		Meters bikeThreshold = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::bikeThreshold<float>();
		Kilometers_Per_Hour bikeSpeed_kph = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::bikeSpeed<float>();
		bool multimodal_dijkstra = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::multimodal_dijkstra<bool>();
		//---------------------------------------------------------------------------------------------------------------------------------------------------

		//Conversions
		//---------------------------------------------------------------------------------------------------------------------------------------------------
		Meters_Per_Second walkSpeed_mps = GLOBALS::Convert_Units<Kilometers_Per_Hour, Meters_Per_Second>(walkSpeed_kph);
		Meters_Per_Second bikeSpeed_mps = GLOBALS::Convert_Units<Kilometers_Per_Hour, Meters_Per_Second>(bikeSpeed_kph);
		Feet_Per_Second walkSpeed_fps = GLOBALS::Convert_Units<Kilometers_Per_Hour, Feet_Per_Second>(walkSpeed_kph);
		Feet_Per_Second bikeSpeed_fps = GLOBALS::Convert_Units<Kilometers_Per_Hour, Feet_Per_Second>(bikeSpeed_kph);
		float bike_time_factor = walkSpeed_mps / bikeSpeed_mps;
		float walkThreshold_Time = walkThreshold / walkSpeed_mps;
		float bikeThreshold_Time = bikeThreshold / bikeSpeed_mps;
		//---------------------------------------------------------------------------------------------------------------------------------------------------

		int graph_id = start_ids.front().graph_id;

		char myLine[2000];
		//Counter A_Star_Time;
		//Counter Visit_Time;
		high_resolution_clock::time_point t1;
		high_resolution_clock::time_point t2;

		high_resolution_clock::time_point t3;
		high_resolution_clock::time_point t4;

		__int64 Total_Visit_Time;

		if (debug_route)
		{
			// do route calculation timing for debug routes
			//A_Star_Time.Start();		
			
		}

		//TODO: Remove when done testing routing execution time		
		t1 = high_resolution_clock::now();

		std::deque< base_edge_type* > modified_edges;
		boost::intrusive::multiset< base_edge_type > open_set;
		//bool early_break = false;

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
			end->_cost = 0;
			end->_time_cost = 0;
			if (end == nullptr) { THROW_WARNING("Destination: " << (*itr).edge_id << " not found in graph!"); return 0.0f; }
			ends.push_back((base_edge_type*)end);
		}
		base_edge_type* end_base = (base_edge_type*)end;

		Feet Distance_x = start_base->_x - end_base->_x;
		Feet Distance_y = start_base->_y - end_base->_y;
		Feet Euc_Distance_ft = sqrt(Distance_x*Distance_x + Distance_y*Distance_y);
		Kilometers Euc_Distance_km = GLOBALS::Convert_Units<Feet, Kilometers>(Euc_Distance_ft);
		float scanThreshold2 = Euc_Distance_km * 2500;

		Routing_Data<base_edge_type> routing_data;

		routing_data.modified_edges = &modified_edges;
		//routing_data.end_transit_edges = &tr_ends;
		routing_data.open_set = &open_set;
		routing_data.start_edge = (base_edge_type*)starts.front();
		routing_data.end_edge = (base_edge_type*)ends.front();
		routing_data.ends = &ends;
		routing_data.start_time = start_time;

		//TODO OMER: Check if these additions damages anything
		//----------------------------------------------------
		routing_data.transferPenalty = transferPenalty;
		routing_data.waitWeight = waitWeight;
		routing_data.walkWeight = walkWeight;
		routing_data.bikeWeight = bikeWeight;
		routing_data.ivtWeight = ivtWeight;
		routing_data.carWeight = carWeight;
		routing_data.scanThreshold = scanThreshold;
		routing_data.costThreshold = costThreshold;
		routing_data.waitThreshold_Time = waitThreshold_Time;
		routing_data.walkThreshold = walkThreshold;
		routing_data.walkSpeed_kph = walkSpeed_kph;
		routing_data.bikeThreshold = bikeThreshold;
		routing_data.bikeSpeed_kph = bikeSpeed_kph;
		routing_data.multimodal_dijkstra = multimodal_dijkstra;
		routing_data.walkSpeed_mps = walkSpeed_mps;
		routing_data.bikeSpeed_mps = bikeSpeed_mps;
		routing_data.walkSpeed_fps = walkSpeed_fps;
		routing_data.bikeSpeed_fps = bikeSpeed_fps;
		routing_data.bike_time_factor = bike_time_factor;
		routing_data.walkThreshold_Time = walkThreshold_Time;
		routing_data.bikeThreshold_Time = bikeThreshold_Time;
		routing_data.sub_mode = sub_mode;
		//----------------------------------------------------

		for (auto itr = starts.begin(); itr != starts.end(); ++itr)
		{
			start = (A_Star_Edge<base_edge_type>*)(*itr);

			global_edge_id start_g;
			start_g.graph_id = graph_id;
			start_g.edge_id = start->_edge_id;

			multimodal_edge_type* start_t = (multimodal_edge_type*)graph_pool->Get_Edge(start_g);
			Link_Components::Types::Link_Type_Keys current_type = start_t->_edge_type;

			if (current_type == Link_Components::Types::Link_Type_Keys::WALK && sub_mode != Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE)
			{
				start->cost_from_origin(walkWeight*start->_time_cost);
				start_t->_walk_time_from_origin = start->_time_cost;
				start_t->_bike_time_from_origin = 0;
				start_t->_car_time_from_origin = 0;
				start->time_from_origin(start->_time_cost);
				start->time_label((float)(start_time + start->_time_cost));
			}
			else if (current_type == Link_Components::Types::Link_Type_Keys::WALK && sub_mode == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE)
			{
				start->cost_from_origin(bike_time_factor*bikeWeight*start->_time_cost);
				start_t->_walk_time_from_origin = 0;
				start_t->_bike_time_from_origin = bike_time_factor*start->_time_cost;
				start_t->_car_time_from_origin = 0;
				start->time_from_origin(bike_time_factor*start->_time_cost);
				start->time_label((float)(start_time + bike_time_factor*start->_time_cost));
			}
			else
			{
				start->cost_from_origin(carWeight*start->_time_cost);
				start_t->_walk_time_from_origin = 0;
				start_t->_bike_time_from_origin = 0;
				start_t->_car_time_from_origin = start->_time_cost;
				start->time_from_origin(start->_time_cost);
				start->time_label((float)(start_time + start->_time_cost));
			}
			/*start->cost_from_origin(0.0f);
			start->time_from_origin(0.0f);*/
					

			start_t->_came_on_seq_index = -1;
			start_t->_came_on_trip = nullptr;
			start_t->_wait_count_from_origin = 0;
			start_t->_wait_time_from_origin = 0;
			start_t->_ivt_time_from_origin = 0;
			start_t->_transfer_pen_from_origin = 0;

			float initial_estimated_cost_origin_destination = start->_cost_from_origin + agent->estimated_cost_between((multimodal_edge_type*)start_t, ends, multimodal_dijkstra);
			start->estimated_cost_origin_destination(initial_estimated_cost_origin_destination);

			open_set.insert(*((base_edge_type*)start));

			if (!start->marked_for_reset())
			{
				modified_edges.push_back((base_edge_type*)start);
				start->marked_for_reset(true);
			}
		}

		bool success = false;
		std::string fail_mode;
		int scanCount = 0;
		Total_Visit_Time = 0;

		global_edge_id global;
		global.graph_id = graph_id;

		A_Star_Edge<base_edge_type>* current_fail;
		while (open_set.size() /*&& !early_break*/)
		{
			A_Star_Edge<base_edge_type>* current = (A_Star_Edge<base_edge_type>*)&(*open_set.begin());
			++scanCount;
			
			/*if (origin_loc_id == 67 && destination_loc_id == 128)
			{								
				global.edge_id = current->_edge_id;
				_Link_Interface* current_link = net->template get_link_ptr<typename MasterType::link_type>(global.edge_id);

				int came_from_id = 0;
				if (current->_came_from != nullptr)
				{
					base_edge_type* temp_prev = (base_edge_type*)current->came_from();
					came_from_id = temp_prev->_edge_id;
				}
				

				sprintf_s(myLine, "\n%d\t%d\t%d\t%d\t%d\t%s\t%s\t%d\t%f\t%f\t%f",
					origin_loc_id,
					destination_loc_id,
					start_time,
					scanCount,
					current->_edge_id,
					current_link->_upstream_intersection->_dbid.c_str(),
					current_link->_downstream_intersection->_dbid.c_str(),
					came_from_id,
					current->_cost,
					current->_estimated_cost_origin_destination,
					current->_cost_from_origin
					);
				detail_paragraph.insert(0, myLine);
			}*/
			
			current_fail = current;
			if (current->_cost_from_origin > costThreshold || scanCount >= (int)scanThreshold || scanCount >= (int)scanThreshold2)
			{
				//current_fail = current;
				break;
			}

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

			t3 = high_resolution_clock::now();
			while (connection_set_iterator != connection_set_end)
			{
				connection_set_iterator = connection_set_iterator->Visit_Multimodal_Neighbors(agent, current, routing_data, graph_pool);
			}
			t4 = high_resolution_clock::now();
			auto elapsed_time2 = duration_cast<microseconds>(t4 - t3).count();
			Total_Visit_Time += elapsed_time2;
		}
		
		
		_Transit_Vehicle_Trip_Interface* current_trip;

		float total_cost = 0.0f;
		float travel_time = 0.0f;
				
		if (success)
		{
			global.edge_id = end_base->_edge_id;

			multimodal_edge_type* current = (multimodal_edge_type*)graph_pool->Get_Edge(global);				

			t2 = high_resolution_clock::now();
			auto elapsed_time = duration_cast<microseconds>(t2 - t1).count();
			astar_time = elapsed_time;
			if (debug_route)
			{										
				sprintf_s(myLine, "%d\t%d\t%d\t%d\t%f\t%f\t%f\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\t%I64d\t%I64d\t%s\t%f\n",
					origin_loc_id,
					destination_loc_id,
					start_time,
					sub_mode,
					current->_time_label,
					current->_cost_from_origin,
					current->_time_from_origin,
					current->_wait_count_from_origin,
					current->_wait_time_from_origin,
					current->_walk_time_from_origin,
					current->_bike_time_from_origin,
					current->_ivt_time_from_origin,
					current->_car_time_from_origin,
					current->_transfer_pen_from_origin,
					current->_estimated_cost_origin_destination,
					scanCount,
					astar_time,
					Total_Visit_Time,
					"success",
					Euc_Distance_km
					);
				summary_paragraph.insert(0,myLine);
			}

			int route_ctr = 0;
			while (current != nullptr)
			{
				global.edge_id = current->_edge_id;

				out_path.push_back(global);
				out_cost.push_back(current->_cost_from_origin);
				out_type.push_back(current->_edge_type);
				out_seq.push_back(current->_came_on_seq_index);

				_Link_Interface* current_link = (_Link_Interface*)current->_source_link;
				
				out_time.push_back(current->_time_from_origin); 
				out_arr_time.push_back(current->_time_label);
				out_wait_count.push_back(current->_wait_count_from_origin);
				out_wait_time.push_back(current->_wait_time_from_origin);
				out_walk_time.push_back(current->_walk_time_from_origin);
				out_ivt_time.push_back(current->_ivt_time_from_origin);
				out_car_time.push_back(current->_car_time_from_origin);
				out_transfer_pen.push_back(current->_transfer_pen_from_origin);
				out_heur_cost.push_back(current->_estimated_cost_origin_destination - current->_cost_from_origin);

				Link_Components::Types::Link_Type_Keys current_type = current->_edge_type;
				if (current_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
				{
					current_trip = static_cast<_Transit_Vehicle_Trip_Interface*>(current->_came_on_trip);
					out_trip.push_back(current_trip->uuid<int>());
					if (debug_route)
					{
						sprintf_s(myLine, "\n%d\t%d\t%d\t%d\t%d\t%s\t%s\t%s\t%d\t%s\t%f\t%f\t%f\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\t%I64d",
							origin_loc_id,
							destination_loc_id,
							start_time,
							sub_mode,
							route_ctr,
							current_link->upstream_intersection<_Intersection_Interface*>()->dbid<std::string>().c_str(),
							current_link->downstream_intersection<_Intersection_Interface*>()->dbid<std::string>().c_str(),
							current_trip->dbid<std::string>().c_str(),
							current->_came_on_seq_index,
							"TRANSIT",
							current->_time_label,
							current->_cost_from_origin,
							current->_time_from_origin,
							current->_wait_count_from_origin,
							current->_wait_time_from_origin,
							current->_walk_time_from_origin,
							current->_bike_time_from_origin,
							current->_ivt_time_from_origin,
							current->_car_time_from_origin,
							current->_transfer_pen_from_origin,
							(current->_estimated_cost_origin_destination - current->_cost_from_origin),
							scanCount,
							astar_time);
						detail_paragraph.insert(0, myLine);
					}				

					
				}
				
				else if (current_type == Link_Components::Types::Link_Type_Keys::WALK)
				{
					out_trip.push_back(-1);
					if (debug_route)
					{
						sprintf_s(myLine, "\n%d\t%d\t%d\t%d\t%d\t%s\t%s\t%s\t%d\t%s\t%f\t%f\t%f\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\t%I64d",
							origin_loc_id,
							destination_loc_id,
							start_time,
							sub_mode,
							route_ctr,
							current_link->upstream_intersection<_Intersection_Interface*>()->dbid<std::string>().c_str(),
							current_link->downstream_intersection<_Intersection_Interface*>()->dbid<std::string>().c_str(),
							"WALK",
							current->_came_on_seq_index,
							"WALK",
							current->_time_label,
							current->_cost_from_origin,
							current->_time_from_origin,
							current->_wait_count_from_origin,
							current->_wait_time_from_origin,
							current->_walk_time_from_origin,
							current->_bike_time_from_origin,
							current->_ivt_time_from_origin,
							current->_car_time_from_origin,
							current->_transfer_pen_from_origin,
							(current->_estimated_cost_origin_destination - current->_cost_from_origin),
							scanCount,
							astar_time);
						detail_paragraph.insert(0, myLine);
					}

				}	

				else
				{
					out_trip.push_back(-1);
					if (debug_route)
					{
						sprintf_s(myLine, "\n%d\t%d\t%d\t%d\t%d\t%s\t%s\t%s\t%d\t%s\t%f\t%f\t%f\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\t%I64d",
							origin_loc_id,
							destination_loc_id,
							start_time,
							sub_mode,
							route_ctr,
							current_link->upstream_intersection<_Intersection_Interface*>()->dbid<std::string>().c_str(),
							current_link->downstream_intersection<_Intersection_Interface*>()->dbid<std::string>().c_str(),
							"DRIVE",
							current->_came_on_seq_index,
							"DRIVE",
							current->_time_label,
							current->_cost_from_origin,
							current->_time_from_origin,
							current->_wait_count_from_origin,
							current->_wait_time_from_origin,
							current->_walk_time_from_origin,
							current->_bike_time_from_origin,
							current->_ivt_time_from_origin,
							current->_car_time_from_origin,
							current->_transfer_pen_from_origin,
							(current->_estimated_cost_origin_destination - current->_cost_from_origin),
							scanCount,
							astar_time);
						detail_paragraph.insert(0, myLine);
					}

				}

				current = (multimodal_edge_type*)current->came_from();
				route_ctr++;
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
			std::reverse(out_heur_cost.begin(), out_heur_cost.end());
			
			total_cost = out_cost.back();
			travel_time = out_time.back();

			for (auto itr = end_ids.begin(); itr != end_ids.end(); ++itr)
			{
				end = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(*itr);
				end->_cost = end->_cost_backup;
				end->_time_cost = end->_time_cost_backup;
			}

			// update start_ids/end_ids to includ final routed start/end
			start_ids.clear();
			start_ids.push_back(out_path.front());
			end_ids.clear();
			end_ids.push_back(out_path.back());
			
		}
		else
		{
			for (auto itr = end_ids.begin(); itr != end_ids.end(); ++itr)
			{
				end = (A_Star_Edge<base_edge_type>*)graph_pool->Get_Edge(*itr);
				end->_cost = end->_cost_backup;
				end->_time_cost = end->_time_cost_backup;
			}

			t2 = high_resolution_clock::now();
			auto elapsed_time = duration_cast<microseconds>(t2 - t1).count();
			astar_time = elapsed_time;

			if (debug_route)
			{
				
				global.edge_id = current_fail->_edge_id;

				multimodal_edge_type* current = (multimodal_edge_type*)graph_pool->Get_Edge(global);
				
				sprintf_s(myLine, "%d\t%d\t%d\t%d\t%f\t%f\t%f\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\t%I64d\t%I64d\t%s\t%f\n",
					origin_loc_id,
					destination_loc_id,
					start_time,
					sub_mode,
					current->_time_label,
					current->_cost_from_origin,
					current->_time_from_origin,
					current->_wait_count_from_origin,
					current->_wait_time_from_origin,
					current->_walk_time_from_origin,
					current->_bike_time_from_origin,
					current->_ivt_time_from_origin,
					current->_car_time_from_origin,
					current->_transfer_pen_from_origin,
					current->_estimated_cost_origin_destination,
					scanCount,
					astar_time,
					Total_Visit_Time,
					"fail",
					Euc_Distance_km
					);
				summary_paragraph.insert(0, myLine);
			}
		}		

		for (auto itr = modified_edges.begin(); itr != modified_edges.end(); itr++)
		{
			(*itr)->reset();
		}

		//return total_cost;
		return travel_time;
	}
}