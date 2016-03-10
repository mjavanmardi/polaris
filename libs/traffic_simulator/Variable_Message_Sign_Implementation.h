#pragma once
#include "Variable_Message_Sign_Prototype.h"
#include "Advisory_ITS_Implementation.h"

namespace Variable_Message_Sign_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		//implementation struct Simple_Variable_Message_Sign:public Advisory_ITS_Components::Implementations::Simple_Advisory_ITS<MasterType,NT,INHERIT(Simple_Variable_Message_Sign)>
		//{
		//	template<typename TargetType> void Initialize(weak_ptr<polaris::io::Instance>& instance)
		//	{
		//		Simple_Advisory_ITS::template Initialize<TargetType>();

		//		using namespace polaris::io;
		//		
		//		_x_position = instance.lock()->getLocation_X();
		//		_y_position = instance.lock()->getLocation_Y();

		//		typename type_of(MasterType::network)::type_of(links_container)& net_links=((Network<typename type_of(MasterType::network)>*)_global_network)->links_container<typename type_of(MasterType::network)::type_of(links_container)&>();
		//		_covered_link = (covered_link_interface*)(net_links[rand()%net_links.size()]);
		//	}

		//	m_prototype(Link<typename type_of(MasterType::link)>,covered_link, NONE, NONE);
		//};

		implementation struct Variable_Speed_Sign:public Advisory_ITS_Components::Implementations::Simple_Advisory_ITS<MasterType,INHERIT(Variable_Speed_Sign)>
		{
			template<typename TargetType> void Initialize(polaris::io::VSS& instance)
			{
				using namespace polaris::io;

				//shared_ptr<LinkList> link_list = instance.getLinks();

				//const std::vector<int>& links = (*link_list).getLinks();

				std::vector<int> links;
				links.push_back(instance.getLink());

				//Simple_Advisory_ITS::template Initialize<const std::vector<int>&>(links);
				((Advisory_ITS_Components::Implementations::Simple_Advisory_ITS<MasterType,INHERIT(Variable_Speed_Sign)>*)this)->template Initialize<const std::vector<int>&>(links);
			}
		};

		implementation struct Variable_Word_Sign:public Advisory_ITS_Components::Implementations::Simple_Advisory_ITS<MasterType,INHERIT(Variable_Word_Sign)>
		{
			template<typename TargetType> void Initialize(polaris::io::VMS& instance)
			{
				using namespace polaris::io;

				std::vector<int> links;
				links.push_back(instance.getLink());

				//Simple_Advisory_ITS::template Initialize<const std::vector<int>&>(links);
				((Advisory_ITS_Components::Implementations::Simple_Advisory_ITS<MasterType,INHERIT(Variable_Word_Sign)>*)this)->template Initialize<const std::vector<int>&>(links);
			}
		};
	}
}

using namespace Variable_Message_Sign_Components::Implementations;
