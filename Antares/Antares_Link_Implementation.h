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
		struct Link_MOE_Column
		{
			int height;
			Link_MOE_Bar front_bar;
			Link_MOE_Bar rear_bar;
			Link_MOE_Bar left_side_bar;
			Link_MOE_Bar right_side_bar;
			Link_MOE_Bar top_bar;
		};

		implementation struct Antares_Link_Implementation:public Polaris_Link_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Link_Implementation)>
		{
			// reference MOE values to determine height of bars
			static Link_MOE_Data visualization_reference;			
			static int base_height;
			static float _vehicle_length; // average length of a vehicle in feet
			Link_MOE_Column column;
			Link_MOE_Column queue_length_column;
			feature_implementation void Initialize()
			{
				((Polaris_Link_Implementation<MasterType,ParentType,APPEND_CHILD(Polaris_Link_Implementation)>*)this)->Initialize<ComponentType,CallerType,TargetType>();
				// set bar attributes that are common for all MOEs.
				initialize_column();
			}

			void configure_queue_length_column()
			{
				define_component_interface(_Intersection_Interface, type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				Point_3D<MasterType> u_node;
				Point_3D<MasterType> d_node;
				
				float column_height = 10.0;
				float column_depth_down = 20.0;
				float column_depth_up = 100.0;

				float u_x = _upstream_intersection->_x_position;
				float u_y = _upstream_intersection->_y_position;
				float d_x = _downstream_intersection->_x_position;
				float d_y = _downstream_intersection->_y_position;

				float sin_alpha = (u_y - d_y) / _length;
				float cos_alpha = (u_x - d_x) / _length;

				float physical_queue_length = realtime_link_moe_data.link_queue_length * _vehicle_length;

				d_node._x = d_x ;
				d_node._y = d_y;
				d_node._z = 0.0;
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(d_node);

				u_node._x = d_x + physical_queue_length * cos_alpha;
				u_node._y = d_y + physical_queue_length * sin_alpha;
				u_node._z = 0.0;
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(u_node);
				
				// configure front bar
				queue_length_column.front_bar.a._x = d_node._x;
				queue_length_column.front_bar.a._y = d_node._y;
				queue_length_column.front_bar.a._z = 1;
				queue_length_column.front_bar.b._x = u_node._x;
				queue_length_column.front_bar.b._y = u_node._y;
				queue_length_column.front_bar.b._z = 1;

				queue_length_column.front_bar.c._x = queue_length_column.front_bar.b._x;
				queue_length_column.front_bar.c._y = queue_length_column.front_bar.b._y;
				queue_length_column.front_bar.c._z = column_height;
				
				queue_length_column.front_bar.d._x = queue_length_column.front_bar.a._x;
				queue_length_column.front_bar.d._y = queue_length_column.front_bar.a._y;
				queue_length_column.front_bar.d._z = column_height;

				// configure rear bar
				queue_length_column.rear_bar.a._x = d_node._x - column_depth_down * sin_alpha;
				queue_length_column.rear_bar.a._y = d_node._y + column_depth_down * cos_alpha;
				queue_length_column.rear_bar.a._z = 1;

				queue_length_column.rear_bar.b._x = u_node._x - column_depth_up * sin_alpha;
				queue_length_column.rear_bar.b._y = u_node._y + column_depth_up * cos_alpha;
				queue_length_column.rear_bar.b._z = 1;

				queue_length_column.rear_bar.c._x = queue_length_column.rear_bar.b._x;
				queue_length_column.rear_bar.c._y = queue_length_column.rear_bar.b._y;
				queue_length_column.rear_bar.c._z = column_height;

				queue_length_column.rear_bar.d._x = queue_length_column.rear_bar.a._x;
				queue_length_column.rear_bar.d._y = queue_length_column.rear_bar.a._y;
				queue_length_column.rear_bar.d._z = column_height;

				// configure left side bar
				queue_length_column.left_side_bar.a = queue_length_column.front_bar.a;
				queue_length_column.left_side_bar.b = queue_length_column.rear_bar.a;
				queue_length_column.left_side_bar.c = queue_length_column.rear_bar.d;
				queue_length_column.left_side_bar.d = queue_length_column.front_bar.d;

				// configure right side bar
				queue_length_column.right_side_bar.a = queue_length_column.front_bar.b;
				queue_length_column.right_side_bar.b = queue_length_column.rear_bar.b;
				queue_length_column.right_side_bar.c = queue_length_column.rear_bar.c;
				queue_length_column.right_side_bar.d = queue_length_column.front_bar.c;

				// configure top bar
				queue_length_column.top_bar.a = queue_length_column.front_bar.d;
				queue_length_column.top_bar.b = queue_length_column.front_bar.c;
				queue_length_column.top_bar.c = queue_length_column.rear_bar.c;
				queue_length_column.top_bar.d = queue_length_column.rear_bar.d;

			}

			void initialize_column()
			{
				define_component_interface(_Intersection_Interface, type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				Point_3D<MasterType> u_node;
				Point_3D<MasterType> d_node;
			
				float u_x = _upstream_intersection->_x_position;
				float u_y = _upstream_intersection->_y_position;
				float d_x = _downstream_intersection->_x_position;
				float d_y = _downstream_intersection->_y_position;
				float c_x = (u_x + d_x) / 2.0f;
				float c_y = (u_y + d_y) / 2.0f;
				float column_width = 100.0;
				float column_depth = 40.0;
				
				float sin_alpha = (d_y - u_y) / _length;
				float cos_alpha = (d_x - u_x) / _length;

				u_node._x = c_x - (c_x - u_x) * (column_width / (0.5 * _length));
				u_node._y = c_y - (c_y - u_y) * (column_width / (0.5 * _length));
				u_node._z=0;
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(u_node);

				d_node._x = c_x - (c_x - d_x) * (column_width / (0.5 * _length));
				d_node._y = c_y - (c_y - d_y) * (column_width / (0.5 * _length));
				d_node._z=0;
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(d_node);

				// configure front bar
				column.front_bar.a._x = u_node._x + column_depth / 2.0 * sin_alpha;
				column.front_bar.a._y = u_node._y - column_depth / 2.0 * cos_alpha;
				column.front_bar.a._z = 1;

				column.front_bar.b._x = d_node._x + column_depth / 2.0 * sin_alpha;
				column.front_bar.b._y = d_node._y - column_depth / 2.0 * cos_alpha;
				column.front_bar.b._z = 1;

				column.front_bar.c._x = column.front_bar.b._x;
				column.front_bar.c._y = column.front_bar.b._y;
				
				column.front_bar.d._x = column.front_bar.a._x;
				column.front_bar.d._y = column.front_bar.a._y;

				// configure rear bar
				column.rear_bar.a._x = u_node._x - column_depth / 2.0 * sin_alpha;
				column.rear_bar.a._y = u_node._y + column_depth / 2.0 * cos_alpha;
				column.rear_bar.a._z = 1;

				column.rear_bar.b._x = d_node._x - column_depth / 2.0 * sin_alpha;
				column.rear_bar.b._y = d_node._y + column_depth / 2.0 * cos_alpha;
				column.rear_bar.b._z = 1;

				column.rear_bar.c._x = column.rear_bar.b._x;
				column.rear_bar.c._y = column.rear_bar.b._y;
				
				column.rear_bar.d._x = column.rear_bar.a._x;
				column.rear_bar.d._y = column.rear_bar.a._y;

				// configure left side bar
				column.left_side_bar.a = column.front_bar.a;

				column.left_side_bar.b = column.rear_bar.a;

				column.left_side_bar.c._x = column.left_side_bar.b._x;
				column.left_side_bar.c._y = column.left_side_bar.b._y;
				
				column.left_side_bar.d._x = column.left_side_bar.a._x;
				column.left_side_bar.d._y = column.left_side_bar.a._y;
				
				// configure right side bar
				column.right_side_bar.a = column.front_bar.b;

				column.right_side_bar.b = column.rear_bar.b;

				column.right_side_bar.c._x = column.right_side_bar.b._x;
				column.right_side_bar.c._y = column.right_side_bar.b._y;
				
				column.right_side_bar.d._x = column.right_side_bar.a._x;
				column.right_side_bar.d._y = column.right_side_bar.a._y;

				// configure top bar
				column.top_bar.a = column.front_bar.a;
				column.top_bar.b = column.front_bar.b;
				column.top_bar.c = column.rear_bar.b;
				column.top_bar.d = column.rear_bar.a;
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
				
				_link_travel_time_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_travel_time_layer),ComponentType>*, string& > >(string("Link Travel Time"));
				_link_travel_time_layer->Initialize<NULLTYPE>(cfg);

				//_link_travel_time_delay_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_travel_time_delay_layer),ComponentType>*, string& > >(string("Link Travel Time Delay"));
				//_link_travel_time_delay_layer->Initialize<NULLTYPE>(cfg);

				_link_speed_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_speed_layer),ComponentType>*, string& > >(string("Link Speed"));
				_link_speed_layer->Initialize<NULLTYPE>(cfg);

				_link_density_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_density_layer),ComponentType>*, string& > >(string("Link Density"));
				_link_density_layer->Initialize<NULLTYPE>(cfg);

				//_link_in_flow_rate_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_in_flow_rate_layer),ComponentType>*, string& > >(string("Link In-Flow Rate"));
				//_link_in_flow_rate_layer->Initialize<NULLTYPE>(cfg);

				//_link_out_flow_rate_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_out_flow_rate_layer),ComponentType>*, string& > >(string("Link Out-Flow Rate"));
				//_link_out_flow_rate_layer->Initialize<NULLTYPE>(cfg);


				_link_travel_time_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_travel_time_ratio_layer),ComponentType>*, string& > >(string("Link Travel Time Ratio"));
				_link_travel_time_ratio_layer->Initialize<NULLTYPE>(cfg);

				_link_speed_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_speed_ratio_layer),ComponentType>*, string& > >(string("Link Speed Ratio"));
				_link_speed_ratio_layer->Initialize<NULLTYPE>(cfg);

				//_link_in_flow_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_in_flow_ratio_layer),ComponentType>*, string& > >(string("Link In-Flow Ratio"));
				//_link_in_flow_ratio_layer->Initialize<NULLTYPE>(cfg);

				//_link_out_flow_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_out_flow_ratio_layer),ComponentType>*, string& > >(string("Link Out-Flow Ratio"));
				//_link_out_flow_ratio_layer->Initialize<NULLTYPE>(cfg);

				_link_density_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_density_ratio_layer),ComponentType>*, string& > >(string("Link Density Ratio"));
				_link_density_ratio_layer->Initialize<NULLTYPE>(cfg);

				//_link_travel_time_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_travel_time_ratio_layer),ComponentType>*, string& > >(string("Link Travel Time Ratio"));
				//_link_travel_time_ratio_layer->Initialize<NULLTYPE>(cfg);

				_link_queue_length_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_queue_length_layer),ComponentType>*, string& > >(string("Link queue length"));
				_link_queue_length_layer->Initialize<NULLTYPE>(cfg);

				//_link_moe_plot=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_moe_plot),ComponentType>*, string& > >(string("Link MOE Plot"));
				//cfg.Configure_Plot();
				//_link_moe_plot->Initialize<NULLTYPE>(cfg);
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

			//member_component(typename MasterType::graphical_link_moe_group_type,graphical_link_moe_group,none,none);
			//static member_prototype(Graphical_Link_MOE_Group,graphical_link_moe_group,typename MasterType::graphical_link_moe_group_type,none,none);
			feature_implementation void visualize_moe()
			{
				Push_To_Link_MOE_Display<ComponentType,CallerType,TargetType>();
			}

			typedef Link_Prototype<typename MasterType::link_type> _Link_Interface;
			feature_implementation void Push_To_Link_MOE_Display()
			{
				push_to_link_travel_time_layer();
				//push_to_link_travel_time_delay_layer();
				push_to_link_speed_layer();
				push_to_link_density_layer();
				//push_to_link_in_flow_rate_layer();
				//push_to_link_out_flow_rate_layer();
				push_to_link_speed_ratio_layer();
				//push_to_link_in_flow_ratio_layer();
				//push_to_link_out_flow_ratio_layer();
				push_to_link_density_ratio_layer();
				push_to_link_travel_time_ratio_layer();
				push_to_link_queue_length_layer();
			}

			void set_column_height(int height)
			{
				column.front_bar.d._z = height;
				column.front_bar.c._z = height;

				column.rear_bar.d._z = height;
				column.rear_bar.c._z = height;

				column.left_side_bar.d._z = height;
				column.left_side_bar.c._z = height;

				column.right_side_bar.d._z = height;
				column.right_side_bar.c._z = height;

				column.top_bar.a._z = height;
				column.top_bar.b._z = height;
				column.top_bar.c._z = height;
				column.top_bar.d._z = height;
			}

			void set_column_color(True_Color_RGBA<NT> color)
			{
				column.front_bar.color = color;
				column.rear_bar.color = color;
				column.left_side_bar.color = color;
				column.right_side_bar.color = color;
				column.top_bar.color = color;
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

#define draw_column(moe_layer)\
	moe_layer->Push_Element<Regular_Element>((void*)&column.front_bar);\
	moe_layer->Push_Element<Regular_Element>((void*)&column.rear_bar);\
	moe_layer->Push_Element<Regular_Element>((void*)&column.left_side_bar);\
	moe_layer->Push_Element<Regular_Element>((void*)&column.right_side_bar);\
	moe_layer->Push_Element<Regular_Element>((void*)&column.top_bar);

			void push_to_link_travel_time_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_travel_time;
				_link_travel_time_cache.push_back(submission);
				
				int height = (realtime_link_moe_data.link_travel_time / visualization_reference.link_travel_time) * base_height;
				set_column_height(height);
				float los = 1.0f - _link_fftt / (realtime_link_moe_data.link_travel_time * 60.0f);
				set_column_color(get_color_by_los(los));
				draw_column(_link_travel_time_layer);
			}

			void push_to_link_travel_time_delay_layer()
			{
				//int height = (realtime_link_moe_data.link_travel_delay / visualization_reference.link_travel_delay) * base_height;
				//set_column_height(height);
				//float los = max(0.0f, 1.0f - _fftt / (realtime_link_moe_data.link_travel_time * 60.0f));
				//bar.color = get_color_by_los(los);
				//_link_travel_time_delay_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_speed_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_speed;
				_link_speed_cache.push_back(submission);

				int height = (realtime_link_moe_data.link_speed / visualization_reference.link_speed) * base_height;
				set_column_height(height);
				float los = 1.0f - realtime_link_moe_data.link_speed / _original_free_flow_speed;
				set_column_color(get_color_by_los(los));
				draw_column(_link_speed_layer)
			}

			void push_to_link_density_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_density;
				_link_density_cache.push_back(submission);

				int height = (realtime_link_moe_data.link_density / visualization_reference.link_density) * base_height;
				set_column_height(height);
				float los = realtime_link_moe_data.link_density / _jam_density;
				set_column_color(get_color_by_los(los));
				draw_column(_link_density_layer);
			}

			void push_to_link_in_flow_rate_layer()
			{
				int height = (realtime_link_moe_data.link_in_flow_rate / visualization_reference.link_in_flow_rate) * base_height;
				set_column_height(height);
				_link_in_flow_rate_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_out_flow_rate_layer()
			{
				int height = (realtime_link_moe_data.link_out_flow_rate / visualization_reference.link_out_flow_rate) * base_height;
				set_column_height(height);
				_link_out_flow_rate_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_speed_ratio_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_speed_ratio;
				_link_speed_ratio_cache.push_back(submission);

				int height = (realtime_link_moe_data.link_speed_ratio / visualization_reference.link_speed_ratio) * base_height;
				set_column_height(height);
				float los = 1.0f - realtime_link_moe_data.link_speed / _original_free_flow_speed;
				set_column_color(get_color_by_los(los));
				draw_column(_link_speed_ratio_layer);
			}

			void push_to_link_in_flow_ratio_layer()
			{
				int height = (realtime_link_moe_data.link_in_flow_ratio / visualization_reference.link_in_flow_ratio) * base_height;
				set_column_height(height);
				_link_in_flow_ratio_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_out_flow_ratio_layer()
			{
				int height = (realtime_link_moe_data.link_out_flow_ratio / visualization_reference.link_out_flow_ratio) * base_height;
				set_column_height(height);
				_link_out_flow_ratio_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_density_ratio_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_density_ratio;
				_link_density_ratio_cache.push_back(submission);

				int height = (realtime_link_moe_data.link_density_ratio / visualization_reference.link_density_ratio) * base_height;
				set_column_height(height);
				float los = realtime_link_moe_data.link_density_ratio;
				set_column_color(get_color_by_los(los));
				draw_column(_link_density_ratio_layer);
			}

			void push_to_link_travel_time_ratio_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_travel_time_ratio;
				_link_travel_time_ratio_cache.push_back(submission);

				int height = (realtime_link_moe_data.link_travel_time_ratio / visualization_reference.link_travel_time_ratio) * base_height;
				set_column_height(height);
				float los = 1.0f - _link_fftt / (realtime_link_moe_data.link_travel_time * 60.0f);
				set_column_color(get_color_by_los(los));
				draw_column(_link_travel_time_ratio_layer);
			}
			
			void push_to_link_queue_length_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_queue_length;
				_link_queue_length_cache.push_back(submission);

				if (realtime_link_moe_data.link_queue_length == 0)
					return;
				configure_queue_length_column();
				float los = realtime_link_moe_data.link_density / _jam_density;
				True_Color_RGBA<NT> color = get_color_by_los(los);
				// set queue length column color
				queue_length_column.front_bar.color = color;
				queue_length_column.rear_bar.color = color;
				queue_length_column.left_side_bar.color = color;
				queue_length_column.right_side_bar.color = color;
				queue_length_column.top_bar.color = color;

				_link_queue_length_layer->Push_Element<Regular_Element>((void*)&queue_length_column.front_bar);
				_link_queue_length_layer->Push_Element<Regular_Element>((void*)&queue_length_column.rear_bar);
				_link_queue_length_layer->Push_Element<Regular_Element>((void*)&queue_length_column.left_side_bar);
				_link_queue_length_layer->Push_Element<Regular_Element>((void*)&queue_length_column.right_side_bar);
				_link_queue_length_layer->Push_Element<Regular_Element>((void*)&queue_length_column.top_bar);
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
				//((typename MasterType::network_type*)_global_network)->_network_avg_link_in_volume_layer->Clear_Accented<NT>();
				//((typename MasterType::network_type*)_global_network)->_network_avg_link_out_volume_layer->Clear_Accented<NT>();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_travel_time_ratio_layer->Clear_Accented<NT>();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_speed_ratio_layer->Clear_Accented<NT>();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_density_ratio_layer->Clear_Accented<NT>();
				((typename MasterType::network_type*)_global_network)->_network_avg_link_queue_length_layer->Clear_Accented<NT>();
#pragma pack(push,1)
				struct Plot_Element
				{
					int num_primitives;
					Point_2D<MasterType>* points;
				};
#pragma pack(pop)
				Plot_Element element;

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

				sprintf(str_buf, "%.2f minutes", _this->realtime_link_moe_data.link_travel_time);
				bucket.push_back(str_buf);
				//sprintf(str_buf, "%.2f minutes", _this->realtime_link_moe_data.link_travel_delay);
				//bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f MPH", _this->realtime_link_moe_data.link_speed);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f VPMPL", _this->realtime_link_moe_data.link_density);
				bucket.push_back(str_buf);
				//sprintf(str_buf, "%.2f VPHPL", _this->realtime_link_moe_data.link_in_flow_rate);
				//bucket.push_back(str_buf);
				//sprintf(str_buf, "%.2f VPHPL", _this->realtime_link_moe_data.link_out_flow_rate);
				//bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_travel_time_ratio);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_speed_ratio);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_density_ratio);
				bucket.push_back(str_buf);
				//sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_in_flow_ratio);
				//bucket.push_back(str_buf);
				//sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_out_flow_ratio);
				//bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_queue_length);
				bucket.push_back(str_buf);
				return true;
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
	}
}

using namespace Link_Components::Implementations;