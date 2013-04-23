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

				cfg.head_color._r = 0;
				cfg.head_color._g = 0;
				cfg.head_color._b = 255;
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
				int num_primitives;
				Link_Line_Segment* segments;
			};
#pragma pack(pop)

			feature_implementation void Initialize(TargetType configuration)
			{
				InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Link_Control)>::Initialize<ComponentType,CallerType,TargetType>(configuration);

				vector<Link_Prototype<typename type_of(MasterType::link),ComponentType>*>::iterator itr;

				Link_Line_Segment segment;

				Link_Line_Group group;
				group.num_primitives = 1;
				group.segments = &segment;

				Link_Line_Segment* current_segment = group.segments;

				Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)_covered_links.front();
				
				Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* intersection;
				
				intersection = link->upstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();
				
				current_segment->a._x = intersection->x_position<float>();
				current_segment->a._y = intersection->y_position<float>();
				current_segment->a._z = 2;

				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->a );

				intersection = link->downstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();

				current_segment->b._x = intersection->x_position<float>();
				current_segment->b._y = intersection->y_position<float>();
				current_segment->b._z = 2;

				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->b );
				
				_its_component_layer->Push_Element<Regular_Element>(&group);
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