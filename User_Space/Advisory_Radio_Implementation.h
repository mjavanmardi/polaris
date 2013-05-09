#pragma once
#include "Advisory_Radio_Prototype.h"
#include "Advisory_ITS_Implementation.h"

namespace Advisory_Radio_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Highway_Advisory_Radio:public Advisory_ITS_Components::Implementations::Simple_Advisory_ITS<MasterType,NT,APPEND_CHILD(Highway_Advisory_Radio)>
		{
			feature_implementation void Initialize(polaris::io::HAR& instance)
			{
				using namespace polaris::io;

				std::tr1::shared_ptr<LinkList> link_list = instance.getLinks();

				const vector<int>& links = (*link_list).getLinks();
				((Advisory_ITS_Components::Implementations::Simple_Advisory_ITS<MasterType,NT,APPEND_CHILD(Highway_Advisory_Radio)>*)this)->template Initialize<ComponentType,ComponentType,const vector<int>&>(links);
			}
		};
	}
}

using namespace Advisory_Radio_Components::Implementations;