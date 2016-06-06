//*********************************************************
//	Graphical_Network_Implementation.h - Graphical Link Variant
//*********************************************************

#pragma once
#include "Dependencies.h"
//#include "Traffic_Simulator\Link_Implementation.h"
#include "Visual_Link_Implementation.h"
#include "visual_traffic_simulator\Antares_Link_Implementation.h"

namespace Link_Components
{
	
	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Antares_Visual_Link_Implementation:public Visual_Link_Implementation<MasterType,INHERIT(Antares_Visual_Link_Implementation)>
		{
			// reference MOE values to determine height of bars
			static Link_MOE_Data visualization_reference;			
			static int base_height;
			static float _vehicle_length; // average length of a vehicle in feet
			static float _link_shift;
			Link_MOE_Column column;
			Link_MOE_Bar queue_length_box;

			Link_MOE_Data historic_link_moe_data;

			template<typename TargetType> void Initialize()
			{
				((Visual_Link_Implementation<MasterType,INHERIT(Antares_Visual_Link_Implementation)>*)this)->Initialize<TargetType>();
				// set bar attributes that are common for all MOEs.
				initialize_column();

				_link_travel_time_cache.reserve((12*60*60)/6);
				_link_speed_cache.reserve((12*60*60)/6);
				_link_density_cache.reserve((12*60*60)/6);
				_link_travel_time_ratio_cache.reserve((12*60*60)/6);
				_link_speed_ratio_cache.reserve((12*60*60)/6);
				_link_density_ratio_cache.reserve((12*60*60)/6);
				_link_queue_length_cache.reserve((12*60*60)/6);
			}

			void configure_queue_length_box()
			{
				typedef  Intersection_Components::Prototypes::Intersection< type_of(upstream_intersection)> _Intersection_Interface;
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
				physical_queue_length = min(distance, physical_queue_length);

				d_node._x = d_x ;
				d_node._y = d_y;
				d_node._z = 0.0;
//TODO
//				//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(d_node);

				u_node._x = d_x - physical_queue_length * cos_alpha;
				u_node._y = d_y - physical_queue_length * sin_alpha;
				u_node._z = 0.0;
//TODO
//				//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(u_node);
				
				queue_length_box.a._x = d_node._x;
				queue_length_box.a._y = d_node._y;
				queue_length_box.a._z = 1;
				queue_length_box.b._x = u_node._x;
				queue_length_box.b._y = u_node._y;
				queue_length_box.b._z = 1;

				queue_length_box.c._x = u_node._x + column_depth_up * sin_alpha;
				queue_length_box.c._y = u_node._y - column_depth_up * cos_alpha;
				queue_length_box.c._z = 1;

				queue_length_box.d._x = d_node._x + column_depth_down * sin_alpha;
				queue_length_box.d._y = d_node._y - column_depth_down * cos_alpha;
				queue_length_box.d._z = 1;
			}

			void initialize_column()
			{
				typedef  Intersection_Components::Prototypes::Intersection< type_of(upstream_intersection)> _Intersection_Interface;
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
//TODO
//				//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(u_node);

				d_node._x = c_x - (c_x - d_x) * (column_width / (0.5 * _length));
				d_node._y = c_y - (c_y - d_y) * (column_width / (0.5 * _length));
				d_node._z=0;
//TODO
//				//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(d_node);

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
				column.bars[REAR_BAR].a._x = d_node._x - column_depth / 2.0 * sin_alpha;
				column.bars[REAR_BAR].a._y = d_node._y + column_depth / 2.0 * cos_alpha;
				column.bars[REAR_BAR].a._z = 1;

				column.bars[REAR_BAR].b._x = u_node._x - column_depth / 2.0 * sin_alpha;
				column.bars[REAR_BAR].b._y = u_node._y + column_depth / 2.0 * cos_alpha;
				column.bars[REAR_BAR].b._z = 1;

				column.bars[REAR_BAR].c._x = column.bars[REAR_BAR].b._x;
				column.bars[REAR_BAR].c._y = column.bars[REAR_BAR].b._y;
				
				column.bars[REAR_BAR].d._x = column.bars[REAR_BAR].a._x;
				column.bars[REAR_BAR].d._y = column.bars[REAR_BAR].a._y;

				// configure left side bar
				column.bars[LEFT_SIDE_BAR].a = column.bars[REAR_BAR].b;

				column.bars[LEFT_SIDE_BAR].b = column.bars[FRONT_BAR].a;

				column.bars[LEFT_SIDE_BAR].c._x = column.bars[LEFT_SIDE_BAR].b._x;
				column.bars[LEFT_SIDE_BAR].c._y = column.bars[LEFT_SIDE_BAR].b._y;
				
				column.bars[LEFT_SIDE_BAR].d._x = column.bars[LEFT_SIDE_BAR].a._x;
				column.bars[LEFT_SIDE_BAR].d._y = column.bars[LEFT_SIDE_BAR].a._y;
				
				// configure right side bar
				column.bars[RIGHT_SIDE_BAR].a = column.bars[FRONT_BAR].b;

				column.bars[RIGHT_SIDE_BAR].b = column.bars[REAR_BAR].a;

				column.bars[RIGHT_SIDE_BAR].c._x = column.bars[RIGHT_SIDE_BAR].b._x;
				column.bars[RIGHT_SIDE_BAR].c._y = column.bars[RIGHT_SIDE_BAR].b._y;
				
				column.bars[RIGHT_SIDE_BAR].d._x = column.bars[RIGHT_SIDE_BAR].a._x;
				column.bars[RIGHT_SIDE_BAR].d._y = column.bars[RIGHT_SIDE_BAR].a._y;

				// configure top bar
				column.bars[TOP_BAR].a = column.bars[FRONT_BAR].a;
				column.bars[TOP_BAR].b = column.bars[FRONT_BAR].b;
				column.bars[TOP_BAR].c = column.bars[REAR_BAR].a;
				column.bars[TOP_BAR].d = column.bars[REAR_BAR].b;
			}

			static void configure_link_moes_layer()
			{
				initialize_reference_MOEs();
				initialize_moe_layers();
			}

			static void initialize_moe_layers()
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

				Antares_Layer_Configuration cfg;
				cfg.Configure_Dynamic_Quads(True_Color_RGBA<NULLTYPE>(0,255,100,255),10);
				cfg.storage_period = ((_Scenario_Interface*)_global_scenario)->simulation_interval_length<int>();
				cfg.target_sub_iteration = Scenario_Components::Types::END_OF_ITERATION + 1;
				cfg.storage_offset = ((_Scenario_Interface*)_global_scenario)->simulation_interval_length<int>() - 1;
				cfg.primitive_color = true;
				cfg.primitive_normal = true;
				cfg.grouped = true;
				cfg.group_color=false;
				cfg.group_normal=false;

				_link_travel_time_layer=Allocate_New_Layer<MT>(string("Link Travel Time"));
				_link_travel_time_layer->Initialize<NULLTYPE>(cfg);

				_link_speed_layer=Allocate_New_Layer<MT>(string("Link Speed"));
				_link_speed_layer->Initialize<NULLTYPE>(cfg);

				_link_density_layer=Allocate_New_Layer<MT>(string("Link Density"));
				_link_density_layer->Initialize<NULLTYPE>(cfg);

//TODO
//				//_link_travel_time_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_travel_time_ratio_layer)>*, string& > >(string("Link Travel Time Ratio"));
				//_link_travel_time_ratio_layer->Initialize<NULLTYPE>(cfg);

//TODO
//				//_link_speed_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_speed_ratio_layer)>*, string& > >(string("Link Speed Ratio"));
				//_link_speed_ratio_layer->Initialize<NULLTYPE>(cfg);

//TODO
//				//_link_density_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_density_ratio_layer)>*, string& > >(string("Link Density Ratio"));
				//_link_density_ratio_layer->Initialize<NULLTYPE>(cfg);

				cfg.grouped = false;

				_link_queue_length_layer=Allocate_New_Layer<MT>(string("Link Queue Length"));
				_link_queue_length_layer->Initialize<NULLTYPE>(cfg);


			}

