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
		
		implementation struct Antares_Link_Implementation:public Polaris_Link_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Link_Implementation)>
		{
			member_prototype(Antares_Layer,link_speed_plot,typename type_of(MasterType::antares_layer),none,none);
			// reference MOE values to determine height of bars
			static Link_MOE_Data visualization_reference;			
			static int base_height;
			Link_MOE_Bar bar;
			
			feature_implementation void Initialize()
			{
				((Polaris_Link_Implementation<MasterType,ParentType,APPEND_CHILD(Polaris_Link_Implementation)>*)this)->Initialize<ComponentType,CallerType,TargetType>();
				// set bar attributes that are common for all MOEs.
				initialize_bar();
			}

			void initialize_bar()
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
				float bar_width = 100.0;

				u_node._x = c_x - (c_x - u_x) * (bar_width / (0.5 * _length));
				u_node._y = c_y - (c_y - u_y) * (bar_width / (0.5 * _length));
				u_node._z=0;
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(u_node);

				d_node._x = c_x - (c_x - d_x) * (bar_width / (0.5 * _length));
				d_node._y = c_y - (c_y - d_y) * (bar_width / (0.5 * _length));
				d_node._z=0;
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(d_node);
				
				bar.color._r = 0;
				bar.color._g = 255;
				bar.color._b = 0;
				bar.color._a = 200;

				bar.a._x = u_node._x;
				bar.a._y = u_node._y;
				bar.a._z = 1;
				bar.b._x = d_node._x;
				bar.b._y = d_node._y;
				bar.b._z = 1;
				bar.c._x = d_node._x;
				bar.c._y = d_node._y;
				bar.d._x = u_node._x;
				bar.d._y = u_node._y;
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

				_link_travel_time_delay_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_travel_time_delay_layer),ComponentType>*, string& > >(string("Link Travel Time Delay"));
				_link_travel_time_delay_layer->Initialize<NULLTYPE>(cfg);

				_link_speed_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_speed_layer),ComponentType>*, string& > >(string("Link Speed"));
				_link_speed_layer->Initialize<NULLTYPE>(cfg);

				_link_density_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_density_layer),ComponentType>*, string& > >(string("Link Density"));
				_link_density_layer->Initialize<NULLTYPE>(cfg);

				_link_in_flow_rate_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_in_flow_rate_layer),ComponentType>*, string& > >(string("Link In-Flow Rate"));
				_link_in_flow_rate_layer->Initialize<NULLTYPE>(cfg);

				_link_out_flow_rate_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_out_flow_rate_layer),ComponentType>*, string& > >(string("Link Out-Flow Rate"));
				_link_out_flow_rate_layer->Initialize<NULLTYPE>(cfg);

				_link_speed_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_speed_ratio_layer),ComponentType>*, string& > >(string("Link Speed Ratio"));
				_link_speed_ratio_layer->Initialize<NULLTYPE>(cfg);

				_link_in_flow_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_in_flow_ratio_layer),ComponentType>*, string& > >(string("Link In-Flow Ratio"));
				_link_in_flow_ratio_layer->Initialize<NULLTYPE>(cfg);

				_link_out_flow_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_out_flow_ratio_layer),ComponentType>*, string& > >(string("Link Out-Flow Ratio"));
				_link_out_flow_ratio_layer->Initialize<NULLTYPE>(cfg);

				_link_density_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_density_ratio_layer),ComponentType>*, string& > >(string("Link Density Ratio"));
				_link_density_ratio_layer->Initialize<NULLTYPE>(cfg);

				_link_travel_time_ratio_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_travel_time_ratio_layer),ComponentType>*, string& > >(string("Link Travel Time Ratio"));
				_link_travel_time_ratio_layer->Initialize<NULLTYPE>(cfg);

				_link_moe_plot=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_moe_plot),ComponentType>*, string& > >(string("Link MOE Plot"));
				cfg.Configure_Plot();
				_link_moe_plot->Initialize<NULLTYPE>(cfg);
			}

			static void initialize_reference_MOEs()
			{
				visualization_reference.link_travel_time = 0.7; // minute
				visualization_reference.link_travel_delay = 0.07; // 10% delay 
				visualization_reference.link_speed = 48.0; // mph
				visualization_reference.link_density = 1.5; // vehicles per minute per lane
				visualization_reference.link_in_flow_rate = 200.0; // vehicles per hour per lane
				visualization_reference.link_out_flow_rate = 200.0; // vehicles per hour per lane
				visualization_reference.link_speed_ratio = 0.5;
				visualization_reference.link_in_flow_ratio = 0.5;
				visualization_reference.link_out_flow_ratio = 0.5;
				visualization_reference.link_density_ratio = 0.5;
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
				push_to_link_travel_time_delay_layer();
				push_to_link_speed_layer();
				push_to_link_density_layer();
				push_to_link_in_flow_rate_layer();
				push_to_link_out_flow_rate_layer();
				push_to_link_speed_ratio_layer();
				push_to_link_in_flow_ratio_layer();
				push_to_link_out_flow_ratio_layer();
				push_to_link_density_ratio_layer();
				push_to_link_travel_time_ratio_layer();
			}

			void set_bar_height(int height)
			{
				bar.d._z = height;
				bar.c._z = height;
			}

			void push_to_link_travel_time_layer()
			{
				int height = (realtime_link_moe_data.link_travel_time / visualization_reference.link_travel_time) * base_height;
				set_bar_height(height);
				_link_travel_time_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_travel_time_delay_layer()
			{
				int height = (realtime_link_moe_data.link_travel_delay / visualization_reference.link_travel_delay) * base_height;
				set_bar_height(height);
				_link_travel_time_delay_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_speed_layer()
			{
				int height = (realtime_link_moe_data.link_speed / visualization_reference.link_speed) * base_height;
				set_bar_height(height);
				_link_speed_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_density_layer()
			{
				Point_2D<MasterType> submission;
				submission._x = _iteration;
				submission._y = realtime_link_moe_data.link_density;

				_link_density_cache.push_back(submission);

				int height = (realtime_link_moe_data.link_density / visualization_reference.link_density) * base_height;
				set_bar_height(height);
				_link_density_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_in_flow_rate_layer()
			{
				int height = (realtime_link_moe_data.link_in_flow_rate / visualization_reference.link_in_flow_rate) * base_height;
				set_bar_height(height);
				_link_in_flow_rate_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_out_flow_rate_layer()
			{
				int height = (realtime_link_moe_data.link_out_flow_rate / visualization_reference.link_out_flow_rate) * base_height;
				set_bar_height(height);
				_link_out_flow_rate_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_speed_ratio_layer()
			{
				int height = (realtime_link_moe_data.link_speed_ratio / visualization_reference.link_speed_ratio) * base_height;
				set_bar_height(height);
				_link_speed_ratio_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_in_flow_ratio_layer()
			{
				int height = (realtime_link_moe_data.link_in_flow_ratio / visualization_reference.link_in_flow_ratio) * base_height;
				set_bar_height(height);
				_link_in_flow_ratio_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_out_flow_ratio_layer()
			{
				int height = (realtime_link_moe_data.link_out_flow_ratio / visualization_reference.link_out_flow_ratio) * base_height;
				set_bar_height(height);
				_link_out_flow_ratio_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_density_ratio_layer()
			{
				int height = (realtime_link_moe_data.link_density_ratio / visualization_reference.link_density_ratio) * base_height;
				set_bar_height(height);
				_link_density_ratio_layer->Push_Element<Regular_Element>((void*)&bar);
			}

			void push_to_link_travel_time_ratio_layer()
			{
				int height = (realtime_link_moe_data.link_travel_time_ratio / visualization_reference.link_travel_time_ratio) * base_height;
				set_bar_height(height);
				_link_travel_time_ratio_layer->Push_Element<Regular_Element>((void*)&bar);
			}
			
			static bool submit_attributes(Antares_Link_Implementation* _this,vector<string>& bucket)
			{
				// not supported yet
				return true;
			}

			static bool fetch_attributes(Antares_Link_Implementation* _this,vector<string>& bucket)
			{
				_link_moe_plot->Clear_Accented<NT>();

#pragma pack(push,1)
				struct Plot_Element
				{
					int num_primitives;
					Point_2D<MasterType>* points;
				};
#pragma pack(pop)

				Plot_Element element;

				element.num_primitives = _this->_link_density_cache.size();
				element.points = &_this->_link_density_cache.front();

				_link_moe_plot->Push_Element<Accented_Element>((void*)&element);

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
				sprintf(str_buf, "%.2f minutes", _this->realtime_link_moe_data.link_travel_delay);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f MPH", _this->realtime_link_moe_data.link_speed);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f VPMPL", _this->realtime_link_moe_data.link_density);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f VPHPL", _this->realtime_link_moe_data.link_in_flow_rate);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f VPHPL", _this->realtime_link_moe_data.link_out_flow_rate);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_travel_time_ratio);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_speed_ratio);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_density_ratio);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_in_flow_ratio);
				bucket.push_back(str_buf);
				sprintf(str_buf, "%.2f", _this->realtime_link_moe_data.link_out_flow_ratio);
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


			member_data(vector<Point_2D<MasterType>>,link_density_cache,none,none);
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
		Antares_Layer<typename type_of(MasterType::antares_layer),Antares_Link_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::_link_moe_plot;

		template<typename MasterType,typename ParentType,typename InheritanceList>
		Link_MOE_Data Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::visualization_reference;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		int Antares_Link_Implementation<MasterType,ParentType,InheritanceList>::base_height = 100;
	}
}

using namespace Link_Components::Implementations;