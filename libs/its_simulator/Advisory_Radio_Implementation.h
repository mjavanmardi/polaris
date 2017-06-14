#pragma once
#include "Advisory_Radio_Prototype.h"
#include "Advisory_ITS_Implementation.h"

namespace Advisory_Radio_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Highway_Advisory_Radio:public Advisory_ITS_Components::Implementations::Simple_Advisory_ITS<MasterType,INHERIT(Highway_Advisory_Radio)>
		{
			template<typename TargetType> void Initialize(polaris::io::HAR& instance)
			{
				using namespace polaris::io;

				//std::shared_ptr<LinkList> link_list = instance.getLinks();

				//const std::vector<int>& links = (*link_list).getLinks();
				//((Advisory_ITS_Components::Implementations::Simple_Advisory_ITS<MasterType,INHERIT(Highway_Advisory_Radio)>*)this)->template Initialize<const std::vector<int>&>(links);
				((Advisory_ITS_Components::Implementations::Simple_Advisory_ITS<MasterType, INHERIT(Highway_Advisory_Radio)>*)this)->Initialize();
			}
		};
	}
}

using namespace Advisory_Radio_Components::Implementations;
