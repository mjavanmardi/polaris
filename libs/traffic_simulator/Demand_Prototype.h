#pragma once

#include "User_Space_Includes.h"
//#include "../File_IO/network_demand_data.h"

/*------------------------------------------------*/

#ifdef Advanced
#include "Transims_Demand_API.h"
#endif

namespace Demand_Components
{
	//==================================================================================================================
	/// TYPE Of interest to the Demand Prototype.
	/// Used in concept checking, component functions, etc.
	//------------------------------------------------------------------------------------------------------------------
	namespace Types
	{
		namespace ModelTypes
		{
			typedef true_type TransimsType;
		}
		namespace FileTypes
		{
			typedef true_type TripFile;
			typedef true_type ControlFile;
		}
		namespace FileFormatTypes
		{
			typedef true_type Tab_Delimited;
			typedef true_type CSV_Delimited;
			typedef true_type Binary;
		}
		
		struct Hard_Coded_Demand{};
		struct ODB_Demand{};
		struct File_Demand{};

	}
	
	namespace Prototypes
	{
		prototype struct Demand
		{
			tag_as_prototype;

			accessor(scenario_reference, NONE, NONE);
			accessor(network_reference, NONE, NONE);
			accessor(vehicles_container, NONE, NONE);
			accessor(first_vehicle_departure_time, NONE, NONE);
			accessor(last_vehicle_departure_time, NONE, NONE);

			template<typename TargetType> void read_demand_data(typename TargetType::ParamType& network_mapping,
				requires(TargetType,check_2(typename TargetType::NetIOType,Network_Components::Types::ODB_Network,is_same)))
			{
				using namespace odb;
				using namespace polaris::io;
				typedef  Scenario_Components::Prototypes::Scenario< typename get_type_of(scenario_reference)> _Scenario_Interface;

				string name(scenario_reference<_Scenario_Interface*>()->template database_name<string&>());
				unique_ptr<database> db (open_sqlite_demand_database (name));

				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;

				typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename _Network_Interface::get_type_of(activity_locations_container)::value_type>::type>  _Activity_Location_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container), _Activity_Location_Interface*> _Activity_Locations_Container_Interface;

				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Activity_Location_Interface::get_type_of(origin_links)::value_type>::type>  _Link_Interface;
				typedef  Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links), _Link_Interface*> _Links_Container_Interface;

				typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename _Network_Interface::get_type_of(zones_container)::data_type>::type>  _Zone_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(zones_container), _Zone_Interface*> _Zones_Container_Interface;
				
				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Link_Interface::get_type_of(outbound_turn_movements)::value_type>::type>  _Movement_Interface;
				typedef  Random_Access_Sequence< typename _Link_Interface::get_type_of(outbound_turn_movements), _Movement_Interface*> _Movements_Container_Interface;

				typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename get_type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;

				typedef Traveler_Components::Prototypes::Traveler<typename ComponentType::traveler_type> _Traveler_Interface;
				typedef  Routing_Components::Prototypes::Routing< typename _Traveler_Interface::get_type_of(router)> _Routing_Interface;
//				define_component_interface(_Plan_Interface, _Traveler_Interface::get_type_of(plan), Plan_Components::Prototypes::Plan_Prototype, ComponentType);
//				define_component_interface(_Movement_Plan_Interface, _Plan_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename get_type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;

				typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename _Vehicle_Interface::get_type_of(movement_plan)> _Movement_Plan_Interface;

				_Network_Interface* network=network_reference<_Network_Interface*>();
				_Activity_Locations_Container_Interface& activity_locations = network->template activity_locations_container<_Activity_Locations_Container_Interface&>();

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();

				transaction t(db->begin());

				result<Trip> trip_result=db->template query<Trip>(query<Trip>::true_expr);
				
				
				_Traveler_Interface* traveler;
				_Vehicle_Interface* vehicle;
				_Routing_Interface* router;
//				_Plan_Interface* plan;
				_Movement_Plan_Interface* movement_plan;
				
