#pragma once

#include "Antares_Includes.h"
#include "User_Space\Zone_Prototype.h"

namespace Zone_Components
{
	namespace Types
	{
		enum COLUMN_COLOR
		{
			RED_COLUMN, GREEN_COLUMN, BLUE_COLUMN, GRAY_COLUMN
		};
		
		implementation struct Quad : public Polaris_Component<APPEND_CHILD(Quad),MasterType,NULLTYPE>
		{
			Quad()
			{
				color._a=200;
				color._b=0;
				color._r=0;
				color._g=0;
			}
			True_Color_RGBA<NULLTYPE> color;
			Point_3D<MasterType> a;
			Point_3D<MasterType> b;
			Point_3D<MasterType> c;
			Point_3D<MasterType> d;
		};
#pragma pack(push,1)
		implementation struct Column : public Polaris_Component<APPEND_CHILD(Column),MasterType,NULLTYPE>
		{
			Column(Point_3D<MasterType> center_point, int width, int height, COLUMN_COLOR primary_color)
			{	
				quads=new Quad<MasterType>[5];
				num_primitives=5;

				if (primary_color == GREEN_COLUMN || primary_color == GRAY_COLUMN)
				{
					quads[0].color._g = 255;
					quads[1].color._g = 220;
					quads[2].color._g = 180;
					quads[3].color._g = 150;
					quads[4].color._g = 255;
				}
				if (primary_color == RED_COLUMN || primary_color == GRAY_COLUMN)
				{
					quads[0].color._r = 255;
					quads[1].color._r = 220;
					quads[2].color._r = 180;
					quads[3].color._r = 150;
					quads[4].color._r = 255;
				}
				if (primary_color == BLUE_COLUMN || primary_color == GRAY_COLUMN)
				{
					quads[0].color._b = 255;
					quads[1].color._b = 220;
					quads[2].color._b = 180;
					quads[3].color._b = 150;
					quads[4].color._b = 255;
				}

				// east side
				quads[0].a._x = center_point._x + width/4;
				quads[0].a._y = center_point._y - width/2;
				quads[0].a._z = center_point._z + 1;
				quads[0].b._x = center_point._x + width/4;
				quads[0].b._y = center_point._y + width/2;
				quads[0].b._z = center_point._z + 1;
				quads[0].c._x = center_point._x + width/4;
				quads[0].c._y = center_point._y + width/2;
				quads[0].c._z = center_point._z + height;
				quads[0].d._x = center_point._x + width/4;
				quads[0].d._y = center_point._y - width/2;
				quads[0].d._z = center_point._z + height;
				
				//west side			
				quads[1].a._x = center_point._x - width/4;
				quads[1].a._y = center_point._y - width/2;
				quads[1].a._z = center_point._z + 1;
				quads[1].b._x = center_point._x - width/4;
				quads[1].b._y = center_point._y + width/2;
				quads[1].b._z = center_point._z + 1;
				quads[1].c._x = center_point._x - width/4;
				quads[1].c._y = center_point._y + width/2;
				quads[1].c._z = center_point._z + height;
				quads[1].d._x = center_point._x - width/4;
				quads[1].d._y = center_point._y - width/2;
				quads[1].d._z = center_point._z + height;
				
				//north side
				quads[2].a._x = center_point._x + width/4;
				quads[2].a._y = center_point._y + width/2;
				quads[2].a._z = center_point._z + 1;
				quads[2].b._x = center_point._x - width/4;
				quads[2].b._y = center_point._y + width/2;
				quads[2].b._z = center_point._z + 1;
				quads[2].c._x = center_point._x - width/4;
				quads[2].c._y = center_point._y + width/2;
				quads[2].c._z = center_point._z + height;
				quads[2].d._x = center_point._x + width/4;
				quads[2].d._y = center_point._y + width/2;
				quads[2].d._z = center_point._z + height;
				
				//south side
				quads[3].a._x = center_point._x + width/4;
				quads[3].a._y = center_point._y - width/2;
				quads[3].a._z = center_point._z + 1;
				quads[3].b._x = center_point._x - width/4;
				quads[3].b._y = center_point._y - width/2;
				quads[3].b._z = center_point._z + 1;
				quads[3].c._x = center_point._x - width/4;
				quads[3].c._y = center_point._y - width/2;
				quads[3].c._z = center_point._z + height;
				quads[3].d._x = center_point._x + width/4;
				quads[3].d._y = center_point._y - width/2;
				quads[3].d._z = center_point._z + height;
				
				//top side
				quads[4].a._x = center_point._x + width/4;
				quads[4].a._y = center_point._y - width/2;
				quads[4].a._z = center_point._z + height;
				quads[4].b._x = center_point._x - width/4;
				quads[4].b._y = center_point._y - width/2;
				quads[4].b._z = center_point._z + height;
				quads[4].c._x = center_point._x - width/4;
				quads[4].c._y = center_point._y + width/2;
				quads[4].c._z = center_point._z + height;
				quads[4].d._x = center_point._x + width/4;
				quads[4].d._y = center_point._y + width/2;
				quads[4].d._z = center_point._z + height;
			}
			feature_implementation void Push_To_Layer(TargetType Layer_Reference)
			{
				//Layer_Reference->Push_Element<Regular_Element>(&num_primitives);
				Layer_Reference->Push_Element<Regular_Element>(this);

				//Layer_Reference->Push_Element<Regular_Element>(&east);
				//Layer_Reference->Push_Element<Regular_Element>(&west);
				//Layer_Reference->Push_Element<Regular_Element>(&north);
				//Layer_Reference->Push_Element<Regular_Element>(&south);
				//Layer_Reference->Push_Element<Regular_Element>(&top);
			}

			void* ptr;
			int num_primitives;
			Quad<MasterType>* quads;

			//Quad<MasterType> east;
			//Quad<MasterType> west;
			//Quad<MasterType> north;
			//Quad<MasterType> south;
			//Quad<MasterType> top;
		};
#pragma pack(pop)
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Graphical_Zone_Group_Implementation : public Polaris_Component<APPEND_CHILD(Graphical_Zone_Group_Implementation),MasterType,Data_Object,ParentType>
		{	
			feature_implementation void accept_zone_information(Point_3D<MasterType>& coordinates, void* ptr, typename TargetType::Param2Type productions, typename TargetType::Param2Type attractions)
			{
				int width = 300;
				int height_prod = productions;
				int height_att = attractions;

				Point_3D<MasterType> origin_col_center, destination_col_center;

				origin_col_center._x = coordinates._x - width*0.25;
				origin_col_center._y = coordinates._y ;
				destination_col_center._x = coordinates._x + width*0.25;
				destination_col_center._y = origin_col_center._y;
				_canvas->Scale_Coordinates<Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(origin_col_center);
				_canvas->Scale_Coordinates<Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(destination_col_center);

				// construct and push to productions column
				Types::Column<MasterType> origin_column = Types::Column<MasterType>(origin_col_center,width,height_prod, Types::GREEN_COLUMN);
				origin_column.ptr = ptr;
				origin_column.Push_To_Layer<ComponentType,CallerType>(_zone_centroids);

				// construct and push to attractions column
				Types::Column<MasterType> destination_column = Types::Column<MasterType>(destination_col_center,width,height_att, Types::BLUE_COLUMN);
				destination_column.ptr = ptr;
				destination_column.Push_To_Layer<ComponentType,CallerType>(_zone_centroids);
			}

			feature_implementation void configure_zones_layer()
			{
				// configure vehicle layer
				cout << "configuring zone layer";
				_zone_centroids=_canvas->Allocate_New_Layer< Target_Type< NULLTYPE,Antares_Layer<type_of(zone_centroids),ComponentType>*, string& > >(string("Zones"));
				Antares_Layer_Configuration cfg;
				cfg.Configure_Static_Quads(True_Color_RGBA<NULLTYPE>(0,255,100,255),10);
				cfg.attributes_schema = string("ID,Productions,Attractions");
				cfg.dynamic_data = true;
				cfg.storage_period = 300;
				cfg.target_sub_iteration = Types::ZONE_UPDATE_SUBITERATION+1;
				cfg.storage_offset = 60/*_iteration*/;
				cfg.storage_size = 3;
				cfg.primitive_color = true;
				cfg.data_stride = sizeof(void*);

				typedef bool (*attributes_callback_type)(void*,string&);

				cfg.attributes_callback = (attributes_callback_type)&Graphical_Zone_Implementation<MasterType>::fetch_attributes;
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
			typedef Zone_Prototype<typename MasterType::zone_type> this_itf;
			typedef Zone_Components::Prototypes::Zone_Prototype<BaseType,BaseType> base_itf;
			define_container_and_value_interface(activity_locations_interface,activity_location_interface,typename BaseType::type_of(origin_activity_locations), Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,NULLTYPE);
			
			feature_implementation void Initialize()
			{
				//cout << endl << "Graphical Zone Initialized.";
				this_itf* pthis = (this_itf*)this;
				pthis->update_increment<Time_Minutes>(5);

				load_event(ComponentType,Default_Zone_Conditional,Default_Zone_Event,60,Types::ZONE_UPDATE_SUBITERATION,NULLTYPE);
			}

			declare_feature_conditional(Default_Zone_Conditional)
			{
				//cout << endl << "Graphical Zone conditional hit.";
				this_itf* pthis = (this_itf*)_this;

				response.result=true;
				response.next._iteration=Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(pthis->update_increment<Simulation_Timestep_Increment>());
				response.next._sub_iteration=Types::ZONE_UPDATE_SUBITERATION;
			}
			declare_feature_event(Default_Zone_Event)
			{
				//cout << endl << "Graphical Zone event fired.";
				this_itf* pthis = (this_itf*)_this;
				//cout << endl << "zone event pushed. enter a key: ";
				//char a;
				//cin >> a;
				pthis->Push_To_Zone_Display<NULLTYPE>();
			}

			member_data_component(typename Basic_Units::Implementations::Time_Implementation<MasterType>,_Update_Increment,none,none);
			member_component_feature(update_increment, _Update_Increment, Value, Basic_Units::Prototypes::Time_Prototype);
			feature_implementation void Push_To_Zone_Display()
			{
				//cout << endl << "at zone push_to_display";

				base_itf* base_this = (base_itf*)this;
				Point_3D<MasterType> coordinate;
			
				coordinate._x=base_this->X<float>();
				coordinate._y=base_this->Y<float>();
				coordinate._z=0;
				
				//int height = base_this->origin_activity_locations<activity_locations_interface*>()->size();
				int prod_height = base_this->production_count<int>();
				int att_height = base_this->attraction_count<int>();

				_graphical_zone_group->push_zone_information<Target_Type<NULLTYPE,NULLTYPE,Point_3D<MasterType>&, int> >(coordinate, this, prod_height*100, att_height*100);
			}

			static member_prototype(Graphical_Zone_Group,graphical_zone_group,typename MasterType::graphical_zone_group_type,none,none);

			static bool fetch_attributes(Graphical_Zone_Implementation* _this,string& bucket)
			{
				this_itf* this_ptr = (this_itf*)_this;
				stringstream s;

				s << this_ptr->uuid<int>() << ",";
				s << this_ptr->production_count<int>() << ",";
				s << this_ptr->attraction_count<int>();

				bucket=s.str();

				return true;
			}

		};
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Zone_Components::Prototypes::Graphical_Zone_Group<typename MasterType::type_of(graphical_zone_group),Graphical_Zone_Implementation<MasterType,ParentType,InheritanceList>>* Graphical_Zone_Implementation<MasterType,ParentType,InheritanceList>::_graphical_zone_group;
	}
}