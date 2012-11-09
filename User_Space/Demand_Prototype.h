#pragma once

#include "User_Space_Includes.h"

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

			feature_prototype void read_demand_data()
			{
				float freeway_demand_rate = 1800.0;
				float ramp_demand_rate = 900.0;
				
				RngStream g1("Demand");
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