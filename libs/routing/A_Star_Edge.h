#pragma once
#include "Input.h"

namespace polaris
{
	prototype struct A_Star_Edge ADD_DEBUG_INFO
	{
		//tag_as_prototype;

		typedef typename ComponentType::base_edge_type base_edge_type;
		typedef typename ComponentType::Master_Type Master_Type;

		p_edge_id_type edge_id(){ return this_component()->edge_id(); }
		void edge_id(p_edge_id_type value){ this_component()->edge_id(value); }

		//const long long id(){ return this_component()->id(); }
		//
		//const int num_forward_edges(){ return this_component()->num_forward_edges(); }
		//A_Star_Neighbor<neighbor_type>* forward_edges(){ return (A_Star_Neighbor<neighbor_type>*)this_component()->forward_edges(); }
		//const A_Star_Neighbor<neighbor_type>* const end_forward_edges(){ return (const A_Star_Neighbor<neighbor_type>* const)this_component()->end_forward_edges(); }

		float estimated_cost_origin_destination(){ return this_component()->estimated_cost_origin_destination(); }
		void estimated_cost_origin_destination(float value){ this_component()->estimated_cost_origin_destination(value); }

		float cost_from_origin(){ return this_component()->cost_from_origin(); }
		void cost_from_origin(float value){ this_component()->cost_from_origin(value); }

		float time_from_origin(){ return this_component()->time_from_origin(); }
		void time_from_origin(float value){ this_component()->time_from_origin(value); }

		//float cost_between(A_Star_Edge* other){ return this_component()->cost_between(other); }
		float cost(){ return this_component()->cost(); }
		void cost(float value){ this_component()->cost(value); }
		
		float time_cost(){ return this_component()->time_cost(); }
		void time_cost(float value){ this_component()->time_cost(value); }

		Anonymous_Connection_Group<Master_Type,base_edge_type>* begin_connection_groups(){ return this_component()->begin_connection_groups(); }
		const Anonymous_Connection_Group<Master_Type,base_edge_type>* const end_connection_groups(){ return this_component()->end_connection_groups(); }

		bool marked_for_reset(){ return this_component()->marked_for_reset(); }
		void marked_for_reset(bool value){ this_component()->marked_for_reset(value); }

		bool in_closed_set(){ return this_component()->in_closed_set(); }
		void in_closed_set(bool value){ this_component()->in_closed_set(value); }

		bool in_open_set(){ return this_component()->in_open_set(); }
		void in_open_set(bool value){ this_component()->in_open_set(value); }
	
		void* came_from(){ return this_component()->came_from(); }
		void came_from(void* value){ this_component()->came_from(value); }
		
		float time_label(){ return this_component()->time_label(); }
		void time_label(float value){ this_component()->time_label(value); }

		float walk_distance_to_transit() { return this_component()->walk_distance_to_transit(); }
		void walk_distance_to_transit(float value) { this_component()->walk_distance_to_transit(value); }

		float drive_fft_to_transit() { return this_component()->drive_fft_to_transit(); }
		void drive_fft_to_transit(float value) { this_component()->drive_fft_to_transit(value); }

		bool touch_transit() { return this_component()->touch_transit(); }
		void touch_transit(bool value) { this_component()->touch_transit(value); }

		void Display(){this_component()->Display();}

		//void reset(){ this_component()->reset(); }

		/* + is compatible with intrusive multiset */
	};
}