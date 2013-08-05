#pragma once
#include "Analyze_Link_Group_Prototype.h"

namespace Analyze_Link_Group_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Polaris_Analyze_Link_Group_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Analyze_Link_Group_Implementation),MasterType,Data_Object,ParentType>
		{
			member_container(vector<typename MasterType::link_type*>, links_container, none, none);
			member_component(typename MasterType::base_network_event_type, event, none, none);
			member_data(fstream, link_moe_output_file, none, none);
			member_data(int, analyze_link_group_id, none, none);

			feature_implementation void initialize(int group_id)
			{
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				define_component_interface(_Network_Event_Interface, type_of(event), Network_Event_Components::Prototypes::Network_Event, ComponentType);
			
				_analyze_link_group_id = group_id;
				string out_link_moe_file_name = ((_Scenario_Interface*)_global_scenario)->template output_dir_name<string&>() + "moe_analyze_link_group_" + to_string((long long)_analyze_link_group_id) + ".csv";
				_link_moe_output_file.open(out_link_moe_file_name, fstream::out);
				//_link_moe_output_file << "event_start_time,event_end_time" << endl;

				//_link_moe_output_file << _event->_start_time << "," << _event->_end_time << endl;
				_link_moe_output_file << "clock,time,link,dbid,direction,unode,dnode,link_type,travel_time_in_min,travel_time_sd_in_min,travel_delay_in_min,travel_delay_sd_in_min,queue_length,speed_in_mph,density_in_vpmpl,in_flow_rate_in_vphpl,out_flow_rate_in_vphpl,in_volume,out_volume,travel_time_ratio,speed_ratio,density_ratio,in_flow_ratio,out_flow_ratio,vht,vmt" << endl;
			}
			
			feature_implementation void output_moe(int time)
			{
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				typedef typename MasterType::link_type _link_component_type;
				typename _Links_Container_Interface::iterator link_itr;
				for(link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
				{
					_link_component_type* link = (_link_component_type*)(*link_itr);
					_link_moe_output_file 
						<< convert_seconds_to_hhmmss(time).c_str() << ","
						<< time << ","
						<< link->_uuid << ","
						<< link->_dbid << ","
						<< link->_direction << ","
						<< link->_upstream_intersection->_uuid << ","
						<< link->_downstream_intersection->_uuid << ","
						<< link->_link_type << ","	
						<< link->link_moe_data.link_travel_time << ","
						<< link->link_moe_data.link_travel_time_standard_deviation << ","
						<< link->link_moe_data.link_travel_delay << ","
						<< link->link_moe_data.link_travel_delay_standard_deviation << ","
						<< link->link_moe_data.link_queue_length << ","
						<< link->link_moe_data.link_speed << ","
						<< link->link_moe_data.link_density << ","
						<< link->link_moe_data.link_in_flow_rate << ","
						<< link->link_moe_data.link_out_flow_rate << ","
						<< link->link_moe_data.link_in_volume << ","
						<< link->link_moe_data.link_out_volume << ","
						<< link->link_moe_data.link_travel_time_ratio << ","
						<< link->link_moe_data.link_speed_ratio << ","
						<< link->link_moe_data.link_density_ratio << ","
						<< link->link_moe_data.link_in_flow_ratio << ","
						<< link->link_moe_data.link_out_flow_ratio << ","
						<< link->_link_vht << ","
						<< link->_link_vmt
						<< endl;
				}
			}

			feature_implementation void close_link_moe_output_file()
			{
				_link_moe_output_file.close();
			}
		};
	}
}