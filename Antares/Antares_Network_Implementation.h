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

			feature_implementation void initialize_link_layer()
			{
				set_link_data<ComponentType,CallerType,TargetType>();
			}



			feature_implementation void set_link_data()
			{
				
				_link_lines=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_lines),Antares_Network_Implementation>*, string& > >(string("Links"));

				Antares_Layer_Configuration cfg;
				cfg.Configure_Lines();
				cfg.draw=true;

				//cfg.head_size_value = 
				cfg.attributes_callback = (attributes_callback_type)&MasterType::type_of(link)::fetch_attributes;
				cfg.attributes_schema = string("Id,Type,Travel time,Travel delay,Speed,Density,In-flow rate,Out-flow rate,Travel time ratio,Speed ratio,Density ratio,In-flow ratio,Out-flow ratio");
				_link_lines->Initialize<NULLTYPE>(cfg);
				struct Link_Line
				{
					void* data;
					Point_3D<MasterType> a;
					Point_3D<MasterType> b;
				} current_line;

				typename _Links_Container_Interface::iterator link_itr;

				for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
				{
					_Link_Interface* link = (_Link_Interface*)(*link_itr);

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

			feature_implementation void printResults() 
			{
				((Polaris_Network_Implementation<MasterType,ParentType,APPEND_CHILD(Integrated_Polaris_Network_Implementation)>*)this)->printResults<ComponentType,CallerType,TargetType>();
				typename _Links_Container_Interface::iterator link_itr;
				for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
				{
					_Link_Interface* link = (_Link_Interface*)(*link_itr);
					link->visualize_moe<NULLTYPE>();
				}

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

			//member_data(Point_2D<MasterType>, input_offset,none,none);
			member_data(Rectangle_XY<MasterType>, network_bounds,none,none);
			
			member_prototype(Antares_Layer,link_lines,typename type_of(MasterType::antares_layer),none,none);
			//member_prototype(Antares_Layer,vehicle_points,typename type_of(MasterType::antares_layer),none,none);
			//member_prototype(Antares_Layer,num_vehicles,typename type_of(MasterType::antares_layer),none,none);
			//member_prototype(Antares_Layer,num_vehicles_B,typename type_of(MasterType::antares_layer),none,none);

			//member_data(volatile int,vehicles_counter,none,none);

			//member_prototype(Canvas,canvas,typename MasterType::type_of(canvas),none,none);
			//member_prototype(Information_Panel,information_panel,typename MasterType::type_of(information_panel),none,none);
			
			define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
		};
	}
}

using namespace Network_Components::Implementations;