				int traveler_id_counter=-1;
				
				
				dense_hash_map<int,_Activity_Location_Interface*> activity_id_to_ptr;
				activity_id_to_ptr.set_empty_key(-1);
				activity_id_to_ptr.set_deleted_key(-2);
				typename _Activity_Locations_Container_Interface::iterator activity_locations_itr;

				cout << "Building Temporary Activity Map" << endl;

				for(activity_locations_itr=activity_locations.begin();activity_locations_itr!=activity_locations.end();activity_locations_itr++)
				{
					_Activity_Location_Interface& activity_location=(_Activity_Location_Interface&)**activity_locations_itr;

					activity_id_to_ptr[activity_location.template uuid<int>()]=&activity_location;
				}

				cout << "Reading Travelers" << endl;


				int counter = -1;
				int departed_time;
				int skipped_counter=0;

				this->template first_vehicle_departure_time<int>(24*60*60.0);
				this->template last_vehicle_departure_time<int>(0.0);


				int simulation_start_time = scenario->template simulation_start_time<int>();
				int simulation_end_time = scenario->template simulation_end_time<int>();

				float demand_percentage= scenario->template demand_reduction_factor<float>();

				cout << "Demand Percentage: " << demand_percentage;

				for(result<Trip>::iterator db_itr = trip_result.begin (); db_itr != trip_result.end (); ++db_itr)
				{
					// perform demand reduction
					if (GLOBALS::Uniform_RNG.Next_Rand<float>() > demand_percentage) continue;


					if (++counter % 100000 == 0)
					{
						cout << counter << " trips processed" << endl;
					}

					departed_time = db_itr->getStart();

					if (departed_time < simulation_start_time || departed_time >= simulation_end_time) {
						continue;
					}

					int org_key=db_itr->getOrigin();

					if(!activity_id_to_ptr.count(org_key))
					{
						if(++skipped_counter%100000==0)
						{
							cout << skipped_counter << " trips skipped" << endl;
						}
						continue;
					}

					int dst_key=db_itr->getDestination();

					if(!activity_id_to_ptr.count(dst_key))
					{
						if(++skipped_counter%100000==0)
						{
							cout << skipped_counter << " trips skipped" << endl;
						}
						continue;
					}

					_Activity_Location_Interface* origin_activity_location = activity_id_to_ptr[org_key];
					_Activity_Location_Interface* destination_activity_location = activity_id_to_ptr[dst_key];
					_Link_Interface* origin_link = origin_activity_location->template origin_links<_Links_Container_Interface&>()[0];
					_Link_Interface* destination_link = destination_activity_location->template destination_links<_Links_Container_Interface&>()[0];
					if (origin_link->template internal_id<int>() == destination_link->template internal_id<int>()  || (origin_link->template outbound_turn_movements<_Movements_Container_Interface&>().size() == 0 || destination_link->template inbound_turn_movements<_Movements_Container_Interface&>().size() == 0))
					{
						// No path can be found. Discard the trip
						continue;
					}

					if (departed_time < this->template first_vehicle_departure_time<int>())
					{
						this->template first_vehicle_departure_time<int>(departed_time);
					}
					else if (departed_time > this->template last_vehicle_departure_time<int>())
					{
						this->template last_vehicle_departure_time<int>(departed_time);
					}

					assert(activity_id_to_ptr.count(org_key));
					assert(activity_id_to_ptr.count(dst_key));

					departed_time = departed_time - scenario->template simulation_start_time<int>();
					
					traveler=(_Traveler_Interface*)Allocate<typename ComponentType::traveler_type>();
					vehicle=(_Vehicle_Interface*)Allocate<typename _Vehicle_Interface::Component_Type>();
					router=(_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();
//					plan = (_Plan_Interface*)Allocate<typename _Plan_Interface::Component_Type>();
					movement_plan = (_Movement_Plan_Interface*)Allocate<typename _Movement_Plan_Interface::Component_Type>();

					vehicle->template uuid<int>(++traveler_id_counter);
					vehicle->template internal_id<int>(traveler_id_counter);
					vehicle->template movement_plan<_Movement_Plan_Interface*>(movement_plan);
					vehicle->template traveler<_Traveler_Interface*>(traveler);
					vehicle->template router<_Routing_Interface*>(router);
					vehicle->template initialize<NT>();


					traveler->template uuid<int>(traveler_id_counter);
					traveler->template internal_id<int>(traveler_id_counter);
					traveler->template router<_Routing_Interface*>(router);
					traveler->template vehicle<_Vehicle_Interface*>(vehicle);

					router->template traveler<_Traveler_Interface*>(traveler);
					router->template network<_Network_Interface*>(network);

					movement_plan->template origin<_Link_Interface*>(origin_link);
					movement_plan->template destination<_Link_Interface*>(destination_link);

					_Activity_Location_Interface* origin_location = origin_link->template activity_locations<_Activity_Locations_Container_Interface&>()[0];
					_Activity_Location_Interface* destination_location = destination_link->template activity_locations<_Activity_Locations_Container_Interface&>()[0];
					movement_plan->template origin<_Activity_Location_Interface*>(origin_location);
					movement_plan->template destination<_Activity_Location_Interface*>(destination_location);

					_Zone_Interface* origin_zone = origin_location->template zone<_Zone_Interface*>();
					_Zone_Interface* destination_zone = destination_location->template zone<_Zone_Interface*>();
					//movement_plan->template origin<_Zone_Interface*>(origin_zone);
					//movement_plan->template destination<_Zone_Interface*>(destination_zone);

					movement_plan->template departed_time<Time_Seconds>(departed_time);
					movement_plan->template initialize_trajectory<NULLTYPE>();

					router->template movement_plan<_Movement_Plan_Interface*>(movement_plan);

					traveler->template Schedule_New_Departure<NULLTYPE>(departed_time);

					vehicles_container<_Vehicles_Container_Interface&>().push_back(vehicle);

					if(traveler_id_counter%10000==0)
					{
						cout << "\t" << traveler_id_counter << endl;
					}
				}
			}

