//*********************************************************
//	Antares_Link_Control_Implementation.h - Graphical Link_Controls
//*********************************************************

#pragma once
#include "Dependencies.h"
#include "Traffic_Simulator\Link_Control_Implementation.h"

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
		template<typename MasterType,typename InheritanceList=NULLTYPELIST,template<class,class> class InheritanceTemplate=NULLTEMPLATE_2>
		struct Antares_Link_Control : public InheritanceTemplate<MasterType,INHERIT(Antares_Link_Control)>
		{
			typedef typename InheritanceTemplate<MasterType,INHERIT(Antares_Link_Control)>::ComponentType ComponentType;
			
			template<typename TargetType> static void Initialize_Type(string& name)
			{
				InheritanceTemplate<MasterType,INHERIT(Antares_Link_Control)>::Initialize_Type<NT>();

				_its_component_layer=Allocate_New_Layer<MT>(name);

				Antares_Layer_Configuration cfg;
				cfg.Configure_Static_Lines();
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
			static bool on_submit(const std::list<void*>& selected,const std::vector<string>& attribute_choices,const std::vector<string>& dropdown_choices)
			{
				//ComponentType* its_component=(ComponentType*)selected.back();

				bool open_shoulder;

				if(dropdown_choices[0] == "Open Shoulder")
				{
					open_shoulder=true;
					//its_component->_shoulder_opened=true;
				}
				else if(dropdown_choices[0] == "Close Shoulder")
				{
					open_shoulder=false;
					//its_component->_shoulder_opened=false;
				}
				else return false;

				for(std::list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
				{
					ComponentType* its_component=(ComponentType*) (*itr);

					if(open_shoulder)
					{
						its_component->_shoulder_opened=true;
					}
					else
					{
						its_component->_shoulder_opened=false;
					}

					for(std::vector<Link<typename MasterType::link_type>*>::iterator itr = its_component->_covered_links.begin(); itr != its_component->_covered_links.end(); itr++)
					{
						Link<typename MasterType::link_type>* link = (Link<typename MasterType::link_type>*)(*itr);
					
						if(open_shoulder)
						{
							link->open_shoulder<NT>();
						}
						else
						{
							link->close_shoulder<NT>();
						}
					}
				}

				return true;
			}

			static void on_double_click(const std::list<void*>& selected,std::vector<pair<string,string>>& attributes,std::vector<std::vector<string>>& dropdowns)
			{
				ComponentType* its_component=(ComponentType*)selected.back();

				dropdowns.resize(1);

				//for(std::vector< Network_Event< typename MasterType::base_network_event_type >* >::iterator itr=its_component->_current_events.begin();itr!=its_component->_current_events.end();itr++)
				//{
					dropdowns[0].push_back( "Open Shoulder" );
					dropdowns[0].push_back( "Close Shoulder" );
				//}
			}

			static void on_select(const std::list<void*>& removed,const std::list<void*>& added,const std::list<void*>& selected,std::vector<pair<string,string>>& bucket)
			{
				if(removed.size())
				{
					_its_component_layer->Clear_Accented<NT>();

					if(selected.size())
					{
						for(std::list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
						{
							((ComponentType*)*itr)->Accent_Self<NT>();
						}
					}
				}
				else if(added.size())
				{
					for(std::list<void*>::const_iterator itr=added.begin();itr!=added.end();itr++)
					{
						((ComponentType*)*itr)->Accent_Self<NT>();
					}
				}

				if(selected.size())
				{
					((ComponentType*) (selected.back()))->Display_Attributes<NT>(bucket);
				}
			}

			template<typename TargetType> void Display_Attributes(std::vector<pair<string,string>>& bucket)
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

			template<typename TargetType> void Accent_Self()
			{
				Link_Line_Segment* segments = new Link_Line_Segment[ _covered_links.size() ];
					
				Link_Line_Group group;
				group.num_primitives = _covered_links.size();
				group.segments = segments;

				Link_Line_Segment* current_segment = group.segments;

				for(std::vector<Link<typename MasterType::link_type>*>::iterator itr = _covered_links.begin(); itr != _covered_links.end(); itr++)
				{
					Link<typename MasterType::link_type>* link = (Link<typename MasterType::link_type>*)(*itr);
					
					Intersection<typename MasterType::intersection_type>* intersection;
					
					intersection = link->upstream_intersection< Intersection<typename MasterType::intersection_type>* >();
					
					current_segment->a._x = intersection->x_position<float>();
					current_segment->a._y = intersection->y_position<float>();
					current_segment->a._z = 3;

					Scale_Coordinates<MT>( current_segment->a );

					intersection = link->downstream_intersection< Intersection<typename MasterType::intersection_type>* >();

					current_segment->b._x = intersection->x_position<float>();
					current_segment->b._y = intersection->y_position<float>();
					current_segment->b._z = 3;

					Scale_Coordinates<MT>( current_segment->b );

					++current_segment;
				}
				
				_its_component_layer->Push_Element<Accented_Element>(&group);

				delete[] segments;
			}

			template<typename TargetType> void Initialize(TargetType configuration)
			{
				InheritanceTemplate<MasterType,INHERIT(Antares_Link_Control)>::Initialize<TargetType>(configuration);

				if(_covered_links.size())
				{
					Link_Line_Segment* segments = new Link_Line_Segment[ _covered_links.size() ];
					
					Link_Line_Group group;
					group.num_primitives = _covered_links.size();
					group.segments = segments;
					group.object = (void*)((ComponentType*)this);

					Link_Line_Segment* current_segment = group.segments;

					for(std::vector<Link<typename MasterType::link_type>*>::iterator itr = _covered_links.begin(); itr != _covered_links.end(); itr++)
					{
						Link<typename MasterType::link_type>* link = (Link<typename MasterType::link_type>*)(*itr);
						
						Intersection<typename MasterType::intersection_type>* intersection;
						
						intersection = link->upstream_intersection< Intersection<typename MasterType::intersection_type>* >();
						
						current_segment->a._x = intersection->x_position<float>();
						current_segment->a._y = intersection->y_position<float>();
						current_segment->a._z = 3;

						Scale_Coordinates<MT>( current_segment->a );

						intersection = link->downstream_intersection< Intersection<typename MasterType::intersection_type>* >();

						current_segment->b._x = intersection->x_position<float>();
						current_segment->b._y = intersection->y_position<float>();
						current_segment->b._z = 3;

						Scale_Coordinates<MT>( current_segment->b );

						++current_segment;
					}
					
					_its_component_layer->Push_Element<Regular_Element>(&group);

					delete[] segments;
				}
			}

			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,its_component_layer, NONE, NONE);
		};
		
		template<typename MasterType,typename InheritanceList,template<class,class> class InheritanceTemplate>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Link_Control<MasterType,InheritanceList,InheritanceTemplate>::_its_component_layer;

		implementation struct Antares_Lane_Link_Control : public Antares_Link_Control<MasterType,INHERIT(Antares_Lane_Link_Control),Lane_Link_Control>
		{
			template<typename TargetType> static void Initialize_Type()
			{
				Antares_Link_Control::Initialize_Type<NT>(string("Lane_Link_Controls"));
			}
		};
	}
}
