#pragma once
#include "Advisory_ITS_Prototype.h"

namespace Advisory_ITS_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Simple_Advisory_ITS : public Polaris_Component<MasterType,INHERIT(Simple_Advisory_ITS),Data_Object>
		{
			
			typedef typename Polaris_Component<MasterType,INHERIT(Simple_Advisory_ITS),Data_Object>::Component_Type ComponentType;

			// Push advisory to all links
			void Initialize()
			{
				std::vector<typename MasterType::link_type* > &links = ((Network<typename MasterType::network_type>*)_global_network)->links_container<std::vector<typename MasterType::link_type*>&>();


				for (typename std::vector<typename MasterType::link_type* >::iterator itr = links.begin();itr != links.end();itr++)
				{
					((Link_Interface*)(*itr))->template Push_ITS< ComponentType* >((ComponentType*)this);
				}
			}

			// Push advisory to select links
			template<typename TargetType> void Initialize(const std::vector<int>& db_covered_links/*,bool flag=false*/)
			{
				using namespace polaris::io;

				std::unordered_map<int,std::vector<typename MasterType::link_type*>>& db_map=((Network<typename MasterType::network_type>*)_global_network)->template db_id_to_links_map<std::unordered_map<int,std::vector<typename MasterType::link_type*>>&>();

				for(typename std::vector<int>::const_iterator itr=db_covered_links.begin();itr!=db_covered_links.end();itr++)
				{
					int link = *itr;

					if(db_map.count(link))
					{
						std::vector<typename MasterType::link_type*>& links=db_map[link];

						typename std::vector<typename MasterType::link_type*>::iterator vitr;

						for(vitr=links.begin();vitr!=links.end();vitr++)
						{
							_covered_links.push_back( (Link_Interface*)(*vitr) );
							//if(flag) cout << "pushing HAR to: " << ((Link_Interface*)(*vitr))->template internal_id<int>();
							((Link_Interface*)(*vitr))->template Push_ITS< ComponentType* >( (ComponentType*)this);
						}
					}
				}
			}

			template<typename TargetType> static void Initialize_Type()
			{
			}

			template<typename TargetType> void Accept_Network_Events(std::vector<Network_Event_Components::Prototypes::Network_Event<typename MasterType::base_network_event_type>*>& network_events)
			{
				_current_events.clear();

				for(typename std::vector<Network_Event<typename MasterType::base_network_event_type>*>::iterator itr = network_events.begin();itr!=network_events.end();itr++)
				{
					_current_events.push_back( *itr );
				}
			}

			template<typename TargetType> void Accept_Displayed_Network_Events(std::vector<Network_Event_Components::Prototypes::Network_Event<typename MasterType::base_network_event_type>*>& network_events)
			{
				_displayed_events.clear();
				for (typename std::vector<Network_Event<typename MasterType::base_network_event_type>*>::iterator itr = network_events.begin(); itr!=network_events.end(); itr++)
				{
					_displayed_events.push_back( *itr );
				}
			}
			
			template<typename TargetType> void Get_Displayed_Messages(std::vector<Network_Event_Components::Prototypes::Network_Event<TargetType>*>& bucket,requires(TargetType,!check_2(TargetType,typename MasterType::base_network_event_type,is_same)))
			{
				const int target_component_id = (int)TargetType::component_id;

				for(typename std::vector<Network_Event<typename MasterType::base_network_event_type>*>::iterator itr=_displayed_events.begin();itr!=_displayed_events.end();itr++)
				{
					if( (*itr)->_component_id == target_component_id )
					{
						bucket.push_back( (Network_Event<TargetType>*)*itr );
					}
				}
			}
			
			template<typename TargetType> void Get_Displayed_Messages(std::vector<Network_Event_Components::Prototypes::Network_Event<TargetType>*>& bucket,requires(TargetType,check_2(TargetType,typename MasterType::base_network_event_type,is_same)))
			{
				for(typename std::vector<Network_Event<typename MasterType::base_network_event_type>*>::iterator itr=_displayed_events.begin();itr!=_displayed_events.end();itr++)
				{
					bucket.push_back( *itr );
				}
			}

			//template<typename TargetType> void Get_Displayed_Messages(std::vector<Network_Event<TargetType>*>& bucket,requires(TargetType,!check(strip_modifiers(TargetType),Is_Polaris_Component)))
			//{
			//	static_assert(false,"TargetType is not a Polaris Component!");
			//}

			m_data(std::vector<Network_Event_Components::Prototypes::Network_Event<typename MasterType::base_network_event_type>*>, current_events, NONE, NONE);
			m_data(std::vector<Network_Event_Components::Prototypes::Network_Event<typename MasterType::base_network_event_type>*>, displayed_events, NONE, NONE);
			
			typedef Link_Components::Prototypes::Link<typename MasterType::link_type> Link_Interface;
			m_data(std::vector<Link_Interface*>,covered_links, NONE, NONE);

			m_prototype(Null_Prototype,typename MasterType::traffic_management_center_type,traffic_management_center, NONE, NONE);
		};

		//template<typename MasterType,typename InheritanceList>
		//std::vector<string> Simple_Advisory_ITS<MasterType,InheritanceList>::_component_keys;
	}
}
