#pragma once

#include "Antares_Includes.h"
#include "User_Space\Zone_Prototype.h"

namespace Zone_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Graphical_Zone_Group_Implementation : public Polaris_Component<APPEND_CHILD(Graphical_Zone_Group_Implementation),MasterType,Data_Object,ParentType>
		{		
			feature_implementation void accept_zone_coordinates(Point_3D<MasterType>& coordinates)
			{
				//cout << coordinates._x << ",";
				//cout << coordinates._y;

				coordinates._x += _input_offset._x;
				coordinates._y += _input_offset._y;
				coordinates._z += 1;

				//cout << " transformed to " << coordinates._x << ",";
				//cout << coordinates._y<<endl;

				_zone_centroids->Push_Element<Regular_Element>(&coordinates,sizeof(Point_3D<MasterType>));
			}

			feature_implementation void configure_zones_layer()
			{
				// configure vehicle layer
				_zone_centroids=_canvas->Allocate_New_Layer< Target_Type< NULLTYPE,Antares_Layer<type_of(zone_centroids),ComponentType>*, string& > >(string("Zones"));
				Antares_Layer_Configuration cfg;
				cfg.Configure_Static_Points(True_Color_RGBA<NULLTYPE>(0,255,100,255),10);
				_zone_centroids->Initialize<NULLTYPE>(cfg);
			}

			member_data(Point_2D<MasterType>, input_offset,none,none);
			member_data(Rectangle_XY<MasterType>, network_bounds,none,none);
			
			member_prototype(Antares_Layer,zone_centroids,typename type_of(MasterType::antares_layer),none,none);

			member_prototype(Canvas,canvas,typename MasterType::type_of(canvas),none,none);
		};


		implementation struct Graphical_Zone_Implementation : public Zone_Components::Implementations::Polaris_Zone_Implementation<MasterType,ParentType,APPEND_CHILD(Graphical_Zone_Implementation)>
		{
			typedef Zone_Components::Implementations::Polaris_Zone_Implementation<MasterType,ParentType,APPEND_CHILD(Graphical_Zone_Implementation)> BaseType;
			typedef Zone_Components::Prototypes::Zone_Prototype<BaseType,BaseType> base_itf;
			
			feature_implementation void Push_To_Zone_Display()
			{
				//cout << endl << "at zone push_to_display";

				base_itf* base_this = (base_itf*)this;
				Point_3D<MasterType> coordinate;
			
				coordinate._x=base_this->X<float>();
				coordinate._y=base_this->Y<float>();
				coordinate._z=0;

				_graphical_zone_group->push_zone_coordinates<Point_3D<MasterType>& >(coordinate);
			}

			member_prototype(static Graphical_Zone_Group,graphical_zone_group,typename MasterType::graphical_zone_group_type,none,none);

			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);

			member_container(vector<typename MasterType::activity_location_type*>, origin_activity_locations, none, none);

			member_container(vector<typename MasterType::link_type*>, destination_activity_locations, none, none);

			member_container(vector<float>, origin_activity_location_choice_cdfs, none, none);

			member_container(vector<float>, destination_activity_location_choice_cdfs, none, none);

		};
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Zone_Components::Prototypes::Graphical_Zone_Group<typename MasterType::type_of(graphical_zone_group),Graphical_Zone_Implementation<MasterType,ParentType,InheritanceList>>* Graphical_Zone_Implementation<MasterType,ParentType,InheritanceList>::_graphical_zone_group;
	}

}