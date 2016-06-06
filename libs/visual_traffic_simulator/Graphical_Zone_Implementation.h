#pragma once

#include "Dependencies.h"
#include "Traffic_Simulator\Zone_Implementation.h"

namespace Zone_Components
{
	namespace Types
	{
		enum COLUMN_COLOR
		{
			RED_COLUMN, GREEN_COLUMN, BLUE_COLUMN, GRAY_COLUMN
		};

#pragma pack(push,1)
		implementation struct Quad : public Polaris_Component<MasterType,INHERIT(Quad),NULLTYPE>
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
#pragma pack(pop)

#pragma pack(push,1)
		implementation struct Column : public Polaris_Component<MasterType,INHERIT(Column),NULLTYPE>
		{
			Column(Point_3D<MasterType> center_point, int width, int height, COLUMN_COLOR primary_color)
			{	
				quads=new Quad<MasterType>[5];
				num_primitives=5;

				//if (primary_color == GREEN_COLUMN || primary_color == GRAY_COLUMN)
				//{
				//	quads[0].color._g = 255;
				//	quads[1].color._g = 220;
				//	quads[2].color._g = 180;
				//	quads[3].color._g = 150;
				//	quads[4].color._g = 255;
				//}
				//if (primary_color == RED_COLUMN || primary_color == GRAY_COLUMN)
				//{
				//	quads[0].color._r = 255;
				//	quads[1].color._r = 220;
				//	quads[2].color._r = 180;
				//	quads[3].color._r = 150;
				//	quads[4].color._r = 255;
				//}
				//if (primary_color == BLUE_COLUMN || primary_color == GRAY_COLUMN)
				//{
				//	quads[0].color._b = 255;
				//	quads[1].color._b = 220;
				//	quads[2].color._b = 180;
				//	quads[3].color._b = 150;
				//	quads[4].color._b = 255;
				//}
				
				if (primary_color == GREEN_COLUMN || primary_color == GRAY_COLUMN)
				{
					quads[0].color._g = 255;
					quads[1].color._g = 255;
					quads[2].color._g = 255;
					quads[3].color._g = 255;
					quads[4].color._g = 255;
				}
				if (primary_color == RED_COLUMN || primary_color == GRAY_COLUMN)
				{
					quads[0].color._r = 255;
					quads[1].color._r = 255;
					quads[2].color._r = 255;
					quads[3].color._r = 255;
					quads[4].color._r = 255;
				}
				if (primary_color == BLUE_COLUMN || primary_color == GRAY_COLUMN)
				{
					quads[0].color._b = 255;
					quads[1].color._b = 255;
					quads[2].color._b = 255;
					quads[3].color._b = 255;
					quads[4].color._b = 255;
				}

				quads[0].color._a = 220;
				quads[1].color._a = 220;
				quads[2].color._a = 220;
				quads[3].color._a = 220;
				quads[4].color._a = 220;

				// east side
				quads[0].a._x = center_point._x + width/4;
				quads[0].a._y = center_point._y - width/2;
				quads[0].a._z = 1;
				quads[0].b._x = center_point._x + width/4;
				quads[0].b._y = center_point._y + width/2;
				quads[0].b._z = 1;
				quads[0].c._x = center_point._x + width/4;
				quads[0].c._y = center_point._y + width/2;
				quads[0].c._z = height;
				quads[0].d._x = center_point._x + width/4;
				quads[0].d._y = center_point._y - width/2;
				quads[0].d._z = height;
				
				//west side			
				quads[1].a._x = center_point._x - width/4;
				quads[1].a._y = center_point._y - width/2;
				quads[1].a._z = 1;
				quads[1].d._x = center_point._x - width/4;
				quads[1].d._y = center_point._y + width/2;
				quads[1].d._z = 1;
				quads[1].c._x = center_point._x - width/4;
				quads[1].c._y = center_point._y + width/2;
				quads[1].c._z = height;
				quads[1].b._x = center_point._x - width/4;
				quads[1].b._y = center_point._y - width/2;
				quads[1].b._z = height;
				
				//north side
				quads[2].a._x = center_point._x + width/4;
				quads[2].a._y = center_point._y + width/2;
				quads[2].a._z = 1;
				quads[2].b._x = center_point._x - width/4;
				quads[2].b._y = center_point._y + width/2;
				quads[2].b._z = 1;
				quads[2].c._x = center_point._x - width/4;
				quads[2].c._y = center_point._y + width/2;
				quads[2].c._z = height;
				quads[2].d._x = center_point._x + width/4;
				quads[2].d._y = center_point._y + width/2;
				quads[2].d._z = height;
				
				//south side
				quads[3].a._x = center_point._x + width/4;
				quads[3].a._y = center_point._y - width/2;
				quads[3].a._z = 1;
				quads[3].d._x = center_point._x - width/4;
				quads[3].d._y = center_point._y - width/2;
				quads[3].d._z = 1;
				quads[3].c._x = center_point._x - width/4;
				quads[3].c._y = center_point._y - width/2;
				quads[3].c._z = height;
				quads[3].b._x = center_point._x + width/4;
				quads[3].b._y = center_point._y - width/2;
				quads[3].b._z = height;
				
				//top side
				quads[4].a._x = center_point._x + width/4;
				quads[4].a._y = center_point._y - width/2;
				quads[4].a._z = height;
				quads[4].d._x = center_point._x - width/4;
				quads[4].d._y = center_point._y - width/2;
				quads[4].d._z = height;
				quads[4].c._x = center_point._x - width/4;
				quads[4].c._y = center_point._y + width/2;
				quads[4].c._z = height;
				quads[4].b._x = center_point._x + width/4;
				quads[4].b._y = center_point._y + width/2;
				quads[4].b._z = height;
			}
			template<typename TargetType> void Push_To_Layer(TargetType Layer_Reference)
			{
				Layer_Reference->Push_Element<Regular_Element>((void*)this);
			}

			void* ptr;
			int num_primitives;
			Quad<MasterType>* quads;
		};
#pragma pack(pop)
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Graphical_Zone_Group_Implementation : public Polaris_Component<MasterType,INHERIT(Graphical_Zone_Group_Implementation),Data_Object>
		{
			template<typename CoordType, typename TargetType> void accept_zone_information(CoordType coordinates, void* ptr, typename TargetType productions, typename TargetType attractions)
			{
				int width = 300;
				int height_prod = productions;
				int height_att = attractions;

				Point_3D<MasterType> origin_col_center, destination_col_center;

				origin_col_center._x = coordinates._x - width*0.25;
				origin_col_center._y = coordinates._y ;
				destination_col_center._x = coordinates._x + width*0.25;
				destination_col_center._y = origin_col_center._y;

				Scale_Coordinates<MT>(origin_col_center);
				Scale_Coordinates<MT>(destination_col_center);

				// construct and push to productions column
				Types::Column<MasterType> origin_column = Types::Column<MasterType>(origin_col_center,width,height_prod, Types::GREEN_COLUMN);
				origin_column.ptr = ptr;
				origin_column.Push_To_Layer(_zone_centroids);

				// construct and push to attractions column
				Types::Column<MasterType> destination_column = Types::Column<MasterType>(destination_col_center,width,height_att, Types::BLUE_COLUMN);
				destination_column.ptr = ptr;
				destination_column.Push_To_Layer(_zone_centroids);
			}

			template<typename TargetType> void configure_zones_layer()
			{
				// configure vehicle layer
				cout << "configuring zone layer";

				_zone_centroids=Allocate_New_Layer<MT>(string("Zones"));
				Antares_Layer_Configuration cfg;
				cfg.Configure_Static_Quads(True_Color_RGBA<NULLTYPE>(0,255,100,255),10);

				//cfg.attributes_schema.push_back("ID");
				//cfg.attributes_schema.push_back("Productions");
				//cfg.attributes_schema.push_back("Attractions");
				//cfg.attributes_schema.push_back("Population");
				//cfg.attributes_schema.push_back("Available");

				cfg.dynamic_data = true;
				cfg.storage_period = 300;
				cfg.target_sub_iteration = Types::ZONE_UPDATE_SUBITERATION+1;
				cfg.storage_offset = 60/*iteration()*/;
				cfg.storage_size = 4;
				cfg.primitive_color = true;
				cfg.primitive_normal = true;
				cfg.grouped = true;
				cfg.selection_callback = (selection_callback_type)&Graphical_Zone_Implementation<MasterType>::on_select;

				//cfg.selection_callback = (selection_callback_type)&Graphical_Zone_Implementation<MasterType>::fetch_attributes;
				//cfg.submission_callback = (selection_callback_type)&Graphical_Zone_Implementation<MasterType>::submit_attributes;
				_zone_centroids->Initialize<NULLTYPE>(cfg);
			}

			m_prototype(Antares_Layer,typename MasterType::antares_layer_type,zone_centroids, NONE, NONE);
		};


