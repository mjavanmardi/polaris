#pragma once
#include "Depot_Prototype.h"
#include "Geometry_Implementation.h"
#include "tmc\depot.h"

namespace Depot_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Tow_Truck_Implementation:public Polaris_Component<APPEND_CHILD(Tow_Truck_Implementation),MasterType,NT>
		{
			typedef typename Polaris_Component<APPEND_CHILD(Tow_Truck_Implementation),MasterType,NT>::Component_Type ComponentType;

			typedef Depot<typename type_of(MasterType::depot)> Depot_Interface;
			typedef Link_Prototype<typename type_of(MasterType::link)> Link_Interface;
			typedef Scenario_Prototype<typename MasterType::scenario_type> Scenario_Interface;
			typedef Intersection_Prototype<typename type_of(MasterType::intersection)> Intersection_Interface;

			feature_implementation static void Initialize_Type()
			{

			}

			member_pointer(Depot_Interface,associated_depot,none,none);
			member_pointer(Link_Interface,truck_location,none,none);
		};
		

		implementation struct Tow_Truck_Depot:public Polaris_Component<APPEND_CHILD(Tow_Truck_Depot),MasterType,Execution_Object>
		{
			typedef typename Polaris_Component<APPEND_CHILD(Tow_Truck_Depot),MasterType,Execution_Object>::Component_Type ComponentType;
			typedef Scenario_Prototype<typename MasterType::scenario_type> Scenario_Interface;
			typedef Tow_Truck<typename type_of(MasterType::tow_truck)> Tow_Truck_Interface;

			feature_implementation static void Initialize_Type()
			{

			}
			
			
			feature_implementation void Accept_Network_Events(vector<Network_Event_Components::Prototypes::Network_Event<typename type_of(MasterType::base_network_event)>*>& network_events)
			{
				_current_events.clear();

				// Filter out accident events

				for(typename vector<Network_Event<typename type_of(MasterType::base_network_event)>*>::iterator itr = network_events.begin();itr!=network_events.end();itr++)
				{
					Network_Event<typename type_of(MasterType::base_network_event)>* net_event = *itr;

					if( net_event->Is_Type<typename type_of(MasterType::accident_network_event)>() )
					{
						_current_events.push_back( *itr );
					}
				}
			}

			template<typename ComponentType,typename CallerType,typename TargetType>
			static void Depot_Condition(void* _this,Conditional_Response& response)
			{
				response.next._iteration = _iteration + 60;
				response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_VISUALIZATION_SUB_ITERATIONS;

				response.result = true;

				//Swap_Event(&Depot_Event<ComponentType,CallerType,TargetType>);
			}
			
			template<typename ComponentType,typename CallerType,typename TargetType>
			static void Depot_Event(void* _this)
			{
				// Do Nothing Currently
			}

			feature_implementation void Initialize(polaris::io::Depot& instance)
			{
				Load_Event<ComponentType>(&ComponentType::Depot_Condition<ComponentType,NT,NT>,&ComponentType::Depot_Event<ComponentType,NT,NT>,((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_VISUALIZATION_SUB_ITERATIONS);

				using namespace polaris::io;
				
				std::shared_ptr<LinkList> link_list = instance.getLinks();

				const vector<int>& db_covered_links = (*link_list).getLinks();

				unordered_map<int,vector<typename MasterType::link_type*>>& db_map=((Network_Prototype<typename type_of(MasterType::network),ComponentType>*)_global_network)->template db_id_to_links_map<unordered_map<int,vector<typename MasterType::link_type*>>&>();

				for(vector<int>::const_iterator itr=db_covered_links.begin();itr!=db_covered_links.end();itr++)
				{
					int link = *itr;

					if(db_map.count(link))
					{
						vector<typename MasterType::link_type*>& links=db_map[link];

						typename vector<typename type_of(MasterType::link)*>::iterator vitr;

						for(vitr=links.begin();vitr!=links.end();vitr++)
						{
							_covered_links.push_back( (Link_Interface*)(*vitr) );
						}
					}
				}

				int resident_link_id = instance.getLink();

				if(db_map.count(resident_link_id))
				{
					_resident_link = (Link_Interface*)db_map[resident_link_id][0];
				}
				else
				{
					cout << "Resident Depot Link: " << resident_link_id << " not found!" << endl;
					exit (0);
				}

				_depot_service = new polaris::Depot( instance );
			}

			typedef Link_Prototype<typename type_of(MasterType::link)> Link_Interface;
			member_pointer(Link_Interface,resident_link,none,none);
			member_data(vector<Link_Interface*>,covered_links,none,none);

			member_data(vector<typename type_of(MasterType::tow_truck)>,tow_trucks,none,none);
			member_pointer(polaris::Depot,depot_service,none,none);
			member_prototype(Traffic_Management_Center,traffic_management_center,typename type_of(MasterType::traffic_management_center),none,none);

			member_data(vector<Network_Event_Components::Prototypes::Network_Event<typename type_of(MasterType::base_network_event)>*>, current_events, none, none);
		};
	}
}

using namespace Depot_Components::Implementations;