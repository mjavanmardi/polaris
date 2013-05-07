#pragma once
#include "Advisory_ITS_Prototype.h"

namespace Advisory_ITS_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Simple_Advisory_ITS : public Polaris_Component<APPEND_CHILD(Simple_Advisory_ITS),MasterType,Data_Object>
		{
			feature_implementation void Initialize(const vector<int>& db_covered_links/*,bool flag=false*/)
			{
				using namespace polaris::io;

				unordered_map<int,vector<typename MasterType::link_type*>>& db_map=((Network_Prototype<typename type_of(MasterType::network),ComponentType>*)_global_network)->db_id_to_links_map<unordered_map<int,vector<typename MasterType::link_type*>>&>();

				for(vector<int>::const_iterator itr=db_covered_links.begin();itr!=db_covered_links.end();itr++)
				{
					int link = *itr;

					if(db_map.count(link))
					{
						vector<typename MasterType::link_type*>& links=db_map[link];

						vector<typename type_of(MasterType::link)*>::iterator vitr;

						for(vitr=links.begin();vitr!=links.end();vitr++)
						{
							_covered_links.push_back( (Link_Interface*)(*vitr) );
							//if(flag) cout << "pushing HAR to: " << ((Link_Interface*)(*vitr))->internal_id<int>();
							((Link_Interface*)(*vitr))->Push_ITS< ComponentType* >( (ComponentType*)this);
						}
					}
				}
			}

			feature_implementation static void Initialize_Type()
			{
			}

			feature_implementation void Accept_Network_Events(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>& network_events)
			{
				_current_events.clear();

				for(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>::iterator itr = network_events.begin();itr!=network_events.end();itr++)
				{
					_current_events.push_back( *itr );
				}
			}

			feature_implementation void Accept_Displayed_Network_Events(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>& network_events)
			{
				_displayed_events.clear();
				for (vector<Network_Event<typename type_of(MasterType::base_network_event)>*>::iterator itr = network_events.begin(); itr!=network_events.end(); itr++)
					_displayed_events.push_back( *itr );
			}
			
			feature_implementation void Get_Displayed_Messages(vector<Network_Event<TargetType>*>& bucket,requires(!check_2(TargetType,typename type_of(MasterType::base_network_event),is_same) && check(TargetType,Is_Polaris_Component)))
			{
				const int target_component_index = TargetType::component_index;

				for(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>::iterator itr=_displayed_events.begin();itr!=_displayed_events.end();itr++)
				{
					if( (*itr)->_component_index == target_component_index )
					{
						bucket.push_back( (Network_Event<TargetType>*)*itr );
					}
				}
			}
			
			feature_implementation void Get_Displayed_Messages(vector<Network_Event<TargetType>*>& bucket,requires(check_2(TargetType,typename type_of(MasterType::base_network_event),is_same)))
			{
				for(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>::iterator itr=_displayed_events.begin();itr!=_displayed_events.end();itr++)
				{
					bucket.push_back( *itr );
				}
			}

			feature_implementation void Get_Displayed_Messages(vector<Network_Event<TargetType>*>& bucket,requires(!check(TargetType,Is_Polaris_Component)))
			{
				static_assert(false,"TargetType is not a Polaris Component!");
			}

			member_data(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>, current_events, none, none);
			member_data(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>, displayed_events, none, none);
			
			typedef Link_Prototype<typename type_of(MasterType::link),ComponentType> Link_Interface;
			member_data(vector<Link_Interface*>,covered_links,none,none);

			member_prototype(Traffic_Management_Center,traffic_management_center,typename type_of(MasterType::traffic_management_center),none,none);

			//static member_data(vector<string>, component_keys, none, none);
		};

		//template<typename MasterType,typename ParentType,typename InheritanceList>
		//vector<string> Simple_Advisory_ITS<MasterType,ParentType,InheritanceList>::_component_keys;
	}
}