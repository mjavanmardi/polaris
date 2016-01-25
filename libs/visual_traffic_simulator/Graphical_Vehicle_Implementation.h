//*********************************************************
//	Graphical_Vehicle_Implementation.h - Graphical Vehicle Variant
//*********************************************************

#pragma once
#include "Dependencies.h"
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
		const float Vehicle_Attribute_Shape::_vehicle_rear_width = 7 * 5; // rear width in feet
		const float Vehicle_Attribute_Shape::_vehicle_front_width = 2 * 5; // front width in feet
		const float Vehicle_Attribute_Shape::_vehicle_length = 13.5 * 5; // length in feet

		implementation struct Graphical_Vehicle_Implementation:public Polaris_Component<MasterType,INHERIT(Graphical_Vehicle_Implementation),Execution_Object>
		{
			Vehicle_Attribute_Shape vehicle_shape;

			static std::vector<Point_2D<MasterType>> _num_vehicles_cache;
			
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,num_vehicles, NONE, NONE);

			static volatile m_data(int,vehicles_counter, NONE, NONE);

			m_data(float, distance_to_stop_bar, NONE, NONE);
			m_data(float, local_speed, NONE, NONE);

			static void Initialize_Layer()
			{
				Initialize_Vehicle_Shapes_Layer();
				Initialize_Vehicle_Points_Layer();
			}

			static void Initialize_Vehicle_Shapes_Layer()
			{
				_vehicle_shapes=Allocate_New_Layer<MT>(string("Vehicles (shape)"));
				Antares_Layer_Configuration cfg;
				cfg.dynamic_data=true;
				cfg.target_sub_iteration=Scenario_Components::Types::END_OF_ITERATION+1;
				cfg.storage_offset=iteration();
				cfg.storage_size=3;
				cfg.storage_period=1;

				cfg.primitive_type=_QUAD;
				cfg.primitive_color=true;

				//cfg.attributes_schema = string("ID,Status,Current_Link");
				
				cfg.attributes_schema.push_back("ID");
				cfg.attributes_schema.push_back("Status");
				cfg.attributes_schema.push_back("Current_Link");

				cfg.attributes_callback = (attributes_callback_type)&fetch_attributes;
				cfg.submission_callback = (attributes_callback_type)&submit_attributes;

				_vehicle_shapes->Initialize<NULLTYPE>(cfg);
			}

			static void Initialize_Vehicle_Points_Layer()
			{
				_vehicle_points=Allocate_New_Layer<MT>(string("Vehicles (point)"));
				Antares_Layer_Configuration cfg;
				cfg.Configure_Points();
				cfg.primitive_color=true;

				//cfg.attributes_schema = string("ID,Status,Current_Link");
				
				cfg.attributes_schema.push_back("ID");
				cfg.attributes_schema.push_back("Status");
				cfg.attributes_schema.push_back("Current_Link");
				
				cfg.attributes_callback = (attributes_callback_type)&fetch_attributes;
				cfg.submission_callback = (attributes_callback_type)&submit_attributes;

				_vehicle_points->Initialize<NULLTYPE>(cfg);
			}

			
			m_data(Vehicle_Components::Types::Vehicle_Status_Keys, simulation_status, NONE, NONE);
			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, internal_id, NONE, NONE);

			m_prototype(Movement_Plan, typename MasterType::movement_plan_type, movement_plan, NONE, NONE);

#ifndef EXCLUDE_DEMAND
			m_prototype(Null_Prototype,typename MasterType::person_type, traveler, NONE, NONE);
#else
			m_prototype(Null_Prototype,typename MasterType::traveler_type, traveler, NONE, NONE);
