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

			float cost_from_origin = current->cost_from_origin() + agent->cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);

			if( cost_from_origin < current_neighbor->cost_from_origin() )
			{
				if (current_neighbor->in_open_set())
				{
					routing_data.open_set->erase(routing_data.open_set->iterator_to(*((base_edge_type*)current_neighbor)));
				}

				float time_cost_between = agent->time_cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
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

				float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, routing_data.end_edge);

				current_neighbor->estimated_cost_origin_destination( neighbor_estimated_cost_origin_destination );
				
				routing_data.open_set->insert( *((base_edge_type*)current_neighbor) );

				current_neighbor->in_open_set(true);

				// update the label
				agent->update_label(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
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

		typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface;

		typedef  Transit_Vehicle_Trip_Components::Prototypes::Transit_Vehicle_Trip<typename remove_pointer< typename Network_Interface::get_type_of(transit_vehicle_trips_container)::value_type>::type>  _Transit_Vehicle_Trip_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(transit_vehicle_trips_container), _Transit_Vehicle_Trip_Interface*> _Transit_Vehicle_Trips_Container_Interface;

		typedef  Transit_Pattern_Components::Prototypes::Transit_Pattern<typename remove_pointer< typename Network_Interface::get_type_of(transit_patterns_container)::value_type>::type>  _Transit_Pattern_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(transit_patterns_container), _Transit_Pattern_Interface*> _Transit_Patterns_Container_Interface;


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

			float cost_from_origin = current->cost_from_origin() + agent->cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);

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

				if (current_neighbor_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
				{
					Evaluate_Transit_Neighbor<AgentType>(agent, current, connection_itr, routing_data, graph_pool);
				}
				else if (current_neighbor_type == Link_Components::Types::Link_Type_Keys::WALK)
				{
					Evaluate_Walk_Neighbor<AgentType>(agent, current, connection_itr, routing_data);
				}
				else if (current_type != Link_Components::Types::Link_Type_Keys::TRANSIT && current_type != Link_Components::Types::Link_Type_Keys::WALK)
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

			float transferPenalty = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::transferPenalty<float>();
			float waitWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::waitWeight<float>();
			float ivtWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::ivtWeight<float>();
			float waitThreshold = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::waitThreshold<float>();
			
			bool multimodal_dijkstra = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::multimodal_dijkstra<bool>();
				
			//if (current_neighbor->in_closed_set()) return;

			int unique_patterns_size = current_neighbor->_unique_patterns.size();
			int patterns_ctr;
			for (patterns_ctr = 0; patterns_ctr < unique_patterns_size; patterns_ctr++)
			{
				_Transit_Pattern_Interface* next_pattern = (_Transit_Pattern_Interface*)current_neighbor->_unique_patterns[patterns_ctr];
				next_pattern->_scanned = false;
			}
			
			int trips_ctr = 0;
			patterns_ctr = 0;

			int trips_by_dep_time_size = current_neighbor->_trips_by_dep_time.size();
			
			while (trips_ctr < trips_by_dep_time_size && patterns_ctr <= unique_patterns_size)
			{						
				_Transit_Vehicle_Trip_Interface* next_trip = (_Transit_Vehicle_Trip_Interface*)current_neighbor->_trips_by_dep_time[trips_ctr];
				int mySeq = current_neighbor->_index_along_trip_at_upstream_node[trips_ctr];
				_Transit_Pattern_Interface* next_pattern = (_Transit_Pattern_Interface*) next_trip->_pattern;

				++trips_ctr;

				if (next_pattern->_scanned)
				{
					continue;
				}

				Link_Components::Types::Link_Type_Keys current_type = current->_edge_type;			

				int wait_binary = 1;											
								 
				if (current_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
				{
					_Transit_Vehicle_Trip_Interface* current_trip = (_Transit_Vehicle_Trip_Interface*)current->_came_on_trip;					
					if (current_trip->_uuid == next_trip->_uuid)
					{
						wait_binary = 0;
					}
				}

				float waitTime = next_trip->_departure_seconds[mySeq] - current->_time_label;
				if (waitTime < 0)
				{
					continue;
				}

				//Since trips are sorted chronologically by departure time, no need to scan beyond this threshold
				if (waitTime > waitThreshold)
				{
					return;
				}
				
				next_pattern->_scanned = true;
				++patterns_ctr;

				int WaitingCount = current->_wait_count_from_origin + wait_binary;

				int TransferCount = 0;
				int nonHomeWait = 0;
				if (WaitingCount > 1)
				{
					TransferCount = WaitingCount - 1;
					nonHomeWait = 1;
				}			

				float effectiveTransferPen = TransferCount * wait_binary * transferPenalty;
																				
				bool seqStay = true;
				bool hit_dest = false;

				int iSeq = mySeq + 1;
				int pattern_links_size = next_pattern->_pattern_links.size();
				while (iSeq <= pattern_links_size && hit_dest == false && seqStay == true)
				{
					A_Star_Edge<neighbor_edge_type>* seq_edge = (A_Star_Edge<neighbor_edge_type>*) next_pattern->_pattern_edges[iSeq - 1];
					
					float ivtTime;
					if (wait_binary == 1)
					{
						ivtTime = next_trip->_arrival_seconds[iSeq] - next_trip->_departure_seconds[mySeq];
					}
					else
					{
						ivtTime = next_trip->_arrival_seconds[iSeq] - current->_time_label;
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
						seq_edge->_ivt_time_from_origin = current->_ivt_time_from_origin + ivtTime;
						seq_edge->_car_time_from_origin = current->_car_time_from_origin;
						seq_edge->_transfer_pen_from_origin = current->_transfer_pen_from_origin + effectiveTransferPen;

						float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)seq_edge, routing_data.end_edge, multimodal_dijkstra);
						seq_edge->estimated_cost_origin_destination(neighbor_estimated_cost_origin_destination);									

						if (!seq_edge->marked_for_reset())
						{
							routing_data.modified_edges->push_back((base_edge_type*)seq_edge);
							seq_edge->marked_for_reset(true);
						}

						if (seq_edge->in_open_set()) routing_data.open_set->erase(routing_data.open_set->iterator_to(*((base_edge_type*)seq_edge)));
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
		}

		template<typename AgentType>
		void Evaluate_Transit_Neighbor(Routable_Agent<AgentType>* agent, current_edge_type* current, connection_type* connection, Routing_Data<base_edge_type>& routing_data, Graph_Pool<graph_pool_type>* graph_pool)
		{
			A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection->neighbor();

			float transferPenalty = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::transferPenalty<float>();
			float waitWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::waitWeight<float>();		
			float ivtWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::ivtWeight<float>();
			float waitThreshold = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::waitThreshold<float>();
			
			bool multimodal_dijkstra = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::multimodal_dijkstra<bool>();
			
			//if (current_neighbor->in_closed_set()) return;

			int unique_patterns_size = current_neighbor->_unique_patterns.size();
			int patterns_ctr;
			for (patterns_ctr = 0; patterns_ctr < unique_patterns_size; patterns_ctr++)
			{
				_Transit_Pattern_Interface* next_pattern = (_Transit_Pattern_Interface*)current_neighbor->_unique_patterns[patterns_ctr];
				next_pattern->_scanned = false;
			}

			int trips_ctr = 0;
			patterns_ctr = 0;

			int trips_by_dep_time_size = current_neighbor->_trips_by_dep_time.size();

			while (trips_ctr < trips_by_dep_time_size && patterns_ctr <= unique_patterns_size)
			{
				_Transit_Vehicle_Trip_Interface* next_trip = (_Transit_Vehicle_Trip_Interface*)current_neighbor->_trips_by_dep_time[trips_ctr];
				int mySeq = current_neighbor->_index_along_trip_at_upstream_node[trips_ctr];
				_Transit_Pattern_Interface* next_pattern = (_Transit_Pattern_Interface*)next_trip->_pattern;

				++trips_ctr;

				float waitTime = next_trip->_departure_seconds[mySeq] - current->_time_label;
				if (waitTime < 0)
				{
					continue;
				}

				//Since trips are sorted chronologically by departure time, no need to scan beyond this threshold
				if (waitTime > waitThreshold)
				{
					return;
				}

				if (next_pattern->_scanned)
				{
					continue;
				}

				next_pattern->_scanned = true;
				++patterns_ctr;

				Link_Components::Types::Link_Type_Keys current_type = current->_edge_type;

				int wait_binary = 1;

				if (current_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
				{
					_Transit_Vehicle_Trip_Interface* current_trip = (_Transit_Vehicle_Trip_Interface*)current->_came_on_trip;
					if (current_trip->_uuid == next_trip->_uuid)
					{
						wait_binary = 0;
					}
				}

				int WaitingCount = current->_wait_count_from_origin + wait_binary;

				int TransferCount = std::max(WaitingCount - 1,0);
				/*int nonHomeWait = 0;
				if (WaitingCount > 1)
				{
					TransferCount = WaitingCount - 1;
					nonHomeWait = 1;
				}*/

				float effectiveTransferPen = TransferCount * wait_binary * transferPenalty;				
				
				float ivtTime;
				if (wait_binary == 1)
				{
					ivtTime = next_trip->_arrival_seconds[mySeq + 1] - next_trip->_departure_seconds[mySeq];
				}
				else
				{
					ivtTime = next_trip->_arrival_seconds[mySeq + 1] - current->_time_label;
				}

				float cost_from_origin = current->cost_from_origin() + waitWeight*wait_binary*waitTime + ivtWeight*ivtTime + effectiveTransferPen;

				if (cost_from_origin < current_neighbor->cost_from_origin())
				{
					current_neighbor->cost_from_origin(cost_from_origin);

					float time_from_origin = current->time_from_origin() + wait_binary*waitTime + ivtTime;

					current_neighbor->time_from_origin(time_from_origin);
					current_neighbor->time_label((float)next_trip->_arrival_seconds[mySeq + 1]);

					current_neighbor->came_from(current);
					current_neighbor->_came_on_trip = next_trip;
					current_neighbor->_came_on_seq_index = mySeq + 1;
					current_neighbor->_wait_count_from_origin = WaitingCount;
					current_neighbor->_wait_time_from_origin = current->_wait_time_from_origin + wait_binary * waitTime;
					current_neighbor->_walk_time_from_origin = current->_walk_time_from_origin;
					current_neighbor->_ivt_time_from_origin = current->_ivt_time_from_origin + ivtTime;
					current_neighbor->_car_time_from_origin = current->_car_time_from_origin;
					current_neighbor->_transfer_pen_from_origin = current->_transfer_pen_from_origin + effectiveTransferPen;

					float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, routing_data.end_edge, multimodal_dijkstra);
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
		}

		template<typename AgentType>
		void Evaluate_Walk_Neighbor(Routable_Agent<AgentType>* agent, current_edge_type* current, connection_type* connection, Routing_Data<base_edge_type>& routing_data)
		{
			A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection->neighbor();

			//if (current_neighbor->in_closed_set()) return;						
						
			
			float walkWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::walkWeight<float>();		
			float walkThreshold = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::walkThreshold<float>();
			float walkSpeed = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::walkSpeed<float>();
			float walkThreshold_Time = walkThreshold / walkSpeed;

			bool multimodal_dijkstra = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::multimodal_dijkstra<bool>();

			
			float cost_from_origin = current->cost_from_origin() + walkWeight*current_neighbor->_time_cost_temp;

			if (current->_walk_time_from_origin + current_neighbor->_time_cost_temp > walkThreshold_Time)
			{
				return;
			}

			if (cost_from_origin < current_neighbor->cost_from_origin())
			{
				current_neighbor->cost_from_origin(cost_from_origin);

				float time_cost_between = current_neighbor->_time_cost_temp;
				current_neighbor->time_from_origin(current->time_from_origin() + time_cost_between);
				current_neighbor->time_label(current->time_label() + time_cost_between);					
					
				current_neighbor->came_from(current);
				current_neighbor->_came_on_seq_index = -1;
				current_neighbor->_wait_count_from_origin = current->_wait_count_from_origin;
				current_neighbor->_wait_time_from_origin = current->_wait_time_from_origin;
				current_neighbor->_walk_time_from_origin = current->_walk_time_from_origin + time_cost_between;
				current_neighbor->_ivt_time_from_origin = current->_ivt_time_from_origin;
				current_neighbor->_car_time_from_origin = current->_car_time_from_origin;
				current_neighbor->_transfer_pen_from_origin = current->_transfer_pen_from_origin;
					
				float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, routing_data.end_edge, multimodal_dijkstra);
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
						
			float carWeight = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::carWeight<float>();		

			bool multimodal_dijkstra = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::multimodal_dijkstra<bool>();
			
			float time_cost_between = agent->time_cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);

			float heuristicPortion = agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, routing_data.end_edge, multimodal_dijkstra);
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

			float cost_from_origin = current->cost_from_origin() + agent->cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);

			if( cost_from_origin < current_neighbor->cost_from_origin() )
			{
				if( current_neighbor->in_open_set() ) routing_data.open_set->erase( routing_data.open_set->iterator_to( *((base_edge_type*)current_neighbor)  ) );

				float time_cost_between = agent->time_cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
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

				float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, routing_data.end_edge);

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