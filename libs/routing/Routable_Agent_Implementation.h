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
		float estimated_cost_between(CurrentEdgeType* current, std::vector<Base_Edge_A_Star<MasterType>*>& destinations)
		{
			float cost = 0;
			int dest_ctr = 0;
			for (auto itr = destinations.begin(); itr != destinations.end(); ++itr)
			{
				Base_Edge_A_Star<MasterType>* itr_destination = (Base_Edge_A_Star<MasterType>*)(*itr);
				float x_dist = current->_x - itr_destination->_x;
				x_dist *= x_dist;

				float y_dist = current->_y - itr_destination->_y;
				y_dist *= y_dist;


				// vehicle speed in fps
				float temp_cost = sqrt(x_dist + y_dist) / 89.0f;
				cost = cost + (temp_cost - cost) / (dest_ctr + 1);
				dest_ctr++;
			}
			return cost;
		}
						
		float estimated_cost_between(typename MT::multimodal_edge_type* current, std::vector<Base_Edge_A_Star<MasterType>*>& destinations, bool multimodal_dijkstra)
		{
			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> Network_Interface;
			Network_Interface* net = (Network_Interface*)_global_network;

			typedef Link_Components::Prototypes::Link<typename remove_pointer< typename Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
			typedef Random_Access_Sequence< typename Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface;
			typedef Random_Access_Sequence<typename _Link_Interface::get_type_of(heur_cost_to_dest)> _Heuristic_Cost_Container_Interface;

			if (!multimodal_dijkstra)
			{
				float cost = 0;
				int dest_ctr = 0;
				for (auto itr = destinations.begin(); itr != destinations.end(); ++itr)
				{
					Base_Edge_A_Star<MasterType>* itr_destination = (Base_Edge_A_Star<MasterType>*)(*itr);
					float x_dist = current->_x - itr_destination->_x;
					x_dist *= x_dist;

					float y_dist = current->_y - itr_destination->_y;
					y_dist *= y_dist;


					// vehicle speed in fps
					float temp_cost = sqrt(x_dist + y_dist) / 89.0f;
					cost = cost + (temp_cost - cost) / (dest_ctr + 1);
					dest_ctr++;
				}
				return cost;
			}
			else
			{
				float cost = 0;
				int dest_ctr = 0;
				for (auto itr = destinations.begin(); itr != destinations.end(); ++itr)
				{
					Base_Edge_A_Star<MasterType>* itr_destination = (Base_Edge_A_Star<MasterType>*)(*itr);					
					_Link_Interface* destination_link = (_Link_Interface*)itr_destination->_source_link;
					_Link_Interface* current_link = (_Link_Interface*)current->_source_link;

					float temp_cost = destination_link->heur_cost_to_dest<_Heuristic_Cost_Container_Interface&>()[current_link->zone_index<int>()];
					cost = cost + (temp_cost - cost) / (dest_ctr + 1);
					dest_ctr++;
				}
				return cost;
			}

		}
		
		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			if(neighbor->_is_highway)
			{
				//cout << "is highway" << endl;
				//return (current->_cost + connection->_cost)*.75f;
				//TODO Omer -> Might want to change it back!
				//return (current->_cost + connection->_cost)*.95f;
				return (connection->_cost + neighbor->_cost)*.95f;
			}
			else
			{
				//cout << "is not highway" << endl;
				//TODO Omer -> Might want to change it back!
				//return current->_cost + connection->_cost;
				return (connection->_cost + neighbor->_cost);
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
				//float ttime_accumulation = 0;
				//float ttime_step = connection->turn_moe_data()->layer_step<float>();
				//ttime_step = ttime_step - current_time % (int)ttime_step;

				// interpolate between current and next time step
				float t = FLT_MAX;
				//TODO Omer -> Might want to change it back!
				//float t1 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
				float t1 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time) + neighbor->_cost;
				
				//float t1_weight = ttime_step / connection->turn_moe_data()->layer_step<float>();

				//if (t1_weight < 0.5) // closer to next time step
				//{
				//	float t2 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_step / t1_weight) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
				//	t = t2*(1 - t1_weight) + t1*t1_weight;
				//}
				//else // closer to previous time step
				//{
				//	float t2 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time - ttime_step / t1_weight) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
				//	t = t1*(1 - t1_weight) + t2*t1_weight;
				//}
				//
				//// if the travel time is longer than the current timestep, get the expected travel time at the estimated link departure time and interpolate between (to capture cases where the travel time is growing rapidly
				//if (t > ttime_step/t1_weight)
				//{
				//	float t3 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + t) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
				//	t = (t + t3)*0.5;
				//}
				//
				t = t1;
				

				//TODO: replace this accumulation step, which requires a lot of lookups, with an interpolation between travel time at the arrival on the link and travel time at the expected departure....
				/*if (t > ttime_step)
				{
					ttime_accumulation += ttime_step;
					t = current->_cost + connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation);
					ttime_step = connection->turn_moe_data()->layer_step<float>();
					while (t - ttime_accumulation > ttime_step)
					{
						ttime_accumulation += ttime_step;
						t = current->_cost + connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation);

					}
				}
				t = ttime_accumulation + std::max(t - ttime_accumulation, 0.0f);*/

				

				// updates to handle mixing of historical and real-time info in cost function

				//TODO Omer -> Might want to change it back!
				//float time_cost_current = current->_cost + connection->_cost;
				float time_cost_current = connection->_cost + neighbor->_cost;

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
					//TODO Omer -> Might want to change it back!
					//return (current->_cost + connection->_cost)*.95f;
					return (connection->_cost + neighbor->_cost)*.95f;
				}
				else
				{
					//cout << "is not highway" << endl;
					//TODO Omer -> Might want to change it back!
					//return current->_cost + connection->_cost;
					return connection->_cost + neighbor->_cost;
				}
				
			}
		}
		
		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			//TODO Omer -> Might want to change it back!
			//return current->_time_cost + connection->_time_cost;
			return connection->_time_cost + neighbor->_time_cost;
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
				//float ttime_accumulation = 0;
				//float ttime_step = connection->turn_moe_data()->layer_step<float>();
				//ttime_step = ttime_step - current_time % (int)ttime_step;

				// interpolate between current and next time step
				float t = FLT_MAX;
				//TODO Omer -> Might want to change it back!
				//float t1 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
				float t1 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time) + neighbor->_time_cost;
				
				//float t1_weight = ttime_step / connection->turn_moe_data()->layer_step<float>();

				//if (t1_weight < 0.5) // closer to next time step
				//{
				//	float t2 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_step / t1_weight) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
				//	t = t2*(1 - t1_weight) + t1*t1_weight;
				//}
				//else // closer to previous time step
				//{
				//	float t2 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time - ttime_step / t1_weight) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
				//	t = t1*(1 - t1_weight) + t2*t1_weight;
				//}
				//
				//// if the travel time is longer than the current timestep, get the expected travel time at the estimated link departure time and interpolate between (to capture cases where the travel time is growing rapidly
				//if (t > ttime_step/t1_weight)
				//{
				//	float t3 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + t) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
				//	t = (t + t3)*0.5;
				//}
				//
				t = t1;


				//TODO: replace this accumulation step, which requires a lot of lookups, with an interpolation between travel time at the arrival on the link and travel time at the expected departure....
				/*

				// get historical time cost - update if traveler will be on link for multiple time periods
				float ttime_accumulation = 0;
				float ttime_step = connection->turn_moe_data()->layer_step<float>();
				ttime_step = ttime_step - current_time % (int)ttime_step;
				float t = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time) + current->_time_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
				if (t > ttime_step)
				{
					ttime_accumulation += ttime_step;
					t = current->_time_cost + connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation);
					ttime_step = connection->turn_moe_data()->layer_step<float>();
					while (t - ttime_accumulation > ttime_step)
					{
						ttime_accumulation += ttime_step;
						t = current->_time_cost + connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation);

					}
				}
				t = ttime_accumulation + std::max(t - ttime_accumulation, 0.0f);
				*/


				// updates to handle mixing of historical and real-time info in cost function
				//TODO Omer -> Might want to change it back!
				//float time_cost_current = current->_time_cost + connection->_time_cost;
				float time_cost_current = connection->_time_cost + neighbor->_time_cost;

				int t_diff = abs(current_time - iteration());

				// modified time dependent mixing function to be parameterized with shape and scale set in scenario
				// ttime_weight_factor allows extra control to turn off information mixing -> setting to 0 will use only historical info
				float w = (exp(-1.0*pow(((float)t_diff / current->ttime_weight_scale()), current->ttime_weight_shape())))*current->ttime_weight_factor();

				float time_cost = w*time_cost_current + (1-w)*t;


				/*if(neighbor->_is_highway)
				{
					return time_cost*.95f;
				}
				else
				{*/

				return time_cost;
				//}
			}
			else
			{

				//if(neighbor->_is_highway)
				//{
				//	//cout << "is highway" << endl;
				//	//TODO Omer -> Might want to change it back!
				//	//return (current->_time_cost + connection->_time_cost)*.95f;
				//	return (connection->_time_cost + neighbor->_time_cost)*.95f;
				//}
				//else
				//{
					//cout << "is not highway" << endl;
					//TODO Omer -> Might want to change it back!
					//return current->_time_cost + connection->_time_cost;
				return connection->_time_cost + neighbor->_time_cost;
				//}
				
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
				/*float ttime_accumulation = 0;
				float ttime_step = current->moe_data()->layer_step<float>();
				ttime_step = ttime_step - current_time % (int)ttime_step;*/

				float t = FLT_MAX;
				float t1 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time) + neighbor->_time_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
				//if (t > ttime_step)
				//{
				//	ttime_accumulation += ttime_step;
				//	t = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation) + neighbor->_time_cost;
				//	//t = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation) + neighbor->_time_cost;
				//	ttime_step = current->moe_data()->layer_step<float>();
				//	while (t - ttime_accumulation > ttime_step)
				//	{
				//		ttime_accumulation += ttime_step;
				//		t = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation) + neighbor->_time_cost;
				//		//t = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation) + neighbor->_time_cost;

				//	}
				//}
				//t = ttime_accumulation + std::max(t - ttime_accumulation, 0.0f);

				t = t1;

				// updates to handle mixing of historical and real-time info in cost function
				float time_cost_current = connection->_time_cost + neighbor->_time_cost;

				int t_diff = abs(current_time - iteration());

				// modified time dependent mixing function to be parameterized with shape and scale set in scenario
				// ttime_weight_factor allows extra control to turn off information mixing -> setting to 0 will use only historical info
				float w = (exp(-1.0*pow(((float)t_diff / current->ttime_weight_scale()), current->ttime_weight_shape())))*current->ttime_weight_factor();

				float time_cost = w*time_cost_current + (1 - w)*t;
				
				return time_cost;
				
			}
			else
			{				
				return connection->_time_cost + neighbor->_time_cost;
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
		float estimated_cost_between(CurrentEdgeType* current, std::vector<Base_Edge_A_Star<MasterType>*>& destinations)
		{
			return 0.0f;
		}

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			//TODO Omer -> Might want to change it back!
			//return current->_cost + connection->_cost;
			return connection->_cost + neighbor->_cost;
		}

		float cost_between(typename MT::time_dependent_edge_type* current, typename MT::time_dependent_edge_type* neighbor, typename MT::time_dependent_to_time_dependent_type* connection)
		{
			// moe lookup - switched from link-based (moe_ptr) to turn-based (turn_moe_ptr) - testing.....
			int current_time = current->time_label();
			//float* moe_ptr = current->moe_ptr();
			float* turn_moe_ptr = connection->turn_moe_ptr();

			if (turn_moe_ptr /*moe_ptr*/ != nullptr)
			{
				int sim_time = iteration();

				// get historical time cost - update if traveler will be on link for multiple time periods
				//float ttime_accumulation = 0;
				//float ttime_step = connection->turn_moe_data()->layer_step<float>();
				//ttime_step = ttime_step - current_time % (int)ttime_step;

				// interpolate between current and next time step
				float t = FLT_MAX;
				//TODO Omer -> Might want to change it back!
				//float t1 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
																														  //float t1_weight = ttime_step / connection->turn_moe_data()->layer_step<float>();

																														  //if (t1_weight < 0.5) // closer to next time step
																														  //{
																														  //	float t2 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_step / t1_weight) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
																														  //	t = t2*(1 - t1_weight) + t1*t1_weight;
																														  //}
																														  //else // closer to previous time step
																														  //{
																														  //	float t2 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time - ttime_step / t1_weight) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
																														  //	t = t1*(1 - t1_weight) + t2*t1_weight;
																														  //}
																														  //
																														  //// if the travel time is longer than the current timestep, get the expected travel time at the estimated link departure time and interpolate between (to capture cases where the travel time is growing rapidly
																														  //if (t > ttime_step/t1_weight)
																														  //{
																														  //	float t3 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + t) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
																														  //	t = (t + t3)*0.5;
																														  //}
																														  //
				float t1 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time) + neighbor->_cost;
				t = t1;


				//TODO: replace this accumulation step, which requires a lot of lookups, with an interpolation between travel time at the arrival on the link and travel time at the expected departure....
				/*if (t > ttime_step)
				{
				ttime_accumulation += ttime_step;
				t = current->_cost + connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation);
				ttime_step = connection->turn_moe_data()->layer_step<float>();
				while (t - ttime_accumulation > ttime_step)
				{
				ttime_accumulation += ttime_step;
				t = current->_cost + connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation);

				}
				}
				t = ttime_accumulation + std::max(t - ttime_accumulation, 0.0f);*/



				// updates to handle mixing of historical and real-time info in cost function
				//TODO Omer -> Might want to change it back!
				//float time_cost_current = current->_cost + connection->_cost;
				float time_cost_current = connection->_cost + neighbor->_cost;

				int t_diff = abs(current_time - iteration());

				// modified time dependent mixing function to be parameterized with shape and scale set in scenario
				// ttime_weight_factor allows extra control to turn off information mixing -> setting to 0 will use only historical info
				float w = (exp(-1.0*pow(((float)t_diff / current->ttime_weight_scale()), current->ttime_weight_shape())))*current->ttime_weight_factor();

				float time_cost = w*time_cost_current + (1 - w)*t;


				if (neighbor->_is_highway)
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

				if (neighbor->_is_highway)
				{
					//cout << "is highway" << endl;
					//TODO Omer -> Might want to change it back!
					//return (current->_cost + connection->_cost)*.95f;
					return (connection->_cost + neighbor->_cost)*.95f;
				}
				else
				{
					//cout << "is not highway" << endl;
					//TODO Omer -> Might want to change it back!
					//return current->_cost + connection->_cost;
					return connection->_cost + neighbor->_cost;
				}

			}
		}

		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			//TODO Omer -> Might want to change it back!
			//return current->_time_cost + connection->_time_cost;
			return connection->_time_cost + neighbor->_time_cost;
		}

		float time_cost_between(typename MT::time_dependent_edge_type* current, typename MT::time_dependent_edge_type* neighbor, typename MT::time_dependent_to_time_dependent_type* connection)
		{
			// moe lookup
			int current_time = current->time_label();
			float* turn_moe_ptr = connection->turn_moe_ptr();

			if (turn_moe_ptr != nullptr)
			{
				int sim_time = iteration();

				// get historical time cost - update if traveler will be on link for multiple time periods
				//float ttime_accumulation = 0;
				//float ttime_step = connection->turn_moe_data()->layer_step<float>();
				//ttime_step = ttime_step - current_time % (int)ttime_step;

				// interpolate between current and next time step
				float t = FLT_MAX;
				//TODO Omer -> Might want to change it back!
				//float t1 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
																														  //float t1_weight = ttime_step / connection->turn_moe_data()->layer_step<float>();

																														  //if (t1_weight < 0.5) // closer to next time step
																														  //{
																														  //	float t2 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_step / t1_weight) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
																														  //	t = t2*(1 - t1_weight) + t1*t1_weight;
																														  //}
																														  //else // closer to previous time step
																														  //{
																														  //	float t2 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time - ttime_step / t1_weight) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
																														  //	t = t1*(1 - t1_weight) + t2*t1_weight;
																														  //}
																														  //
																														  //// if the travel time is longer than the current timestep, get the expected travel time at the estimated link departure time and interpolate between (to capture cases where the travel time is growing rapidly
																														  //if (t > ttime_step/t1_weight)
																														  //{
																														  //	float t3 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + t) + current->_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
																														  //	t = (t + t3)*0.5;
																														  //}
																														  //
				float t1 = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time) + neighbor->_time_cost;
				t = t1;


				//TODO: replace this accumulation step, which requires a lot of lookups, with an interpolation between travel time at the arrival on the link and travel time at the expected departure....
				/*

				// get historical time cost - update if traveler will be on link for multiple time periods
				float ttime_accumulation = 0;
				float ttime_step = connection->turn_moe_data()->layer_step<float>();
				ttime_step = ttime_step - current_time % (int)ttime_step;
				float t = connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time) + current->_time_cost; // I believe that the edge cost (current->_cost) is always the free flow time, so do not need to lookup historical values
				if (t > ttime_step)
				{
				ttime_accumulation += ttime_step;
				t = current->_time_cost + connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation);
				ttime_step = connection->turn_moe_data()->layer_step<float>();
				while (t - ttime_accumulation > ttime_step)
				{
				ttime_accumulation += ttime_step;
				t = current->_time_cost + connection->turn_moe_data()->get_closest_element(turn_moe_ptr, current_time + ttime_accumulation);

				}
				}
				t = ttime_accumulation + std::max(t - ttime_accumulation, 0.0f);
				*/


				// updates to handle mixing of historical and real-time info in cost function
				//TODO Omer -> Might want to change it back!
				//float time_cost_current = current->_time_cost + connection->_time_cost;
				float time_cost_current = connection->_time_cost + neighbor->_time_cost;

				int t_diff = abs(current_time - iteration());

				// modified time dependent mixing function to be parameterized with shape and scale set in scenario
				// ttime_weight_factor allows extra control to turn off information mixing -> setting to 0 will use only historical info
				float w = (exp(-1.0*pow(((float)t_diff / current->ttime_weight_scale()), current->ttime_weight_shape())))*current->ttime_weight_factor();

				float time_cost = w*time_cost_current + (1 - w)*t;


				/*if (neighbor->_is_highway)
				{
					return time_cost*.95f;
				}
				else
				{*/

				return time_cost;
				//}
			}
			else
			{

				//if (neighbor->_is_highway)
				//{
				//	//cout << "is highway" << endl;
				//	//TODO Omer -> Might want to change it back!
				//	//return (current->_time_cost + connection->_time_cost)*.95f;
				//	return (connection->_time_cost + neighbor->_time_cost)*.95f;
				//}
				//else
				//{
					//cout << "is not highway" << endl;
					//TODO Omer -> Might want to change it back!
					//return current->_time_cost + connection->_time_cost;
				return connection->_time_cost + neighbor->_time_cost;
				//}

			}
		}

		template<typename CurrentEdgeType,typename NeighborEdgeType, typename ConnectionType>
		void update_label(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection){}
	};

	template<typename MasterType>
	struct Multi_Modal_Tree_Agent_Implementation
	{			
		typedef Network_Components::Prototypes::Network<typename MasterType::network_type> Network_Interface;
		Network_Interface* net = (Network_Interface*)_global_network;

		typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface;
		
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
		float estimated_cost_between(CurrentEdgeType* current, std::vector<Base_Edge_A_Star<MasterType>*>& destinations)
		{
			return 0.0f;
		}
		
		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			_Link_Interface* neighbor_link = (_Link_Interface*)neighbor->_source_link;
			return neighbor_link->_min_multi_modal_cost;
			//return neighbor->_min_multi_modal_cost;
		}

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			_Link_Interface* neighbor_link = (_Link_Interface*)neighbor->_source_link;
			return neighbor_link->template min_multi_modal_cost<float>();
			//return neighbor->_min_multi_modal_cost;
		}

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		void update_label(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection) {}
	};
	
	template<typename MasterType>
	struct Walk_to_Transit_Tree_Agent_Implementation
	{
		typedef Network_Components::Prototypes::Network<typename MasterType::network_type> Network_Interface;
		Network_Interface* net = (Network_Interface*)_global_network;

		typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface;
		
		template<typename CurrentEdgeType, typename ConnectionType>
		bool process_connection_set(CurrentEdgeType* current)
		{
			return true;
		}

		bool at_destination(Base_Edge_A_Star<MasterType>* current, Base_Edge_A_Star<MasterType>* destination)
		{
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
		float estimated_cost_between(CurrentEdgeType* current, std::vector<Base_Edge_A_Star<MasterType>*>& destinations)
		{
			return 0.0f;
		}

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			_Link_Interface* neighbor_link = (_Link_Interface*)neighbor->_source_link;
			return neighbor_link->_walk_length;
			//return neighbor->_walk_length;
		}

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			_Link_Interface* neighbor_link = (_Link_Interface*)neighbor->_source_link;
			return neighbor_link->template walk_length<float>();
			//return neighbor->_walk_length;
		}

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		void update_label(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection) {}
	};

	template<typename MasterType>
	struct Drive_to_Transit_Tree_Agent_Implementation
	{
		typedef Network_Components::Prototypes::Network<typename MasterType::network_type> Network_Interface;
		Network_Interface* net = (Network_Interface*)_global_network;

		typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface;
		
		template<typename CurrentEdgeType, typename ConnectionType>
		bool process_connection_set(CurrentEdgeType* current)
		{
			return true;
		}

		bool at_destination(Base_Edge_A_Star<MasterType>* current, Base_Edge_A_Star<MasterType>* destination)
		{
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
		float estimated_cost_between(CurrentEdgeType* current, std::vector<Base_Edge_A_Star<MasterType>*>& destinations)
		{
			return 0.0f;
		}

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			_Link_Interface* neighbor_link = (_Link_Interface*)neighbor->_source_link;
			return connection->_time_cost + neighbor_link->_drive_time;
			//return connection->_time_cost + neighbor->_drive_time;
		}

		template<typename CurrentEdgeType, typename NeighborEdgeType, typename ConnectionType>
		float time_cost_between(CurrentEdgeType* current, NeighborEdgeType* neighbor, ConnectionType* connection)
		{
			_Link_Interface* neighbor_link = (_Link_Interface*)neighbor->_source_link;
			return connection->_time_cost + neighbor_link->template drive_time<float>();
			//return connection->_time_cost + neighbor->_drive_time;
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