#pragma once
#include "Traffic_Simulator_Forward_Declaration.h"
#include "routing/Routing.h"

namespace Routing_Components
{
	namespace Prototypes
	{
		prototype struct Routable_Network ADD_DEBUG_INFO
		{
			tag_as_prototype;

			Routable_Network<ComponentType>* create_copy()
			{
				return this_component()->create_copy();
			}

			void update_edge_turn_cost(unsigned int edge_id,float edge_cost,unsigned int outbound_turn_index,float turn_cost)
			{
				this_component()->update_edge_turn_cost(edge_id,edge_cost,outbound_turn_index,turn_cost);
			}
			
			void update_edge_turn_costs(unsigned int edge_id,float edge_cost,std::deque<float>& turn_costs)
			{
				this_component()->update_edge_turn_costs(edge_id,edge_cost,turn_costs);
			}

			template<typename Source_Network_Type>
			void construct_routable_network(Network_Components::Prototypes::Network<Source_Network_Type>* source_network)
			{
				this_component()->construct_routable_network(source_network);
			}
			
			template<typename Source_Network_Type>
			void construct_time_dependent_routable_network(Network_Components::Prototypes::Network<Source_Network_Type>* source_network)
			{
				this_component()->construct_time_dependent_routable_network(source_network);
			}

			template<typename Source_Network_Type>
			void construct_multimodal_routable_network(Network_Components::Prototypes::Network<Source_Network_Type>* source_network)
			{
				this_component()->construct_multimodal_routable_network(source_network);
			}

			float compute_static_network_path(std::vector<unsigned int>& origins, std::vector<unsigned int>& destinations, unsigned int start_time, std::deque<global_edge_id>& path_container, std::deque<float>& cost_container, unsigned int origin_loc_id, unsigned int destination_loc_id, bool debug_route, std::string& summary_paragraph)
			{
				return this_component()->compute_static_network_path(origins,destinations, start_time, path_container,cost_container, origin_loc_id, destination_loc_id, debug_route, summary_paragraph);
			}

			//TODO: Remove when done testing routing execution time
			float compute_multimodal_network_path(
				std::vector<unsigned int>& origins,
				std::vector<unsigned int>& destinations,
				/*std::vector<unsigned int>& tr_destinations,*/
				unsigned int start_time,
				std::deque<global_edge_id>& path_container,
				std::deque<float>& cost_container,
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
				//TODO: Remove when done testing routing execution time
				return this_component()->compute_multimodal_network_path(origins, destinations, /*tr_destinations,*/ start_time, path_container, cost_container, out_type, out_trip, out_seq, out_time, out_arr_time, out_wait_time, out_walk_time, out_ivt_time, out_car_time, out_wait_count, out_transfer_pen, out_heur_cost, astar_time, origin_loc_id, destination_loc_id, debug_route, summary_paragraph, detail_paragraph, sub_mode);
			}

			float compute_time_dependent_network_path(std::vector<unsigned int>& origins, std::vector<unsigned int>& destinations, unsigned int start_time, std::deque<global_edge_id>& path_container, std::deque<float>& cost_container, unsigned int origin_loc_id, unsigned int destination_loc_id, bool debug_route, std::string& summary_paragraph)
			{
				return this_component()->compute_time_dependent_network_path(origins,destinations,start_time,path_container,cost_container, origin_loc_id, destination_loc_id, debug_route, summary_paragraph);
			}

			float compute_static_network_tree(unsigned int origin, std::vector<float>& edge_cost_container)
			{
				return this_component()->compute_static_network_tree(origin,edge_cost_container);
			}

			template<typename Source_Network_Type>
			void schedule_dijkstra_network_tree(Network_Components::Prototypes::Network<Source_Network_Type>* source_network)
			{
				return this_component()->schedule_dijkstra_network_tree(source_network);
			}

			void compute_dijkstra_network_tree(std::vector<unsigned int>& origins, int zone_index, bool debug_route, std::string& summary_paragraph)
			{
				return this_component()->compute_dijkstra_network_tree(origins, zone_index, debug_route, summary_paragraph);
			}

			template<typename Source_Network_Type>
			void compute_walk_distance_to_transit(Network_Components::Prototypes::Network<Source_Network_Type>* source_network)
			{
				return this_component()->compute_walk_distance_to_transit(source_network);
			}

			template<typename Source_Network_Type>
			void compute_drive_fft_to_transit(Network_Components::Prototypes::Network<Source_Network_Type>* source_network)
			{
				return this_component()->compute_drive_fft_to_transit(source_network);
			}
			
			void initialize()
			{
				this_component()->initialize();
			}

			void finalize()
			{
				this_component()->finalize();
			}
			void test_routable_network()
			{
				this_component()->test_routable_network();
			}
		};
	}
}

using namespace Routing_Components::Prototypes;
