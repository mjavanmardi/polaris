#pragma once
#include "A_Star_Edge_Implementation.h"

namespace polaris
{
	template<typename MasterType>
	struct Routable_Agent_Implementation
	{
		template<typename CurrentEdgeType, typename ConnectionType>
		bool process_connection_set(CurrentEdgeType* current)
		{
			return true;
		}

		//template<>
		//bool process_connection_set<typename MasterType::transit_edge,typename MasterType::walk_transfer_type>(typename MasterType::transit_edge* current)
		//{
		//	if(current->_transfers_made < 3) return true;
		//	else return false;
		//}

		//template<>
		//bool process_connection_set<typename MasterType::transit_edge,typename MasterType::transfer_at_stop_type>(typename MasterType::transit_edge* current)
		//{
		//	if(current->_transfers_made < 3) return true;
		//	else return false;
		//}
		//
		//template<>
		//bool process_connection_set<typename MasterType::stop_edge,typename MasterType::stop_to_stop_type>(typename MasterType::stop_edge* current)
		//{
		//	if(current->_walk_transfers_made < 2) return true;
		//	else return false;
		//}

		bool at_destination(Base_Edge_A_Star<MasterType>* current, Base_Edge_A_Star<MasterType>* destination)
		{
			if(current->_edge_id == destination->_edge_id) return true;
			else return false;
		}

		template<typename CurrentEdgeType>
		float estimated_cost_between(CurrentEdgeType* current, Base_Edge_A_Star<MasterType>* destination)
		{
			float x_dist = current->_x - destination->_x;
			x_dist *= x_dist;
			
			float y_dist = current->_y - destination->_y;
			y_dist *= y_dist;

			// vehicle speed
			float cost = sqrt(x_dist + y_dist)/89.0f;
			
			return cost;
		}

		//template<>
		//float estimated_cost_between<typename MasterType::stop_edge>(typename MasterType::stop_edge* current, Base_Edge_A_Star<MasterType>* destination)
		//{
		//	float x_dist = current->_x - destination->_x;
		//	x_dist *= x_dist;
		//
		//	float y_dist = current->_y - destination->_y;
		//	y_dist *= y_dist;

		//	// walk speed
		//	float cost = sqrt(x_dist + y_dist)/1.34f;
		//	
		//	return cost;
		//}

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			return current->_cost + connection->_cost;
		}

		//template<>
		//float cost_between<typename MasterType::transit_edge,typename MasterType::transit_edge,typename MasterType::ride_on_type>(typename MasterType::transit_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::ride_on_type* connection)
		//{
		//	return current->_time_cost + connection->_time_cost;
		//}
		//
		//template<>
		//float cost_between<typename MasterType::transit_edge,typename MasterType::transit_edge,typename MasterType::transfer_at_stop_type>(typename MasterType::transit_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::transfer_at_stop_type* connection)
		//{
		//	// disincentivize these sort of transfers
		//	//const float transfer_penalty = 2.0f;
		//	const float transfer_penalty = 1.0f;
		//	return (current->_time_cost + connection->_time_cost)*transfer_penalty;
		//}

		//template<>
		//float cost_between<typename MasterType::transit_edge,typename MasterType::transit_edge,typename MasterType::walk_transfer_type>(typename MasterType::transit_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::walk_transfer_type* connection)
		//{
		//	// strongly disincentivize these sort of transfers
		//	//const float walk_transfer_penalty = 3.0f;
		//	const float walk_transfer_penalty = 1.0f;
		//	return (current->_time_cost + connection->_time_cost)*walk_transfer_penalty;
		//}

