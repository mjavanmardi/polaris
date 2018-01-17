#pragma once
#include "Traffic_Simulator.h"
#include "Traveler_Simulator.h"
#include "Activity_Location_Prototype.h"
#include "Traffic_Simulator_Concepts.h"

namespace Trip_Components
{
	namespace Types
	{
		enum CALCULATION_STATUS
		{
			PROCESSING,
			COMPLETE
		};
		enum SUB_ITERATIONS
		{
			INITIALIZE = 19,
			PROCESS = 20,
			PATH_BUILDING = 21,
			UPDATE = 22,
			GET_RESULTS=23
		};
	}

	namespace Prototypes
	{
		prototype struct Assignment_Analyzer ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(routed_trips_container, NONE, NONE);
			accessor(network_reference, NONE, NONE);		

			static void Assignment_Analyzer_Conditional(ComponentType* _this,Event_Response& response)
			{
				typedef Assignment_Analyzer<ComponentType> _This_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_This_Interface* this_ptr=(_This_Interface*)_pthis;

				this_ptr->write_analysis_results();

				response.next._iteration = END;
				response.next._sub_iteration = END;
			}

			void read_trip_data()
			{
				this_component()->Load_Event<ComponentType>(Assignment_Analyzer_Conditional,END-2,0);

				int counter = -1;
				int trip_id = 0;
				int veh_id = 0;

				try
				{
					typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
					typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
					typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename _Network_Interface::get_type_of(activity_locations_container)::value_type>::type>  _Activity_Location_Interface;
					typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container), _Activity_Location_Interface*> _Activity_Locations_Container_Interface;
					typedef  Link_Components::Prototypes::Link<typename MasterType::link_type>  _Link_Interface;
					typedef  Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links), _Link_Interface*> _Links_Container_Interface;
					typedef  Random_Access_Sequence< typename get_type_of(routed_trips_container)> _Trips_Container_Interface;
					typedef  Routed_Trip< get_component_type(_Trips_Container_Interface)> _Trip_Interface;

					
					_Network_Interface* network=network_reference<_Network_Interface*>();
					_Activity_Locations_Container_Interface& activity_locations = network->template activity_locations_container<_Activity_Locations_Container_Interface&>();
					_Scenario_Interface* scenario=(_Scenario_Interface*)_global_scenario;
					_Trips_Container_Interface* trips = this->routed_trips_container<_Trips_Container_Interface*>();

					// Open the demand database from the historical results database filepath
					string name(scenario->template historical_results_database_name<string&>());
					unique_ptr<odb::database> db (open_sqlite_demand_database (name));
					odb::transaction t(db->begin());
					odb::result<polaris::io::Trip> trip_result=db->query<polaris::io::Trip>(odb::query<polaris::io::Trip>::true_expr);	

				
					// Build activity location id to pointer map
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

					int simulation_start_time = scenario->template simulation_start_time<int>();
					int simulation_end_time = scenario->template simulation_end_time<int>();