			template<typename TargetType> void read_demand_data(requires(TargetType,!check_2(TargetType,typename Network_Components::Types::ODB_Network,is_same) && !check_2(TargetType,typename Network_Components::Types::File_Network,is_same)))
			{
				assert_check_2(TargetType,typename Network_Components::Types::ODB_Network,is_same,"TargetType is ill-defined");
				assert_check_2(TargetType,typename Network_Components::Types::ODB_Network,is_same,"TargetType should indicate ODB_Network if you want to read it in with ODB");
				assert_check_2(TargetType,typename Network_Components::Types::File_Network,is_same,"TargetType should indicate Static_Network if you want to read in the hard coded network");
			}

			// template<typename TargetType> void read_demand_data(network_models::network_information::scenario_data_information::ScenarioData& scenario_data, network_models::network_information::network_data_information::NetworkData& network_data, network_models::network_information::demand_data_information::DemandData& demand_data)
			// {
				// typedef  Scenario_Components::Prototypes::Scenario< typename get_type_of(scenario_reference)> _Scenario_Interface;
				// typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				
				// typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename _Network_Interface::get_type_of(activity_locations_container)::value_type>::type>  _Activity_Location_Interface;
				// typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container), _Activity_Location_Interface*> _Activity_Locations_Container_Interface;

				// typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Activity_Location_Interface::get_type_of(origin_links)::value_type>::type>  _Link_Interface;
				// typedef  Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links), _Link_Interface*> _Links_Container_Interface;

				// typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename _Network_Interface::get_type_of(zones_container)::value_type>::type>  _Zone_Interface;
				// typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(zones_container), _Zone_Interface*> _Zones_Container_Interface;


				// typedef Traveler_Components::Prototypes::Traveler<typename ComponentType::traveler_type> _Traveler_Interface;

