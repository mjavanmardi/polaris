//*********************************************************
//	Graphical_Network_Implementation.h - Graphical Network Variant
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Polaris_Network_Implementation.h"
#include "Canvas.h"
#include "Information_Panel.h"

namespace Network_Components
{
	namespace Implementations
	{
		//---------------------------------------------------------
		//	Antares_Network_Implementation - network class definition
		//---------------------------------------------------------
#ifdef IntegratedModelApplication
		implementation struct Antares_Network_Implementation:public Integrated_Polaris_Network_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Network_Implementation)>
#else
		implementation struct Antares_Network_Implementation:public Polaris_Network_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Network_Implementation)>
#endif
		{
			vector<Point_2D<MasterType>> _network_vmt_cache;
			vector<Point_2D<MasterType>> _network_vht_cache;
			vector<Point_2D<MasterType>> _network_cumulative_loaded_vehicles_cache;
			vector<Point_2D<MasterType>> _network_cumulative_departed_vehicles_cache;
			vector<Point_2D<MasterType>> _network_in_network_vehicles_cache;
			vector<Point_2D<MasterType>> _network_cumulative_arrived_vehicles_cache;

			vector<Point_2D<MasterType>> _network_avg_link_travel_time_cache;
			vector<Point_2D<MasterType>> _network_avg_link_speed_cache;
			vector<Point_2D<MasterType>> _network_avg_link_density_cache;
			vector<Point_2D<MasterType>> _network_avg_link_in_volume_cache;
			vector<Point_2D<MasterType>> _network_avg_link_out_volume_cache;
			vector<Point_2D<MasterType>> _network_avg_link_travel_time_ratio_cache;
			vector<Point_2D<MasterType>> _network_avg_link_speed_ratio_cache;
			vector<Point_2D<MasterType>> _network_avg_link_density_ratio_cache;
			vector<Point_2D<MasterType>> _network_avg_link_queue_length_cache;

			member_prototype(Antares_Layer,network_vmt_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,network_vht_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,network_cumulative_loaded_vehicles_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,network_cumulative_departed_vehicles_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,network_in_network_vehicles_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,network_cumulative_arrived_vehicles_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,network_cumulative_combined_layer,typename type_of(MasterType::antares_layer),none,none);

			member_prototype(Antares_Layer,network_avg_link_travel_time_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,network_avg_link_speed_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,network_avg_link_density_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,network_avg_link_in_volume_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,network_avg_link_out_volume_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,network_avg_link_travel_time_ratio_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,network_avg_link_speed_ratio_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,network_avg_link_density_ratio_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,network_avg_link_queue_length_layer,typename type_of(MasterType::antares_layer),none,none);

			feature_implementation void set_network_bounds()
			{
				_network_bounds.reset<type_of(network_bounds),Antares_Network_Implementation,NULLTYPE>();
				typename _Intersections_Container_Interface::iterator intersection_itr;
				for(intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersection_itr);

					if(intersection->template x_position<float>() < _network_bounds._xmin)
					{
						_network_bounds._xmin = intersection->template x_position<float>();
					}
					else if(intersection->template x_position<float>() > _network_bounds._xmax)
					{
						_network_bounds._xmax = intersection->template x_position<float>();
					}
					
					if(intersection->template y_position<float>() < _network_bounds._ymin)
					{
						_network_bounds._ymin = intersection->template y_position<float>();
					}
					else if(intersection->template y_position<float>() > _network_bounds._ymax)
					{
						_network_bounds._ymax = intersection->template y_position<float>();
					}
				}
			}

			feature_implementation void initialize_antares_layers()
			{
				initialize_network_moe_plotting_layers<ComponentType,CallerType,TargetType>();
				initialize_link_map_layers<ComponentType,CallerType,TargetType>();
			}

			feature_implementation void initialize_network_moe_plotting_layers()
			{
				// configure plot layer
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;

				Antares_Layer_Configuration pcfg;

				pcfg.Configure_Plot();
				pcfg.storage_period = ((_Scenario_Interface*)_global_scenario)->simulation_interval_length<int>();
				pcfg.storage_offset = ((_Scenario_Interface*)_global_scenario)->simulation_interval_length<int>() - 1;
				pcfg.x_label = "time (second)";
				
				pcfg.y_label = "vechile-miles traveled";
				_network_vmt_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_vmt_layer),Antares_Network_Implementation>*, string& > >(string("VMT"));
				_network_vmt_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "vechiles-hours traveled";
				_network_vht_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_vht_layer),Antares_Network_Implementation>*, string& > >(string("VHT"));
				_network_vht_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "cumulative loaded vehicles";
				_network_cumulative_loaded_vehicles_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_cumulative_loaded_vehicles_layer),Antares_Network_Implementation>*, string& > >(string("cumulative loaded"));
				_network_cumulative_loaded_vehicles_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "cumulative departed vehicles";
				_network_cumulative_departed_vehicles_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_cumulative_departed_vehicles_layer),Antares_Network_Implementation>*, string& > >(string("cumulative departed"));
				_network_cumulative_departed_vehicles_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "cumulative arrived vehicles";
				_network_cumulative_arrived_vehicles_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_cumulative_arrived_vehicles_layer),Antares_Network_Implementation>*, string& > >(string("cumulative arrived"));
				_network_cumulative_arrived_vehicles_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "number of vehicles";
				_network_cumulative_combined_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_cumulative_combined_layer),Antares_Network_Implementation>*, string& > >(string("cumulative curves"));
				_network_cumulative_combined_layer->Initialize<NULLTYPE>(pcfg);
				
				pcfg.y_label = "in-network vehicles";
				_network_in_network_vehicles_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_in_network_vehicles_layer),Antares_Network_Implementation>*, string& > >(string("in-network"));
				_network_in_network_vehicles_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "link travel time (minute)";
				_network_avg_link_travel_time_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_avg_link_travel_time_layer),Antares_Network_Implementation>*, string& > >(string("avg link travel time"));
				_network_avg_link_travel_time_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "link speed (miles/hour)";
				_network_avg_link_speed_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_avg_link_speed_layer),Antares_Network_Implementation>*, string& > >(string("avg link speed"));
				_network_avg_link_speed_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "link density (vehiles/mile/lane)";
				_network_avg_link_density_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_avg_link_density_layer),Antares_Network_Implementation>*, string& > >(string("avg link density"));
				_network_avg_link_density_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "link travel time ratio";
				_network_avg_link_travel_time_ratio_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_avg_link_travel_time_ratio_layer),Antares_Network_Implementation>*, string& > >(string("avg link travel time ratio"));
				_network_avg_link_travel_time_ratio_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "link speed ratio";
				_network_avg_link_speed_ratio_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_avg_link_speed_ratio_layer),Antares_Network_Implementation>*, string& > >(string("avg link speed ratio"));
				_network_avg_link_speed_ratio_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "link density ratio";
				_network_avg_link_density_ratio_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_avg_link_density_ratio_layer),Antares_Network_Implementation>*, string& > >(string("avg link density ratio"));
				_network_avg_link_density_ratio_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "link queue length ratio";
				_network_avg_link_queue_length_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_avg_link_queue_length_layer),Antares_Network_Implementation>*, string& > >(string("avg link queue length"));
				_network_avg_link_queue_length_layer->Initialize<NULLTYPE>(pcfg);
			}

			feature_implementation void initialize_link_map_layers()
			{
				
				_link_lines=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_lines),Antares_Network_Implementation>*, string& > >(string("Links"));

				Antares_Layer_Configuration cfg;
				cfg.Configure_Lines();
				cfg.draw=true;
				cfg.primitive_color=true;

				//cfg.head_size_value = 
				cfg.attributes_callback = (attributes_callback_type)&MasterType::type_of(link)::fetch_attributes;
				//cfg.attributes_schema = string("Id,Type,Travel time,Travel delay,Speed,Density,In-flow rate,Out-flow rate,Travel time ratio,Speed ratio,Density ratio,In-flow ratio,Out-flow ratio");

				cfg.attributes_schema.push_back("Id");
				cfg.attributes_schema.push_back("Type");
				cfg.attributes_schema.push_back("Travel time");
				cfg.attributes_schema.push_back("Travel delay");
				cfg.attributes_schema.push_back("Speed");
				cfg.attributes_schema.push_back("Density");
				cfg.attributes_schema.push_back("In-flow rate");
				cfg.attributes_schema.push_back("Out-flow rate");
				cfg.attributes_schema.push_back("Travel time ratio");
				cfg.attributes_schema.push_back("Speed ratio");
				cfg.attributes_schema.push_back("Density ratio");
				cfg.attributes_schema.push_back("In-flow ratio");
				cfg.attributes_schema.push_back("Out-flow ratio");


				_link_lines->Initialize<NULLTYPE>(cfg);
				struct Link_Line
				{
					void* data;
					True_Color_RGBA<NT> color;
					Point_3D<MasterType> a;
					Point_3D<MasterType> b;
				} current_line;

				typename _Links_Container_Interface::iterator link_itr;

				for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
				{
					_Link_Interface* link = (_Link_Interface*)(*link_itr);

					current_line.color = get_color_by_type(link->template link_type<int>());

					current_line.a._x = link->upstream_intersection<_Intersection_Interface*>()->x_position<float>();
					current_line.a._y = link->upstream_intersection<_Intersection_Interface*>()->y_position<float>();
					current_line.a._z = 0;

					Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(current_line.a);

					current_line.b._x = link->downstream_intersection<_Intersection_Interface*>()->x_position<float>();
					current_line.b._y = link->downstream_intersection<_Intersection_Interface*>()->y_position<float>();
					current_line.b._z = 0;

					Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(current_line.b);

					current_line.data = (void*)(*link_itr);
					_link_lines->Push_Element<Regular_Element>(&current_line);
				}
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
			
			feature_implementation void printResults() 
			{
				((Polaris_Network_Implementation<MasterType,ParentType,APPEND_CHILD(Integrated_Polaris_Network_Implementation)>*)this)->printResults<ComponentType,CallerType,TargetType>();
				typename _Links_Container_Interface::iterator link_itr;
				for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
				{
					_Link_Interface* link = (_Link_Interface*)(*link_itr);
					link->visualize_moe<NULLTYPE>();
				}

				plot_network_moes();
			}

			void plot_vehicle_number()
			{
#pragma pack(push,1)
				struct Plot_Element
				{
					int num_primitives;
					Point_2D<MasterType>* points;
				};
#pragma pack(pop)
				Point_2D<MasterType> submission;
				submission._x=_iteration;
				submission._y=MasterType::vehicle_type::_vehicles_counter;

				MasterType::vehicle_type::_num_vehicles_cache.push_back(submission);

				Plot_Element element;

				element.num_primitives = MasterType::vehicle_type::_num_vehicles_cache.size();
				element.points = &MasterType::vehicle_type::_num_vehicles_cache.front();

				MasterType::vehicle_type::_num_vehicles->Push_Element<Regular_Element>((void*)&element);

				MasterType::vehicle_type::_vehicles_counter=0;
			}

			void plot_network_moes()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
