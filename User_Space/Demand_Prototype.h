#pragma once

#include "User_Space_Includes.h"
#include "../File_IO/network_demand_data.h"

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
//#ifndef FOR_LINUX_PORTING
			feature_prototype void read_demand_data(typename TargetType::ParamType& network_mapping,
				requires(check_2(typename TargetType::NetIOType,Network_Components::Types::ODB_Network,is_same)))
			{
				using namespace odb;
				using namespace polaris::io;
				define_component_interface(_Scenario_Interface, typename get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);

				string name(scenario_reference<_Scenario_Interface*>()->template database_name<string&>());
				auto_ptr<database> db (open_sqlite_demand_database (name));

				define_component_interface(_Network_Interface, typename get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);

				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, _Network_Interface::get_type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Link_Interface::get_type_of(outbound_turn_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, typename get_type_of(vehicles_container), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				typedef Traveler_Components::Prototypes::Traveler_Prototype<typename ComponentType::traveler_type, ComponentType> _Traveler_Interface;
				define_component_interface(_Routing_Interface, _Traveler_Interface::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
//				define_component_interface(_Plan_Interface, _Traveler_Interface::get_type_of(plan), Plan_Components::Prototypes::Plan_Prototype, ComponentType);
//				define_component_interface(_Movement_Plan_Interface, _Plan_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, typename get_type_of(vehicles_container), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Movement_Plan_Interface, typename _Vehicle_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				

				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename get_type_of(network_reference)::type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);



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
					_Activity_Location_Interface& activity_location=iterator_to_interface(_Activity_Location_Interface,activity_locations_itr);

					activity_id_to_ptr[activity_location.uuid<int>()]=&activity_location;
				}

				cout << "Reading Travelers" << endl;


				int counter = -1;
				int departed_time;
				int skipped_counter=0;

				this->template first_vehicle_departure_time<int>(24*60*60.0);
				this->template last_vehicle_departure_time<int>(0.0);


				int simulation_start_time = scenario->template simulation_start_time<int>();
				int simulation_end_time = scenario->template simulation_end_time<int>();

				for(result<Trip>::iterator db_itr = trip_result.begin (); db_itr != trip_result.end (); ++db_itr)
				{
cout << "one demand record read" << endl;
					if (++counter % 10000 == 0)
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

					traveler->template uuid<int>(++traveler_id_counter);
					traveler->template internal_id<int>(traveler_id_counter);
					traveler->template router<_Routing_Interface*>(router);
					traveler->template vehicle<_Vehicle_Interface*>(vehicle);
//					traveler->template plan<_Plan_Interface*>(plan);

					router->template traveler<_Traveler_Interface*>(traveler);
					router->template network<_Network_Interface*>(network);

					vehicle->template uuid<int>(traveler_id_counter);
					vehicle->template internal_id<int>(traveler_id_counter);
					vehicle->template movement_plan<_Movement_Plan_Interface*>(movement_plan);

//					plan->template movement_plan<_Movement_Plan_Interface*>(movement_plan);
//					plan->template traveler<_Traveler_Interface*>(traveler);

					movement_plan->template origin<_Link_Interface*>(origin_link);
					movement_plan->template destination<_Link_Interface*>(destination_link);
					movement_plan->template departed_time<int>(departed_time);
//					movement_plan->template plan<_Plan_Interface*>(plan);

					traveler->template Schedule_New_Departure<NULLTYPE>(departed_time);

					vehicles_container<_Vehicles_Container_Interface&>().push_back(vehicle);

					if(traveler_id_counter%10000==0)
					{
						cout << "\t" << traveler_id_counter << endl;
					}
				}
			}
