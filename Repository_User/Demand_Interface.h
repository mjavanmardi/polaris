#pragma once

#include "Repository_User_Includes.h"

#ifdef Advanced
#include "Transims_Demand_API.h"
#endif

namespace Demand_Components
{
	//==================================================================================================================
	/// TYPE Of interest to the Demand Interface.
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
	}

	//==================================================================================================================
	/// Concept checks related to the Demand generation interface
	//------------------------------------------------------------------------------------------------------------------
	namespace Concepts
	{
		concept Is_Transims
		{
			begin_requirements_list;
			requires_typename_state(none,TransimsType,true_type,"Type is not a Transims type");
			end_requirements_list(TransimsType);
		};
		concept Is_Trip_File
		{
			begin_requirements_list;
			requires_typename_state(none,TripFile,true_type,"Type is not in a TripFile");
			requires_typed_member(TripFile, filename,char*,"Type does not have a 'filename' member of char* type");
			end_requirements_list(filename);
		};
		concept Is_Control_File
		{
			begin_requirements_list;
			requires_typename_state(none,ControlFile,true_type,"Type is not in a ControlFile");
			requires_typed_member(ControlFile, filename, char*,"Type does not have a 'filename' member of char* type");
			end_requirements_list(filename);
		};
		concept Is_CSV_Delimited
		{
			begin_requirements_list;
			requires_typename_state(none,CSV_Delimited,true_type,"Type is not CSV_Delimited");
			end_requirements_list(CSV_Delimited);
		};
		concept Is_Tab_Delimited
		{
			begin_requirements_list;
			requires_typename_state(none,Tab_Delimited,true_type,"Type is not Tab_Delimited");
			end_requirements_list(Tab_Delimited);
		};
		concept Is_Transims_Control_File
		{
			begin_requirements_list;
			requires_concept(none,Is_Transims);
			requires_concept(Is_Transims,Is_Control_File);
			end_requirements_list(Is_Control_File);
		};
		concept Is_Transims_Trip_File
		{
			begin_requirements_list;
			requires_concept(none,Is_Transims);
			requires_concept(Is_Transims,Is_Trip_File);
			end_requirements_list(Is_Trip_File);
		};

	}
	
	namespace Interfaces
	{
		template<typename ThisType,typename CallerType>
		struct Demand_Interface
		{
			facet_accessor(scenario_reference);
			facet_accessor(network_reference);

			facet void read_demand_data()
			{
				float freeway_demand_rate = 1800.0;
				float ramp_demand_rate = 900.0;
				
				//RngStream g1("Demand");
				//g1.SetSeed(1);
				
				//int vehilce_index = -1;



				//sort demand by departure time
				//demand_data.time_dependent_vehicle_index_array.resize(scenario_data.num_simulation_intervals*scenario_data.simulation_interval_length);

				typedef typename ThisType::network_type NetworkType;
				typedef Network_Components::Interfaces::Network_Interface<NetworkType,ThisType> Network_Interface;

				Network_Interface* network=network_reference<Network_Interface*>();
				
				typedef typename ThisType::links_container_type LinksContainerType;
				typedef typename ThisType::links_container_element_type LinkType;
				typedef Link_Components::Interfaces::Link_Interface<LinkType,ThisType> Link_Interface;

				//LinksContainerType& network_links=network->links_container<LinksContainerType&>();

				typedef typename ThisType::activity_locations_container_type ActivityLocationsContainerType;
				typedef typename ThisType::activity_locations_container_element_type ActivityLocationType;
				typedef Activity_Location_Components::Interfaces::Activity_Location_Interface<ActivityLocationType,ThisType> Activity_Location_Interface;

				typedef typename ThisType::scenario_type ScenarioType;
				typedef Scenario_Components::Interfaces::Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();

				typedef typename ThisType::traveler_type TravelerType;
				typedef Traveler_Components::Interfaces::Traveler_Interface<TravelerType,ThisType> Traveler_Interface;
				
				typedef typename ThisType::vehicle_type VehicleType;
				typedef Vehicle_Components::Interfaces::Vehicle_Interface<VehicleType,ThisType> Vehicle_Interface;
				
				typedef typename ThisType::routing_type RoutingType;
				typedef Routing_Components::Interfaces::Routing_Interface<RoutingType,ThisType> Routing_Interface;



				for(int simulation_interval_index=0;simulation_interval_index<scenario->num_simulation_intervals<int>();simulation_interval_index++)
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
					Activity_Location_Interface* freeway_origin_activity_location = (Activity_Location_Interface*)(network->activity_locations_container<ActivityLocationsContainerType&>()[freeway_origin_activity_location_index]);
					Link_Interface* freeway_origin_link = (Link_Interface*)freeway_origin_activity_location->origin_links<LinksContainerType&>()[0];
					int num_lanes_freeway = freeway_origin_link->num_lanes<int>();

					int ramp_origin_activity_location_index = 2;
					Activity_Location_Interface* ramp_origin_activity_location = (Activity_Location_Interface*)(network->activity_locations_container<ActivityLocationsContainerType&>()[ramp_origin_activity_location_index]);
					Link_Interface* ramp_origin_link = (Link_Interface*)ramp_origin_activity_location->origin_links<LinksContainerType&>()[0];
					int num_lanes_ramp = ramp_origin_link->num_lanes<int>();
					
					float num_generated_vehicles_freeway = (float) ((1.0+num_lanes_freeway*scenario->simulation_interval_length<int>() * freeway_demand_rate)/3600.0);
					float num_generated_vehicles_ramp = (float) ((1.0+num_lanes_ramp*scenario->simulation_interval_length<int>() * ramp_demand_rate)/3600.0);

					int destination_activity_location_index = 1;
					Activity_Location_Interface* destination_activity_location = (Activity_Location_Interface*)(network->activity_locations_container<ActivityLocationsContainerType&>()[destination_activity_location_index]);
					Link_Interface* destination_link = (Link_Interface*)(destination_activity_location->destination_links<LinksContainerType&>()[0]);
					
					Traveler_Interface* traveler;
					Vehicle_Interface* vehicle;
					Routing_Interface* router;

					float vehicle_rate_per_simulation_interval;
					int departure_time = -1;
					Link_Interface* origin_link;
					Activity_Location_Interface* origin_activity_location;
					int assignment_interval_index;
					assignment_interval_index = (int)((simulation_interval_index+1) * scenario->simulation_interval_length<int>()) / scenario->assignment_interval_length<int>();					
					
					vehicle_rate_per_simulation_interval = (float)(num_generated_vehicles_freeway / scenario->simulation_interval_length<float>());
					origin_link = freeway_origin_link;
					origin_activity_location = (Activity_Location_Interface*)network->activity_locations_container<ActivityLocationsContainerType&>()[freeway_origin_activity_location_index];


					for (int i=0;i<scenario_reference<Scenario_Interface*>()->simulation_interval_length<int>();i++)
					{
						float cur_vehicle_rate_per_simulation_interval = vehicle_rate_per_simulation_interval;
						while(cur_vehicle_rate_per_simulation_interval>0)
						{
							if (cur_vehicle_rate_per_simulation_interval>=1.0)
							{
								
								traveler=(Traveler_Interface*)Allocate<TravelerType>();
								vehicle=(Vehicle_Interface*)Allocate<VehicleType>();
								router=(Routing_Interface*)Allocate<RoutingType>();

								traveler->router<Routing_Interface*>(router);
								traveler->vehicle<Vehicle_Interface*>(vehicle);
								router->vehicle<Vehicle_Interface*>(vehicle);
								router->network<Network_Interface*>(network);

								vehicle->origin_link<Link_Interface*>(origin_link);
								vehicle->destination_link<Link_Interface*>(destination_link);
								vehicle->origin_activity_location<Activity_Location_Interface*>(origin_activity_location);
								vehicle->destination_activity_location<Activity_Location_Interface*>(destination_activity_location);
								//vehicle_data.set_seed(scenario_data.iseed);
								vehicle->departure_simulation_interval_index<int>(assignment_interval_index);
								vehicle->departure_assignment_interval_index<int>(simulation_interval_index);

								departure_time = simulation_interval_index * scenario->simulation_interval_length<int>()  + i;
								traveler->Schedule_New_Departure<NULLTYPE>(departure_time);

								cur_vehicle_rate_per_simulation_interval = (float) (cur_vehicle_rate_per_simulation_interval - 1.0);
							}
							else
							{//monte carlo
								double r1;
								//r1 = (double) g1.RandU01();
								r1 = 5.8;
								if (r1<=cur_vehicle_rate_per_simulation_interval)
								{

									traveler=(Traveler_Interface*)Allocate<TravelerType>();
									vehicle=(Vehicle_Interface*)Allocate<VehicleType>();
									router=(Routing_Interface*)Allocate<RoutingType>();

									traveler->router<Routing_Interface*>(router);
									traveler->vehicle<Vehicle_Interface*>(vehicle);
									router->vehicle<Vehicle_Interface*>(vehicle);
									router->network<Network_Interface*>(network);

									vehicle->origin_link<Link_Interface*>(origin_link);
									vehicle->destination_link<Link_Interface*>(destination_link);
									vehicle->origin_activity_location<Activity_Location_Interface*>(origin_activity_location);
									vehicle->destination_activity_location<Activity_Location_Interface*>(destination_activity_location);
									//vehicle_data.set_seed(scenario_data.iseed);
									vehicle->departure_simulation_interval_index<int>(assignment_interval_index);
									vehicle->departure_assignment_interval_index<int>(simulation_interval_index);

									departure_time = simulation_interval_index * scenario->simulation_interval_length<int>()  + i;
									traveler->Schedule_New_Departure<NULLTYPE>(departure_time);
								}

								cur_vehicle_rate_per_simulation_interval = (float)(cur_vehicle_rate_per_simulation_interval - 1.0);
							}
						}
					}




					vehicle_rate_per_simulation_interval = (float)(num_generated_vehicles_ramp / scenario->simulation_interval_length<float>());
					origin_link = ramp_origin_link;
					origin_activity_location = (Activity_Location_Interface*)network->activity_locations_container<ActivityLocationsContainerType&>()[ramp_origin_activity_location_index];


					for (int i=0;i<scenario_reference<Scenario_Interface*>()->simulation_interval_length<int>();i++)
					{
						float cur_vehicle_rate_per_simulation_interval = vehicle_rate_per_simulation_interval;
						while(cur_vehicle_rate_per_simulation_interval>0)
						{
							if (cur_vehicle_rate_per_simulation_interval>=1.0)
							{
								traveler=(Traveler_Interface*)Allocate<TravelerType>();
								vehicle=(Vehicle_Interface*)Allocate<VehicleType>();
								router=(Routing_Interface*)Allocate<RoutingType>();

								traveler->router<Routing_Interface*>(router);
								traveler->vehicle<Vehicle_Interface*>(vehicle);
								router->vehicle<Vehicle_Interface*>(vehicle);
								router->network<Network_Interface*>(network);

								vehicle->origin_link<Link_Interface*>(origin_link);
								vehicle->destination_link<Link_Interface*>(destination_link);
								vehicle->origin_activity_location<Activity_Location_Interface*>(origin_activity_location);
								vehicle->destination_activity_location<Activity_Location_Interface*>(destination_activity_location);
								//vehicle_data.set_seed(scenario_data.iseed);
								vehicle->departure_simulation_interval_index<int>(assignment_interval_index);
								vehicle->departure_assignment_interval_index<int>(simulation_interval_index);

								departure_time = simulation_interval_index * scenario->simulation_interval_length<int>()  + i;
								traveler->Schedule_New_Departure<NULLTYPE>(departure_time);

								cur_vehicle_rate_per_simulation_interval = (float) (cur_vehicle_rate_per_simulation_interval - 1.0);
							}
							else
							{//monte carlo
								double r1;
								//r1 = (double) g1.RandU01();
								r1 = 0.23;
								if (r1<=cur_vehicle_rate_per_simulation_interval)
								{
									traveler=(Traveler_Interface*)Allocate<TravelerType>();
									vehicle=(Vehicle_Interface*)Allocate<VehicleType>();
									router=(Routing_Interface*)Allocate<RoutingType>();

									traveler->router<Routing_Interface*>(router);
									traveler->vehicle<Vehicle_Interface*>(vehicle);
									router->vehicle<Vehicle_Interface*>(vehicle);
									router->network<Network_Interface*>(network);

									vehicle->origin_link<Link_Interface*>(origin_link);
									vehicle->destination_link<Link_Interface*>(destination_link);
									vehicle->origin_activity_location<Activity_Location_Interface*>(origin_activity_location);
									vehicle->destination_activity_location<Activity_Location_Interface*>(destination_activity_location);
									//vehicle_data.set_seed(scenario_data.iseed);
									vehicle->departure_simulation_interval_index<int>(assignment_interval_index);
									vehicle->departure_assignment_interval_index<int>(simulation_interval_index);

									departure_time = simulation_interval_index * scenario->simulation_interval_length<int>()  + i;
									traveler->Schedule_New_Departure<NULLTYPE>(departure_time);
								}

								cur_vehicle_rate_per_simulation_interval = (float)(cur_vehicle_rate_per_simulation_interval - 1.0);
							}
						}
					}
				}
				
				//done with function
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
			template<typename ThisType, typename CallerType, typename TripType, typename InputFileType, typename TimeStructType>
			void Initialize(InputFileType& input_file_struct, TimeStructType& iteration_step_struct, call_requires(ThisType, Is_Dispatchable))
			{
				cout <<endl<<"GETTING DEMAND FROM TRANSIMS..."<<endl;
				PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TripType,InputFileType,TimeStructType>(input_file_struct,iteration_step_struct);
				cout <<endl<<"DONE."<<endl<<endl;
				schedule_event_local(ThisType,Get_Trips_Conditional,Get_Trips_Event,0,TripType);
			}


			//==================================================================================================================
			// Interface Event functionality and conditional
			// Notice the event functionality and conditional are entirely contained in the interface and depend on no other vars
			//------------------------------------------------------------------------------------------------------------------
			/// Defines the Get_Trips_Event which is the primary event of the external Demand API
			declare_facet_event(Get_Trips_Event)
			{
				char print;
				cout <<endl<<"Getting Trips"<<endl;
				Demand_Interface* demand = pthis;
				demand->Get_Current_Trips_From_External<ThisType,CallerType,TargetType>();
				demand->Display_Timestep<ThisType,CallerType,TargetType>();
				demand->Increment_Time<ThisType,CallerType,TargetType>();

				cout <<endl<<"Print trips to screen ('y' or 'n'): ";
				cin >> print;

				if (print == 'y' || print == 'Y') demand->Print_Trips<ThisType,CallerType,TargetType>();

				demand->Clear_Trips<ThisType,CallerType,TargetType>();

			}
			/// Conditional function used to trigger the event facet - currently check every iteration
			declare_facet_conditional(Get_Trips_Conditional)
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
			facet void Get_Current_Trips_From_External(call_requirements(requires(ThisType, Is_Dispatchable) && requires(ThisType, Concepts::Is_Transims)))
			{
				Time_Components::Interfaces::Time_Interface* tstart = timestep_start<ThisType,CallerType,Time_Components::Components::Time>();
				Time_Components::Interfaces::Time_Interface* tend = timestep_end<ThisType,CallerType,Time_Components::Components::Time>();
				
				tstart->Write<Time_Components::Components::Time,CallerType,int>();
				auto s = tstart->Convert_Time<Time_Components::Components::Time, CallerType, Time_Components::Data_Structures::Time_DRSeconds>();
				auto e = tend->Convert_Time<Time_Components::Components::Time, CallerType, Time_Components::Data_Structures::Time_DRSeconds>();

				Time start = Time((int)s.Time);
				Time end = Time((int)e.Time);

				vector<Trip_Info*> v;
				vector<Trip_Info*>::iterator itr;

				Get_Trips(start,end, v);

				int trip_count = 1;
				
				for (itr=v.begin(); itr != v.end(); itr++)
				{
					this->push_trip<ThisType,ThisType,Trip_Components::Components::Trip>(*itr);
					trip_count++;
				}
				cout <<endl<<endl<<"TIMESTEP "<<iteration<<endl<<"Trip count = "<<this->Trip_Count<ThisType,CallerType,int>()<<endl;
			}
			/// Time incrementing facet
			facet void Increment_Time()
			{
				auto start = timestep_start<ThisType,CallerType,Time_Components::Components::Time>();
				auto end = timestep_end<ThisType,CallerType,Time_Components::Components::Time>();
				auto tstep = this->t_step<ThisType,CallerType,Time_Components::Data_Structures::Time_Seconds&>();

				start->Add_Time<Time_Components::Components::Time,CallerType>(tstep);
				end->Add_Time<Time_Components::Components::Time,CallerType>(tstep);
			}
			/// Display the current timestep information
			facet void Display_Timestep()
			{
				cout<<"Timestep Start:  ";
				timestep_start<ThisType,ThisType,Time_Components::Components::Time>()->Write<Time_Components::Components::Time,CallerType,int>();
				cout<<endl<<"Timestep End:  ";
				timestep_end<ThisType,ThisType,Time_Components::Components::Time>()->Write<Time_Components::Components::Time,CallerType,int>();			
			}
			//------------------------------------------------------------------------------------------------------------------
			//  Base function dispatchers
			//------------------------------------------------------------------------------------------------------------------
			/// Push a trip returned from external source into the local trip list through a base function call
			facet void push_trip(Trip_Info* trip, call_requirements(requires(ThisType,Is_Dispatchable) && requires(TargetType, Trip_Components::Concepts::Is_Trip)))
			{
				PTHIS(ThisType)->push_trip<Dispatch<ThisType>,CallerType,TargetType>(trip);
			}
			/// Print the current trip list usign base function call
			facet void Print_Trips(call_requires(ThisType, Is_Dispatchable))
			{
				PTHIS(ThisType)->Print_Trips<Dispatch<ThisType>,CallerType,TargetType>();
			}
			/// Get a count of the number of trips returned in current timestep using base function call
			facet TargetType Trip_Count(call_requires(ThisType,Is_Dispatchable))
			{
				return PTHIS(ThisType)->Trip_Count<Dispatch<ThisType>,CallerType,TargetType>();
			}
			/// Clear the current trip list using base function call
			facet void Clear_Trips(call_requires(ThisType, Is_Dispatchable))
			{
				PTHIS(ThisType)->Clear_Trips<Dispatch<ThisType>,CallerType,TargetType>();
			}


			//==================================================================================================================
			// Accessor facets - notice no types are associated with accessors in the interface
			//------------------------------------------------------------------------------------------------------------------
			/// filename accessor
			facet_accessor(filename);
			/// Get the timestep length
			facet_accessor(timestep_length);
			/// Get a time structure representing the current timestep
			facet_accessor(t_step);
			/// Get the trips in the current timestep
			facet_accessor(current_trips);
			/// Get some type of interface to the timestep start time
			facet_accessor_interface(timestep_start);
			/// Get some type of interface to the timestep end time
			facet_accessor_interface(timestep_end);

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