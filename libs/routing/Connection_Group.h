#pragma once
#include "Graph_Pool_Implementation.h"

namespace polaris
{
	//abstract base class for any connection, everybody understands it
	template<typename MasterType,typename Base_Edge_Type>
	struct Anonymous_Connection_Group
	{
		virtual Anonymous_Connection_Group* Next_Connection_Group() = 0;

		virtual Anonymous_Connection_Group* Unlink_Edges() = 0;

		virtual Anonymous_Connection_Group* Link_Edges(void* graph_pool) = 0;

		virtual Anonymous_Connection_Group* Visit_Neighbors(Routable_Agent<typename MasterType::routable_agent_type>* agent, void* current, Routing_Data<Base_Edge_Type>& routing_data) = 0;

		virtual Anonymous_Connection_Group* Visit_Neighbors(Routable_Agent<typename MasterType::tree_agent_type>* agent, void* current, Routing_Data<Base_Edge_Type>& routing_data) = 0;

		virtual Anonymous_Connection_Group* Visit_Neighbors(Routable_Agent<typename MasterType::walk_to_transit_tree_agent_type>* agent, void* current, Routing_Data<Base_Edge_Type>& routing_data) = 0;

		virtual Anonymous_Connection_Group* Visit_Neighbors(Routable_Agent<typename MasterType::drive_to_transit_tree_agent_type>* agent, void* current, Routing_Data<Base_Edge_Type>& routing_data) = 0;

		virtual Anonymous_Connection_Group* Visit_Neighbors(Routable_Agent<typename MasterType::multi_modal_tree_agent_type>* agent, void* current, Routing_Data<Base_Edge_Type>& routing_data) = 0;

		virtual Anonymous_Connection_Group* Visit_Multimodal_Neighbors(Routable_Agent<typename MasterType::routable_agent_type>* agent, void* current, Routing_Data<Base_Edge_Type>& routing_data, void* graph_pool) = 0;

		//virtual Anonymous_Connection_Group* Visit_Neighbors(Routable_Agent<typename MasterType::time_dependent_agent_type>* agent, void* current, Routing_Data<Base_Edge_Type>& routing_data) = 0;
	};

	prototype struct Connection ADD_DEBUG_INFO
	{
		typedef typename ComponentType::ci_neighbor_edge_type neighbor_edge_type;
		typedef typename ComponentType::connection_type connection_type;
		typedef typename ComponentType::ci_connection_attributes_type connection_attributes_type;

		connection_attributes_type* connection_attributes(){ return this_component()->connection_attributes(); }

		Connection<connection_type>* next_connection(){ return (Connection<connection_type>*)this_component()->next_connection(); }

		neighbor_edge_type* neighbor(){ return this_component()->neighbor(); }
		void neighbor(neighbor_edge_type* value){ this_component()->neighbor(value); }

		p_edge_id_type edge_id(){ return this_component()->edge_id(); }
		void edge_id(p_edge_id_type value){ this_component()->edge_id(value); }			
	};

	prototype struct Connection_Group ADD_DEBUG_INFO
	{
		typedef typename ComponentType::connection_type connection_type;

		p_graph_id_type linked_graph() { return this_component()->linked_graph(); }
		void linked_graph(p_graph_id_type value) { this_component()->linked_graph(value); }

		unsigned int num_forward_edges() { return this_component()->num_forward_edges(); }
		void num_forward_edges(unsigned int value) { this_component()->num_forward_edges(value); }

		Connection<connection_type>* forward_edges(){ return (Connection<connection_type>*)this_component()->forward_edges(); }
		const Connection<connection_type>* const end_forward_edges(){ return (const Connection<connection_type>* const)this_component()->end_forward_edges(); }
	};
}