#pragma once

#include "Traveler_Simulator_Includes.h"
#include "Activity_Simulator_Includes.h"
//#include "Link_Prototype.h"
//#include "../File_IO/network_demand_data.h"

/*------------------------------------------------*/


namespace Demand_Components
{
	
	namespace Prototypes
	{
		prototype struct Demand ADD_DEBUG_INFO
		{
			tag_as_prototype;

			typedef typename ComponentType::Master_Type MasterType;

			accessor(scenario_reference, NONE, NONE);
			accessor(network_reference, NONE, NONE);
			accessor(vehicles_container, NONE, NONE);
			accessor(vehicle_types_container, NONE, NONE);
			accessor(first_vehicle_departure_time, NONE, NONE);
			accessor(last_vehicle_departure_time, NONE, NONE);

			template<typename TargetType> void Initialize();
			template<typename TargetType> void Add_Trip_Record(TargetType movement_plan, bool write_trajectory);

			template<typename TargetType> void read_vehicle_type_data()
			{
				try
				{
					using namespace odb;
					using namespace polaris::io;
					typedef  Scenario_Components::Prototypes::Scenario< typename get_type_of(scenario_reference)> _Scenario_Interface;
					typedef  Vehicle_Components::Prototypes::Vehicle_Characteristics<typename remove_pointer< typename get_type_of(vehicle_types_container)::value_type>::type>  _Vehicle_Type_Interface;
					typedef  Random_Access_Sequence< typename get_type_of(vehicle_types_container), _Vehicle_Type_Interface*> _Vehicle_Types_Container_Interface;

					string name(scenario_reference<_Scenario_Interface*>()->template database_name<string&>());
					unique_ptr<database> db (open_sqlite_demand_database (name));

					transaction t(db->begin());
					result<Vehicle_Type> type_result=db->template query<Vehicle_Type>(query<Vehicle_Type>::true_expr);
				
					cout << "Reading Vehicle Types" << endl;

					_Vehicle_Types_Container_Interface* veh_types = this->vehicle_types_container<_Vehicle_Types_Container_Interface*>();

					for(result<Vehicle_Type>::iterator db_itr = type_result.begin (); db_itr != type_result.end (); ++db_itr)
					{
						_Vehicle_Type_Interface* veh_type = (_Vehicle_Type_Interface*)Allocate<typename MasterType::vehicle_characteristics_type>();
						veh_type->initialize(db_itr.load());
						veh_types->push_back(veh_type);
					}
				}
				catch (const odb::exception& e) {THROW_EXCEPTION(e.what());}
			}

			template<typename TargetType> void read_demand_data(typename TargetType::ParamType& network_mapping,
				requires(TargetType,check_2(typename TargetType::NetIOType,Network_Components::Types::ODB_Network,is_same)))
			{
				int counter = -1;
				int trip_id = 0;
				int path_id = -1;

				try
				{
				using namespace odb;
				using namespace polaris::io;
				typedef  Scenario_Components::Prototypes::Scenario< typename get_type_of(scenario_reference)> _Scenario_Interface;

				string name(scenario_reference<_Scenario_Interface*>()->template database_name<string&>());
				unique_ptr<database> db (open_sqlite_demand_database (name));

				typedef  Scenario_Components::Prototypes::Scenario< typename get_type_of(scenario_reference)> _Scenario_Interface;
				typedef  Vehicle_Components::Prototypes::Vehicle_Characteristics<typename remove_pointer< typename get_type_of(vehicle_types_container)::value_type>::type>  _Vehicle_Type_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(vehicle_types_container), _Vehicle_Type_Interface*> _Vehicle_Types_Container_Interface;

				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;

				typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename _Network_Interface::get_type_of(activity_locations_container)::value_type>::type>  _Activity_Location_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container), _Activity_Location_Interface*> _Activity_Locations_Container_Interface;

				typedef  Link_Components::Prototypes::Link<typename MasterType::link_type>  _Link_Interface;
				typedef  Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links), _Link_Interface*> _Links_Container_Interface;

				typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename _Network_Interface::get_type_of(zones_container)::data_type>::type>  _Zone_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(zones_container), _Zone_Interface*> _Zones_Container_Interface;
				
				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Link_Interface::get_type_of(outbound_turn_movements)::value_type>::type>  _Movement_Interface;
				typedef  Random_Access_Sequence< typename _Link_Interface::get_type_of(outbound_turn_movements), _Movement_Interface*> _Movements_Container_Interface;

				typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename get_type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;

				typedef  Vehicle_Components::Prototypes::Vehicle_Characteristics<typename remove_pointer< typename get_type_of(vehicle_types_container)::value_type>::type>  _Vehicle_Type_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(vehicle_types_container), _Vehicle_Type_Interface*> _Vehicle_Types_Container_Interface;
				
				//TODO: Omer
				//typedef Traveler_Components::Prototypes::Traveler<typename ComponentType::traveler_type> _Traveler_Interface;
				typedef Person_Components::Prototypes::Person<typename ComponentType::person_type> _Person_Interface;
				typedef Person_Components::Prototypes::Person_Mover< typename _Person_Interface::get_type_of(Moving_Faculty)> _Movement_Faculty_Interface;

				//TODO: Omer
				//typedef  Routing_Components::Prototypes::Routing< typename _Traveler_Interface::get_type_of(router) > _Routing_Interface;
				typedef  Routing_Components::Prototypes::Routing< typename _Person_Interface::get_type_of(router) > _Routing_Interface;

				typedef  Routing_Components::Prototypes::Routable_Network<typename MasterType::routable_network_type> _Routable_Network_Interface;
				typedef  Prototype_Random_Access_Sequence <typename _Network_Interface::get_type_of(routable_networks), Routing_Components::Prototypes::Routable_Network> _Routable_Networks_Container_Interface;
//				define_component_interface(_Plan_Interface, _Traveler_Interface::get_type_of(plan), Plan_Components::Prototypes::Plan_Prototype, ComponentType);
//				define_component_interface(_Movement_Plan_Interface, _Plan_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				//typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename get_type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
				//typedef  Random_Access_Sequence< typename get_type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;

				typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename _Vehicle_Interface::get_type_of(movement_plan)> _Movement_Plan_Interface;

				_Network_Interface* network=network_reference<_Network_Interface*>();
				_Routable_Networks_Container_Interface* routable_nets = network->template routable_networks <typename _Network_Interface::get_type_of(routable_networks)>();
				_Routable_Network_Interface* routable_network = routable_nets->at(0);

				_Activity_Locations_Container_Interface& activity_locations = network->template activity_locations_container<_Activity_Locations_Container_Interface&>();

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();


				transaction t(db->begin());

				
				
				//TODO: Omer
				//_Traveler_Interface* traveler;
				_Person_Interface* person;
				_Movement_Faculty_Interface* movement_faculty;

				_Vehicle_Interface* vehicle;

				_Routing_Interface* router;
//				_Plan_Interface* plan;
				_Movement_Plan_Interface* movement_plan;
				
				//TODO: Omer
				//int traveler_id_counter=-1;
				int person_id_counter = -1;
				
				
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

				int departed_time;
				int skipped_counter=0;

				this->template first_vehicle_departure_time<int>(24*60*60.0);
				this->template last_vehicle_departure_time<int>(0.0);


				int simulation_start_time = scenario->template simulation_start_time<int>();
				int simulation_end_time = scenario->template simulation_end_time<int>();

				float demand_percentage= scenario->template demand_reduction_factor<float>();

				cout << "Demand Percentage: " << demand_percentage;

				//// read the paths/path links
				//typedef unordered_map<int, shared_ptr<Path>> trajectory_container_type;
				//typedef trajectory_container_type::iterator trajectory_iterator_type;
				//trajectory_container_type trajectories;
				//result<Path> path_result = db->template query<Path>(query<Path>::true_expr);
				//for (result<Path>::iterator db_itr = path_result.begin(); db_itr != path_result.end(); ++db_itr)
				//{
				//	shared_ptr<Path> p (new Path(*db_itr));
				//	trajectories.insert(pair<int, shared_ptr<Path>>(db_itr->getId(), p));
				//}


				// read the trips
				result<Trip> trip_result = db->template query<Trip>(query<Trip>::true_expr);
				for (result<Trip>::iterator db_itr = trip_result.begin(); db_itr != trip_result.end(); ++db_itr)
				{
					// perform demand reduction
					if (GLOBALS::Uniform_RNG.template Next_Rand<float>() > demand_percentage) continue;

					trip_id = db_itr->getPrimaryKey();

					//=======================================
					//TODO: re-evaluate later, for now we are throwing out all records in the input database that are not SOV, as it is assumed that all of these will be sov trips for simulation purposes....
					int mode_id = db_itr->getMode();
					if (!scenario->multimodal_routing<bool>() && mode_id != Vehicle_Components::Types::Vehicle_Type_Keys::SOV) continue;
					//=======================================


					if (++counter % 100000 == 0)
					{
						cout << counter << " trips processed" << endl;
					}

					departed_time = db_itr->getStart();

					if (departed_time < simulation_start_time || departed_time >= simulation_end_time) {
						continue;
					}

					int org_key = db_itr->getOrigin();

					if (!activity_id_to_ptr.count(org_key))
					{
						if (++skipped_counter % 100000 == 0)
						{
							cout << skipped_counter << " trips skipped" << endl;
						}
						continue;
					}

					int dst_key = db_itr->getDestination();

					if (!activity_id_to_ptr.count(dst_key))
					{
						if (++skipped_counter % 100000 == 0)
						{
							cout << skipped_counter << " trips skipped" << endl;
						}
						continue;
					}

					_Activity_Location_Interface* origin_activity_location = activity_id_to_ptr[org_key];
					_Activity_Location_Interface* destination_activity_location = activity_id_to_ptr[dst_key];
					_Link_Interface* origin_link = origin_activity_location->template origin_links<_Links_Container_Interface&>()[0];
					_Link_Interface* destination_link = destination_activity_location->template destination_links<_Links_Container_Interface&>()[0];
					if (origin_link->template dbid<int>() == destination_link->template dbid<int>() || (origin_link->template outbound_turn_movements<_Movements_Container_Interface&>().size() == 0 || destination_link->template inbound_turn_movements<_Movements_Container_Interface&>().size() == 0))
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

					//TODO: Omer
					//traveler=(_Traveler_Interface*)Allocate<typename ComponentType::traveler_type>();
					person = (_Person_Interface*)Allocate<typename ComponentType::person_type>();
					movement_faculty = (_Movement_Faculty_Interface*)Allocate<typename _Person_Interface::get_type_of(Moving_Faculty)>();
					movement_faculty->Parent_Person(person);

					vehicle = (_Vehicle_Interface*)Allocate<typename _Vehicle_Interface::Component_Type>();

					router = (_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();

					movement_plan = (_Movement_Plan_Interface*)Allocate<typename _Movement_Plan_Interface::Component_Type>();
					movement_plan->template network<_Network_Interface*>(network);
					movement_plan->experienced_gap(db_itr->getGap());

					//TODO: Omer
					//vehicle->template uuid<int>(++traveler_id_counter);
					vehicle->template uuid<int>(++person_id_counter);
					//vehicle->template internal_id<int>(traveler_id_counter);
					vehicle->template internal_id<int>(person_id_counter);
					vehicle->template movement_plan<_Movement_Plan_Interface*>(movement_plan);
					//vehicle->template traveler<_Traveler_Interface*>(traveler);
					vehicle->template traveler<_Person_Interface*>(person);
					vehicle->template router<_Routing_Interface*>(router);
					vehicle->template is_integrated<bool>(db_itr->getType());
					vehicle->template vehicle_ptr< shared_ptr<polaris::io::Vehicle> >(db_itr->getVehicle());
					//vehicle->initialize(*(this->vehicle_types_container<_Vehicle_Types_Container_Interface*>()->begin()), traveler_id_counter);
					vehicle->initialize(*(this->vehicle_types_container<_Vehicle_Types_Container_Interface*>()->begin()), person_id_counter);
					vehicle->is_integrated(false);

					//TODO: Omer
					//traveler->template uuid<int>(traveler_id_counter);
					//traveler->template internal_id<int>(traveler_id_counter);
					//traveler->template router<_Routing_Interface*>(router);
					//traveler->template vehicle<_Vehicle_Interface*>(vehicle);
					person->template uuid<int>(person_id_counter);
					person->template internal_id<int>(person_id_counter);
					person->template router<_Routing_Interface*>(router);
					person->template vehicle<_Vehicle_Interface*>(vehicle);
					person->Moving_Faculty(movement_faculty);
					//person->template network_reference<_Network_Interface*>(network);

					//TODO:ROUTING_OPERATION
					//router->template traveler<_Traveler_Interface*>(traveler);
					router->template network<_Network_Interface*>(network);

					movement_plan->traveler_id(trip_id);
					movement_plan->template origin<_Link_Interface*>(origin_link);
					movement_plan->template destination<_Link_Interface*>(destination_link);

					_Activity_Location_Interface* origin_location = origin_activity_location;
					_Activity_Location_Interface* destination_location = destination_activity_location;
					movement_plan->template origin<_Activity_Location_Interface*>(origin_location);
					movement_plan->template destination<_Activity_Location_Interface*>(destination_location);

					//Vehicle_Components::Types::Vehicle_Type_Keys myMode = mode_id;
					movement_plan->template mode<int>(mode_id);

					_Zone_Interface* origin_zone = origin_location->template zone<_Zone_Interface*>();
					_Zone_Interface* destination_zone = destination_location->template zone<_Zone_Interface*>();
					//TODO: commented out to quickly address compatibility
					//movement_plan->template origin<_Zone_Interface*>(origin_zone);
					//movement_plan->template destination<_Zone_Interface*>(destination_zone);

					movement_plan->template departed_time<Time_Seconds>(departed_time);
					movement_plan->template initialize_trajectory<NULLTYPE>();
					//movement_plan->template initialize_multimodal_trajectory<NULLTYPE>();

					//=======================================================================
					// Fill the movement plan from the PATH table if it was logged,
					shared_ptr<Path> path = db_itr->getPath();

					/*path_id = db_itr->getPath_id();
					trajectory_iterator_type t_itr = trajectories.find(path_id);*/
					if (path && scenario->read_trajectories<bool>())
					{
						//shared_ptr<Path> path = t_itr->second;

						// what was the gap?
						float gap = 1.0f;
						if (path->getRouted_Time() > 0.0f) gap = std::max((float)path->getTravel_Time() / (float)path->getRouted_Time() - 1.0f, 0.0f);

						// use the gap to determine if we will follow the same route (more likely for small gaps...)
						if (GLOBALS::Uniform_RNG.Next_Rand<float>() < gap)
						{
							// schedule routing
							router->template Attach_New_Movement_Plan<typename _Movement_Plan_Interface::Component_Type>(movement_plan);
							movement_faculty->template Schedule_Movement<Simulation_Timestep_Increment, _Movement_Plan_Interface*>(movement_plan->template departed_time<Simulation_Timestep_Increment>(), movement_plan);
							vehicles_container<_Vehicles_Container_Interface&>().push_back(vehicle);
							continue;
						}


						// containers to use in set_trajectory
						std::deque<global_edge_id> path_container;
						std::deque<float> cost_container;

						// update the movement plan characteristics
						movement_plan->valid_trajectory(true);
						movement_plan->routed_travel_time(path->getTravel_Time());
						movement_plan->estimated_time_of_arrival(path->getDeparture_Time() + path->getTravel_Time());
						movement_plan->estimated_travel_time_when_departed(path->getTravel_Time());

						// which graph will we route on???
						int graph_id = 0;
						if (((_Scenario_Interface*)_global_scenario)->template multimodal_routing<bool>()) graph_id = routable_network->multimodal_network_graph_id<int>();
						else if (((_Scenario_Interface*)_global_scenario)->template time_dependent_routing<bool>()) graph_id = routable_network->time_dependent_network_graph_id<int>();
						else graph_id = routable_network->static_network_graph_id<int>();

						// scan all the links
						const std::vector<link_travel >& trajectory_links = path->getLinks();
						for (std::vector<link_travel>::const_iterator c_itr = trajectory_links.begin(); c_itr != trajectory_links.end(); ++c_itr)
						{
							const link_travel* link = &(*c_itr);
							global_edge_id edge;
							edge.edge_id = link->getLink() * 2 + link->getDir();
							edge.graph_id = graph_id;
							path_container.push_back(edge);
							cost_container.push_back(link->getTravel_Time());
						}


						bool ofound, dfound;
						for (_Links_Container_Interface::iterator o_itr = origin_activity_location->template origin_links<_Links_Container_Interface&>().begin(); o_itr != origin_activity_location->template origin_links<_Links_Container_Interface&>().end(); ++o_itr)
						{
							if ((*o_itr)->uuid<int>() == path_container.front().edge_id)
							{
								movement_plan->origin(*o_itr);
								ofound = true;
								break;
							}
						}
						for (_Links_Container_Interface::iterator d_itr = destination_activity_location->template destination_links<_Links_Container_Interface&>().begin(); d_itr != destination_activity_location->template destination_links<_Links_Container_Interface&>().end(); ++d_itr)
						{
							if ((*d_itr)->uuid<int>() == path_container.back().edge_id)
							{
								movement_plan->destination(*d_itr);
								dfound = true;
								break;
							}
						}
						if (!ofound || !dfound) { THROW_EXCEPTION("ERROR: origin or destination link from trip table could not be matched to path table."); }

						// schedule moving
						movement_plan->set_trajectory(path_container, cost_container);
						movement_faculty->template Schedule_Movement<Simulation_Timestep_Increment, _Movement_Plan_Interface*>(movement_plan->template departed_time<Simulation_Timestep_Increment>(), movement_plan, false);
						vehicles_container<_Vehicles_Container_Interface&>().push_back(vehicle);
					}


					//===============================================================================
					// Otherwise, start the router process
					else
					{
						router->template Attach_New_Movement_Plan<typename _Movement_Plan_Interface::Component_Type>(movement_plan);
						movement_faculty->template Schedule_Movement<Simulation_Timestep_Increment, _Movement_Plan_Interface*>(movement_plan->template departed_time<Simulation_Timestep_Increment>(), movement_plan);
						vehicles_container<_Vehicles_Container_Interface&>().push_back(vehicle);
					}
					

				
					//if(traveler_id_counter%10000==0)
					//{
					//	cout << "\t" << traveler_id_counter << endl;
					//}
				}
				t.commit();

				// write vehicles to output demand database
				shared_ptr<odb::database> out_db_ptr = scenario->template demand_db_ptr<shared_ptr<odb::database>>();
				transaction t2(out_db_ptr->begin());

				for (typename _Vehicles_Container_Interface::iterator v_itr = vehicles_container<_Vehicles_Container_Interface&>().begin(); v_itr != vehicles_container<_Vehicles_Container_Interface&>().end(); ++v_itr)
				{
					shared_ptr<MasterType::vehicle_db_rec_type> veh_rec((*v_itr)->vehicle_ptr<shared_ptr<MasterType::vehicle_db_rec_type>>());
					out_db_ptr->persist(veh_rec);
					counter++;
				}
				t2.commit();
				}
				catch (exception& e)
				{
					std::cout << "\nException message: " << e.what() << "\n";
					THROW_EXCEPTION("Error: seems to be an ODB exception here, reading trip number "<<counter<<", trip id="<<trip_id);
				}
			}

			template<typename TargetType> void read_demand_data(requires(TargetType,!check_2(TargetType,typename Network_Components::Types::ODB_Network,is_same) && !check_2(TargetType,typename Network_Components::Types::File_Network,is_same)))
			{
				assert_check_2(TargetType,typename Network_Components::Types::ODB_Network,is_same,"TargetType is ill-defined");
				assert_check_2(TargetType,typename Network_Components::Types::ODB_Network,is_same,"TargetType should indicate ODB_Network if you want to read it in with ODB");
				assert_check_2(TargetType,typename Network_Components::Types::File_Network,is_same,"TargetType should indicate Static_Network if you want to read in the hard coded network");
			}	
		};

		template<typename ComponentType>
		template<typename TargetType> void Demand<ComponentType>::Initialize()
		{
			this_component()->Initialize<TargetType>();
		}
		template<typename ComponentType>
		template<typename TargetType> void Demand<ComponentType>::Add_Trip_Record(TargetType movement_plan, bool write_trajectory)
		{
			this_component()->Add_Trip_Record<TargetType>(movement_plan, write_trajectory);
		}
	}
}

using namespace Demand_Components::Prototypes;
