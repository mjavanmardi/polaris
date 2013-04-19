#pragma once
#include "Variable_Message_Sign_Prototype.h"
#include "Advisory_ITS_Implementation.h"

namespace Variable_Message_Sign_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Simple_Variable_Message_Sign:public Advisory_ITS_Components::Implementations::Simple_Advisory_ITS<MasterType,NT,APPEND_CHILD(Simple_Variable_Message_Sign)>
		{
			feature_implementation void Initialize(weak_ptr<polaris::io::Instance>& instance)
			{
				Simple_Advisory_ITS::Initialize<ComponentType,CallerType,TargetType>();

				using namespace polaris::io;
				
				_x_position = instance.lock()->getLocation_X();
				_y_position = instance.lock()->getLocation_Y();

				typename type_of(MasterType::network)::type_of(links_container)& net_links=((Network_Prototype<typename type_of(MasterType::network),ComponentType>*)_global_network)->links_container<typename type_of(MasterType::network)::type_of(links_container)&>();
				_covered_link = (covered_link_interface*)(net_links[rand()%net_links.size()]);

			}

			member_prototype(Link_Prototype,covered_link,typename type_of(MasterType::link),none,none);
		};

		implementation struct Variable_Speed_Sign:public Simple_Variable_Message_Sign<MasterType,NT,APPEND_CHILD(Variable_Speed_Sign)>
		{
			feature_implementation void Initialize(weak_ptr<polaris::io::Instance>& instance)
			{
				using namespace polaris::io;

				Simple_Variable_Message_Sign::Initialize< ComponentType,ComponentType,weak_ptr<Instance>& >(instance);				

				_covered_link->variable_speed_sign< typename type_of(MasterType::variable_speed_sign)* >( (ComponentType*)this );

				//const vector<shared_ptr<Instance_Value>>& values=instance.lock()->getValues();

				//for(vector<shared_ptr<Instance_Value>>::const_iterator itr=values.begin();itr!=values.end();itr++)
				//{
				//}
			}
		};

		implementation struct Variable_Word_Sign:public Simple_Variable_Message_Sign<MasterType,NT,APPEND_CHILD(Variable_Word_Sign)>
		{
			feature_implementation void Initialize(weak_ptr<polaris::io::Instance>& instance)
			{
				using namespace polaris::io;

				Simple_Variable_Message_Sign::Initialize< ComponentType,ComponentType,weak_ptr<Instance>& >(instance);
				
				_covered_link->variable_word_sign< typename type_of(MasterType::variable_word_sign)* >( (ComponentType*)this );

				//const vector<shared_ptr<Instance_Value>>& values=instance.lock()->getValues();

				//for(vector<shared_ptr<Instance_Value>>::const_iterator itr=values.begin();itr!=values.end();itr++)
				//{
				//}
			}
		};
	}
}

using namespace Variable_Message_Sign_Components::Implementations;