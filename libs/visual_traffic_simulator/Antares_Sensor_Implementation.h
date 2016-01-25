//*********************************************************
//	Antares_Sensor_Implementation.h - Graphical Sensors
//*********************************************************

#pragma once
#include "Dependencies.h"
#include "Traffic_Simulator\Sensor_Implementation.h"

namespace Sensor_Components
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
		struct Antares_Fixed_Sensor : public InheritanceTemplate<MasterType,INHERIT(Antares_Fixed_Sensor)>
		{
			typedef typename InheritanceTemplate<MasterType,INHERIT(Antares_Fixed_Sensor)>::ComponentType ComponentType;

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
				True_Color_RGBA<MasterType> color;
				int num_primitives;
				Link_Line_Segment* segments;
			};
#pragma pack(pop)
			
			declare_event(Sensor_Event)
			{
				InheritanceTemplate<MasterType,INHERIT(Antares_Fixed_Sensor)>::Sensor_Event<TargetType>(_this);


				ComponentType* pthis = (ComponentType*)_this;

				

				if( (iteration() + 1) % (((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()*10) == 0 )
				{
					//int write_some = rand()%100;

					//if(write_some==0)
					//{
					//	cout << "Coloring: " << iteration() << endl;
					//}

					pthis->Color_Sensors();
				}
			}

			template<typename TargetType> static void Initialize_Type(string& name)
			{
				//InheritanceTemplate<MasterType,NT,INHERIT(Antares_Depot)>::Initialize_Type<NT>();

				_its_component_layer=Allocate_New_Layer<MT>(name);

				Antares_Layer_Configuration cfg;
				cfg.Configure_Dynamic_Lines();
				cfg.grouped=true;
				cfg.head_size_value=4;
				cfg.head_accent_size_value=6;
				cfg.selection_callback=&on_select;
				cfg.target_sub_iteration=0;
				cfg.storage_offset=((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()*10-1;
				cfg.group_color=true;
				cfg.storage_period=((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()*10;

				cfg.head_color._r = 0;
				cfg.head_color._g = 0;
				cfg.head_color._b = 255;
				cfg.head_color._a = 255;

				_its_component_layer->Initialize<NULLTYPE>(cfg);
			}

			void Color_Sensors()
			{
				if(_detector)
				{
					Link_Line_Segment* segments = new Link_Line_Segment[ 1 ];
				
					Link_Line_Group group;
					group.num_primitives = 1;
					group.segments = segments;
					group.object = (void*)((ComponentType*)this);
				
					if(_outlier_detected)
					{
						group.color._r = 200;
						group.color._g = 0;
						group.color._b = 0;
						group.color._a = 255;
					}
					else
					{
						group.color._r = 0;
						group.color._g = 200;
						group.color._b = 0;
						group.color._a = 255;
					}

					Link_Line_Segment* current_segment = group.segments;


					Link<typename MasterType::link_type>* link = (Link<typename MasterType::link_type>*)_covered_link;
				
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


					_its_component_layer->Push_Element<Regular_Element>(&group);


				


					delete[] segments;
				}
			}

			template<typename TargetType> void Initialize(TargetType configuration)
			{
				InheritanceTemplate<MasterType,INHERIT(Antares_Fixed_Sensor)>::Initialize<TargetType>(configuration);

				//Link_Line_Segment* segments = new Link_Line_Segment[ 1 ];
				//
				//Link_Line_Group group;
				//group.num_primitives = 1;
				//group.segments = segments;
				//group.object = (void*)((ComponentType*)this);
				//group.color._r = 0;
				//group.color._g = 200;
				//group.color._b = 0;
				//group.color._a = 255;

				//Link_Line_Segment* current_segment = group.segments;


				//Link<typename MasterType::link_type>* link = (Link<typename MasterType::link_type>*)_covered_link;
				//
				//Intersection<typename MasterType::intersection_type>* intersection;
				//
				//intersection = link->upstream_intersection< Intersection<typename MasterType::intersection_type>* >();
				//		
				//current_segment->a._x = intersection->x_position<float>();
				//current_segment->a._y = intersection->y_position<float>();
				//current_segment->a._z = 3;

//TODO
//				//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->a );

				//intersection = link->downstream_intersection< Intersection<typename MasterType::intersection_type>* >();

				//current_segment->b._x = intersection->x_position<float>();
				//current_segment->b._y = intersection->y_position<float>();
				//current_segment->b._z = 3;

//TODO
//				//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->b );


				//_its_component_layer->Push_Element<Regular_Element>(&group);

				//delete[] segments;
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
			}

			template<typename TargetType> void Accent_Self()
			{
				Link_Line_Segment* segments = new Link_Line_Segment[ 1 ];
				
				Link_Line_Group group;
				group.num_primitives = 1;
				group.segments = segments;

				Link_Line_Segment* current_segment = group.segments;

				Link<typename MasterType::link_type>* link = (Link<typename MasterType::link_type>*)_covered_link;
				
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
				
				_its_component_layer->Push_Element<Accented_Element>(&group);

				delete[] segments;
			}

			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,its_component_layer, NONE, NONE);
		};
		
		template<typename MasterType,typename InheritanceList,template<class,class> class InheritanceTemplate>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Fixed_Sensor<MasterType,InheritanceList,InheritanceTemplate>::_its_component_layer;

		implementation struct Antares_Link_Sensor : public Antares_Fixed_Sensor<MasterType,INHERIT(Antares_Link_Sensor),Link_Sensor>
		{
			template<typename TargetType> static void Initialize_Type()
			{
				Antares_Fixed_Sensor::Initialize_Type<NT>(string("Link Sensors"));
			}
		};
	}
}
