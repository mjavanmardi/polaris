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

		bool at_destination(Base_Edge_A_Star<MasterType>* current, Base_Edge_A_Star<MasterType>* destination)
		{
			if(current->_edge_id == destination->_edge_id) return true;
			else return false;
		}
		
		bool at_destination(Base_Edge_A_Star<MasterType>* current, std::vector<Base_Edge_A_Star<MasterType>*>& destinations, Base_Edge_A_Star<MasterType>** final_destination)
		{
			for (auto itr = destinations.begin(); itr != destinations.end(); ++itr)
			{
				*final_destination = (Base_Edge_A_Star<MasterType>*)(*itr);
				if(current->_edge_id == (*final_destination)->_edge_id) return true;	
			}
			return false;
		}

		bool at_destination(Base_Edge_A_Star<MasterType>* current, std::vector<Base_Edge_A_Star<MasterType>*>& destinations)
		{
			for (auto itr = destinations.begin(); itr != destinations.end(); ++itr)
			{
				Base_Edge_A_Star<MasterType>* itr_destination = (Base_Edge_A_Star<MasterType>*)(*itr);
				if (current->_edge_id == itr_destination->_edge_id) return true;
			}
			return false;
		}

		template<typename CurrentEdgeType>
		float estimated_cost_between(CurrentEdgeType* current, Base_Edge_A_Star<MasterType>* destination)
		{
			float x_dist = current->_x - destination->_x;
			x_dist *= x_dist;
			
			float y_dist = current->_y - destination->_y;
			y_dist *= y_dist;

			// vehicle speed
			//float cost = sqrt(x_dist + y_dist)/89.0f;
			float cost = sqrt(x_dist + y_dist) / 50.0f;
			return cost;
		}

		template<typename CurrentEdgeType>
		float estimated_walk_distance_between(CurrentEdgeType* current, Base_Edge_A_Star<MasterType>* destination)
		{
			float x_dist = current->_x - destination->_x;
			x_dist *= x_dist;

			float y_dist = current->_y - destination->_y;
			y_dist *= y_dist;

			float cost = sqrt(x_dist + y_dist);
			return cost;
		}

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			if(neighbor->_is_highway)
			{
				//cout << "is highway" << endl;
				//return (current->_cost + connection->_cost)*.75f;
				return (current->_cost + connection->_cost)*.95f;
			}
			else
			{
				//cout << "is not highway" << endl;
				return current->_cost + connection->_cost;
			}
		}

		float cost_between(typename MT::time_dependent_edge_type* current, typename MT::time_dependent_edge_type* neighbor, typename MT::time_dependent_to_time_dependent_type* connection)
		{
			// moe lookup - switched from link-based (moe_ptr) to turn-based (turn_moe_ptr) - testing.....
			int current_time = current->time_label();
			//float* moe_ptr = current->moe_ptr();
			float* turn_moe_ptr = connection->turn_moe_ptr();

			if(turn_moe_ptr /*moe_ptr*/ != nullptr)
			{
				int sim_time = iteration();

				// get historical time cost - update if traveler will be on link for multiple time periods
				float ttime_accumulation = 0;
				float ttime_step = current->moe_data()->layer_step<float>();
				ttime_step = ttime_step - current_time % (int)ttime_step;
				float t = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
				if (t > ttime_step)
				{
					ttime_accumulation += ttime_step;
					t = current->_cost + connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation);
					ttime_step = current->moe_data()->layer_step<float>();
					while (t - ttime_accumulation > ttime_step)
					{
						ttime_accumulation += ttime_step;
						t = current->_cost + connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation);

					}
				}
				t = ttime_accumulation + std::max(t - ttime_accumulation, 0.0f);

				

				// updates to handle mixing of historical and real-time info in cost function
				float time_cost_current = current->_cost + connection->_cost;

				int t_diff = abs(current_time - iteration());

				// modified time dependent mixing function to be parameterized with shape and scale set in scenario
				// ttime_weight_factor allows extra control to turn off information mixing -> setting to 0 will use only historical info
				float w = (exp(-1.0*pow(((float)t_diff / current->ttime_weight_scale()), current->ttime_weight_shape())))*current->ttime_weight_factor();

				float time_cost = w*time_cost_current + (1-w)*t;


				if(neighbor->_is_highway)
				{
					return time_cost*.95f;
				}
				else
				{

					return time_cost;
				}
			}
			else
			{

				if(neighbor->_is_highway)
				{
					//cout << "is highway" << endl;
					return (current->_cost + connection->_cost)*.95f;
				}
				else
				{
					//cout << "is not highway" << endl;
					return current->_cost + connection->_cost;
				}
				
			}
		}
		
		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			return current->_time_cost + connection->_time_cost;
		}

		float time_cost_between(typename MT::time_dependent_edge_type* current, typename MT::time_dependent_edge_type* neighbor, typename MT::time_dependent_to_time_dependent_type* connection)
		{
			// moe lookup
			int current_time = current->time_label();
			float* turn_moe_ptr = connection->turn_moe_ptr();

			if(turn_moe_ptr != nullptr)
			{
				int sim_time = iteration();

				// get historical time cost - update if traveler will be on link for multiple time periods
				float ttime_accumulation = 0;
				float ttime_step = current->moe_data()->layer_step<float>();
				ttime_step = ttime_step - current_time % (int)ttime_step;
				float t = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time) + current->_time_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
				if (t > ttime_step)
				{
					ttime_accumulation += ttime_step;
					t = current->_time_cost + connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation);
					ttime_step = current->moe_data()->layer_step<float>();
					while (t - ttime_accumulation > ttime_step)
					{
						ttime_accumulation += ttime_step;
						t = current->_time_cost + connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation);

					}
				}
				t = ttime_accumulation + std::max(t - ttime_accumulation, 0.0f);



				// updates to handle mixing of historical and real-time info in cost function
				float time_cost_current = current->_time_cost + connection->_time_cost;

				int t_diff = abs(current_time - iteration());

				// modified time dependent mixing function to be parameterized with shape and scale set in scenario
				// ttime_weight_factor allows extra control to turn off information mixing -> setting to 0 will use only historical info
				float w = (exp(-1.0*pow(((float)t_diff / current->ttime_weight_scale()), current->ttime_weight_shape())))*current->ttime_weight_factor();

				float time_cost = w*time_cost_current + (1-w)*t;


				if(neighbor->_is_highway)
				{
					return time_cost*.95f;
				}
				else
				{

					return time_cost;
				}
			}
			else
			{

				if(neighbor->_is_highway)
				{
					//cout << "is highway" << endl;
					return (current->_time_cost + connection->_time_cost)*.95f;
				}
				else
				{
					//cout << "is not highway" << endl;
					return current->_time_cost + connection->_time_cost;
				}
				
			}
		}

		float time_cost_between(typename MT::multimodal_edge_type* current, typename MT::multimodal_edge_type* neighbor, typename MT::multimodal_to_multimodal_type* connection)
		{
			// moe lookup
			int current_time = current->time_label();
			float* turn_moe_ptr = connection->turn_moe_ptr();

			if (turn_moe_ptr != nullptr)
			{
				int sim_time = iteration();

				// get historical time cost - update if traveler will be on link for multiple time periods
				float ttime_accumulation = 0;
				float ttime_step = current->moe_data()->layer_step<float>();
				ttime_step = ttime_step - current_time % (int)ttime_step;
				float t = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time) + current->_time_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
				if (t > ttime_step)
				{
					ttime_accumulation += ttime_step;
					t = current->_time_cost + connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation);
					ttime_step = current->moe_data()->layer_step<float>();
					while (t - ttime_accumulation > ttime_step)
					{
						ttime_accumulation += ttime_step;
						t = current->_time_cost + connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation);

					}
				}
				t = ttime_accumulation + std::max(t - ttime_accumulation, 0.0f);



				// updates to handle mixing of historical and real-time info in cost function
				float time_cost_current = current->_time_cost + connection->_time_cost;

				int t_diff = abs(current_time - iteration());

				// modified time dependent mixing function to be parameterized with shape and scale set in scenario
				// ttime_weight_factor allows extra control to turn off information mixing -> setting to 0 will use only historical info
				float w = (exp(-1.0*pow(((float)t_diff / current->ttime_weight_scale()), current->ttime_weight_shape())))*current->ttime_weight_factor();

				float time_cost = w*time_cost_current + (1 - w)*t;


				if (neighbor->_is_highway)
				{
					//return time_cost*.95f;
					return time_cost;
				}
				else
				{

					return time_cost;
				}
			}
			else
			{

				if (neighbor->_is_highway)
				{
					//cout << "is highway" << endl;
					//return (current->_time_cost + connection->_time_cost)*.95f;
					return (current->_time_cost + connection->_time_cost);
				}
				else
				{
					//cout << "is not highway" << endl;
					return current->_time_cost + connection->_time_cost;
				}

			}
		}

		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		void update_label(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){}

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

	template<typename MasterType>
	struct Multi_Modal_Tree_Agent_Implementation
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

		bool at_destination(Base_Edge_A_Star<MasterType>* current, std::vector<Base_Edge_A_Star<MasterType>*>& destinations, Base_Edge_A_Star<MasterType>** final_destination)
		{
			for (auto itr = destinations.begin(); itr != destinations.end(); ++itr)
			{
				*final_destination = (Base_Edge_A_Star<MasterType>*)(*itr);
				if (current->_edge_id == (*final_destination)->_edge_id) return true;
			}
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
			return neighbor->_min_multi_modal_cost;
		}

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			return neighbor->_min_multi_modal_cost;
		}

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		void update_label(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection) {}
	};


	//template<typename MasterType>
	//struct Time_Dependent_Agent_Implementation
	//{
	//	template<typename CurrentEdgeType, typename ConnectionType>
	//	bool process_connection_set(CurrentEdgeType* current)
	//	{
	//		return true;
	//	}

	//	//template<>
	//	//bool process_connection_set<typename MasterType::transit_edge,typename MasterType::walk_transfer_type>(typename MasterType::transit_edge* current)
	//	//{
	//	//	if(current->_transfers_made < 3) return true;
	//	//	else return false;
	//	//}

	//	//template<>
	//	//bool process_connection_set<typename MasterType::transit_edge,typename MasterType::transfer_at_stop_type>(typename MasterType::transit_edge* current)
	//	//{
	//	//	if(current->_transfers_made < 3) return true;
	//	//	else return false;
	//	//}
	//	//
	//	//template<>
	//	//bool process_connection_set<typename MasterType::stop_edge,typename MasterType::stop_to_stop_type>(typename MasterType::stop_edge* current)
	//	//{
	//	//	if(current->_walk_transfers_made < 2) return true;
	//	//	else return false;
	//	//}

	//	bool at_destination(Base_Edge_A_Star<MasterType>* current, Base_Edge_A_Star<MasterType>* destination)
	//	{
	//		if(current->_edge_id == destination->_edge_id) return true;
	//		else return false;
	//	}

	//	template<typename CurrentEdgeType>
	//	float estimated_cost_between(CurrentEdgeType* current, Base_Edge_A_Star<MasterType>* destination)
	//	{
	//		float x_dist = current->_x - destination->_x;
	//		x_dist *= x_dist;
	//		
	//		float y_dist = current->_y - destination->_y;
	//		y_dist *= y_dist;

	//		// vehicle speed
	//		float cost = sqrt(x_dist + y_dist)/89.0f;
	//		
	//		return cost;
	//	}

	//	//template<>
	//	//float estimated_cost_between<typename MasterType::stop_edge>(typename MasterType::stop_edge* current, Base_Edge_A_Star<MasterType>* destination)
	//	//{
	//	//	float x_dist = current->_x - destination->_x;
	//	//	x_dist *= x_dist;
	//	//
	//	//	float y_dist = current->_y - destination->_y;
	//	//	y_dist *= y_dist;

	//	//	// walk speed
	//	//	float cost = sqrt(x_dist + y_dist)/1.34f;
	//	//	
	//	//	return cost;
	//	//}

	//	template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
	//	float cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
	//	{
	//		// moe lookup
	//		int current_time = current->time_label();
	//		float* moe_ptr = current->moe_ptr();

	//		if(moe_ptr != nullptr)
	//		{
	//			float time_cost = current->moe_data()->get_closest_element(moe_ptr,current_time);

	//			if(neighbor->_is_highway)
	//			{
	//				//t_pointer(float,moe_ptr);
	//				//static t_pointer(Layered_Data_Array<float>,moe_data);

	//				return time_cost*.75f;
	//			}
	//			else
	//			{

	//				return time_cost;
	//			}
	//		}
	//		else
	//		{

	//			if(neighbor->_is_highway)
	//			{
	//				return (current->_time_cost + connection->_time_cost)*.75f;
	//			}
	//			else
	//			{
	//				return current->_time_cost + connection->_time_cost;
	//			}
	//			
	//		}
	//	}

	//	//template<>
	//	//float cost_between<typename MasterType::transit_edge,typename MasterType::transit_edge,typename MasterType::ride_on_type>(typename MasterType::transit_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::ride_on_type* connection)
	//	//{
	//	//	return current->_time_cost + connection->_time_cost;
	//	//}
	//	//
	//	//template<>
	//	//float cost_between<typename MasterType::transit_edge,typename MasterType::transit_edge,typename MasterType::transfer_at_stop_type>(typename MasterType::transit_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::transfer_at_stop_type* connection)
	//	//{
	//	//	// disincentivize these sort of transfers
	//	//	//const float transfer_penalty = 2.0f;
	//	//	const float transfer_penalty = 1.0f;
	//	//	return (current->_time_cost + connection->_time_cost)*transfer_penalty;
	//	//}

	//	//template<>
	//	//float cost_between<typename MasterType::transit_edge,typename MasterType::transit_edge,typename MasterType::walk_transfer_type>(typename MasterType::transit_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::walk_transfer_type* connection)
	//	//{
	//	//	// strongly disincentivize these sort of transfers
	//	//	//const float walk_transfer_penalty = 3.0f;
	//	//	const float walk_transfer_penalty = 1.0f;
	//	//	return (current->_time_cost + connection->_time_cost)*walk_transfer_penalty;
	//	//}

	//	//template<>
	//	//float cost_between<typename MasterType::transit_edge,typename MasterType::stop_edge,typename MasterType::transit_to_stop_type>(typename MasterType::transit_edge* current, typename MasterType::stop_edge* neighbor, typename MasterType::transit_to_stop_type* connection)
	//	//{
	//	//	return current->_time_cost + connection->_time_cost;
	//	//}
	//	//
	//	//template<>
	//	//float cost_between<typename MasterType::stop_edge,typename MasterType::transit_edge,typename MasterType::stop_to_transit_type>(typename MasterType::stop_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::stop_to_transit_type* connection)
	//	//{
	//	//	// have to wait at the stop until the trip leaves
	//	//	return connection->_departure_time - current->_time_label;
	//	//}

	//	template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
	//	float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
	//	{
	//		// moe lookup
	//		int current_time = current->time_label();
	//		float* moe_ptr = current->moe_ptr();

	//		if(moe_ptr != nullptr)
	//		{
	//			float time_cost = current->moe_data()->get_closest_element(moe_ptr,current_time);

	//			if(neighbor->_is_highway)
	//			{
	//				//t_pointer(float,moe_ptr);
	//				//static t_pointer(Layered_Data_Array<float>,moe_data);

	//				return time_cost*.75f;
	//			}
	//			else
	//			{

	//				return time_cost;
	//			}
	//		}
	//		else
	//		{

	//			if(neighbor->_is_highway)
	//			{
	//				return (current->_time_cost + connection->_time_cost)*.75f;
	//			}
	//			else
	//			{
	//				return current->_time_cost + connection->_time_cost;
	//			}
	//			
	//		}
	//	}

	//	//template<>
	//	//float time_cost_between<typename MasterType::stop_edge,typename MasterType::transit_edge,typename MasterType::stop_to_transit_type>(typename MasterType::stop_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::stop_to_transit_type* connection)
	//	//{
	//	//	// have to wait at the stop until the trip leaves
	//	//	return connection->_departure_time - current->_time_label;
	//	//}

	//	template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
	//	void update_label(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){}

	//	//template<>
	//	//void update_label<typename MasterType::transit_edge,typename MasterType::transit_edge,typename MasterType::ride_on_type>(typename MasterType::transit_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::ride_on_type* connection)
	//	//{
	//	//	neighbor->_transfers_made = current->_transfers_made;
	//	//	neighbor->_walk_transfers_made = current->_walk_transfers_made;
	//	//}
	//	//
	//	//template<>
	//	//void update_label<typename MasterType::transit_edge,typename MasterType::stop_edge,typename MasterType::transit_to_stop_type>(typename MasterType::transit_edge* current, typename MasterType::stop_edge* neighbor, typename MasterType::transit_to_stop_type* connection)
	//	//{
	//	//	neighbor->_transfers_made = current->_transfers_made;
	//	//	neighbor->_walk_transfers_made = current->_walk_transfers_made;
	//	//}

	//	//template<>
	//	//void update_label<typename MasterType::stop_edge,typename MasterType::transit_edge,typename MasterType::stop_to_transit_type>(typename MasterType::stop_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::stop_to_transit_type* connection)
	//	//{
	//	//	neighbor->_transfers_made = current->_transfers_made;
	//	//	neighbor->_walk_transfers_made = current->_walk_transfers_made;
	//	//}

	//	//template<>
	//	//void update_label<typename MasterType::transit_edge,typename MasterType::transit_edge,typename MasterType::transfer_at_stop_type>(typename MasterType::transit_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::transfer_at_stop_type* connection)
	//	//{
	//	//	neighbor->_transfers_made = current->_transfers_made + 1;
	//	//	neighbor->_walk_transfers_made = current->_walk_transfers_made;
	//	//}

	//	//template<>
	//	//void update_label<typename MasterType::transit_edge,typename MasterType::transit_edge,typename MasterType::walk_transfer_type>(typename MasterType::transit_edge* current, typename MasterType::transit_edge* neighbor, typename MasterType::walk_transfer_type* connection)
	//	//{
	//	//	neighbor->_transfers_made = current->_transfers_made + 1;
	//	//	neighbor->_walk_transfers_made = current->_walk_transfers_made;
	//	//}

	//	//template<>
	//	//void update_label<typename MasterType::stop_edge,typename MasterType::stop_edge,typename MasterType::stop_to_stop_type>(typename MasterType::stop_edge* current, typename MasterType::stop_edge* neighbor, typename MasterType::stop_to_stop_type* connection)
	//	//{
	//	//	neighbor->_transfers_made = current->_transfers_made;
	//	//	neighbor->_walk_transfers_made = current->_walk_transfers_made + 1;
	//	//}

	//	t_data(float,wait_horizon);
	//};
}