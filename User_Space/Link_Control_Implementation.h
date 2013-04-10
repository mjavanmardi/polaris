#pragma once
#include "Link_Control_Prototype.h"

namespace Link_Control_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Lane_Link_Control:public Polaris_Component<APPEND_CHILD(Lane_Link_Control),MasterType,Data_Object>
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
			member_prototype(Traffic_Management_Center,traffic_management_center,typename type_of(MasterType::traffic_management_center),none,none);
			static member_data(vector<string>, component_keys, none, none);
			member_prototype(Link_Prototype,covered_link,typename type_of(MasterType::link),none,none);
		};

		template<typename MasterType,typename ParentType,typename InheritanceList>
		vector<string> Lane_Link_Control<MasterType,ParentType,InheritanceList>::_component_keys;
	}
}

using namespace Link_Control_Components::Implementations;