#endif

			typedef Link<typename MasterType::link_type> Link_Interface;
			typedef Intersection<typename MasterType::intersection_type> Intersection_Interface;

			static void compute_vehicle_position_condition(ComponentType* _this,Event_Response& response)
			{
				response.result=true;
				response.next._iteration=_iteration+1;
				response.next._sub_iteration=Scenario_Components::Types::END_OF_ITERATION;
			}

			declare_event(compute_vehicle_position)
			{
				Graphical_Vehicle_Implementation* pthis=(Graphical_Vehicle_Implementation*)_this;

				if(pthis->simulation_status<Types::Vehicle_Status_Keys>() != Types::Vehicle_Status_Keys::IN_NETWORK) return;

				if(pthis->_movement_plan == NULL)
				{
					cout << "verified!" << endl;
					return;
				}
				Link_Interface* link=pthis->_movement_plan->current_link<Link_Interface*>();

				if(link == nullptr)
				{
					cout << "verified!" << endl;
					return;
				}

				Intersection_Interface* upstream_intersection=link->upstream_intersection<Intersection_Interface*>();
				Intersection_Interface* downstream_intersection=link->downstream_intersection<Intersection_Interface*>();

				pthis->vehicle_shape.ptr = _this;


				float u_x = upstream_intersection->x_position<float>();
				float u_y = upstream_intersection->y_position<float>();
				float d_x = downstream_intersection->x_position<float>();
				float d_y = downstream_intersection->y_position<float>();
				float sin_alpha = (d_y - u_y) / link->length<float>();
				float cos_alpha = (d_x - u_x) / link->length<float>();

				float travel_distance = (pthis->_local_speed * 5280.0f / 3600.0f);
				float current_distance = pthis->_distance_to_stop_bar;
				float new_distance = max(0.0f,(current_distance - travel_distance));
				pthis->_distance_to_stop_bar = new_distance; 
				float distance_from_up = link->length<float>() - pthis->_distance_to_stop_bar;
				
				Point_3D<MasterType> vehicle_center;

				vehicle_center._x = u_x + distance_from_up * cos_alpha;
				vehicle_center._y = u_y + distance_from_up * sin_alpha;

				Scale_Coordinates<MT>(vehicle_center);

				// display on shape vehicle layer
				float rear_x = vehicle_center._x - (Vehicle_Attribute_Shape::_vehicle_length / 2.0f) * cos_alpha;
				float rear_y = vehicle_center._y - (Vehicle_Attribute_Shape::_vehicle_length / 2.0f) * sin_alpha;
				float front_x = vehicle_center._x + (Vehicle_Attribute_Shape::_vehicle_length / 2.0f) * cos_alpha;
				float front_y = vehicle_center._y + (Vehicle_Attribute_Shape::_vehicle_length / 2.0f) * sin_alpha;

				pthis->vehicle_shape.a._x = rear_x + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
				pthis->vehicle_shape.a._y = rear_y - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
				pthis->vehicle_shape.a._z = 1;
				
				pthis->vehicle_shape.b._x = front_x + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
				pthis->vehicle_shape.b._y = front_y - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
				pthis->vehicle_shape.a._z = 1;

				pthis->vehicle_shape.c._x = front_x - (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * sin_alpha;
				pthis->vehicle_shape.c._y = front_y + (Vehicle_Attribute_Shape::_vehicle_front_width / 2.0f) * cos_alpha;
				pthis->vehicle_shape.c._z = 1;

				pthis->vehicle_shape.d._x = rear_x - (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * sin_alpha;
				pthis->vehicle_shape.d._y = rear_y + (Vehicle_Attribute_Shape::_vehicle_rear_width / 2.0f) * cos_alpha;
				pthis->vehicle_shape.d._z = 1;

				float los = ((MasterType::link_type*)link)->realtime_link_moe_data.link_density / ((MasterType::link_type*)link)->_jam_density;
				pthis->vehicle_shape.color = ((MasterType::link_type*)link)->get_color_by_los(los);
				pthis->vehicle_shape.color._a = 255;
				_vehicle_shapes->Push_Element<Regular_Element>(&pthis->vehicle_shape);


				// display on point vehicle layer
#pragma pack(push,1)
				struct attribute_coordinate
				{
					void* ptr;
					True_Color_RGBA<NT> color;
					Point_3D<MasterType> vertex;
				} coordinate;
#pragma pack(pop)
				coordinate.ptr = _this;				
				coordinate.vertex._x=vehicle_center._x;//upstream_intersection->x_position<float>();
				coordinate.vertex._y=vehicle_center._y;//upstream_intersection->y_position<float>();
				coordinate.vertex._z=1;
				coordinate.color = ((MasterType::link_type*)link)->get_color_by_los(los);
				_vehicle_points->Push_Element<Regular_Element>(&coordinate);

				pthis->_vehicles_counter++;
			}

			template<typename TargetType> void load(requires(TargetType,check_2(TargetType,Types::Load_To_Entry_Queue,is_same)))
			{
				_simulation_status = Types::Vehicle_Status_Keys::IN_ENTRY_QUEUE;
			}

			template<typename TargetType> void load(requires(TargetType,check_2(TargetType,Types::Load_To_Origin_Link,is_same)))
			{
				typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type> _Movement_Plan_Interface;
				_simulation_status =  Types::Vehicle_Status_Keys::IN_NETWORK;
				((_Movement_Plan_Interface*)_movement_plan)->template initialize_trajectory<NULLTYPE>();

				Load_Event<Graphical_Vehicle_Implementation>(&compute_vehicle_position_condition<NULLTYPE>,&compute_vehicle_position<NULLTYPE>,iteration()+1,Scenario_Components::Types::END_OF_ITERATION);
			}

			template<typename TargetType> void load(requires(TargetType,!check_2(TargetType,Types::Load_To_Origin_Link,is_same) && !check_2(TargetType,Types::Load_To_Entry_Queue,is_same)))
			{
				static_assert(false,"Unrecognized load type!");
			}

			template<typename TargetType> void unload()
			{
				_simulation_status = Types::Vehicle_Status_Keys::OUT_NETWORK;
			}


			static bool fetch_attributes(Graphical_Vehicle_Implementation* _this,std::vector<string>& bucket)
			{
				//_this->_graphical_network->accent_num_vehicles<NT>();

				stringstream s;
				
				s << _this->_internal_id;
				bucket.push_back(s.str());
				s.str("");
				
				bucket.push_back(string("IN_NETWORK"));
				
				s << _this->_movement_plan->current_link<Link<typename MasterType::type_of(link)>*>()->uuid<int>();
				bucket.push_back(s.str());

				return true;
			}
			
			static bool submit_attributes(Graphical_Vehicle_Implementation* _this,std::vector<string>& bucket)
			{
				std::vector<string>::iterator itr;

				int new_id=atoi(bucket[0].c_str());

				if(new_id%2==0)
				{
					_this->_internal_id = new_id;

					return true;
				}
				else
				{
					return false;
				}
			}

			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,vehicle_shapes, NONE, NONE);
			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,vehicle_points, NONE, NONE);
		};

		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type,Graphical_Vehicle_Implementation<MasterType,InheritanceList>>* Graphical_Vehicle_Implementation<MasterType,InheritanceList>::_vehicle_shapes;
		
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type,Graphical_Vehicle_Implementation<MasterType,InheritanceList>>* Graphical_Vehicle_Implementation<MasterType,InheritanceList>::_vehicle_points;
		
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type,Graphical_Vehicle_Implementation<MasterType,InheritanceList>>* Graphical_Vehicle_Implementation<MasterType,InheritanceList>::_num_vehicles;

		template<typename MasterType,typename InheritanceList>
		volatile int Graphical_Vehicle_Implementation<MasterType,InheritanceList>::_vehicles_counter;

		template<typename MasterType,typename InheritanceList>
		std::vector<Point_2D<MasterType>> Graphical_Vehicle_Implementation<MasterType,InheritanceList>::_num_vehicles_cache;
	}

}

using namespace Vehicle_Components::Implementations;
