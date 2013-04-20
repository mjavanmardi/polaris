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
			feature_implementation void Initialize()
			{
			}

			feature_implementation static void Initialize_Type(const vector<shared_ptr<polaris::io::Component_Key>>& keys)
			{
				for(vector<shared_ptr<polaris::io::Component_Key>>::const_iterator itr=keys.begin();itr!=keys.end();itr++)
				{
					_component_keys.push_back( (*itr)->getKey() );
				}
			}

			feature_implementation void Accept_Network_Events(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>& network_events)
			{
				_current_events.clear();

				for(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>::iterator itr = network_events.begin();itr!=network_events.end();itr++)
				{
					_current_events.push_back( *itr );
					_displayed_events.push_back( *itr );
				}
			}
			
			feature_implementation void Get_Displayed_Messages(vector<Network_Event<TargetType>*>& bucket,requires(!check_2(TargetType,typename type_of(MasterType::base_network_event),is_same)))
			{
				const int target_component_index = TargetType::component_index;

				for(vector<Network_Event<typename type_of(MasterType::base_network_event)>*::iterator itr=_displayed_events.begin();itr!=_displayed_events.end();itr++)
				{
					if( (*itr)->_component_index == target_component_index )
					{
						bucket.push_back( *itr );
					}
				}
			}
			
			feature_implementation void Get_Displayed_Messages(vector<Network_Event<TargetType>*>& bucket,requires(check_2(TargetType,typename type_of(MasterType::base_network_event),is_same)))
			{
				for(vector<Network_Event<typename type_of(MasterType::base_network_event)>*::iterator itr=_displayed_events.begin();itr!=_displayed_events.end();itr++)
				{
					bucket.push_back( *itr );
				}
			}

			member_data(float, x_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, y_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>, current_events, none, none);
			member_data(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>, displayed_events, none, none);

			member_prototype(Traffic_Management_Center,traffic_management_center,typename type_of(MasterType::traffic_management_center),none,none);

			static member_data(vector<string>, component_keys, none, none);
		};

		template<typename MasterType,typename ParentType,typename InheritanceList>
		vector<string> Simple_Advisory_ITS<MasterType,ParentType,InheritanceList>::_component_keys;
	}
}