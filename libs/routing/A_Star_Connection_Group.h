#pragma once
#include "Connection_Group_Implementation.h"
#include "A_Star_Edge.h"

namespace polaris
{


	template<typename MasterType>
	struct Custom_Connection_Group<MasterType,typename MasterType::static_graph_type,typename MasterType::static_graph_type,typename MasterType::static_to_static_type>
		: public Connection_Group_Base< MasterType, typename MasterType::static_graph_type, typename MasterType::static_graph_type, typename MasterType::static_to_static_type >
	{
		template<typename AgentType>
		Anonymous_Connection_Group* Visit_Neighbors(Routable_Agent<AgentType>* agent, current_edge_type* current, Routing_Data<base_edge_type>& routing_data)
		{
			const Connection_Implementation* const end_connection_itr = end_forward_edges();

			for(Connection_Implementation* connection_itr = forward_edges(); connection_itr != end_connection_itr; ++connection_itr)
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