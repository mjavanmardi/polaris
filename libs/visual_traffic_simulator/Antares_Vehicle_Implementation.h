//**************************************************************************************************
//	Antares_Vehicle_Implementation.h - Graphical Vehicle Variant
//**************************************************************************************************

#pragma once
#include "Dependencies.h"
#include <sstream>
#include "Traffic_Simulator\Vehicle_Implementation.h"



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
		struct Vehicle_Attribute_Primitive
		{
			True_Color_RGBA<NT> color;
			//int texture_index;
			Point_3D<NT> a;
			Point_3D<NT> b;
			Point_3D<NT> c;
			Point_3D<NT> d;
		};
#pragma pack(pop)

#pragma pack(push,1)
		struct Vehicle_Attribute_Shape
		{
			void Initialize(int input_num_primitives = 10)
			{
				data = new Vehicle_Attribute_Primitive[input_num_primitives];
				num_primitives = input_num_primitives;
			}

			Vehicle_Attribute_Shape()
			{
				data = nullptr;
				//data = new Vehicle_Attribute_Primitive[num_primitives];
				//num_primitives = num_primitives;
			}

			~Vehicle_Attribute_Shape()
			{
				if(data!=nullptr) delete[] data;
			}

			static const float _vehicle_rear_width;
			static const float _vehicle_front_width;
			static const float _vehicle_length;
			static const float _vehicle_height;
			static const float _vehicle_mid_height;

			//static int front_index;
			//static int back_index;
			//static int left_index;
			//static int right_index;
			//static int top_index;

			void* ptr;
			int num_primitives;
			Vehicle_Attribute_Primitive* data;
		};
