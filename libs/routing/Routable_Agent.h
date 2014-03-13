#pragma once
#include "core\Core.h"

namespace polaris
{
	prototype struct Routable_Agent
	{
		//typedef typename ComponentType::agent_state_type agent_state_type;
		
		template<typename CurrentEdgeType, typename ConnectionType>
		bool process_connection_set(CurrentEdgeType* current){ return this_component()->process_connection_set<CurrentEdgeType,ConnectionType>(current); }

		template<typename CurrentEdgeType, typename DestinationEdgeType>
		bool at_destination(CurrentEdgeType* current, DestinationEdgeType* destination){ return this_component()->at_destination(current, destination); }

		template<typename CurrentEdgeType, typename DestinationEdgeType>
		float estimated_cost_between(CurrentEdgeType* current, DestinationEdgeType* destination){ return this_component()->estimated_cost_between(current, destination); }

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){ return this_component()->cost_between(current, neighbor, connection); }	

		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){ return this_component()->time_cost_between(current, neighbor, connection); }	

		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		void update_label(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){ this_component()->update_label(current, neighbor, connection); }	
	};

	prototype struct Tree_Agent
	{
		//typedef typename ComponentType::agent_state_type agent_state_type;
		
		template<typename CurrentEdgeType, typename ConnectionType>
		bool process_connection_set(CurrentEdgeType* current){ return this_component()->process_connection_set<CurrentEdgeType,ConnectionType>(current); }

		template<typename CurrentEdgeType, typename DestinationEdgeType>
		bool at_destination(CurrentEdgeType* current, DestinationEdgeType* destination){ return this_component()->at_destination(current, destination); }

		template<typename CurrentEdgeType, typename DestinationEdgeType>
		float estimated_cost_between(CurrentEdgeType* current, DestinationEdgeType* destination){ return this_component()->estimated_cost_between(current, destination); }

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){ return this_component()->cost_between(current, neighbor, connection); }	

		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){ return this_component()->time_cost_between(current, neighbor, connection); }	

		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		void update_label(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){ this_component()->update_label(current, neighbor, connection); }	
	};
}