			static void initialize_reference_MOEs()
			{
				visualization_reference.link_travel_time = 2.1; // minute
				visualization_reference.link_travel_delay = 0.21; // 10% delay 
				visualization_reference.link_speed = 16.0; // mph
				visualization_reference.link_density = 15; // vehicles per mile per lane
				visualization_reference.link_in_flow_rate = 200.0; // vehicles per hour per lane
				visualization_reference.link_out_flow_rate = 200.0; // vehicles per hour per lane
				visualization_reference.link_speed_ratio = 0.15;
				visualization_reference.link_in_flow_ratio = 0.1;
				visualization_reference.link_out_flow_ratio = 0.1;
				visualization_reference.link_density_ratio = 0.3;
				visualization_reference.link_travel_time_ratio = 1.5;
			}

			template<typename TargetType> void visualize_moe()
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				Push_To_Link_MOE_Plot_Display<TargetType>();
				Push_To_Link_MOE_Map_Display<TargetType>();
				if (((_Scenario_Interface*)_global_scenario)->template compare_with_historic_moe<bool>())
				{
					Push_To_Historic_Link_MOE_Plot_Display<TargetType>();
				}
			}

			template<typename TargetType> void Push_To_Historic_Link_MOE_Plot_Display()
			{
				Point_2D<MasterType> submission;
				submission._x = iteration();
				
				submission._y = historic_link_moe_data.link_travel_time;
				_historic_link_travel_time_cache.push_back(submission);

				submission._y = historic_link_moe_data.link_speed;
				_historic_link_speed_cache.push_back(submission);

				submission._y = historic_link_moe_data.link_density;
				_historic_link_density_cache.push_back(submission);

				//submission._y = historic_link_moe_data.link_speed_ratio;
				//_historic_link_speed_ratio_cache.push_back(submission);

				//submission._y = historic_link_moe_data.link_density_ratio;
				//_historic_link_density_ratio_cache.push_back(submission);

				//submission._y = historic_link_moe_data.link_travel_time_ratio;
				//_historic_link_travel_time_ratio_cache.push_back(submission);

				//submission._y = historic_link_moe_data.link_queue_length;
				//_historic_link_queue_length_cache.push_back(submission);
			}

			template<typename TargetType> void Push_To_Link_MOE_Plot_Display()
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
				Point_2D<MasterType> submission;
				submission._x = iteration();
				
				int current_time = ((_Network_Interface*)_global_network)->start_of_current_simulation_interval_relative<int>();
				if (current_time <= ((_Scenario_Interface*)_global_scenario)->assignment_interval_length<int>())
				{
					submission._y = realtime_link_moe_data.link_travel_time;
					_link_travel_time_cache.push_back(submission);

					submission._y = realtime_link_moe_data.link_speed;
					_link_speed_cache.push_back(submission);

					submission._y = realtime_link_moe_data.link_density;
					_link_density_cache.push_back(submission);
				}
				else
				{
					submission._y = non_volatile_link_moe_data.link_travel_time;
					_link_travel_time_cache.push_back(submission);

					submission._y = non_volatile_link_moe_data.link_speed;
					_link_speed_cache.push_back(submission);

					submission._y = non_volatile_link_moe_data.link_density;
					_link_density_cache.push_back(submission);
				}

				//submission._y = realtime_link_moe_data.link_speed_ratio;
				//_link_speed_ratio_cache.push_back(submission);

				//submission._y = realtime_link_moe_data.link_density_ratio;
				//_link_density_ratio_cache.push_back(submission);

				//submission._y = realtime_link_moe_data.link_travel_time_ratio;
				//_link_travel_time_ratio_cache.push_back(submission);

				//submission._y = realtime_link_moe_data.link_queue_length;
				//_link_queue_length_cache.push_back(submission);
			}

			typedef Link<typename MasterType::link_type> _Link_Interface;
			template<typename TargetType> void Push_To_Link_MOE_Map_Display()
			{
				if (_link_travel_time_layer->template draw<bool>())
					push_to_link_travel_time_layer();
				if (_link_speed_layer->template draw<bool>())
					push_to_link_speed_layer();
				if (_link_density_layer->template draw<bool>())
					push_to_link_density_layer();
				//if (_link_speed_ratio_layer->template draw<bool>())
					//push_to_link_speed_ratio_layer();
				//if (_link_density_ratio_layer->template draw<bool>())
					//push_to_link_density_ratio_layer();
				//if (_link_travel_time_ratio_layer->template draw<bool>())
					//push_to_link_travel_time_ratio_layer();
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
				color._a = 255;

				if (fraction < 0.3)
				{
					color._r = 0;
					color._g = 255;
					color._b = 0;
				}
				else if (fraction < 0.6)
				{
					color._r = 255;
					color._g = 255;
					color._b = 0;
				}
				else if (fraction < 0.9)
				{
					color._r = 255;
					color._g = 50;
					color._b = 0;
				}
				else
				{
					color._r = 150;
					color._g = 0;
					color._b = 0;
				}
				return color;
			}

			void push_to_link_travel_time_layer()
			{
				//int height = (realtime_link_moe_data.link_travel_time / visualization_reference.link_travel_time) * base_height;
				int height = int((min((float)realtime_link_moe_data.link_travel_time, 1.0f) / 1.0f) * (float)base_height);
				set_column_height(height);
				//float los = 1.0f - _link_fftt / (realtime_link_moe_data.link_travel_time * 60.0f);
				float los = get_link_los();
				set_column_color(get_color_by_los(los));
				_link_travel_time_layer->template Push_Element<Regular_Element>((void*)(&column));
			}

			float get_link_los()
			{
				//return realtime_link_moe_data.link_density / _jam_density;
				return max(0.0f, 1.0f - (float) (_length/(_original_free_flow_speed*5280.0/3600.0)) / ((float)realtime_link_moe_data.link_travel_time * 60.0f));
			}

			void push_to_link_speed_layer()
			{

				//int height = (realtime_link_moe_data.link_speed / visualization_reference.link_speed) * base_height;
				int height = int((min((float)realtime_link_moe_data.link_speed, 80.0f) / 80.0f) * (float)base_height);
				set_column_height(height);
				//float los = 1.0f - realtime_link_moe_data.link_speed / _original_free_flow_speed;
				float los = get_link_los();
				set_column_color(get_color_by_los(los));
				_link_speed_layer->template Push_Element<Regular_Element>((void*)(&column));
			}

			void push_to_link_density_layer()
			{
				//int height = (realtime_link_moe_data.link_density / visualization_reference.link_density) * base_height;
				int height = int((min((float)realtime_link_moe_data.link_density, 120.0f) / 120.0f) * (float)base_height);
				set_column_height(height);
				//float los = realtime_link_moe_data.link_density / _jam_density;
				float los = get_link_los();
				set_column_color(get_color_by_los(los));
				_link_density_layer->template Push_Element<Regular_Element>((void*)(&column));
			}

			void push_to_link_speed_ratio_layer()
			{
				int height = (realtime_link_moe_data.link_speed_ratio / visualization_reference.link_speed_ratio) * base_height;
				set_column_height(height);
				//float los = 1.0f - realtime_link_moe_data.link_speed / _original_free_flow_speed;
				float los = get_link_los();
				set_column_color(get_color_by_los(los));
				_link_speed_ratio_layer->template Push_Element<Regular_Element>((void*)(&column));
			}

			void push_to_link_density_ratio_layer()
			{
				int height = (realtime_link_moe_data.link_density_ratio / visualization_reference.link_density_ratio) * base_height;
				set_column_height(height);
				//float los = realtime_link_moe_data.link_density_ratio;
				float los = get_link_los();
				set_column_color(get_color_by_los(los));
				_link_density_ratio_layer->template Push_Element<Regular_Element>((void*)(&column));
			}

			void push_to_link_travel_time_ratio_layer()
			{
				int height = (realtime_link_moe_data.link_travel_time_ratio / visualization_reference.link_travel_time_ratio) * base_height;
				set_column_height(height);
				//float los = 1.0f - _link_fftt / (realtime_link_moe_data.link_travel_time * 60.0f);
				float los = get_link_los();
				set_column_color(get_color_by_los(los));
				_link_travel_time_ratio_layer->template Push_Element<Regular_Element>((void*)(&column));
			}
			
			void push_to_link_queue_length_layer()
			{
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

			static bool submit_attributes(Antares_Visual_Link_Implementation* _this,std::vector<string>& bucket)
			{
				// not supported yet
				return true;
			}

			void plot_link_moe()
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				//((typename MasterType::network_type*)_global_network)->_network_avg_link_travel_time_layer->Clear_Accented<NT>();
				//((typename MasterType::network_type*)_global_network)->_network_avg_link_speed_layer->Clear_Accented<NT>();
				//((typename MasterType::network_type*)_global_network)->_network_avg_link_density_layer->Clear_Accented<NT>();
				//((typename MasterType::network_type*)_global_network)->_network_avg_link_travel_time_ratio_layer->Clear_Accented<NT>();
				//((typename MasterType::network_type*)_global_network)->_network_avg_link_speed_ratio_layer->Clear_Accented<NT>();
				//((typename MasterType::network_type*)_global_network)->_network_avg_link_density_ratio_layer->Clear_Accented<NT>();
				//((typename MasterType::network_type*)_global_network)->_network_avg_link_queue_length_layer->Clear_Accented<NT>();

				//plot_current_link_moe();


				((typename MasterType::network_type*)_global_network)->_historic_link_travel_time_layer->Clear_Accented<NT>();
				((typename MasterType::network_type*)_global_network)->_historic_link_speed_layer->Clear_Accented<NT>();
				((typename MasterType::network_type*)_global_network)->_historic_link_density_layer->Clear_Accented<NT>();
				//((typename MasterType::network_type*)_global_network)->_historic_link_travel_time_ratio_layer->Clear_Accented<NT>();
				//((typename MasterType::network_type*)_global_network)->_historic_link_speed_ratio_layer->Clear_Accented<NT>();
				//((typename MasterType::network_type*)_global_network)->_historic_link_density_ratio_layer->Clear_Accented<NT>();
				//((typename MasterType::network_type*)_global_network)->_historic_link_queue_length_layer->Clear_Accented<NT>();
				plot_current_link_moe_in_historic_plot();
				if (((_Scenario_Interface*)_global_scenario)->template compare_with_historic_moe<bool>())
				{
					plot_historic_link_moe();
				}
			}

			void plot_current_link_moe()
			{
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

				//// plot link_travel_time_ratio
				//element.num_primitives = _link_travel_time_ratio_cache.size();
				//element.points = &_link_travel_time_ratio_cache.front();
				//((typename MasterType::network_type*)_global_network)->_network_avg_link_travel_time_ratio_layer->Push_Element<Accented_Element>((void*)&element);

				//// plot link_speed_ratio
				//element.num_primitives = _link_speed_ratio_cache.size();
				//element.points = &_link_speed_ratio_cache.front();
				//((typename MasterType::network_type*)_global_network)->_network_avg_link_speed_ratio_layer->Push_Element<Accented_Element>((void*)&element);

				//// plot link_density_ratio
				//element.num_primitives = _link_density_ratio_cache.size();
				//element.points = &_link_density_ratio_cache.front();
				//((typename MasterType::network_type*)_global_network)->_network_avg_link_density_ratio_layer->Push_Element<Accented_Element>((void*)&element);

				//// plot link_queue_length
				//element.num_primitives = _link_queue_length_cache.size();
				//element.points = &_link_queue_length_cache.front();
				//((typename MasterType::network_type*)_global_network)->_network_avg_link_queue_length_layer->Push_Element<Accented_Element>((void*)&element);
			}

			void plot_current_link_moe_in_historic_plot()
			{
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
				((typename MasterType::network_type*)_global_network)->_historic_link_travel_time_layer->Push_Element<Accented_Element>((void*)&element);

				// plot link_speed
				element.num_primitives = _link_speed_cache.size();
				element.points = &_link_speed_cache.front();
				((typename MasterType::network_type*)_global_network)->_historic_link_speed_layer->Push_Element<Accented_Element>((void*)&element);

				// plot link_density
				element.num_primitives = _link_density_cache.size();
				element.points = &_link_density_cache.front();
				((typename MasterType::network_type*)_global_network)->_historic_link_density_layer->Push_Element<Accented_Element>((void*)&element);

				//// plot link_travel_time_ratio
				//element.num_primitives = _link_travel_time_ratio_cache.size();
				//element.points = &_link_travel_time_ratio_cache.front();
				//((typename MasterType::network_type*)_global_network)->_historic_link_travel_time_ratio_layer->Push_Element<Accented_Element>((void*)&element);

				//// plot link_speed_ratio
				//element.num_primitives = _link_speed_ratio_cache.size();
				//element.points = &_link_speed_ratio_cache.front();
				//((typename MasterType::network_type*)_global_network)->_historic_link_speed_ratio_layer->Push_Element<Accented_Element>((void*)&element);

				//// plot link_density_ratio
				//element.num_primitives = _link_density_ratio_cache.size();
				//element.points = &_link_density_ratio_cache.front();
				//((typename MasterType::network_type*)_global_network)->_historic_link_density_ratio_layer->Push_Element<Accented_Element>((void*)&element);

				//// plot link_queue_length
				//element.num_primitives = _link_queue_length_cache.size();
				//element.points = &_link_queue_length_cache.front();
				//((typename MasterType::network_type*)_global_network)->_historic_link_queue_length_layer->Push_Element<Accented_Element>((void*)&element);
			}
			void plot_historic_link_moe()
			{
#pragma pack(push,1)
				struct Plot_Element
				{
					True_Color_RGBA<NT> color;
					int num_primitives;
					Point_2D<MasterType>* points;
				};
#pragma pack(pop)
				Plot_Element element;
				element.color._r = 0;
				element.color._g = 255;
				element.color._b = 255;
				element.color._a = 255;
				// plot link_travel_time
				element.num_primitives = _historic_link_travel_time_cache.size();
				element.points = &_historic_link_travel_time_cache.front();
				((typename MasterType::network_type*)_global_network)->_historic_link_travel_time_layer->Push_Element<Accented_Element>((void*)&element);

				// plot link_speed
				element.num_primitives = _historic_link_speed_cache.size();
				element.points = &_historic_link_speed_cache.front();
				((typename MasterType::network_type*)_global_network)->_historic_link_speed_layer->Push_Element<Accented_Element>((void*)&element);

				// plot link_density
				element.num_primitives = _historic_link_density_cache.size();
				element.points = &_historic_link_density_cache.front();
				((typename MasterType::network_type*)_global_network)->_historic_link_density_layer->Push_Element<Accented_Element>((void*)&element);

				//// plot link_travel_time_ratio
				//element.num_primitives = _historic_link_travel_time_ratio_cache.size();
				//element.points = &_historic_link_travel_time_ratio_cache.front();
				//((typename MasterType::network_type*)_global_network)->_historic_link_travel_time_ratio_layer->Push_Element<Accented_Element>((void*)&element);

				//// plot link_speed_ratio
				//element.num_primitives = _historic_link_speed_ratio_cache.size();
				//element.points = &_historic_link_speed_ratio_cache.front();
				//((typename MasterType::network_type*)_global_network)->_historic_link_speed_ratio_layer->Push_Element<Accented_Element>((void*)&element);

				//// plot link_density_ratio
				//element.num_primitives = _historic_link_density_ratio_cache.size();
				//element.points = &_historic_link_density_ratio_cache.front();
				//((typename MasterType::network_type*)_global_network)->_historic_link_density_ratio_layer->Push_Element<Accented_Element>((void*)&element);

				//// plot link_queue_length
				//element.num_primitives = _historic_link_queue_length_cache.size();
				//element.points = &_historic_link_queue_length_cache.front();
				//((typename MasterType::network_type*)_global_network)->_historic_link_queue_length_layer->Push_Element<Accented_Element>((void*)&element);
			}			

			// Elements passed through callback as void pointers whihc represent the start of the geometry-head, the first element of which is a pointer to the actual object, making it castable to that object
			static void on_select(const std::list<void*>& removed,const std::list<void*>& added,const std::list<void*>& selected,std::vector<pair<string,string>>& bucket)
			{
				if(removed.size())
				{
					((MasterType::network_type*) _global_network)->_link_lines->Clear_Accented<NT>();

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

			template<typename TargetType> void Accent_Self()
			{
				Link_Line<MasterType> accented_line;

				accented_line.data = _displayed_line.data;
				accented_line.color._r = 255;
				accented_line.color._g = 128;
				accented_line.color._b = 0;
				accented_line.up_node = _displayed_line.up_node;
				accented_line.down_node = _displayed_line.down_node;

				((MasterType::network_type*) _global_network)->_link_lines->Push_Element<Accented_Element>(&accented_line);
			}
			
			template<typename TargetType> void Display_Attributes(std::vector<pair<string,string>>& bucket)
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				plot_link_moe();

				stringstream s;
				char str_buf[128];

				pair<string,string> key_value_pair;
				
				key_value_pair.first="Id";
				s << _dbid;
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
				case Types::Link_Type_Keys::LOCAL:
					key_value_pair.second="LOCAL";
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
				sprintf(str_buf, "%d", _upstream_intersection->internal_id<int>());
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);
				
				key_value_pair.first="Downstream node";
				sprintf(str_buf, "%d", _downstream_intersection->internal_id<int>());
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="Original speed limit";
				sprintf(str_buf, "%.0f MPH", _original_speed_limit);
				key_value_pair.second=str_buf;
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="Speed limit";
				sprintf(str_buf, "%.0f MPH", _speed_limit);
				key_value_pair.second=str_buf;
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);
				
				key_value_pair.first="Original maximum flow rate";
				sprintf(str_buf, "%.0f VPHPL", _original_maximum_flow_rate);
				key_value_pair.second=str_buf;
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="Maximum flow rate";
				sprintf(str_buf, "%.0f VPHPL", _maximum_flow_rate);
				key_value_pair.second=str_buf;
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="Travel time";
				sprintf(str_buf, "%.2f minutes", realtime_link_moe_data.link_travel_time);
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);
				
				if (((_Scenario_Interface*)_global_scenario)->template compare_with_historic_moe<bool>()) 
				{
					key_value_pair.first="Historic travel time";
					sprintf(str_buf, "%.2f minutes", historic_link_moe_data.link_travel_time);
					key_value_pair.second=str_buf;				
					memset(&str_buf[0],0,128);
					bucket.push_back(key_value_pair);
				}

				key_value_pair.first="Speed";
				sprintf(str_buf, "%.2f MPH", realtime_link_moe_data.link_speed);
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);
				
				if (((_Scenario_Interface*)_global_scenario)->template compare_with_historic_moe<bool>()) 
				{
					key_value_pair.first="Historic speed";
					sprintf(str_buf, "%.2f MPH", historic_link_moe_data.link_speed);
					key_value_pair.second=str_buf;				
					memset(&str_buf[0],0,128);
					bucket.push_back(key_value_pair);
				}
				
				key_value_pair.first="Density";
				sprintf(str_buf, "%.2f VPMPL", realtime_link_moe_data.link_density);
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				if (((_Scenario_Interface*)_global_scenario)->template compare_with_historic_moe<bool>()) 
				{
					key_value_pair.first="Historic density";
					sprintf(str_buf, "%.2f VPMPL", historic_link_moe_data.link_density);
					key_value_pair.second=str_buf;				
					memset(&str_buf[0],0,128);
					bucket.push_back(key_value_pair);
				}

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

			static bool fetch_attributes(Antares_Visual_Link_Implementation* _this,std::vector<string>& bucket)
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
				case Types::Link_Type_Keys::LOCAL:
					bucket.push_back("LOCAL");
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

			template<typename TargetType> void configure_displayed_line()
			{
				_displayed_line.color = get_color_by_type(_link_type);
				_displayed_line.data = (void*)(this);

				float u_x = _upstream_intersection->x_position<float>();
				float u_y = _upstream_intersection->y_position<float>();
				float d_x = _downstream_intersection->x_position<float>();
				float d_y = _downstream_intersection->y_position<float>();
				float distance = sqrt((u_x - d_x) * (u_x - d_x) + (u_y - d_y) * (u_y - d_y));
				float sin_alpha = (d_y - u_y) / distance;
				float cos_alpha = (d_x - u_x) / distance;

				_displayed_line.up_node._x = u_x + _link_shift * sin_alpha;
				_displayed_line.up_node._y = u_y - _link_shift * cos_alpha;
				_displayed_line.up_node._z = 0;

				Scale_Coordinates<MT>(_displayed_line.up_node);
				
				_displayed_line.down_node._x = d_x + _link_shift * sin_alpha;
				_displayed_line.down_node._y = d_y - _link_shift * cos_alpha;
				_displayed_line.down_node._z = 0;

				Scale_Coordinates<MT>(_displayed_line.down_node);
			}

			True_Color_RGBA<NT> get_color_by_type(int link_type)
			{
				True_Color_RGBA<NT> color;
				switch(link_type)
				{
				case Link_Components::Types::Link_Type_Keys::FREEWAY:
				case Link_Components::Types::Link_Type_Keys::ON_RAMP:
					if (_ramp_meter != nullptr)
					{
						color._r = 0;
						color._g = 130;
						color._b = 190;
						color._a = 255;
						return color;
					}
				case Link_Components::Types::Link_Type_Keys::OFF_RAMP:
				case Link_Components::Types::Link_Type_Keys::EXPRESSWAY:
					color._r = 0;
					color._g = 255;
					color._b = 255;
					color._a = 255;
					return color;
				case Link_Components::Types::Link_Type_Keys::ARTERIAL:
				case Link_Components::Types::Link_Type_Keys::LOCAL:
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

			//void set_historic_link_moe(Link_MOE_Data historic_link_moe)
			//{
			//	Point_2D<MasterType> submission;
			//	submission._x = iteration();
			//	submission._y = historic_link_moe.link_travel_time;
			//	_historic_link_travel_time_cache.push_back(submission);

			//	submission._y = historic_link_moe.link_speed;
			//	_historic_link_speed_cache.push_back(submission);

			//	submission._y = historic_link_moe.link_density;
			//	_historic_link_density_cache.push_back(submission);

			//	submission._y = historic_link_moe.link_travel_time_ratio;
			//	_historic_link_travel_time_ratio_cache.push_back(submission);

			//	submission._y = historic_link_moe.link_speed_ratio;
			//	_historic_link_speed_ratio_cache.push_back(submission);

			//	submission._y = historic_link_moe.link_density_ratio;
			//	_historic_link_density_ratio_cache.push_back(submission);

			//	submission._y = historic_link_moe.link_queue_length;
			//	_historic_link_queue_length_cache.push_back(submission);
			//}

			//static void Newells_Conditional(ComponentType* _this,Event_Response& response)
			//{
			//	typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			//	typedef Link<typename MasterType::link_type> _Link_Interface;
			//	_Link_Interface* _this_ptr=(_Link_Interface*)_this;
			//	if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::EVENTS_UPDATE_SUB_ITERATION)
			//	{
			//		//((typename MasterType::link_type*)_this)->Swap_Event((Event)&Update_Events<NULLTYPE>);
			//		
			//		_this->Update_Events();

			//		response.next._iteration=iteration();
			//		response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION;
			//	}
			//	else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION)
			//	{
			//		//((typename MasterType::link_type*)_this)->Swap_Event((Event)&Compute_Step_Flow_Supply_Update<NULLTYPE>);

			//		_this->Compute_Step_Flow_Supply_Update();

			//		response.next._iteration=iteration();
			//		response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_ORIGIN_LINK_LOADING_SUB_ITERATION;
			//	}
			//	else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_ORIGIN_LINK_LOADING_SUB_ITERATION)
			//	{
			//		//((typename MasterType::link_type*)_this)->Swap_Event((Event)&Compute_Step_Origin_Link_Loading<NULLTYPE>);

			//		_this->Compute_Step_Origin_Link_Loading();

			//		response.next._iteration=iteration();
			//		response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_LINK_MOVING_SUB_ITERATION;
			//	}
			//	else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_LINK_MOVING_SUB_ITERATION)
			//	{
			//		//((typename MasterType::link_type*)_this)->Swap_Event((Event)&Compute_Step_Flow_Link_Moving<NULLTYPE>);

			//		_this->Compute_Step_Flow_Link_Moving();

			//		response.next._iteration=iteration();
			//		response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_VISUALIZATION_SUB_ITERATIONS;
			//	}
			//	else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_VISUALIZATION_SUB_ITERATIONS)
			//	{
			//		//((typename MasterType::link_type*)_this)->Swap_Event((Event)&Visualize_Link_MOE<NULLTYPE>);

			//		_this->Visualize_Link_MOE();

			//		response.next._iteration=iteration()+((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
			//		response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::EVENTS_UPDATE_SUB_ITERATION;
			//	}
			//	else
			//	{
			//		assert(false);
			//		cout << "Should never reach here in Antares link conditional!" << endl;
			//	}
			//}
			//
			//declare_event(Visualize_Link_MOE)
			void Visualize_Link_MOE()
			{
				typedef Link<typename MasterType::link_type> _Link_Interface;
				_Link_Interface* _this_ptr=(_Link_Interface*)this;
				_this_ptr->template visualize_moe<NULLTYPE>();
			}

			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,link_travel_time_layer, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,link_travel_time_delay_layer, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,link_speed_layer, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,link_density_layer, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,link_in_flow_rate_layer, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,link_out_flow_rate_layer, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,link_speed_ratio_layer, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,link_in_flow_ratio_layer, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,link_out_flow_ratio_layer, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,link_density_ratio_layer, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,link_travel_time_ratio_layer, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,link_queue_length_layer, NONE, NONE);

			m_data(std::vector<Point_2D<MasterType>>,link_travel_time_cache, NONE, NONE);
			m_data(std::vector<Point_2D<MasterType>>,link_speed_cache, NONE, NONE);
			m_data(std::vector<Point_2D<MasterType>>,link_density_cache, NONE, NONE);
			m_data(std::vector<Point_2D<MasterType>>,link_travel_time_ratio_cache, NONE, NONE);
			m_data(std::vector<Point_2D<MasterType>>,link_speed_ratio_cache, NONE, NONE);
			m_data(std::vector<Point_2D<MasterType>>,link_density_ratio_cache, NONE, NONE);
			m_data(std::vector<Point_2D<MasterType>>,link_queue_length_cache, NONE, NONE);

			m_data(std::vector<Point_2D<MasterType>>,historic_link_travel_time_cache, NONE, NONE);
			m_data(std::vector<Point_2D<MasterType>>,historic_link_speed_cache, NONE, NONE);
			m_data(std::vector<Point_2D<MasterType>>,historic_link_density_cache, NONE, NONE);
			m_data(std::vector<Point_2D<MasterType>>,historic_link_travel_time_ratio_cache, NONE, NONE);
			m_data(std::vector<Point_2D<MasterType>>,historic_link_speed_ratio_cache, NONE, NONE);
			m_data(std::vector<Point_2D<MasterType>>,historic_link_density_ratio_cache, NONE, NONE);
			m_data(std::vector<Point_2D<MasterType>>,historic_link_queue_length_cache, NONE, NONE);

			m_data(Link_Line<MasterType>, displayed_line, NONE, NONE);

			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,link_moe_plot, NONE, NONE);
		};
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_link_travel_time_layer;
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_link_travel_time_delay_layer;
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_link_speed_layer;
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_link_density_layer;
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_link_in_flow_rate_layer;
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_link_out_flow_rate_layer;
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_link_speed_ratio_layer;
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_link_in_flow_ratio_layer;
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_link_out_flow_ratio_layer;
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_link_density_ratio_layer;
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_link_travel_time_ratio_layer;
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_link_queue_length_layer;
		
		
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_link_moe_plot;

		template<typename MasterType,typename InheritanceList>
		Link_MOE_Data Antares_Visual_Link_Implementation<MasterType,InheritanceList>::visualization_reference;
		template<typename MasterType,typename InheritanceList>
		int Antares_Visual_Link_Implementation<MasterType,InheritanceList>::base_height = 700;
		template<typename MasterType,typename InheritanceList>
		float Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_vehicle_length = 5280/220; // space occupied by a vehicle under jam density
		template<typename MasterType,typename InheritanceList>
		float Antares_Visual_Link_Implementation<MasterType,InheritanceList>::_link_shift = 20;
	}
}

using namespace Link_Components::Implementations;