#pragma pack(pop)
		//int Vehicle_Attribute_Shape::front_index;
		//int Vehicle_Attribute_Shape::back_index;
		//int Vehicle_Attribute_Shape::left_index;
		//int Vehicle_Attribute_Shape::right_index;
		//int Vehicle_Attribute_Shape::top_index;

		//const float Vehicle_Attribute_Shape::_vehicle_rear_width = 6.775 * 3; // rear width in feet
		//const float Vehicle_Attribute_Shape::_vehicle_front_width = 6.775 * 3; // front width in feet
		//const float Vehicle_Attribute_Shape::_vehicle_length = 17 * 3; // length in feet
		//const float Vehicle_Attribute_Shape::_vehicle_height = 6.583 * 3; // height in feet
		//const float Vehicle_Attribute_Shape::_vehicle_mid_height = 6.583*2.0/3.0 * 3; // height in feet

		const float Vehicle_Attribute_Shape_size_factor = 3.0;
		const float Vehicle_Attribute_Shape::_vehicle_rear_width = 6.775 * 2 * Vehicle_Attribute_Shape_size_factor; // rear width in feet
		const float Vehicle_Attribute_Shape::_vehicle_front_width = 6.775 * 2 * Vehicle_Attribute_Shape_size_factor; // front width in feet
		const float Vehicle_Attribute_Shape::_vehicle_length = 17 * 2* Vehicle_Attribute_Shape_size_factor; // length in feet
		const float Vehicle_Attribute_Shape::_vehicle_height = 6.0 * 2* Vehicle_Attribute_Shape_size_factor; // height in feet
		const float Vehicle_Attribute_Shape::_vehicle_mid_height = 6.0*5.0/9.0 * 2* Vehicle_Attribute_Shape_size_factor; // height in feet



		implementation struct Antares_Vehicle_Implementation:public Vehicle_Implementation<MasterType,INHERIT(Antares_Vehicle_Implementation)>
		{
			typedef typename Vehicle_Implementation<MasterType,INHERIT(Antares_Vehicle_Implementation)>::ComponentType ComponentType;
			Vehicle_Attribute_Shape vehicle_shape;
			True_Color_RGBA<NT> vehicle_color;

			static std::vector<Point_2D<MasterType>> _num_vehicles_cache;
		
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,num_vehicles, NONE, NONE);

			static volatile m_data(int,vehicles_counter, NONE, NONE);

			static m_data(typename MasterType::vehicle_type*, route_displayed_vehicle, NONE, NONE);

			bool route_being_displayed;

			static void Initialize_Layer()
			{
				Initialize_Vehicle_Shapes_Layer();
				Initialize_Vehicle_Points_Layer();
				Initialize_Vehicle_Routes_Layer();
				Initialize_Vehicle_Locations_Layer();
			}

			static void Initialize_Vehicle_Shapes_Layer()
			{
				_vehicle_shapes=Allocate_New_Layer<MT>(string("Vehicles (shape)"));
				Antares_Layer_Configuration cfg;
				cfg.dynamic_data=true;
				cfg.target_sub_iteration=Scenario_Components::Types::END_OF_ITERATION+1;
				cfg.storage_offset=iteration();
				cfg.storage_size=4;
				cfg.storage_period=1;
				cfg.grouped=true;
				//cfg.primitive_texture = true;
				cfg.primitive_normal = true;

				//Vehicle_Attribute_Shape::front_index = cfg.Add_Texture(string("Front_Alpha.png"));
				//Vehicle_Attribute_Shape::back_index = cfg.Add_Texture(string("Back_Alpha.png"));
				//Vehicle_Attribute_Shape::left_index = cfg.Add_Texture(string("Side_Alpha_Left.png"));
				//Vehicle_Attribute_Shape::right_index = cfg.Add_Texture(string("Side_Alpha_Right.png"));
				//Vehicle_Attribute_Shape::top_index = cfg.Add_Texture(string("Top_Alpha.png"));

				//cfg.head_texture = cfg.Add_Texture(string("Car.png"));

				cfg.primitive_type=_QUAD;
				cfg.primitive_color=true;

				cfg.selection_callback = &on_select_shape;
				//cfg.attributes_callback = (attributes_callback_type)&fetch_attributes;
				//cfg.submission_callback = (attributes_callback_type)&submit_attributes;


				_vehicle_shapes->Initialize<NULLTYPE>(cfg);
			}

			static float pixel_size_callback(float meter_per_pixel_mid)
			{
				float size = floor((1.57777/meter_per_pixel_mid)*28.0f+6.0f);
				//float size = 10.0f;
				return size;
			}
			
			static float accented_pixel_size_callback(float meter_per_pixel_mid)
			{
				float size = floor((1.57777/meter_per_pixel_mid)*42.0f+8.0f);
				return size;
			}

			static void Initialize_Vehicle_Points_Layer()
			{
				_vehicle_points=Allocate_New_Layer<MT>(string("Vehicles (point)"));
				Antares_Layer_Configuration cfg;
				cfg.Configure_Dynamic_Points();
				cfg.primitive_color=true;
				cfg.head_accent_size_value=10;
				cfg.head_size_value=8;
				cfg.pixel_size_callback = &pixel_size_callback;
				cfg.accent_pixel_size_callback = &accented_pixel_size_callback;
				cfg.head_texture = cfg.Add_Texture(string("C:\\opt\\polarisdeps\\antares\\Car_Front.png"));

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
				_routes_layer=Allocate_New_Layer<MT>(string("Routes"));
				Antares_Layer_Configuration cfg;
				cfg.Configure_Static_Lines();
				cfg.primitive_color=true;
				
				cfg.head_accent_size_value=6;

				_routes_layer->Initialize<NULLTYPE>(cfg); 
				//_route_displayed_vehicle = nullptr;
			}
			
			static void Initialize_Vehicle_Locations_Layer()
			{
				_locations_layer=Allocate_New_Layer<MT>(string("Locations"));
				Antares_Layer_Configuration cfg;
				cfg.primitive_type=_TRIANGLE;
				//cfg.head_accent_size_value=14;
				//cfg.primitive_color=true;
				cfg.grouped=true;
				
				cfg.primitive_normal=true;
				cfg.group_color=true;

				cfg.head_color._r=255;
				cfg.head_color._g=0;
				cfg.head_color._b=0;
				cfg.head_color._a=255;

				_locations_layer->Initialize<NULLTYPE>(cfg);
			}

			typedef Link<typename MasterType::link_type> Link_Interface;
			typedef Intersection<typename MasterType::intersection_type> Intersection_Interface;
			
			static void compute_vehicle_position_condition(ComponentType* _this,Event_Response& response)
			{
				if (((Antares_Vehicle_Implementation*)_this)->simulation_status<Types::Vehicle_Status_Keys>() != Types::Vehicle_Status_Keys::OUT_NETWORK)
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

			//declare_event(compute_vehicle_position)
			void compute_vehicle_position()
			{
				Antares_Vehicle_Implementation* pthis=(Antares_Vehicle_Implementation*)this;
				if (pthis->simulation_status<Types::Vehicle_Status_Keys>() != Types::Vehicle_Status_Keys::IN_NETWORK) return;
				pthis->update_vehicle_position<NT>();
				if (_vehicle_shapes->template draw<bool>() || _vehicle_points->template draw<bool>())
				{
					pthis->display_vehicle_position<NT>();
				}
			}

			template<typename TargetType> void update_vehicle_position()
			{
				float travel_distance = _local_speed * 5280.0f / 3600.0f;
				float current_distance = _distance_to_stop_bar;
				float new_distance = max(0.0f,(current_distance - travel_distance));
				_distance_to_stop_bar = new_distance; 
			}

			template<typename TargetType> void display_vehicle_position()
			{
				typedef Scenario<typename MasterType::scenario_type> Scenario_Interface;
				Scenario_Interface* scenario = (Scenario_Interface*)_global_scenario;

				if(vehicle_shape.data == nullptr)
				{
					if(_internal_id % 10 < 2)
					{
						vehicle_shape.Initialize(10);
					}
					else
					{
						vehicle_shape.Initialize(11);
					}

					// set random vehicle color
					vehicle_color._r = GLOBALS::Uniform_RNG.Next_Rand<float>()*175;
					vehicle_color._g = GLOBALS::Uniform_RNG.Next_Rand<float>()*175;
					vehicle_color._b = GLOBALS::Uniform_RNG.Next_Rand<float>()*175;
				}

				if (_movement_plan == nullptr) return;


				Vehicle_Implementation<MasterType,INHERIT(Antares_Vehicle_Implementation)>* bthis = (Vehicle_Implementation<MasterType,INHERIT(Antares_Vehicle_Implementation)>*)this;
				
				typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type> _Movement_Plan_Interface;

				Link_Interface* link=((_Movement_Plan_Interface*)_movement_plan)->template current_link<Link_Interface*>();

				if (link == nullptr) return;

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
				//True_Color_RGBA<NT> vehicle_color;

				vehicle_center._x = u_x + distance_from_up * cos_alpha;
				vehicle_center._y = u_y + distance_from_up * sin_alpha;
				//vehicle_color._r=0; vehicle_color._g=255; vehicle_color._b=0;
								
				float los = ((MasterType::link_type*)link)->realtime_link_moe_data.link_density / ((MasterType::link_type*)link)->_jam_density;

				typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> _Zone_Interface;
				_Zone_Interface* zone = ((_Movement_Plan_Interface*)_movement_plan)->template destination<_Zone_Interface*>();

			#pragma region VEHICLE SHAPES				
				if (_vehicle_shapes->template draw<bool>())
				{
					True_Color_RGBA<NT> body_color;
					
					int num_switch_decisions = (int)_switch_decisions_container.size();
					
					typedef Activity_Location<typename MasterType::activity_location_type> Activity_Location_Interface;

#ifdef IntegratedModelApplication		
					Person<typename ComponentType::type_of(traveler)>* person=(Person<typename ComponentType::type_of(traveler)>*)_traveler;	
					if(person->has_done_replanning<bool>() && ((ComponentType*)this)->_is_integrated)
					{
						body_color._r = 0;
						body_color._g = 100;
						body_color._b = 255;
					}
					else
#endif
					if (num_switch_decisions > 0)
					{
						if (num_switch_decisions == 1)
						{
							if(_its_switch)
							{
								body_color._r = 255;
								body_color._g = 0;
								body_color._b = 255;
							}
							else
							{
								body_color._r = 150;
								body_color._g = 150;
								body_color._b = 150;
							}
						}
						else
						{
							body_color._r = 175;
							body_color._g = 0;
							body_color._b = 175;
						}
					}
					else
					{
						body_color = ((MasterType::link_type*)link)->get_color_by_los(los);
					}

					if (!((ComponentType*)this)->_is_integrated)
					{
						float color_scale = 0.75;
						body_color._r *= color_scale;
						body_color._g *= color_scale;
						body_color._b *= color_scale;
					}

					if (scenario->color_cars_randomly<bool>())
					{
						body_color._r = vehicle_color._r;
						body_color._g = vehicle_color._g;
						body_color._b = vehicle_color._b;

						// CAV coloration for lakeside model only - remove when done
						//if (((ComponentType*)this)->_is_integrated && zone->uuid<int>() < 47)
						//{
						//	body_color._r = 255;
						//	body_color._g = 255;
						//	body_color._b = 0;
						//}
					}


					if(_internal_id % 10 < 2)
					{
						vehicle_shape.ptr = this;
//TODO
//						// Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(vehicle_center);

						const float size_factor = 1.0f;

						// display on shape vehicle layer
						float rear_x = vehicle_center._x - (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 2.0f) * cos_alpha;
						float rear_y = vehicle_center._y - (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 2.0f) * sin_alpha;
						float front_x = vehicle_center._x + (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 2.0f) * cos_alpha;
						float front_y = vehicle_center._y + (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 2.0f) * sin_alpha;
					
						float rear_windshield_x = vehicle_center._x - (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 2.3f) * cos_alpha;
						float rear_windshield_y = vehicle_center._y - (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 2.3f) * sin_alpha;

						float front_windshield_x = vehicle_center._x + (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 6.0f) * cos_alpha;
						float front_windshield_y = vehicle_center._y + (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 6.0f) * sin_alpha;

						float front_hood_x = vehicle_center._x + (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 4.0f) * cos_alpha;
						float front_hood_y = vehicle_center._y + (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 4.0f) * sin_alpha;

						//vehicle_shape.data[0].texture_index = Vehicle_Attribute_Shape::top_index;
						//vehicle_shape.data[1].texture_index = Vehicle_Attribute_Shape::back_index;
						//vehicle_shape.data[2].texture_index = Vehicle_Attribute_Shape::front_index;
						//vehicle_shape.data[3].texture_index = Vehicle_Attribute_Shape::left_index;
						//vehicle_shape.data[4].texture_index = Vehicle_Attribute_Shape::right_index;

						int counter = 0;

						//top_back
						vehicle_shape.data[counter].color._r = 255;
						vehicle_shape.data[counter].color._b = 255;
						vehicle_shape.data[counter].color._g = 255;
						vehicle_shape.data[counter].color._a = 255;

						vehicle_shape.data[counter].a._x = rear_windshield_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = rear_windshield_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_height*size_factor;

						vehicle_shape.data[counter].b._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;

						vehicle_shape.data[counter].c._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;
					
						vehicle_shape.data[counter].d._x = rear_windshield_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = rear_windshield_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_height*size_factor;
						++counter;

						//top_mid

						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = front_windshield_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_windshield_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_height*size_factor;

						vehicle_shape.data[counter].b._x = rear_windshield_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_windshield_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_height*size_factor;

						vehicle_shape.data[counter].c._x = rear_windshield_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_windshield_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_height*size_factor;
					
						vehicle_shape.data[counter].d._x = front_windshield_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_windshield_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_height*size_factor;
						++counter;


						//top_front
						vehicle_shape.data[counter].color._r = 255;
						vehicle_shape.data[counter].color._b = 255;
						vehicle_shape.data[counter].color._g = 255;
						vehicle_shape.data[counter].color._a = 255;

						vehicle_shape.data[counter].a._x = front_hood_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_hood_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;

						vehicle_shape.data[counter].b._x = front_windshield_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_windshield_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_height*size_factor;

						vehicle_shape.data[counter].c._x = front_windshield_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_windshield_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_height*size_factor;
					
						vehicle_shape.data[counter].d._x = front_hood_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_hood_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;
						++counter;

						//top_hood
						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;

						vehicle_shape.data[counter].b._x = front_hood_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_hood_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;

						vehicle_shape.data[counter].c._x = front_hood_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_hood_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;
					
						vehicle_shape.data[counter].d._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;
						++counter;


						//back
						vehicle_shape.data[counter].color = body_color;
					
						vehicle_shape.data[counter].a._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;
					
						vehicle_shape.data[counter].b._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;


						vehicle_shape.data[counter].c._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = 1;
					
						vehicle_shape.data[counter].d._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = 1;


						++counter;

						//front
						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = 1;

						vehicle_shape.data[counter].b._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = 1;
					
						vehicle_shape.data[counter].c._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;

						vehicle_shape.data[counter].d._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;
						++counter;					
					
					
					
					
						//right_bottom
						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = 1;

						vehicle_shape.data[counter].b._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = 1;
					
						vehicle_shape.data[counter].c._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;

						vehicle_shape.data[counter].d._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;
						++counter;

						//right_top
						vehicle_shape.data[counter].color._r = 255;
						vehicle_shape.data[counter].color._b = 255;
						vehicle_shape.data[counter].color._g = 255;
						vehicle_shape.data[counter].color._a = 255;

						vehicle_shape.data[counter].a._x = front_hood_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_hood_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;

						vehicle_shape.data[counter].b._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;
					
						vehicle_shape.data[counter].c._x = rear_windshield_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_windshield_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_height*size_factor;

						vehicle_shape.data[counter].d._x = front_windshield_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_windshield_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_height*size_factor;
						++counter;

						//left_bottom
						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = 1;

						vehicle_shape.data[counter].b._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = 1;

						vehicle_shape.data[counter].c._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;

						vehicle_shape.data[counter].d._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;
						++counter;
					
						//left_top
						vehicle_shape.data[counter].color._r = 255;
						vehicle_shape.data[counter].color._b = 255;
						vehicle_shape.data[counter].color._g = 255;
						vehicle_shape.data[counter].color._a = 255;

						vehicle_shape.data[counter].a._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;

						vehicle_shape.data[counter].b._x = front_hood_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_hood_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height*size_factor;

						vehicle_shape.data[counter].c._x = front_windshield_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_windshield_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_height*size_factor;

						vehicle_shape.data[counter].d._x = rear_windshield_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = rear_windshield_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_height*size_factor;

						_vehicle_shapes->Push_Element<Regular_Element>(&vehicle_shape);
					}
					else if(_internal_id % 10 < 4)
					{
						const float size_factor = 1.0f;

						vehicle_shape.ptr = this;
//TODO
//						// Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(vehicle_center);

						// display on shape vehicle layer
						float rear_x = vehicle_center._x - (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 2.0f) * cos_alpha;
						float rear_y = vehicle_center._y - (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 2.0f) * sin_alpha;
						float front_x = vehicle_center._x + (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 2.0f) * cos_alpha;
						float front_y = vehicle_center._y + (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 2.0f) * sin_alpha;
						
						//float rear_windshield_x = vehicle_center._x - (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 8.0f) * cos_alpha;
						//float rear_windshield_y = vehicle_center._y - (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 8.0f) * sin_alpha;
						
						float rear_windshield_x = vehicle_center._x;
						float rear_windshield_y = vehicle_center._y;

						float front_windshield_x = vehicle_center._x + (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 4.0f) * cos_alpha;
						float front_windshield_y = vehicle_center._y + (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 4.0f) * sin_alpha;

						float front_hood_x = vehicle_center._x + (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 3.0f) * cos_alpha;
						float front_hood_y = vehicle_center._y + (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 3.0f) * sin_alpha;
						
						//float rear_hood_x = vehicle_center._x - (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 8.0f) * cos_alpha;
						//float rear_hood_y = vehicle_center._y - (Vehicle_Attribute_Shape::_vehicle_length*size_factor / 8.0f) * sin_alpha;
						
						float rear_hood_x = vehicle_center._x;
						float rear_hood_y = vehicle_center._y;

						//vehicle_shape.data[0].texture_index = Vehicle_Attribute_Shape::top_index;
						//vehicle_shape.data[1].texture_index = Vehicle_Attribute_Shape::back_index;
						//vehicle_shape.data[2].texture_index = Vehicle_Attribute_Shape::front_index;
						//vehicle_shape.data[3].texture_index = Vehicle_Attribute_Shape::left_index;
						//vehicle_shape.data[4].texture_index = Vehicle_Attribute_Shape::right_index;

						int counter = 0;

						//top_back
						vehicle_shape.data[counter].color._r = 255;
						vehicle_shape.data[counter].color._b = 255;
						vehicle_shape.data[counter].color._g = 255;
						vehicle_shape.data[counter].color._a = 255;

						vehicle_shape.data[counter].a._x = rear_hood_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = rear_hood_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].b._x = rear_windshield_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_windshield_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_height;

						vehicle_shape.data[counter].c._x = rear_windshield_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_windshield_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_height;
					
						vehicle_shape.data[counter].d._x = rear_hood_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = rear_hood_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						++counter;

						//top_mid

						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = front_windshield_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_windshield_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_height;

						vehicle_shape.data[counter].b._x = rear_windshield_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_windshield_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_height;

						vehicle_shape.data[counter].c._x = rear_windshield_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_windshield_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_height;
					
						vehicle_shape.data[counter].d._x = front_windshield_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_windshield_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_height;
						++counter;


						//top_front
						vehicle_shape.data[counter].color._r = 255;
						vehicle_shape.data[counter].color._b = 255;
						vehicle_shape.data[counter].color._g = 255;
						vehicle_shape.data[counter].color._a = 255;

						vehicle_shape.data[counter].a._x = front_hood_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_hood_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].b._x = front_windshield_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_windshield_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_height;

						vehicle_shape.data[counter].c._x = front_windshield_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_windshield_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_height;
					
						vehicle_shape.data[counter].d._x = front_hood_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_hood_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						++counter;

						//top_hood
						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].b._x = front_hood_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_hood_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].c._x = front_hood_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_hood_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
					
						vehicle_shape.data[counter].d._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						++counter;

						//back_hood
						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].b._x = rear_hood_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_hood_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						
						vehicle_shape.data[counter].c._x = rear_hood_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_hood_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].d._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						++counter;


						//back
						vehicle_shape.data[counter].color = body_color;
						
						vehicle_shape.data[counter].a._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						
						vehicle_shape.data[counter].b._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height;


						vehicle_shape.data[counter].c._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = 1;
					
						vehicle_shape.data[counter].d._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = 1;


						++counter;

						//front
						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = 1;

						vehicle_shape.data[counter].b._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = 1;
					
						vehicle_shape.data[counter].c._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].d._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						++counter;					
					
					
					
					
						//right_bottom
						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = 1;

						vehicle_shape.data[counter].b._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = 1;
					
						vehicle_shape.data[counter].c._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].d._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						++counter;

						//right_top
						vehicle_shape.data[counter].color._r = 255;
						vehicle_shape.data[counter].color._b = 255;
						vehicle_shape.data[counter].color._g = 255;
						vehicle_shape.data[counter].color._a = 255;

						vehicle_shape.data[counter].a._x = front_hood_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_hood_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].b._x = rear_hood_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_hood_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
					
						vehicle_shape.data[counter].c._x = rear_windshield_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_windshield_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_height;

						vehicle_shape.data[counter].d._x = front_windshield_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_windshield_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_height;
						++counter;

						//left_bottom
						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = 1;

						vehicle_shape.data[counter].b._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = 1;

						vehicle_shape.data[counter].c._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].d._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						++counter;
						
						//left_top
						vehicle_shape.data[counter].color._r = 255;
						vehicle_shape.data[counter].color._b = 255;
						vehicle_shape.data[counter].color._g = 255;
						vehicle_shape.data[counter].color._a = 255;

						vehicle_shape.data[counter].a._x = rear_hood_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = rear_hood_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].b._x = front_hood_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_hood_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].c._x = front_windshield_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_windshield_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_height;

						vehicle_shape.data[counter].d._x = rear_windshield_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = rear_windshield_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_height;

						_vehicle_shapes->Push_Element<Regular_Element>(&vehicle_shape);
					}
					else
					{
						vehicle_shape.ptr = this;
//TODO
//						// Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(vehicle_center);

						// display on shape vehicle layer
						float rear_x = vehicle_center._x - (Vehicle_Attribute_Shape::_vehicle_length / 2.0f) * cos_alpha;
						float rear_y = vehicle_center._y - (Vehicle_Attribute_Shape::_vehicle_length / 2.0f) * sin_alpha;
						float front_x = vehicle_center._x + (Vehicle_Attribute_Shape::_vehicle_length / 2.0f) * cos_alpha;
						float front_y = vehicle_center._y + (Vehicle_Attribute_Shape::_vehicle_length / 2.0f) * sin_alpha;
					
						float rear_windshield_x = vehicle_center._x - (Vehicle_Attribute_Shape::_vehicle_length / 4.0f) * cos_alpha;
						float rear_windshield_y = vehicle_center._y - (Vehicle_Attribute_Shape::_vehicle_length / 4.0f) * sin_alpha;

						float front_windshield_x = vehicle_center._x + (Vehicle_Attribute_Shape::_vehicle_length / 6.0f) * cos_alpha;
						float front_windshield_y = vehicle_center._y + (Vehicle_Attribute_Shape::_vehicle_length / 6.0f) * sin_alpha;

						float front_hood_x = vehicle_center._x + (Vehicle_Attribute_Shape::_vehicle_length / 4.0f) * cos_alpha;
						float front_hood_y = vehicle_center._y + (Vehicle_Attribute_Shape::_vehicle_length / 4.0f) * sin_alpha;
						
						float rear_hood_x = vehicle_center._x - (Vehicle_Attribute_Shape::_vehicle_length / 3.0f) * cos_alpha;
						float rear_hood_y = vehicle_center._y - (Vehicle_Attribute_Shape::_vehicle_length / 3.0f) * sin_alpha;

						//vehicle_shape.data[0].texture_index = Vehicle_Attribute_Shape::top_index;
						//vehicle_shape.data[1].texture_index = Vehicle_Attribute_Shape::back_index;
						//vehicle_shape.data[2].texture_index = Vehicle_Attribute_Shape::front_index;
						//vehicle_shape.data[3].texture_index = Vehicle_Attribute_Shape::left_index;
						//vehicle_shape.data[4].texture_index = Vehicle_Attribute_Shape::right_index;

						int counter = 0;

						//top_back
						vehicle_shape.data[counter].color._r = 255;
						vehicle_shape.data[counter].color._b = 255;
						vehicle_shape.data[counter].color._g = 255;
						vehicle_shape.data[counter].color._a = 255;

						vehicle_shape.data[counter].a._x = rear_hood_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = rear_hood_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].b._x = rear_windshield_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_windshield_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_height;

						vehicle_shape.data[counter].c._x = rear_windshield_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_windshield_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_height;
					
						vehicle_shape.data[counter].d._x = rear_hood_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = rear_hood_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						++counter;

						//top_mid

						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = front_windshield_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_windshield_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_height;

						vehicle_shape.data[counter].b._x = rear_windshield_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_windshield_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_height;

						vehicle_shape.data[counter].c._x = rear_windshield_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_windshield_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_height;
					
						vehicle_shape.data[counter].d._x = front_windshield_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_windshield_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_height;
						++counter;


						//top_front
						vehicle_shape.data[counter].color._r = 255;
						vehicle_shape.data[counter].color._b = 255;
						vehicle_shape.data[counter].color._g = 255;
						vehicle_shape.data[counter].color._a = 255;

						vehicle_shape.data[counter].a._x = front_hood_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_hood_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].b._x = front_windshield_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_windshield_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_height;

						vehicle_shape.data[counter].c._x = front_windshield_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_windshield_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_height;
					
						vehicle_shape.data[counter].d._x = front_hood_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_hood_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						++counter;

						//top_hood
						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].b._x = front_hood_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_hood_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].c._x = front_hood_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_hood_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
					
						vehicle_shape.data[counter].d._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						++counter;

						//back_hood
						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].b._x = rear_hood_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_hood_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						
						vehicle_shape.data[counter].c._x = rear_hood_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_hood_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].d._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						++counter;


						//back
						vehicle_shape.data[counter].color = body_color;
						
						vehicle_shape.data[counter].a._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						
						vehicle_shape.data[counter].b._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height;


						vehicle_shape.data[counter].c._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = 1;
					
						vehicle_shape.data[counter].d._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = 1;


						++counter;

						//front
						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = 1;

						vehicle_shape.data[counter].b._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = 1;
					
						vehicle_shape.data[counter].c._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].d._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						++counter;					
					
					
					
					
						//right_bottom
						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = 1;

						vehicle_shape.data[counter].b._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = 1;
					
						vehicle_shape.data[counter].c._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].d._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						++counter;

						//right_top
						vehicle_shape.data[counter].color._r = 255;
						vehicle_shape.data[counter].color._b = 255;
						vehicle_shape.data[counter].color._g = 255;
						vehicle_shape.data[counter].color._a = 255;

						vehicle_shape.data[counter].a._x = front_hood_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = front_hood_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].b._x = rear_hood_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = rear_hood_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
					
						vehicle_shape.data[counter].c._x = rear_windshield_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = rear_windshield_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_height;

						vehicle_shape.data[counter].d._x = front_windshield_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = front_windshield_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_height;
						++counter;

						//left_bottom
						vehicle_shape.data[counter].color = body_color;

						vehicle_shape.data[counter].a._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = 1;

						vehicle_shape.data[counter].b._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = 1;

						vehicle_shape.data[counter].c._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].d._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_mid_height;
						++counter;
						
						//left_top
						vehicle_shape.data[counter].color._r = 255;
						vehicle_shape.data[counter].color._b = 255;
						vehicle_shape.data[counter].color._g = 255;
						vehicle_shape.data[counter].color._a = 255;

						vehicle_shape.data[counter].a._x = rear_hood_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].a._y = rear_hood_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].a._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].b._x = front_hood_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].b._y = front_hood_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].b._z = Vehicle_Attribute_Shape::_vehicle_mid_height;

						vehicle_shape.data[counter].c._x = front_windshield_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].c._y = front_windshield_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].c._z = Vehicle_Attribute_Shape::_vehicle_height;

						vehicle_shape.data[counter].d._x = rear_windshield_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
						vehicle_shape.data[counter].d._y = rear_windshield_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
						vehicle_shape.data[counter].d._z = Vehicle_Attribute_Shape::_vehicle_height;

						_vehicle_shapes->Push_Element<Regular_Element>(&vehicle_shape);
					}
				}
	#pragma endregion

			#pragma region VEHICLE POINTS
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
					
					typedef Activity_Location<typename MasterType::activity_location_type> Activity_Location_Interface;
