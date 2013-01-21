#pragma once
#include <hash_map>
#include "User_Space.h"
#include "../File_IO/utilities.h"

#include "../File_IO/network_data.h"
//using namespace Basic_Units::Data_Structures;
//using namespace Signal_Components::Data_Structures;

/*
 * Needed for Linux compatability. Do not hurt Windows compilation.
 */
namespace Scenario_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Scenario_Prototype;
	}
};

namespace Activity_Location_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Activity_Location_Prototype;
	}
};
namespace Link_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Link_Prototype;
	}
};
namespace Vehicle_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Vehicle_Prototype;
	}
};
namespace Intersection_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Intersection_Prototype;
		forward_declare_prototype struct Movement_Prototype;
		forward_declare_prototype struct Outbound_Inbound_Movements_Prototype;
		forward_declare_prototype struct Inbound_Outbound_Movements_Prototype;
	}
};
namespace Zone_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Zone_Prototype;
	}
};
namespace Intersection_Control_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Intersection_Control_Prototype;
	}
};
/*------------------------------------------------*/

namespace Network_Components
{
	namespace Types
	{
		struct Hard_Coded_Network{};
		struct ODB_Network{};
		struct File_Network{};

				
		struct Network_IO_Maps
		{
			dense_hash_map<int,void*> intersection_id_to_ptr;
			dense_hash_map<long long,void*> link_id_dir_to_ptr;
		};

		template<typename _NetIOType,typename _ParamType>
		struct Network_Initialization_Type
		{
			typedef _NetIOType NetIOType;
			typedef _ParamType ParamType;
		};

		enum Scan_List_Status_Keys
		{
				UNSCANNED=0,
				SCANNED,
				INSELIST,
		};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Network_Prototype
		{
			tag_as_prototype;

			//==================================================================================================================
			/// basic network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(intersections_container, none, none);
			feature_accessor(links_container, none, none);
			feature_accessor(max_free_flow_speed, none, none);
			//------------------------------------------------------------------------------------------------------------------
			
			//==================================================================================================================
			/// transportation network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(turn_movements_container, none, none);
			feature_accessor(activity_locations_container, none, none);
			feature_accessor(zones_container, none, none);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// routable network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(routable_network, none, none);
			feature_accessor(routable_networks_container, none, none);
			feature_accessor(scan_list, none, none);
			feature_accessor(reversed_path_container, none, none);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// simulation network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(scenario_reference, none, none);
			volatile long _intersection_finish_counter; // To track the end of an iteration, track how many links have finished processing
			long _intersection_finish_target; // If link_finish_counter reaches link_finish_target, then an iteration has completed
			//------------------------------------------------------------------------------------------------------------------

			feature_prototype void read_network_data(typename TargetType::ParamType& network_mapping,
				requires(check_2(TargetType::NetIOType,Types::ODB_Network,is_same)))
			{
				using namespace odb;
				using namespace pio;

				Network_Components::Types::Network_IO_Maps& net_io_maps=(Network_Components::Types::Network_IO_Maps&)network_mapping;
				string name("chicago_with_demand_no_ref.sqlite");
				auto_ptr<database> db (open_sqlite_database (name));


				int counter=0;
				const float link_length = 5280.0; // in foot
				const float speed_limit = 60.0; // in miles per hour
				const float speed_limit_ramp = 30.0; 
				const float maximum_flow_rate = 2200.0; // in vehicles per hour per lane
				const float maximum_flow_rate_ramp = 600.0; // 
				const float maximum_flow_rate_arterial = 900;
				const float jam_density = 220.0; // in vehiles per mile per lane
				const float backward_wave_speed = 12.0;
				const float distance_factor = 1.5;				

				max_free_flow_speed<int>(-1);

				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				_Intersections_Container_Interface* intersections_container_ptr=intersections_container<_Intersections_Container_Interface*>();
				get_type_of(intersections_container)& intersections_container_monitor=(get_type_of(intersections_container)&)(*intersections_container_ptr);
				define_component_interface(_Intersection_Control_Interface, _Intersection_Interface::get_type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, _Intersection_Control_Interface::get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				

				intersections_container_ptr->clear();
				
				transaction t(db->begin());

				result<Node> node_result=db->query<Node>(query<Node>::true_expr);

				net_io_maps.intersection_id_to_ptr.set_empty_key(-1);
				net_io_maps.intersection_id_to_ptr.set_deleted_key(-2);

				_Intersection_Interface* intersection;

				counter=-1;

				cout << "Reading Nodes" << endl;

				for(result<Node>::iterator db_itr = node_result.begin (); db_itr != node_result.end (); ++db_itr)
				{
					counter++;
					if(counter%10000==0) cout << "\t" << counter << endl;

					intersection=(_Intersection_Interface*)Allocate<typename _Intersection_Interface::Component_Type>();

					intersection->template uuid<int>( db_itr->getNode() );
					intersection->template internal_id<int>(counter);
					intersection->template x_position<float>( scenario_reference<_Scenario_Interface*>()->meterToFoot<NULLTYPE>(db_itr->getX()));
					intersection->template y_position<float>( scenario_reference<_Scenario_Interface*>()->meterToFoot<NULLTYPE>(db_itr->getY()));
					intersection->template intersection_control<_Intersection_Control_Interface*>((_Intersection_Control_Interface*)nullptr);
					
					net_io_maps.intersection_id_to_ptr[db_itr->getNode()]=intersection;

					if (scenario_reference<_Scenario_Interface*>()->intersection_control_flag<int>() == 0) 
					{
						intersection->template intersection_type<int>(Intersection_Components::Types::NO_CONTROL);

						_Intersection_Control_Interface* intersection_control = (_Intersection_Control_Interface*)Allocate<typename _Intersection_Control_Interface::Component_Type>();
						intersection_control->template intersection<_Intersection_Interface*>(intersection);
					
						_Control_Plan_Interface* control_plan = (_Control_Plan_Interface*)Allocate<typename _Control_Plan_Interface::Component_Type>();
						control_plan->template control_plan_index<int>(0.0);
						control_plan->template control_type<int>(Intersection_Components::Types::NO_CONTROL);
						control_plan->template starting_time<int>(0.0);
						control_plan->template ending_time<int>(24*60*60);
						intersection_control->template control_plan_data_array<_Control_Plans_Container_Interface&>().push_back(control_plan);
						intersection->template intersection_control<_Intersection_Control_Interface*>(intersection_control);
					}
					intersections_container_ptr->push_back(intersection);
				}
				



				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				_Links_Container_Interface* links_container_ptr=links_container<_Links_Container_Interface*>();
				get_type_of(links_container)& links_container_monitor=(get_type_of(links_container)&)(*links_container_ptr);				
				

				union Link_ID_Dir
				{
					struct
					{
						long id;
						long dir;
					};

					long long id_dir;
				} link_id_dir;
				

				result<Link> link_result=db->query<Link>(query<Link>::true_expr);

				net_io_maps.link_id_dir_to_ptr.set_empty_key(-1);
				net_io_maps.link_id_dir_to_ptr.set_deleted_key(-2);

				_Link_Interface* link;
				
				counter=0;

				cout << "Reading Links" << endl;
				
				int link_counter=-1;

				for(result<Link>::iterator db_itr = link_result.begin (); db_itr != link_result.end (); ++db_itr)
				{
					counter++;
					if(counter%10000==0) cout << "\t" << counter << endl;

					if(db_itr->getLanes_Ab()>0)
					{
						link=(_Link_Interface*)Allocate<typename _Link_Interface::Component_Type>();
						
						link_id_dir.id=db_itr->getLink();
						link_id_dir.dir=0;

						net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir]=link;



						link->template upstream_intersection<_Intersection_Interface*>((_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[db_itr->getNode_A()->getNode()]);
						link->template downstream_intersection<_Intersection_Interface*>((_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[db_itr->getNode_B()->getNode()]);

						
						link->template uuid<int>(++link_counter);
						link->template internal_id<int>(link_counter);

						link->template num_lanes<int>(db_itr->getLanes_Ab());
						
						link->template length<float>(scenario_reference<_Scenario_Interface*>()->meterToFoot<NULLTYPE>(db_itr->getLength()));
						link->template speed_limit<float>(scenario_reference<_Scenario_Interface*>()->mepsToMiph<NULLTYPE>(db_itr->getSpeed_Ab()));
						
						link->template num_left_turn_bays<int>(db_itr->getLeft_Ab());
						link->template num_right_turn_bays<int>(db_itr->getRight_Ab());
						
						link->template left_turn_bay_length<float>(scenario_reference<_Scenario_Interface*>()->meterToFoot<NULLTYPE>(0.0));
						link->template right_turn_bay_length<float>(scenario_reference<_Scenario_Interface*>()->meterToFoot<NULLTYPE>(0.0));

						
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

						link->template free_flow_speed<float>(scenario_reference<_Scenario_Interface*>()->mepsToMiph<NULLTYPE>(db_itr->getSpeed_Ab()) + 10.0);
						link->template maximum_flow_rate<float>(maximum_flow_rate);
						link->template backward_wave_speed<float>(backward_wave_speed);
						link->template jam_density<float>(jam_density);



						max_free_flow_speed<float>( max(max_free_flow_speed<float>(),link->template free_flow_speed<float>()) );

						link->template upstream_intersection<_Intersection_Interface*>()->template outbound_links<_Links_Container_Interface&>().push_back(link);
						link->template downstream_intersection<_Intersection_Interface*>()->template inbound_links<_Links_Container_Interface&>().push_back(link);

						links_container<_Links_Container_Interface&>().push_back(link);
					}

					if(db_itr->getLanes_Ba()>0)
					{
						link=(_Link_Interface*)Allocate<typename _Link_Interface::Component_Type>();

						link_id_dir.id=db_itr->getLink();
						link_id_dir.dir=1;

						net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir]=link;

						link->template upstream_intersection<_Intersection_Interface*>((_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[db_itr->getNode_B()->getNode()]);
						link->template downstream_intersection<_Intersection_Interface*>((_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[db_itr->getNode_A()->getNode()]);

						
						link->template uuid<int>(++link_counter);
						link->template internal_id<int>(link_counter);

						link->template num_lanes<int>(db_itr->getLanes_Ba());
						
						link->template length<float>(scenario_reference<_Scenario_Interface*>()->meterToFoot<NULLTYPE>(db_itr->getLength()));
						link->template speed_limit<float>(scenario_reference<_Scenario_Interface*>()->mepsToMiph<NULLTYPE>(db_itr->getSpeed_Ba()));
						
						link->template num_left_turn_bays<int>(db_itr->getLeft_Ba());
						link->template num_right_turn_bays<int>(db_itr->getRight_Ba());
						
						link->template left_turn_bay_length<float>(scenario_reference<_Scenario_Interface*>()->meterToFoot<NULLTYPE>(0.0));
						link->template right_turn_bay_length<float>(scenario_reference<_Scenario_Interface*>()->meterToFoot<NULLTYPE>(0.0));

						
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

						link->template free_flow_speed<float>(scenario_reference<_Scenario_Interface*>()->mepsToMiph<NULLTYPE>(db_itr->getSpeed_Ba()) + 10.0);
						link->template maximum_flow_rate<float>(maximum_flow_rate);
						link->template backward_wave_speed<float>(backward_wave_speed);
						link->template jam_density<float>(jam_density);



						max_free_flow_speed<float>( max(max_free_flow_speed<float>(),link->template free_flow_speed<float>()) );

						link->template upstream_intersection<_Intersection_Interface*>()->template outbound_links<_Links_Container_Interface&>().push_back(link);
						link->template downstream_intersection<_Intersection_Interface*>()->template inbound_links<_Links_Container_Interface&>().push_back(link);

						links_container<_Links_Container_Interface&>().push_back(link);
					}
				}


				define_container_and_value_interface(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, get_type_of(turn_movements_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				_Turn_Movement_Interface* turn_movement;
				get_type_of(turn_movements_container)& turn_movements_monitor=turn_movements_container<get_type_of(turn_movements_container)&>();

				result<Connect> connect_result=db->query<Connect>(query<Connect>::true_expr);
				
				counter=-1;

				cout << "Reading Connections" << endl;

				for(result<Connect>::iterator db_itr = connect_result.begin (); db_itr != connect_result.end (); ++db_itr)
				{

					if(counter%10000==0) cout << "\t" << counter << endl;

					turn_movement = (_Turn_Movement_Interface*)Allocate<typename _Turn_Movement_Interface::Component_Type>();

					link_id_dir.id=db_itr->getLink()->getLink();
					link_id_dir.dir=db_itr->getDir();
					
					assert(net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir));
					
					counter++;
					link=(_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir];

					turn_movement->template inbound_link<_Link_Interface*>(link);

					int target_intersection_id=access(_Intersection_Interface,link->template downstream_intersection).internal_id<int>();


					link_id_dir.id=db_itr->getTo_Link()->getLink();
					link_id_dir.dir=0;

					if(net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir))
					{
						link=(_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir];

						if(access(_Intersection_Interface,link->template upstream_intersection).internal_id<int>()!=target_intersection_id)
						{
							link_id_dir.dir=1;
							assert(net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir));
							link=(_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir];
						}
					}
					else
					{
						link_id_dir.dir=1;
						assert(net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir));
						link=(_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir];
					}

					turn_movement->template outbound_link<_Link_Interface*>(link);

					turn_movement->template uuid<int>(db_itr->getAuto_id());
					turn_movement->template internal_id<int>(counter);

					const string& connect_type=db_itr->getType();

					if(connect_type=="THRU")
					{
						turn_movement->template movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::THROUGH_TURN);
					}
					else if(connect_type=="LEFT" || connect_type=="L_SPLIT" || connect_type=="L_MERGE")
					{
						turn_movement->template movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::LEFT_TURN);
					}
					else if(connect_type=="RIGHT" || connect_type=="R_SPLIT" || connect_type=="R_MERGE")
					{
						turn_movement->template movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::RIGHT_TURN);
					}
					else if(connect_type=="UTURN")
					{
						turn_movement->template movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::U_TURN);
					}
					else
					{
						assert(false);
						break;
					}

					
					turn_movement->template movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);


