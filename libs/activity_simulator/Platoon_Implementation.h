#pragma once

#include "Platoon_Prototype.h"
//#include "Vehicle_Prototype.h"
//#include "Household_Prototype.h"		
//#include "Demand.h"

namespace Platoon_Components
{
	namespace Implementations
	{
		implementation struct Platoon_Data_Implementation : public Polaris_Component<MasterType, INHERIT(Platoon_Data_Implementation), Data_Object>
		{
			m_data(int, lead_vehicle_id, NONE, NONE);
			m_data(int, link_arrival_time, NONE, NONE);
			m_data(int, node_wait_time, NONE, NONE);			
			m_prototype(Link_Components::Prototypes::Link, typename MasterType::link_type, link, NONE, NONE);
		};
		
		implementation struct Platoon_Implementation : public Polaris_Component<MasterType,INHERIT(Platoon_Implementation),Data_Object>
		{			
			//typedef std::unordered_map<int, std::vector < typename MasterType::platoon_data_type* > *> platoon_data_container_type ;
			typedef std::unordered_map<int, std::deque <  Platoon_Components::Prototypes::Platoon_Data < typename MasterType::platoon_data_type> * > * > platoon_data_container_type;
			
			static m_container(platoon_data_container_type, platoon_data_container, NONE, NONE);

			// Static initialization
			static m_data(bool, is_initialized, NONE, NONE);
			template<typename T> static void static_initializer(string filename, T demand)
			{				
				if (filename == "") return;

				std::ifstream data_file;
				data_file.open(filename.c_str(), ios::in);
				if (!data_file.is_open()) cout << endl << "Could not open platoon_information.txt in the working directory.  Check to make sure the file file exists." << endl;

				// make sure this is only run once				
				if(_is_initialized ) return; else _is_initialized = true;
				
				typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
				auto network_reference = demand->network_reference<_Network_Interface*>();
				typedef std::map<pair<int, int>, Link_Components::Prototypes::Link < typename MasterType::link_type > *> _Links_Map_Container_Interface;
				auto links_map_container_ptr = network_reference->template links_map_container<typename _Links_Map_Container_Interface*>();

				string line;
				string strText;
				
				string type;
				int trip_id, lead_veh_id, node_i, node_j , path_id;
				double link_arrival_time, node_wait_time;
				getline(data_file, line); //throw out header
				while (getline(data_file, line))
				{
					//trip_id	path_id	front_veh_id	node_i	node_j	node_wait_time	link_arrival_time
					std::stringstream   linestream(line);					
					std::getline(linestream, strText, '\t');  trip_id = stoi(strText);
					std::getline(linestream, strText, '\t');  path_id = stoi(strText);
					std::getline(linestream, strText, '\t');  strText!=""? lead_veh_id = stoi(strText) : lead_veh_id = -1;
					std::getline(linestream, strText, '\t');  node_i = stoi(strText);
					std::getline(linestream, strText, '\t');  node_j = stoi(strText);
					//std::getline(linestream, strText, '\t');  link_arrival_time = stod(strText) - 28800.0;
					std::getline(linestream, strText, '\t');  link_arrival_time = stod(strText) ;
					std::getline(linestream, strText, '\t');  strText != "" ? node_wait_time = stod(strText) : node_wait_time = 0.0f;
					//std::getline(linestream, strText, '\t');  strText != "" ? node_wait_time = stod(strText)  : node_wait_time = 0.0f;
					

					//find the link where the vehicle joins the platoon 
					auto link_it = links_map_container_ptr->find(pair<int, int>(node_i, node_j));
					if (link_it == links_map_container_ptr->end()) 
					{ 
						cout << "Error: Could not find link between node_i= " << node_i << " and  node_j= " << node_j << endl; 
						//use a random link for now!
						/*
						auto it = links_map_container_ptr->begin();
						std::advance(it, rand() % links_map_container_ptr->size());
						auto random_key = it->first;
						link_it = links_map_container_ptr->find(random_key);
						*/
					}

					typedef Platoon_Components::Prototypes::Platoon_Data<typename MasterType::platoon_data_type >  platoon_data_itf;
					auto newData = (platoon_data_itf*) Allocate<  MasterType::platoon_data_type>();

					newData->link<Link_Components::Prototypes::Link<MasterType::link_type > * >(link_it->second);
					newData->lead_vehicle_id(lead_veh_id);
					newData->link_arrival_time( link_arrival_time);
					newData->node_wait_time(node_wait_time);
					
					auto veh_platoon_data_it =  _platoon_data_container.find(trip_id);
					if (veh_platoon_data_it == _platoon_data_container.end())
					{
						auto return_value =  _platoon_data_container.insert(pair<int, std::deque< typename Platoon_Components::Prototypes::Platoon_Data < MasterType::platoon_data_type> * > * > (trip_id, new std::deque< typename Platoon_Components::Prototypes::Platoon_Data<MasterType::platoon_data_type> *>()));
						veh_platoon_data_it = return_value.first;
					}
					veh_platoon_data_it->second->push_back(newData);
				}		
			}
		};

		template<typename MasterType, typename InheritanceList> typename Platoon_Implementation <MasterType, InheritanceList>::type_of(is_initialized) Platoon_Implementation<MasterType, InheritanceList>::_is_initialized = false;		
		//template<typename MasterType, typename InheritanceList> std::unordered_map<int, typename MasterType::platoon_data_type* >  Platoon_Implementation<MasterType, InheritanceList>::_platoon_data_container;
		template<typename MasterType, typename InheritanceList> std::unordered_map<int, std::deque <Platoon_Components::Prototypes::Platoon_Data < typename MasterType::platoon_data_type> *> * >  Platoon_Implementation<MasterType, InheritanceList>::_platoon_data_container;
		
	}
}