#ifdef IntegratedModelApplication
					Person<typename ComponentType::type_of(traveler)>* person=(Person<typename ComponentType::type_of(traveler)>*)_traveler;				
					
					if(person->has_done_replanning<bool>() && ((ComponentType*)this)->_is_integrated)
					{
						coordinate.color._r = 0;
						coordinate.color._g = 100;
						coordinate.color._b = 255;
					}
					else
#endif
					if (num_switch_decisions > 0)
					{
						if (num_switch_decisions == 1)
						{
							if(_its_switch)
							{
								coordinate.color._r = 255;
								coordinate.color._g = 0;
								coordinate.color._b = 255;
							}
							else
							{
								coordinate.color._r = 150;
								coordinate.color._g = 150;
								coordinate.color._b = 150;
							}
						}
						else
						{
							coordinate.color._r = 175;
							coordinate.color._g = 0;
							coordinate.color._b = 175;
						}
					}
					else
					{
						coordinate.color = ((MasterType::link_type*)link)->get_color_by_los(los);
					}

					if (!((ComponentType*)this)->_is_integrated)
					{
						float color_scale = 0.75;
						coordinate.color._r *= color_scale;
						coordinate.color._g *= color_scale;
						coordinate.color._b *= color_scale;
					}

					if (scenario->color_cars_randomly<bool>())
					{
						coordinate.color._r = vehicle_color._r;
						coordinate.color._g = vehicle_color._g;
						coordinate.color._b = vehicle_color._b;

						// CAV coloration for lakeside model only - remove when done
						//if (((ComponentType*)this)->_is_integrated && zone->uuid<int>() < 47)
						//{
						//	coordinate.color._r = 255;
						//	coordinate.color._g = 255;
						//	coordinate.color._b = 0;
						//}
					}

					_vehicle_points->Push_Element<Regular_Element>(&coordinate);
					vehicle_color.r(coordinate.color._r);
					vehicle_color.g(coordinate.color._g);
					vehicle_color.b(coordinate.color._b);
				}
			#pragma endregion

				//TODO: log vehicle position in binary snapshot file: set up scenario parameter so this can be turned off
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface; 
				typedef Vehicle_Components::Prototypes::Vehicle_Data_Logger< typename MasterType::vehicle_data_logger_type> _Logger_Interface;
				if (((_Scenario_Interface*)_global_scenario)->write_visualizer_snapshot<bool>())
				{
					((_Logger_Interface*)_global_vehicle_logger)->Add_Record(vehicle_center,vehicle_color);
				}
			}

			//template<typename TargetType> void load(requires(TargetType,check_2(TargetType,Types::Load_To_Origin_Link,is_same)))
			//{
			//	((Vehicle_Implementation<MasterType,INHERIT(Vehicle_Implementation)>*)this)->load<Types::Load_To_Origin_Link>();
			//	Load_Register<Antares_Vehicle_Implementation>(&compute_vehicle_position_condition<NULLTYPE>,&compute_vehicle_position<NULLTYPE>,iteration()+1,Scenario_Components::Types::END_OF_ITERATION);
			//}
			template<typename TargetType> void start_agent()
			{
				_suggested_action = Vehicle_Components::Types::DO_NOTHING;
				route_being_displayed = false;
				//int first_triggeriteration() = ((iteration() / ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()) + 1) * ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() - 1;
				if ((iteration() + 1) % ((_Scenarion_Interface*)_global_scenario)->template simulation_interval_length<int>() == 0)
				{

					Load_Event<ComponentType>(&Vehicle_Action_Condition,iteration(),Scenario_Components::Types::Type_Sub_Iteration_keys::VEHICLE_ACTION_ORIGIN_LOADING_SUB_ITERATION);
					//TODO
					//load_event(ComponentType,ComponentType::template Vehicle_Action_Condition,ComponentType::template Vehicle_Action,iteration(),Scenario_Components::Types::Type_Sub_Iteration_keys::VEHICLE_ACTION_ORIGIN_LOADING_SUB_ITERATION,NULLTYPE);
				}
				else
				{
					Load_Event<ComponentType>(&Vehicle_Action_Condition,iteration(),Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION);
					//TODO
					//load_event(ComponentType,ComponentType::template Vehicle_Action_Condition,ComponentType::template compute_vehicle_position,iteration(),Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION,NULLTYPE);
				}
			}

			static void Vehicle_Action_Condition(ComponentType* _this,Event_Response& response)
			{
				typedef Vehicle_Components::Prototypes::Vehicle<typename MasterType::vehicle_type> _Vehicle_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface; 
				if (((_Vehicle_Interface*)_this)->template simulation_status<Types::Vehicle_Status_Keys>() == Types::Vehicle_Status_Keys::OUT_NETWORK)
				{
					response.next._iteration=END;
				}
				else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::VEHICLE_ACTION_ORIGIN_LOADING_SUB_ITERATION && ((_Scenario_Interface*)_global_scenario)->template vehicle_taking_action<bool>())
				{
					if ((iteration() + 1) % ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() != 0)
					{
						cout << "iteration() " << iteration() << " is invalid" << endl;
						assert(false);
					}
					//((typename MasterType::vehicle_type*)_this)->Swap_Event((Event)&Vehicle_Action<NULLTYPE>);
					_this->Vehicle_Action();

					if (((_Vehicle_Interface*)_this)->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Types::Vehicle_Status_Keys::IN_NETWORK)
					{
						response.next._iteration=iteration();
						response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::VEHICLE_ACTION_TRANSFER_SUB_ITERATION;
					}
					else
					{
						response.next._iteration=iteration();
						response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
				}
				else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::VEHICLE_ACTION_TRANSFER_SUB_ITERATION && ((_Scenario_Interface*)_global_scenario)->template vehicle_taking_action<bool>())
				{
					if ((iteration() + 1) % ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() != 0)
					{
						cout << "iteration() " << iteration() << " is invalid" << endl;
						assert(false);
					}
					//((typename MasterType::vehicle_type*)_this)->Swap_Event((Event)&Vehicle_Action<NULLTYPE>);
					_this->Vehicle_Action();
					response.next._iteration=iteration();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
				}
				else if (sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION)
				{
					//((typename MasterType::vehicle_type*)_this)->Swap_Event((Event)&compute_vehicle_position<NULLTYPE>);
					_this->compute_vehicle_position();

					if(!((_Scenario_Interface*)_global_scenario)->template vehicle_taking_action<bool>())
					{
						response.next._iteration=iteration() + 1;
						response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
					}
					else
					{
						// depending on whether the next iteration is a simulation interval border iteration
						if ((iteration() + 2) % ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() != 0)
						{
							response.next._iteration=iteration() + 1;
							response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
						}
						else
						{
							if (((_Vehicle_Interface*)_this)->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Types::Vehicle_Status_Keys::IN_NETWORK)
							{
								response.next._iteration=iteration() + 1;
								response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::VEHICLE_ACTION_TRANSFER_SUB_ITERATION;
							}
							else
							{
								response.next._iteration=iteration() + 1;
								response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::VEHICLE_ACTION_ORIGIN_LOADING_SUB_ITERATION;
							}
						}
					}
				}
				else
				{
					cout << "Should never reach here in vehicle action conditional!" << endl;
					assert(false);				
				}
			}

			static void on_select_point(const std::list<void*>& removed,const std::list<void*>& added,const std::list<void*>& selected,std::vector<pair<string,string>>& bucket)
			{
				if(removed.size())
				{
					//Select removed one or more objects

					(MasterType::vehicle_type::_vehicle_points)->Clear_Accented<NT>();
					(MasterType::vehicle_type::_routes_layer)->Clear_Accented<NT>();
					(MasterType::vehicle_type::_locations_layer)->Clear_Accented<NT>();

					if(selected.size())
					{
						for(std::list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
						{
							((ComponentType*)*itr)->Accent_Self_Point<NT>();
						}
					}
				}
				else if(added.size())
				{
					//Select added new object

					for(std::list<void*>::const_iterator itr=added.begin();itr!=added.end();itr++)
					{
						((ComponentType*)*itr)->Accent_Self_Point<NT>();
					}
				}
				else
				{
					//Refreshing on time step

					(MasterType::vehicle_type::_vehicle_points)->Clear_Accented<NT>();
					(MasterType::vehicle_type::_routes_layer)->Clear_Accented<NT>();
					(MasterType::vehicle_type::_locations_layer)->Clear_Accented<NT>();

					if(selected.size())
					{
						for(std::list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
						{
							((ComponentType*)*itr)->Accent_Self_Point<NT>();
						}
					}
				}

				if(selected.size())
				{
					//Have at least one selected

					((ComponentType*) (selected.back()))->Display_Attributes<NT>(bucket);
				}
			}

			static void on_select_shape(const std::list<void*>& removed,const std::list<void*>& added,const std::list<void*>& selected,std::vector<pair<string,string>>& bucket)
			{
				if(removed.size())
				{
					//Select removed one or more objects

					(MasterType::vehicle_type::_vehicle_shapes)->Clear_Accented<NT>();
					(MasterType::vehicle_type::_routes_layer)->Clear_Accented<NT>();

					if(selected.size())
					{
						for(std::list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
						{
							((ComponentType*)*itr)->Accent_Self_Point<NT>();
						}
					}
				}
				else if(added.size())
				{
					//Select added new object

					for(std::list<void*>::const_iterator itr=added.begin();itr!=added.end();itr++)
					{
						((ComponentType*)*itr)->Accent_Self_Shape<NT>();
					}
				}
				else
				{
					//Refreshing on time step

					(MasterType::vehicle_type::_vehicle_shapes)->Clear_Accented<NT>();
					(MasterType::vehicle_type::_routes_layer)->Clear_Accented<NT>();

					if(selected.size())
					{
						for(std::list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
						{
							((ComponentType*)*itr)->Accent_Self_Shape<NT>();
						}
					}
				}

				if(selected.size())
				{
					//Have at least one selected

					((ComponentType*) (selected.back()))->Display_Attributes<NT>(bucket);
				}
			}

			template<typename TargetType> void Accent_Self_Shape()
			{
				if(simulation_status<Types::Vehicle_Status_Keys>() != Types::Vehicle_Status_Keys::IN_NETWORK) return;

				display_route<NT>();

				Vehicle_Attribute_Shape accented_vehicle_shape;

				accented_vehicle_shape.ptr=this;

				int num_primitives = 0;

				if(_internal_id % 10 < 2)
				{
					num_primitives = 10;
				}
				else
				{
					num_primitives = 11;
				}


				accented_vehicle_shape.Initialize(num_primitives);

				for(int i=0;i<num_primitives;i++)
				{
					accented_vehicle_shape.data[i].color._r = 255 - vehicle_shape.data[i].color._r;
					accented_vehicle_shape.data[i].color._g = 255 - vehicle_shape.data[i].color._g;
					accented_vehicle_shape.data[i].color._b = 255 - vehicle_shape.data[i].color._b;
					accented_vehicle_shape.data[i].color._a = 255;

					accented_vehicle_shape.data[i].a = vehicle_shape.data[i].a;
					accented_vehicle_shape.data[i].b = vehicle_shape.data[i].b;
					accented_vehicle_shape.data[i].c = vehicle_shape.data[i].c;
					accented_vehicle_shape.data[i].d = vehicle_shape.data[i].d;
				}

				_vehicle_shapes->Push_Element<Accented_Element>(&accented_vehicle_shape);
			}

			template<typename TargetType> void Accent_Self_Point()
			{
				if(simulation_status<Types::Vehicle_Status_Keys>() != Types::Vehicle_Status_Keys::IN_NETWORK) return;

				typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type> _Movement_Plan_Interface;

				display_route<NT>();

#ifdef IntegratedModelApplication
				if (((ComponentType*)this)->_is_integrated)
				{
					if (_locations_layer->template draw<bool>())
					{
											// Activity Attributes
						typedef Activity_Location<typename MasterType::activity_location_type> Activity_Location_Interface;
						typedef Zone<typename MasterType::zone_type> zone_interface;
						typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> activity_interface;
						typedef Person<typename get_type_of(traveler)> traveler_itf;
						typedef Household<typename traveler_itf::get_type_of(Household)> household_itf;
						typedef Person_Planner<typename traveler_itf::get_type_of(Planning_Faculty)> planner_itf;
						typedef Person_Scheduler<typename traveler_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
						typedef Back_Insertion_Sequence<typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
						typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Activity_Plan;
						typedef Back_Insertion_Sequence<typename traveler_itf::get_type_of(Activity_Record_Container)> Discarded_Activity_Plans;

						traveler_itf* person=(traveler_itf*)_traveler;
						household_itf* household = person->Household<household_itf*>();
						planner_itf* planner=person->Planning_Faculty<planner_itf* >();
						scheduler_itf* scheduler = person->Scheduling_Faculty<scheduler_itf*>();

						Activity_Plans* activities = scheduler->Activity_Container<Activity_Plans*>();
						Activity_Location_Interface* previous_location;

						//cout << endl <<endl<< "Num activities="<<activities->size();

						scheduler->Sort_Activity_Schedule<void>();

						if(activities->size() && person->Home_Location<Activity_Location_Interface*>())
						{
							previous_location = person->Home_Location<Activity_Location_Interface*>();
							int prev_end, current_start, current_end;
							Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type>* activity_planner = (Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type>*)(*(activities->begin()));
							prev_end = activity_planner->Start_Time<Time_Minutes>() - activity_planner->Expected_Travel_Time<Time_Minutes>();

							display_location<typename MasterType::vehicle_type>(previous_location, previous_location,0,0, prev_end, false );
						
							for(Activity_Plans::iterator itr=activities->begin();itr!=activities->end();itr++)
							{
								Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type>* activity_planner = (Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type>*)(*itr);
								if(activity_planner->Location<Activity_Location_Interface*>() == nullptr) continue;
								current_start = activity_planner->Start_Time<Time_Minutes>();
								current_end = activity_planner->End_Time<Time_Minutes>();

								display_location<typename MasterType::vehicle_type>( activity_planner->Location<Activity_Location_Interface*>(), previous_location,prev_end,current_start, current_end, false );
								prev_end = current_end; //current_start + activity_planner->Duration<Time_Minutes>();
								previous_location = activity_planner->Location<Activity_Location_Interface*>();
							}
							display_location<typename MasterType::vehicle_type>( person->Home_Location<Activity_Location_Interface*>(), previous_location, prev_end, prev_end + 15, 1440, false );
						}

						Discarded_Activity_Plans* discarded_activities = person->Activity_Record_Container<Discarded_Activity_Plans*>();

						//cout << endl <<endl<< "Num discarded activities="<<discarded_activities->size();

						if(discarded_activities->size() && person->Home_Location<Activity_Location_Interface*>())
						{
							previous_location = person->Home_Location<Activity_Location_Interface*>();

							for(Discarded_Activity_Plans::iterator itr=discarded_activities->begin();itr!=discarded_activities->end();itr++)
							{
								Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_record_type>* activity_planner = (Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_record_type>*)(*itr);
								if(activity_planner->Location<Activity_Location_Interface*>() == nullptr) continue;

								display_location<typename MasterType::vehicle_type>( activity_planner->Location<Activity_Location_Interface*>(), previous_location, 1,1,1,true );
								previous_location = activity_planner->Location<Activity_Location_Interface*>();
							}

							display_location<typename MasterType::vehicle_type>( person->Home_Location<Activity_Location_Interface*>(), previous_location, 1,1,1,true );
						}
					}	
				}
#endif

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
			
			template<typename TargetType> void Display_Attributes(std::vector<pair<string,string>>& bucket)
			{
				typedef Link<typename MasterType::link_type> _Link_Interface;
				typedef Movement_Plan<typename MasterType::movement_plan_type> _Movement_Plan_Interface;

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

				key_value_pair.first="departure time";
				string departure_time_str = convert_seconds_to_hhmm(((_Movement_Plan_Interface*)_movement_plan)->template absolute_departure_time<int>());
				sprintf(str_buf, "%s", departure_time_str.c_str());
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

#ifdef IntegratedModelApplication
				if (((ComponentType*)this)->_is_integrated)
				{
					// Activity Attributes
					typedef Activity_Location<typename MasterType::activity_location_type> Activity_Location_Interface;
					typedef Zone<typename MasterType::zone_type> zone_interface;
					typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> activity_interface;
					typedef Person<typename get_type_of(traveler)> traveler_itf;
					typedef Household<typename traveler_itf::get_type_of(Household)> household_itf;
					typedef Person_Planner<typename traveler_itf::get_type_of(Planning_Faculty)> planner_itf;
					typedef Person_Scheduler<typename traveler_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
					typedef Back_Insertion_Sequence<typename scheduler_itf::get_type_of(Activity_Container)> Activity_Plans;
					typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Activity_Plan;

					traveler_itf* person=(traveler_itf*)_traveler;
					household_itf* household = person->Household<household_itf*>();
					planner_itf* planner=person->Planning_Faculty<planner_itf* >();
					scheduler_itf* scheduler = person->Scheduling_Faculty<scheduler_itf*>();

					key_value_pair.first="Person ID";
					s.str("");
					//s << household->uuid<int>() <<"."<<person->uuid<int>();

					key_value_pair.second = s.str();
					bucket.push_back(key_value_pair);

					Activity_Plans* acts = scheduler->Activity_Container<Activity_Plans*>();
					
					//std::list<activity_interface*>* activities = scheduler->Activity_Container<std::list<activity_interface*>*>();

					stringstream ss("");
					time_str;
					int i = 1;
					//for (typename std::list<activity_interface*>::iterator itr = activities->begin(); itr != activities->end(); ++itr, ++i)
					for (Activity_Plans::iterator itr = acts->begin(); itr != acts->end(); ++itr, ++i)
					{
						// blank space
						key_value_pair.first="";
						time_str = "";
						sprintf(str_buf, "%s", time_str.c_str());
						key_value_pair.second=str_buf;				
						memset(&str_buf[0],0,128);
						bucket.push_back(key_value_pair);

						// activity type
						activity_interface* act = *itr;
						ss.str("");
						ss << "Act "<<i<<": Type";
						key_value_pair.first=ss.str();
						if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY) time_str = "Home";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::EAT_OUT_ACTIVITY) time_str = "Eat out";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::ERRANDS_ACTIVITY) time_str = "Errands";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::HEALTHCARE_ACTIVITY) time_str = "Healthcare";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::LEISURE_ACTIVITY) time_str = "Leisure";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::MAJOR_SHOPPING_ACTIVITY) time_str = "Shopping";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::OTHER_SHOPPING_ACTIVITY) time_str = "Shopping";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::OTHER_WORK_ACTIVITY) time_str = "Work";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::PERSONAL_BUSINESS_ACTIVITY) time_str = "Personal";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::PICK_UP_OR_DROP_OFF_ACTIVITY) time_str = "Pick/drop";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::PRIMARY_WORK_ACTIVITY) time_str = "Work";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::RECREATION_ACTIVITY) time_str = "Recreation";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::RELIGIOUS_OR_CIVIC_ACTIVITY) time_str = "Religious";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::SCHOOL_ACTIVITY) time_str = "School";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::SERVICE_VEHICLE_ACTIVITY) time_str = "Service";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::SOCIAL_ACTIVITY) time_str = "Social";
						else if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::WORK_AT_HOME_ACTIVITY) time_str = "Work";
						sprintf(str_buf, "%s", time_str.c_str());
						key_value_pair.second=str_buf;				
						memset(&str_buf[0],0,128);
						bucket.push_back(key_value_pair);


						// activity start time
						ss.str("");
						ss << "Act "<<i<<": Start Time";
						key_value_pair.first=ss.str();
						time_str = convert_seconds_to_hhmm(act->template Start_Time<Time_Seconds>());
						sprintf(str_buf, "%s", time_str.c_str());
						key_value_pair.second=str_buf;				
						memset(&str_buf[0],0,128);
						bucket.push_back(key_value_pair);

						// activity duration
						ss.str("");
						ss << "Act "<<i<<": Duration";
						key_value_pair.first=ss.str();
						time_str = convert_seconds_to_hhmm(act->template Duration<Time_Seconds>());
						sprintf(str_buf, "%s", time_str.c_str());
						key_value_pair.second=str_buf;				
						memset(&str_buf[0],0,128);
						bucket.push_back(key_value_pair);

						// activity destination
						ss.str("");
						ss << "Act "<<i<<": Destination (TAZ, loc)";
						key_value_pair.first=ss.str();
						ss.str("");
						ss << act->Location<Activity_Location_Interface*>()->zone<zone_interface*>()->uuid<int>() << ", "<< act->Location<Activity_Location_Interface*>()->uuid<int>();
						key_value_pair.second=ss.str();
						//sprintf(str_buf, "%d", act->Location<Activity_Location_Interface*>()->zone<zone_interface*>()->uuid<int>());
						//key_value_pair.second=str_buf;				
						memset(&str_buf[0],0,128);
						bucket.push_back(key_value_pair);
						// activity destination
	/*					ss.str("");
						ss << "Act "<<i<<": Location destination";
						key_value_pair.first=ss.str();
						sprintf(str_buf, "%d", act->Location<Activity_Location_Interface*>()->uuid<int>());
						key_value_pair.second=str_buf;				
						memset(&str_buf[0],0,128);
						bucket.push_back(key_value_pair);
	*/				}
				}