		implementation struct Graphical_Zone_Implementation : public Zone_Components::Implementations::Zone_Implementation<MasterType,INHERIT(Graphical_Zone_Implementation)>
		{
			typedef Zone_Components::Implementations::Zone_Implementation<MasterType,INHERIT(Graphical_Zone_Implementation)> BaseType;
			typedef Zone<typename MasterType::zone_type> this_itf;
			typedef Zone_Components::Prototypes::Zone<BaseType> base_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename BaseType::type_of(origin_activity_locations)::value_type>::type> activity_location_interface;
			typedef  Random_Access_Sequence<typename BaseType::type_of(origin_activity_locations),activity_location_interface*> activity_locations_interface;

			
			static void Default_Zone_Conditional(ComponentType* _this,Event_Response& response)
			{
				this_itf* pthis = (this_itf*)_this;
				response.next._iteration=Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(pthis->update_increment<Simulation_Timestep_Increment>());
				//response.next._iteration = iteration() + pthis->update_increment<Simulation_Timestep_Increment>();
				response.next._sub_iteration=Types::ZONE_UPDATE_SUBITERATION;

				_this->Default_Zone_Event();
			}
			void Default_Zone_Event()
			{
				this->Push_To_Zone_Display<NULLTYPE>();
				this->reset_counters<NULLTYPE>();
			}		

			template<typename TargetType> void Initialize()
			{
				base_itf* base_this = (base_itf*)this;
				base_this->zone_is_available<bool>(true);
				this_itf* pthis = (this_itf*)this;
				pthis->update_increment<Time_Minutes>(5);

				((ComponentType*)this)->Load_Event<ComponentType>(&Default_Zone_Conditional,60,Types::ZONE_UPDATE_SUBITERATION);	
			}
			template<typename TargetType> void Push_To_Zone_Display()
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
				_production_count_buffer = prod_height;
				_attraction_count_buffer = att_height;

				int height_scale = 50;

				_graphical_zone_group->accept_zone_information<Point_3D<MasterType>, int>(coordinate, this, prod_height*height_scale, att_height*height_scale);
			}
			
