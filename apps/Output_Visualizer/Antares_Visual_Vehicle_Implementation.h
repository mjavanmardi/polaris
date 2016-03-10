//**************************************************************************************************
//	Antares_Vehicle_Implementation.h - Graphical Vehicle Variant
//**************************************************************************************************

#pragma once
#include "Vehicle_Prototype.h"
#include "Visual_Traffic_Simulator\Dependencies.h"
#include <sstream>


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

		implementation struct Antares_Visual_Vehicle_Implementation:public Polaris_Component<MasterType,INHERIT(Antares_Visual_Vehicle_Implementation), Execution_Object>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Antares_Visual_Vehicle_Implementation), Execution_Object>::ComponentType ComponentType;

			m_data(int, num_vehicles,NONE,NONE);
			m_data(float*, vehicle_pos,NONE,NONE);
		
			static void Initialize_Layer()
			{
				Initialize_Vehicle_Points_Layer();
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

			typedef Link<typename MasterType::link_type> Link_Interface;
			typedef Intersection<typename MasterType::intersection_type> Intersection_Interface;
			
			void compute_vehicle_position()
			{
				this->display_vehicle_position<NT>();
			}

			template<typename TargetType> void Initialize(int first_iteration)
			{
				this->_num_vehicles=0;
				this->_vehicle_pos = nullptr;
				Load_Event<ComponentType>(&Vehicle_Action_Condition,first_iteration,Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION);
			}

			template<typename TargetType> void Reschedule_Execution(int new_iteration)
			{
				// update scheduling for the vehicle class
				Reschedule<ComponentType>(new_iteration,Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION);

				// also update scheduling for the associated antares layer
				_vehicle_points->Reschedule_Execution<NT>(new_iteration);
			}

			template<typename TargetType> void Update(int num_vehicles, float* data)
			{
				this->_num_vehicles=num_vehicles;
				this->_vehicle_pos = data;
				//Load_Event<ComponentType>(&Vehicle_Action_Condition,iteration(),Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION);
			}

			template<typename TargetType> void display_vehicle_position()
			{
				for (int i=0; i<_num_vehicles; ++i)
				{
					float x = _vehicle_pos[i];
					float y = _vehicle_pos[++i];
					float color = _vehicle_pos[++i];
					True_Color_RGBA<NT>* pcolor = (True_Color_RGBA<NT>*)&color;
				
					//cout <<"X,Y="<<x<<","<<y<<endl;

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
						coordinate.vertex._x=x;//upstream_intersection->x_position<float>();
						coordinate.vertex._y=y;//upstream_intersection->y_position<float>();
						coordinate.vertex._z=1;
						//coordinate.color = ((MasterType::link_type*)link)->get_color_by_los(los);
						coordinate.color._r = pcolor->_r;
						coordinate.color._g = pcolor->_g;
						coordinate.color._b = pcolor->_b;

						_vehicle_points->Push_Element<Regular_Element>(&coordinate);
					}
				}
				delete _vehicle_pos;
			}

			static void Vehicle_Action_Condition(ComponentType* _this,Event_Response& response)
			{
				typedef Vehicle_Components::Prototypes::Vehicle<typename MasterType::vehicle_type> _Vehicle_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface; 
				
				if (sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION)
				{
					_this->compute_vehicle_position();

					response.next._iteration = iteration()+1;
					response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;				
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

					//((ComponentType*) (selected.back()))->Display_Attributes<NT>(bucket);
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

//
//				Link_Interface* link=((_Movement_Plan_Interface*)_movement_plan)->template current_link<Link_Interface*>();
//
//				Link_Line<MasterType>& link_line = link->template displayed_line<Link_Line<MasterType>&>();
//
//				float u_x = link_line.up_node._x;
//				float u_y = link_line.up_node._y;
//				float d_x = link_line.down_node._x;
//				float d_y = link_line.down_node._y;
//				float distance = sqrt((u_x - d_x) * (u_x - d_x) + (u_y - d_y) * (u_y - d_y));
//				float sin_alpha = (d_y - u_y) / distance;
//				float cos_alpha = (d_x - u_x) / distance;
//				float distance_from_up = distance - _distance_to_stop_bar * (distance / link->length<float>());
//
//				Point_3D<MasterType> vehicle_center;
//
//				vehicle_center._x = u_x + distance_from_up * cos_alpha;
//				vehicle_center._y = u_y + distance_from_up * sin_alpha;
//
//#pragma pack(push,1)
//				struct attribute_coordinate
//				{
//					void* ptr;
//					True_Color_RGBA<NT> color;
//					Point_3D<MasterType> vertex;
//				} coordinate;
//#pragma pack(pop)
//				coordinate.ptr = this;
//				coordinate.vertex._x=vehicle_center._x;//upstream_intersection->x_position<float>();
//				coordinate.vertex._y=vehicle_center._y;//upstream_intersection->y_position<float>();
//				coordinate.vertex._z=1;
//				coordinate.color._r=255;
//				coordinate.color._g=0;
//				coordinate.color._b=255;
//				_vehicle_points->Push_Element<Accented_Element>(&coordinate);
			}
			
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,vehicle_points, NONE, NONE);
		};
	
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Visual_Vehicle_Implementation<MasterType,InheritanceList>::_vehicle_points;
	}

}

using namespace Vehicle_Components::Implementations;
