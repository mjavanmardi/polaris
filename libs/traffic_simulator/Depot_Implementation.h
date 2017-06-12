#pragma once
#include "Depot_Prototype.h"
//#include "Geometry_Implementation.h"
//#include "tmc/depot.h"


namespace Depot_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Tow_Truck_Implementation:public Polaris_Component<MasterType,INHERIT(Tow_Truck_Implementation),NT>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Tow_Truck_Implementation),NT>::Component_Type ComponentType;

			typedef Depot<typename MasterType::depot_type> Depot_Interface;
			typedef Link<typename MasterType::link_type> Link_Interface;
			typedef Scenario<typename MasterType::scenario_type> Scenario_Interface;
			typedef Intersection<typename MasterType::intersection_type> Intersection_Interface;

			template<typename TargetType> static void Initialize_Type()
			{

			}

			m_data(Depot_Interface*,associated_depot, NONE, NONE);
			m_data(Link_Interface*,truck_location, NONE, NONE);
		};
		

		implementation struct Tow_Truck_Depot:public Polaris_Component<MasterType,INHERIT(Tow_Truck_Depot),Execution_Object>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Tow_Truck_Depot),Execution_Object>::Component_Type ComponentType;
			typedef Scenario<typename MasterType::scenario_type> Scenario_Interface;
			typedef Tow_Truck<typename MasterType::tow_truck_type> Tow_Truck_Interface;

			template<typename TargetType> static void Initialize_Type()
			{

			}
			
			
			template<typename TargetType> void Accept_Network_Events(std::vector<Network_Event_Components::Prototypes::Network_Event<typename MasterType::base_network_event_type>*>& network_events)
			{
				_current_events.clear();

				// Filter out accident events

				//for(typename std::vector<Network_Event<typename MasterType::base_network_event_type>*>::iterator itr = network_events.begin();itr!=network_events.end();itr++)
				//{
				//	Network_Event<typename MasterType::base_network_event_type>* net_event = *itr;

				//	if( net_event->template Is_Type<typename MasterType::accident_network_event_type>() )
				//	{
				//		_current_events.push_back( *itr );
				//	}
				//}
				for (const auto& net_event : network_events)
				{
					//if (net_event->Is_Type<typename MasterType::accident_network_event_type>())
					if (typeid(net_event) == typeid(MasterType::accident_network_event_type))
					{
						_current_events.push_back(net_event);
					}
				}
			}

			static void Depot_Conditional(ComponentType* _this,Event_Response& response)
			{
				response.next._iteration = iteration() + 60;
				response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_VISUALIZATION_SUB_ITERATIONS;

				//response.result = true;

				//Swap_Event(&Depot_Event<TargetType>);
			}
			
			// template<typename ComponentType,typename TargetType>
			// static void Depot_Event(void* _this)
			// {
				// // Do Nothing Currently
			// }

			/*template<typename TargetType> void Initialize(polaris::io::Depot& instance)
			{
				this->template Load_Event<ComponentType>(&Depot_Conditional,((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_VISUALIZATION_SUB_ITERATIONS);
				
				////TODO
				//load_event(ComponentType,Depot_Condition,Depot_Event, ((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_VISUALIZATION_SUB_ITERATIONS,NULLTYPE);
				using namespace polaris::io;
				
				std::shared_ptr<LinkList> link_list = instance.getLinks();

				const std::vector<int>& db_covered_links = (*link_list).getLinks();

				std::unordered_map<int,std::vector<typename MasterType::link_type*>>& db_map=((Network<typename MasterType::network_type>*)_global_network)->template db_id_to_links_map<std::unordered_map<int,std::vector<typename MasterType::link_type*>>&>();

				for(std::vector<int>::const_iterator itr=db_covered_links.begin();itr!=db_covered_links.end();itr++)
				{
					int link = *itr;

					if(db_map.count(link))
					{
						std::vector<typename MasterType::link_type*>& links=db_map[link];

						typename std::vector<typename MasterType::link_type*>::iterator vitr;

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

				//TODO
                //RLW%%% _depot_service = new polaris::Depot( instance );
			}
			*/
			typedef Link_Components::Prototypes::Link<typename MasterType::link_type> Link_Interface;
			m_data(Link_Interface*,resident_link, NONE, NONE);
			m_data(std::vector<Link_Interface*>,covered_links, NONE, NONE);

			m_data(std::vector<typename MasterType::tow_truck_type>,tow_trucks, NONE, NONE);
			//m_data(polaris::Depot*,depot_service, NONE, NONE);
			m_prototype(Null_Prototype,typename MasterType::traffic_management_center_type,traffic_management_center, NONE, NONE);

			m_data(std::vector<Network_Event_Components::Prototypes::Network_Event<typename MasterType::base_network_event_type>*>, current_events, NONE, NONE);
		};
	}
}

using namespace Depot_Components::Implementations;
