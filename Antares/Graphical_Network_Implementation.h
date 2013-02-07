//*********************************************************
//	Graphical_Network_Implementation.h - Graphical Network Variant
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Network_Prototype.h"

namespace Network_Components
{
	namespace Implementations
	{
		//---------------------------------------------------------
		//	Graphical_Network_Implementation - network class definition
		//---------------------------------------------------------

		implementation struct Graphical_Network_Implementation:public Polaris_Component_Class<Graphical_Network_Implementation,MasterType,Data_Object,ParentType>
		{
			feature_implementation void accept_vehicle_coordinates(Point_3D<MasterType>& coordinates)
			{
				coordinates._x+= _input_offset._x;
				coordinates._y+= _input_offset._y;

				_vehicle_coordinates[_iteration][_thread_id].push_back(coordinates);
			}
			
			feature_implementation void clear_vehicle_coordinates(int iteration)
			{
				for(int i=0;i<_num_threads;i++)
				{
					_vehicle_coordinates[iteration][i].clear();
				}
			}

			feature_implementation void read_network_data(string& name)
			{
				using namespace odb;
				using namespace pio;
				
				Network_Components::Types::Network_IO_Maps net_io_maps;

				auto_ptr<database> db (open_sqlite_database (name));
				transaction t(db->begin());
				
				// reset bounds, they will be set in the node reading function
				_network_bounds.reset<type_of(network_bounds),Graphical_Network_Implementation,NULLTYPE>();

				// read intersections
				read_intersection_data<ComponentType,CallerType,TargetType>(db, net_io_maps);

				// set up input_offset and shift network bounds
				_input_offset._x = -( _network_bounds._xmax + _network_bounds._xmin )/2.0f;

				_network_bounds._xmax += _input_offset._x;
				_network_bounds._xmin += _input_offset._x;

				_input_offset._y = -( _network_bounds._ymax + _network_bounds._ymin )/2.0f;

				_network_bounds._ymax += _input_offset._y;
				_network_bounds._ymin += _input_offset._y;
				
				// read links
				read_link_data<ComponentType,CallerType,TargetType>(db, net_io_maps);
			}

			feature_implementation void read_intersection_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps)
			{
				using namespace odb;
				using namespace pio;

				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				_Intersections_Container_Interface* intersections_container_ptr=intersections_container<ComponentType,CallerType,_Intersections_Container_Interface*>();

				result<Node> node_result=db->query<Node>(query<Node>::true_expr);

				net_io_maps.intersection_id_to_ptr.set_empty_key(-1);
				net_io_maps.intersection_id_to_ptr.set_deleted_key(-2);

				_Intersection_Interface* intersection;

				int counter=-1;

				cout << "Reading Graphical Nodes" << endl;

				for(result<Node>::iterator db_itr = node_result.begin (); db_itr != node_result.end (); ++db_itr)
				{
					counter++;
					if(counter%10000==0) cout << "\t" << counter << endl;

					intersection=(_Intersection_Interface*)Allocate<typename _Intersection_Interface::Component_Type>();

					intersection->template uuid<int>( db_itr->getNode() );
					intersection->template internal_id<int>(counter);
					//intersection->template x_position<float>( scenario->meterToFoot<NULLTYPE>(db_itr->getX()));
					//intersection->template y_position<float>( scenario->meterToFoot<NULLTYPE>(db_itr->getY()));
					intersection->template x_position<float>( db_itr->getX());
					intersection->template y_position<float>( db_itr->getY());

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

					net_io_maps.intersection_id_to_ptr[db_itr->getNode()]=intersection;
					
					intersections_container_ptr->push_back(intersection);
				}
			}

