//*********************************************************
//	Antares_Link_Control_Implementation.h - Graphical Link_Controls
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Link_Control_Prototype.h"

namespace Link_Control_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{

	}
	
	namespace Implementations
	{
		template<typename MasterType,typename ParentType=NULLTYPE,typename InheritanceList=NULLTYPELIST,template<class,class,class> class InheritanceTemplate=NULLTEMPLATE_3>
		struct Antares_Link_Control : public InheritanceTemplate<MasterType,ParentType,APPEND_CHILD(Antares_Link_Control)>
		{
			typedef typename InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Link_Control)>::ComponentType ComponentType;
			
			feature_implementation static void Initialize_Type(string& name)
			{
				InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Link_Control)>::Initialize_Type<ComponentType,CallerType,NT>();

				_its_component_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer),ComponentType>*, string& > >(name);

				Antares_Layer_Configuration cfg;
				cfg.Configure_Lines();
				cfg.grouped=true;
				cfg.head_size_value=4;
				cfg.head_accent_size_value=6;
				cfg.selection_callback=&on_select;
				cfg.submission_callback=&on_submit;
				cfg.double_click_callback=&on_double_click;

				cfg.head_color._r = 200;
				cfg.head_color._g = 0;
				cfg.head_color._b = 200;
				cfg.head_color._a = 255;

				_its_component_layer->Initialize<NULLTYPE>(cfg);
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

				bool open_shoulder;

				if(dropdown_choices[0] == "Open Shoulder")
				{
					open_shoulder=true;
					its_component->_shoulder_opened=true;
				}
				else if(dropdown_choices[0] == "Close Shoulder")
				{
					open_shoulder=false;
					its_component->_shoulder_opened=false;
				}
				else return false;

				for(vector<Link_Prototype<typename type_of(MasterType::link),ComponentType>*>::iterator itr = its_component->_covered_links.begin(); itr != its_component->_covered_links.end(); itr++)
				{
					Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)(*itr);
					
					if(open_shoulder)
					{
						link->open_shoulder<NT>();
					}
					else
					{
						link->close_shoulder<NT>();
					}
				}

				return true;
			}

			static void on_double_click(const list<void*>& selected,vector<pair<string,string>>& attributes,vector<vector<string>>& dropdowns)
			{
				ComponentType* its_component=(ComponentType*)selected.back();

				dropdowns.resize(1);

				//for(vector< Network_Event< typename type_of(MasterType::base_network_event) >* >::iterator itr=its_component->_current_events.begin();itr!=its_component->_current_events.end();itr++)
				//{
					dropdowns[0].push_back( "Open Shoulder" );
					dropdowns[0].push_back( "Close Shoulder" );
				//}
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
				
				key_value_pair.first="Shoulder Opened";
				
				if(_shoulder_opened)
				{
					key_value_pair.second="Yes";
				}
				else
				{
					key_value_pair.second="No";
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

			feature_implementation void Initialize(TargetType configuration)
			{
				InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Link_Control)>::Initialize<ComponentType,CallerType,TargetType>(configuration);

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

			static member_prototype(Antares_Layer,its_component_layer,typename type_of(MasterType::antares_layer),none,none);
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList,template<class,class,class> class InheritanceTemplate>
		Antares_Layer<typename type_of(MasterType::antares_layer),typename Antares_Link_Control<MasterType,ParentType,InheritanceList,InheritanceTemplate>::ComponentType>* Antares_Link_Control<MasterType,ParentType,InheritanceList,InheritanceTemplate>::_its_component_layer;

		implementation struct Antares_Lane_Link_Control : public Antares_Link_Control<MasterType,NT,APPEND_CHILD(Antares_Lane_Link_Control),Lane_Link_Control>
		{
			feature_implementation static void Initialize_Type()
			{
				Antares_Link_Control::Initialize_Type<ComponentType,CallerType,NT>(string("Lane_Link_Controls"));
			}
		};
	}
}