					cout << "Reading Travelers" << endl;
					int counter = 0;
					int skipped_counter = 0;
					for(odb::result<polaris::io::Trip>::iterator db_itr = trip_result.begin (); db_itr != trip_result.end (); ++db_itr)
					{
						trip_id = db_itr->getPrimaryKey();
						
						int mode = db_itr->getMode();
						if (mode > 0) continue;

						if (mode == 0) veh_id = db_itr->getVehicle()->getPrimaryKey();

						if (++counter % 100000 == 0) cout << counter << " trips processed" << endl;

						// Get the departure and OD info from DB, and validate them
						int departed_time = db_itr->getStart();
						if (departed_time < simulation_start_time || departed_time >= simulation_end_time) {
							continue;
						}
						int org_key=db_itr->getOrigin();
						if(!activity_id_to_ptr.count(org_key))
						{
							if(++skipped_counter%100000==0) cout << skipped_counter << " trips skipped" << endl;
							continue;
						}
						int dst_key=db_itr->getDestination();
						if(!activity_id_to_ptr.count(dst_key))
						{
							if(++skipped_counter%100000==0) cout << skipped_counter << " trips skipped" << endl;
							continue;
						}

						int travel_time = db_itr->getEnd() - departed_time;

						// ignore non-travelling trips
						if (travel_time <= 0 || org_key == dst_key) continue;

						_Trip_Interface* trip=(_Trip_Interface*)Allocate<typename MasterType::trip_type>();
						trip->Initialize<_Network_Interface*,_Activity_Location_Interface*,Simulation_Timestep_Increment>(trip_id, network,activity_id_to_ptr[org_key], activity_id_to_ptr[dst_key],departed_time);
						trip->input_travel_time<Simulation_Timestep_Increment>(travel_time);
						trip->veh_id(veh_id);
						trips->push_back(trip);
					}
				}
				catch (...)
				{
					THROW_EXCEPTION("Error: seems to be an ODB exception here, reading trip number "<<counter<<", trip id="<<trip_id);
				}
			}

			void write_analysis_results()
			{
				typedef  Random_Access_Sequence< typename get_type_of(routed_trips_container)> _Trips_Container_Interface;
				typedef  Routed_Trip< get_component_type(_Trips_Container_Interface)> _Trip_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef  Activity_Location_Components::Prototypes::Activity_Location<typename _Trip_Interface::get_type_of(origin)>  _Activity_Location_Interface;
				typedef Random_Access_Sequence<typename _Trip_Interface::get_type_of(result_trajectory)> _Trajectory_Container_Interface;
				typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<get_component_type(_Trajectory_Container_Interface)> _Trajectory_Unit_Interface;
				

				string filename(((_Scenario_Interface*)_global_scenario)->template historical_results_database_name<string&>());
				string extension("-assignment_analysis.csv");
				filename = filename + extension;

				File_IO::File_Writer fh;
				fh.Open(filename);
				stringstream out("");
				fh.Write_Line("Record_Type,Trip_ID,Veh_ID,O,D,Depart,TTime_minimum,TTime_simulated,TTimes(T)/Links(L)");

				_Trips_Container_Interface* trips = this->routed_trips_container<_Trips_Container_Interface*>();

				double ttime_min = 0;
				double ttime_sim = 0;
				

				for (_Trips_Container_Interface::iterator itr = trips->begin(); itr != trips->end(); ++itr)
				{
					_Trip_Interface* trip = (_Trip_Interface*)*itr;
					_Activity_Location_Interface* o = trip->origin<_Activity_Location_Interface*>();
					_Activity_Location_Interface* d = trip->destination<_Activity_Location_Interface*>();
					ttime_min += trip->result_travel_time<Simulation_Timestep_Increment>();
					ttime_sim += trip->input_travel_time<Simulation_Timestep_Increment>();

					out<<"T,"<<trip->trip_id<int>()<<","<<trip->veh_id<int>()<<","<<o->uuid<int>()<<","<<d->uuid<int>()<<","<<trip->departure_time<Simulation_Timestep_Increment>()<<","<<trip->result_travel_time<Time_Minutes>()<<","<<trip->input_travel_time<Time_Minutes>()<<","<<trip->Print_Trajectory_Times();
					fh.Write_Line(out);
					out<<"L,"<<trip->trip_id<int>()<<","<<trip->veh_id<int>()<<","<<o->uuid<int>()<<","<<d->uuid<int>()<<","<<trip->departure_time<Simulation_Timestep_Increment>()<<","<<trip->result_travel_time<Time_Minutes>()<<","<<trip->input_travel_time<Time_Minutes>()<<","<<trip->Print_Trajectory_Links();
					fh.Write_Line(out);
				}

				cout <<"Relative gap = "<<(ttime_sim - ttime_min)/ttime_min;

				File_IO::File_Writer fhr;
				string res_filename(((_Scenario_Interface*)_global_scenario)->template historical_results_database_name<string&>());
				string res_extension("-assignment_results.csv");
				res_filename = res_filename + res_extension;
				fhr.Open(res_filename);
				fhr.Write_Line("TTIME_MINIMUM,TTIME_SIMULATION,REL_GAP");
				out<<ttime_min<<","<<ttime_sim<<","<<(ttime_sim - ttime_min)/ttime_min;
				fhr.Write_Line(out);
				fhr.Close();

			}

		};

		prototype struct Routed_Trip ADD_DEBUG_INFO
		{
			tag_as_prototype;

			//=============================================
			// Primary events
			//---------------------------------------------
			static void Trip_Conditional(ComponentType* _this,Event_Response& response)
			{
				typedef Routed_Trip<ComponentType> _This_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_This_Interface* this_ptr=(_This_Interface*)_pthis;

				this_ptr->Get_Results<NT>();

				response.next._iteration = END;
				response.next._sub_iteration = END;
			}


			//=============================================
			// Primary data accessors
			//---------------------------------------------
			// links back to the network to be skimmed
			accessor(trip_id,NONE,NONE);
			accessor(veh_id,NONE,NONE);
			accessor(network_reference,NONE,NONE);
			accessor(router,NONE,NONE);
			accessor(origin,NONE,NONE);
			accessor(destination,NONE,NONE);
			accessor(mode,NONE,NONE);
			accessor(departure_time,NONE,NONE);
			accessor(input_travel_time,NONE,NONE);
			accessor(result_travel_time,NONE,NONE);
			accessor(result_distance,NONE,NONE);
			accessor(result_trajectory,NONE,NONE);
			accessor(link_failure,NONE,NONE);
			accessor(turn_failure,NONE,NONE);
			accessor(results_processed,NONE,NONE);

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			template<typename NetworkType, typename LocationType, typename TimeType> void Initialize(int Trip_Id, NetworkType network_ref, LocationType orig, LocationType dest, TimeType depart, requires(LocationType,check(LocationType, is_pointer) && check(strip_modifiers(LocationType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename _Network_Interface::get_type_of(activity_locations_container)::value_type>::type>  _Activity_Location_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container), _Activity_Location_Interface*> _Activity_Locations_Container_Interface;
				typedef  Link_Components::Prototypes::Link<typename MasterType::link_type>  _Link_Interface;
				typedef  Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links), _Link_Interface*> _Links_Container_Interface;
				typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename _Network_Interface::get_type_of(zones_container)::data_type>::type>  _Zone_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(zones_container), _Zone_Interface*> _Zones_Container_Interface;
				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Link_Interface::get_type_of(outbound_turn_movements)::value_type>::type>  _Movement_Interface;
				typedef  Random_Access_Sequence< typename _Link_Interface::get_type_of(outbound_turn_movements), _Movement_Interface*> _Movements_Container_Interface;
				typedef  Routing_Components::Prototypes::Routing< get_type_of(router) > _Routing_Interface;
				typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename MasterType::movement_plan_type> _Movement_Plan_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

				this->network_reference<_Network_Interface*>(network_ref);
				this->trip_id(Trip_Id);
				this->results_processed(false);
				_Network_Interface* network=network_reference<_Network_Interface*>();
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				_Activity_Locations_Container_Interface& activity_locations = network->template activity_locations_container<_Activity_Locations_Container_Interface&>();
				
				
				// Validate departure time
				int departed_time = GLOBALS::Time_Converter.Convert_Value<TimeType,Simulation_Timestep_Increment>(depart);
				this->departure_time<TimeType>(depart);
				int simulation_start_time = scenario->template simulation_start_time<int>();
				int simulation_end_time = scenario->template simulation_end_time<int>();
				if (departed_time < simulation_start_time || departed_time >= simulation_end_time)
				{
					cout<<"Trip start time is out of bounds. Trip "<<Trip_Id<<" is skipped."<<endl;
					return;
				}

				// Validate that origin/destination pair is routable
				_Activity_Location_Interface* origin_activity_location = orig;
				this->origin<_Activity_Location_Interface*>(orig);
				_Activity_Location_Interface* destination_activity_location = dest;
				this->destination<_Activity_Location_Interface*>(dest);
				_Link_Interface* origin_link = origin_activity_location->template origin_links<_Links_Container_Interface&>()[0];
				_Link_Interface* destination_link = destination_activity_location->template destination_links<_Links_Container_Interface&>()[0];
				if (origin_link->template internal_id<int>() == destination_link->template internal_id<int>()  || (origin_link->template outbound_turn_movements<_Movements_Container_Interface&>().size() == 0 || destination_link->template inbound_turn_movements<_Movements_Container_Interface&>().size() == 0))
				{
					int o = origin_link->template internal_id<int>();
					int d = destination_link->template internal_id<int>();
					int otm = origin_link->template outbound_turn_movements<_Movements_Container_Interface&>().size();
					int dtm = destination_link->template inbound_turn_movements<_Movements_Container_Interface&>().size();
					//cout<<"Origin/Destination pair is not routable. Trip "<<Trip_Id<<" is skipped."<<endl;
					return;
				}

				// Allocate linked classes
				_Routing_Interface* router=(_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();
				_Movement_Plan_Interface* movement_plan = (_Movement_Plan_Interface*)Allocate<typename _Movement_Plan_Interface::Component_Type>();
				this->router<_Routing_Interface*>(router);

				// Assign values to sub-classes used in routing
				movement_plan->network<_Network_Interface*>(network);
				movement_plan->template origin<_Link_Interface*>(origin_link);
				movement_plan->template destination<_Link_Interface*>(destination_link);
				movement_plan->template origin<_Activity_Location_Interface*>(orig);
				movement_plan->template destination<_Activity_Location_Interface*>(dest);
				movement_plan->template departed_time<Time_Seconds>(departed_time);
				movement_plan->template initialize_trajectory<NULLTYPE>();
				movement_plan->traveler_id(Trip_Id);

				router->template network<_Network_Interface*>(network);
				router->Attach_New_Movement_Plan<typename _Movement_Plan_Interface::Component_Type>(movement_plan);
				
				// Get a random timestep in which to perform route calculation
				Simulation_Timestep_Increment dt = GLOBALS::Uniform_RNG.Next_Rand<double>() * (END - 10 -iteration());
				Simulation_Timestep_Increment t = GLOBALS::Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(dt);

				// Schedule routing at timestep t + 1
				router->Schedule_Route_Computation(departed_time, t+1);

				// Load the trip conditional at timestep t + 5, which fills the trip results after routing is completed
				this_component()->Load_Event<ComponentType>(Trip_Conditional,t+5,Types::SUB_ITERATIONS::INITIALIZE);
			}			
			template<typename NetworkType, typename LocationType, typename TimeType> void Initialize(int Trip_Id, NetworkType network, LocationType orig, LocationType dest, TimeType depart, requires(LocationType,!check(LocationType, is_pointer) || !check(strip_modifiers(LocationType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				assert_check(TargetType, is_pointer,"TargetType is not a pointer" );
				assert_check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype, "TargetType is not a valid location interface");
			}			
			
			template<typename TargetType> void Get_Results()
			{
				typedef Routing_Components::Prototypes::Routing< get_type_of(router) > _Routing_Interface;
				typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename _Routing_Interface::get_type_of(movement_plan)> _Movement_Plan_Interface;
				typedef Random_Access_Sequence<typename _Movement_Plan_Interface::get_type_of(trajectory_container)> _Trajectory_Container_Interface;
				typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<get_component_type(_Trajectory_Container_Interface)> _Trajectory_Unit_Interface;
				typedef Random_Access_Sequence<typename get_type_of(result_trajectory)> _Result_Trajectory_Container_Interface;
				
				this->results_processed(true);

				_Routing_Interface* router = this->router<_Routing_Interface*>();
				_Movement_Plan_Interface* movement = router->movement_plan<_Movement_Plan_Interface*>();

				if (movement->valid_trajectory<bool>())
				{
					this->result_travel_time<Simulation_Timestep_Increment>(movement->routed_travel_time<float>());
										
					_Trajectory_Container_Interface* trajectory = movement->trajectory_container<_Trajectory_Container_Interface*>();
					_Result_Trajectory_Container_Interface& result_trajectory = this->result_trajectory<_Result_Trajectory_Container_Interface&>();

					if (trajectory->size() == 0)
					{
						cout <<"Error: null trajectory.";
					}

					for (_Trajectory_Container_Interface::iterator itr = trajectory->begin(); itr != trajectory->end(); ++itr)
					{
						_Trajectory_Unit_Interface* tu = (_Trajectory_Unit_Interface*)*itr;
						result_trajectory.push_back(tu);
					}

					//for (_Trajectory_Container_Interface::iterator itr = trajectory->begin(); itr != trajectory->end(); ++itr)
					//{
					//	_Trajectory_Unit_Interface* tu = (_Trajectory_Unit_Interface*)*itr;
					//	link_itf* link = tu->link<link_itf*>();
					//	cout << link->dbid<int>()<<"."<<link->direction<int>()<<": "<<tu->estimated_link_accepting_time<int>()<<endl;
					//}				
				}
				else
				{
					cout<<"Error: movement was not routable."<<endl;
				}
				
			}

			string Print_Trajectory_Times()
			{
				typedef Random_Access_Sequence<get_type_of(result_trajectory)> _Trajectory_Container_Interface;
				typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<get_component_type(_Trajectory_Container_Interface)> _Trajectory_Unit_Interface;
				typedef Link_Components::Prototypes::Link<typename _Trajectory_Unit_Interface::get_type_of(link)> _Link_Interface;

				stringstream result("");

				_Trajectory_Container_Interface* trajectory = result_trajectory<_Trajectory_Container_Interface*>();

				for (_Trajectory_Container_Interface::iterator itr = trajectory->begin(); itr != trajectory->end(); ++itr)
				{
					_Trajectory_Unit_Interface* tu = (_Trajectory_Unit_Interface*)*itr;
					_Link_Interface* link = tu->link<_Link_Interface*>();
					result<<link->dbid<int>()<<"."<<link->direction<int>()<<",";
				}
				return result.str();
			}
			string Print_Trajectory_Links()
			{
				typedef Random_Access_Sequence<get_type_of(result_trajectory)> _Trajectory_Container_Interface;
				typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<get_component_type(_Trajectory_Container_Interface)> _Trajectory_Unit_Interface;
				typedef Link_Components::Prototypes::Link<typename _Trajectory_Unit_Interface::get_type_of(link)> _Link_Interface;

				stringstream result("");

				_Trajectory_Container_Interface* trajectory = result_trajectory<_Trajectory_Container_Interface*>();

				if (trajectory->size() == 0)
				{
					//cout <<"Error: null trajectory.";
					return result.str();
				}

				_Trajectory_Container_Interface::iterator itr = trajectory->begin();
				++itr;
				int stored_time = 0;
				for (; itr != trajectory->end(); ++itr)
				{
					_Trajectory_Unit_Interface* tu = (_Trajectory_Unit_Interface*)*itr;
					_Link_Interface* link = tu->link<_Link_Interface*>();
					result<<tu->estimated_link_accepting_time<int>()-stored_time<<",";
					stored_time = tu->estimated_link_accepting_time<int>();
				}
				return result.str();
			}
			
		};

	}
}

