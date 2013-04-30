//*********************************************************
//	Antares_Variable_Message_Sign_Implementation.h - Graphical VMS
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Variable_Message_Sign_Prototype.h"
#include "Antares_Advisory_ITS_Implementation.h"

namespace Variable_Message_Sign_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{

	}
	
	namespace Implementations
	{
		//template<typename MasterType,typename ParentType=NULLTYPE,typename InheritanceList=NULLTYPELIST,template<class,class,class> class InheritanceTemplate=NULLTEMPLATE_3>
		//struct Antares_Variable_Message_Sign : public Advisory_ITS_Components::Implementations::Antares_Advisory_ITS<MasterType,ParentType,APPEND_CHILD(Antares_Variable_Message_Sign),InheritanceTemplate>
		//{
		//	typedef typename Antares_Advisory_ITS::ComponentType ComponentType;

		//	feature_implementation static void Initialize_Type(const vector<shared_ptr<polaris::io::Component_Key>>& keys,string& name)
		//	{
		//		InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Variable_Message_Sign)>::Initialize_Type<ComponentType,CallerType,NT>(keys);

		//		_its_component_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer),ComponentType>*, string& > >(name);

		//		Antares_Layer_Configuration cfg;
		//		cfg.Configure_Lines();
		//		cfg.grouped=true;
		//		cfg.head_size_value=4;
		//		cfg.selection_callback=&on_select;
		//		cfg.double_click_callback=&on_double_click;
		//		cfg.submission_callback=&on_submit;

		//		cfg.head_color._r = 0;
		//		cfg.head_color._g = 0;
		//		cfg.head_color._b = 255;
		//		cfg.head_color._a = 255;

		//		_its_component_layer->Initialize<NULLTYPE>(cfg);
		//	}

		//	feature_implementation void Accent_Self()
		//	{
		//		Link_Line_Segment segment;

		//		Link_Line_Group group;
		//		group.num_primitives = 1;
		//		group.segments = &segment;

		//		Link_Line_Segment* current_segment = group.segments;

		//		Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)_covered_link;
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
		//		
		//		_its_component_layer->Push_Element<Accented_Element>(&group);
		//	}

		//	feature_implementation void Initialize(weak_ptr<polaris::io::Instance>& instance)
		//	{
		//		Antares_Advisory_ITS::Initialize<ComponentType,CallerType,NT>(instance);

		//		vector<Link_Prototype<typename type_of(MasterType::link),ComponentType>*>::iterator itr;

		//		Link_Line_Segment segment;

		//		Link_Line_Group group;
		//		group.num_primitives = 1;
		//		group.segments = &segment;
		//		group.object = (void*)((ComponentType*)this);

		//		Link_Line_Segment* current_segment = group.segments;

		//		Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)_covered_link;
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

		//		_its_component_layer->Push_Element<Regular_Element>(&group);
		//	}
		//};

		implementation struct Antares_Variable_Speed_Sign : public Advisory_ITS_Components::Implementations::Antares_Advisory_ITS<MasterType,ParentType,APPEND_CHILD(Antares_Variable_Speed_Sign),Variable_Speed_Sign>
		{
			feature_implementation static void Initialize_Type()
			{
				using namespace Advisory_ITS_Components::Implementations;

				//Antares_Advisory_ITS::Initialize_Type<ComponentType,CallerType,NT>();

				_its_component_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer),ComponentType>*, string& > >(string("Variable Speed Signs"));

				Antares_Layer_Configuration cfg;
				cfg.Configure_Lines();
				cfg.grouped=true;
				cfg.head_size_value=4;
				cfg.head_accent_size_value=6;
				cfg.selection_callback=&on_select;
				cfg.double_click_callback=&on_double_click;
				cfg.submission_callback=&on_submit;

				cfg.head_color._r = 0;
				cfg.head_color._g = 0;
				cfg.head_color._b = 255;
				cfg.head_color._a = 255;

				_its_component_layer->Initialize<NULLTYPE>(cfg);
			}
		};

		implementation struct Antares_Variable_Word_Sign : public Advisory_ITS_Components::Implementations::Antares_Advisory_ITS<MasterType,ParentType,APPEND_CHILD(Antares_Variable_Word_Sign),Variable_Word_Sign>
		{
			feature_implementation static void Initialize_Type()
			{
				using namespace Advisory_ITS_Components::Implementations;

				//Antares_Advisory_ITS::Initialize_Type<ComponentType,CallerType,NT>();

				_its_component_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer),ComponentType>*, string& > >(string("Variable Message Signs"));

				Antares_Layer_Configuration cfg;
				cfg.Configure_Lines();
				cfg.grouped=true;
				cfg.head_size_value=4;
				cfg.selection_callback=&on_select;
				cfg.double_click_callback=&on_double_click;
				cfg.submission_callback=&on_submit;

				cfg.head_color._r = 0;
				cfg.head_color._g = 0;
				cfg.head_color._b = 255;
				cfg.head_color._a = 255;

				_its_component_layer->Initialize<NULLTYPE>(cfg);
			}
		};
	}
}