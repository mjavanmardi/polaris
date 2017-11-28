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
		implementation struct Analyze_Link_Group_Implementation:public Polaris_Component<MasterType,INHERIT(Analyze_Link_Group_Implementation),Data_Object>
		{
			m_container(std::vector<typename MasterType::link_type*>, links_container, NONE, NONE);
			m_prototype(Null_Prototype,typename MasterType::base_network_event_type, event, NONE, NONE);
			m_data(fstream, link_moe_output_file, NONE, NONE);
			m_data(int, analyze_link_group_id, NONE, NONE);

			template<typename TargetType> void initialize(int group_id)
			{
				typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef  Network_Event_Components::Prototypes::Network_Event< type_of(event)> _Network_Event_Interface;
			
				_analyze_link_group_id = group_id;
				string out_link_moe_file_name = ((_Scenario_Interface*)_global_scenario)->template output_dir_name<string&>() + "moe_analyze_link_group_" + to_string((long long)_analyze_link_group_id) + ".csv";
				_link_moe_output_file.open(out_link_moe_file_name, fstream::out);
				//_link_moe_output_file << "event_start_time,event_end_time" << endl;

				//_link_moe_output_file << _event->_start_time << "," << _event->_end_time << endl;
				_link_moe_output_file << "clock,time,link,dbid,direction,unode,dnode,link_type,travel_time_in_min,travel_time_sd_in_min,travel_delay_in_min,travel_delay_sd_in_min,queue_length,speed_in_mph,density_in_vpmpl,in_flow_rate_in_vphpl,out_flow_rate_in_vphpl,in_volume,out_volume,travel_time_ratio,speed_ratio,density_ratio,in_flow_ratio,out_flow_ratio,vht,vmt" << endl;
			}
			
			template<typename TargetType> void output_moe(int time)
			{
				typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
				typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

				typedef typename MasterType::link_type _link_component_type;
				typedef typename MasterType::intersection_type _intersection_component_type;
				
				typename _Links_Container_Interface::iterator link_itr;
				for(link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
				{
					_link_component_type* link = (_link_component_type*)(*link_itr);
					_link_moe_output_file 
						<< convert_seconds_to_hhmmss(time).c_str() << ","
						<< time << ","
						<< link->uuid<int>() << ","
						<< link->dbid<int>() << ","
						<< link->direction<int>() << ","
						<< link->upstream_intersection<_intersection_component_type*>()->uuid<int>() << ","
						<< link->downstream_intersection<_intersection_component_type*>()->uuid<int>() << ","
						<< link->link_type<Link_Components::Types::Link_Type_Keys>() << ","	
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
						<< link->link_vht<float>() << ","
						<< link->link_vmt<float>()
						<< endl;
				}
			}

			template<typename TargetType> void close_link_moe_output_file()
			{
				_link_moe_output_file.close();
			}
		};
	}
}