				// typedef  Routing_Components::Prototypes::Routing< typename _Traveler_Interface::get_type_of(router)> _Routing_Interface;

				// typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename get_type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
				// typedef  Random_Access_Sequence< typename get_type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;

				// typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename _Vehicle_Interface::get_type_of(movement_plan)> _Movement_Plan_Interface;
				
				// this->template first_vehicle_departure_time<int>(demand_data.first_vehicle_departure_time);
				// this->template last_vehicle_departure_time<int>(demand_data.last_vehicle_departure_time);
				// int i;
				// for (i = 0; i < demand_data.demand_vehicle_size; i++)
				// {
					// network_models::network_information::demand_data_information::VehicleData& raw_vehicle = demand_data.vehilce_data_array[i];
					// _Vehicle_Interface* vehicle = (_Vehicle_Interface*)Allocate<typename _Vehicle_Interface::Component_Type>(); 
					// _Traveler_Interface* traveler=(_Traveler_Interface*)Allocate<typename ComponentType::traveler_type>();
					// _Routing_Interface* router=(_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();
					// _Movement_Plan_Interface* movement_plan = (_Movement_Plan_Interface*)Allocate<typename _Movement_Plan_Interface::Component_Type>();

					// vehicle->template uuid<int>(raw_vehicle.get_vehicle_id());
					// vehicle->template internal_id<int>(i);
					// vehicle->template movement_plan<_Movement_Plan_Interface*>(movement_plan);
					// vehicle->template traveler<_Traveler_Interface*>(traveler);
					// vehicle->template router<_Routing_Interface*>(router);

					// vehicle->template initialize<NT>();

					// traveler->template uuid<int>(raw_vehicle.get_vehicle_id());
					// traveler->template internal_id<int>(i);
					// traveler->template router<_Routing_Interface*>(router);
					// traveler->template vehicle<_Vehicle_Interface*>(vehicle);

					// router->template traveler<_Traveler_Interface*>(traveler);
					// router->template network<_Network_Interface*>(network_reference<_Network_Interface*>());

					// int f = raw_vehicle.get_origin_link_index();
					// int size = (int)network_reference<_Network_Interface*>()->template links_container<_Links_Container_Interface&>().size();
								
					// movement_plan->template origin<_Link_Interface*>(network_reference<_Network_Interface*>()->template links_container<_Links_Container_Interface&>().at(raw_vehicle.get_origin_link_index()));
					// movement_plan->template destination<_Link_Interface*>(network_reference<_Network_Interface*>()->template links_container<_Links_Container_Interface&>().at(raw_vehicle.get_destination_link_index()));

					// int departed_time = raw_vehicle.get_departure_time();

					// movement_plan->template departed_time<Time_Seconds>(departed_time);
					// movement_plan->template initialize_trajectory<NULLTYPE>();

					// router->template movement_plan<_Movement_Plan_Interface*>(movement_plan);
					
					// traveler->template Schedule_New_Departure<NULLTYPE>(departed_time);
					
					// vehicles_container<_Vehicles_Container_Interface&>().push_back(vehicle);

				// }
				
			// }

			// template<typename TargetType> void read_demand_data(network_models::network_information::scenario_data_information::ScenarioData& scenario_data, network_models::network_information::network_data_information::NetworkData& network_data, network_models::network_information::demand_data_information::DemandData& demand_data, requires(TargetType,!check_2(TargetType,typename Network_Components::Types::File_Network,is_same)))
			// {
				// assert_check_2(TargetType,typename Network_Components::Types::File_Network,is_same,"TargetType is ill-defined");
				// assert_check_2(TargetType,typename Network_Components::Types::File_Network,is_same,"TargetType should indicate File_Network if you want to read it in from file");
			// }

			// template<typename TargetType> void write_demand_data(network_models::network_information::demand_data_information::DemandData& demand_data)
			// {
				// typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename get_type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
				// typedef  Random_Access_Sequence< typename get_type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;

				// typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				// typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename _Network_Interface::get_type_of(activity_locations_container)::value_type>::type>  _Activity_Location_Interface;
				// typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container), _Activity_Location_Interface*> _Activity_Locations_Container_Interface;

				// typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Activity_Location_Interface::get_type_of(origin_links)::value_type>::type>  _Link_Interface;
				// typedef  Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links), _Link_Interface*> _Links_Container_Interface;

				// typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename _Network_Interface::get_type_of(zones_container)::value_type>::type>  _Zone_Interface;
				// typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(zones_container), _Zone_Interface*> _Zones_Container_Interface;

				// typedef Person_Components::Prototypes::Person<typename ComponentType::traveler_type> _Traveler_Interface;
// //				define_component_interface(_Plan_Interface, typename _Traveler_Interface::get_type_of(plan), Plan_Components::Prototypes::Plan_Prototype, ComponentType);
				// typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename get_type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
				// typedef  Random_Access_Sequence< typename get_type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;

				// typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename _Vehicle_Interface::get_type_of(movement_plan)> _Movement_Plan_Interface;

// //				define_component_interface(_Movement_Plan_Interface, typename _Plan_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
                // typedef  Scenario_Components::Prototypes::Scenario< typename get_type_of(scenario_reference)> _Scenario_Interface;

				// demand_data.first_vehicle_departure_time = this->template first_vehicle_departure_time<int>();
				// demand_data.last_vehicle_departure_time = this->template last_vehicle_departure_time<int>();
				// demand_data.time_dependent_vehicle_index_array.resize(scenario_reference<_Scenario_Interface*>()->template planning_horizon<int>());
				// int counter = -1;

				// for (int i = 0; i < (int)vehicles_container<_Vehicles_Container_Interface&>().size(); i++) 
				// {
					// if (++counter % 10000 == 0)
					// {
						// cout << counter << " vehicles converted " << endl;
					// }
					// _Vehicle_Interface* vehicle = vehicles_container<_Vehicles_Container_Interface&>()[i];
					// _Movement_Plan_Interface* movement_plan = vehicle->template movement_plan<_Movement_Plan_Interface*>();
					// network_models::network_information::demand_data_information::VehicleData vehicle_data;
					
					// vehicle_data.set_vehicle_id(vehicle->template uuid<int>());
					// vehicle_data.set_vehicle_index(vehicle->template internal_id<int>());

					// vehicle_data.set_origin_link_index(movement_plan->template origin<_Link_Interface*>()->template internal_id<int>());
					// vehicle_data.set_destination_link_index(movement_plan->template destination<_Link_Interface*>()->template internal_id<int>());
					// vehicle_data.set_origin_activity_location_index(movement_plan->template origin<_Activity_Location_Interface*>()->template internal_id<int>());
					// vehicle_data.set_destination_activity_location_index(movement_plan->template destination<_Activity_Location_Interface*>()->template internal_id<int>());
					// vehicle_data.set_origin_zone_index(movement_plan->template origin<_Zone_Interface*>()->template internal_id<int>());
					// vehicle_data.set_destination_zone_index(movement_plan->template destination<_Zone_Interface*>()->template internal_id<int>());

					// vehicle_data.set_departure_time(movement_plan->template departed_time<Time_Seconds>());
					// vehicle_data.set_departure_simulation_interval_index(movement_plan->template departed_time<Time_Seconds>() / ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>());
					// vehicle_data.set_departure_assignment_interval_index(movement_plan->template departed_time<Time_Seconds>() / ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>());

					// demand_data.vehilce_data_array.push_back(vehicle_data);
					// demand_data.time_dependent_vehicle_index_array[movement_plan->template departed_time<Time_Seconds>()].push_back(vehicle_data.get_vehicle_index());

				// }
				// demand_data.demand_vehicle_size = (int)demand_data.vehilce_data_array.size();
			// }

		};
	}
}

using namespace Demand_Components::Prototypes;
