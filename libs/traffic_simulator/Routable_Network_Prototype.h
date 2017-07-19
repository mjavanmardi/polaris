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
			void construct_routable_multimodal_network(Network_Components::Prototypes::Network<Source_Network_Type>* source_network)
			{
				this_component()->construct_routable_multimodal_network(source_network);
			}

			float compute_static_network_path(std::vector<unsigned int>& origins, std::vector<unsigned int>& destinations, unsigned int start_time, std::deque<global_edge_id>& path_container, std::deque<float>& cost_container, bool debug_route = false)
			{
				return this_component()->compute_static_network_path(origins,destinations, start_time, path_container,cost_container, debug_route);
			}

			float compute_multimodal_network_path(std::vector<unsigned int>& origins, std::vector<unsigned int>& destinations, std::vector<unsigned int>& tr_destinations, unsigned int start_time, std::deque<global_edge_id>& path_container, std::deque<float>& cost_container, bool debug_route = false)
			{
				return this_component()->compute_multimodal_network_path(origins, destinations, tr_destinations, start_time, path_container, cost_container, debug_route);
			}

			float compute_time_dependent_network_path(std::vector<unsigned int>& origins, std::vector<unsigned int>& destinations, unsigned int start_time, std::deque<global_edge_id>& path_container, std::deque<float>& cost_container, bool debug_route=false)
			{
				return this_component()->compute_time_dependent_network_path(origins,destinations,start_time,path_container,cost_container, debug_route);
			}

			float compute_static_network_tree(unsigned int origin, std::vector<float>& edge_cost_container)
			{
				return this_component()->compute_static_network_tree(origin,edge_cost_container);
			}

			template<typename Source_Network_Type>
			void compute_dijkstra_network_tree(Network_Components::Prototypes::Network<Source_Network_Type>* source_network)
			{
				return this_component()->compute_dijkstra_network_tree(source_network);
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