			feature_implementation void read_link_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps)
			{
				using namespace odb;
				using namespace pio;

				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				_Intersections_Container_Interface* intersections_container_ptr=intersections_container<ComponentType,CallerType,_Intersections_Container_Interface*>();
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				_Links_Container_Interface* links_container_ptr=links_container<ComponentType,CallerType,_Links_Container_Interface*>();			
				
				result<Link> link_result=db->query<Link>(query<Link>::true_expr);

				_Link_Interface* link;
				
				int counter=0;

				cout << "Reading Graphical Links" << endl;
				
				int link_counter=-1;
				
				Point_2D<MasterType> current_point;

				for(result<Link>::iterator db_itr = link_result.begin (); db_itr != link_result.end (); ++db_itr)
				{
					counter++;
					if(counter%10000==0) cout << "\t" << counter << endl;

					if(db_itr->getLanes_Ab()>0)
					{
						link=(_Link_Interface*)Allocate<typename _Link_Interface::Component_Type>();

						link->template upstream_intersection<_Intersection_Interface*>((_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[db_itr->getNode_A()->getNode()]);
						link->template downstream_intersection<_Intersection_Interface*>((_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[db_itr->getNode_B()->getNode()]);
						
						link->template group_reference<int>(db_itr->getLink());

						link->template uuid<int>(++link_counter);
						link->template internal_id<int>(link_counter);

						link->template num_lanes<int>(db_itr->getLanes_Ab());
						
						//link->template length<float>(scenario->meterToFoot<NULLTYPE>(db_itr->getLength()));
						//link->template speed_limit<float>(scenario->mepsToMiph<NULLTYPE>(db_itr->getSpeed_Ab()));
						
						link->template length<float>(db_itr->getLength());
						link->template speed_limit<float>(db_itr->getSpeed_Ab());

						link->template num_left_turn_bays<int>(db_itr->getLeft_Ab());
						link->template num_right_turn_bays<int>(db_itr->getRight_Ab());
						
						link->template left_turn_bay_length<float>(0.0);
						link->template right_turn_bay_length<float>(0.0);

						
						const string& facility_type=db_itr->getType();

						if(facility_type=="FREEWAY")
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::FREEWAY);
						}
						else if(facility_type=="EXPRESSWAY")
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::EXPRESSWAY);
						}
						else if(facility_type=="RAMP")
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::ON_RAMP);
						}
						else
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::ARTERIAL);
						}

						links_container<ComponentType,CallerType,_Links_Container_Interface&>().push_back(link);
					}

					if(db_itr->getLanes_Ba()>0)
					{
						link=(_Link_Interface*)Allocate<typename _Link_Interface::Component_Type>();


						link->template upstream_intersection<_Intersection_Interface*>((_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[db_itr->getNode_B()->getNode()]);
						link->template downstream_intersection<_Intersection_Interface*>((_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[db_itr->getNode_A()->getNode()]);

						link->template group_reference<int>(db_itr->getLink());
						
						link->template uuid<int>(++link_counter);
						link->template internal_id<int>(link_counter);

						link->template num_lanes<int>(db_itr->getLanes_Ba());
						
						//link->template length<float>(scenario->meterToFoot<NULLTYPE>(db_itr->getLength()));
						//link->template speed_limit<float>(scenario->mepsToMiph<NULLTYPE>(db_itr->getSpeed_Ba()));
						link->template length<float>(db_itr->getLength());
						link->template speed_limit<float>(db_itr->getSpeed_Ba());

						link->template num_left_turn_bays<int>(db_itr->getLeft_Ba());
						link->template num_right_turn_bays<int>(db_itr->getRight_Ba());
						
						link->template left_turn_bay_length<float>(0.0);
						link->template right_turn_bay_length<float>(0.0);

						
						const string& facility_type=db_itr->getType();

						if(facility_type=="FREEWAY")
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::FREEWAY);
						}
						else if(facility_type=="EXPRESSWAY")
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::EXPRESSWAY);
						}
						else if(facility_type=="RAMP")
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::ON_RAMP);
						}
						else
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::ARTERIAL);
						}

						links_container<ComponentType,CallerType,_Links_Container_Interface&>().push_back(link);
					}

					current_point._x = link->upstream_intersection<_Intersection_Interface*>()->x_position<float>() + _input_offset._x;
					current_point._y = link->upstream_intersection<_Intersection_Interface*>()->y_position<float>() + _input_offset._y;

					_link_lines.push_back(current_point);

					current_point._x = link->downstream_intersection<_Intersection_Interface*>()->x_position<float>() + _input_offset._x;
					current_point._y = link->downstream_intersection<_Intersection_Interface*>()->y_position<float>() + _input_offset._y;

					_link_lines.push_back(current_point);
				}
			}

			member_data(Point_2D<MasterType>, input_offset,none,none);
			member_data(Rectangle_XY<MasterType>, network_bounds,none,none);
			member_data(vector< Point_2D<MasterType> >, link_lines,none,none);

			member_data(concat(Multi_Buffer< vector< Point_3D<MasterType> >[_num_threads], 3 >),vehicle_coordinates,none,none);

			member_container(vector<typename MasterType::graphical_intersection_type*>, intersections_container, none, none);
			member_container(vector<typename MasterType::graphical_link_type*>, links_container, none, none);
		};
	}
}

using namespace Network_Components::Implementations;