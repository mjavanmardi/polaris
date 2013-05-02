//*********************************************************
//	Antares_Advisory_Radio_Implementation.h - Graphical Advisory Radio
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Advisory_Radio_Prototype.h"
#include "Antares_Advisory_ITS_Implementation.h"

namespace Advisory_Radio_Components
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
		struct Antares_Advisory_Radio : public Advisory_ITS_Components::Implementations::Antares_Advisory_ITS<MasterType,ParentType,APPEND_CHILD(Antares_Advisory_Radio),InheritanceTemplate>
		{
			typedef typename Antares_Advisory_ITS::ComponentType ComponentType;

			feature_implementation static void Initialize_Type(string& name)
			{
				Antares_Advisory_ITS::Initialize_Type<ComponentType,CallerType,NT>();

				_its_component_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer),ComponentType>*, string& > >(name);

				Antares_Layer_Configuration cfg;
				cfg.Configure_Lines();
				cfg.grouped=true;
				cfg.head_size_value=4;
				cfg.head_accent_size_value=6;
				cfg.selection_callback=&on_select;
				cfg.double_click_callback=&on_double_click;
				cfg.submission_callback=&on_submit;

				cfg.head_color._r = 150;
				cfg.head_color._g = 75;
				cfg.head_color._b = 0;
				cfg.head_color._a = 255;

				_its_component_layer->Initialize<NULLTYPE>(cfg);
			}

			//feature_implementation void Accent_Self()
			//{
			//	Link_Line_Segment* segments = new Link_Line_Segment[ _covered_links.size() ];
			//		
			//	Link_Line_Group group;
			//	group.num_primitives = _covered_links.size();
			//	group.segments = segments;

			//	Link_Line_Segment* current_segment = group.segments;

			//	for(vector<Link_Prototype<typename type_of(MasterType::link),ComponentType>*>::iterator itr = _covered_links.begin(); itr != _covered_links.end(); itr++)
			//	{
			//		Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)(*itr);
			//		
			//		Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* intersection;
			//		
			//		intersection = link->upstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();
			//		
			//		current_segment->a._x = intersection->x_position<float>();
			//		current_segment->a._y = intersection->y_position<float>();
			//		current_segment->a._z = 2;

			//		Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->a );

			//		intersection = link->downstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();

			//		current_segment->b._x = intersection->x_position<float>();
			//		current_segment->b._y = intersection->y_position<float>();
			//		current_segment->b._z = 2;

			//		Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->b );

			//		++current_segment;
			//	}
			//	
			//	_its_component_layer->Push_Element<Accented_Element>(&group);

			//	delete[] segments;
			//}

			//feature_implementation void Initialize(vector<int>& db_covered_links)
			//{
			//	Antares_Advisory_ITS::Initialize<ComponentType,CallerType,NT>(db_covered_links);

			//	Link_Line_Segment* segments = new Link_Line_Segment[ _covered_links.size() ];
			//		
			//	Link_Line_Group group;
			//	group.num_primitives = _covered_links.size();
			//	group.segments = segments;
			//	group.object = (void*)((ComponentType*)this);

			//	Link_Line_Segment* current_segment = group.segments;

			//	for(vector<Link_Prototype<typename type_of(MasterType::link),ComponentType>*>::iterator itr = _covered_links.begin(); itr != _covered_links.end(); itr++)
			//	{
			//		Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)(*itr);
			//			
			//		Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* intersection;
			//			
			//		intersection = link->upstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();
			//			
			//		current_segment->a._x = intersection->x_position<float>();
			//		current_segment->a._y = intersection->y_position<float>();
			//		current_segment->a._z = 2;

			//		Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->a );

			//		intersection = link->downstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();

			//		current_segment->b._x = intersection->x_position<float>();
			//		current_segment->b._y = intersection->y_position<float>();
			//		current_segment->b._z = 2;

			//		Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->b );

			//		++current_segment;
			//	}
			//		
			//	_its_component_layer->Push_Element<Regular_Element>(&group);

			//	delete[] segments;
			//}
		};

		implementation struct Antares_Highway_Advisory_Radio : public Antares_Advisory_Radio<MasterType,NT,APPEND_CHILD(Antares_Highway_Advisory_Radio),Highway_Advisory_Radio>
		{
			feature_implementation static void Initialize_Type()
			{
				Antares_Advisory_Radio::Initialize_Type<ComponentType,CallerType,NT>(string("Highway Advisory Radios"));
			}
		};
	}
}