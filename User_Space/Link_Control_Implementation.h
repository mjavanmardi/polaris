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
			typedef typename Polaris_Component<APPEND_CHILD(Lane_Link_Control),MasterType,Data_Object>::Component_Type ComponentType;
			feature_implementation static void Initialize_Type(/*const vector<shared_ptr<polaris::io::Component_Key>>& keys*/)
			{
				//for(vector<shared_ptr<polaris::io::Component_Key>>::const_iterator itr=keys.begin();itr!=keys.end();itr++)
				//{
				//	_component_keys.push_back( (*itr)->getKey() );
				//}
			}
			
			feature_implementation void Initialize(polaris::io::OpenShoulder& instance)
			{
				using namespace polaris::io;
				
				std::tr1::shared_ptr<LinkList> link_list = instance.getLinks();

				const vector<int>& db_covered_links = (*link_list).getLinks();

				unordered_map<int,vector<typename MasterType::link_type*>>& db_map=((Network_Prototype<typename type_of(MasterType::network),ComponentType>*)_global_network)->template db_id_to_links_map<unordered_map<int,vector<typename MasterType::link_type*>>&>();

				for(vector<int>::const_iterator itr=db_covered_links.begin();itr!=db_covered_links.end();itr++)
				{
					int link = *itr;

					if(db_map.count(link))
					{
						vector<typename MasterType::link_type*>& links=db_map[link];

						typename vector<typename type_of(MasterType::link)*>::iterator vitr;

						for(vitr=links.begin();vitr!=links.end();vitr++)
						{
							_covered_links.push_back( (Link_Interface*)(*vitr) );
							//((Link_Interface*)(*vitr))->Push_ITS< ComponentType* >( (ComponentType*)this);
						}
					}
				}

				_shoulder_opened=false;
			}

			//member_data(float, x_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			//member_data(float, y_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			typedef Link_Prototype<typename type_of(MasterType::link),ComponentType> Link_Interface;
			member_data(vector<Link_Interface*>,covered_links,none,none);
			member_data(bool,shoulder_opened,none,none);

			//member_prototype(Traffic_Management_Center,traffic_management_center,typename type_of(MasterType::traffic_management_center),none,none);
			//static member_data(vector<string>, component_keys, none, none);
			//member_prototype(Link_Prototype,covered_link,typename type_of(MasterType::link),none,none);
		};

		//template<typename MasterType,typename ParentType,typename InheritanceList>
		//vector<string> Lane_Link_Control<MasterType,ParentType,InheritanceList>::_component_keys;
	}
}

using namespace Link_Control_Components::Implementations;