#endif
			}
			
			template<typename TargetType> void make_pyramid(Point_3D<MasterType>* vertex,const Point_3D<MasterType>& center,const float radius)
			{
				//---TOP---
				vertex->_x = center._x;
				vertex->_y = center._y;
				vertex->_z = radius*2;

				Scale_Coordinates<MT>(*vertex);
				++vertex;
				
				vertex->_x = center._x - radius;
				vertex->_y = center._y + radius;
				vertex->_z = 0;

				Scale_Coordinates<MT>(*vertex);
				++vertex;

				vertex->_x = center._x + radius;
				vertex->_y = center._y + radius;
				vertex->_z = 0;

				Scale_Coordinates<MT>(*vertex);
				++vertex;



				//---RIGHT---
				vertex->_x = center._x;
				vertex->_y = center._y;
				vertex->_z = radius*2;

				Scale_Coordinates<MT>(*vertex);
				++vertex;
				
				vertex->_x = center._x + radius;
				vertex->_y = center._y + radius;
				vertex->_z = 0;

				Scale_Coordinates<MT>(*vertex);
				++vertex;

				vertex->_x = center._x + radius;
				vertex->_y = center._y - radius;
				vertex->_z = 0;

				Scale_Coordinates<MT>(*vertex);
				++vertex;



				//---BOTTOM---
				vertex->_x = center._x;
				vertex->_y = center._y;
				vertex->_z = radius*2;

				Scale_Coordinates<MT>(*vertex);
				++vertex;
				
				vertex->_x = center._x + radius;
				vertex->_y = center._y - radius;
				vertex->_z = 0;

				Scale_Coordinates<MT>(*vertex);
				++vertex;

				vertex->_x = center._x - radius;
				vertex->_y = center._y - radius;
				vertex->_z = 0;

				Scale_Coordinates<MT>(*vertex);
				++vertex;



				//---LEFT---
				vertex->_x = center._x;
				vertex->_y = center._y;
				vertex->_z = radius*2;

				Scale_Coordinates<MT>(*vertex);
				++vertex;
				
				vertex->_x = center._x - radius;
				vertex->_y = center._y - radius;
				vertex->_z = 0;

				Scale_Coordinates<MT>(*vertex);
				++vertex;

				vertex->_x = center._x - radius;
				vertex->_y = center._y + radius;
				vertex->_z = 0;

				Scale_Coordinates<MT>(*vertex);
				//++vertex;


			}

			template<typename TargetType> void display_location(Activity_Location<typename MasterType::activity_location_type>* location, Activity_Location<typename MasterType::activity_location_type>* previous_location, int previous_act_end, int current_act_start, int current_act_end, bool discarded=false)
			{

				int time_scale_factor = 5;
				previous_act_end *= time_scale_factor;
				current_act_start *= time_scale_factor;
				current_act_end *= time_scale_factor;

#pragma pack(push,1)
				struct attribute_coordinate
				{
					True_Color_RGBA<MasterType> group_color;
					int num_primitives;
					Point_3D<MasterType>* vertex;
				} coordinate;
#pragma pack(pop)

				Point_3D<MasterType> vertices[12];
				coordinate.num_primitives = 4;
				coordinate.vertex = &vertices[0];

				Point_3D<MasterType>* current_vertex;
				
				float radius = 200;
				Point_3D<MasterType> center;

				center._x = location->x_position<Feet>();
				center._y = location->y_position<Feet>();
				center._z = 0;
				
				current_vertex = &vertices[0];

				make_pyramid<NT>(current_vertex,center,radius);

				if(!discarded)
				{
					coordinate.group_color._r=235;
					coordinate.group_color._g=100;
					coordinate.group_color._b=50;
					coordinate.group_color._a=200;
				}
				else
				{
					coordinate.group_color._r=0;
					coordinate.group_color._g=0;
					coordinate.group_color._b=225;
					coordinate.group_color._a=150;
				}

				_locations_layer->Push_Element<Accented_Element>(&coordinate);


#pragma pack(push,1)
				struct Link_Line
				{
					True_Color_RGBA<NT> color;
					Point_3D<MasterType> up_node;
					Point_3D<MasterType> down_node;
				} link_line;
#pragma pack(pop)

				if(previous_location!=nullptr)
				{
					Point_3D<MasterType> start;
					start._x = previous_location->x_position<Feet>();
					start._y = previous_location->y_position<Feet>();
					start._z = previous_act_end;

					//cout <<endl<< "Unscaled previous location: "<<previous_location->x_position<float>()<<","<<previous_location->y_position<float>()<<endl;

					Scale_Coordinates<MT>(start);

					Point_3D<MasterType> end;
					end._x = location->x_position<Feet>();
					end._y = location->y_position<Feet>();
					end._z = current_act_start;

					//cout << "Unscaled current location: "<<location->x_position<float>()<<","<<location->y_position<float>()<<endl;

					//cout << "Start: " << start._x << "," << start._y << "," << start._z << ".  End: " << end._x << ", " << end._y << ", " << end._z<<endl;

					Scale_Coordinates<MT>(end);

					Point_3D<MasterType> mid;
					mid._x = (start._x + end._x)/2;
					mid._y = (start._y + end._y)/2;
					mid._z = (start._z + end._z)/2;

					Point_3D<MasterType> start_act, end_act;
					start_act._x = location->x_position<Feet>();
					start_act._y = location->y_position<Feet>();
					start_act._z = current_act_start;
					end_act._x = location->x_position<Feet>();
					end_act._y = location->y_position<Feet>();
					end_act._z = current_act_end;

					Scale_Coordinates<MT>(start_act);

					Scale_Coordinates<MT>(end_act);

					//cout << "Start_act: " << start_act._x << "," << start_act._y << "," << start_act._z << ".  End_act: " << end_act._x << ", " << end_act._y << ", " << end_act._z<<endl;
					
					if(!discarded)
					{
						link_line.color._r=25;
						link_line.color._g=255;
						link_line.color._b=25;
						link_line.color._a=255;
					}
					else
					{
						link_line.color._r=0;
						link_line.color._g=0;
						link_line.color._b=200;
						link_line.color._a=200;
					}

					link_line.down_node=start;
					link_line.up_node=mid;

					/*if(!discarded)*/_routes_layer->template Push_Element<Accented_Element>(&link_line);
					
					if(!discarded)
					{
						/*link_line.color._r=0;
						link_line.color._g=0;
						link_line.color._b=0;
						link_line.color._a=255;*/
						link_line.color._r=25;
						link_line.color._g=255;
						link_line.color._b=25;
						link_line.color._a=255;
					}
					else
					{
						/*link_line.color._r=0;
						link_line.color._g=0;
						link_line.color._b=0;
						link_line.color._a=200;*/
						link_line.color._r=0;
						link_line.color._g=0;
						link_line.color._b=200;
						link_line.color._a=200;
					}

					link_line.down_node=mid;
					link_line.up_node=end;

					/*if(!discarded) */_routes_layer->template Push_Element<Accented_Element>(&link_line);

					// Push activity time line
					link_line.color._r=255;
					link_line.color._g=0;
					link_line.color._b=0;
					link_line.color._a=200;
					link_line.down_node = start_act;
					link_line.up_node=end_act;
					//cout << endl << "Push activity time line.";
					if(!discarded) _routes_layer->template Push_Element<Accented_Element>(&link_line);
				}
			}

			template<typename TargetType> void display_route()
			{
				typedef  Movement_Plan_Components::Prototypes::Movement_Plan< type_of(movement_plan)> _Movement_Plan_Interface;
				typedef  Trajectory_Unit<typename remove_pointer< _Movement_Plan_Interface::get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
				typedef  Back_Insertion_Sequence< _Movement_Plan_Interface::get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;

				typedef  Link_Components::Prototypes::Link< _Trajectory_Unit_Interface::get_type_of(link)> _Link_In_Trajectory_Interface;
				typedef  Intersection_Components::Prototypes::Intersection< _Link_In_Trajectory_Interface::get_type_of(upstream_intersection)> _Intersection_Interface;
				typedef  Switch_Decision_Data<typename remove_pointer<typename  typename type_of(switch_decisions_container)::value_type>::type>  _Switch_Decision_Data_Interface;
				typedef  Random_Access_Sequence< typename type_of(switch_decisions_container), _Switch_Decision_Data_Interface*> _Switch_Decision_Data_Container_Interface;

				typedef  Link<typename remove_pointer< _Switch_Decision_Data_Interface::get_type_of(route_links_container)::value_type>::type>  _Link_Interface;
				typedef  Random_Access_Sequence< _Switch_Decision_Data_Interface::get_type_of(route_links_container), _Link_Interface*> _Links_Container_Interface;


				//_routes_layer->Clear_Accented<NT>();

#pragma pack(push,1)
				struct Link_Line
				{
					True_Color_RGBA<NT> color;
					Point_3D<MasterType> up_node;
					Point_3D<MasterType> down_node;
				} link_line;
#pragma pack(pop)

				_Trajectory_Container_Interface* trajectory_container = ((_Movement_Plan_Interface*)_movement_plan)->trajectory_container<_Trajectory_Container_Interface*>();

				for(typename _Trajectory_Container_Interface::iterator itr = trajectory_container->begin(); itr != trajectory_container->end(); itr++)
				{
					_Trajectory_Unit_Interface* vehicle_trajectory_data=(_Trajectory_Unit_Interface*)(*itr);
					_Link_In_Trajectory_Interface* link = vehicle_trajectory_data->template link<_Link_In_Trajectory_Interface*>();
					
					link_line.up_node._x = link->template upstream_intersection<_Intersection_Interface*>()->template x_position<float>();
					link_line.up_node._y = link->template upstream_intersection<_Intersection_Interface*>()->template y_position<float>();
					link_line.up_node._z = link->template upstream_intersection<_Intersection_Interface*>()->template z_position<float>();

					Scale_Coordinates<MT>(link_line.up_node);

					link_line.down_node._x = link->template downstream_intersection<_Intersection_Interface*>()->template x_position<float>();
					link_line.down_node._y = link->template downstream_intersection<_Intersection_Interface*>()->template y_position<float>();
					link_line.down_node._z = link->template downstream_intersection<_Intersection_Interface*>()->template z_position<float>();

					Scale_Coordinates<MT>(link_line.down_node);
					
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

						Scale_Coordinates<MT>(link_line.up_node);

						link_line.down_node._x = link->template downstream_intersection<_Intersection_Interface*>()->template x_position<float>();
						link_line.down_node._y = link->template downstream_intersection<_Intersection_Interface*>()->template y_position<float>();
						link_line.down_node._z = link->template downstream_intersection<_Intersection_Interface*>()->template z_position<float>();

						Scale_Coordinates<MT>(link_line.down_node);
					
						link_line.color._r = 0;
						link_line.color._g = 0;
						link_line.color._b = max(0, link_line.color._b - 50);
						link_line.color._a = 128;
						_routes_layer->template Push_Element<Accented_Element>(&link_line);
					}
				}

			}

			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,vehicle_shapes, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,vehicle_points, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,routes_layer, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,locations_layer, NONE, NONE);
		};

		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Vehicle_Implementation<MasterType,InheritanceList>::_vehicle_shapes;
		
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Vehicle_Implementation<MasterType,InheritanceList>::_vehicle_points;

		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Vehicle_Implementation<MasterType,InheritanceList>::_routes_layer;

		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Vehicle_Implementation<MasterType,InheritanceList>::_num_vehicles;

		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Vehicle_Implementation<MasterType,InheritanceList>::_locations_layer;

		template<typename MasterType,typename InheritanceList>
		volatile int Antares_Vehicle_Implementation<MasterType,InheritanceList>::_vehicles_counter;

		template<typename MasterType,typename InheritanceList>
		std::vector<Point_2D<MasterType>> Antares_Vehicle_Implementation<MasterType,InheritanceList>::_num_vehicles_cache;

		template<typename MasterType,typename InheritanceList>
		typename MasterType::vehicle_type* Antares_Vehicle_Implementation<MasterType,InheritanceList>::_route_displayed_vehicle;
	}

}

using namespace Vehicle_Components::Implementations;