		//template<>
		//float cost_between<typename MasterType::transit_edge,typename MasterType::stop_edge,typename MasterType::transit_to_stop_type>(typename MasterType::transit_edge* current, typename MasterType::stop_edge* neighbor, typename MasterType::transit_to_stop_type* connection)
		//{
		//	return current->_time_cost + connection->_time_cost;
		//}
		//
		//template<>
		//float cost_between<typename MasterType::stop_edge,typename MasterType::transit_edge,typename MasterType::stop_to_transit_type>(typename MasterType::stop_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::stop_to_transit_type* connection)
		//{
		//	// have to wait at the stop until the trip leaves
		//	return connection->_departure_time - current->_time_label;
		//}

		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			return current->_time_cost + connection->_time_cost;
		}

		//template<>
		//float time_cost_between<typename MasterType::stop_edge,typename MasterType::transit_edge,typename MasterType::stop_to_transit_type>(typename MasterType::stop_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::stop_to_transit_type* connection)
		//{
		//	// have to wait at the stop until the trip leaves
		//	return connection->_departure_time - current->_time_label;
		//}

		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		void update_label(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){}

		//template<>
		//void update_label<typename MasterType::transit_edge,typename MasterType::transit_edge,typename MasterType::ride_on_type>(typename MasterType::transit_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::ride_on_type* connection)
		//{
		//	neighbor->_transfers_made = current->_transfers_made;
		//	neighbor->_walk_transfers_made = current->_walk_transfers_made;
		//}
		//
		//template<>
		//void update_label<typename MasterType::transit_edge,typename MasterType::stop_edge,typename MasterType::transit_to_stop_type>(typename MasterType::transit_edge* current, typename MasterType::stop_edge* neighbor, typename MasterType::transit_to_stop_type* connection)
		//{
		//	neighbor->_transfers_made = current->_transfers_made;
		//	neighbor->_walk_transfers_made = current->_walk_transfers_made;
		//}

		//template<>
		//void update_label<typename MasterType::stop_edge,typename MasterType::transit_edge,typename MasterType::stop_to_transit_type>(typename MasterType::stop_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::stop_to_transit_type* connection)
		//{
		//	neighbor->_transfers_made = current->_transfers_made;
		//	neighbor->_walk_transfers_made = current->_walk_transfers_made;
		//}

		//template<>
		//void update_label<typename MasterType::transit_edge,typename MasterType::transit_edge,typename MasterType::transfer_at_stop_type>(typename MasterType::transit_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::transfer_at_stop_type* connection)
		//{
		//	neighbor->_transfers_made = current->_transfers_made + 1;
		//	neighbor->_walk_transfers_made = current->_walk_transfers_made;
		//}

		//template<>
		//void update_label<typename MasterType::transit_edge,typename MasterType::transit_edge,typename MasterType::walk_transfer_type>(typename MasterType::transit_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::walk_transfer_type* connection)
		//{
		//	neighbor->_transfers_made = current->_transfers_made + 1;
		//	neighbor->_walk_transfers_made = current->_walk_transfers_made;
		//}

		//template<>
		//void update_label<typename MasterType::stop_edge,typename MasterType::stop_edge,typename MasterType::stop_to_stop_type>(typename MasterType::stop_edge* current, typename MasterType::stop_edge* neighbor, typename MasterType::stop_to_stop_type* connection)
		//{
		//	neighbor->_transfers_made = current->_transfers_made;
		//	neighbor->_walk_transfers_made = current->_walk_transfers_made + 1;
		//}

		t_data(float,wait_horizon);
	};


	template<typename MasterType>
	struct Tree_Agent_Implementation
	{
		template<typename CurrentEdgeType, typename ConnectionType>
		bool process_connection_set(CurrentEdgeType* current)
		{
			return true;
		}

		bool at_destination(Base_Edge_A_Star<MasterType>* current, Base_Edge_A_Star<MasterType>* destination)
		{
			return false;
		}

		template<typename CurrentEdgeType>
		float estimated_cost_between(CurrentEdgeType* current, Base_Edge_A_Star<MasterType>* destination)
		{
			return 0.0f;
		}

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			return current->_cost + connection->_cost;
		}

		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			return current->_time_cost + connection->_time_cost;
		}

		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		void update_label(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){}
	};
}