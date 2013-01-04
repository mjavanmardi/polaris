#pragma once

#include "User_Space_Includes.h"
#include "../File_IO/network_demand_data.h"

/*
 * Needed for Linux compatability. Do not hurt Windows compilation.
 */
namespace Scenario_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Scenario_Prototype;
	}
};
namespace Network_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Network_Prototype;
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
namespace Routing_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Routing_Prototype;
	}
};
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

	//==================================================================================================================
	/// Concept checks related to the Demand generation interface
	//------------------------------------------------------------------------------------------------------------------
	// commented by Bo.
	//namespace Concepts
	//{
	//	concept Is_Transims
	//	{
	//		begin_requirements_list(none);
	//		requires_typename_state(none,TransimsType,true_type,"Type is not a Transims type");
	//		end_requirements_list(TransimsType);
	//	};
	//	concept Is_Trip_File
	//	{
	//		begin_requirements_list(none);
	//		requires_typename_state(none,TripFile,true_type,"Type is not in a TripFile");
	//		requires_typed_member(TripFile, filename,char*,"Type does not have a 'filename' member of char* type");
	//		end_requirements_list(filename);
	//	};
	//	concept Is_Control_File
	//	{
	//		begin_requirements_list(none);
	//		requires_typename_state(none,ControlFile,true_type,"Type is not in a ControlFile");
	//		requires_typed_member(ControlFile, filename, char*,"Type does not have a 'filename' member of char* type");
	//		end_requirements_list(filename);
	//	};
	//	concept Is_CSV_Delimited
	//	{
	//		begin_requirements_list(none);
	//		requires_typename_state(none,CSV_Delimited,true_type,"Type is not CSV_Delimited");
	//		end_requirements_list(CSV_Delimited);
	//	};
	//	concept Is_Tab_Delimited
	//	{
	//		begin_requirements_list(none);
	//		requires_typename_state(none,Tab_Delimited,true_type,"Type is not Tab_Delimited");
	//		end_requirements_list(Tab_Delimited);
	//	};
	//	concept Is_Transims_Control_File
	//	{
	//		begin_requirements_list(none);
	//		requires_concept(none,Is_Transims);
	//		requires_concept(Is_Transims,Is_Control_File);
	//		end_requirements_list(Is_Control_File);
	//	};
	//	concept Is_Transims_Trip_File
	//	{
	//		begin_requirements_list(none);
	//		requires_concept(none,Is_Transims);
	//		requires_concept(Is_Transims,Is_Trip_File);
	//		end_requirements_list(Is_Trip_File);
	//	};

	//}
	
	namespace Prototypes
	{
		prototype struct Demand_Prototype
		{
			tag_as_prototype;

			feature_accessor(scenario_reference, none, none);
			feature_accessor(network_reference, none, none);
			feature_accessor(vehicles_container, none, none);
			feature_accessor(first_vehicle_departure_time, none, none);
			feature_accessor(last_vehicle_departure_time, none, none);
			feature_prototype void read_demand_data(typename TargetType::ParamType& network_mapping,
				requires(check_2(TargetType::NetIOType,Network_Components::Types::ODB_Network,is_same)))
			{
				using namespace odb;
				using namespace pio;

				//string name("C:\\Users\\hope\\Desktop\\POLARIS_v03\\Polaris_IO\\chicago_with_demand.sqlite");
				//string name("C:\\Users\\bxu\\Desktop\\Good_copy_new_network_io-database\\Polaris_IO\\TestNet50.sqlite");
				//string name("C:\\Users\\bxu\\Desktop\\Good_copy_new_network_io-database - chicago\\Polaris_IO\\chicago_with_demand.sqlite");
				string name("chicago_with_demand_no_ref.sqlite");
				auto_ptr<database> db (open_sqlite_database (name));

				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_component_interface(_Network_Interface, get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);

				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, _Network_Interface::get_type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Link_Interface::get_type_of(outbound_turn_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, get_type_of(vehicles_container), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Routing_Interface, ComponentType::traveler_type::router_type, Routing_Components::Prototypes::Routing_Prototype, ComponentType);
				typedef Traveler_Components::Prototypes::Traveler_Prototype<typename ComponentType::traveler_type, ComponentType> _Traveler_Interface;

				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, get_type_of(network_reference)::type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);



				_Network_Interface* network=network_reference<_Network_Interface*>();
				_Activity_Locations_Container_Interface& activity_locations = network->template activity_locations_container<_Activity_Locations_Container_Interface&>();

				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();

				transaction t(db->begin());

				//result<Location> location_result=db->query<Location>(query<Location>::true_expr);
				result<TripNoRef> trip_result=db->query<TripNoRef>(query<TripNoRef>::true_expr);
				
				
				_Traveler_Interface* traveler;
				_Vehicle_Interface* vehicle;
				_Routing_Interface* router;
				
				int traveler_id_counter=-1;
				
				

				dense_hash_map<int,_Activity_Location_Interface*> activity_id_to_ptr;
				activity_id_to_ptr.set_empty_key(-1);
				activity_id_to_ptr.set_deleted_key(-2);

				typename _Activity_Locations_Container_Interface::iterator activity_locations_itr;

				cout << "Building Temporary Activity Map" << endl;

				for(activity_locations_itr=activity_locations.begin();activity_locations_itr!=activity_locations.end();activity_locations_itr++)
				{
					_Activity_Location_Interface& activity_location=iterator_to_interface(_Activity_Location_Interface,activity_locations_itr);

					activity_id_to_ptr[activity_location.uuid<int>()]=&activity_location;
				}

				cout << "Reading Travelers" << endl;


int counter = -1;
				int departed_time;
				int skipped_counter=0;

				this->first_vehicle_departure_time<int>(24*60*60.0);
				this->last_vehicle_departure_time<int>(0.0);


				int simulation_start_time = scenario->simulation_start_time<int>();
				int simulation_end_time = scenario->simulation_end_time<int>();

				for(result<TripNoRef>::iterator db_itr = trip_result.begin (); db_itr != trip_result.end (); ++db_itr)
				{
if (++counter % 10000 == 0)
{
	cout << counter << " trips processed" << endl;
}

					departed_time = db_itr->getStart();

					if (departed_time < simulation_start_time || departed_time >= simulation_end_time) {
						continue;
					}

					//shared_ptr<Location> org=db_itr->getOrigin();				
					//shared_ptr<Location> dst=db_itr->getDestination();
					//if(org.get() && dst.get())
					//{
					int org_key=db_itr->getOrigin();

					if(!activity_id_to_ptr.count(org_key))
					{
						if(++skipped_counter%10000==0)
						{
							cout << skipped_counter << " trips skipped" << endl;
						}
						continue;
					}

					int dst_key=db_itr->getDestination();

					if(!activity_id_to_ptr.count(dst_key))
					{
						if(++skipped_counter%10000==0)
						{
							cout << skipped_counter << " trips skipped" << endl;
						}
						continue;
					}




					//dense_hash_map<int,_Activity_Location_Interface*>::const_iterator itr;
					//itr = activity_id_to_ptr.find(org_key);
					//if (itr == activity_id_to_ptr.end())
					//{
						// not such activity location
						//continue;
					//}
					_Activity_Location_Interface* origin_activity_location = activity_id_to_ptr[org_key];
					//itr = activity_id_to_ptr.find(dst_key);
					//if (itr == activity_id_to_ptr.end())
					//{
						// not such activity location
						//continue;
					//}						
					_Activity_Location_Interface* destination_activity_location = activity_id_to_ptr[dst_key];
//cout << "k = " << db_itr->getPrimaryKey() << " a = " << db_itr->getAuto_id() << " m = " << db_itr->getMode() << " v = " << db_itr->getVehicle() << endl;
					//_Activity_Location_Interface* origin_activity_location=(_Activity_Location_Interface*)activity_id_to_ptr[org_key];
						
					//_Activity_Location_Interface* destination_activity_location=(_Activity_Location_Interface*)activity_id_to_ptr[dst_key];
//cout << "origin_acitivity_location internal id = " << origin_activity_location->internal_id<int>() << ", uuid = " << origin_activity_location->uuid<int>() << endl;
					_Link_Interface* origin_link = origin_activity_location->origin_links<_Links_Container_Interface&>()[0];
					_Link_Interface* destination_link = destination_activity_location->destination_links<_Links_Container_Interface&>()[0];
					//if (origin_link == nullptr || destination_link == nullptr)
					//{
					//	continue;
					//}
					if (origin_link->internal_id<int>() == destination_link->internal_id<int>()  || (origin_link->outbound_turn_movements<_Movements_Container_Interface&>().size() == 0 || destination_link->inbound_turn_movements<_Movements_Container_Interface&>().size() == 0))
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

					departed_time = departed_time - scenario->simulation_start_time<int>();
					
					traveler=(_Traveler_Interface*)Allocate<typename ComponentType::traveler_type>();
					vehicle=(_Vehicle_Interface*)Allocate<typename _Vehicle_Interface::Component_Type>();
					router=(_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();
					
					traveler->template uuid<int>(++traveler_id_counter);
					traveler->template internal_id<int>(traveler_id_counter);
						

					traveler->template router<_Routing_Interface*>(router);
					traveler->template vehicle<_Vehicle_Interface*>(vehicle);
					router->template traveler<_Traveler_Interface*>(traveler);
					router->template network<_Network_Interface*>(network);

					assert(activity_id_to_ptr.count(org_key));

						
					vehicle->template uuid<int>(traveler_id_counter);
					vehicle->template internal_id<int>(traveler_id_counter);
					vehicle->template origin_activity_location<_Activity_Location_Interface*>(origin_activity_location);
					vehicle->template origin_link<_Link_Interface*>(origin_link);

						
						
					assert(activity_id_to_ptr.count(dst_key));

						
					vehicle->template destination_activity_location<_Activity_Location_Interface*>(destination_activity_location);
					vehicle->template destination_link<_Link_Interface*>(destination_link);

						
					vehicle->template origin_zone<_Zone_Interface*>(network->template zones_container<_Zones_Container_Interface&>()[0]);
					vehicle->template destination_zone<_Zone_Interface*>(network->template zones_container<_Zones_Container_Interface&>()[0]);
					vehicle->template departed_time<int>(departed_time);
if (vehicle->internal_id<int>() == 23379)
{
cout << "trip primary key " << db_itr->getPrimaryKey() << endl;

cout << "hello" << endl;
}

					traveler->template Schedule_New_Departure<NULLTYPE>(departed_time);
					vehicle->template departed_simulation_interval_index<int>(departed_time);
					vehicle->template departed_assignment_interval_index<int>(departed_time);

					vehicles_container<_Vehicles_Container_Interface&>().push_back(vehicle);

					if(traveler_id_counter%10000==0)
					{
						cout << "\t" << traveler_id_counter << endl;
					}
				}

				//}

			}

#ifdef HARD_CODED_IO
			feature_prototype void read_demand_data()
			{
				float freeway_demand_rate = 1800.0;
				float ramp_demand_rate = 900.0;
				
				Uesr_Space::RngStream g1("Demand");
				g1.SetSeed(1);
				
				//int vehilce_index = -1;

				//sort demand by departed time
				//demand_data.time_dependent_vehicle_index_array.resize(scenario_data.num_simulation_intervals*scenario_data.simulation_interval_length);

				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_component_interface(_Network_Interface, get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				
				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);

				define_component_interface(_Vehicle_Interface, ComponentType::traveler_type::vehicle_type, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Routing_Interface, ComponentType::traveler_type::router_type, Routing_Components::Prototypes::Routing_Prototype, ComponentType);
				typedef Traveler_Components::Prototypes::Traveler_Prototype<typename ComponentType::traveler_type, ComponentType> _Traveler_Interface;

				_Network_Interface* network=network_reference<_Network_Interface*>();
				_Scenario_Interface* scenario=scenario_reference<_Scenario_Interface*>();


				int traveler_id_counter=-1;

				int num_intervals = scenario->template num_simulation_intervals<int>();

				for(int simulation_interval_index=0;simulation_interval_index<scenario->template num_simulation_intervals<int>();simulation_interval_index++)
				{// generate vehicles for each simulation interval

					if(simulation_interval_index<=300)
					{
						freeway_demand_rate += 5.0;
						ramp_demand_rate +=5.0;
					}
					else
					{
						freeway_demand_rate -= 5.0;
						ramp_demand_rate -=5.0;
					}

					int freeway_origin_activity_location_index = 0;
					_Activity_Location_Interface* freeway_origin_activity_location = (_Activity_Location_Interface*)(network->template activity_locations_container<_Activity_Locations_Container_Interface&>()[freeway_origin_activity_location_index]);
					_Link_Interface* freeway_origin_link = (_Link_Interface*)freeway_origin_activity_location->template origin_links<_Links_Container_Interface&>()[0];
					int num_lanes_freeway = freeway_origin_link->template num_lanes<int>();

					int ramp_origin_activity_location_index = 2;
					_Activity_Location_Interface* ramp_origin_activity_location = (_Activity_Location_Interface*)(network->template activity_locations_container<_Activity_Locations_Container_Interface&>()[ramp_origin_activity_location_index]);
					_Link_Interface* ramp_origin_link = (_Link_Interface*)ramp_origin_activity_location->template origin_links<_Links_Container_Interface&>()[0];
					int num_lanes_ramp = ramp_origin_link->template num_lanes<int>();
					
					float num_generated_vehicles_freeway = (float) ((1.0+num_lanes_freeway*scenario->template simulation_interval_length<int>() * freeway_demand_rate)/3600.0);
					float num_generated_vehicles_ramp = (float) ((1.0+num_lanes_ramp*scenario->template simulation_interval_length<int>() * ramp_demand_rate)/3600.0);

					int destination_activity_location_index = 1;
					_Activity_Location_Interface* destination_activity_location = (_Activity_Location_Interface*)(network->template activity_locations_container<_Activity_Locations_Container_Interface&>()[destination_activity_location_index]);
					_Link_Interface* destination_link = (_Link_Interface*)(destination_activity_location->template destination_links<_Links_Container_Interface&>()[0]);
					
					_Traveler_Interface* traveler;
					_Vehicle_Interface* vehicle;
					_Routing_Interface* router;

					float vehicle_rate_per_simulation_interval;
					int departed_time = -1;
					_Link_Interface* origin_link;
					_Activity_Location_Interface* origin_activity_location;
					int assignment_interval_index;
					assignment_interval_index = (int)((simulation_interval_index+1) * scenario->template simulation_interval_length<int>()) / scenario->template assignment_interval_length<int>();					
					
					vehicle_rate_per_simulation_interval = (float)(num_generated_vehicles_freeway / scenario->template simulation_interval_length<float>());
					origin_link = freeway_origin_link;
					origin_activity_location = (_Activity_Location_Interface*)network->template activity_locations_container<_Activity_Locations_Container_Interface&>()[freeway_origin_activity_location_index];


					for (int i=0;i<scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>();i++)
					{
						float cur_vehicle_rate_per_simulation_interval = vehicle_rate_per_simulation_interval;
						while(cur_vehicle_rate_per_simulation_interval>0)
						{
							if (cur_vehicle_rate_per_simulation_interval>=1.0)
							{
								
								traveler=(_Traveler_Interface*)Allocate<typename ComponentType::traveler_type>();
								vehicle=(_Vehicle_Interface*)Allocate<typename _Vehicle_Interface::Component_Type>();
								router=(_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();
								
								traveler->template uuid<int>(++traveler_id_counter);
								traveler->template router<_Routing_Interface*>(router);
								traveler->template vehicle<_Vehicle_Interface*>(vehicle);
								router->template traveler<_Traveler_Interface*>(traveler);
								router->template network<_Network_Interface*>(network);

								vehicle->template uuid<int>(traveler_id_counter);
								vehicle->template origin_link<_Link_Interface*>(origin_link);
								vehicle->template destination_link<_Link_Interface*>(destination_link);
								vehicle->template origin_activity_location<_Activity_Location_Interface*>(origin_activity_location);
								vehicle->template destination_activity_location<_Activity_Location_Interface*>(destination_activity_location);
								//vehicle_data.set_seed(scenario_data.iseed);simulation_interval_index

								

								departed_time = simulation_interval_index * scenario->template simulation_interval_length<int>();
								traveler->template Schedule_New_Departure<NULLTYPE>(departed_time);
								vehicle->template departed_simulation_interval_index<int>(departed_time);
								vehicle->template departed_assignment_interval_index<int>(departed_time);
								//cout << endl<<traveler->template uuid<int>() <<"\t"<<departed_time;

								cur_vehicle_rate_per_simulation_interval = (float) (cur_vehicle_rate_per_simulation_interval - 1.0);
							}
							else
							{//monte carlo
								double r1 = (double) g1.RandU01();
								if (r1<=cur_vehicle_rate_per_simulation_interval)
								{

									traveler=(_Traveler_Interface*)Allocate<typename ComponentType::traveler_type>();
									vehicle=(_Vehicle_Interface*)Allocate<typename _Vehicle_Interface::Component_Type>();
									router=(_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();
									
									traveler->template uuid<int>(++traveler_id_counter);
									traveler->template router<_Routing_Interface*>(router);
									traveler->template vehicle<_Vehicle_Interface*>(vehicle);
									router->template traveler<_Traveler_Interface*>(traveler);
									router->template network<_Network_Interface*>(network);

									vehicle->template origin_link<_Link_Interface*>(origin_link);
									vehicle->template destination_link<_Link_Interface*>(destination_link);
									vehicle->template origin_activity_location<_Activity_Location_Interface*>(origin_activity_location);
									vehicle->template destination_activity_location<_Activity_Location_Interface*>(destination_activity_location);
									//vehicle_data.set_seed(scenario_data.iseed);
									


									departed_time = simulation_interval_index * scenario->template simulation_interval_length<int>();
									traveler->template Schedule_New_Departure<NULLTYPE>(departed_time);
									vehicle->template departed_assignment_interval_index<int>(departed_time);
									vehicle->template departed_simulation_interval_index<int>(departed_time);
									//cout << endl<<traveler->template uuid<int>() <<"\t"<<departed_time;
								}

								cur_vehicle_rate_per_simulation_interval = (float)(cur_vehicle_rate_per_simulation_interval - 1.0);
							}
						}
					}




					vehicle_rate_per_simulation_interval = (float)(num_generated_vehicles_ramp / scenario->template simulation_interval_length<float>());
					origin_link = ramp_origin_link;
					origin_activity_location = (_Activity_Location_Interface*)network->template activity_locations_container<_Activity_Locations_Container_Interface&>()[ramp_origin_activity_location_index];


					for (int i=0;i<scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>();i++)
					{
						float cur_vehicle_rate_per_simulation_interval = vehicle_rate_per_simulation_interval;
						while(cur_vehicle_rate_per_simulation_interval>0)
						{
							if (cur_vehicle_rate_per_simulation_interval>=1.0)
							{
								traveler=(_Traveler_Interface*)Allocate<typename ComponentType::traveler_type>();
								vehicle=(_Vehicle_Interface*)Allocate<typename _Vehicle_Interface::Component_Type>();
								router=(_Routing_Interface*)Allocate<typename _Vehicle_Interface::Component_Type>();
								
								traveler->template uuid<int>(++traveler_id_counter);
								traveler->template router<_Routing_Interface*>(router);
								traveler->template vehicle<_Vehicle_Interface*>(vehicle);
								router->template traveler<_Traveler_Interface*>(traveler);
								router->template network<_Network_Interface*>(network);

								vehicle->template origin_link<_Link_Interface*>(origin_link);
								vehicle->template destination_link<_Link_Interface*>(destination_link);
								vehicle->template origin_activity_location<_Activity_Location_Interface*>(origin_activity_location);
								vehicle->template destination_activity_location<_Activity_Location_Interface*>(destination_activity_location);
								//vehicle_data.set_seed(scenario_data.iseed);

								

								departed_time = simulation_interval_index * scenario->template simulation_interval_length<int>();
								traveler->template Schedule_New_Departure<NULLTYPE>(departed_time);
								vehicle->template departed_simulation_interval_index<int>(departed_time);
								vehicle->template departed_assignment_interval_index<int>(departed_time);

								cur_vehicle_rate_per_simulation_interval = (float) (cur_vehicle_rate_per_simulation_interval - 1.0);
								//cout << endl<<"RAMP\t"<<traveler->template uuid<int>() <<"\t"<<departed_time;
							}
							else
							{//monte carlo
								double r1 = (double) g1.RandU01();
								if (r1<=cur_vehicle_rate_per_simulation_interval)
								{
									traveler=(_Traveler_Interface*)Allocate<typename ComponentType::traveler_type>();
									vehicle=(_Vehicle_Interface*)Allocate<typename _Vehicle_Interface::Component_Type>();
									router=(_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();
									
									traveler->template uuid<int>(++traveler_id_counter);
									traveler->template router<_Routing_Interface*>(router);
									traveler->template vehicle<_Vehicle_Interface*>(vehicle);
									router->template traveler<_Traveler_Interface*>(traveler);
									router->template network<_Network_Interface*>(network);

									vehicle->template origin_link<_Link_Interface*>(origin_link);
									vehicle->template destination_link<_Link_Interface*>(destination_link);
									vehicle->template origin_activity_location<_Activity_Location_Interface*>(origin_activity_location);
									vehicle->template destination_activity_location<_Activity_Location_Interface*>(destination_activity_location);
									//vehicle_data.set_seed(scenario_data.iseed);

									

									departed_time = simulation_interval_index * scenario->template simulation_interval_length<int>();
									traveler->template Schedule_New_Departure<NULLTYPE>(departed_time);
									vehicle->template departed_simulation_interval_index<int>(departed_time);
									vehicle->template departed_assignment_interval_index<int>(departed_time);
									//cout << endl<<"RAMP\t"<<traveler->template uuid<int>() <<"\t"<<departed_time;
								}

								cur_vehicle_rate_per_simulation_interval = (float)(cur_vehicle_rate_per_simulation_interval - 1.0);
							}
						}
					}
				}
				
				//done with function
			}
#endif
			feature_prototype void read_demand_data(requires(!check_2(TargetType,Network_Components::Types::ODB_Network,is_same) && !check_2(TargetType,Network_Components::Types::Hard_Coded_Network,is_same)))
			{
				assert_check_2(TargetType,Types::ODB_Network,is_same,"TargetType is ill-defined");
				assert_check_2(TargetType,Types::ODB_Network,is_same,"TargetType should indicate ODB_Network if you want to read it in with ODB");
				assert_check_2(TargetType,Types::Hard_Coded_Network,is_same,"TargetType should indicate Static_Network if you want to read in the hard coded network");
			}

			feature_prototype void read_demand_data(network_models::network_information::scenario_data_information::ScenarioData& scenario_data, network_models::network_information::network_data_information::NetworkData& network_data, network_models::network_information::demand_data_information::DemandData& demand_data)
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_component_interface(_Network_Interface, get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				
				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, _Network_Interface::get_type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);

				define_component_interface(_Vehicle_Interface, ComponentType::traveler_type::vehicle_type, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Routing_Interface, ComponentType::traveler_type::router_type, Routing_Components::Prototypes::Routing_Prototype, ComponentType);
				typedef Traveler_Components::Prototypes::Traveler_Prototype<typename ComponentType::traveler_type, ComponentType> _Traveler_Interface;
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, get_type_of(vehicles_container), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				
				//if (scenario_data.demand_od_flag == 1)
				//{
				//	network_models::network_information::demand_data_information::read_demand_od("", scenario_data, demand_data, network_data);
				//}
				//else
				//{
				//	network_models::network_information::demand_data_information::read_demand_vehicle("", scenario_data, demand_data, network_data);
				//}

				this->first_vehicle_departure_time<int>(demand_data.first_vehicle_departure_time);
				this->last_vehicle_departure_time<int>(demand_data.last_vehicle_departure_time);
				int i;
				for (i = 0; i < demand_data.demand_vehicle_size; i++)
				{
					network_models::network_information::demand_data_information::VehicleData& raw_vehicle = demand_data.vehilce_data_array[i];
					_Vehicle_Interface* vehicle = (_Vehicle_Interface*)Allocate<typename _Vehicle_Interface::Component_Type>(); 
					_Traveler_Interface* traveler=(_Traveler_Interface*)Allocate<typename ComponentType::traveler_type>();
					_Routing_Interface* router=(_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();
								
					vehicle->template uuid<int>(raw_vehicle.get_vehicle_id());
					vehicle->template internal_id<int>(i);
					traveler->template uuid<int>(raw_vehicle.get_vehicle_id());
					traveler->template internal_id<int>(i);
					traveler->template router<_Routing_Interface*>(router);
					traveler->template vehicle<_Vehicle_Interface*>(vehicle);
					router->template traveler<_Traveler_Interface*>(traveler);
					router->template network<_Network_Interface*>(network_reference<_Network_Interface*>());

					int f = raw_vehicle.get_origin_link_index();
					int size = (int)network_reference<_Network_Interface*>()->template links_container<_Links_Container_Interface&>().size();
					vehicle->template origin_link<_Link_Interface*>(network_reference<_Network_Interface*>()->template links_container<_Links_Container_Interface&>().at(raw_vehicle.get_origin_link_index()));
					vehicle->template destination_link<_Link_Interface*>(network_reference<_Network_Interface*>()->template links_container<_Links_Container_Interface&>().at(raw_vehicle.get_destination_link_index()));
					vehicle->template origin_activity_location<_Activity_Location_Interface*>(network_reference<_Network_Interface*>()->template activity_locations_container<_Activity_Locations_Container_Interface&>().at(raw_vehicle.get_origin_activity_location_index()));
					vehicle->template destination_activity_location<_Activity_Location_Interface*>(network_reference<_Network_Interface*>()->template activity_locations_container<_Activity_Locations_Container_Interface&>().at(raw_vehicle.get_destination_activity_location_index()));
					vehicle->template origin_zone<_Zone_Interface*>(network_reference<_Network_Interface*>()->template zones_container<_Zones_Container_Interface&>().at(raw_vehicle.get_origin_zone_index()));
					vehicle->template destination_zone<_Zone_Interface*>(network_reference<_Network_Interface*>()->template zones_container<_Zones_Container_Interface&>().at(raw_vehicle.get_destination_zone_index()));

					int departed_time = raw_vehicle.get_departure_time();
//if (departed_time < 0) {
//	cout << "here" << endl;
//}
					traveler->template Schedule_New_Departure<NULLTYPE>(departed_time);
					vehicle->template departed_simulation_interval_index<int>(raw_vehicle.get_departure_simulation_interval_index());
					vehicle->template departed_assignment_interval_index<int>(raw_vehicle.get_departure_assignment_interval_index());
					vehicle->template departed_time<int>(departed_time);

					vehicles_container<_Vehicles_Container_Interface&>().push_back(vehicle);

				}

				int* toload = new int[scenario_data.planning_horizon];
				
				//for (i = 0; i < scenario_data.planning_horizon; i++) {
				//	toload[i] = 0;
				//	for (int j = 0; j < demand_data.demand_vehicle_size; j++)
				//	{
				//		_Vehicle_Interface* vehicle = vehicles.at(j);
				//		if (vehicle->template departed_time<int>() == i)
				//		{
				//			toload[i]++;
				//			cout << "to load " << vehicle->uuid<int>() << " at link " << vehicle->origin_link<_Link_Interface*>()->uuid<int>() << " at time " << i << endl;
				//		}
				//	}
				//	//cout << "to load at " << i << " = " << toload[i] << endl;
				//}
			}

			feature_prototype void read_demand_data(network_models::network_information::scenario_data_information::ScenarioData& scenario_data, network_models::network_information::network_data_information::NetworkData& network_data, network_models::network_information::demand_data_information::DemandData& demand_data, requires(!check_2(TargetType,Network_Components::Types::File_Network,is_same)))
			{
				assert_check_2(TargetType,Types::File_Network,is_same,"TargetType is ill-defined");
				assert_check_2(TargetType,Types::File_Network,is_same,"TargetType should indicate File_Network if you want to read it in from file");
			}


			feature_prototype void write_demand_data(network_models::network_information::demand_data_information::DemandData& demand_data)
			{
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, get_type_of(vehicles_container), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_component_interface(_Network_Interface, get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, _Network_Interface::get_type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);

				demand_data.first_vehicle_departure_time = this->first_vehicle_departure_time<int>();
				demand_data.last_vehicle_departure_time = this->last_vehicle_departure_time<int>();
				demand_data.time_dependent_vehicle_index_array.resize(scenario_reference<_Scenario_Interface*>()->template planning_horizon<int>());
int counter = -1;

				for (int i = 0; i < (int)vehicles_container<_Vehicles_Container_Interface&>().size(); i++) 
				{
if (++counter % 10000 == 0)
{
	cout << counter << " vehicles converted " << endl;
}
					_Vehicle_Interface* vehicle = vehicles_container<_Vehicles_Container_Interface&>()[i];
					network_models::network_information::demand_data_information::VehicleData vehicle_data;
					
					vehicle_data.set_vehicle_id(vehicle->template uuid<int>());
					vehicle_data.set_vehicle_index(vehicle->template internal_id<int>());

					vehicle_data.set_origin_link_index(vehicle->template origin_link<_Link_Interface*>()->template internal_id<int>());
					vehicle_data.set_destination_link_index(vehicle->template destination_link<_Link_Interface*>()->template internal_id<int>());
					vehicle_data.set_origin_activity_location_index(vehicle->template origin_activity_location<_Activity_Location_Interface*>()->template internal_id<int>());
					vehicle_data.set_destination_activity_location_index(vehicle->template destination_activity_location<_Activity_Location_Interface*>()->template internal_id<int>());
					vehicle_data.set_origin_zone_index(vehicle->template origin_zone<_Zone_Interface*>()->template internal_id<int>());
					vehicle_data.set_destination_zone_index(vehicle->template destination_zone<_Zone_Interface*>()->template internal_id<int>());

					vehicle_data.set_departure_time(vehicle->departed_time<int>());
					vehicle_data.set_departure_simulation_interval_index(vehicle->template departed_simulation_interval_index<int>());
					vehicle_data.set_departure_assignment_interval_index(vehicle->template departed_assignment_interval_index<int>());

					demand_data.vehilce_data_array.push_back(vehicle_data);
					demand_data.time_dependent_vehicle_index_array[vehicle->departed_time<int>()].push_back(vehicle_data.get_vehicle_index());

				}
				demand_data.demand_vehicle_size = (int)demand_data.vehilce_data_array.size();
			}

		};
	}

#ifdef Advanced
	//==================================================================================================================
	/// Demand interfaces namespace
	/// Usually just one interface here, unless a logical split develops at some point.
	//------------------------------------------------------------------------------------------------------------------
	namespace Interfaces
	{
		struct Demand_Interface
		{
			//==================================================================================================================
			/// Demand initializer
			/// Calls the initializer in the connected base, given an input file structure and an iteration time-type structure.
			//------------------------------------------------------------------------------------------------------------------
			template<typename ComponentType, typename CallerType, typename TripType, typename InputFileType, typename TimeStructType>
			void Initialize(InputFileType& input_file_struct, TimeStructType& iteration_step_struct, call_requires(ComponentType, Is_Dispatchable))
			{
				cout <<endl<<"GETTING DEMAND FROM TRANSIMS..."<<endl;
				PTHIS(ComponentType)->template Initialize<Dispatch<ComponentType>,CallerType,TripType,InputFileType,TimeStructType>(input_file_struct,iteration_step_struct);
				cout <<endl<<"DONE."<<endl<<endl;
				schedule_event_local(ComponentType,Get_Trips_Conditional,Get_Trips_Event,0,TripType);
			}


			//==================================================================================================================
			// Interface Event functionality and conditional
			// Notice the event functionality and conditional are entirely contained in the interface and depend on no other vars
			//------------------------------------------------------------------------------------------------------------------
			/// Defines the Get_Trips_Event which is the primary event of the external Demand API
			declare_feature_event(Get_Trips_Event)
			{
				char print;
				cout <<endl<<"Getting Trips"<<endl;
				Demand_Interface* demand = pthis;
				demand->template Get_Current_Trips_From_External<ComponentType,CallerType,TargetType>();
				demand->template Display_Timestep<ComponentType,CallerType,TargetType>();
				demand->template Increment_Time<ComponentType,CallerType,TargetType>();

				cout <<endl<<"Print trips to screen ('y' or 'n'): ";
				cin >> print;

				if (print == 'y' || print == 'Y') demand->template Print_Trips<ComponentType,CallerType,TargetType>();

				demand->template Clear_Trips<ComponentType,CallerType,TargetType>();

			}
			/// Conditional function used to trigger the event feature - currently check every iteration
			declare_feature_conditional(Get_Trips_Conditional)
			{
				response.result = true;
				response.next = iteration + 1;			
			}


			//==================================================================================================================
			// Other Interface functionality
			//------------------------------------------------------------------------------------------------------------------
			//  Local functions
			//------------------------------------------------------------------------------------------------------------------
			/// The Get_Current_Trips_From_External function provides the link to the external program (Transims)
			/// Notice the function requires that it is attached to a transims compliant base using a concept check
			feature void Get_Current_Trips_From_External(call_requirements(requires(ComponentType, Is_Dispatchable) && requires(ComponentType, Concepts::Is_Transims)))
			{
				Time_Components::Interfaces::Time_Interface* tstart = timestep_start<ComponentType,CallerType,Time_Components::Components::Time>();
				Time_Components::Interfaces::Time_Interface* tend = timestep_end<ComponentType,CallerType,Time_Components::Components::Time>();
				
				tstart->template Write<Time_Components::Components::Time,CallerType,int>();
				auto s = tstart->template Convert_Time<Time_Components::Components::Time, CallerType, Time_Components::Data_Structures::Time_DRSeconds>();
				auto e = tend->template Convert_Time<Time_Components::Components::Time, CallerType, Time_Components::Data_Structures::Time_DRSeconds>();

				Time start = Time((int)s.Time);
				Time end = Time((int)e.Time);

				vector<Trip_Info*> v;
				vector<Trip_Info*>::iterator itr;

				Get_Trips(start,end, v);

				int trip_count = 1;
				
				for (itr=v.begin(); itr != v.end(); itr++)
				{
					this->template push_trip<ComponentType,ComponentType,Trip_Components::Components::Trip>(*itr);
					trip_count++;
				}
				cout <<endl<<endl<<"TIMESTEP "<<iteration<<endl<<"Trip count = "<<this->template Trip_Count<ComponentType,CallerType,int>()<<endl;
			}
			/// Time incrementing feature
			feature void Increment_Time()
			{
				auto start = timestep_start<ComponentType,CallerType,Time_Components::Components::Time>();
				auto end = timestep_end<ComponentType,CallerType,Time_Components::Components::Time>();
				auto tstep = this->template t_step<ComponentType,CallerType,Time_Components::Data_Structures::Time_Seconds&>();

				start->template Add_Time<Time_Components::Components::Time,CallerType>(tstep);
				end->template Add_Time<Time_Components::Components::Time,CallerType>(tstep);
			}
			/// Display the current timestep information
			feature void Display_Timestep()
			{
				cout<<"Timestep Start:  ";
				timestep_start<ComponentType,ComponentType,Time_Components::Components::Time>()->template Write<Time_Components::Components::Time,CallerType,int>();
				cout<<endl<<"Timestep End:  ";
				timestep_end<ComponentType,ComponentType,Time_Components::Components::Time>()->template Write<Time_Components::Components::Time,CallerType,int>();			
			}
			//------------------------------------------------------------------------------------------------------------------
			//  Implementation function dispatchers
			//------------------------------------------------------------------------------------------------------------------
			/// Push a trip returned from external source into the local trip list through a base function call
			feature void push_trip(Trip_Info* trip, call_requirements(requires(ComponentType,Is_Dispatchable) && requires(TargetType, Trip_Components::Concepts::Is_Trip)))
			{
				PTHIS(ComponentType)->template push_trip<Dispatch<ComponentType>,CallerType,TargetType>(trip);
			}
			/// Print the current trip list usign base function call
			feature void Print_Trips(call_requires(ComponentType, Is_Dispatchable))
			{
				PTHIS(ComponentType)->template Print_Trips<Dispatch<ComponentType>,CallerType,TargetType>();
			}
			/// Get a count of the number of trips returned in current timestep using base function call
			feature TargetType Trip_Count(call_requires(ComponentType,Is_Dispatchable))
			{
				return PTHIS(ComponentType)->template Trip_Count<Dispatch<ComponentType>,CallerType,TargetType>();
			}
			/// Clear the current trip list using base function call
			feature void Clear_Trips(call_requires(ComponentType, Is_Dispatchable))
			{
				PTHIS(ComponentType)->template Clear_Trips<Dispatch<ComponentType>,CallerType,TargetType>();
			}


			//==================================================================================================================
			// Accessor features - notice no types are associated with accessors in the interface
			//------------------------------------------------------------------------------------------------------------------
			/// filename accessor
			feature_accessor(filename);
			/// Get the timestep length
			feature_accessor(timestep_length);
			/// Get a time structure representing the current timestep
			feature_accessor(t_step);
			/// Get the trips in the current timestep
			feature_accessor(current_trips);
			/// Get some type of interface to the timestep start time
			feature_accessor_interface(timestep_start);
			/// Get some type of interface to the timestep end time
			feature_accessor_interface(timestep_end);

		};
	}



	//==================================================================================================================
	/// Defines useful data structures for base classes which carry tagged semantic information
	///
	//------------------------------------------------------------------------------------------------------------------
	namespace Data_Structures
	{
		struct Transims_Control_File_CSV_Struct
		{
			typedef Types::ModelTypes::TransimsType TransimsType;
			typedef Types::FileFormatTypes::CSV_Delimited CSV_Delimited;
			typedef Types::FileTypes::ControlFile ControlFile;
			char* filename;
		};
		struct Transims_Control_File_TAB_Struct
		{
			typedef Types::ModelTypes::TransimsType TransimsType;
			typedef Types::FileFormatTypes::Tab_Delimited Tab_Delimited;
			typedef Types::FileTypes::ControlFile ControlFile;
			char* filename;
		};
		struct Transims_Trip_File_CSV_Struct
		{
			typedef Types::ModelTypes::TransimsType TransimsType;
			typedef Types::FileFormatTypes::CSV_Delimited CSV_Delimited;
			typedef Types::FileTypes::TripFile TripFile;
			char* filename;
		};
		struct Transims_Trip_File_TAB_Struct
		{
			typedef Types::ModelTypes::TransimsType TransimsType;
			typedef Types::FileFormatTypes::Tab_Delimited Tab_Delimited;
			typedef Types::FileTypes::TripFile TripFile;
			char* filename;
		};
		struct Transims_Trip_File_BIN_Struct
		{
			typedef Types::ModelTypes::TransimsType TransimsType;
			typedef Types::FileFormatTypes::Binary Binary;
			typedef Types::FileTypes::TripFile TripFile;
			char* filename;
		};
	}
#endif
}

using namespace Demand_Components::Prototypes;