//#endif

			feature_prototype void read_demand_data(requires(!check_2(TargetType,typename Network_Components::Types::ODB_Network,is_same) && !check_2(TargetType,typename Network_Components::Types::File_Network,is_same)))
			{
				assert_check_2(TargetType,typename Network_Components::Types::ODB_Network,is_same,"TargetType is ill-defined");
				assert_check_2(TargetType,typename Network_Components::Types::ODB_Network,is_same,"TargetType should indicate ODB_Network if you want to read it in with ODB");
				assert_check_2(TargetType,typename Network_Components::Types::File_Network,is_same,"TargetType should indicate Static_Network if you want to read in the hard coded network");
			}

			feature_prototype void read_demand_data(network_models::network_information::scenario_data_information::ScenarioData& scenario_data, network_models::network_information::network_data_information::NetworkData& network_data, network_models::network_information::demand_data_information::DemandData& demand_data)
			{
				define_component_interface(_Scenario_Interface, typename get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_component_interface(_Network_Interface, typename get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				
				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
#ifndef EXCLUDE_DEMAND
				typedef Person_Components::Prototypes::Person<typename ComponentType::traveler_type, ComponentType> _Traveler_Interface;
#else
				typedef Traveler_Components::Prototypes::Traveler_Prototype<typename ComponentType::traveler_type, ComponentType> _Traveler_Interface;
#endif
				define_component_interface(_Routing_Interface, typename _Traveler_Interface::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
#ifndef EXCLUDE_DEMAND
				define_component_interface(_Planning_Interface, typename _Traveler_Interface::get_type_of(Planning_Faculty), Person_Components::Prototypes::Person_Planner, ComponentType);
#endif
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, typename get_type_of(vehicles_container), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Movement_Plan_Interface, typename _Vehicle_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				
				this->template first_vehicle_departure_time<int>(demand_data.first_vehicle_departure_time);
				this->template last_vehicle_departure_time<int>(demand_data.last_vehicle_departure_time);
				int i;
				for (i = 0; i < demand_data.demand_vehicle_size; i++)
				{
					network_models::network_information::demand_data_information::VehicleData& raw_vehicle = demand_data.vehilce_data_array[i];
					_Vehicle_Interface* vehicle = (_Vehicle_Interface*)Allocate<typename _Vehicle_Interface::Component_Type>(); 
					_Traveler_Interface* traveler=(_Traveler_Interface*)Allocate<typename ComponentType::traveler_type>();
					_Routing_Interface* router=(_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();
					//_Plan_Interface* plan = (_Plan_Interface*)Allocate<typename _Plan_Interface::Component_Type>();
					_Movement_Plan_Interface* movement_plan = (_Movement_Plan_Interface*)Allocate<typename _Movement_Plan_Interface::Component_Type>();
#ifndef EXCLUDE_DEMAND
					_Planning_Interface* planner = (_Planning_Interface*)Allocate<_Traveler_Interface::get_type_of(Planning_Faculty)>();
#endif

					vehicle->template uuid<int>(raw_vehicle.get_vehicle_id());
					vehicle->template internal_id<int>(i);
					vehicle->template movement_plan<_Movement_Plan_Interface*>(movement_plan);
					vehicle->template traveler<_Traveler_Interface*>(traveler);

					traveler->template uuid<int>(raw_vehicle.get_vehicle_id());
					traveler->template internal_id<int>(i);
					traveler->template router<_Routing_Interface*>(router);
					traveler->template vehicle<_Vehicle_Interface*>(vehicle);
#ifndef EXCLUDE_DEMAND
					traveler->template Planning_Faculty<_Planning_Interface*>(planner);
					traveler->template Planning_Faculty<_Planning_Interface*>()->template Parent_Person<_Traveler_Interface*>(traveler);
#endif
					router->template traveler<_Traveler_Interface*>(traveler);
					router->template network<_Network_Interface*>(network_reference<_Network_Interface*>());

					int f = raw_vehicle.get_origin_link_index();
					int size = (int)network_reference<_Network_Interface*>()->template links_container<_Links_Container_Interface&>().size();
										
					movement_plan->template origin<_Link_Interface*>(network_reference<_Network_Interface*>()->template links_container<_Links_Container_Interface&>().at(raw_vehicle.get_origin_link_index()));
					movement_plan->template destination<_Link_Interface*>(network_reference<_Network_Interface*>()->template links_container<_Links_Container_Interface&>().at(raw_vehicle.get_destination_link_index()));

					int departed_time = raw_vehicle.get_departure_time();
#ifndef EXCLUDE_DEMAND
					traveler->template Planning_Faculty<_Planning_Interface*>()->template Schedule_New_Departure<NULLTYPE>(departed_time);
#else
					traveler->template Schedule_New_Departure<NULLTYPE>(departed_time);
#endif
					movement_plan->template departed_time<Time_Seconds>(departed_time);

					vehicles_container<_Vehicles_Container_Interface&>().push_back(vehicle);

				}
				
			}

			feature_prototype void read_demand_data(network_models::network_information::scenario_data_information::ScenarioData& scenario_data, network_models::network_information::network_data_information::NetworkData& network_data, network_models::network_information::demand_data_information::DemandData& demand_data, requires(!check_2(TargetType,typename Network_Components::Types::File_Network,is_same)))
			{
				assert_check_2(TargetType,typename Network_Components::Types::File_Network,is_same,"TargetType is ill-defined");
				assert_check_2(TargetType,typename Network_Components::Types::File_Network,is_same,"TargetType should indicate File_Network if you want to read it in from file");
			}


			feature_prototype void write_demand_data(network_models::network_information::demand_data_information::DemandData& demand_data)
			{
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, typename get_type_of(vehicles_container), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Network_Interface, typename get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
				typedef Person_Components::Prototypes::Person<typename ComponentType::traveler_type, ComponentType> _Traveler_Interface;
//				define_component_interface(_Plan_Interface, typename _Traveler_Interface::get_type_of(plan), Plan_Components::Prototypes::Plan_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, typename get_type_of(vehicles_container), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Movement_Plan_Interface, typename _Vehicle_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);

//				define_component_interface(_Movement_Plan_Interface, typename _Plan_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
                define_component_interface(_Scenario_Interface, typename get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);

				demand_data.first_vehicle_departure_time = this->template first_vehicle_departure_time<int>();
				demand_data.last_vehicle_departure_time = this->template last_vehicle_departure_time<int>();
				demand_data.time_dependent_vehicle_index_array.resize(scenario_reference<_Scenario_Interface*>()->template planning_horizon<int>());
				int counter = -1;

				for (int i = 0; i < (int)vehicles_container<_Vehicles_Container_Interface&>().size(); i++) 
				{
					if (++counter % 10000 == 0)
					{
						cout << counter << " vehicles converted " << endl;
					}
					_Vehicle_Interface* vehicle = vehicles_container<_Vehicles_Container_Interface&>()[i];
					_Movement_Plan_Interface* movement_plan = vehicle->template movement_plan<_Movement_Plan_Interface*>();
					network_models::network_information::demand_data_information::VehicleData vehicle_data;
					
					vehicle_data.set_vehicle_id(vehicle->template uuid<int>());
					vehicle_data.set_vehicle_index(vehicle->template internal_id<int>());

					vehicle_data.set_origin_link_index(movement_plan->template origin<_Link_Interface*>()->template internal_id<int>());
					vehicle_data.set_destination_link_index(movement_plan->template destination<_Link_Interface*>()->template internal_id<int>());
					vehicle_data.set_origin_activity_location_index(0);
					vehicle_data.set_destination_activity_location_index(0);
					vehicle_data.set_origin_zone_index(0);
					vehicle_data.set_destination_zone_index(0);

					vehicle_data.set_departure_time(movement_plan->template departed_time<int>());
					vehicle_data.set_departure_simulation_interval_index(movement_plan->template departed_time<int>() / ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>());
					vehicle_data.set_departure_assignment_interval_index(movement_plan->template departed_time<int>() / ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>());

					demand_data.vehilce_data_array.push_back(vehicle_data);
					demand_data.time_dependent_vehicle_index_array[movement_plan->template departed_time<int>()].push_back(vehicle_data.get_vehicle_index());

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
				demand->template Get_Current_Trips_From_External<CallerType,TargetType>();
				demand->template Display_Timestep<CallerType,TargetType>();
				demand->template Increment_Time<CallerType,TargetType>();

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