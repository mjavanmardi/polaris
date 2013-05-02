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

			feature_implementation void Initialize(TargetType configuration)
			{
				InheritanceTemplate<MasterType,ParentType,APPEND_CHILD(Antares_Advisory_ITS)>::Initialize<ComponentType,CallerType,TargetType>(configuration);

				if(_covered_links.size())
				{
					Link_Line_Segment* segments = new Link_Line_Segment[ _covered_links.size() ];
					
					Link_Line_Group group;
					group.num_primitives = _covered_links.size();
					group.segments = segments;
					group.object = (void*)((ComponentType*)this);

					Link_Line_Segment* current_segment = group.segments;

					for(vector<Link_Prototype<typename type_of(MasterType::link),ComponentType>*>::iterator itr = _covered_links.begin(); itr != _covered_links.end(); itr++)
					{
						Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)(*itr);
						
						Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* intersection;
						
						intersection = link->upstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();
						
						current_segment->a._x = intersection->x_position<float>();
						current_segment->a._y = intersection->y_position<float>();
						current_segment->a._z = 3;

						Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->a );

						intersection = link->downstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();

						current_segment->b._x = intersection->x_position<float>();
						current_segment->b._y = intersection->y_position<float>();
						current_segment->b._z = 3;

						Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->b );

						++current_segment;
					}
					
					_its_component_layer->Push_Element<Regular_Element>(&group);

					delete[] segments;
				}
			}

			feature_implementation void Accept_Network_Events(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>& network_events)
			{
				_current_events.clear();

				for(vector<Network_Event<typename type_of(MasterType::base_network_event)>*>::iterator itr = network_events.begin();itr!=network_events.end();itr++)
				{
					_current_events.push_back( *itr );
				}
			}

			static bool on_submit(const list<void*>& selected,const vector<string>& attribute_choices,const vector<string>& dropdown_choices)
			{
				string user_event_choice = dropdown_choices[0];
				bool update_successful = false;

				for(list<void*>::const_iterator sitr=selected.begin();sitr!=selected.end();sitr++)
				{
					ComponentType* its_component=(ComponentType*) (*sitr);

					for(vector< Network_Event< typename type_of(MasterType::base_network_event) >* >::iterator itr=its_component->_current_events.begin();itr!=its_component->_current_events.end();itr++)
					{
						if(user_event_choice == (*itr)->notes<string&>())
						{
							its_component->_displayed_events.clear();
							its_component->_displayed_events.push_back( *itr );

							update_successful = true;
						}
					}
				}

				if(update_successful) return true;
				else return false;
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

				if(selected.size())
				{
					((ComponentType*) (selected.back()))->Display_Attributes<ComponentType,ComponentType,NT>(bucket);
				}
			}

			feature_implementation void Display_Attributes(vector<pair<string,string>>& bucket)
			{
				pair<string,string> key_value_pair;
				
				key_value_pair.first="Displayed";

				if(_displayed_events.size())
				{
					key_value_pair.second=_displayed_events[0]->notes<string&>();
				}
				else
				{
					key_value_pair.second="None Displayed";
				}
				
				bucket.push_back(key_value_pair);
			}

			feature_implementation void Accent_Self()
			{
				Link_Line_Segment* segments = new Link_Line_Segment[ _covered_links.size() ];
					
				Link_Line_Group group;
				group.num_primitives = _covered_links.size();
				group.segments = segments;

				Link_Line_Segment* current_segment = group.segments;

				for(vector<Link_Prototype<typename type_of(MasterType::link),ComponentType>*>::iterator itr = _covered_links.begin(); itr != _covered_links.end(); itr++)
				{
					Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)(*itr);
					
					Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* intersection;
					
					intersection = link->upstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();
					
					current_segment->a._x = intersection->x_position<float>();
					current_segment->a._y = intersection->y_position<float>();
					current_segment->a._z = 3;

					Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->a );

					intersection = link->downstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();

					current_segment->b._x = intersection->x_position<float>();
					current_segment->b._y = intersection->y_position<float>();
					current_segment->b._z = 3;

					Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->b );

					++current_segment;
				}
				
				_its_component_layer->Push_Element<Accented_Element>(&group);

				delete[] segments;
			}

			static member_prototype(Antares_Layer,its_component_layer,typename type_of(MasterType::antares_layer),none,none);
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList,template<class,class,class> class InheritanceTemplate>
		Antares_Layer<typename type_of(MasterType::antares_layer),typename Antares_Advisory_ITS<MasterType,ParentType,InheritanceList,InheritanceTemplate>::ComponentType>* Antares_Advisory_ITS<MasterType,ParentType,InheritanceList,InheritanceTemplate>::_its_component_layer;

	}
}