//*********************************************************
//	Antares_Advisory_ITS_Implementation.h - Graphical Adv ITS
//*********************************************************

#pragma once
#include "Antares_Includes.h"

namespace Advisory_ITS_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		template<typename MasterType,typename ParentType=NULLTYPE,typename InheritanceList=NULLTYPELIST,template<class,class,class> class InheritanceTemplate=NULLTEMPLATE_3>
		struct Antares_Advisory_ITS : public InheritanceTemplate<MasterType,ParentType,APPEND_CHILD(Antares_Advisory_ITS)>
		{
			typedef typename InheritanceTemplate<MasterType,ParentType,APPEND_CHILD(Antares_Advisory_ITS)>::ComponentType ComponentType;
			
			feature_implementation void Accept_Network_Events(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>& network_events)
			{
				_current_events.clear();

				for(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>::iterator itr = network_events.begin();itr!=network_events.end();itr++)
				{
					_current_events.push_back( *itr );
				}
			}

#pragma pack(push,1)
			struct Link_Line_Segment
			{
				Point_3D<MasterType> a;
				Point_3D<MasterType> b;
			};
#pragma pack(pop)

#pragma pack(push,1)
			struct Link_Line_Group
			{
				void* object;
				int num_primitives;
				Link_Line_Segment* segments;
			};
#pragma pack(pop)

			static bool on_submit(const list<void*>& selected,const vector<string>& attribute_choices,const vector<string>& dropdown_choices)
			{
				ComponentType* its_component=(ComponentType*)selected.back();
				
				string user_event_choice = dropdown_choices[0];

				for(vector< Network_Event< typename type_of(MasterType::base_network_event) >* >::iterator itr=its_component->_current_events.begin();itr!=its_component->_current_events.end();itr++)
				{
					if(user_event_choice == (*itr)->notes<string&>())
					{
						its_component->_displayed_events.push_back( *itr );
						return true;
					}
				}

				return false;
			}

			static void on_double_click(const list<void*>& selected,vector<pair<string,string>>& attributes,vector<vector<string>>& dropdowns)
			{
				ComponentType* its_component=(ComponentType*)selected.back();

				dropdowns.resize(1);

				for(vector< Network_Event< typename type_of(MasterType::base_network_event) >* >::iterator itr=its_component->_current_events.begin();itr!=its_component->_current_events.end();itr++)
				{
					dropdowns[0].push_back( (*itr)->notes<string&>() );
				}
			}

			static void on_select(const list<void*>& removed,const list<void*>& added,const list<void*>& selected,vector<pair<string,string>>& bucket)
			{
				if(removed.size())
				{
					_its_component_layer->Clear_Accented<NT>();

					if(selected.size())
					{
						for(list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
						{
							((ComponentType*)*itr)->Accent_Self<ComponentType,ComponentType,NT>();
						}
					}
				}
				else if(added.size())
				{
					for(list<void*>::const_iterator itr=added.begin();itr!=added.end();itr++)
					{
						((ComponentType*)*itr)->Accent_Self<ComponentType,ComponentType,NT>();
					}
				}
			}

			static member_prototype(Antares_Layer,its_component_layer,typename type_of(MasterType::antares_layer),none,none);
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList,template<class,class,class> class InheritanceTemplate>
		Antares_Layer<typename type_of(MasterType::antares_layer),typename Antares_Advisory_ITS<MasterType,ParentType,InheritanceList,InheritanceTemplate>::ComponentType>* Antares_Advisory_ITS<MasterType,ParentType,InheritanceList,InheritanceTemplate>::_its_component_layer;

	}
}