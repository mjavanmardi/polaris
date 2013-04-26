//*********************************************************
//	Antares_Vehicle_Implementation.h - Graphical Vehicle Variant
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Vehicle_Prototype.h"

namespace Vehicle_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{

	}
	
	namespace Implementations
	{
#pragma pack(push,1)
		struct Vehicle_Attribute_Shape
		{
			static const float _vehicle_rear_width;
			static const float _vehicle_front_width;
			static const float _vehicle_length;
			void* ptr;
			True_Color_RGBA<NT> color;
			Point_3D<NT> a;
			Point_3D<NT> b;
			Point_3D<NT> c;
			Point_3D<NT> d;
		};
#pragma pack(pop)
		const float Vehicle_Attribute_Shape::_vehicle_rear_width = 7 * 6; // rear width in feet
		const float Vehicle_Attribute_Shape::_vehicle_front_width = 2 * 6; // front width in feet
		const float Vehicle_Attribute_Shape::_vehicle_length = 13.5 * 6; // length in feet

		implementation struct Antares_Vehicle_Implementation:public Polaris_Vehicle_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Vehicle_Implementation)>
		{
			Vehicle_Attribute_Shape vehicle_shape;

			static vector<Point_2D<MasterType>> _num_vehicles_cache;
			
			static member_prototype(Antares_Layer,num_vehicles,typename type_of(MasterType::antares_layer),none,none);

			static volatile member_data(int,vehicles_counter,none,none);

			static member_data(typename MasterType::vehicle_type*, route_displayed_vehicle, none, none);

			bool route_being_displayed;

			static void Initialize_Layer()
			{
				Initialize_Vehicle_Shapes_Layer();
				Initialize_Vehicle_Points_Layer();
				Initialize_Vehicle_Routes_Layer();
			}

			static void Initialize_Vehicle_Shapes_Layer()
			{
				_vehicle_shapes=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(vehicle_shapes),Antares_Vehicle_Implementation>*, string& > >(string("Vehicles (shape)"));
				Antares_Layer_Configuration cfg;
				cfg.dynamic_data=true;
				cfg.target_sub_iteration=Scenario_Components::Types::END_OF_ITERATION+1;
				cfg.storage_offset=_iteration;
				cfg.storage_size=3;
				cfg.storage_period=1;

				cfg.primitive_type=_QUAD;
				cfg.primitive_color=true;

				cfg.selection_callback = &on_select_shape;
				//cfg.attributes_callback = (attributes_callback_type)&fetch_attributes;
				//cfg.submission_callback = (attributes_callback_type)&submit_attributes;

				_vehicle_shapes->Initialize<NULLTYPE>(cfg);
			}

			static void Initialize_Vehicle_Points_Layer()
			{
				_vehicle_points=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(vehicle_points),Antares_Vehicle_Implementation>*, string& > >(string("Vehicles (point)"));
				Antares_Layer_Configuration cfg;
				cfg.Configure_Points();
				cfg.primitive_color=true;
				cfg.head_accent_size_value=6;

				//cfg.attributes_schema = string("ID,Status,Current_Link");
				
				//cfg.attributes_schema.push_back("ID");
				//cfg.attributes_schema.push_back("Status");
				//cfg.attributes_schema.push_back("Current_Link");
				
				cfg.selection_callback = &on_select_point;
				//cfg.attributes_callback = (attributes_callback_type)&fetch_attributes;
				//cfg.submission_callback = (attributes_callback_type)&submit_attributes;

				_vehicle_points->Initialize<NULLTYPE>(cfg);
			}
			
			static void Initialize_Vehicle_Routes_Layer()
			{
				_routes_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(routes_layer),Antares_Vehicle_Implementation>*, string& > >(string("Routes"));
				Antares_Layer_Configuration cfg;
				cfg.Configure_Lines();
				cfg.primitive_color=true;
				cfg.head_accent_size_value=4;

				_routes_layer->Initialize<NULLTYPE>(cfg); 
				//_route_displayed_vehicle = nullptr;
			}

			define_component_interface(Link_Interface,typename type_of(MasterType::link),Link_Prototype,Antares_Vehicle_Implementation);
			define_component_interface(Intersection_Interface,typename type_of(MasterType::intersection),Intersection_Prototype,Antares_Vehicle_Implementation);

			declare_feature_conditional(compute_vehicle_position_condition)
			{
				if (((Antares_Vehicle_Implementation*)_this)->simulation_status<ComponentType,ComponentType,Types::Vehicle_Status_Keys>() != Types::Vehicle_Status_Keys::OUT_NETWORK)
				{
					response.result=true;
					response.next._iteration=_iteration+1;
					response.next._sub_iteration=Scenario_Components::Types::END_OF_ITERATION;
				}
				else
				{
					response.result = false;
					response.next._iteration=END;
				}
			}

			declare_feature_event(compute_vehicle_position)
			{
				Antares_Vehicle_Implementation* pthis=(Antares_Vehicle_Implementation*)_this;
				if (pthis->simulation_status<ComponentType,ComponentType,Types::Vehicle_Status_Keys>() != Types::Vehicle_Status_Keys::IN_NETWORK) return;
				pthis->update_vehicle_position<ComponentType,ComponentType,NT>();
				if (_vehicle_shapes->template draw<bool>() || _vehicle_points->template draw<bool>())
				{
					pthis->display_vehicle_position<ComponentType,ComponentType,NT>();
				}
			}

			feature_implementation void update_vehicle_position()
			{
				float travel_distance = _local_speed * 5280.0f / 3600.0f;
				float current_distance = _distance_to_stop_bar;
				float new_distance = max(0.0f,(current_distance - travel_distance));
				_distance_to_stop_bar = new_distance; 
			}

			feature_implementation void display_vehicle_position()
			{
				typedef Movement_Plan_Components::Prototypes::Movement_Plan_Prototype<typename MasterType::movement_plan_type,ComponentType> _Movement_Plan_Interface;
				Link_Interface* link=((_Movement_Plan_Interface*)_movement_plan)->template current_link<Link_Interface*>();
				Link_Line<MasterType>& link_line = link->template displayed_line<Link_Line<MasterType>&>();
				float u_x = link_line.up_node._x;
				float u_y = link_line.up_node._y;
				float d_x = link_line.down_node._x;
				float d_y = link_line.down_node._y;
				float distance = sqrt((u_x - d_x) * (u_x - d_x) + (u_y - d_y) * (u_y - d_y));
				float sin_alpha = (d_y - u_y) / distance;
				float cos_alpha = (d_x - u_x) / distance;
				//float distance_from_up = link->length<float>() - pthis->_distance_to_stop_bar;
				float distance_from_up = distance - _distance_to_stop_bar * (distance / link->length<float>());
				Point_3D<MasterType> vehicle_center;

				vehicle_center._x = u_x + distance_from_up * cos_alpha;
				vehicle_center._y = u_y + distance_from_up * sin_alpha;
				
				float los = ((MasterType::link_type*)link)->realtime_link_moe_data.link_density / ((MasterType::link_type*)link)->_jam_density;
				
				if (_vehicle_shapes->template draw<bool>())
				{
					vehicle_shape.ptr = this;
					// Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(vehicle_center);

					// display on shape vehicle layer
					float rear_x = vehicle_center._x - (Vehicle_Attribute_Shape::_vehicle_length / 2.0f) * cos_alpha;
					float rear_y = vehicle_center._y - (Vehicle_Attribute_Shape::_vehicle_length / 2.0f) * sin_alpha;
					float front_x = vehicle_center._x + (Vehicle_Attribute_Shape::_vehicle_length / 2.0f) * cos_alpha;
					float front_y = vehicle_center._y + (Vehicle_Attribute_Shape::_vehicle_length / 2.0f) * sin_alpha;

					vehicle_shape.a._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
					vehicle_shape.a._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
					vehicle_shape.a._z = 1;
				
					vehicle_shape.b._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
					vehicle_shape.b._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
					vehicle_shape.a._z = 1;

					vehicle_shape.c._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
					vehicle_shape.c._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
					vehicle_shape.c._z = 1;

					vehicle_shape.d._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
					vehicle_shape.d._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
					vehicle_shape.d._z = 1;


					vehicle_shape.color = ((MasterType::link_type*)link)->get_color_by_los(los);
					vehicle_shape.color._a = 255;
				
					_vehicle_shapes->Push_Element<Regular_Element>(&vehicle_shape);
				}

				if (_vehicle_points->template draw<bool>())
				{
					// display on point vehicle layer
	#pragma pack(push,1)
					struct attribute_coordinate
					{
						void* ptr;
						True_Color_RGBA<NT> color;
						Point_3D<MasterType> vertex;
					} coordinate;
	#pragma pack(pop)
					coordinate.ptr = this;
					coordinate.vertex._x=vehicle_center._x;//upstream_intersection->x_position<float>();
					coordinate.vertex._y=vehicle_center._y;//upstream_intersection->y_position<float>();
					coordinate.vertex._z=1;
					//coordinate.color = ((MasterType::link_type*)link)->get_color_by_los(los);
					//
					int num_switch_decisions = (int)_switch_decisions_container.size();
					if (num_switch_decisions > 0)
					{
						if (num_switch_decisions == 1)
						{
							coordinate.color._r = 191;
							coordinate.color._g = 0;
							coordinate.color._b = 255;
						}
						else
						{
							coordinate.color._r = 102;
							coordinate.color._g = 0;
							coordinate.color._b = 153;
						}
					}
					else
					{
						coordinate.color = ((MasterType::link_type*)link)->get_color_by_los(los);
					}
					_vehicle_points->Push_Element<Regular_Element>(&coordinate);
				}
			}

			//feature_implementation void load(requires(check_2(TargetType,Types::Load_To_Origin_Link,is_same)))
			//{
			//	((Polaris_Vehicle_Implementation<MasterType,ParentType,APPEND_CHILD(Polaris_Vehicle_Implementation)>*)this)->load<ComponentType,CallerType,Types::Load_To_Origin_Link>();
			//	Load_Register<Antares_Vehicle_Implementation>(&compute_vehicle_position_condition<NULLTYPE>,&compute_vehicle_position<NULLTYPE>,_iteration+1,Scenario_Components::Types::END_OF_ITERATION);
			//}

			feature_implementation void load_register()
			{
				route_being_displayed = false;
				Load_Event<Antares_Vehicle_Implementation>(&compute_vehicle_position_condition<NULLTYPE>,&compute_vehicle_position<NULLTYPE>,_iteration+1,Scenario_Components::Types::END_OF_ITERATION);
			}

			static void on_select_point(const list<void*>& removed,const list<void*>& added,const list<void*>& selected,vector<pair<string,string>>& bucket)
			{
				if(removed.size())
				{
					//Select removed one or more objects

					(type_of(MasterType::vehicle)::_vehicle_points)->Clear_Accented<NT>();
					(type_of(MasterType::vehicle)::_routes_layer)->Clear_Accented<NT>();

					if(selected.size())
					{
						for(list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
						{
							((ComponentType*)*itr)->Accent_Self_Point<ComponentType,ComponentType,NT>();
						}
					}
				}
				else if(added.size())
				{
					//Select added new object

					for(list<void*>::const_iterator itr=added.begin();itr!=added.end();itr++)
					{
						((ComponentType*)*itr)->Accent_Self_Point<ComponentType,ComponentType,NT>();
					}
				}
				else
				{
					//Refreshing on time step

					(type_of(MasterType::vehicle)::_vehicle_points)->Clear_Accented<NT>();
					(type_of(MasterType::vehicle)::_routes_layer)->Clear_Accented<NT>();

					if(selected.size())
					{
						for(list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
						{
							((ComponentType*)*itr)->Accent_Self_Point<ComponentType,ComponentType,NT>();
						}
					}
				}

				if(selected.size())
				{
					//Have at least one selected

					((ComponentType*) (selected.back()))->Display_Attributes<ComponentType,ComponentType,NT>(bucket);
				}
			}

			static void on_select_shape(const list<void*>& removed,const list<void*>& added,const list<void*>& selected,vector<pair<string,string>>& bucket)
			{
				if(removed.size())
				{
					//Select removed one or more objects

					(type_of(MasterType::vehicle)::_vehicle_shapes)->Clear_Accented<NT>();
					(type_of(MasterType::vehicle)::_routes_layer)->Clear_Accented<NT>();

					if(selected.size())
					{
						for(list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
						{
							((ComponentType*)*itr)->Accent_Self_Point<ComponentType,ComponentType,NT>();
						}
					}
				}
				else if(added.size())
				{
					//Select added new object

					for(list<void*>::const_iterator itr=added.begin();itr!=added.end();itr++)
					{
						((ComponentType*)*itr)->Accent_Self_Shape<ComponentType,ComponentType,NT>();
					}
				}
				else
				{
					//Refreshing on time step

					(type_of(MasterType::vehicle)::_vehicle_shapes)->Clear_Accented<NT>();
					(type_of(MasterType::vehicle)::_routes_layer)->Clear_Accented<NT>();

					if(selected.size())
					{
						for(list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
						{
							((ComponentType*)*itr)->Accent_Self_Shape<ComponentType,ComponentType,NT>();
						}
					}
				}

				if(selected.size())
				{
					//Have at least one selected

					((ComponentType*) (selected.back()))->Display_Attributes<ComponentType,ComponentType,NT>(bucket);
				}
			}

			feature_implementation void Accent_Self_Shape()
			{
				if(simulation_status<ComponentType,ComponentType,Types::Vehicle_Status_Keys>() != Types::Vehicle_Status_Keys::IN_NETWORK) return;

				display_route<typename MasterType::vehicle_type,NT,NT>();

				Vehicle_Attribute_Shape accented_vehicle_shape;

				accented_vehicle_shape.ptr=this;
				accented_vehicle_shape.color._r = 255 - vehicle_shape.color._r;
				accented_vehicle_shape.color._g = 255 - vehicle_shape.color._g;
				accented_vehicle_shape.color._b = 255 - vehicle_shape.color._b;
				accented_vehicle_shape.color._a = 255;

				accented_vehicle_shape.a = vehicle_shape.a;
				accented_vehicle_shape.b = vehicle_shape.b;
				accented_vehicle_shape.c = vehicle_shape.c;
				accented_vehicle_shape.d = vehicle_shape.d;

				_vehicle_shapes->Push_Element<Accented_Element>(&accented_vehicle_shape);
			}

			feature_implementation void Accent_Self_Point()
			{
				if(simulation_status<ComponentType,ComponentType,Types::Vehicle_Status_Keys>() != Types::Vehicle_Status_Keys::IN_NETWORK) return;

				display_route<typename MasterType::vehicle_type,NT,NT>();

				typedef Movement_Plan_Components::Prototypes::Movement_Plan_Prototype<typename MasterType::movement_plan_type,ComponentType> _Movement_Plan_Interface;
				Link_Interface* link=((_Movement_Plan_Interface*)_movement_plan)->template current_link<Link_Interface*>();

				Link_Line<MasterType>& link_line = link->template displayed_line<Link_Line<MasterType>&>();

				float u_x = link_line.up_node._x;
				float u_y = link_line.up_node._y;
				float d_x = link_line.down_node._x;
				float d_y = link_line.down_node._y;
				float distance = sqrt((u_x - d_x) * (u_x - d_x) + (u_y - d_y) * (u_y - d_y));
				float sin_alpha = (d_y - u_y) / distance;
				float cos_alpha = (d_x - u_x) / distance;
				float distance_from_up = distance - _distance_to_stop_bar * (distance / link->length<float>());

				Point_3D<MasterType> vehicle_center;

				vehicle_center._x = u_x + distance_from_up * cos_alpha;
				vehicle_center._y = u_y + distance_from_up * sin_alpha;

#pragma pack(push,1)
				struct attribute_coordinate
				{
					void* ptr;
					True_Color_RGBA<NT> color;
					Point_3D<MasterType> vertex;
				} coordinate;
#pragma pack(pop)
				coordinate.ptr = this;
				coordinate.vertex._x=vehicle_center._x;//upstream_intersection->x_position<float>();
				coordinate.vertex._y=vehicle_center._y;//upstream_intersection->y_position<float>();
				coordinate.vertex._z=1;
				coordinate.color._r=255;
				coordinate.color._g=0;
				coordinate.color._b=255;
				_vehicle_points->Push_Element<Accented_Element>(&coordinate);
			}
			
			feature_implementation void Display_Attributes(vector<pair<string,string>>& bucket)
			{
				typedef Link_Prototype<typename MasterType::link_type> _Link_Interface;
				typedef Movement_Plan_Prototype<typename MasterType::movement_plan_type> _Movement_Plan_Interface;

				pair<string,string> key_value_pair;
				stringstream s;
				char str_buf[128];

				key_value_pair.first="ID";
				s << _internal_id;
				key_value_pair.second = s.str();
				bucket.push_back(key_value_pair);

				key_value_pair.first="origin link";
				_Link_Interface* origin_link=((_Movement_Plan_Interface*)_movement_plan)->template origin<_Link_Interface*>();
				sprintf(str_buf, "%d", origin_link->template uuid<int>());
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="destination link";
				_Link_Interface* destination_link=((_Movement_Plan_Interface*)_movement_plan)->template destination<_Link_Interface*>();
				sprintf(str_buf, "%d", destination_link->template uuid<int>());
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="realtime enroute information";
				if (_enroute_information_type == Vehicle_Components::Types::Enroute_Information_Keys::WITH_REALTIME_INFORMATION)
				{
					sprintf(str_buf, "yes");
				}
				else
				{
					sprintf(str_buf, "no");
				}
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);				

				key_value_pair.first="information compliance rate";
				sprintf(str_buf, "%.2f", _information_compliance_rate);
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="number of switches";
				sprintf(str_buf, "%d", (int)_switch_decisions_container.size());
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);

				key_value_pair.first="estimated time of arrival";
				string time_str = convert_seconds_to_hhmm(((_Movement_Plan_Interface*)_movement_plan)->template estimated_time_of_arrival<int>());
				sprintf(str_buf, "%s", time_str.c_str());
				key_value_pair.second=str_buf;				
				memset(&str_buf[0],0,128);
				bucket.push_back(key_value_pair);
			}

			//static bool fetch_attributes(Antares_Vehicle_Implementation* _this,vector<string>& bucket)
			//{
			//	//_this->_graphical_network->accent_num_vehicles<NT>();
			//	typedef Movement_Plan_Components::Prototypes::Movement_Plan_Prototype<typename MasterType::movement_plan_type,ComponentType> _Movement_Plan_Interface;
			//	stringstream s;
			//	
			//	s << _this->_internal_id;
			//	bucket.push_back(s.str());
			//	s.str("");
			//	
			//	bucket.push_back(string("IN_NETWORK"));
			//	
			//	s << ((_Movement_Plan_Interface*)_this->_movement_plan)->template current_link<Link_Prototype<typename MasterType::type_of(link)>*>()->internal_id<int>();
			//	bucket.push_back(s.str());

			//	if (!_this->route_being_displayed)
			//	{
			//		_this->display_route<typename MasterType::vehicle_type,NT,NT>();
			//		_this->route_being_displayed = true;
			//		if (_route_displayed_vehicle != nullptr)
			//		{
			//			_route_displayed_vehicle->route_being_displayed = false;
			//		}
			//		_route_displayed_vehicle = _this;
			//	}
			//	return true;
			//}

			feature_implementation void display_route()
			{
				define_component_interface(_Movement_Plan_Interface, type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, _Movement_Plan_Interface::get_type_of(trajectory_container), Back_Insertion_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				define_component_interface(_Link_In_Trajectory_Interface, _Trajectory_Unit_Interface::get_type_of(link), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_component_interface(_Intersection_Interface, _Link_In_Trajectory_Interface::get_type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface_unqualified_container(_Switch_Decision_Data_Container_Interface, _Switch_Decision_Data_Interface, typename type_of(switch_decisions_container), Random_Access_Sequence_Prototype, Switch_Decision_Data_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Switch_Decision_Data_Interface::get_type_of(route_links_container), Random_Access_Sequence_Prototype, Link_Prototype, ComponentType);

				//_routes_layer->Clear_Accented<NT>();

#pragma pack(push,1)
				struct Link_Line
				{
					True_Color_RGBA<NT> color;
					Point_3D<MasterType> up_node;
					Point_3D<MasterType> down_node;
				} link_line;
#pragma pack(pop)
				for(typename _Trajectory_Container_Interface::iterator itr = _movement_plan->_trajectory_container.begin(); itr != _movement_plan->_trajectory_container.end(); itr++)
				{
					_Trajectory_Unit_Interface* vehicle_trajectory_data=(_Trajectory_Unit_Interface*)(*itr);
					_Link_In_Trajectory_Interface* link = vehicle_trajectory_data->template link<_Link_In_Trajectory_Interface*>();
					
					link_line.up_node._x = link->template upstream_intersection<_Intersection_Interface*>()->template x_position<float>();
					link_line.up_node._y = link->template upstream_intersection<_Intersection_Interface*>()->template y_position<float>();
					link_line.up_node._z = link->template upstream_intersection<_Intersection_Interface*>()->template z_position<float>();

					Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(link_line.up_node);

					link_line.down_node._x = link->template downstream_intersection<_Intersection_Interface*>()->template x_position<float>();
					link_line.down_node._y = link->template downstream_intersection<_Intersection_Interface*>()->template y_position<float>();
					link_line.down_node._z = link->template downstream_intersection<_Intersection_Interface*>()->template z_position<float>();
					Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(link_line.down_node);
					
					link_line.color._r = 0;
					link_line.color._g = 0;
					link_line.color._b = 255;
					link_line.color._a = 128;
					_routes_layer->template Push_Element<Accented_Element>(&link_line);
				}

				_Switch_Decision_Data_Container_Interface::reverse_iterator switch_decision_data_itr;
				for (switch_decision_data_itr = _switch_decisions_container.rbegin(); switch_decision_data_itr != _switch_decisions_container.rend(); switch_decision_data_itr++)
				{
					_Switch_Decision_Data_Interface* switch_decision_data = (_Switch_Decision_Data_Interface*)(*switch_decision_data_itr);
					_Links_Container_Interface* links_container = switch_decision_data->template route_links_container<_Links_Container_Interface*>();
					_Links_Container_Interface::iterator link_itr;
					
					for (link_itr = links_container->begin(); link_itr != links_container->end(); link_itr++)
					{
						_Link_Interface* link = (_Link_Interface*)(*link_itr);
						link_line.up_node._x = link->template upstream_intersection<_Intersection_Interface*>()->template x_position<float>();
						link_line.up_node._y = link->template upstream_intersection<_Intersection_Interface*>()->template y_position<float>();
						link_line.up_node._z = link->template upstream_intersection<_Intersection_Interface*>()->template z_position<float>();

						Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(link_line.up_node);

						link_line.down_node._x = link->template downstream_intersection<_Intersection_Interface*>()->template x_position<float>();
						link_line.down_node._y = link->template downstream_intersection<_Intersection_Interface*>()->template y_position<float>();
						link_line.down_node._z = link->template downstream_intersection<_Intersection_Interface*>()->template z_position<float>();
						Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(link_line.down_node);
					
						link_line.color._r = 0;
						link_line.color._g = 0;
						link_line.color._b = max(0, link_line.color._b - 50);
						link_line.color._a = 128;
						_routes_layer->template Push_Element<Accented_Element>(&link_line);
					}
				}

			}
			
			//static bool submit_attributes(Antares_Vehicle_Implementation* _this,vector<string>& bucket)
			//{
			//	vector<string>::iterator itr;

			//	int new_id=atoi(bucket[0].c_str());

			//	if(new_id%2==0)
			//	{
			//		_this->_internal_id = new_id;

			//		return true;
			//	}
			//	else
			//	{
			//		return false;
			//	}
			//}

			static member_prototype(Antares_Layer,vehicle_shapes,typename type_of(MasterType::antares_layer),none,none);
			static member_prototype(Antares_Layer,vehicle_points,typename type_of(MasterType::antares_layer),none,none);
			static member_prototype(Antares_Layer,routes_layer,typename type_of(MasterType::antares_layer),none,none);

		};

		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename MasterType::type_of(antares_layer),Antares_Vehicle_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Vehicle_Implementation<MasterType,ParentType,InheritanceList>::_vehicle_shapes;
		
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename MasterType::type_of(antares_layer),Antares_Vehicle_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Vehicle_Implementation<MasterType,ParentType,InheritanceList>::_vehicle_points;

		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename MasterType::type_of(antares_layer),Antares_Vehicle_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Vehicle_Implementation<MasterType,ParentType,InheritanceList>::_routes_layer;

		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename MasterType::type_of(antares_layer),Antares_Vehicle_Implementation<MasterType,ParentType,InheritanceList>>* Antares_Vehicle_Implementation<MasterType,ParentType,InheritanceList>::_num_vehicles;

		template<typename MasterType,typename ParentType,typename InheritanceList>
		volatile int Antares_Vehicle_Implementation<MasterType,ParentType,InheritanceList>::_vehicles_counter;

		template<typename MasterType,typename ParentType,typename InheritanceList>
		vector<Point_2D<MasterType>> Antares_Vehicle_Implementation<MasterType,ParentType,InheritanceList>::_num_vehicles_cache;

		template<typename MasterType,typename ParentType,typename InheritanceList>
		typename MasterType::vehicle_type* Antares_Vehicle_Implementation<MasterType,ParentType,InheritanceList>::_route_displayed_vehicle;
	}

}

using namespace Vehicle_Components::Implementations;