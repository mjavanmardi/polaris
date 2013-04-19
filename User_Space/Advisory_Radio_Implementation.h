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
			feature_implementation void Initialize(vector<int>& db_covered_links)
			{
				Simple_Advisory_ITS::Initialize<ComponentType,CallerType,TargetType>();
				
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
							((Link_Interface*)(*vitr))->advisory_radio< typename type_of(MasterType::advisory_radio)* >( (ComponentType*)this);
						}
					}
				}
			}
			
			typedef Link_Prototype<typename type_of(MasterType::link),ComponentType> Link_Interface;
			member_data(vector<Link_Interface*>,covered_links,none,none);
		};
	}
}

using namespace Advisory_Radio_Components::Implementations;