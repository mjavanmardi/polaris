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

	template<typename MasterType>
	struct Custom_Connection_Group<MasterType, typename MasterType::transit_graph_type, typename MasterType::transit_graph_type, typename MasterType::transit_to_transit_type>
		: public Connection_Group_Base< MasterType, typename MasterType::transit_graph_type, typename MasterType::transit_graph_type, typename MasterType::transit_to_transit_type >
	{
		typedef Connection_Group_Base< MasterType, typename MasterType::transit_graph_type, typename MasterType::transit_graph_type, typename MasterType::transit_to_transit_type > Base_t;
		typedef typename Base_t::Anonymous_Connection_Group Anonymous_Connection_Group;
		typedef typename Base_t::current_edge_type current_edge_type;
		typedef typename Base_t::base_edge_type base_edge_type;
		typedef typename Base_t::connection_type connection_type;
		typedef typename Base_t::Connection_Implementation Connection_Implementation;
		typedef typename Base_t::neighbor_edge_type neighbor_edge_type;
		typedef typename Base_t::connection_attributes_type connection_attributes_type;

		typedef Network<typename MasterType::network_type> Network_Interface;

		typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface;

		typedef  Transit_Vehicle_Trip_Components::Prototypes::Transit_Vehicle_Trip<typename remove_pointer< typename Network_Interface::get_type_of(transit_vehicle_trips_container)::value_type>::type>  _Transit_Vehicle_Trip_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(transit_vehicle_trips_container), _Transit_Vehicle_Trip_Interface*> _Transit_Vehicle_Trips_Container_Interface;

		typedef  Transit_Pattern_Components::Prototypes::Transit_Pattern<typename remove_pointer< typename Network_Interface::get_type_of(transit_patterns_container)::value_type>::type>  _Transit_Pattern_Interface;
		typedef  Random_Access_Sequence< typename Network_Interface::get_type_of(transit_patterns_container), _Transit_Pattern_Interface*> _Transit_Patterns_Container_Interface;

		//typedef typename MasterType::transit_pattern_type* transit_pattern_type;

		template<typename AgentType>
		Anonymous_Connection_Group* Visit_Transit_Neighbors(Routable_Agent<AgentType>* agent, current_edge_type* current, Routing_Data<base_edge_type>& routing_data)
		{
			//end_forward_edges is a member functon of Connection_Group_Base and returns the end of the current connection group
			const Connection_Implementation* const end_connection_itr = this->end_forward_edges();

			for (Connection_Implementation* connection_itr = this->forward_edges(); connection_itr != end_connection_itr; ++connection_itr)
			{
				
				A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection_itr->neighbor();
				Link_Components::Types::Link_Type_Keys current_neighbor_type = current_neighbor->_edge_type;

				if (current_neighbor_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
				{
					Evaluate_Transit_Neighbor<AgentType>(agent, current, connection_itr, routing_data);
				}
				else if (current_neighbor_type == Link_Components::Types::Link_Type_Keys::WALK)
				{
					Evaluate_Walk_Neighbor<AgentType>(agent, current, connection_itr, routing_data);
				}

			}

			return (Anonymous_Connection_Group*)end_connection_itr;
		}

		//basic A* stuff
		template<typename AgentType>
		void Evaluate_Transit_Neighbor(Routable_Agent<AgentType>* agent, current_edge_type* current, connection_type* connection, Routing_Data<base_edge_type>& routing_data)
		{
			A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection->neighbor();

			if (current_neighbor->in_closed_set()) return;

			A_Star_Edge<current_edge_type>* current_edge = (A_Star_Edge<current_edge_type>*)current;

			for (int trips_itr = 0; trips_itr < current_neighbor->_trips_by_dep_time.size(); ++trips_itr)
			{
				_Transit_Vehicle_Trip_Interface* next_trip = (_Transit_Vehicle_Trip_Interface*)current_neighbor->_trips_by_dep_time.at(trips_itr);
				int mySeq = current_neighbor->_index_along_trip_at_upstream_node.at(trips_itr);
				_Transit_Pattern_Interface* next_pattern = (_Transit_Pattern_Interface*) next_trip->_pattern;

				int wait_binary = 1;
				if (current->came_on_trip() == next_trip)
				{
					wait_binary = 0;
				}

				int walkbinary = 0;
				Link_Components::Types::Link_Type_Keys current_type = current->_edge_type;
				if (current_type == Link_Components::Types::Link_Type_Keys::WALK)
				{
					walkbinary = 1;
				}

				int CandidateWaitingCount = current->_wait_count_from_origin + wait_binary;

				int CandidateTransferCount = 0;
				int nonHomeWait = 0;
				if (CandidateWaitingCount > 1)
				{
					CandidateTransferCount = CandidateWaitingCount - 1;
					nonHomeWait = 1;
				}

				float transferPenalty = 300;
				float effectiveTransferPen = CandidateTransferCount * wait_binary * transferPenalty;


				float waitTime = next_trip->_departure_seconds.at(mySeq) - (current->_time_from_origin + current->_time_cost);
				float CandidateWaitLabel = wait_binary * waitTime + current->_wait_time_from_origin;


				//float CandidateDriveLabel = current->_drive_time_from_origin + drivebinary * current->._time_cost;
				float CandidateWalkLabel = current->_walk_time_from_origin + walkbinary * current->_time_cost;



				for (int links_itr = mySeq; links_itr < (int)next_pattern->_pattern_links.size(); links_itr++)
				{

					_Link_Interface* seq_Link = (_Link_Interface*)next_pattern->_pattern_links.at(links_itr);

					
				}
				//float cost_from_origin = current->cost_from_origin() + agent->cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);

				/*if (cost_from_origin < current_neighbor->cost_from_origin())
				{
					if (current_neighbor->in_open_set()) routing_data.open_set->erase(routing_data.open_set->iterator_to(*((base_edge_type*)current_neighbor)));

					float time_cost_between = agent->time_cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
					float time_from_origin = current->time_from_origin() + time_cost_between;

					if (!current_neighbor->marked_for_reset())
					{
						routing_data.modified_edges->push_back((base_edge_type*)current_neighbor);
						current_neighbor->marked_for_reset(true);
					}

					current_neighbor->came_from(current);

					current_neighbor->cost_from_origin(cost_from_origin);
					current_neighbor->time_from_origin(time_from_origin);

					current_neighbor->time_label(current_edge->time_label() + time_cost_between);

					float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, routing_data.end_edge);

					current_neighbor->estimated_cost_origin_destination(neighbor_estimated_cost_origin_destination);

					routing_data.open_set->insert(*((base_edge_type*)current_neighbor));

					current_neighbor->in_open_set(true);

					// update the label
					agent->update_label(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
				}*/
			}
		}

		template<typename AgentType>
		void Evaluate_Walk_Neighbor(Routable_Agent<AgentType>* agent, current_edge_type* current, connection_type* connection, Routing_Data<base_edge_type>& routing_data)
		{
			A_Star_Edge<neighbor_edge_type>* current_neighbor = (A_Star_Edge<neighbor_edge_type>*)connection->neighbor();

			if (current_neighbor->in_closed_set()) return;

			A_Star_Edge<current_edge_type>* current_edge = (A_Star_Edge<current_edge_type>*)current;

			float cost_from_origin = current->cost_from_origin() + agent->cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);

			if (cost_from_origin < current_neighbor->cost_from_origin())
			{
				if (current_neighbor->in_open_set()) routing_data.open_set->erase(routing_data.open_set->iterator_to(*((base_edge_type*)current_neighbor)));

				float time_cost_between = agent->time_cost_between(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
				float time_from_origin = current->time_from_origin() + time_cost_between;

				if (!current_neighbor->marked_for_reset())
				{
					routing_data.modified_edges->push_back((base_edge_type*)current_neighbor);
					current_neighbor->marked_for_reset(true);
				}

				current_neighbor->came_from(current);

				current_neighbor->cost_from_origin(cost_from_origin);
				current_neighbor->time_from_origin(time_from_origin);

				current_neighbor->time_label(current_edge->time_label() + time_cost_between);

				float neighbor_estimated_cost_origin_destination = cost_from_origin + agent->estimated_cost_between((neighbor_edge_type*)current_neighbor, routing_data.end_edge);

				current_neighbor->estimated_cost_origin_destination(neighbor_estimated_cost_origin_destination);

				routing_data.open_set->insert(*((base_edge_type*)current_neighbor));

				current_neighbor->in_open_set(true);

				// update the label
				agent->update_label(current, (neighbor_edge_type*)current_neighbor, (connection_attributes_type*)connection);
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