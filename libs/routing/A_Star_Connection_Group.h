#pragma once
#include "Connection_Group_Implementation.h"
#include "A_Star_Edge.h"
#include "Network_DB_Reader_Implementations.h"

namespace polaris
{
	template<typename MasterType>
	struct Custom_Connection_Group<MasterType,typename MasterType::static_graph_type,typename MasterType::static_graph_type,typename MasterType::static_to_static_type>
		: public Connection_Group_Base< MasterType, typename MasterType::static_graph_type, typename MasterType::static_graph_type, typename MasterType::static_to_static_type >
	{
		typedef Connection_Group_Base< MasterType, typename MasterType::static_graph_type, typename MasterType::static_graph_type, typename MasterType::static_to_static_type > Base_t;
		typedef typename Base_t::Anonymous_Connection_Group Anonymous_Connection_Group;
		typedef typename Base_t::current_edge_type current_edge_type;
		typedef typename Base_t::base_edge_type base_edge_type;
		typedef typename Base_t::connection_type connection_type;
		typedef typename Base_t::Connection_Implementation Connection_Implementation;
		typedef typename Base_t::neighbor_edge_type neighbor_edge_type;
		typedef typename Base_t::connection_attributes_type connection_attributes_type;

		template<typename AgentType>
		Anonymous_Connection_Group* Visit_Neighbors(Routable_Agent<AgentType>* agent, current_edge_type* current, Routing_Data<base_edge_type>& routing_data)
		{
			//end_forward_edges is a member functon of Connection_Group_Base and returns the end of the current connection group
			const Connection_Implementation* const end_connection_itr = this->end_forward_edges();

			for(Connection_Implementation* connection_itr = this->forward_edges(); connection_itr != end_connection_itr; ++connection_itr)
			{
				Evaluate_Neighbor<AgentType>(agent,current,connection_itr,routing_data);
			}

			return (Anonymous_Connection_Group*)end_connection_itr;
		}
				
		//basic A* stuff
		template<typename AgentType>
		void Evaluate_Neighbor(Routable_Agent<AgentType>* agent, current_edge_type* current, connection_type* connection, Routing_Data<base_edge_type>& routing_data)
		{
			A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection->neighbor();

			if(current_neighbor->in_closed_set()) return;

			A_Star_Edge<current_edge_type>* current_edge = (A_Star_Edge<current_edge_type>*)current;

			float time_cost_between = agent->time_cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);

			//float cost_from_origin = current->cost_from_origin() + agent->cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
			float cost_from_origin = current->cost_from_origin() + time_cost_between;

			if( cost_from_origin < current_neighbor->cost_from_origin() )
			{
				if (current_neighbor->in_open_set())
				{
					routing_data.open_set->erase(routing_data.open_set->iterator_to(*((base_edge_type*)current_neighbor)));
				}

				//float time_cost_between = agent->time_cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
				float time_from_origin = current->time_from_origin() + time_cost_between;

				if(!current_neighbor->marked_for_reset())
				{
					routing_data.modified_edges->push_back((base_edge_type*)current_neighbor);
					current_neighbor->marked_for_reset(true);
				}
				
				current_neighbor->came_from( current );

				current_neighbor->cost_from_origin( cost_from_origin );
				current_neighbor->time_from_origin( time_from_origin );
				
				current_neighbor->time_label( current_edge->time_label() + time_cost_between );

				float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, *(routing_data.ends));

				current_neighbor->estimated_cost_origin_destination( neighbor_estimated_cost_origin_destination );
				
				routing_data.open_set->insert( *((base_edge_type*)current_neighbor) );

				current_neighbor->in_open_set(true);

				// update the label
				//agent->update_label(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
			}
		}
	};

	template<typename MasterType>
	struct Custom_Connection_Group<MasterType, typename MasterType::multimodal_graph_type, typename MasterType::multimodal_graph_type, typename MasterType::multimodal_to_multimodal_type>
		: public Connection_Group_Base< MasterType, typename MasterType::multimodal_graph_type, typename MasterType::multimodal_graph_type, typename MasterType::multimodal_to_multimodal_type >
	{
		typedef Connection_Group_Base< MasterType, typename MasterType::multimodal_graph_type, typename MasterType::multimodal_graph_type, typename MasterType::multimodal_to_multimodal_type > Base_t;
		typedef typename Base_t::Anonymous_Connection_Group Anonymous_Connection_Group;
		typedef typename Base_t::current_edge_type current_edge_type;
		typedef typename Base_t::base_edge_type base_edge_type;
		typedef typename Base_t::connection_type connection_type;
		typedef typename Base_t::Connection_Implementation Connection_Implementation;
		typedef typename Base_t::neighbor_edge_type neighbor_edge_type;
		typedef typename Base_t::connection_attributes_type connection_attributes_type;

		typedef Network<typename MasterType::network_type> Network_Interface;

		typedef Scenario<typename MasterType::scenario_type> Scenario_Interface;

		typedef Link_Components::Prototypes::Link<typename remove_pointer< typename Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
		typedef Random_Access_Sequence< typename Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface;

		typedef Transit_Vehicle_Trip_Components::Prototypes::Transit_Vehicle_Trip<typename remove_pointer< typename Network_Interface::get_type_of(transit_vehicle_trips_container)::value_type>::type>  _Transit_Vehicle_Trip_Interface;
		typedef Random_Access_Sequence< typename Network_Interface::get_type_of(transit_vehicle_trips_container), _Transit_Vehicle_Trip_Interface*> _Transit_Vehicle_Trips_Container_Interface;

		typedef Transit_Pattern_Components::Prototypes::Transit_Pattern<typename remove_pointer< typename Network_Interface::get_type_of(transit_patterns_container)::value_type>::type>  _Transit_Pattern_Interface;
		typedef typename Network_Interface::get_type_of(transit_patterns_container) _Transit_Patterns_Container_Interface;

		typedef typename _Transit_Pattern_Interface::get_type_of(pattern_trips) _Pattern_Trips_Container_Interface;
		typedef typename _Transit_Vehicle_Trip_Interface::get_type_of(departure_seconds) _Departure_Seconds_Container_Interface;
		typedef typename _Transit_Vehicle_Trip_Interface::get_type_of(arrival_seconds) _Arrival_Seconds_Container_Interface;

		template<typename AgentType>
		Anonymous_Connection_Group* Visit_Neighbors(Routable_Agent<AgentType>* agent, current_edge_type* current, Routing_Data<base_edge_type>& routing_data)
		{
			//end_forward_edges is a member functon of Connection_Group_Base and returns the end of the current connection group
			const Connection_Implementation* const end_connection_itr = this->end_forward_edges();

			for (Connection_Implementation* connection_itr = this->forward_edges(); connection_itr != end_connection_itr; ++connection_itr)
			{
				Evaluate_Neighbor<AgentType>(agent, current, connection_itr, routing_data);
			}

			return (Anonymous_Connection_Group*)end_connection_itr;
		}

		//basic A* stuff
		template<typename AgentType>
		void Evaluate_Neighbor(Routable_Agent<AgentType>* agent, current_edge_type* current, connection_type* connection, Routing_Data<base_edge_type>& routing_data)
		{
			A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection->neighbor();

			if (current_neighbor->in_closed_set()) return;

			float time_cost_between = agent->time_cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);

			//float cost_from_origin = current->cost_from_origin() + agent->cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
			float cost_from_origin = current->cost_from_origin() + time_cost_between;

			if (cost_from_origin < current_neighbor->cost_from_origin())
			{
				if (current_neighbor->in_open_set()) routing_data.open_set->erase(routing_data.open_set->iterator_to(*((base_edge_type*)current_neighbor)));
								
				if (!current_neighbor->marked_for_reset())
				{
					routing_data.modified_edges->push_back((base_edge_type*)current_neighbor);
					current_neighbor->marked_for_reset(true);
				}

				current_neighbor->came_from(current);
				current_neighbor->cost_from_origin(cost_from_origin);
				current_neighbor->estimated_cost_origin_destination(cost_from_origin);

				routing_data.open_set->insert(*((base_edge_type*)current_neighbor));

				current_neighbor->in_open_set(true);

				// update the label
				//agent->update_label(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
			}
		}


		template<typename AgentType>
		Anonymous_Connection_Group* Visit_Multimodal_Neighbors(Routable_Agent<AgentType>* agent, current_edge_type* current, Routing_Data<base_edge_type>& routing_data, Graph_Pool<graph_pool_type>* graph_pool)
		{
			//end_forward_edges is a member functon of Connection_Group_Base and returns the end of the current connection group
			const Connection_Implementation* const end_connection_itr = this->end_forward_edges();

			for (Connection_Implementation* connection_itr = this->forward_edges(); connection_itr != end_connection_itr; ++connection_itr)
			{
				
				Link_Components::Types::Link_Type_Keys current_type = current->_edge_type;
				A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection_itr->neighbor();
				Link_Components::Types::Link_Type_Keys current_neighbor_type = current_neighbor->_edge_type;
				Vehicle_Components::Types::Vehicle_Type_Keys sub_mode = routing_data.sub_mode;

				if (current_neighbor_type == Link_Components::Types::Link_Type_Keys::TRANSIT && sub_mode == Vehicle_Components::Types::Vehicle_Type_Keys::BUS)
				{
					//Evaluate_Transit_Neighbor_Seq<AgentType>(agent, current, connection_itr, routing_data, graph_pool);
					Evaluate_Transit_Neighbor<AgentType>(agent, current, connection_itr, routing_data);
				}
				else if (current_neighbor_type == Link_Components::Types::Link_Type_Keys::WALK && sub_mode != Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE)
				{
 					Evaluate_Walk_Neighbor<AgentType>(agent, current, connection_itr, routing_data);
				}
				else if (current_neighbor_type == Link_Components::Types::Link_Type_Keys::WALK && sub_mode == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE)
				{
					Evaluate_Bike_Neighbor<AgentType>(agent, current, connection_itr, routing_data);
				}
				else if (current_type != Link_Components::Types::Link_Type_Keys::TRANSIT && current_type != Link_Components::Types::Link_Type_Keys::WALK && sub_mode == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE)
				//else
				{
					Evaluate_Drive_Neighbor<AgentType>(agent, current, connection_itr, routing_data);
				}

			}

			return (Anonymous_Connection_Group*)end_connection_itr;
		}
				
		template<typename AgentType>
		void Evaluate_Transit_Neighbor_Seq(Routable_Agent<AgentType>* agent, current_edge_type* current, connection_type* connection, Routing_Data<base_edge_type>& routing_data, Graph_Pool<graph_pool_type>* graph_pool)
		{
			A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection->neighbor();

			float transferPenalty = routing_data.transferPenalty;
			float waitWeight = routing_data.waitWeight;
			float ivtWeight = routing_data.ivtWeight;
			float waitThreshold_Time = routing_data.waitThreshold_Time;
			bool multimodal_dijkstra = routing_data.multimodal_dijkstra;
			float walkSpeed_fps = routing_data.walkSpeed_fps;
			float bikeSpeed_fps = routing_data.bikeSpeed_fps;
			
			//if (current_neighbor->in_closed_set()) return;

			int patterns_size = current_neighbor->_unique_patterns.size();

			int patterns_ctr = 0;
			while (patterns_ctr < patterns_size)
			{
				_Transit_Pattern_Interface* next_pattern = (_Transit_Pattern_Interface*)current_neighbor->_unique_patterns[patterns_ctr];
				int mySeq = current_neighbor->_index_along_pattern_at_upstream_node[patterns_ctr];

				int trips_size = next_pattern->_pattern_trips.size();
				int trips_ctr = 0;
				bool trip_found = false;
				while (trips_ctr < trips_size && !trip_found)
				{
					_Transit_Vehicle_Trip_Interface* next_trip = (_Transit_Vehicle_Trip_Interface*)next_pattern->_pattern_trips[trips_ctr];

					++trips_ctr;

					int wait_binary = 1;
					float waitTime = (float)next_trip->_departure_seconds[mySeq] - current->_time_label;

					if (waitTime < 0)
					{
						continue;
					}

					//Since trips are sorted chronologically by departure time, no need to scan beyond this threshold
					if (waitTime > waitThreshold_Time)
					{
						break;
					}

					trip_found = true;

					Link_Components::Types::Link_Type_Keys current_type = current->_edge_type;
					if (current_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
					{
						_Transit_Vehicle_Trip_Interface* current_trip = static_cast<_Transit_Vehicle_Trip_Interface*>(current->_came_on_trip);
						if (current_trip->_uuid == next_trip->_uuid)
						{
							wait_binary = 0;
						}
					}

					int WaitingCount = current->_wait_count_from_origin + wait_binary;

					int TransferCount = std::max(WaitingCount - 1, 0);
					/*int nonHomeWait = 0;
					if (TransferCount > 0)
					{
						nonHomeWait = 1;
						if (waitTime > waitThreshold)
						{
							break;
						}
					}*/

					float effectiveTransferPen = TransferCount * wait_binary * transferPenalty;

					bool seqStay = true;
					bool hit_dest = false;

					int iSeq = mySeq + 1;
					int pattern_links_size = next_pattern->_pattern_links.size();
					while (iSeq <= pattern_links_size && hit_dest == false && seqStay == true)
					{
						global_edge_id seq_edge_g;
						seq_edge_g.graph_id = 1;
						seq_edge_g.edge_id = next_pattern->_pattern_edge_ids[iSeq - 1];

						A_Star_Edge<neighbor_edge_type>* seq_edge = (A_Star_Edge<neighbor_edge_type>*)graph_pool->Get_Edge(seq_edge_g);

						float ivtTime;
						if (wait_binary == 1)
						{
							ivtTime = (float)next_trip->_arrival_seconds[iSeq] - (float)next_trip->_departure_seconds[mySeq];
						}
						else
						{
							ivtTime = (float)next_trip->_arrival_seconds[iSeq] - current->_time_label;
						}

						float cost_from_origin = current->cost_from_origin() + waitWeight*wait_binary*waitTime + ivtWeight*ivtTime + effectiveTransferPen;

						if (cost_from_origin < seq_edge->cost_from_origin())
						{
							seq_edge->cost_from_origin(cost_from_origin);

							float time_from_origin = current->time_from_origin() + wait_binary*waitTime + ivtTime;

							seq_edge->time_from_origin(time_from_origin);
							seq_edge->time_label((float)next_trip->_arrival_seconds[iSeq]);

							seq_edge->came_from(current);
							seq_edge->_came_on_trip = next_trip;
							seq_edge->_came_on_seq_index = iSeq;
							seq_edge->_wait_count_from_origin = WaitingCount;
							seq_edge->_wait_time_from_origin = current->_wait_time_from_origin + wait_binary * waitTime;
							seq_edge->_walk_time_from_origin = current->_walk_time_from_origin;
							seq_edge->_bike_time_from_origin = current->_bike_time_from_origin;
							seq_edge->_ivt_time_from_origin = current->_ivt_time_from_origin + ivtTime;
							seq_edge->_car_time_from_origin = current->_car_time_from_origin;
							seq_edge->_transfer_pen_from_origin = current->_transfer_pen_from_origin + effectiveTransferPen;

							float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)seq_edge, *(routing_data.ends), multimodal_dijkstra);
							seq_edge->estimated_cost_origin_destination(neighbor_estimated_cost_origin_destination);

							if (!seq_edge->marked_for_reset())
							{
								routing_data.modified_edges->push_back((base_edge_type*)seq_edge);
								seq_edge->marked_for_reset(true);
							}

							if (seq_edge->in_open_set())
							{
								routing_data.open_set->erase(routing_data.open_set->iterator_to(*((base_edge_type*)seq_edge)));
							}
							routing_data.open_set->insert(*((base_edge_type*)seq_edge));
							seq_edge->in_open_set(true);

							//agent->update_label(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
						}
						else
						{
							seqStay = false;
						}

						/*if (agent->at_destination((base_edge_type*)seq_edge, *(routing_data.end_transit_edges)))
						{
							hit_dest = true;
						}*/

						iSeq++;
					}
				}
				++patterns_ctr;
			}
		}		

		template<typename AgentType>
		void Evaluate_Transit_Neighbor(Routable_Agent<AgentType>* agent, current_edge_type* current, connection_type* connection, Routing_Data<base_edge_type>& routing_data)
		{
			A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection->neighbor();

			float transferPenalty = routing_data.transferPenalty;
			float waitWeight = routing_data.waitWeight;
			float ivtWeight = routing_data.ivtWeight;
			float waitThreshold_Time = routing_data.waitThreshold_Time;
			bool multimodal_dijkstra = routing_data.multimodal_dijkstra;
			float walkSpeed_fps = routing_data.walkSpeed_fps;
			float bikeSpeed_fps = routing_data.bikeSpeed_fps;

			//if (current_neighbor->in_closed_set()) return;

			int patterns_size = current_neighbor->_unique_patterns.size();			

			int patterns_ctr = 0;			
			while (patterns_ctr < patterns_size)
			{
				_Transit_Pattern_Interface* next_pattern = (_Transit_Pattern_Interface*)current_neighbor->_unique_patterns[patterns_ctr];
				int mySeq = current_neighbor->_index_along_pattern_at_upstream_node[patterns_ctr];

				int trips_size = next_pattern->pattern_trips<_Pattern_Trips_Container_Interface&>().size();
				int trips_ctr = 0;
				bool trip_found = false;

				while (trips_ctr < trips_size && !trip_found)
				{
					_Transit_Vehicle_Trip_Interface* next_trip = (_Transit_Vehicle_Trip_Interface*)next_pattern->pattern_trips<_Pattern_Trips_Container_Interface&>()[trips_ctr];
					
					++trips_ctr;

					int wait_binary = 1;
					float departure_time_here = (float)next_trip->departure_seconds<_Departure_Seconds_Container_Interface&>()[mySeq];
					float waitTime = departure_time_here - current->_time_label;

					if (waitTime < 0.0)
					{
						continue;
					}

					//Since trips are sorted chronologically by departure time, no need to scan beyond this threshold
					if (waitTime > waitThreshold_Time)
					{
						break;
					}

					trip_found = true;

					Link_Components::Types::Link_Type_Keys current_type = current->_edge_type;
					if (current_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
					{
						_Transit_Vehicle_Trip_Interface* current_trip = static_cast<_Transit_Vehicle_Trip_Interface*>(current->_came_on_trip);
						if (current_trip->uuid<int>() == next_trip->uuid<int>())
						{
							wait_binary = 0;
						}
					}

					int WaitingCount = current->_wait_count_from_origin + wait_binary;

					int TransferCount = std::max(WaitingCount - 1, 0);
					/*int nonHomeWait = 0;
					if (TransferCount > 0)
					{
						nonHomeWait = 1;
						if (waitTime > waitThreshold)
						{
							break;
						}
					}*/

					float effectiveTransferPen = TransferCount * wait_binary * transferPenalty;

					float ivtTime;
					float arrival_time_there = (float)next_trip->arrival_seconds<_Arrival_Seconds_Container_Interface&>()[mySeq + 1];
					if (wait_binary == 1)
					{
						ivtTime = arrival_time_there - departure_time_here;
					}
					else
					{
						ivtTime = arrival_time_there - current->_time_label;
					}

					float cost_from_origin = current->cost_from_origin() + waitWeight*wait_binary*waitTime + ivtWeight*ivtTime + effectiveTransferPen;

					if (cost_from_origin < current_neighbor->cost_from_origin())
					{
						current_neighbor->cost_from_origin(cost_from_origin);

						float time_from_origin = current->time_from_origin() + wait_binary*waitTime + ivtTime;

						current_neighbor->time_from_origin(time_from_origin);
						current_neighbor->time_label(arrival_time_there);

						current_neighbor->came_from(current);
						current_neighbor->_came_on_trip = next_trip;
						current_neighbor->_came_on_seq_index = mySeq + 1;
						current_neighbor->_wait_count_from_origin = WaitingCount;
						current_neighbor->_wait_time_from_origin = current->_wait_time_from_origin + wait_binary * waitTime;
						current_neighbor->_walk_time_from_origin = current->_walk_time_from_origin;
						current_neighbor->_bike_time_from_origin = current->_bike_time_from_origin;
						current_neighbor->_ivt_time_from_origin = current->_ivt_time_from_origin + ivtTime;
						current_neighbor->_car_time_from_origin = current->_car_time_from_origin;
						current_neighbor->_transfer_pen_from_origin = current->_transfer_pen_from_origin + effectiveTransferPen;

						float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, *(routing_data.ends), multimodal_dijkstra);
						current_neighbor->estimated_cost_origin_destination(neighbor_estimated_cost_origin_destination);

						if (!current_neighbor->marked_for_reset())
						{
							routing_data.modified_edges->push_back((base_edge_type*)current_neighbor);
							current_neighbor->marked_for_reset(true);
						}

						if (current_neighbor->in_open_set()) routing_data.open_set->erase(routing_data.open_set->iterator_to(*((base_edge_type*)current_neighbor)));
						routing_data.open_set->insert(*((base_edge_type*)current_neighbor));
						current_neighbor->in_open_set(true);

						//agent->update_label(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
					}
				}
				++patterns_ctr;
			}
		}

		template<typename AgentType>
		void Evaluate_Walk_Neighbor(Routable_Agent<AgentType>* agent, current_edge_type* current, connection_type* connection, Routing_Data<base_edge_type>& routing_data)
		{
			A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection->neighbor();

			//if (current_neighbor->in_closed_set()) return;						
						
			float walkWeight = routing_data.walkWeight;
			float walkThreshold_Time = routing_data.walkThreshold_Time;
			bool multimodal_dijkstra = routing_data.multimodal_dijkstra;
			float walkSpeed_fps = routing_data.walkSpeed_fps;
			float bikeSpeed_fps = routing_data.bikeSpeed_fps;
			
			float cost_from_origin = current->cost_from_origin() + walkWeight*current_neighbor->_time_cost;
			//float cost_from_origin = current->cost_from_origin() + walkWeight*current_neighbor->_time_cost;

			if (current->_walk_time_from_origin + current_neighbor->_time_cost > walkThreshold_Time)
			{
				return;
			}

			if (cost_from_origin < current_neighbor->cost_from_origin())
			{	
				current_neighbor->cost_from_origin(cost_from_origin);

				float time_cost_between = current_neighbor->_time_cost;
				//float time_cost_between = current_neighbor->_time_cost;
				current_neighbor->time_from_origin(current->time_from_origin() + time_cost_between);
				current_neighbor->time_label(current->time_label() + time_cost_between);					
					
				current_neighbor->came_from(current);
				current_neighbor->_came_on_seq_index = -1;
				current_neighbor->_wait_count_from_origin = current->_wait_count_from_origin;
				current_neighbor->_wait_time_from_origin = current->_wait_time_from_origin;
				current_neighbor->_walk_time_from_origin = current->_walk_time_from_origin + time_cost_between;
				current_neighbor->_bike_time_from_origin = current->_bike_time_from_origin;
				current_neighbor->_ivt_time_from_origin = current->_ivt_time_from_origin;
				current_neighbor->_car_time_from_origin = current->_car_time_from_origin;
				current_neighbor->_transfer_pen_from_origin = current->_transfer_pen_from_origin;
					
				float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, *(routing_data.ends), multimodal_dijkstra);
				current_neighbor->estimated_cost_origin_destination(neighbor_estimated_cost_origin_destination);

				if (!current_neighbor->marked_for_reset())
				{
					routing_data.modified_edges->push_back((base_edge_type*)current_neighbor);
					current_neighbor->marked_for_reset(true);
				}

				if (current_neighbor->in_open_set()) routing_data.open_set->erase(routing_data.open_set->iterator_to(*((base_edge_type*)current_neighbor)));
				routing_data.open_set->insert(*((base_edge_type*)current_neighbor));
				current_neighbor->in_open_set(true);

				// update the label
				//agent->update_label(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);					
			}
			
		}

		template<typename AgentType>
		void Evaluate_Bike_Neighbor(Routable_Agent<AgentType>* agent, current_edge_type* current, connection_type* connection, Routing_Data<base_edge_type>& routing_data)
		{
			A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection->neighbor();

			//if (current_neighbor->in_closed_set()) return;						

			float bikeWeight = routing_data.bikeWeight;
			float bikeThreshold_Time = routing_data.bikeThreshold_Time;
			float bike_time_factor = routing_data.bike_time_factor;
			bool multimodal_dijkstra = routing_data.multimodal_dijkstra;
			float walkSpeed_fps = routing_data.walkSpeed_fps;
			float bikeSpeed_fps = routing_data.bikeSpeed_fps;

			float cost_from_origin = current->cost_from_origin() + bike_time_factor*bikeWeight*current_neighbor->_time_cost;
			//float cost_from_origin = current->cost_from_origin() + bikeWeight*current_neighbor->_time_cost;

			if (current->_bike_time_from_origin + bike_time_factor*current_neighbor->_time_cost > bikeThreshold_Time)
				//if (current->_bike_time_from_origin + current_neighbor->_time_cost > bikeThreshold_Time)
			{
				return;
			}

			if (cost_from_origin < current_neighbor->cost_from_origin())
			{
				current_neighbor->cost_from_origin(cost_from_origin);

				float time_cost_between = bike_time_factor*current_neighbor->_time_cost;
				//float time_cost_between = current_neighbor->_time_cost;
				current_neighbor->time_from_origin(current->time_from_origin() + time_cost_between);
				current_neighbor->time_label(current->time_label() + time_cost_between);

				current_neighbor->came_from(current);
				current_neighbor->_came_on_seq_index = -1;
				current_neighbor->_wait_count_from_origin = current->_wait_count_from_origin;
				current_neighbor->_wait_time_from_origin = current->_wait_time_from_origin;
				current_neighbor->_walk_time_from_origin = current->_walk_time_from_origin;
				current_neighbor->_bike_time_from_origin = current->_bike_time_from_origin + time_cost_between;
				current_neighbor->_ivt_time_from_origin = current->_ivt_time_from_origin;
				current_neighbor->_car_time_from_origin = current->_car_time_from_origin;
				current_neighbor->_transfer_pen_from_origin = current->_transfer_pen_from_origin;

				float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, *(routing_data.ends), multimodal_dijkstra);
				current_neighbor->estimated_cost_origin_destination(neighbor_estimated_cost_origin_destination);

				if (!current_neighbor->marked_for_reset())
				{
					routing_data.modified_edges->push_back((base_edge_type*)current_neighbor);
					current_neighbor->marked_for_reset(true);
				}

				if (current_neighbor->in_open_set()) routing_data.open_set->erase(routing_data.open_set->iterator_to(*((base_edge_type*)current_neighbor)));
				routing_data.open_set->insert(*((base_edge_type*)current_neighbor));
				current_neighbor->in_open_set(true);

				// update the label
				//agent->update_label(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);					
			}

		}

		template<typename AgentType>
		void Evaluate_Drive_Neighbor(Routable_Agent<AgentType>* agent, current_edge_type* current, connection_type* connection, Routing_Data<base_edge_type>& routing_data)
		{
			A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection->neighbor();

			//if (current_neighbor->in_closed_set()) return;
						
			float carWeight = routing_data.carWeight;
			bool multimodal_dijkstra = routing_data.multimodal_dijkstra;
			float walkSpeed_fps = routing_data.walkSpeed_fps;
			float bikeSpeed_fps = routing_data.bikeSpeed_fps;
			
			float time_cost_between = agent->time_cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);

			float heuristicPortion = agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, *(routing_data.ends), multimodal_dijkstra);
			if (carWeight*(current->_car_time_from_origin + time_cost_between) > heuristicPortion)
			{
				return;
			}				

			float cost_from_origin = current->cost_from_origin() + carWeight * time_cost_between;

			if (cost_from_origin < current_neighbor->cost_from_origin())
			{				
				current_neighbor->cost_from_origin(cost_from_origin);

				current_neighbor->time_from_origin(current->time_from_origin() + time_cost_between);
				current_neighbor->time_label(current->time_label() + time_cost_between);

				current_neighbor->came_from(current);
				current_neighbor->_came_on_seq_index = -1;
				current_neighbor->_wait_count_from_origin = current->_wait_count_from_origin;
				current_neighbor->_wait_time_from_origin = current->_wait_time_from_origin;
				current_neighbor->_walk_time_from_origin = current->_walk_time_from_origin;
				current_neighbor->_bike_time_from_origin = current->_bike_time_from_origin;
				current_neighbor->_ivt_time_from_origin = current->_ivt_time_from_origin;
				current_neighbor->_car_time_from_origin = current->_car_time_from_origin + time_cost_between;
				current_neighbor->_transfer_pen_from_origin = current->_transfer_pen_from_origin;

				float neighbor_estimated_cost_origin_destination = cost_from_origin + heuristicPortion;
				current_neighbor->estimated_cost_origin_destination(neighbor_estimated_cost_origin_destination);			

				if (!current_neighbor->marked_for_reset())
				{
					routing_data.modified_edges->push_back((base_edge_type*)current_neighbor);
					current_neighbor->marked_for_reset(true);
				}

				if (current_neighbor->in_open_set()) routing_data.open_set->erase(routing_data.open_set->iterator_to(*((base_edge_type*)current_neighbor)));
				routing_data.open_set->insert(*((base_edge_type*)current_neighbor));
				current_neighbor->in_open_set(true);

				// update the label
				//agent->update_label(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);					
			}			
		}
	};

	template<typename MasterType>
	struct Custom_Connection_Group<MasterType,typename MasterType::time_dependent_graph_type,typename MasterType::time_dependent_graph_type,typename MasterType::time_dependent_to_time_dependent_type>
		: public Connection_Group_Base< MasterType, typename MasterType::time_dependent_graph_type, typename MasterType::time_dependent_graph_type, typename MasterType::time_dependent_to_time_dependent_type >
	{
		typedef Connection_Group_Base< MasterType, typename MasterType::time_dependent_graph_type, typename MasterType::time_dependent_graph_type, typename MasterType::time_dependent_to_time_dependent_type > Base_t;
		typedef typename Base_t::Anonymous_Connection_Group Anonymous_Connection_Group;
		typedef typename Base_t::current_edge_type current_edge_type;
		typedef typename Base_t::base_edge_type base_edge_type;
		typedef typename Base_t::connection_type connection_type;
		typedef typename Base_t::Connection_Implementation Connection_Implementation;
		typedef typename Base_t::neighbor_edge_type neighbor_edge_type;
		typedef typename Base_t::connection_attributes_type connection_attributes_type;

		template<typename AgentType>
		Anonymous_Connection_Group* Visit_Neighbors(Routable_Agent<AgentType>* agent, current_edge_type* current, Routing_Data<base_edge_type>& routing_data)
		{
			const Connection_Implementation* const end_connection_itr = this->end_forward_edges();

			for(Connection_Implementation* connection_itr = this->forward_edges(); connection_itr != end_connection_itr; ++connection_itr)
			{
				Evaluate_Neighbor<AgentType>(agent,current,connection_itr,routing_data);
			}

			return (Anonymous_Connection_Group*)end_connection_itr;
		}

		template<typename AgentType>
		void Evaluate_Neighbor(Routable_Agent<AgentType>* agent, current_edge_type* current, connection_type* connection, Routing_Data<base_edge_type>& routing_data)
		{
			A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection->neighbor();

			if(current_neighbor->in_closed_set()) return;

			A_Star_Edge<current_edge_type>* current_edge = (A_Star_Edge<current_edge_type>*)current;

			float time_cost_between = agent->time_cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);

			//float cost_from_origin = current->cost_from_origin() + agent->cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
			float cost_from_origin = current->cost_from_origin() + time_cost_between;

			if( cost_from_origin < current_neighbor->cost_from_origin() )
			{
				if( current_neighbor->in_open_set() ) routing_data.open_set->erase( routing_data.open_set->iterator_to( *((base_edge_type*)current_neighbor)  ) );

				//float time_cost_between = agent->time_cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
				float time_from_origin = current->time_from_origin() + time_cost_between;

				if(!current_neighbor->marked_for_reset())
				{
					routing_data.modified_edges->push_back((base_edge_type*)current_neighbor);
					current_neighbor->marked_for_reset(true);
				}
				
				current_neighbor->came_from( current );

				current_neighbor->cost_from_origin( cost_from_origin );
				current_neighbor->time_from_origin( time_from_origin );
				
				current_neighbor->time_label( current_edge->time_label() + time_cost_between );

				float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, *(routing_data.ends));

				current_neighbor->estimated_cost_origin_destination( neighbor_estimated_cost_origin_destination );
				
				routing_data.open_set->insert( *((base_edge_type*)current_neighbor) );

				current_neighbor->in_open_set(true);

				// update the label
				agent->update_label(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
			}
		}
	};

	//template<typename MasterType, typename Current_Graph_Type, typename Neighbor_Graph_Type, typename Connection_Attributes_Type>
	//struct Custom_Connection_Group : public Connection_Group_Base< MasterType, Current_Graph_Type, Neighbor_Graph_Type, Connection_Attributes_Type >
	//{
	//	template<typename AgentType>
	//	Anonymous_Connection_Group* Visit_Neighbors(Routable_Agent<AgentType>* agent, current_edge_type* current, Routing_Data<base_edge_type>& routing_data)
	//	{
	//		const Connection_Implementation* const end_connection_itr = end_forward_edges();

	//		if( agent->process_connection_set<current_edge_type, Connection_Attributes_Type>(current) )
	//		{
	//			//cout << "\tvisiting neighbors of connection type: " << typeid(Connection_Attributes_Type).name() << endl;

	//			for(Connection_Implementation* connection_itr = forward_edges(); connection_itr != end_connection_itr; ++connection_itr)
	//			{
	//				Evaluate_Neighbor<AgentType>(agent,current,connection_itr,routing_data);
	//			}
	//		}

	//		return (Anonymous_Connection_Group*)end_connection_itr;
	//	}

	//	template<typename AgentType>
	//	void Evaluate_Neighbor(Routable_Agent<AgentType>* agent, current_edge_type* current, connection_type* connection, Routing_Data<base_edge_type>& routing_data)
	//	{
	//		//cout << "\t" << typeid(AgentType).name() << " evaluating neighbor of connection type: " << endl;
	//		//cout << "\t\t" << typeid(connection_attributes_type).name() << endl;

	//		A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection->neighbor();

	//		if(current_neighbor->in_closed_set()) return;

	//		//cout << "\tNeighbor id: " << current_neighbor->edge_id() << endl;
	//		
	//		transit_edge_id id;
	//		
	//		id.id = current->edge_id();

	//		transit_edge_id neighbor_id;

	//		neighbor_id.id = current_neighbor->edge_id();
	//		
	//		//if(id.composite_id.trip_id == 25980 && id.composite_id.time == 70283) cout << "Evaluating: " << neighbor_id.composite_id.time << ", " << neighbor_id.composite_id.trip_id << " " << current->estimated_cost_origin_destination() << endl;
	//		//if(id.composite_id.trip_id == 25980 && id.composite_id.time == 70283) cout << "\t\tNeighbor ID: " << neighbor_id.composite_id.time << ", " << neighbor_id.composite_id.trip_id << ": cost " << agent->cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection) << "\n\t\t\t" << typeid(connection_attributes_type).name() << endl;
	//		//if(id.composite_id.trip_id == 26370 && id.composite_id.time == 71190) cout << "\t\tNeighbor ID: " << neighbor_id.composite_id.time << ", " << neighbor_id.composite_id.trip_id << ": cost " << agent->cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection) << "\n\t\t\t" << typeid(connection_attributes_type).name() << endl;
	//		//if(id.composite_id.time == 9740) cout << "\t\tNeighbor ID: " << neighbor_id.composite_id.time << ", " << neighbor_id.composite_id.trip_id << ": cost " << agent->cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection) << "\n\t\t\t" << typeid(connection_attributes_type).name() << endl;

	//		A_Star_Edge<current_edge_type>* current_edge = (A_Star_Edge<current_edge_type>*)current;

	//		float cost_from_origin = current->cost_from_origin() + agent->cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);

	//		if( cost_from_origin < current_neighbor->cost_from_origin() )
	//		{
	//			if( current_neighbor->in_open_set() ) routing_data.open_set->erase( routing_data.open_set->iterator_to( *((base_edge_type*)current_neighbor)  ) );

	//			float time_cost_between = agent->time_cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
	//			//float time_from_origin = current->time_from_origin() + time_cost_between;

	//			if(!current_neighbor->marked_for_reset())
	//			{
	//				routing_data.modified_edges->push_back((base_edge_type*)current_neighbor);
	//				current_neighbor->marked_for_reset(true);
	//			}
	//			
	//			current_neighbor->came_from( current );

	//			current_neighbor->cost_from_origin( cost_from_origin );
	//			//current_neighbor->time_from_origin( time_from_origin );
	//			
	//			current_neighbor->time_label( current_edge->time_label() + time_cost_between );

	//			float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, routing_data.end_edge);

	//			//if(id.composite_id.trip_id == 25352) cout << "\t\t\tcurrent time: " << current_neighbor->time_label() << ", current cost: " << cost_from_origin << ", estimated cost OD: " << neighbor_estimated_cost_origin_destination << endl;

	//			current_neighbor->estimated_cost_origin_destination( neighbor_estimated_cost_origin_destination );
	//			
	//			routing_data.open_set->insert( *((base_edge_type*)current_neighbor) );

	//			current_neighbor->in_open_set(true);

	//			// update the label
	//			agent->update_label(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
	//		}
	//	}
	//};

}