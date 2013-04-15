//*********************************************************
//	Antares_Depot_Implementation.h - Graphical Depot
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Depot_Prototype.h"

namespace Depot_Components
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
		struct Antares_Depot : public InheritanceTemplate<MasterType,ParentType,APPEND_CHILD(Antares_Depot)>
		{
			typedef typename InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Depot)>::ComponentType ComponentType;
			
			feature_implementation static void Initialize_Type(const vector<shared_ptr<polaris::io::Component_Key>>& keys,string& name)
			{
				InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Depot)>::Initialize_Type<ComponentType,CallerType,NT>(keys);

				_its_component_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer),ComponentType>*, string& > >(name);

				Antares_Layer_Configuration cfg;
				cfg.Configure_Lines();
				cfg.grouped=true;
				//cfg.group_color=true;
				cfg.head_size_value=4;
				
				for(vector<string>::iterator itr=ComponentType::_component_keys.begin();itr!=ComponentType::_component_keys.end();itr++)
				{
					cfg.attributes_schema.push_back(*itr);
				}

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

			feature_implementation void Initialize(vector<int>& db_covered_links)
			{
				InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Depot)>::Initialize<ComponentType,CallerType,NT>(db_covered_links);

				vector<Link_Prototype<typename type_of(MasterType::link),ComponentType>*>::iterator itr;

				Link_Line_Segment* segments = new Link_Line_Segment[ _covered_links.size() ];
					
				Link_Line_Group group;
				group.num_primitives = _covered_links.size();
				group.segments = segments;

				Link_Line_Segment* current_segment = group.segments;

				for(itr = _covered_links.begin(); itr != _covered_links.end(); itr++)
				{
					Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)(*itr);
						
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

					++current_segment;
				}
					
				_its_component_layer->Push_Element<Regular_Element>(&group);

				delete[] segments;
			}

			static member_prototype(Antares_Layer,its_component_layer,typename type_of(MasterType::antares_layer),none,none);
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList,template<class,class,class> class InheritanceTemplate>
		Antares_Layer<typename type_of(MasterType::antares_layer),typename Antares_Depot<MasterType,ParentType,InheritanceList,InheritanceTemplate>::ComponentType>* Antares_Depot<MasterType,ParentType,InheritanceList,InheritanceTemplate>::_its_component_layer;

		implementation struct Antares_Tow_Truck_Depot : public Antares_Depot<MasterType,NT,APPEND_CHILD(Antares_Tow_Truck_Depot),Tow_Truck_Depot>
		{
			feature_implementation static void Initialize_Type(const vector<shared_ptr<polaris::io::Component_Key>>& keys)
			{
				Antares_Depot::Initialize_Type<ComponentType,CallerType,NT>(keys,string("Tow Truck Depots"));
			}
		};
	}
}