#pragma once
#include "User_Space_Forward_Declaration.h"
#include "routing/Routing.h"

namespace Routing_Components
{
	namespace Prototypes
	{
		prototype struct Routable_Network
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
			
			void update_edge_turn_costs(unsigned int edge_id,float edge_cost,boost::container::deque<float>& turn_costs)
			{
				this_component()->update_edge_turn_costs(edge_id,edge_cost,turn_costs);
			}

			template<typename Source_Network_Type>
			void construct_routable_network(Network_Components::Prototypes::Network<Source_Network_Type>* source_network)
			{
				this_component()->construct_routable_network(source_network);
			}

			float compute_static_network_path(unsigned int origin, unsigned int destination, boost::container::deque<global_edge_id>& path_container, boost::container::deque<float>& cost_container)
			{
				return this_component()->compute_static_network_path(origin,destination,path_container,cost_container);
			}

			void test_routable_network()
			{
				this_component()->test_routable_network();
			}
		};
	}
}

using namespace Routing_Components::Prototypes;
