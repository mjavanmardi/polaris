#pragma once

#include "Demand_Interface.h"


namespace Demand_Components
{
	namespace Bases
	{
		struct Demand_Base
		{
			//==================================================================================================================
			/// Demand_Base Typedef.
			/// Defines a component created using this base as a Transims dependent Demand generator.
			//------------------------------------------------------------------------------------------------------------------
			typedef true_type TransimsType;

			//==================================================================================================================
			// Demand initializers
			//------------------------------------------------------------------------------------------------------------------
			/// Demand initializer for ConvertTrips version of Transims
			/// Creates a new trip file using the ConvertTrips executable.
			facet_base void Initialize(typename TargetType::ParamType input_file_struct, typename TargetType::Param2Type iteration_step_struct, call_requirements(
				requires(ThisType, Is_Dispatched) &&
				requires(typename TargetType::Param2Type,Time_Components::Concepts::Has_Time) &&
				requires(typename TargetType::ParamType,Concepts::Is_Transims_Control_File)))
			{
				/// Setup up input data file information
				_filename = input_file_struct.filename;
				set_file_format<InputFileType>();
				Initialize_Demand_Generator(_filename,true,_file_format);

				/// Get the length of the iteration step in seconds from whatever TimeType time is passed in
				Time_Components::Interfaces::Time_Interface* t = Allocate<Time_Components::Components::Time>();
				t->Initialize<Time_Components::Components::Time,NULLTYPE,TimeStructType>(iteration_step_struct);

				//Time_Components::Interfaces::Time_Interface* t = (Time_Components::Interfaces::Time_Interface*) iteration_step_struct;
				_timestep_length = t->Total_Seconds<Time_Components::Components::Time,CallerType,int>();

				/// Intialize the timestep start and end TimeType variables using a Time_Struct structure
				_t_step.Time = 0;
				_timestep_start = Allocate<Time_Components::Components::Time>();
				_timestep_end = Allocate<Time_Components::Components::Time>();

				/// Set the timestep start at 0;
				_timestep_start->Initialize<Time_Components::Components::Time,CallerType>(_t_step);

				// Set the timestep end at iteration_size
				_t_step.Time += _timestep_length;
				_timestep_end->Initialize<Time_Components::Components::Time,CallerType>(_t_step);


			}
			//------------------------------------------------------------------------------------------------------------------
			/// Demand initializer for ReadTrips version of Transims.
			/// Reads from a previously generated trip file.
			facet_base void Initialize(typename TargetType::ParamType input_file_struct, typename TargetType::Param2Type iteration_step_struct, call_requirements(
				requires(ThisType,Is_Dispatched) &&
				requires(typename TargetType::Param2Type,Time_Components::Concepts::Has_Time) &&
				requires(typename TargetType::ParamType,Concepts::Is_Transims_Trip_File)))
			{
				/// Setup up input data file information
				_filename = input_file_struct;
				set_file_format<typename TargetType::ParamType>();

				/// Run the Demand_Generator in the linked Demand_Generator.lib library	
				Initialize_Demand_Generator(_filename,false,_file_format);

				/// Get the length of the iteration step in seconds from whatever TimeType time is passed in
				Time_Components::Interfaces::Time_Interface<Time_Components::Components::Time,NULLTYPE>* t = (Time_Components::Interfaces::Time_Interface<Time_Components::Components::Time,NULLTYPE>*)Allocate<Time_Components::Components::Time>();
				t->Initialize<>(iteration_step_struct);

				//Time_Components::Interfaces::Time_Interface* t = (Time_Components::Interfaces::Time_Interface*) iteration_step_struct;
				_timestep_length = (int)t->Total_Time<Time_Components::Data_Structures::Time_Seconds>();

				/// Intialize the timestep start and end TimeType variables using a Time_Struct structure
				_t_step = 0;
				_timestep_start = (Time_Components::Interfaces::Time_Interface<Time_Components::Components::Time,NULLTYPE>*)Allocate<Time_Components::Components::Time>();
				_timestep_end = (Time_Components::Interfaces::Time_Interface<Time_Components::Components::Time,NULLTYPE>*)Allocate<Time_Components::Components::Time>();

				/// Set the timestep start at 0;
				_timestep_start->Initialize<>(_t_step);

				// Set the timestep end at iteration_size
				_t_step = _t_step + _timestep_length;
				_timestep_end->Initialize<>(_t_step);
			}
			//------------------------------------------------------------------------------------------------------------------
			/// Demand initializers error handler
			///
			facet_base void Initialize(typename TargetType::ParamType input_file_struct, typename TargetType::Param2Type iteration_step_struct, call_requirements(
				requires(ThisType,!Is_Dispatched) ||
				requires(typename TargetType::Param2Type,!Time_Components::Concepts::Has_Time) || (
				requires(typename TargetType::ParamType,!Concepts::Is_Transims_Control_File) &&
				requires(typename TargetType::ParamType,!Concepts::Is_Transims_Trip_File))))
			{
				assert_requirements(ThisType, Is_Dispatched, "Your type has not been dispatched - and - ");
				assert_requirements(typename TargetType::Param2Type,Time_Components::Concepts::Has_Time," - Your Target Time type does not have a TimeType specifier.");
				assert_requirements(typename TargetType::ParamType,Concepts::Is_Transims_Control_File," - Your InputFileType is not a ControlFile - and - ");
				assert_requirements(typename TargetType::ParamType,Concepts::Is_Transims_Trip_File," - Your InputFileType is not a TripFile - END");
			}



