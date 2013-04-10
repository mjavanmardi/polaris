#pragma once
#include "Advisory_Radio_Prototype.h"

namespace Advisory_Radio_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Highway_Advisory_Radio:public Polaris_Component<APPEND_CHILD(Highway_Advisory_Radio),MasterType,Data_Object>
		{
			feature_implementation static void Initialize_Type(void* obj)
			{
			}

			feature_implementation static void Setup_Type(const vector<shared_ptr<polaris::io::Component_Key>>& keys)
			{
				for(vector<shared_ptr<polaris::io::Component_Key>>::const_iterator itr=keys.begin();itr!=keys.end();itr++)
				{
					_component_keys.push_back( (*itr)->getKey() );
				}
			}
			
			feature_implementation void Setup(weak_ptr<polaris::io::Instance>& instance)
			{
				using namespace polaris::io;
				
				_x_position = instance.lock()->getLocation_X();
				_y_position = instance.lock()->getLocation_Y();
			}

			member_data(float, x_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, y_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			typedef Link_Prototype<typename type_of(MasterType::link),ComponentType> Link_Interface;
			member_data(vector<Link_Interface*>,covered_links,none,none);
			member_prototype(Traffic_Management_Center,traffic_management_center,typename type_of(MasterType::traffic_management_center),none,none);
			static member_data(vector<string>, component_keys, none, none);
		};

		template<typename MasterType,typename ParentType,typename InheritanceList>
		vector<string> Highway_Advisory_Radio<MasterType,ParentType,InheritanceList>::_component_keys;
	}
}

using namespace Advisory_Radio_Components::Implementations;