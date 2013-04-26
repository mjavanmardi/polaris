//*********************************************************
//	Graphical_Network_Implementation.h - Graphical Link Variant
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Polaris_Link_Implementation.h"

namespace Link_Components
{
	namespace Types
	{
		enum BAR_COLOR
		{
			RED_BAR, GREEN_BAR, BLUE_BAR
		};
	}
	
	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		enum BARS_IN_COLUMN
		{
			FRONT_BAR=0, REAR_BAR, LEFT_SIDE_BAR, RIGHT_SIDE_BAR, TOP_BAR
		};
#pragma pack(push,1)
		struct Link_MOE_Bar
		{
			True_Color_RGBA<NT> color;
			Point_3D<NT> a;
			Point_3D<NT> b;
			Point_3D<NT> c;
			Point_3D<NT> d;
		};
#pragma pack(pop)
#pragma pack(push,1)
		struct Link_MOE_Column
		{
			Link_MOE_Column()
			{
				bars=new Link_MOE_Bar[5];
				num_primitives=5;
			}
			int num_primitives;
			Link_MOE_Bar* bars;
		};
#pragma pack(pop)
		template<typename MasterType>
		struct Link_Line
		{
			void* data;
			True_Color_RGBA<NT> color;
			Point_3D<MasterType> up_node;
			Point_3D<MasterType> down_node;
		};
		implementation struct Antares_Link_Implementation:public Polaris_Link_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Link_Implementation)>
		{
			// reference MOE values to determine height of bars
			static Link_MOE_Data visualization_reference;			
			static int base_height;
			static float _vehicle_length; // average length of a vehicle in feet
			static float _link_shift;
			Link_MOE_Column column;
			Link_MOE_Bar queue_length_box;
			feature_implementation void Initialize()
			{
				((Polaris_Link_Implementation<MasterType,ParentType,APPEND_CHILD(Polaris_Link_Implementation)>*)this)->Initialize<ComponentType,CallerType,TargetType>();
				// set bar attributes that are common for all MOEs.
				initialize_column();
			}

			void configure_queue_length_box()
			{
				define_component_interface(_Intersection_Interface, type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				Point_3D<MasterType> u_node;
				Point_3D<MasterType> d_node;
				
				float column_depth_down = 100.0;
				float column_depth_up = 100.0;

				float u_x = _displayed_line.up_node._x;
				float u_y = _displayed_line.up_node._y;
				float d_x = _displayed_line.down_node._x;
				float d_y = _displayed_line.down_node._y;
				float distance = sqrt((u_x - d_x) * (u_x - d_x) + (u_y - d_y) * (u_y - d_y));
				float sin_alpha = (d_y - u_y) / distance;
				float cos_alpha = (d_x - u_x) / distance;

				float physical_queue_length = realtime_link_moe_data.link_queue_length * _vehicle_length;

				d_node._x = d_x ;
				d_node._y = d_y;
				d_node._z = 0.0;
				//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(d_node);

				u_node._x = d_x + physical_queue_length * cos_alpha;
				u_node._y = d_y + physical_queue_length * sin_alpha;
				u_node._z = 0.0;
				//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(u_node);
				
				queue_length_box.a._x = d_node._x;
				queue_length_box.a._y = d_node._y;
				queue_length_box.a._z = 1;
				queue_length_box.b._x = u_node._x;
				queue_length_box.b._y = u_node._y;
				queue_length_box.b._z = 1;

				queue_length_box.c._x = u_node._x - column_depth_up * sin_alpha;
				queue_length_box.c._y = u_node._y + column_depth_up * cos_alpha;
				queue_length_box.c._z = 1;

				queue_length_box.d._x = d_node._x - column_depth_down * sin_alpha;
				queue_length_box.d._y = d_node._y + column_depth_down * cos_alpha;
				queue_length_box.d._z = 1;
			}

			void initialize_column()
			{
				define_component_interface(_Intersection_Interface, type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				Point_3D<MasterType> u_node;
				Point_3D<MasterType> d_node;
			
				float u_x = _displayed_line.up_node._x;
				float u_y = _displayed_line.up_node._y;
				float d_x = _displayed_line.down_node._x;
				float d_y = _displayed_line.down_node._y;
				float c_x = (u_x + d_x) / 2.0f;
				float c_y = (u_y + d_y) / 2.0f;
				float column_width = 100.0;
				float column_depth = 30.0;
				float distance = sqrt((u_x - d_x) * (u_x - d_x) + (u_y - d_y) * (u_y - d_y));
				float sin_alpha = (d_y - u_y) / distance;
				float cos_alpha = (d_x - u_x) / distance;

				u_node._x = c_x - (c_x - u_x) * (column_width / (0.5 * _length));
				u_node._y = c_y - (c_y - u_y) * (column_width / (0.5 * _length));
				u_node._z=0;
				//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(u_node);

				d_node._x = c_x - (c_x - d_x) * (column_width / (0.5 * _length));
				d_node._y = c_y - (c_y - d_y) * (column_width / (0.5 * _length));
				d_node._z=0;
				//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(d_node);

				// configure front bar
				column.bars[FRONT_BAR].a._x = u_node._x + column_depth / 2.0 * sin_alpha;
				column.bars[FRONT_BAR].a._y = u_node._y - column_depth / 2.0 * cos_alpha;
				column.bars[FRONT_BAR].a._z = 1;

				column.bars[FRONT_BAR].b._x = d_node._x + column_depth / 2.0 * sin_alpha;
				column.bars[FRONT_BAR].b._y = d_node._y - column_depth / 2.0 * cos_alpha;
				column.bars[FRONT_BAR].b._z = 1;

				column.bars[FRONT_BAR].c._x = column.bars[FRONT_BAR].b._x;
				column.bars[FRONT_BAR].c._y = column.bars[FRONT_BAR].b._y;
				
				column.bars[FRONT_BAR].d._x = column.bars[FRONT_BAR].a._x;
				column.bars[FRONT_BAR].d._y = column.bars[FRONT_BAR].a._y;

				// configure rear bar
				column.bars[REAR_BAR].a._x = u_node._x - column_depth / 2.0 * sin_alpha;
				column.bars[REAR_BAR].a._y = u_node._y + column_depth / 2.0 * cos_alpha;
				column.bars[REAR_BAR].a._z = 1;

				column.bars[REAR_BAR].b._x = d_node._x - column_depth / 2.0 * sin_alpha;
				column.bars[REAR_BAR].b._y = d_node._y + column_depth / 2.0 * cos_alpha;
				column.bars[REAR_BAR].b._z = 1;

				column.bars[REAR_BAR].c._x = column.bars[REAR_BAR].b._x;
				column.bars[REAR_BAR].c._y = column.bars[REAR_BAR].b._y;
				
				column.bars[REAR_BAR].d._x = column.bars[REAR_BAR].a._x;
				column.bars[REAR_BAR].d._y = column.bars[REAR_BAR].a._y;

				// configure left side bar
				column.bars[LEFT_SIDE_BAR].a = column.bars[FRONT_BAR].a;

				column.bars[LEFT_SIDE_BAR].b = column.bars[REAR_BAR].a;

				column.bars[LEFT_SIDE_BAR].c._x = column.bars[LEFT_SIDE_BAR].b._x;
				column.bars[LEFT_SIDE_BAR].c._y = column.bars[LEFT_SIDE_BAR].b._y;
				
				column.bars[LEFT_SIDE_BAR].d._x = column.bars[LEFT_SIDE_BAR].a._x;
				column.bars[LEFT_SIDE_BAR].d._y = column.bars[LEFT_SIDE_BAR].a._y;
				
				// configure right side bar
				column.bars[RIGHT_SIDE_BAR].a = column.bars[FRONT_BAR].b;

				column.bars[RIGHT_SIDE_BAR].b = column.bars[REAR_BAR].b;

				column.bars[RIGHT_SIDE_BAR].c._x = column.bars[RIGHT_SIDE_BAR].b._x;
				column.bars[RIGHT_SIDE_BAR].c._y = column.bars[RIGHT_SIDE_BAR].b._y;
				
				column.bars[RIGHT_SIDE_BAR].d._x = column.bars[RIGHT_SIDE_BAR].a._x;
				column.bars[RIGHT_SIDE_BAR].d._y = column.bars[RIGHT_SIDE_BAR].a._y;

				// configure top bar
				column.bars[TOP_BAR].a = column.bars[FRONT_BAR].a;
				column.bars[TOP_BAR].b = column.bars[FRONT_BAR].b;
				column.bars[TOP_BAR].c = column.bars[REAR_BAR].b;
				column.bars[TOP_BAR].d = column.bars[REAR_BAR].a;
			}

			static void configure_link_moes_layer()
			{
				initialize_reference_MOEs();
				initialize_moe_layers();
			}

			static void initialize_moe_layers()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;

				Antares_Layer_Configuration cfg;
				cfg.Configure_Dynamic_Quads(True_Color_RGBA<NULLTYPE>(0,255,100,255),10);
				cfg.storage_period = ((_Scenario_Interface*)_global_scenario)->simulation_interval_length<int>();
				cfg.storage_offset = ((_Scenario_Interface*)_global_scenario)->simulation_interval_length<int>() - 1;
				cfg.primitive_color = true;
				cfg.primitive_normal = true;
				cfg.grouped = true;
				cfg.group_color=false;
				cfg.group_normal=false;
				
				_link_travel_time_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_travel_time_layer),ComponentType>*, string& > >(string("Link Travel Time"));
				_link_travel_time_layer->Initialize<NULLTYPE>(cfg);

				_link_speed_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_speed_layer),ComponentType>*, string& > >(string("Link Speed"));
				_link_speed_layer->Initialize<NULLTYPE>(cfg);

				_link_density_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_density_layer),ComponentType>*, string& > >(string("Link Density"));
				_link_density_layer->Initialize<NULLTYPE>(cfg);

				_link_travel_time_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_travel_time_ratio_layer),ComponentType>*, string& > >(string("Link Travel Time Ratio"));
				_link_travel_time_ratio_layer->Initialize<NULLTYPE>(cfg);

				_link_speed_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_speed_ratio_layer),ComponentType>*, string& > >(string("Link Speed Ratio"));
				_link_speed_ratio_layer->Initialize<NULLTYPE>(cfg);

				_link_density_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_density_ratio_layer),ComponentType>*, string& > >(string("Link Density Ratio"));
				_link_density_ratio_layer->Initialize<NULLTYPE>(cfg);

				cfg.grouped = false;
				_link_queue_length_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_queue_length_layer),ComponentType>*, string& > >(string("Link queue length"));
				_link_queue_length_layer->Initialize<NULLTYPE>(cfg);

			}

			static void initialize_reference_MOEs()
			{
				visualization_reference.link_travel_time = 0.7; // minute
				visualization_reference.link_travel_delay = 0.07; // 10% delay 
				visualization_reference.link_speed = 48.0; // mph
				visualization_reference.link_density = 5; // vehicles per minute per lane
				visualization_reference.link_in_flow_rate = 200.0; // vehicles per hour per lane
				visualization_reference.link_out_flow_rate = 200.0; // vehicles per hour per lane
				visualization_reference.link_speed_ratio = 0.5;
				visualization_reference.link_in_flow_ratio = 0.1;
				visualization_reference.link_out_flow_ratio = 0.1;
				visualization_reference.link_density_ratio = 0.1;
				visualization_reference.link_travel_time_ratio = 0.5;
			}

			feature_implementation void visualize_moe()
			{
				Push_To_Link_MOE_Display<ComponentType,CallerType,TargetType>();
			}

			typedef Link_Prototype<typename MasterType::link_type> _Link_Interface;
			feature_implementation void Push_To_Link_MOE_Display()
			{
				if (_link_travel_time_layer->template draw<bool>())
					push_to_link_travel_time_layer();
				if (_link_speed_layer->template draw<bool>())
					push_to_link_speed_layer();
				if (_link_density_layer->template draw<bool>())
					push_to_link_density_layer();
				if (_link_speed_ratio_layer->template draw<bool>())
					push_to_link_speed_ratio_layer();
				if (_link_density_ratio_layer->template draw<bool>())
					push_to_link_density_ratio_layer();
				if (_link_travel_time_ratio_layer->template draw<bool>())
					push_to_link_travel_time_ratio_layer();
				if (_link_queue_length_layer->template draw<bool>())
					push_to_link_queue_length_layer();
			}

			void set_column_height(int height)
			{
				column.bars[FRONT_BAR].d._z = height;
				column.bars[FRONT_BAR].c._z = height;

				column.bars[REAR_BAR].d._z = height;
				column.bars[REAR_BAR].c._z = height;

				column.bars[LEFT_SIDE_BAR].d._z = height;
				column.bars[LEFT_SIDE_BAR].c._z = height;

				column.bars[RIGHT_SIDE_BAR].d._z = height;
				column.bars[RIGHT_SIDE_BAR].c._z = height;

				column.bars[TOP_BAR].a._z = height;
				column.bars[TOP_BAR].b._z = height;
				column.bars[TOP_BAR].c._z = height;
				column.bars[TOP_BAR].d._z = height;
			}

			void set_column_color(True_Color_RGBA<NT> color)
			{
				column.bars[FRONT_BAR].color = color;
				column.bars[REAR_BAR].color = color;
				column.bars[LEFT_SIDE_BAR].color = color;
				column.bars[RIGHT_SIDE_BAR].color = color;
				column.bars[TOP_BAR].color = color;
			}

			True_Color_RGBA<NT> get_color_by_los(float fraction)
			{
				True_Color_RGBA<NT> color;
				color._a = 200;

				if (fraction < 0.3)
				{
					color._r = 51;
					color._g = 177;
					color._b = 0;
				}
				else if (fraction < 0.6)
				{
					color._r = 255;
					color._g = 204;
					color._b = 0;
				}
				else if (fraction < 0.9)
				{
					color._r = 153;
					color._g = 0;
					color._b = 0;
				}
				else
				{
					color._r = 100;
					color._g = 0;
					color._b = 0;
				}
				return color;
			}

			void push_to_link_travel_time_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_travel_time;
				_link_travel_time_cache.push_back(submission);
				
				int height = (realtime_link_moe_data.link_travel_time / visualization_reference.link_travel_time) * base_height;
				set_column_height(height);
				//float los = 1.0f - _link_fftt / (realtime_link_moe_data.link_travel_time * 60.0f);
				float los = get_link_los();
				set_column_color(get_color_by_los(los));
				_link_travel_time_layer->template Push_Element<Regular_Element>((void*)(&column));
			}

			float get_link_los()
			{
				return realtime_link_moe_data.link_density / _jam_density;
			}

			void push_to_link_speed_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_speed;
				_link_speed_cache.push_back(submission);

				int height = (realtime_link_moe_data.link_speed / visualization_reference.link_speed) * base_height;
				set_column_height(height);
				//float los = 1.0f - realtime_link_moe_data.link_speed / _original_free_flow_speed;
				float los = get_link_los();
				set_column_color(get_color_by_los(los));
				_link_speed_layer->template Push_Element<Regular_Element>((void*)(&column));
			}

			void push_to_link_density_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_density;
				_link_density_cache.push_back(submission);

				int height = (realtime_link_moe_data.link_density / visualization_reference.link_density) * base_height;
				set_column_height(height);
				//float los = realtime_link_moe_data.link_density / _jam_density;
				float los = get_link_los();
				set_column_color(get_color_by_los(los));
				_link_density_layer->template Push_Element<Regular_Element>((void*)(&column));
			}

			void push_to_link_speed_ratio_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_speed_ratio;
				_link_speed_ratio_cache.push_back(submission);

				int height = (realtime_link_moe_data.link_speed_ratio / visualization_reference.link_speed_ratio) * base_height;
				set_column_height(height);
				//float los = 1.0f - realtime_link_moe_data.link_speed / _original_free_flow_speed;
				float los = get_link_los();
				set_column_color(get_color_by_los(los));
				_link_speed_ratio_layer->template Push_Element<Regular_Element>((void*)(&column));
			}

			void push_to_link_density_ratio_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_density_ratio;
				_link_density_ratio_cache.push_back(submission);

				int height = (realtime_link_moe_data.link_density_ratio / visualization_reference.link_density_ratio) * base_height;
				set_column_height(height);
				//float los = realtime_link_moe_data.link_density_ratio;
				float los = get_link_los();
				set_column_color(get_color_by_los(los));
				_link_density_ratio_layer->template Push_Element<Regular_Element>((void*)(&column));
			}

			void push_to_link_travel_time_ratio_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_travel_time_ratio;
				_link_travel_time_ratio_cache.push_back(submission);

				int height = (realtime_link_moe_data.link_travel_time_ratio / visualization_reference.link_travel_time_ratio) * base_height;
				set_column_height(height);
				//float los = 1.0f - _link_fftt / (realtime_link_moe_data.link_travel_time * 60.0f);
				float los = get_link_los();
				set_column_color(get_color_by_los(los));
				_link_travel_time_ratio_layer->template Push_Element<Regular_Element>((void*)(&column));
			}
			
			void push_to_link_queue_length_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_queue_length;
				_link_queue_length_cache.push_back(submission);

				if (realtime_link_moe_data.link_queue_length == 0)
					return;
				configure_queue_length_box();
				//float los = realtime_link_moe_data.link_density / _jam_density;
				float los = get_link_los();
				True_Color_RGBA<NT> color = get_color_by_los(los);
				// set queue length column color
				queue_length_box.color = color;

				_link_queue_length_layer->Push_Element<Regular_Element>((void*)&queue_length_box);
			}

			static bool submit_attributes(Antares_Link_Implementation* _this,vector<string>& bucket)
			{
				// not supported yet
				return true;
			}

			void plot_link_moe()
			{
				((typename MasterType::network_type*)_global_network)->_network_avg_link_travel_time_layer->Clear_Accented<NT>();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_speed_layer->Clear_Accented<NT>();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_density_layer->Clear_Accented<NT>();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_travel_time_ratio_layer->Clear_Accented<NT>();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_speed_ratio_layer->Clear_Accented<NT>();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_density_ratio_layer->Clear_Accented<NT>();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_queue_length_layer->Clear_Accented<NT>();
#pragma pack(push,1)
				struct Plot_Element
				{
					True_Color_RGBA<NT> color;
					int num_primitives;
					Point_2D<MasterType>* points;
				};
#pragma pack(pop)
				Plot_Element element;
				element.color._r = 255;
				element.color._g = 0;
				element.color._b = 0;
				element.color._a = 255;
				// plot link_travel_time
				element.num_primitives = _link_travel_time_cache.size();
				element.points = &_link_travel_time_cache.front();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_travel_time_layer->Push_Element<Accented_Element>((void*)&element);

				// plot link_speed
				element.num_primitives = _link_speed_cache.size();
				element.points = &_link_speed_cache.front();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_speed_layer->Push_Element<Accented_Element>((void*)&element);

				// plot link_density
				element.num_primitives = _link_density_cache.size();
				element.points = &_link_density_cache.front();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_density_layer->Push_Element<Accented_Element>((void*)&element);

				// plot link_travel_time_ratio
				element.num_primitives = _link_travel_time_ratio_cache.size();
				element.points = &_link_travel_time_ratio_cache.front();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_travel_time_ratio_layer->Push_Element<Accented_Element>((void*)&element);

				// plot link_speed_ratio
				element.num_primitives = _link_speed_ratio_cache.size();
				element.points = &_link_speed_ratio_cache.front();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_speed_ratio_layer->Push_Element<Accented_Element>((void*)&element);

				// plot link_density_ratio
				element.num_primitives = _link_density_ratio_cache.size();
				element.points = &_link_density_ratio_cache.front();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_density_ratio_layer->Push_Element<Accented_Element>((void*)&element);

				// plot link_queue_length
				element.num_primitives = _link_queue_length_cache.size();
				element.points = &_link_queue_length_cache.front();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_queue_length_layer->Push_Element<Accented_Element>((void*)&element);
			}
			
			static void on_select(const list<void*>& removed,const list<void*>& added,const list<void*>& selected,vector<pair<string,string>>& bucket)
			{
				if(removed.size())
				{
					((type_of(MasterType::network)*) _global_network)->_link_lines->Clear_Accented<NT>();

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

			feature_implementation void Accent_Self()
			{
				Link_Line<MasterType> accented_line;

				accented_line.data = _displayed_line.data;
				accented_line.color._r = 255;
				accented_line.color._g = 128;
				accented_line.color._b = 0;
				accented_line.up_node = _displayed_line.up_node;
				accented_line.down_node = _displayed_line.down_node;

				((type_of(MasterType::network)*) _global_network)->_link_lines->Push_Element<Accented_Element>(&accented_line);
			}
			
			feature_implementation void Display_Attributes(vector<pair<string,string>>& bucket)
			{
				plot_link_moe();

				stringstream s;
				char str_buf[128];

				pair<string,string> key_value_pair;
				
				key_value_pair.first="Id";
				s << _internal_id;
				key_value_pair.second=s.str();
				s.str("");				
				bucket.push_back(key_value_pair);
				

				key_value_pair.first="Type";
				switch(_link_type)
				{
				case Types::Link_Type_Keys::ARTERIAL:
					key_value_pair.second="ARTERIAL";
					break;
				case Types::Link_Type_Keys::EXPRESSWAY:
					key_value_pair.second="EXPRESSWAY";
					break;
				case Types::Link_Type_Keys::FREEWAY:
					key_value_pair.second="FREEWAY";
					break;
				case Types::Link_Type_Keys::OFF_RAMP:
					key_value_pair.second="OFF_RAMP";
					break;
				case Types::Link_Type_Keys::ON_RAMP:
					key_value_pair.second="ON_RAMP";
					break;
				default:
					key_value_pair.second="NON_RECOGNIZED";
					break;
				}
				bucket.push_back(key_value_pair);
				
				key_value_pair.first="Length";
				sprintf(str_buf, "%.0f feet", _length);
				key_value_pair.second=str_buf;
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);
				
				key_value_pair.first="Number of lanes";
				sprintf(str_buf, "%d", _num_lanes);
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);
				
				key_value_pair.first="Upstream node";
				sprintf(str_buf, "%d", _upstream_intersection->_internal_id);
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);
				
				key_value_pair.first="Downstream node";
				sprintf(str_buf, "%d", _downstream_intersection->_internal_id);
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="Free-flow speed";
				sprintf(str_buf, "%.0f MPH", _original_free_flow_speed);
				key_value_pair.second=str_buf;
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="Travel time";
				sprintf(str_buf, "%.2f minutes", realtime_link_moe_data.link_travel_time);
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="Speed";
				sprintf(str_buf, "%.2f MPH", realtime_link_moe_data.link_speed);
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="Density";
				sprintf(str_buf, "%.2f VPMPL", realtime_link_moe_data.link_density);
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="Travel time ratio";
				sprintf(str_buf, "%.2f", realtime_link_moe_data.link_travel_time_ratio);
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="Speed ratio";
				sprintf(str_buf, "%.2f", realtime_link_moe_data.link_speed_ratio);
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="Density ratio";
				sprintf(str_buf, "%.2f", realtime_link_moe_data.link_density_ratio);
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="Queue length";
				sprintf(str_buf, "%.2f", realtime_link_moe_data.link_queue_length);
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);
			}

			static bool fetch_attributes(Antares_Link_Implementation* _this,vector<string>& bucket)
			{
				_this->plot_link_moe();

				stringstream s;
				char str_buf[256];
				s << _this->_internal_id;
				bucket.push_back(s.str());
				s.str("");
				
				switch(_this->_link_type)
				{
				case Types::Link_Type_Keys::ARTERIAL:
					bucket.push_back("ARTERIAL");
					break;
				case Types::Link_Type_Keys::EXPRESSWAY:
					bucket.push_back("EXPRESSWAY");
					break;
				case Types::Link_Type_Keys::FREEWAY:
					bucket.push_back("FREEWAY");
					break;
				case Types::Link_Type_Keys::OFF_RAMP:
					bucket.push_back("OFF_RAMP");
					break;
				case Types::Link_Type_Keys::ON_RAMP:
					bucket.push_back("ON_RAMP");
					break;
				default:
					bucket.push_back("NON_RECOGNIZED");
					break;
				}
				sprintf(str_buf, "%.0f feet", _this->_length);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%d", _this->_num_lanes);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%d", _this->_upstream_intersection->_internal_id);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%d", _this->_downstream_intersection->_internal_id);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.0f MPH", _this->_free_flow_speed);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f minutes", _this->realtime_link_moe_data.link_travel_time);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f MPH", _this->realtime_link_moe_data.link_speed);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f VPMPL", _this->realtime_link_moe_data.link_density);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_travel_time_ratio);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_speed_ratio);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_density_ratio);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_queue_length);
				bucket.push_back(str_buf);
				return true;
			}

			feature_implementation void configure_displayed_line()
			{
				_displayed_line.color = get_color_by_type(_link_type);
				_displayed_line.data = (void*)(this);

				float u_x = _upstream_intersection->_x_position;
				float u_y = _upstream_intersection->_y_position;
				float d_x = _downstream_intersection->_x_position;
				float d_y = _downstream_intersection->_y_position;
				float distance = sqrt((u_x - d_x) * (u_x - d_x) + (u_y - d_y) * (u_y - d_y));
				float sin_alpha = (d_y - u_y) / distance;
				float cos_alpha = (d_x - u_x) / distance;

				_displayed_line.up_node._x = u_x + _link_shift * sin_alpha;
				_displayed_line.up_node._y = u_y - _link_shift * cos_alpha;
				_displayed_line.up_node._z = 0;
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(_displayed_line.up_node);
				
				_displayed_line.down_node._x = d_x + _link_shift * sin_alpha;
				_displayed_line.down_node._y = d_y - _link_shift * cos_alpha;
				_displayed_line.down_node._z = 0;
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(_displayed_line.down_node);
			}

			True_Color_RGBA<NT> get_color_by_type(int link_type)
			{
				True_Color_RGBA<NT> color;
				switch(link_type)
				{
				case Link_Components::Types::Link_Type_Keys::FREEWAY:
				case Link_Components::Types::Link_Type_Keys::ON_RAMP:
				case Link_Components::Types::Link_Type_Keys::OFF_RAMP:
				case Link_Components::Types::Link_Type_Keys::EXPRESSWAY:
					color._r = 0;
					color._g = 255;
					color._b = 255;
					color._a = 255;
					return color;
				case Link_Components::Types::Link_Type_Keys::ARTERIAL:
					color._r = 128;
					color._g = 128;
					color._b = 128;
					color._a = 255;
					return color;
				default:
					color._r = 128;
					color._g = 128;
					color._b = 128;
					color._a = 255;
					return color;
				}
			}

			declare_feature_conditional(Newells_Conditional)
			{
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Link_Prototype<typename MasterType::link_type> _Link_Interface;
				_Link_Interface* _this_ptr=(_Link_Interface*)_this;
				if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::EVENTS_UPDATE_SUB_ITERATION)
				{
					((typename MasterType::link_type*)_this)->Swap_Event((Event)&Update_Events<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration;
					response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION;
				}
				else if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION)
				{
					((typename MasterType::link_type*)_this)->Swap_Event((Event)&Compute_Step_Flow_Supply_Update<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration;
					response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_LINK_MOVING_SUB_ITERATION;
				}
				else if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_LINK_MOVING_SUB_ITERATION)
				{
					((typename MasterType::link_type*)_this)->Swap_Event((Event)&Compute_Step_Flow_Link_Moving<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration;
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_VISUALIZATION_SUB_ITERATIONS;
				}
				else if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_VISUALIZATION_SUB_ITERATIONS)
				{
					((typename MasterType::link_type*)_this)->Swap_Event((Event)&Visualize_Link_MOE<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration+((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::EVENTS_UPDATE_SUB_ITERATION;
				}
				else
				{
					assert(false);
					cout << "Should never reach here in Antares link conditional!" << endl;
				}
			}
			
			declare_feature_event(Visualize_Link_MOE)
			{
				typedef Link_Prototype<typename MasterType::link_type> _Link_Interface;
				_Link_Interface* _this_ptr=(_Link_Interface*)_this;
				_this_ptr->template visualize_moe<NULLTYPE>();
			}

			static member_prototype(Antares_Layer,link_travel_time_layer,typename type_of(MasterType::antares_layer),none,none);
			static member_prototype(Antares_Layer,link_travel_time_delay_layer,typename type_of(MasterType::antares_layer),none,none);
			static member_prototype(Antares_Layer,link_speed_layer,typename type_of(MasterType::antares_layer),none,none);
			static member_prototype(Antares_Layer,link_density_layer,typename type_of(MasterType::antares_layer),none,none);
			static member_prototype(Antares_Layer,link_in_flow_rate_layer,typename type_of(MasterType::antares_layer),none,none);
			static member_prototype(Antares_Layer,link_out_flow_rate_layer,typename type_of(MasterType::antares_layer),none,none);
			static member_prototype(Antares_Layer,link_speed_ratio_layer,typename type_of(MasterType::antares_layer),none,none);
			static member_prototype(Antares_Layer,link_in_flow_ratio_layer,typename type_of(MasterType::antares_layer),none,none);
			static member_prototype(Antares_Layer,link_out_flow_ratio_layer,typename type_of(MasterType::antares_layer),none,none);
			static member_prototype(Antares_Layer,link_density_ratio_layer,typename type_of(MasterType::antares_layer),none,none);
			static member_prototype(Antares_Layer,link_travel_time_ratio_layer,typename type_of(MasterType::antares_layer),none,none);
			static member_prototype(Antares_Layer,link_queue_length_layer,typename type_of(MasterType::antares_layer),none,none);

			member_data(vector<Point_2D<MasterType>>,link_travel_time_cache,none,none);
			member_data(vector<Point_2D<MasterType>>,link_speed_cache,none,none);
			member_data(vector<Point_2D<MasterType>>,link_density_cache,none,none);
			member_data(vector<Point_2D<MasterType>>,link_travel_time_ratio_cache,none,none);
			member_data(vector<Point_2D<MasterType>>,link_speed_ratio_cache,none,none);
			member_data(vector<Point_2D<MasterType>>,link_density_ratio_cache,none,none);
			member_data(vector<Point_2D<MasterType>>,link_queue_length_cache,none,none);

			member_data(Link_Line<MasterType>, displayed_line, none, none);

			static member_prototype(Antares_Layer,link_moe_plot,typename type_of(MasterType::antares_layer),none,none);
		};
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_travel_time_layer;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_travel_time_delay_layer;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_speed_layer;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_density_layer;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_in_flow_rate_layer;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_out_flow_rate_layer;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_speed_ratio_layer;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_in_flow_ratio_layer;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_out_flow_ratio_layer;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_density_ratio_layer;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_travel_time_ratio_layer;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_queue_length_layer;
		
		
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_moe_plot;

		template<typename MasterType,typename ParentType,typename InheritanceList>
		Link_MOE_Data Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::visualization_reference;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		int Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::base_height = 100;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		float Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_vehicle_length = 13.5;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		float Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_shift = 20;
	}
}

using namespace Link_Components::Implementations;