					turn_movements_container<_Turn_Movements_Container_Interface&>().push_back(turn_movement);
				}

				cout << "Configuring Connections" << endl;

				typename _Turn_Movements_Container_Interface::iterator turn_movements_itr;
				for(turn_movements_itr = turn_movements_container<_Turn_Movements_Container_Interface&>().begin(); turn_movements_itr != turn_movements_container<_Turn_Movements_Container_Interface&>().end(); turn_movements_itr++)
				{
					_Turn_Movement_Interface* turn_mvmt=(_Turn_Movement_Interface*)(*turn_movements_itr);

					//outbound turn movement of an inbound link
					_Link_Interface* inbound_link = turn_mvmt->template inbound_link<_Link_Interface*>();
					inbound_link->template outbound_turn_movements<_Turn_Movements_Container_Interface&>().push_back(turn_mvmt);
					//inbound turn movement of an outbound link
					_Link_Interface* outbound_link = turn_mvmt->template outbound_link<_Link_Interface*>();
					outbound_link->template inbound_turn_movements<_Turn_Movements_Container_Interface&>().push_back(turn_mvmt);
				}

				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);			

				// configure outbound_inbound_movements
				typename _Intersections_Container_Interface::iterator intersections_itr;

				cout << "Configuring Outbound Inbound Movements" << endl;

				for(intersections_itr = intersections_container<_Intersections_Container_Interface&>().begin(); intersections_itr != intersections_container<_Intersections_Container_Interface&>().end(); intersections_itr++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersections_itr);
					get_type_of(intersections_container)::type_of(unqualified_value)& intersection_monitor=(get_type_of(intersections_container)::type_of(unqualified_value)&)*intersection;

					_Links_Container_Interface& outbound_links = intersection->template outbound_links<_Links_Container_Interface&>();
					
					// skip intersections_container that do not have any outbound links
					if (outbound_links.size() == 0) continue;
					
					typename _Links_Container_Interface::iterator links_itr;
					
					for (links_itr = outbound_links.begin(); links_itr != outbound_links.end(); links_itr++)
					{
						_Link_Interface* outboundLink = (_Link_Interface*)(*links_itr);
						
						_Outbound_Inbound_Movements_Interface* outboundInboundMovements = (_Outbound_Inbound_Movements_Interface*)Allocate<typename _Outbound_Inbound_Movements_Interface::Component_Type>();
						
						outboundInboundMovements->template outbound_link_reference<_Link_Interface*>(outboundLink);
						_Turn_Movements_Container_Interface& inboundTurnMovements = outboundLink->template inbound_turn_movements<_Turn_Movements_Container_Interface&>();
						
						for (int k = 0; k < (int)inboundTurnMovements.size(); k++)
						{
							outboundInboundMovements->template inbound_movements<_Turn_Movements_Container_Interface&>().push_back(inboundTurnMovements[k]);
						}

						intersection->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>().push_back(outboundInboundMovements);
					}
				}

				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, _Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);

				cout << "Configuring Inbound Outbound Movements" << endl;

				// configure inbound_outbound_movements
				for (int i = 0; i < (int)intersections_container<_Intersections_Container_Interface&>().size(); i++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)intersections_container<_Intersections_Container_Interface&>()[i];
					
					// skip intersections_container that do not have any inbound links
					if (intersection->template inbound_links<_Links_Container_Interface&>().size() == 0) continue;
					
					for (int j = 0; j < (int)intersection->template inbound_links<_Links_Container_Interface&>().size(); j++) 
					{
						_Inbound_Outbound_Movements_Interface* inboundOutboundMovements = (_Inbound_Outbound_Movements_Interface*)Allocate<typename _Inbound_Outbound_Movements_Interface::Component_Type>();
						_Link_Interface* inboundLink = (_Link_Interface*)intersection->template inbound_links<_Links_Container_Interface&>()[j];
						inboundOutboundMovements->template inbound_link_reference<_Link_Interface*>(inboundLink);
						_Turn_Movements_Container_Interface& outboundTurnMovements = inboundLink->template outbound_turn_movements<_Turn_Movements_Container_Interface&>();

						for (int k = 0; k < (int)outboundTurnMovements.size(); k++)
						{
							inboundOutboundMovements->template outbound_movements<_Turn_Movements_Container_Interface&>().push_back(outboundTurnMovements[k]);
						}
						intersection->template inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>().push_back(inboundOutboundMovements);
					}
				}



				cout << "Reading Activity Locations" << endl;

				counter=-1;

				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				activity_locations_container<_Activity_Locations_Container_Interface&>().clear();

				result<Location> location_result=db->query<Location>(query<Location>::true_expr);
				
				// create a single zone
				define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, get_type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
				zones_container<_Zones_Container_Interface&>().clear();
				_Zone_Interface* zone = (_Zone_Interface*)Allocate<typename _Zone_Interface::Component_Type>();
				zone->template uuid<int>(1.0);
				zone->template internal_id<int>(0.0);
				zones_container<_Zones_Container_Interface&>().push_back(zone);

				_Activity_Location_Interface* activity_location;
				int skipped_counter=0;
				for(result<Location>::iterator db_itr = location_result.begin (); db_itr != location_result.end (); ++db_itr)
				{

					link_id_dir.id=db_itr->getLink()->getLink();
					link_id_dir.dir=db_itr->getDir();

					if(!net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir))
					{
						if(++skipped_counter%10000==0)
						{
							cout << skipped_counter << " locations skipped" << endl;
						}
						continue;				
					}

					++counter;
					if(counter%10000==0) cout << "\t" << counter << endl;

					activity_location = (_Activity_Location_Interface*)Allocate<typename _Activity_Location_Interface::Component_Type>();
					assert(net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir));
					link=(_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir];

					activity_location->origin_links<_Links_Container_Interface&>().push_back(link);
					zone->template origin_activity_locations<_Activity_Locations_Container_Interface&>().push_back(activity_location);
					activity_location->destination_links<_Links_Container_Interface&>().push_back(link);
					zone->template destination_activity_locations<_Activity_Locations_Container_Interface&>().push_back(activity_location);
					activity_location->zone<_Zone_Interface*>(zone);
					activity_location->uuid<int>(db_itr->getPrimaryKey());
					activity_location->internal_id<int>(counter);
					
					activity_locations_container<_Activity_Locations_Container_Interface&>().push_back(activity_location);
				}

			}


			feature_prototype void read_network_data(requires(!check_2(TargetType,Types::ODB_Network,is_same) && !check_2(TargetType,Types::Hard_Coded_Network,is_same)))
			{
				assert_check_2(TargetType,Types::ODB_Network,is_same,"TargetType is ill-defined");
				assert_check_2(TargetType,Types::ODB_Network,is_same,"TargetType should indicate ODB_Network if you want to read it in with ODB");
				assert_check_2(TargetType,Types::Hard_Coded_Network,is_same,"TargetType should indicate Hard_Coded_Network if you want to read from file");
			}

			feature_prototype void read_network_data(network_models::network_information::network_data_information::NetworkData& network_data)
			{
				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, get_type_of(turn_movements_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);			
				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, _Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, get_type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
				define_component_interface(_Intersection_Control_Interface, _Intersection_Interface::get_type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);

				int i;
				_Intersection_Interface* intersection;
				for (i = 0; i < network_data.network_node_size; i++) {
					intersection = (_Intersection_Interface*)Allocate<typename _Intersection_Interface::Component_Type>();
					intersection->template uuid<int>(network_data.node_data_array[i].uuid);
					intersection->template internal_id<int>(i);
					intersection->template x_position<float>(network_data.node_data_array[i].x);
					intersection->template y_position<float>(network_data.node_data_array[i].y);
					intersection->template intersection_type<int>(network_data.node_data_array[i].node_type);
					intersection->template intersection_control<_Intersection_Control_Interface*>((_Intersection_Control_Interface*)nullptr);
					intersections_container<_Intersections_Container_Interface&>().push_back(intersection);
				}

				_Link_Interface* link;
				for (i = 0; i < network_data.network_link_size; i++) {
					link = (_Link_Interface*)Allocate<typename _Link_Interface::Component_Type>();
					int upstream_intersection_id = network_data.link_data_array[i].unode_index;
					_Intersection_Interface* upstream_intersection = intersections_container<_Intersections_Container_Interface&>().at(upstream_intersection_id);
					link->template upstream_intersection<_Intersection_Interface*>(upstream_intersection);

					int downstream_intersection_id = network_data.link_data_array[i].dnode_index;
					_Intersection_Interface* downstream_intersection = intersections_container<_Intersections_Container_Interface&>().at(downstream_intersection_id);
					link->template downstream_intersection<_Intersection_Interface*>(downstream_intersection);

					network_models::network_information::network_data_information::LinkData& raw_link = network_data.link_data_array[i];
					link->template uuid<int>(raw_link.uuid);
					link->template internal_id<int>(i);
					link->template num_lanes<int>(raw_link.num_lanes);
					link->template link_type<int>(raw_link.link_type);
					link->template length<float>(raw_link.length);
					link->template speed_limit<float>(raw_link.speed_limit);
					link->template maximum_flow_rate<float>(raw_link.maximum_flow_rate);
					link->template backward_wave_speed<float>(raw_link.backward_wave_speed);
					link->template free_flow_speed<float>(raw_link.free_flow_speed);
					link->template jam_density<float>(raw_link.jam_density);
					link->template left_turn_bay_length<float>(raw_link.left_turn_bay_length);
					link->template right_turn_bay_length<float>(raw_link.right_turn_bay_length);
					link->template num_left_turn_bays<int>(raw_link.num_left_turn_bays);
					link->template num_right_turn_bays<int>(raw_link.num_right_turn_bays);

					links_container<_Links_Container_Interface&>().push_back(link);
				}

				//========================================================================================================
				//Create inbound/outbound links for each intersection
				//--------------------------------------------------------------------------------------------------------
				//
				typename _Links_Container_Interface::iterator links_itr;
				for (links_itr = links_container<_Links_Container_Interface&>().begin(); links_itr != links_container<_Links_Container_Interface&>().end(); links_itr++)
				{
					_Link_Interface* link = (_Link_Interface*)(*links_itr);
					link->template upstream_intersection<_Intersection_Interface*>()->template outbound_links<_Links_Container_Interface&>().push_back(link);
					link->template downstream_intersection<_Intersection_Interface*>()->template inbound_links<_Links_Container_Interface&>().push_back(link);
				}
				
				_Turn_Movement_Interface* turn_movement;
				for (i = 0; i < network_data.network_turn_movement_size; i++)
				{

					turn_movement = (_Turn_Movement_Interface*)Allocate<typename _Turn_Movement_Interface::Component_Type>();
					
					network_models::network_information::network_data_information::TurnMovementData& raw_turn_movement = network_data.turn_movement_data_array[i];

					turn_movement->template inbound_link<_Link_Interface*>(links_container<_Links_Container_Interface&>().at(raw_turn_movement.inbound_link_index));
					turn_movement->template outbound_link<_Link_Interface*>(links_container<_Links_Container_Interface&>().at(raw_turn_movement.outbound_link_index));
					turn_movement->template movement_type<int>(raw_turn_movement.turn_movement_type);
					turn_movement->template movement_rule<int>(raw_turn_movement.turn_movement_rule);

					turn_movement->template uuid<int>(raw_turn_movement.uuid);
					turn_movement->template internal_id<int>(i);
					//// assign the detector
					//turn_movement_1->template detector<Detector_Interface*>(lane->template Detector_Thru<Detector_Interface*>());
					turn_movements_container<_Turn_Movements_Container_Interface&>().push_back(turn_movement);
				}

				typename _Turn_Movements_Container_Interface::iterator turn_movements_itr;
				for (turn_movements_itr = turn_movements_container<_Turn_Movements_Container_Interface&>().begin(); turn_movements_itr != turn_movements_container<_Turn_Movements_Container_Interface&>().end(); turn_movements_itr++)
				{
					_Turn_Movement_Interface* turn_mvmt=(_Turn_Movement_Interface*)(*turn_movements_itr);

					//outbound turn movement of an inbound link
					_Link_Interface* inbound_link = turn_mvmt->template inbound_link<_Link_Interface*>();
					inbound_link->template outbound_turn_movements<_Turn_Movements_Container_Interface&>().push_back(turn_mvmt);
					//inbound turn movement of an outbound link
					_Link_Interface* outbound_link = turn_mvmt->template outbound_link<_Link_Interface*>();
					outbound_link->template inbound_turn_movements<_Turn_Movements_Container_Interface&>().push_back(turn_mvmt);
				}

				// configure outbound_inbound_movements
				typename _Intersections_Container_Interface::iterator intersections_itr;
				for (intersections_itr = intersections_container<_Intersections_Container_Interface&>().begin(); intersections_itr != intersections_container<_Intersections_Container_Interface&>().end(); intersections_itr++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersections_itr);
					_Links_Container_Interface& outbound_links = intersection->template outbound_links<_Links_Container_Interface&>();
					// skip intersections_container that do not have any outbound links
					if (outbound_links.size() == 0)
						continue;
					typename _Links_Container_Interface::iterator links_itr;
					
					for (links_itr = outbound_links.begin(); links_itr != outbound_links.end(); links_itr++)
					{
						_Link_Interface* outboundLink = (_Link_Interface*)(*links_itr);
						
						_Outbound_Inbound_Movements_Interface* outboundInboundMovements = (_Outbound_Inbound_Movements_Interface*)Allocate<typename _Outbound_Inbound_Movements_Interface::Component_Type>();
						
						outboundInboundMovements->template outbound_link_reference<_Link_Interface*>(outboundLink);
						_Turn_Movements_Container_Interface& inboundTurnMovements = outboundLink->template inbound_turn_movements<_Turn_Movements_Container_Interface&>();
						
						for (int k = 0; k < (int)inboundTurnMovements.size(); k++)
						{
							outboundInboundMovements->template inbound_movements<_Turn_Movements_Container_Interface&>().push_back(inboundTurnMovements[k]);
						}
						intersection->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>().push_back(outboundInboundMovements);
					}
					size_t size = intersection->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>().size();
				}

				// configure inbound_outbound_movements
				for (i = 0; i < (int)intersections_container<_Intersections_Container_Interface&>().size(); i++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)intersections_container<_Intersections_Container_Interface&>()[i];
					// skip intersections_container that do not have any inbound links
					if (intersection->template inbound_links<_Links_Container_Interface&>().size() == 0)
						continue;
					
					for (int j = 0; j < (int)intersection->template inbound_links<_Links_Container_Interface&>().size(); j++) 
					{
						_Inbound_Outbound_Movements_Interface* inboundOutboundMovements = (_Inbound_Outbound_Movements_Interface*)Allocate<typename _Inbound_Outbound_Movements_Interface::Component_Type>();
						_Link_Interface* inboundLink = (_Link_Interface*)intersection->template inbound_links<_Links_Container_Interface&>()[j];
						inboundOutboundMovements->template inbound_link_reference<_Link_Interface*>(inboundLink);
						_Turn_Movements_Container_Interface& outboundTurnMovements = inboundLink->template outbound_turn_movements<_Turn_Movements_Container_Interface&>();
						for (int k = 0; k < (int)outboundTurnMovements.size(); k++)
						{
							inboundOutboundMovements->template outbound_movements<_Turn_Movements_Container_Interface&>().push_back(outboundTurnMovements[k]);
						}
						intersection->template inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>().push_back(inboundOutboundMovements);
					}
				}


				_Activity_Location_Interface* activity_location;
				for (i = 0; i < network_data.network_activity_location_size; i++) 
				{
					activity_location = (_Activity_Location_Interface*)Allocate<typename _Activity_Location_Interface::Component_Type>();
					network_models::network_information::network_data_information::ActivityLocationData& raw_activity_location = network_data.activity_location_data_array[i];

					int j;
					int link_index;
					// initialize origins
					for (j = 0; j < raw_activity_location.num_origin_links; j++) 
					{
						link_index = raw_activity_location.origin_link_index_array[j];
						activity_location->template origin_links<_Links_Container_Interface&>().push_back(links_container<_Links_Container_Interface&>().at(link_index));
						activity_location->template origin_link_choice_cdfs<vector<float>&>().push_back(raw_activity_location.origin_link_choice_cdf_array[j]);
					}
					// initialize destinations
					for (j = 0; j < raw_activity_location.num_destination_links; j++) 
					{
						link_index = raw_activity_location.destination_link_index_array[j];
						activity_location->template destination_links<_Links_Container_Interface&>().push_back(links_container<_Links_Container_Interface&>().at(link_index));
						activity_location->template destination_link_choice_cdfs<vector<float>&>().push_back(raw_activity_location.destination_link_choice_cdf_array[j]);
					}
					activity_location->template uuid<int>(raw_activity_location.uuid);
					activity_location->template internal_id<int>(i);
					activity_locations_container<_Activity_Locations_Container_Interface&>().push_back(activity_location);
				}

				_Zone_Interface* zone;
				for (i = 0; i < network_data.network_zone_size; i++)
				{
					zone = (_Zone_Interface*)Allocate<typename _Zone_Interface::Component_Type>();
					network_models::network_information::network_data_information::ZoneData& raw_zone = network_data.zone_data_array[i];

					int j;
					int activity_location_index;

					// initialize origins
					for (j = 0; j < raw_zone.num_origin_activity_locations; j++) 
					{
						activity_location_index = raw_zone.origin_activity_location_index_array[j];
						zone->template origin_activity_locations<_Activity_Locations_Container_Interface&>().push_back(activity_locations_container<_Activity_Locations_Container_Interface&>().at(activity_location_index));
						zone->template origin_activity_location_choice_cdfs<vector<float>&>().push_back(raw_zone.origin_activity_location_choice_cdf_array[j]);
					}
					// initialize destinations
					for (j = 0; j < raw_zone.num_destination_activity_locations; j++) 
					{
						activity_location_index = raw_zone.destination_activity_location_index_array[j];
						zone->template destination_activity_locations<_Activity_Locations_Container_Interface&>().push_back(activity_locations_container<_Activity_Locations_Container_Interface&>().at(activity_location_index));
						zone->template destination_activity_location_choice_cdfs<vector<float>&>().push_back(raw_zone.destination_activity_location_choice_cdf_array[j]);
					}
					
					zone->template uuid<int>(raw_zone.uuid);
					zone->template internal_id<int>(i);
					zones_container<_Zones_Container_Interface&>().push_back(zone);
				}

				for (i = 0; i < network_data.network_activity_location_size; i++)
				{
					network_models::network_information::network_data_information::ActivityLocationData& raw_activity_location = network_data.activity_location_data_array[i];
					_Activity_Location_Interface* activity_location = activity_locations_container<_Activity_Locations_Container_Interface&>()[i];
					activity_location->template zone<_Zone_Interface*>(zones_container<_Zones_Container_Interface&>().at(raw_activity_location.zone_index));
				}
			}

			feature_prototype void initialize_intersection_control()
			{
				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_component_interface(_Intersection_Control_Interface, _Intersection_Interface::get_type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);

				for(int i=0;i< (int)intersections_container<_Intersections_Container_Interface&>().size();i++)
				{
					_Intersection_Interface* intersection = intersections_container<_Intersections_Container_Interface&>()[i];
					intersection->template intersection_control<_Intersection_Control_Interface*>()->template scenario_reference<_Scenario_Interface*>(scenario_reference<_Scenario_Interface*>());
					intersection->template intersection_control<_Intersection_Control_Interface*>()->template set_node_current_control_plan_index<NULLTYPE>();
				}
			}

			feature_prototype void simulation_initialize()
			{

				typedef typename Network_Components::Prototypes::Network_Prototype<ComponentType> _Network_Interface;
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_simple_container_interface(_Int_Container_Interface, _Link_Interface::get_type_of(cached_link_downstream_cumulative_vehicles_array), Random_Access_Sequence_Prototype, int, ComponentType);
				define_container_and_value_interface(_Vehicles_Vector_Interface, _Vehicle_Interface, _Link_Interface::get_type_of(link_origin_vehicle_array), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);				

				

				this->template initialize_intersection_control<NULLTYPE>();

				typename _Links_Container_Interface::iterator links_itr;
				int i, j;
				for (links_itr = links_container<_Links_Container_Interface&>().begin(); links_itr != links_container<_Links_Container_Interface&>().end(); links_itr++)
				{
					_Link_Interface* link = (_Link_Interface*)(*links_itr);
							
					//network_data
					link->template link_origin_cumulative_arrived_vehicles<int>(0.0);
					link->template link_origin_vehicle_array<_Vehicles_Vector_Interface&>().clear();
					
					link->template scenario_reference<_Scenario_Interface*>(scenario_reference<_Scenario_Interface*>());
					link->template network_reference<_Network_Interface*>((_Network_Interface*)this);

					//init link simulation model
					link->template link_capacity<int>(0.0);
					//link_simulation_model: in Kuilin's code but not converted
					link->template link_upstream_arrived_vehicles<int>(0.0);
					link->template link_downstream_departed_vehicles<int>(0.0);
					link->template link_origin_arrived_vehicles<int>(0.0);
					link->template link_origin_departed_vehicles<int>(0.0);
					link->template link_origin_loaded_vehicles<int>(0.0);
					link->template link_destination_arrived_vehicles<int>(0.0);
					link->template link_origin_vehicle_current_position<int>(0.0);
					
					//supply
					link->template link_supply<float>(link->template num_lanes<int>() * link->template length<float>() * link->template jam_density<float>());
					
					//cumulative vehicles
					link->template link_destination_arrived_vehicles<int>(0.0);
					link->template link_origin_cumulative_arrived_vehicles<int>(0.0);
					link->template link_origin_cumulative_departed_vehicles<int>(0.0);
					link->template link_upstream_cumulative_arrived_vehicles<int>(0.0);
					link->template link_upstream_cumulative_vehicles<int>(0.0);
				
					////bwtt and fftt
					float bwtt = (float) (link->template length<float>()/(link->template backward_wave_speed<float>()*5280.0/3600.0)); // in seconds
					float fftt = (float) (link->template length<float>()/(link->template free_flow_speed<float>()*5280.0/3600.0)); //in seconds
					link->template link_fftt<float>(fftt);
					link->template link_bwtt<float>(bwtt);
				
					link->template link_fftt_cached_simulation_interval_size<int>(int(ceil(float(fftt/(float(link->template scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<float>()))))));
					link->template link_bwtt_cached_simulation_interval_size<int>(int(ceil(float(bwtt/(float(link->template scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<float>()))))));

					//downstream cumulative vehicles
					link->template cached_link_downstream_cumulative_vehicles_array<_Int_Container_Interface&>().clear();
					link->template cached_link_downstream_cumulative_vehicles_array<_Int_Container_Interface&>().resize(link->template link_bwtt_cached_simulation_interval_size<int>());
					for (j = 0; j < (int)link->template cached_link_downstream_cumulative_vehicles_array<_Int_Container_Interface&>().size(); j++)
					{
						link->template cached_link_downstream_cumulative_vehicles_array<_Int_Container_Interface&>()[j] = 0;
					}

					//upstream cumulative vehicles
					link->template cached_link_upstream_cumulative_vehicles_array<_Int_Container_Interface&>().clear();
					link->template cached_link_upstream_cumulative_vehicles_array<_Int_Container_Interface&>().resize(link->template link_fftt_cached_simulation_interval_size<int>());
					for (j = 0; j < (int)link->template cached_link_upstream_cumulative_vehicles_array<_Int_Container_Interface&>().size(); j++)
					{
						link->template cached_link_upstream_cumulative_vehicles_array<_Int_Container_Interface&>()[j] = 0;
					}
					
				}
				//determine minimum merge rate
				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Inbound_Movements_Container_Interface, _Inbound_Movement_Interface, _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);

				define_simple_container_interface(_Float_Container_Interface, _Inbound_Movement_Interface::get_type_of(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array), Random_Access_Sequence_Prototype, float, ComponentType);
				define_container_and_value_interface(_Vehicles_Queue_Interface, _Vehicle_Interface_2, _Inbound_Movement_Interface::get_type_of(vehicles_container), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);


				for (i = 0; i < (int)intersections_container<_Intersections_Container_Interface&>().size(); i++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)intersections_container<_Intersections_Container_Interface&>()[i];
					intersection->template network_reference<_Network_Interface*>((_Network_Interface*)this);
					unsigned long seed = scenario_reference<_Scenario_Interface*>()->template iseed<unsigned int>()+intersection->template internal_id<int>()+1;
					(intersection->template rng_stream<typename _Intersection_Interface::get_type_of(rng_stream)&>()).SetSeed(seed);

					intersection->template scenario_reference<_Scenario_Interface*>(scenario_reference<_Scenario_Interface*>());

					_Outbound_Inbound_Movements_Container_Interface& outbound_inbound_movements_container = intersection->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
					typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
					for (outbound_inbound_movements_itr=outbound_inbound_movements_container.begin(),j=0;outbound_inbound_movements_itr!=outbound_inbound_movements_container.end();outbound_inbound_movements_itr++,j++)
					{
						_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);

						_Inbound_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Inbound_Movements_Container_Interface&>();
						typename _Inbound_Movements_Container_Interface::iterator inbound_movement_itr;
						for (inbound_movement_itr=inbound_movements_container.begin();inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++)
						{
							_Inbound_Movement_Interface* inbound_movement = (_Inbound_Movement_Interface*)(*inbound_movement_itr);
							inbound_movement->template movement_capacity<float>(0.0);
							inbound_movement->template movement_demand<float>(0.0);
							inbound_movement->template movement_supply<float>(0.0);
							inbound_movement->template movement_flow<float>(0.0);
							inbound_movement->template movement_transferred<int>(0.0);
							inbound_movement->template turn_movement_cumulative_vehicles<int>(0.0);
							inbound_movement->template turn_movement_cumulative_arrived_vehicles<int>(0.0);
							inbound_movement->template turn_movement_cumulative_shifted_arrived_vehicles<int>(0.0);
							
							//inbound_movement->template vehicles_container<_Vehicles_Queue_Interface&>().clear();
							
							inbound_movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0.0);
							inbound_movement->template inbound_link_departed_time_based_experienced_link_turn_travel_delay<float>(0.0);
							inbound_movement->template cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>().clear();
							inbound_movement->template cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>().clear();
							inbound_movement->template cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>().resize(scenario_reference<_Scenario_Interface*>()->template num_simulation_intervals_per_assignment_interval<int>());							inbound_movement->template cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>().resize(scenario_reference<_Scenario_Interface*>()->template num_simulation_intervals_per_assignment_interval<int>());
							for (j=0;j<scenario_reference<_Scenario_Interface*>()->template num_simulation_intervals_per_assignment_interval<int>();j++)
							{
								inbound_movement->template cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[j] = 0.0;
								inbound_movement->template cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array<_Float_Container_Interface&>()[j] = 0.0;
							}
									
							_Link_Interface* inbound_link = inbound_movement->template inbound_link<_Link_Interface*>();
							inbound_movement->template cached_turn_movement_cumulative_shifted_arrived_vehicles_array<_Int_Container_Interface&>().clear();
							inbound_movement->template cached_turn_movement_cumulative_shifted_arrived_vehicles_array<_Int_Container_Interface&>().resize(inbound_link->template link_fftt_cached_simulation_interval_size<int>());
							for (j=0;j<inbound_link->template link_fftt_cached_simulation_interval_size<int>();j++)
							{
								inbound_movement->template cached_turn_movement_cumulative_shifted_arrived_vehicles_array<_Int_Container_Interface&>()[j] = 0; 
							}

							if (inbound_movement->template movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>() == Intersection_Components::Types::THROUGH_TURN)
							{
								inbound_movement->template minimum_merge_rate<float>(1.0);
							}
							else
							{
								inbound_movement->template minimum_merge_rate<float>(0.2);
							}
						}
					}
				}

				_intersection_finish_target = 0;
				_intersection_finish_counter = 0;
			}

			feature_prototype void increment_intersection_finish_counter()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);		
				if (AtomicIncrement(&_intersection_finish_counter) == _intersection_finish_target)
				{
					_intersection_finish_counter = 0;
					// print output
					printResults<NULLTYPE>();
					
					if (scenario_reference<_Scenario_Interface*>()->template current_time<int>() > scenario_reference<_Scenario_Interface*>()->template simulation_end_time<int>() && scenario_reference<_Scenario_Interface*>()->template network_in_network_vehicles<int>() == 0)
					{
						scenario_reference<_Scenario_Interface*>()->template close_output_files<NULLTYPE>();
						exit(0);
					}
				}
			}

			feature_prototype void increment_intersection_finish_target()
			{
				_intersection_finish_target++;
			}

			feature_prototype void printResults() 
			{
						
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				printf("%s, ", convert_seconds_to_hhmmss(scenario_reference<_Scenario_Interface*>()->template current_time<int>()).c_str());
				printf("loaded=%7d, departed=%7d, arrived=%7d, in_network=%7d\n",scenario_reference<_Scenario_Interface*>()->template network_cumulative_loaded_vehicles<int>(),scenario_reference<_Scenario_Interface*>()->template network_cumulative_departed_vehicles<int>(),scenario_reference<_Scenario_Interface*>()->template network_cumulative_arrived_vehicles<int>(),scenario_reference<_Scenario_Interface*>()->template network_in_network_vehicles<int>());

				//write_node_control_state<NULLTYPE>();
				//write_vehicle_trajectory<NULLTYPE>();
				//write_network_link_flow<NULLTYPE>();
				write_network_link_turn_time<NULLTYPE>();
				write_output_summary<NULLTYPE>();

			}

			feature_prototype void printNetwork()
			{
				// for debug, we printout everything of the network
				
				cout<<endl<<"***********************************Regular Network************************************************"<<endl;
				cout<<"all links"<<endl;
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				typename _Links_Container_Interface::iterator links_itr;
				for (links_itr = links_container<_Links_Container_Interface&>().begin(); links_itr != links_container<_Links_Container_Interface&>().end(); links_itr++)
				{			

					_Link_Interface* link = (_Link_Interface*)(*links_itr);
					cout<<"\t link_"<<link->template uuid<int>()<<endl;
					cout<<"\t\t upstream: " << link->template upstream_intersection<_Intersection_Interface*>()->template uuid<int>()<<endl;
					cout<<"\t\t downstream: " << link->template downstream_intersection<_Intersection_Interface*>()->template uuid<int>()<<endl;
					cout<<"\t\t num_lanes: " << link->template num_lanes<int>()<<endl;
					cout<<"\t\t link_type: " << link->template link_type<Link_Components::Types::Link_Type_Keys>()<<endl;
					cout<<"\t\t length: " << link->template length<float>()<<endl;
					cout<<"\t\t speed_limit: " << link->template speed_limit<float>()<<endl;
					cout<<"\t\t maximum_flow_rate: " << link->template maximum_flow_rate<float>()<<endl;
					cout<<"\t\t backward_wave_speed: " << link->template backward_wave_speed<float>()<<endl;
					cout<<"\t\t free_flow_speed: " << link->template free_flow_speed<float>()<<endl;
					cout<<"\t\t jam_density: " << link->template jam_density<float>()<<endl;
					cout<<"\t\t left_turn_bay_length: " << link->template left_turn_bay_length<float>()<<endl;
					cout<<"\t\t right_turn_bay_length: " << link->template right_turn_bay_length<float>()<<endl;
					cout<<"\t\t num_left_turn_bays: " << link->template num_left_turn_bays<int>()<<endl;
					cout<<"\t\t num_right_turn_bays: " << link->template num_right_turn_bays<int>()<<endl;
					cout<<"\t\t travel_time: " <<link->template travel_time<float>()<<endl;
					cout<<"\t\t -----------------------------------"<<endl<<endl;
				}

				cout <<"-------------------------------------------------------------------------------------"<<endl;
				cout << "all intersections" << endl;
				
				define_container_and_value_interface(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, get_type_of(turn_movements_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);	
				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, _Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				
				int j, k;
				typename _Intersections_Container_Interface::iterator intersections_itr;
				for (intersections_itr = intersections_container<_Intersections_Container_Interface&>().begin(); intersections_itr != intersections_container<_Intersections_Container_Interface&>().end(); intersections_itr++)
				{			

					_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersections_itr);
					cout<<"\t intersection_"<<intersection->template uuid<int>()<<endl;
					cout<<"\t\t x_position: "<<intersection->template x_position<float>()<<endl;
					cout<<"\t\t y_position: "<<intersection->template y_position<float>()<<endl;
					cout<<"\t\t intersection_type: "<<intersection->template intersection_type<int>()<<endl;
					
					
					
					cout<<"\t\t all inbound_outbound_movements: "<<endl;
					_Inbound_Outbound_Movements_Container_Interface& inbound_outbound_movements_container = intersection->template inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>();
					typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
					for (inbound_outbound_movements_itr=inbound_outbound_movements_container.begin(),j=0;inbound_outbound_movements_itr!=inbound_outbound_movements_container.end();inbound_outbound_movements_itr++,j++)
					{
						_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
						cout<<"\t\t inbound_outbound_movements_"<<j<<endl;
						cout<<"\t\t\t inbound_link: "<<inbound_outbound_movements->template inbound_link_reference<_Link_Interface*>()->template uuid<int>()<<endl;
						cout<<"\t\t\t outbound_movements: "<<endl;
						_Turn_Movements_Container_Interface& outbound_movements_container = inbound_outbound_movements->template outbound_movements<_Turn_Movements_Container_Interface&>();
						typename _Turn_Movements_Container_Interface::iterator outbound_movement_itr;
						for (outbound_movement_itr=outbound_movements_container.begin(),k=0;outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++,k++)
						{
							_Turn_Movement_Interface* outbound_movement = (_Turn_Movement_Interface*)(*outbound_movement_itr);
							cout<<"\t\t\t\t outbound_movements_"<<k<<endl;
							cout<<"\t\t\t\t\t outbound_link: "<<outbound_movement->template outbound_link<_Link_Interface*>()->template uuid<int>()<<endl;
							cout<<"\t\t\t\t\t movement_type: "<<outbound_movement->template movement_type<int>()<<endl;
							cout<<"\t\t\t\t\t forward_link_turn_travel_time: "<<outbound_movement->template forward_link_turn_travel_time<float>()<<endl;
						}
					}

					cout<<"\t\t all outbound_inbound_movements: " <<endl;
					_Outbound_Inbound_Movements_Container_Interface& outbound_inbound_movements_container = intersection->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
					typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
					for (outbound_inbound_movements_itr=outbound_inbound_movements_container.begin(),j=0;outbound_inbound_movements_itr!=outbound_inbound_movements_container.end();outbound_inbound_movements_itr++,j++)
					{
						_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);
						cout<<"\t\t outbound_inbound_movements_"<<j<<endl;
						cout<<"\t\t\t outbound_link: "<<outbound_inbound_movements->template outbound_link_reference<_Link_Interface*>()->template uuid<int>()<<endl;
						cout<<"\t\t\t inbound_movements: "<<endl;
						_Turn_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Turn_Movements_Container_Interface&>();
						typename _Turn_Movements_Container_Interface::iterator inbound_movement_itr;
						for (inbound_movement_itr=inbound_movements_container.begin(),k=0;inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++,k++)
						{
							_Turn_Movement_Interface* inbound_movement = (_Turn_Movement_Interface*)(*inbound_movement_itr);
							cout<<"\t\t\t\t inbound_movements_"<<k<<endl;
							cout<<"\t\t\t\t\t inbound_link: "<<inbound_movement->template inbound_link<_Link_Interface*>()->template uuid<int>()<<endl;
							cout<<"\t\t\t\t\t movement_type: "<<inbound_movement->template movement_type<int>()<<endl;
							cout<<"\t\t\t\t\t forward_link_turn_travel_time: "<<inbound_movement->template forward_link_turn_travel_time<float>()<<endl;
						}
					}
					cout<<"\t\t --------------------------------"<<endl<<endl;
				}

				cout <<"***********************************************************************************"<<endl;
				cout << "all activity locations" << endl;

				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
                int i;
                typename _Activity_Locations_Container_Interface::iterator activity_locations_itr;
                for (activity_locations_itr = activity_locations_container<_Activity_Locations_Container_Interface&>().begin(), i = 0; activity_locations_itr != activity_locations_container<_Activity_Locations_Container_Interface&>().end(); activity_locations_itr++, i++)
                {
                    _Activity_Location_Interface* activity_location = (_Activity_Location_Interface*)(*activity_locations_itr);
					cout<<"\t activity_location_"<<i<<endl;
					cout<<"\t\t zone: "<<activity_location->template zone<int>()<<endl;
					cout<<"\t\t origin_links:"<<endl;
					typename _Links_Container_Interface::iterator origin_link_itr;
					_Links_Container_Interface& origin_links_container = activity_location->template origin_links<_Links_Container_Interface&>();
					for (origin_link_itr=origin_links_container.begin(); origin_link_itr!=origin_links_container.end(); origin_link_itr++)
					{
						cout<<"\t\t link_"<<((_Link_Interface*)(*origin_link_itr))->template uuid<int>()<<endl;
					}
					cout<<"\t\t destination_links:"<<endl;
					typename _Links_Container_Interface::iterator destination_link_itr;
					_Links_Container_Interface& destination_links_container = activity_location->template destination_links<_Links_Container_Interface&>();
					for (destination_link_itr=destination_links_container.begin(); destination_link_itr!=destination_links_container.end(); destination_link_itr++)
					{
						cout<<"\t\t link_"<<((_Link_Interface*)(*destination_link_itr))->template uuid<int>()<<endl;
					}
				}

			}

			feature_prototype void construct_network_cost()
			{

				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				typename _Links_Container_Interface::iterator links_itr;
				max_free_flow_speed<float>(-1.0);			
				for (links_itr = links_container<_Links_Container_Interface&>().begin(); links_itr != links_container<_Links_Container_Interface&>().end(); links_itr++)
				{			

					_Link_Interface* link = (_Link_Interface*)(*links_itr);
					float free_flow_speed = (float) ((5280.0) * link->template free_flow_speed<float>()/3600.0); // feet per second
					float link_travel_time = float (link->template length<float>() / free_flow_speed);
					
					max_free_flow_speed<float>(max(max_free_flow_speed<float>(),free_flow_speed));

					link_travel_time = max((float)1.0,link_travel_time);
					link->template travel_time<float>(link_travel_time);
				}
		
				define_container_and_value_interface(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, get_type_of(turn_movements_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				typename _Turn_Movements_Container_Interface::iterator turn_movements_itr;
				for (turn_movements_itr = turn_movements_container<_Turn_Movements_Container_Interface&>().begin(); turn_movements_itr != turn_movements_container<_Turn_Movements_Container_Interface&>().end(); turn_movements_itr++)
				{
					_Turn_Movement_Interface* mvmt_itf=(_Turn_Movement_Interface*)(*turn_movements_itr);

					_Link_Interface* inboundLink = mvmt_itf->template inbound_link<_Link_Interface*>();
					_Link_Interface* outboundLink = mvmt_itf->template outbound_link<_Link_Interface*>();

					float turn_travel_penalty = 0.0f;
					if (mvmt_itf->movement_rule<int>() == Intersection_Components::Types::PROHIBITED)
					{
						turn_travel_penalty = INFINITY_FLOAT;
					}
					float forward_link_turn_travel_time = inboundLink->template travel_time<float>() + turn_travel_penalty;
					
					mvmt_itf->template turn_travel_penalty<float>(turn_travel_penalty);
					mvmt_itf->template forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
					for (int j = 0; j < (int)mvmt_itf->template replicas_container<_Turn_Movements_Container_Interface&>().size(); j++)
					{
						_Turn_Movement_Interface* replica = (_Turn_Movement_Interface*)mvmt_itf->template replicas_container<_Turn_Movements_Container_Interface&>()[j];
						replica->template forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
				}
			}
			}

			feature_prototype void construct_routable_network()
			{

				define_container_and_value_interface(_Routable_Networks_Container_Interface, _Routable_Network_Interface, get_type_of(routable_networks_container), Random_Access_Sequence_Prototype, Network_Components::Prototypes::Network_Prototype, ComponentType);
				_Routable_Networks_Container_Interface& routable_networks=routable_networks_container<_Routable_Networks_Container_Interface&>();

				for(int i=0;i<_num_threads;i++)
				{
					_Routable_Network_Interface* routable_network = (_Routable_Network_Interface*)Allocate<typename _Routable_Network_Interface::Component_Type>();
					routable_network->template read_routable_network_data<NULLTYPE>(this);
					routable_networks.push_back(routable_network);
				}
			}

			feature_prototype void write_vehicle_trajectory()
			{
				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, get_type_of(turn_movements_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);			
				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, _Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, get_type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, _Link_Interface::get_type_of(link_destination_vehicle_queue), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Movement_Plan_Interface, _Vehicle_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);				
				define_container_and_value_interface(_Trajecotry_Container_Interface, _Trajectory_Unit_Interface, _Movement_Plan_Interface::get_type_of(trajectory_container), Random_Access_Sequence_Prototype, Movement_Plan_Components::Prototypes::Trajectory_Unit_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				fstream& vehicle_trajectory_file = scenario_reference<_Scenario_Interface*>()->template vehicle_trajectory_file<fstream&>();
				
				for(int i = 0; i < (int)this->template links_container<_Links_Container_Interface&>().size(); i++)
				{
					_Link_Interface* destination_link = this->template links_container<_Links_Container_Interface&>()[i];
					int num_arrived_vehicls_of_a_link = (int)destination_link->link_destination_vehicle_queue<_Vehicles_Container_Interface&>().size();
					if (num_arrived_vehicls_of_a_link > 0)
					{//output vehicle trajectory
						while(num_arrived_vehicls_of_a_link)
						{
							_Vehicle_Interface* vehicle = destination_link->template link_destination_vehicle_queue<_Vehicles_Container_Interface&>().front();
							_Movement_Plan_Interface* movement_plan = vehicle->template movement_plan<_Movement_Plan_Interface*>();
							destination_link->link_destination_vehicle_queue<_Vehicles_Container_Interface&>().pop_front();

							int vehicle_id = vehicle->template uuid<int>();
							int origin_zone_index = 0;
							int destination_zone_index = 0;
							int origin_activity_location_index = 0;
							int destination_activity_location_index = 0;
							int origin_link_index = movement_plan->template origin<_Link_Interface*>()->template uuid<int>();
							int destination_link_index = movement_plan->template destination<_Link_Interface*>()->template uuid<int>();
							int num_links = (int)movement_plan->trajectory_container<_Trajecotry_Container_Interface&>().size();

							int departure_time = movement_plan->template departed_time<int>();
							int arrival_time = movement_plan->template arrived_time<int>();
							float travel_time = float ((arrival_time - departure_time)/60.0f);

							
							vehicle_trajectory_file
								<< vehicle_id << ","
								<< 0 << ","
								<< 0 << ","
								<< 0 << ","
								<< 0 << ","
								<< origin_link_index << ","
								<< destination_link_index << ","
								<< num_links << ","
								<< convert_seconds_to_hhmmss(departure_time)<< ","
								<< convert_seconds_to_hhmmss(arrival_time)<< ","
								<< travel_time
								<<endl;

							float path_delayed_time = 0;
							for (int route_link_counter=0;route_link_counter<num_links;route_link_counter++)
							{

								_Trajectory_Unit_Interface* trajectory_unit = movement_plan->trajectory_container<_Trajecotry_Container_Interface&>()[route_link_counter];
								_Link_Interface* route_link = trajectory_unit->template link<_Link_Interface*>();
								int route_link_id = route_link->template uuid<int>();
								int route_link_enter_time = trajectory_unit->enter_time<int>();
								float route_link_delayed_time = float(trajectory_unit->delayed_time<float>()/60.0f);
								
								int route_link_exit_time = movement_plan->get_route_link_exit_time<NULLTYPE>(route_link_counter);
								float route_link_travel_time = float((route_link_exit_time - route_link_enter_time)/60.0f);

								path_delayed_time+=route_link_delayed_time;
					
								vehicle_trajectory_file
									<<route_link_counter + 1 << ","
									<<route_link_id << ","
									<<convert_seconds_to_hhmmss(route_link_enter_time) << ","
									<<route_link_travel_time << ","
									<<route_link_delayed_time
									<<endl;
							}

							//deallocate vehicle

							num_arrived_vehicls_of_a_link--;
						}
					}
				}

			};

			feature_prototype void write_network_link_flow()
			{
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, _Link_Interface::get_type_of(link_destination_vehicle_queue), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);

				int simulation_interval_length = scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>();
				int simulation_interval_index = scenario_reference<_Scenario_Interface*>()->template current_simulation_interval_index<int>();
				
				fstream& network_link_flow_file = scenario_reference<_Scenario_Interface*>()->template network_link_flow_file<fstream&>();
				
				for (int i=0;i < (int)links_container<_Links_Container_Interface&>().size();i++)
				{
					//flow
					_Link_Interface* link = links_container<_Links_Container_Interface&>()[i];
					float bwtt = (float) (link->template length<float>()/(link->template backward_wave_speed<float>()*5280.0/3600.0)); // in seconds
					float fftt = (float) (link->template length<float>()/(link->template free_flow_speed<float>()*5280.0/3600.0)); //in seconds

					int link_fftt_cached_simulation_interval_size = int(ceil(float(fftt/((float)simulation_interval_length))));
					int link_bwtt_cached_simulation_interval_size = int(ceil(float(bwtt/((float)simulation_interval_length))));
					
					network_link_flow_file
						<< convert_seconds_to_hhmmss(simulation_interval_index) <<  ","
						<< i <<  ","
						<< link_fftt_cached_simulation_interval_size <<  ","
						<< link_bwtt_cached_simulation_interval_size <<  ","
						<< link->template link_origin_cumulative_arrived_vehicles<int>() << ","
						<< link->template link_origin_cumulative_departed_vehicles<int>() << ","
						<< link->template link_destination_cumulative_arrived_vehicles<int>() << ","
						<< link->template link_upstream_cumulative_arrived_vehicles<int>() << ","
						<< link->template link_upstream_cumulative_vehicles<int>() << ","
						<< link->template link_shifted_cumulative_arrived_vehicles<int>() << ","
						<< link->template link_downstream_cumulative_vehicles<int>() << ","
						<< link->template link_num_vehicles_in_queue<int>()
						<<endl;		
				}
			};

			feature_prototype void write_network_link_turn_time()
			{
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, _Link_Interface::get_type_of(link_destination_vehicle_queue), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_container_and_value_interface(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, get_type_of(turn_movements_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);

				int simulation_interval_length = scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>();
				int simulation_interval_index = scenario_reference<_Scenario_Interface*>()->template current_simulation_interval_index<int>();
				int simulation_start_time = scenario_reference<_Scenario_Interface*>()->template simulation_start_time<int>();
				int current_starting_time = simulation_start_time + simulation_interval_length*simulation_interval_index;

				fstream& network_link_turn_time_file = scenario_reference<_Scenario_Interface*>()->template network_link_turn_time_file<fstream&>();
				for (int i=0; i < (int)turn_movements_container<_Turn_Movements_Container_Interface&>().size();i++)
				{
					_Turn_Movement_Interface* movement = turn_movements_container<_Turn_Movements_Container_Interface&>()[i];
					_Link_Interface* link = movement->inbound_link<_Link_Interface*>();
					
					float fftt = (float) (movement->inbound_link<_Link_Interface*>()->template length<float>()/(movement->inbound_link<_Link_Interface*>()->template free_flow_speed<float>()*5280.0/3600.0)); //in seconds
					int link_fftt_cached_simulation_interval_size = int(ceil(float(fftt/(simulation_interval_length*1.0))));
					
					
					network_link_turn_time_file
						<< convert_seconds_to_hhmmss(current_starting_time) <<  ","
						<< current_starting_time << ","
						<< movement->template inbound_link<_Link_Interface*>()->template downstream_intersection<_Intersection_Interface*>()->uuid<int>() << ","
						<< movement->template uuid<int>() <<  ","
						<< movement->template inbound_link<_Link_Interface*>()->template uuid<int>() <<  ","
						<< movement->template outbound_link<_Link_Interface*>()->template uuid<int>() <<  ","
						<< fftt << ","
						<< link_fftt_cached_simulation_interval_size*simulation_interval_length << ","

						<< movement->template inbound_link<_Link_Interface*>()->template link_supply<float>() << ","
						<< movement->template outbound_link<_Link_Interface*>()->template link_supply<float>() << ","
						<< movement->template inbound_link<_Link_Interface*>()->template link_origin_arrived_vehicles<int>() << ","
						<<movement->template outbound_link<_Link_Interface*>()->template link_origin_arrived_vehicles<int>() << ","
						<< movement->template inbound_link<_Link_Interface*>()->template link_origin_departed_vehicles<int>() << ","
						<<movement->template outbound_link<_Link_Interface*>()->template link_origin_departed_vehicles<int>() << ","
						<< movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>() << ","
						<< movement->template green_time<float>() << ","
						<< movement->template movement_capacity<float>() << ","
						<< movement->template movement_demand<float>() << ","
						<< movement->template movement_supply<float>() << ","
						<< movement->template movement_flow<float>() << ","
						<< movement->template movement_transferred<float>() << ","
						//<< movement->template vehicles_container<_Vehicles_Container_Interface&>().size() << ","
						<< movement->template turn_travel_penalty<float>() << ","
						<< movement->template forward_link_turn_travel_time<float>()
						<<endl;

					
					

				}

			};

			feature_prototype void write_node_control_state()
			{

				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_component_interface(_Intersection_Control_Interface, _Intersection_Interface::get_type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, _Intersection_Control_Interface::get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				define_container_and_value_interface(_Phases_Container_Interface, _Phase_Interface, _Control_Plan_Interface::get_type_of(phase_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Phase_Prototype, ComponentType);
				define_container_and_value_interface(_Phase_Movements_Container_Interface, _Phase_Movement_Interface, _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Phase_Movement_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_component_interface(_Movement_Interface, _Phase_Movement_Interface::get_type_of(movement), Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, get_type_of(turn_movements_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Approaches_Container_Interface, _Approach_Interface, _Control_Plan_Interface::get_type_of(approach_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Approach_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				fstream& network_node_control_state_file = scenario_reference<_Scenario_Interface*>()->template network_node_control_state_file<fstream&>();
				for (int i = 0; i < (int)intersections_container<_Intersections_Container_Interface&>().size(); i++)
				{
					_Intersection_Interface* intersection = intersections_container<_Intersections_Container_Interface&>()[i];
					_Intersection_Control_Interface* intersection_control = intersection->template intersection_control<_Intersection_Control_Interface*>();
					_Control_Plan_Interface* current_control_plan = intersection_control->template current_control_plan<_Control_Plan_Interface*>();
					int starting_time = current_control_plan->template starting_time<int>();
					int ending_time = current_control_plan->template ending_time<int>();
					Node_Type_Keys control_type = current_control_plan->template control_type<Node_Type_Keys>();

					network_node_control_state_file
						<< convert_seconds_to_hhmmss(scenario_reference<_Scenario_Interface*>()->template current_time<int>()) <<  ","
						<< scenario_reference<_Scenario_Interface*>()->template current_simulation_interval_index<int>() <<  ","
						<< scenario_reference<_Scenario_Interface*>()->template current_simulation_time<int>() <<  ","
						<< intersection->template uuid<int>() <<  ","
						<< current_control_plan->template control_plan_index<int>() <<  ","
						<< current_control_plan->template control_type<int>() <<  ","
						<< convert_seconds_to_hhmmss(starting_time) <<  ","
						<< convert_seconds_to_hhmmss(ending_time) <<  ","
						<< current_control_plan->template approach_data_array<_Approaches_Container_Interface&>().size() <<  ",";
		
					if (control_type == PRE_TIMED_SIGNAL_CONTROL || control_type == ACTUATED_SIGNAL_CONTROL)
					{
						int num_phases = (int)current_control_plan->template phase_data_array<_Phases_Container_Interface&>().size();
						int last_phase = num_phases - 1;
						int cycle_index = current_control_plan->template cycle_index<int>();
						int cycle_starting_time = current_control_plan->template cycle_starting_time<int>();
						int cycle_ending_time = current_control_plan->template cycle_ending_time<int>();
						int cycle_length = cycle_ending_time - cycle_starting_time;

						network_node_control_state_file
							<< cycle_index <<  ","
							<< cycle_length <<  ","
							<< convert_seconds_to_hhmmss(cycle_starting_time) <<  ","
							<< convert_seconds_to_hhmmss(cycle_ending_time) <<  ","
							<< num_phases << ",";

						for (int iphase=0;iphase<num_phases;iphase++)
						{
							_Phase_Interface* phase = current_control_plan->template phase_data_array<_Phases_Container_Interface&>()[iphase];
							int green_starting_time = phase->template green_starting_time<int>();
							int yellow_starting_time = cycle_starting_time +  phase->template yellow_starting_time<int>();
							int red_starting_time = cycle_starting_time + phase->template red_start_time<int>();
							int phase_end_time = red_starting_time + phase->template all_red_time<int>();

							network_node_control_state_file
								<< iphase <<  ","
								<< convert_seconds_to_hhmmss(green_starting_time) <<  ","
								<< convert_seconds_to_hhmmss(yellow_starting_time) <<  ","
								<< convert_seconds_to_hhmmss(red_starting_time) <<  ","
								<< convert_seconds_to_hhmmss(phase_end_time) <<  ",";
						}
					}
					network_node_control_state_file <<endl;
				}
			}

			feature_prototype void write_output_summary()
			{

				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_component_interface(_Intersection_Control_Interface, _Intersection_Interface::get_type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, _Intersection_Control_Interface::get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				define_container_and_value_interface(_Phases_Container_Interface, _Phase_Interface, _Control_Plan_Interface::get_type_of(phase_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Phase_Prototype, ComponentType);
				define_container_and_value_interface(_Phase_Movements_Container_Interface, _Phase_Movement_Interface, _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Phase_Movement_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_component_interface(_Movement_Interface, _Phase_Movement_Interface::get_type_of(movement), Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, get_type_of(turn_movements_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Approaches_Container_Interface, _Approach_Interface, _Control_Plan_Interface::get_type_of(approach_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Approach_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				fstream& output_summary_file = scenario_reference<_Scenario_Interface*>()->template output_summary_file<fstream&>();

				output_summary_file 
					<< convert_seconds_to_hhmmss(6*((int)scenario_reference<_Scenario_Interface*>()->template current_time<int>()/6)).c_str() << ","
					<< scenario_reference<_Scenario_Interface*>()->template network_cumulative_loaded_vehicles<int>() <<  ","
					<< scenario_reference<_Scenario_Interface*>()->template network_cumulative_departed_vehicles<int>() << ","
					<< scenario_reference<_Scenario_Interface*>()->template network_cumulative_arrived_vehicles<int>() << ","
					<< scenario_reference<_Scenario_Interface*>()->template network_in_network_vehicles<int>()
					<<endl;
			}

			feature_prototype void write_network_data(network_models::network_information::network_data_information::NetworkData& network_data)
			{
				define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, get_type_of(turn_movements_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);			
				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, _Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, get_type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);

				network_models::network_information::network_data_information::network_data_initialization(network_data);

				int i;
				cout << "writing intersections" << endl;
				for (i = 0; i < (int)intersections_container<_Intersections_Container_Interface&>().size(); i++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)intersections_container<_Intersections_Container_Interface&>()[i];
					network_models::network_information::network_data_information::NodeData node_data;
					node_data.uuid = intersection->template uuid<int>();
					node_data.node_index = intersection->template internal_id<int>();
					node_data.x = intersection->template x_position<float>();
					node_data.y = intersection->template y_position<float>();
					node_data.node_type = intersection->template intersection_type<network_models::network_information::network_data_information::Node_Type_Keys>();
					network_data.node_data_array.push_back(node_data);
				}
				network_data.network_node_size = (int)network_data.node_data_array.size();
				cout << "writing links" << endl;
				network_data.network_link_size = (int)links_container<_Links_Container_Interface&>().size();
				for (i=0;i < (int)links_container<_Links_Container_Interface&>().size();i++)
				{
					//flow
					_Link_Interface* link = links_container<_Links_Container_Interface&>()[i];
					_Intersection_Interface* upstream_intersection = link->template upstream_intersection<_Intersection_Interface*>();
					network_models::network_information::network_data_information::LinkData link_data;
					
					link_data.unode_index = upstream_intersection->template internal_id<int>();

					_Intersection_Interface* downstream_intersection = link->template downstream_intersection<_Intersection_Interface*>();
					link_data.dnode_index = downstream_intersection->template internal_id<int>();

					link_data.uuid = link->template uuid<int>();
					link_data.link_index = link->template internal_id<int>();
					link_data.num_lanes = link->template num_lanes<int>();
					link_data.link_type = link->template link_type<network_models::network_information::network_data_information::Link_Type_Keys>();
					link_data.length = link->template length<float>();
					link_data.speed_limit = link->template speed_limit<float>();
					link_data.maximum_flow_rate = link->template maximum_flow_rate<float>();
					link_data.backward_wave_speed = link->template backward_wave_speed<float>();
					link_data.free_flow_speed = link->template free_flow_speed<float>();
					link_data.jam_density = link->template jam_density<float>();
					link_data.left_turn_bay_length = link->template left_turn_bay_length<float>();
					link_data.right_turn_bay_length = link->template right_turn_bay_length<float>();
					link_data.num_left_turn_bays = link->template num_left_turn_bays<int>();
					link_data.num_right_turn_bays = link->template num_right_turn_bays<int>();

					network_data.link_data_array.push_back(link_data);
				}
				network_data.network_link_size = (int)network_data.link_data_array.size();

				//========================================================================================================
				//Create inbound/outbound links for each intersection
				//--------------------------------------------------------------------------------------------------------
				//
				typename _Links_Container_Interface::iterator links_itr;
				for (i = 0; i < (int)links_container<_Links_Container_Interface&>().size(); i++)
				{
					_Link_Interface* link = links_container<_Links_Container_Interface&>()[i];
					int unode_index = network_data.link_data_array[link->template internal_id<int>()].unode_index;
					network_data.node_data_array[unode_index].outbound_link_index_array.push_back(network_data.link_data_array[link->template internal_id<int>()].link_index);
					
					int dnode_index = network_data.link_data_array[link->template internal_id<int>()].dnode_index;
					network_data.node_data_array[dnode_index].inbound_link_index_array.push_back(network_data.link_data_array[link->template internal_id<int>()].link_index);
				}

				for (i = 0; i < network_data.network_node_size; i++) {
					network_data.node_data_array[i].outbound_link_size = (int)network_data.node_data_array[i].outbound_link_index_array.size();
					network_data.node_data_array[i].inbound_link_size = (int)network_data.node_data_array[i].inbound_link_index_array.size();
				}
				cout << "writing turn movements" << endl;
				for (i = 0; i < (int)turn_movements_container<_Turn_Movements_Container_Interface&>().size(); i++)
				{

					_Turn_Movement_Interface* turn_movement = turn_movements_container<_Turn_Movements_Container_Interface&>()[i];
					
					network_models::network_information::network_data_information::TurnMovementData turn_movement_data;

					turn_movement_data.inbound_link_index = turn_movement->template inbound_link<_Link_Interface*>()->template internal_id<int>();
					turn_movement_data.outbound_link_index = turn_movement->template outbound_link<_Link_Interface*>()->template internal_id<int>();
					turn_movement_data.turn_movement_type = turn_movement->template movement_type<network_models::network_information::network_data_information::Turn_Movement_Type_Keys>();
					turn_movement_data.turn_movement_rule = turn_movement->template movement_rule<network_models::network_information::network_data_information::Turn_Movement_Rule_Keys>();
					turn_movement_data.uuid = turn_movement->template uuid<int>();
					turn_movement_data.turn_movement_index = turn_movement->internal_id<int>();
					//// assign the detector
					//turn_movement_1->template detector<Detector_Interface*>(lane->template Detector_Thru<Detector_Interface*>());
					network_data.turn_movement_data_array.push_back(turn_movement_data);
				}
				network_data.network_turn_movement_size = (int)network_data.turn_movement_data_array.size();

				typename _Turn_Movements_Container_Interface::iterator turn_movements_itr;
				for (turn_movements_itr = turn_movements_container<_Turn_Movements_Container_Interface&>().begin(); turn_movements_itr != turn_movements_container<_Turn_Movements_Container_Interface&>().end(); turn_movements_itr++)
				{
					_Turn_Movement_Interface* turn_mvmt=(_Turn_Movement_Interface*)(*turn_movements_itr);
					network_models::network_information::network_data_information::TurnMovementData& turn_movement_data = network_data.turn_movement_data_array[turn_mvmt->template internal_id<int>()];

					//outbound turn movement of an inbound link
					_Link_Interface* inbound_link = turn_mvmt->template inbound_link<_Link_Interface*>();
					
					network_data.link_data_array[inbound_link->template internal_id<int>()].outbound_turn_movement_index_array.push_back(turn_movement_data.turn_movement_index);
					
					//inbound turn movement of an outbound link
					_Link_Interface* outbound_link = turn_mvmt->template outbound_link<_Link_Interface*>();
					
					network_data.link_data_array[outbound_link->template internal_id<int>()].inbound_turn_movement_index_array.push_back(turn_movement_data.turn_movement_index);
				}

				for (i = 0; i < network_data.network_link_size; i++)
				{
					network_data.link_data_array[i].outbound_turn_movement_size = (int)network_data.link_data_array[i].outbound_turn_movement_index_array.size();
					network_data.link_data_array[i].inbound_turn_movement_size = (int)network_data.link_data_array[i].inbound_turn_movement_index_array.size();
				}
	
				cout << "writing activity locations" << endl;				
				for (i = 0; i < (int)activity_locations_container<_Activity_Locations_Container_Interface&>().size(); i++) 
				{
					_Activity_Location_Interface* activity_location = activity_locations_container<_Activity_Locations_Container_Interface&>()[i];
					
					network_models::network_information::network_data_information::ActivityLocationData activity_location_data;

					int j;
					int link_index;
					// initialize origins
					for (j = 0; j < (int)activity_location->template origin_links<_Links_Container_Interface&>().size(); j++) 
					{
						link_index = activity_location->template origin_links<_Links_Container_Interface&>()[j]->template internal_id<int>();
						activity_location_data.origin_link_index_array.push_back(link_index);
						activity_location_data.origin_link_choice_cdf_array.push_back(link_index);
					}
					activity_location_data.num_origin_links = (int)activity_location->template origin_links<_Links_Container_Interface&>().size();

					// initialize destinations
					for (j = 0; j < (int)activity_location->template destination_links<_Links_Container_Interface&>().size(); j++) 
					{
						link_index = activity_location->template destination_links<_Links_Container_Interface&>()[j]->template internal_id<int>();
						activity_location_data.destination_link_index_array.push_back(link_index);
						activity_location_data.destination_link_choice_cdf_array.push_back(link_index);
					}
					activity_location_data.num_destination_links = (int)activity_location->template destination_links<_Links_Container_Interface&>().size();

					activity_location_data.uuid = activity_location->template uuid<int>();
					activity_location_data.activity_location_index = activity_location->template internal_id<int>();
					activity_location_data.zone_index = activity_location->template zone<_Zone_Interface*>()-> template internal_id<int>();
					network_data.activity_location_data_array.push_back(activity_location_data);
				}
				network_data.network_activity_location_size = (int)network_data.activity_location_data_array.size();

				cout << "writing zones" << endl;			
				for (i = 0; i < (int)zones_container<_Zones_Container_Interface&>().size(); i++)
				{
					_Zone_Interface* zone = zones_container<_Zones_Container_Interface&>()[i];
					
					network_models::network_information::network_data_information::ZoneData zone_data;

					int j;
					int activity_location_index;

					// initialize origins
					for (j = 0; j < (int)zone->template origin_activity_locations<_Activity_Locations_Container_Interface&>().size(); j++) 
					{
						activity_location_index = zone->template origin_activity_locations<_Activity_Locations_Container_Interface&>()[j]->template internal_id<int>();
						zone_data.origin_activity_location_index_array.push_back(activity_location_index);
						zone_data.origin_activity_location_choice_cdf_array.push_back(activity_location_index);
					}
					zone_data.num_origin_activity_locations = (int)zone_data.origin_activity_location_index_array.size();

					// initialize destinations
					for (j = 0; j < (int)zone->template destination_activity_locations<_Activity_Locations_Container_Interface&>().size(); j++) 
					{
						activity_location_index = zone->template destination_activity_locations<_Activity_Locations_Container_Interface&>()[j]->template internal_id<int>();
						zone_data.destination_activity_location_index_array.push_back(activity_location_index);
						zone_data.destination_activity_location_choice_cdf_array.push_back(activity_location_index);
					}
					zone_data.num_destination_activity_locations = (int)zone_data.destination_activity_location_index_array.size();

					zone_data.uuid = zone->template uuid<int>();
					zone_data.zone_index = zone->template internal_id<int>();
					network_data.zone_data_array.push_back(zone_data);
				}
				network_data.network_zone_size = (int)network_data.zone_data_array.size();
			}

			feature_prototype void Reset()
			{
				typedef typename ComponentType::scan_list_type ScanListType;
				scan_list<ScanListType&>().clear();
	
				define_container_and_value_interface(_Reversed_Path_Container_Interface, _Regular_Link_Interface, get_type_of(reversed_path_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);  
				reversed_path_container<_Reversed_Path_Container_Interface&>().clear();
				define_container_and_value_interface(_Routable_Links_Container_Interface, _Routable_Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				typename _Routable_Links_Container_Interface::iterator link_itr;

				for(link_itr=links_container<_Routable_Links_Container_Interface&>().begin();link_itr!=links_container<_Routable_Links_Container_Interface&>().end();link_itr++)
				{
					_Routable_Link_Interface* link_ptr = (_Routable_Link_Interface*)(*link_itr);
					link_ptr->template label_cost<float>(INFINITY_FLOAT);
					link_ptr->template label_pointer<_Routable_Link_Interface*>(link_ptr);
					link_ptr->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>(Network_Components::Types::UNSCANNED);
					link_ptr->template f_cost<float>(0.0);
					link_ptr->template h_cost<float>(0.0);
				}
			}

			feature_prototype void read_routable_network_data(void* regular_network)
			{
				define_container_and_value_interface(_Routable_Links_Container_Interface, _Routable_Link_Interface, get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Intersections_Container_Interface, _Routable_Intersection_Interface, get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Outbound_Inbound_Movements_Container_Interface, _Routable_Outbound_Inbound_Movements_Interface, _Routable_Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype,Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Inbound_Outbound_Movements_Container_Interface, _Routable_Inbound_Outbound_Movements_Interface, _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Movements_Container_Interface, _Routable_Movement_Interface, _Routable_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);

				typedef Network_Components::Prototypes::Network_Prototype<typename ComponentType::regular_network_type, ComponentType> _Regular_Network_Interface;
				define_container_and_value_interface(_Regular_Links_Container_Interface, _Regular_Link_Interface, ComponentType::regular_network_type::links_container_type, Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Intersections_Container_Interface, _Regular_Intersection_Interface, ComponentType::regular_network_type::intersections_container_type, Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Outbound_Inbound_Movements_Container_Interface, _Regular_Outbound_Inbound_Movements_Interface, _Regular_Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Inbound_Outbound_Movements_Container_Interface, _Regular_Inbound_Outbound_Movements_Interface, _Regular_Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Movements_Container_Interface, _Regular_Movement_Interface, _Regular_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				

				max_free_flow_speed<float>(((_Regular_Network_Interface*)regular_network)->template max_free_flow_speed<float>());

				
				_Regular_Links_Container_Interface& regular_links_container = ((_Regular_Network_Interface*)regular_network)->template links_container<_Regular_Links_Container_Interface&>();
				
				// copy all links
				cout << "copying links" << endl;
				dense_hash_map<int, _Routable_Link_Interface*> linksMap;
				linksMap.set_empty_key(-1);
				linksMap.set_deleted_key(-2);
				typename _Regular_Links_Container_Interface::iterator regular_link_itr;
				for(regular_link_itr=regular_links_container.begin(); regular_link_itr!=regular_links_container.end(); regular_link_itr++)
				{
					_Regular_Link_Interface* regular_link = (_Regular_Link_Interface*)(*regular_link_itr);
					_Routable_Link_Interface* routable_link =  (_Routable_Link_Interface*)Allocate<typename _Routable_Link_Interface::Component_Type>();
					routable_link->template network_link_reference<_Regular_Link_Interface*>(regular_link);
					routable_link->template f_cost<float>(0.0);
					routable_link->template h_cost<float>(0.0);
					routable_link->template label_pointer<void*>(routable_link);
					routable_link->template label_cost<float>(0.0);
					routable_link->template uuid<int>(regular_link->template uuid<int>());
					routable_link->template internal_id<int>(regular_link->template internal_id<int>());
					routable_link->template travel_time<float>(regular_link->template travel_time<float>());
					links_container<_Routable_Links_Container_Interface&>().push_back(routable_link);
					linksMap.insert(pair<int, _Routable_Link_Interface*>(regular_link->template internal_id<int>(), routable_link));
					regular_link->template replicas_container<_Routable_Links_Container_Interface&>().push_back(routable_link);
				}
				cout << "copying intersections" << endl;
				// copy all intersections
				int counter = -1;
				_Regular_Intersections_Container_Interface& regular_intersections_container = ((_Regular_Network_Interface*)regular_network)->template intersections_container<_Regular_Intersections_Container_Interface&>();
				dense_hash_map<int, _Routable_Intersection_Interface*> intersectionsMap;
				intersectionsMap.set_empty_key(-1);
				intersectionsMap.set_deleted_key(-2);
				typename _Regular_Intersections_Container_Interface::iterator regular_intersection_itr;
				for(regular_intersection_itr=regular_intersections_container.begin(); regular_intersection_itr!=regular_intersections_container.end(); regular_intersection_itr++)
				{
					if (++counter % 10000 == 0)
					{
						cout << counter << " intersections copied" << endl;
					}
					_Regular_Intersection_Interface* regular_intersection = (_Regular_Intersection_Interface*)(*regular_intersection_itr);
					_Routable_Intersection_Interface* routable_intersection =  (_Routable_Intersection_Interface*)Allocate<typename _Routable_Intersection_Interface::Component_Type>();
					routable_intersection->template x_position<float>(regular_intersection->template x_position<float>());
					routable_intersection->template y_position<float>(regular_intersection->template y_position<float>());
					routable_intersection->template uuid<int>(regular_intersection->template uuid<int>());
					routable_intersection->template internal_id<int>(regular_intersection->template internal_id<int>());
					
					// create inbound_outbound_movements
					_Regular_Inbound_Outbound_Movements_Container_Interface& regular_inbound_outbound_movements_container = regular_intersection->template inbound_outbound_movements<_Regular_Inbound_Outbound_Movements_Container_Interface&>();
					typename _Regular_Inbound_Outbound_Movements_Container_Interface::iterator regular_inbound_outbound_movements_itr;
					
					for(regular_inbound_outbound_movements_itr=regular_inbound_outbound_movements_container.begin(); regular_inbound_outbound_movements_itr!=regular_inbound_outbound_movements_container.end(); regular_inbound_outbound_movements_itr++)
					{
						_Regular_Inbound_Outbound_Movements_Interface* regular_inbound_outbound_movements = (_Regular_Inbound_Outbound_Movements_Interface*)(*regular_inbound_outbound_movements_itr);
						_Routable_Inbound_Outbound_Movements_Interface* routable_inbound_outbound_movements = (_Routable_Inbound_Outbound_Movements_Interface*)Allocate<typename _Routable_Inbound_Outbound_Movements_Interface::Component_Type>();
						_Regular_Link_Interface* regular_link = regular_inbound_outbound_movements->template inbound_link_reference<_Regular_Link_Interface*>();
						_Routable_Link_Interface* routable_link = linksMap.find(regular_link->template internal_id<int>())->second;
						routable_inbound_outbound_movements->template inbound_link_reference<_Routable_Link_Interface*>(routable_link);
						//another level of loop
						_Regular_Movements_Container_Interface& regular_outbound_movements_container = regular_inbound_outbound_movements->template outbound_movements<_Regular_Movements_Container_Interface&>();
						typename _Regular_Movements_Container_Interface::iterator regular_outbound_movement_itr;
						for(regular_outbound_movement_itr=regular_outbound_movements_container.begin(); regular_outbound_movement_itr!=regular_outbound_movements_container.end(); regular_outbound_movement_itr++)
						{
							_Regular_Movement_Interface* regular_outbound_movement = (_Regular_Movement_Interface*)(*regular_outbound_movement_itr);
							_Routable_Movement_Interface* routable_outbound_movement = (_Routable_Movement_Interface*)Allocate<typename _Routable_Movement_Interface::Component_Type>();
							routable_outbound_movement->template forward_link_turn_travel_time<float>(regular_outbound_movement->template forward_link_turn_travel_time<float>());
							_Regular_Link_Interface* regular_outbound_link = regular_outbound_movement->template outbound_link<_Regular_Link_Interface*>();
							_Routable_Link_Interface* routable_outbound_link = linksMap.find(regular_outbound_link->template internal_id<int>())->second;
							routable_outbound_movement->template outbound_link<_Routable_Link_Interface*>(routable_outbound_link);
							regular_outbound_movement->template replicas_container<_Routable_Movements_Container_Interface&>().push_back(routable_outbound_movement);
							routable_inbound_outbound_movements->template outbound_movements<_Routable_Movements_Container_Interface&>().push_back(routable_outbound_movement);
						}
						routable_intersection->template inbound_outbound_movements<_Routable_Inbound_Outbound_Movements_Container_Interface&>().push_back(routable_inbound_outbound_movements);
					}
					
					// create outbound_inbound_movements
					_Regular_Outbound_Inbound_Movements_Container_Interface& regular_outbound_inbound_movements_container = regular_intersection->template outbound_inbound_movements<_Regular_Outbound_Inbound_Movements_Container_Interface&>();
					typename _Regular_Outbound_Inbound_Movements_Container_Interface::iterator regular_outbound_inbound_movements_itr;
					
					for(regular_outbound_inbound_movements_itr=regular_outbound_inbound_movements_container.begin(); regular_outbound_inbound_movements_itr!=regular_outbound_inbound_movements_container.end(); regular_outbound_inbound_movements_itr++)
					{
						_Regular_Outbound_Inbound_Movements_Interface* regular_outbound_inbound_movements = (_Regular_Outbound_Inbound_Movements_Interface*)(*regular_outbound_inbound_movements_itr);
						_Routable_Outbound_Inbound_Movements_Interface* routable_outbound_inbound_movements = (_Routable_Outbound_Inbound_Movements_Interface*)Allocate<typename _Routable_Outbound_Inbound_Movements_Interface::Component_Type>();
						_Regular_Link_Interface* regular_link = regular_outbound_inbound_movements->template outbound_link_reference<_Regular_Link_Interface*>();
						_Routable_Link_Interface* routable_link = linksMap.find(regular_link->template internal_id<int>())->second;
						routable_outbound_inbound_movements->template outbound_link_reference<_Routable_Link_Interface*>(routable_link);
						//another level of loop
						_Regular_Movements_Container_Interface& regular_inbound_movements_container = regular_outbound_inbound_movements->template inbound_movements<_Regular_Movements_Container_Interface&>();
						typename _Regular_Movements_Container_Interface::iterator regular_inbound_movement_itr;
						for(regular_inbound_movement_itr=regular_inbound_movements_container.begin(); regular_inbound_movement_itr!=regular_inbound_movements_container.end(); regular_inbound_movement_itr++)
						{
							_Regular_Movement_Interface* regular_inbound_movement = (_Regular_Movement_Interface*)(*regular_inbound_movement_itr);
							_Routable_Movement_Interface* routable_inbound_movement = (_Routable_Movement_Interface*)Allocate<typename _Routable_Movement_Interface::Component_Type>();
							routable_inbound_movement->template forward_link_turn_travel_time<float>(regular_inbound_movement->template forward_link_turn_travel_time<float>());
							_Regular_Link_Interface* regular_inbound_link = regular_inbound_movement->template inbound_link<_Regular_Link_Interface*>();
							_Routable_Link_Interface* routable_inbound_link = linksMap.find(regular_inbound_link->template internal_id<int>())->second;
							routable_inbound_movement->template inbound_link<_Routable_Link_Interface*>(routable_inbound_link);
							regular_inbound_movement->template replicas_container<_Routable_Movements_Container_Interface&>().push_back(routable_inbound_movement);
							routable_outbound_inbound_movements->template inbound_movements<_Routable_Movements_Container_Interface&>().push_back(routable_inbound_movement);
						}
						routable_intersection->template outbound_inbound_movements<_Routable_Outbound_Inbound_Movements_Container_Interface&>().push_back(routable_outbound_inbound_movements);
					}

					intersections_container<_Routable_Intersections_Container_Interface&>().push_back(routable_intersection);
					intersectionsMap.insert(pair<int, _Routable_Intersection_Interface*>(regular_intersection->template internal_id<int>(), routable_intersection));
				}
				cout << " adding up/down streams to each link" << endl;
				// add upstream and downstream to each link
				typename _Routable_Links_Container_Interface::iterator routable_link_itr;
				for(routable_link_itr=links_container<_Routable_Links_Container_Interface&>().begin(); routable_link_itr!=links_container<_Routable_Links_Container_Interface&>().end(); routable_link_itr++)
				{

					_Routable_Link_Interface* routable_link = (_Routable_Link_Interface*)(*routable_link_itr);
					_Regular_Link_Interface* regular_link = routable_link->template network_link_reference<_Regular_Link_Interface*>();
		
					_Regular_Intersection_Interface* regular_upstream_intersection = regular_link->template upstream_intersection<_Regular_Intersection_Interface*>();
					_Routable_Intersection_Interface* routable_upstream_intersection = intersectionsMap.find(regular_upstream_intersection->template internal_id<int>())->second;
					routable_link->template upstream_intersection<_Routable_Intersection_Interface*>(routable_upstream_intersection);
					_Regular_Intersection_Interface* regular_downstream_intersection = regular_link->template downstream_intersection<_Regular_Intersection_Interface*>();
					_Routable_Intersection_Interface* routable_downstream_intersection = intersectionsMap.find(regular_downstream_intersection->template internal_id<int>())->second;
					routable_link->template downstream_intersection<_Routable_Intersection_Interface*>(routable_downstream_intersection);
				}
				cout << " streams added" << endl;
			}
		};
	}
}

using namespace Network_Components::Prototypes;