#pragma pack(push,1)
				struct Plot_Element
				{
					int num_primitives;
					Point_2D<MasterType>* points;
				};
#pragma pack(pop)
				Point_2D<MasterType> submission;
				Plot_Element element;
				submission._x=_iteration;

				// plot vmt
				submission._y=_network_vmt;
				_network_vmt_cache.push_back(submission);
				element.num_primitives = _network_vmt_cache.size();
				element.points = &_network_vmt_cache.front();
				_network_vmt_layer->Push_Element<Regular_Element>((void*)&element);

				// plot vht
				submission._y=_network_vht;
				_network_vht_cache.push_back(submission);
				element.num_primitives = _network_vht_cache.size();
				element.points = &_network_vht_cache.front();
				_network_vht_layer->Push_Element<Regular_Element>((void*)&element);

				// plot loaded vehicles
				submission._y=((_Scenario_Interface*)_global_scenario)->template network_cumulative_loaded_vehicles<int>();
				_network_cumulative_loaded_vehicles_cache.push_back(submission);
				element.num_primitives = _network_cumulative_loaded_vehicles_cache.size();
				element.points = &_network_cumulative_loaded_vehicles_cache.front();
				_network_cumulative_loaded_vehicles_layer->Push_Element<Regular_Element>((void*)&element);

				// plot departed vehicles
				submission._y=((_Scenario_Interface*)_global_scenario)->template network_cumulative_departed_vehicles<int>();
				_network_cumulative_departed_vehicles_cache.push_back(submission);
				element.num_primitives = _network_cumulative_departed_vehicles_cache.size();
				element.points = &_network_cumulative_departed_vehicles_cache.front();
				_network_cumulative_departed_vehicles_layer->Push_Element<Regular_Element>((void*)&element);

				// plot arrived vehicles
				submission._y=((_Scenario_Interface*)_global_scenario)->template network_cumulative_arrived_vehicles<int>();
				_network_cumulative_arrived_vehicles_cache.push_back(submission);
				element.num_primitives = _network_cumulative_arrived_vehicles_cache.size();
				element.points = &_network_cumulative_arrived_vehicles_cache.front();
				_network_cumulative_arrived_vehicles_layer->Push_Element<Regular_Element>((void*)&element);

				// plot in_network vehicles
				submission._y=((_Scenario_Interface*)_global_scenario)->template network_in_network_vehicles<int>();
				_network_in_network_vehicles_cache.push_back(submission);
				element.num_primitives = _network_in_network_vehicles_cache.size();
				element.points = &_network_in_network_vehicles_cache.front();
				_network_in_network_vehicles_layer->Push_Element<Regular_Element>((void*)&element);

				// plot cumulative combined
				element.num_primitives = _network_cumulative_loaded_vehicles_cache.size();
				element.points = &_network_cumulative_loaded_vehicles_cache.front();
				_network_cumulative_combined_layer->Push_Element<Regular_Element>((void*)&element);
				Plot_Element element2;
				
				element2.num_primitives = _network_cumulative_arrived_vehicles_cache.size();
				element2.points = &_network_cumulative_arrived_vehicles_cache.front();
				_network_cumulative_combined_layer->Push_Element<Regular_Element>((void*)&element2);

				// plot network_avg_link_travel_time
				submission._y=realtime_network_moe_data.network_avg_link_travel_time;
				_network_avg_link_travel_time_cache.push_back(submission);
				element.num_primitives = _network_avg_link_travel_time_cache.size();
				element.points = &_network_avg_link_travel_time_cache.front();
				_network_avg_link_travel_time_layer->Push_Element<Regular_Element>((void*)&element);

				// plot _network_avg_link_speed_cache
				submission._y=realtime_network_moe_data.network_avg_link_speed;
				_network_avg_link_speed_cache.push_back(submission);
				element.num_primitives = _network_avg_link_speed_cache.size();
				element.points = &_network_avg_link_speed_cache.front();
				_network_avg_link_speed_layer->Push_Element<Regular_Element>((void*)&element);


				// plot _network_avg_link_density_cache
				submission._y=realtime_network_moe_data.network_avg_link_density;
				_network_avg_link_density_cache.push_back(submission);
				element.num_primitives = _network_avg_link_density_cache.size();
				element.points = &_network_avg_link_density_cache.front();
				_network_avg_link_density_layer->Push_Element<Regular_Element>((void*)&element);

				// plot _network_avg_link_travel_time_ratio
				submission._y=realtime_network_moe_data.network_avg_link_travel_time_ratio;
				_network_avg_link_travel_time_ratio_cache.push_back(submission);
				element.num_primitives = _network_avg_link_travel_time_ratio_cache.size();
				element.points = &_network_avg_link_travel_time_ratio_cache.front();
				_network_avg_link_travel_time_ratio_layer->Push_Element<Regular_Element>((void*)&element);


				// plot _network_avg_link_speed_ratio_cache
				submission._y=realtime_network_moe_data.network_avg_link_speed_ratio;
				_network_avg_link_speed_ratio_cache.push_back(submission);
				element.num_primitives = _network_avg_link_speed_ratio_cache.size();
				element.points = &_network_avg_link_speed_ratio_cache.front();
				_network_avg_link_speed_ratio_layer->Push_Element<Regular_Element>((void*)&element);


				// plot _network_avg_link_density_ratio_cache
				submission._y=realtime_network_moe_data.network_avg_link_density_ratio;
				_network_avg_link_density_ratio_cache.push_back(submission);
				element.num_primitives = _network_avg_link_density_ratio_cache.size();
				element.points = &_network_avg_link_density_ratio_cache.front();
				_network_avg_link_density_ratio_layer->Push_Element<Regular_Element>((void*)&element);

				// plot _network_avg_link_queue_length_cache
				submission._y=realtime_network_moe_data.network_avg_link_queue_length;
				_network_avg_link_queue_length_cache.push_back(submission);
				element.num_primitives = _network_avg_link_queue_length_cache.size();
				element.points = &_network_avg_link_queue_length_cache.front();
				_network_avg_link_queue_length_layer->Push_Element<Regular_Element>((void*)&element);
			}

			member_data(Rectangle_XY<MasterType>, network_bounds,none,none);
			
			member_prototype(Antares_Layer,link_lines,typename type_of(MasterType::antares_layer),none,none);
	
			define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
		};
	}
}

using namespace Network_Components::Implementations;