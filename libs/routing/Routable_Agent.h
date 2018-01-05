#pragma once
#include "core/Core.h"

namespace polaris
{
	prototype struct Routable_Agent ADD_DEBUG_INFO
	{
		//some of those functions are called in Evaluate_Neighbor

		//returns a set of edges the agent wold like to visit, thus agent is responsable for prooning
		//typedef typename ComponentType::agent_state_type agent_state_type;
		template<typename CurrentEdgeType, typename ConnectionType>
		bool process_connection_set(CurrentEdgeType* current){ return this_component()->process_connection_set<CurrentEdgeType,ConnectionType>(current); }

		//are you at your destanation, so you can have many possable link/node destanations, but might also cause a conflict if this function never returns true, then the entire graph will be explored
		template<typename CurrentEdgeType, typename DestinationEdgeType>
		bool at_destination(CurrentEdgeType* current, DestinationEdgeType* destination){ return this_component()->at_destination(current, destination); }

		template<typename CurrentEdgeType, typename DestinationEdgeType>
		bool at_destination(CurrentEdgeType* current, std::vector<DestinationEdgeType*>& destinations, DestinationEdgeType** final_destination){ return this_component()->at_destination(current, destinations, final_destination); }

		template<typename CurrentEdgeType, typename DestinationEdgeType>
		bool at_destination(CurrentEdgeType* current, std::vector<DestinationEdgeType*>& destinations) { return this_component()->at_destination(current, destinations); }
		
		template<typename CurrentEdgeType, typename DestinationEdgeType>
		float estimated_cost_between(CurrentEdgeType* current, std::vector<DestinationEdgeType*>& destinations) { return this_component()->estimated_cost_between(current, destinations); }

		template<typename CurrentEdgeType, typename DestinationEdgeType>
		float estimated_cost_between(CurrentEdgeType* current, std::vector<DestinationEdgeType*>& destinations, bool multimodal_dijkstra) { return this_component()->estimated_cost_between(current, destinations, multimodal_dijkstra); }

		//cost between 2 edges. Can implement different turn movements preferences for an agent, for example if left turns are not prefered, then wehn passed, use very high cost to avoid those movements
		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){ return this_component()->cost_between(current, neighbor, connection); }	

		//specific to start time
		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){ return this_component()->time_cost_between(current, neighbor, connection); }	

		//update the internal agent's state. For example different agents might have different preferences for number of transit transfers
		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		void update_label(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){ this_component()->update_label(current, neighbor, connection); }	
	};

    //prototype struct Tree_Agent ADD_DEBUG_INFO
	//{
	//	//typedef typename ComponentType::agent_state_type agent_state_type;
	//	
	//	template<typename CurrentEdgeType, typename ConnectionType>
	//	bool process_connection_set(CurrentEdgeType* current){ return this_component()->process_connection_set<CurrentEdgeType,ConnectionType>(current); }

	//	template<typename CurrentEdgeType, typename DestinationEdgeType>
	//	bool at_destination(CurrentEdgeType* current, DestinationEdgeType* destination){ return this_component()->at_destination(current, destination); }

	//	template<typename CurrentEdgeType, typename DestinationEdgeType>
	//	float estimated_cost_between(CurrentEdgeType* current, DestinationEdgeType* destination){ return this_component()->estimated_cost_between(current, destination); }

	//	template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
	//	float cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){ return this_component()->cost_between(current, neighbor, connection); }	

	//	template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
	//	float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){ return this_component()->time_cost_between(current, neighbor, connection); }	

	//	template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
	//	void update_label(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){ this_component()->update_label(current, neighbor, connection); }	
	//};
}