			static void on_select(const std::list<void*>& removed,const std::list<void*>& added,const std::list<void*>& selected,std::vector<pair<string,string>>& bucket)
			{

			}

			static bool fetch_attributes(Graphical_Zone_Implementation* _this,std::vector<string>& bucket)
			{
				this_itf* this_ptr = (this_itf*)_this;
				stringstream s;
				
				s << this_ptr->uuid<int>();
				bucket.push_back(s.str());
				s.str("");
				s << _this->_production_count_buffer;
				bucket.push_back(s.str());
				s.str("");
				s << _this->_attraction_count_buffer;
				bucket.push_back(s.str());
				s.str("");
				s << this_ptr->population<int>();
				bucket.push_back(s.str());
				s.str("");
				s << boolalpha << this_ptr->zone_is_available<bool>();	
				bucket.push_back(s.str());
				s.str("");

				return true;
			}
			static bool submit_attributes(Graphical_Zone_Implementation* _this,std::vector<string>& bucket)
			{
				this_itf* this_ptr = (this_itf*)_this;

				std::vector<string>::iterator itr;


				if(bucket[4] == "True" ||bucket[4] == "true" || bucket[4] == "t" || bucket[4] == "T" )
				{
					this_ptr->zone_is_available<bool>(true);
					return true;
				}
				else if (bucket[4] == "False" || bucket[4] == "false" || bucket[4] == "f" || bucket[4] == "F" )
				{
					this_ptr->zone_is_available<bool>(false);
					return true;
				}
				else
				{
					return false;
				}
			}

			m_data(typename Basic_Units::Implementations::Time_Implementation<MasterType>,_Update_Increment, NONE, NONE);
			member_component_feature(update_increment, _Update_Increment, Value, Basic_Units::Prototypes::Time);
			static m_prototype(Graphical_Zone_Group,typename MasterType::graphical_zone_group_type,graphical_zone_group, NONE, NONE);
			m_data(int, production_count_buffer, NONE, NONE);
			m_data(int, attraction_count_buffer, NONE, NONE);
		};

		template<typename MasterType,typename InheritanceList>
		Zone_Components::Prototypes::Graphical_Zone_Group<typename MasterType::graphical_zone_group_type>* Graphical_Zone_Implementation<MasterType,InheritanceList>::_graphical_zone_group;
	}
}
