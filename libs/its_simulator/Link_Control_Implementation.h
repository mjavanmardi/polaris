#pragma once
#include "Link_Control_Prototype.h"

namespace Link_Control_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Lane_Link_Control:public Polaris_Component<MasterType,INHERIT(Lane_Link_Control),Data_Object>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Lane_Link_Control),Data_Object>::Component_Type ComponentType;
			template<typename TargetType> static void Initialize_Type(/*const std::vector<shared_ptr<polaris::io::Component_Key>>& keys*/)
			{
				//for(std::vector<shared_ptr<polaris::io::Component_Key>>::const_iterator itr=keys.begin();itr!=keys.end();itr++)
				//{
				//	_component_keys.push_back( (*itr)->getKey() );
				//}
			}
			
			//template<typename TargetType> void Initialize(polaris::io::OpenShoulder& instance)
			//{
			//	using namespace polaris::io;
			//	
			//	std::shared_ptr<LinkList> link_list = instance.getLinks();

			//	const std::vector<int>& db_covered_links = (*link_list).getLinks();

			//	std::unordered_map<int,std::vector<typename MasterType::link_type*>>& db_map=((Network<typename MasterType::network_type>*)_global_network)->template db_id_to_links_map<std::unordered_map<int,std::vector<typename MasterType::link_type*>>&>();

			//	for(std::vector<int>::const_iterator itr=db_covered_links.begin();itr!=db_covered_links.end();itr++)
			//	{
			//		int link = *itr;

			//		if(db_map.count(link))
			//		{
			//			std::vector<typename MasterType::link_type*>& links=db_map[link];

			//			typename std::vector<typename MasterType::link_type*>::iterator vitr;

			//			for(vitr=links.begin();vitr!=links.end();vitr++)
			//			{
			//				_covered_links.push_back( (Link_Interface*)(*vitr) );
			//				//((Link_Interface*)(*vitr))->Push_ITS< ComponentType* >( (ComponentType*)this);
			//			}
			//		}
			//	}

			//	_shoulder_opened=false;
			//}

			//m_data(float, x_position, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			//m_data(float, y_position, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			typedef Link_Components::Prototypes::Link<typename MasterType::link_type> Link_Interface;
			m_data(std::vector<Link_Interface*>,covered_links, NONE, NONE);
			m_data(bool,shoulder_opened, NONE, NONE);

			//m_prototype(Traffic_Management_Center<typename type_of(MasterType::traffic_management_center)>,traffic_management_center, NONE, NONE);
			//m_prototype(Link<typename type_of(MasterType::link)>,covered_link, NONE, NONE);
		};

		//template<typename MasterType,typename InheritanceList>
		//std::vector<string> Lane_Link_Control<MasterType,InheritanceList>::_component_keys;
	}
}

using namespace Link_Control_Components::Implementations;
