#pragma once
#include "Edge_Implementation.h"

namespace polaris
{
	using namespace boost::intrusive;

	template<typename MasterType>
	struct Base_Edge_A_Star : public set_base_hook< optimize_size<false> >
	{
		typedef MasterType Master_Type;

		Base_Edge_A_Star():_x(0.0f),_y(0.0f),_source_link(nullptr),_cost_from_origin(FLT_MAX / 2.0f),_time_label(0.0f),
					 _estimated_cost_origin_destination(FLT_MAX / 2.0f),_marked_for_reset(false),_in_closed_set(false),_in_open_set(false),
					 _begin_connection_groups(nullptr),_end_connection_groups(nullptr),_came_from(nullptr),_edge_id(-1),
					_cost(0.0f),_time_cost(0.0f) {}

		typedef Base_Edge_A_Star base_edge_type;
		typedef Base_Edge_A_Star attribute_type;
		
		virtual void reset()
		{
			_cost_from_origin = FLT_MAX / 4.0f;
			//_time_from_origin = FLT_MAX / 2.0f;
			_estimated_cost_origin_destination = FLT_MAX / 2.0f;

			_in_closed_set = false;
			_marked_for_reset = false;
			_in_open_set = false;

			_came_from = nullptr;
		}

		t_data(float, x);
		t_data(float, y);
		t_data(typename MasterType::link_type*, source_link);

		t_data(float, cost);
		t_data(float, cost_backup);
		t_data(float, time_cost);
		t_data(float, time_cost_backup);
		t_data(float, cost_from_origin);
		t_data(float, time_from_origin);
		t_data(float, time_label);		

		t_data(float, estimated_cost_origin_destination);
		t_data(bool, marked_for_reset);
		t_data(bool, in_closed_set);
		t_data(bool, in_open_set);

		t_data(void*, came_from);
		
		t_data(p_edge_id_type, edge_id);
		
		Anonymous_Connection_Group<MasterType,Base_Edge_A_Star>* _begin_connection_groups;
		Anonymous_Connection_Group<MasterType,Base_Edge_A_Star>* begin_connection_groups(){ return _begin_connection_groups; }

		void begin_connection_groups(Anonymous_Connection_Group<MasterType,Base_Edge_A_Star>* value){ _begin_connection_groups = value; }

		Anonymous_Connection_Group<MasterType,Base_Edge_A_Star>* _end_connection_groups;
		const Anonymous_Connection_Group<MasterType,Base_Edge_A_Star>* const end_connection_groups(){ return _end_connection_groups; }

		void end_connection_groups(Anonymous_Connection_Group<MasterType,Base_Edge_A_Star>* value){ _end_connection_groups = value; }

		friend __forceinline bool operator< (const Base_Edge_A_Star &lhs,const Base_Edge_A_Star &rhs)
		{  return lhs._estimated_cost_origin_destination < rhs._estimated_cost_origin_destination;  }
		friend __forceinline bool operator> (const Base_Edge_A_Star &lhs,const Base_Edge_A_Star &rhs)
		{  return lhs._estimated_cost_origin_destination > rhs._estimated_cost_origin_destination;  }
		friend __forceinline bool operator== (const Base_Edge_A_Star &lhs,const Base_Edge_A_Star &rhs)
		{  return lhs._estimated_cost_origin_destination == rhs._estimated_cost_origin_destination;  }

		void Display()
		{
			cout << (int)(_edge_id/2.0) <<"."<<_edge_id - ((int)(_edge_id/2.0)*2)<<" ("<<_time_cost<<"); ";
			Base_Edge_A_Star* prev = (Base_Edge_A_Star*)_came_from;
			while (prev != nullptr)
			{
				cout << (int)(prev->_edge_id/2.0) <<"."<<prev->_edge_id - ((int)(prev->_edge_id/2.0)*2)<<" ("<<prev->_time_cost<<"); ";
				prev = (Base_Edge_A_Star*)(prev->_came_from);
			}
			cout <<"\n";

		}
	};
}