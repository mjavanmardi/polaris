#pragma once
#include "Input.h"

//struct Routable_Agent_Implementation;

namespace polaris
{
	template<typename Base_Edge_Type>
	struct Routing_Data
	{
		std::deque< Base_Edge_Type* >* modified_edges;
		//std::vector< Base_Edge_Type* >* end_transit_edges;
		boost::intrusive::multiset< Base_Edge_Type >* open_set;
		Base_Edge_Type* start_edge;
		Base_Edge_Type* end_edge;
		std::vector< Base_Edge_Type* >* ends;
		unsigned int start_time;

		//TODO OMER: Check if these additions damages anything
		//----------------------------------------------------
		float transferPenalty;
		float waitWeight;
		float walkWeight;
		float bikeWeight;
		float ivtWeight;
		float carWeight;
		float scanThreshold;
		float costThreshold;
		float waitThreshold_Time;
		Meters walkThreshold;
		Kilometers_Per_Hour walkSpeed_kph;
		Meters bikeThreshold;
		Kilometers_Per_Hour bikeSpeed_kph;
		bool multimodal_dijkstra;
		Meters_Per_Second walkSpeed_mps;
		Meters_Per_Second bikeSpeed_mps;
		Feet_Per_Second walkSpeed_fps;
		Feet_Per_Second bikeSpeed_fps;
		float bike_time_factor;
		float walkThreshold_Time;
		float bikeThreshold_Time;
		Vehicle_Components::Types::Vehicle_Type_Keys sub_mode;
		//----------------------------------------------------
	};

	template<typename MasterType,typename Base_Edge_Type>
	struct Anonymous_Connection_Group;



	prototype struct Edge ADD_DEBUG_INFO
	{
		typedef typename ComponentType::base_edge_type base_edge_type;
		typedef typename ComponentType::attribute_type attribute_type;
		typedef typename ComponentType::Master_Type Master_Type;

		attribute_type* attributes(){ return this_component()->attributes(); }

		p_edge_id_type edge_id(){ return this_component()->edge_id(); }
		void edge_id(p_edge_id_type value){ this_component()->edge_id(value); }
		
		//unsigned int edge_size(){ return this_component()->edge_size(); }
		//void edge_size(unsigned int value){ this_component()->edge_size(value); }

		Anonymous_Connection_Group<Master_Type,base_edge_type>* begin_connection_groups(){ return this_component()->begin_connection_groups(); }
		void begin_connection_groups(Anonymous_Connection_Group<Master_Type,base_edge_type>* value){ this_component()->begin_connection_groups(value); }

		const Anonymous_Connection_Group<Master_Type,base_edge_type>* const end_connection_groups(){ return this_component()->end_connection_groups(); }
		void end_connection_groups(Anonymous_Connection_Group<Master_Type,base_edge_type>* value){ this_component()->end_connection_groups(value); }
	};
}