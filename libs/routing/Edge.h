#pragma once
#include "Input.h"

//struct Routable_Agent_Implementation;

namespace polaris
{
	template<typename Base_Edge_Type>
	struct Routing_Data
	{
		boost::container::deque< Base_Edge_Type* >* modified_edges;
		boost::intrusive::multiset< Base_Edge_Type >* open_set;
		Base_Edge_Type* start_edge;
		Base_Edge_Type* end_edge;
		unsigned int start_time;
	};

	template<typename MasterType,typename Base_Edge_Type>
	struct Anonymous_Connection_Group
	{
		virtual Anonymous_Connection_Group* Next_Connection_Group() = 0;

		virtual Anonymous_Connection_Group* Unlink_Edges() = 0;

		virtual Anonymous_Connection_Group* Link_Edges(void* graph_pool) = 0;

		virtual Anonymous_Connection_Group* Visit_Neighbors(Routable_Agent<typename MasterType::routable_agent_type>* agent, void* current, Routing_Data<Base_Edge_Type>& routing_data) = 0;
	};


	prototype struct Edge
	{
		typedef typename ComponentType::base_edge_type base_edge_type;
		typedef typename ComponentType::attribute_type attribute_type;
		typedef typename ComponentType::Master_Type Master_Type;

		attribute_type* attributes(){ return this_component()->attributes(); }

		edge_id_type edge_id(){ return this_component()->edge_id(); }
		void edge_id(edge_id_type value){ this_component()->edge_id(value); }
		
		//unsigned int edge_size(){ return this_component()->edge_size(); }
		//void edge_size(unsigned int value){ this_component()->edge_size(value); }

		Anonymous_Connection_Group<Master_Type,base_edge_type>* begin_connection_groups(){ return this_component()->begin_connection_groups(); }
		void begin_connection_groups(Anonymous_Connection_Group<Master_Type,base_edge_type>* value){ this_component()->begin_connection_groups(value); }

		const Anonymous_Connection_Group<Master_Type,base_edge_type>* const end_connection_groups(){ return this_component()->end_connection_groups(); }
		void end_connection_groups(Anonymous_Connection_Group<Master_Type,base_edge_type>* value){ this_component()->end_connection_groups(value); }
	};
}