			//==================================================================================================================
			// Function Handlers for other interface requests
			//------------------------------------------------------------------------------------------------------------------
			/// Push a new trip into the local trip array holder
			facet_base void push_trip(Trip_Info* trip, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, Trip_Components::Concepts::Is_Trip) /*&& requires(ThisType,Is_Same_Component)*/))
			{
				Trip_Components::Interfaces::Trip_Interface<TargetType,NULLTYPE>* new_trip;
				new_trip = (Trip_Components::Interfaces::Trip_Interface<TargetType,NULLTYPE>*)Allocate<TargetType>();

				Time_Components::Data_Structures::Time_Seconds t;
				t = (Time_Components::Data_Structures::Time_Seconds::ValueType)trip->Departure_Time.Total_Seconds();

				Location_Components::Data_Structures::Location_Struct o, d;
				o = trip->Start_Location;
				d = trip->End_Location;

				new_trip->Initialize<Target_Type<NULLTYPE,int,Time_Components::Data_Structures::Time_Seconds,Location_Components::Data_Structures::Location_Struct>>(trip->hh_id * 100 + trip->vehicle_id,t,t,o,d);
				_current_trips.push_back(new_trip);

			}
			/// Error handler for push_trip functionality
			facet_base void push_trip(Trip_Info* trip, call_requirements(requires(ThisType, !Is_Dispatched) || requires(TargetType, !Trip_Components::Concepts::Is_Trip) ))
			{
				assert_requirements(ThisType, Is_Dispatched, "ThisType is not dispatched");
				//assert_requirements(ThisType, Is_Same_Component, "ThisType is not from the same component as target");
				assert_requirements(TargetType, Trip_Components::Concepts::Is_Trip, "TargetType is not a valid trip type");
			}
			/// Clears the trips from the local trip array
			facet_base void Clear_Trips(call_requirements(requires(ThisType, Is_Dispatched) /*&& requires(ThisType,Is_Same_Component)*/))
			{
				for (trip_itr = _current_trips.begin(); trip_itr != _current_trips.end(); trip_itr++)
				{
					//Free_Singleton<Trip_Components::Trip>(*trip_itr);
				}
				_current_trips.clear();
			}
			/// Prints trips from the local trip array
			facet_base void Print_Trips(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, Trip_Components::Concepts::Is_Trip)))
			{
				for (trip_itr = _current_trips.begin(); trip_itr != _current_trips.end(); trip_itr++)
				{
					((TargetType::Interface_Type<TargetType,NULLTYPE>::type*)(*trip_itr))->Write<int>();
				}
			}
			/// Error handler for print trips, displayed if the requested output type (TargetType) does not satisfy the Is_Trip concept.
			facet_base void Print_Trips(call_requirements(requires(ThisType, !Is_Dispatched) || requires(TargetType, !Trip_Components::Concepts::Is_Trip)))
			{
				assert_requirements(ThisType, Is_Dispatched, "ThisType is not dispatched");
				//assert_requirements(ThisType, Is_Same_Component, "ThisType is not from the same component as target");
				assert_requirements(TargetType, Trip_Components::Concepts::Is_Trip, "TargetType is not a valid trip type");
			}
			/// Returns the current size of the trip array for the timestep.
			facet_base TargetType Trip_Count(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, is_arithmetic)))
			{
				return (TargetType)_current_trips.size();
			}



			//==================================================================================================================
			// Base data members and accessors
			//------------------------------------------------------------------------------------------------------------------
			/// Timestep length data member.
			/// Also creates getter and setter functions for the timestep length.  Requires that the input or return type is an arithmetic type.
			member_data(int, timestep_length, requires(TargetType,is_arithmetic),requires(TargetType,is_arithmetic));	
			//------------------------------------------------------------------------------------------------------------------
			/// Filename data member, holds the name of the trip or control file to open.  
			/// Also creates the filename getter and setter.  Setter is required to be invoked by "Demand" component only.
			member_data(char*,filename,true,requires(ThisType,Is_Same_Component));
			//------------------------------------------------------------------------------------------------------------------
			/// t_step data member, holds a time struct which defines the timestep length 
			/// Also creates the t_step getter and setter.  The set or return values are required to be a valid Time_Struct.
			member_data(Time_Components::Data_Structures::Time_Seconds, t_step, requires(TargetType, Time_Components::Concepts::Has_Time),false);	
			//------------------------------------------------------------------------------------------------------------------
			/// Interface to time component which defines the current start time of the timestep.
			member_component(Time_Components::Components::Time, timestep_start, requires(TargetType, Time_Components::Concepts::Is_Time), requires(TargetType, Time_Components::Concepts::Is_Time))
			//------------------------------------------------------------------------------------------------------------------
			/// Interface to time component which defines the current end time of the timestep.
			member_component(Time_Components::Components::Time, timestep_end, requires(TargetType, Time_Components::Concepts::Is_Time), requires(TargetType, Time_Components::Concepts::Is_Time))
					

			//==================================================================================================================
			// Local variables and functions, not used outside of base
			//------------------------------------------------------------------------------------------------------------------
			/// File format
			string _file_format;
			vector<Trip_Components::Interfaces::Trip_Interface<Trip_Components::Components::Trip, NULLTYPE>*> _current_trips;
			vector<Trip_Components::Interfaces::Trip_Interface<Trip_Components::Components::Trip, NULLTYPE>*>::iterator trip_itr;
			//------------------------------------------------------------------------------------------------------------------
			// Functions used to extract file_format tag from the input_file_struct used to initialize the component
			/// CSV delimited set_file_format overload
			template<typename TargetType> void set_file_format(call_requires(TargetType,Concepts::Is_CSV_Delimited)) {_file_format="CSV_DELIMITED";}
			/// TAB delimited set_file_format overload
			template<typename TargetType> void set_file_format(call_requires(TargetType,Concepts::Is_Tab_Delimited)) {_file_format="TAB_DELIMITED";}
			/// Error-handler if a non-CSV and non-TAB delimited file type is specified.
			template<typename TargetType> void set_file_format(call_requirements(requires(TargetType,!Concepts::Is_CSV_Delimited) && requires(TargetType,!Concepts::Is_Tab_Delimited)))
			{
				assert_requirements(TargetType,Is_Csv_Delimited,"- Unsupported file_format specified - ");
			}	
		};

	}

	namespace Components
	{
		//==================================================================================================================
		/// Demand_Executor component definition
		/// Created by combining the demand interface with the Demand_Base
		//------------------------------------------------------------------------------------------------------------------
		typedef Polaris_Component_Execution<Interfaces::Demand_Interface, Bases::Demand_Base> Demand_Executor;
	}
}