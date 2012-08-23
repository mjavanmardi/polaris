#pragma once

#include "Demand_API.h"


namespace Demand_API
{
	class Demand_Base
	{
	public:
		//==================================================================================================================
		/// Demand_Base Typedef.
		/// Defines a component created using this base as a Transims dependent Demand generator.
		//------------------------------------------------------------------------------------------------------------------
		typedef true_type TransimsType;


	protected:
		//==================================================================================================================
		// Demand initializers
		//------------------------------------------------------------------------------------------------------------------
		/// Demand initializer for ConvertTrips version of Transims
		/// Creates a new trip file using the ConvertTrips executable.
		template<typename ThisType, typename CallerType, typename TripType, typename InputFileType, typename TimeStructType>
		void Initialize(InputFileType& input_file_struct, TimeStructType& iteration_step_struct, call_requirements(
			requires(ThisType, Is_Dispatched) &&
			requires(TripType,Trip_API::Concepts::Is_Trip) &&
			requires(InputFileType,Concepts::Is_Transims_Control_File)))
		{
			/// Setup up input data file information
			_filename = input_file_struct.filename;
			set_file_format<InputFileType>();
			Initialize_Demand_Generator(_filename,true,_file_format);

			/// Get the length of the iteration step in seconds from whatever TimeType time is passed in
			Time_API::Interfaces::Time_Interface* t = Allocate<Time_API::Time>();
			t->Initialize<Time_API::Time,NULLTYPE,TimeStructType>(iteration_step_struct);

			//Time_API::Interfaces::Time_Interface* t = (Time_API::Interfaces::Time_Interface*) iteration_step_struct;
			_timestep_length = t->Total_Seconds<Time_API::Time,CallerType,int>();

			/// Intialize the timestep start and end TimeType variables using a Time_Struct structure
			_t_step.Time = 0;
			_timestep_start = Allocate<Time_API::Time>();
			_timestep_end = Allocate<Time_API::Time>();

			/// Set the timestep start at 0;
			_timestep_start->Initialize<Time_API::Time,CallerType>(_t_step);

			// Set the timestep end at iteration_size
			_t_step.Time += _timestep_length;
			_timestep_end->Initialize<Time_API::Time,CallerType>(_t_step);


		}
		//------------------------------------------------------------------------------------------------------------------
		/// Demand initializer for ReadTrips version of Transims.
		/// Reads from a previously generated trip file.
		template<typename ThisType, typename CallerType, typename TripType, typename InputFileType, typename TimeStructType>
		void Initialize(InputFileType& input_file_struct, TimeStructType& iteration_step_struct, call_requirements(
			requires(ThisType,Is_Dispatched) &&
			requires(TripType,Trip_API::Concepts::Is_Trip) &&
			requires(InputFileType,Concepts::Is_Transims_Trip_File)))
		{
			/// Setup up input data file information
			_filename = input_file_struct.filename;
			set_file_format<InputFileType>();

			/// Run the Demand_Generator in the linked Demand_Generator.lib library	
			Initialize_Demand_Generator(_filename,false,_file_format);

			/// Get the length of the iteration step in seconds from whatever TimeType time is passed in
			Time_API::Interfaces::Time_Interface* t = Allocate<Time_API::Time>();
			t->Initialize<Time_API::Time,NULLTYPE,TimeStructType>(iteration_step_struct);

			//Time_API::Interfaces::Time_Interface* t = (Time_API::Interfaces::Time_Interface*) iteration_step_struct;
			_timestep_length = t->Total_Seconds<Time_API::Time,CallerType,int>();

			/// Intialize the timestep start and end TimeType variables using a Time_Struct structure
			_t_step.Time = 0;
			_timestep_start = Allocate<Time_API::Time>();
			_timestep_end = Allocate<Time_API::Time>();

			/// Set the timestep start at 0;
			_timestep_start->Initialize<Time_API::Time,CallerType>(_t_step);

			// Set the timestep end at iteration_size
			_t_step.Time += _timestep_length;
			_timestep_end->Initialize<Time_API::Time,CallerType>(_t_step);
		}
		//------------------------------------------------------------------------------------------------------------------
		/// Demand initializers error handler
		///
		template<typename ThisType, typename CallerType, typename TripType, typename InputFileType, typename TimeStructType>
		void Initialize(InputFileType& input_file_struct, TimeStructType& iteration_step_struct, call_requirements(
			requires(ThisType,!Is_Dispatched) ||
			requires(TripType,!Trip_API::Concepts::Is_Trip) || (
			requires(InputFileType,!Concepts::Is_Transims_Control_File) &&
			requires(InputFileType,!Concepts::Is_Transims_Trip_File))))
		{
			assert_requirements(ThisType, Is_Dispatched, "Your type has not been dispatched - and - ");
			assert_requirements(TripType, Trip_API::Concepts::Is_Trip, "Your TripType type does not satisfy the requirements of a Trip - ");
			assert_requirements(InputFileType,Concepts::Is_Transims_Control_File," - Your InputFileType is not a ControlFile - and - ");
			assert_requirements(InputFileType,Concepts::Is_Transims_Trip_File," - Your InputFileType is not a TripFile - END");
		}



		//==================================================================================================================
		// Function Handlers for other interface requests
		//------------------------------------------------------------------------------------------------------------------
		/// Push a new trip into the local trip array holder
		facet void push_trip(Trip_Info* trip, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, Trip_API::Concepts::Is_Trip) /*&& requires(ThisType,Is_Same_Component)*/))
		{
			Trip_API::Interfaces::Trip_Interface* new_trip = Allocate<Trip_API::Trip>();

			Time_API::Data_Structures::Time_Seconds t;
			t.Time = (Time_API::Data_Structures::Time_Seconds::ValueType)trip->Departure_Time.Total_Seconds();

			Location_API::Data_Structures::Location_Struct o, d;
			o.location = trip->Start_Location;
			d.location = trip->End_Location;

			new_trip->Initialize<Trip_API::Trip,ThisType,int,Time_API::Data_Structures::Time_Seconds,Location_API::Data_Structures::Location_Struct>(trip->hh_id * 100 + trip->vehicle_id,t,t,o,d);
			_current_trips.push_back(new_trip);

		}
		/// Error handler for push_trip functionality
		facet void push_trip(Trip_Info* trip, call_requirements(requires(ThisType, !Is_Dispatched) || requires(TargetType, !Trip_API::Concepts::Is_Trip) ))
		{
			assert_requirements(ThisType, Is_Dispatched, "ThisType is not dispatched");
			//assert_requirements(ThisType, Is_Same_Component, "ThisType is not from the same component as target");
			assert_requirements(TargetType, Trip_API::Concepts::Is_Trip, "TargetType is not a valid trip type");
		}
		/// Clears the trips from the local trip array
		facet void Clear_Trips(call_requirements(requires(ThisType, Is_Dispatched) /*&& requires(ThisType,Is_Same_Component)*/))
		{
			for (trip_itr = _current_trips.begin(); trip_itr != _current_trips.end(); trip_itr++)
			{
				//Free_Singleton<Trip_API::Trip>(*trip_itr);
			}
			_current_trips.clear();
		}
		/// Prints trips from the local trip array
		facet void Print_Trips(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, Trip_API::Concepts::Is_Trip)))
		{
			for (trip_itr = _current_trips.begin(); trip_itr != _current_trips.end(); trip_itr++)
			{
				((TargetType::Interface_Type*)(*trip_itr))->Write<TargetType,CallerType,int>();
			}
		}
		/// Error handler for print trips, displayed if the requested output type (TargetType) does not satisfy the Is_Trip concept.
		facet void Print_Trips(call_requirements(requires(ThisType, !Is_Dispatched) || requires(TargetType, !Trip_API::Concepts::Is_Trip)))
		{
			assert_requirements(ThisType, Is_Dispatched, "ThisType is not dispatched");
			//assert_requirements(ThisType, Is_Same_Component, "ThisType is not from the same component as target");
			assert_requirements(TargetType, Trip_API::Concepts::Is_Trip, "TargetType is not a valid trip type");
		}
		/// Returns the current size of the trip array for the timestep.
		facet TargetType Trip_Count(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, is_arithmetic)))
		{
			return (TargetType)_current_trips.size();
		}



		//==================================================================================================================
		// Base data members and accessors
		//------------------------------------------------------------------------------------------------------------------
		/// Timestep length data member.
		/// Also creates getter and setter functions for the timestep length.  Requires that the input or return type is an arithmetic type.
		member_data(int, timestep_length, requires(TargetType,is_arithmetic)&& requires(ThisType,Is_Same_Component),requires(TargetType,is_arithmetic)&& requires(ThisType,Is_Same_Component));	
		//------------------------------------------------------------------------------------------------------------------
		/// Filename data member, holds the name of the trip or control file to open.  
		/// Also creates the filename getter and setter.  Setter is required to be invoked by "Demand" component only.
		member_data(char*,filename,true,requires(ThisType,Is_Same_Component));
		//------------------------------------------------------------------------------------------------------------------
		/// t_step data member, holds a time struct which defines the timestep length 
		/// Also creates the t_step getter and setter.  The set or return values are required to be a valid Time_Struct.
		member_data(Time_API::Data_Structures::Time_Seconds, t_step, requires(TargetType, Time_API::Concepts::Has_Time),false);	
		//------------------------------------------------------------------------------------------------------------------
		/// Interface to time component which defines the current start time of the timestep.
		Time_API::Interfaces::Time_Interface* _timestep_start;
		tag_getter_setter(timestep_start);
		facet void timestep_start(typename TargetType::Interface_Type* set_value, call_requirements(requires(ThisType, Is_Dispatched) && requires(ThisType,Is_Same_Component) && requires(TargetType, Time_API::Concepts::Is_Time)))
		{
			_timestep_start = (Time_API::Interfaces::Time_Interface*)set_value;
		}
		facet void timestep_start(typename TargetType::Interface_Type*  set_value, call_requirements(requires(ThisType, !Is_Dispatched) || requires(ThisType,!Is_Same_Component) || requires(TargetType, !Time_API::Concepts::Is_Time)))
		{ 
			assert_requirements(ThisType, Is_Dispatched,"ThisType is not dispatched");
			assert_requirements(ThisType, Is_Same_Component, "ThisType does not have access, because it is not the same component.");
			assert_requirements(TargetType, Time_API::Concepts::Is_Time,"TargetType is not a valid TimeType");
		}
		facet typename TargetType::Interface_Type* timestep_start(call_requirements(requires(ThisType, Is_Dispatched)&& requires(TargetType, Time_API::Concepts::Is_Time)))
		{ 
			return (TargetType::Interface_Type* )_timestep_start;
		}		
		//------------------------------------------------------------------------------------------------------------------
		/// Interface to time component which defines the current end time of the timestep.
		Time_API::Interfaces::Time_Interface* _timestep_end;
		tag_getter_setter(timestep_end);
		facet void timestep_end(typename TargetType::Interface_Type* set_value, call_requirements(requires(ThisType, Is_Dispatched) && requires(ThisType,Is_Same_Component) && requires(TargetType, Time_API::Concepts::Is_Time)))
		{
			_timestep_end = (Time_API::Interfaces::Time_Interface*)set_value;
		}
		facet void timestep_end(typename TargetType::Interface_Type*  set_value, call_requirements(requires(ThisType, !Is_Dispatched) || requires(ThisType,!Is_Same_Component) || requires(TargetType, !Time_API::Concepts::Is_Time)))
		{ 
			assert_requirements(ThisType, Is_Dispatched,"ThisType is not dispatched");
			assert_requirements(ThisType, Is_Same_Component, "ThisType does not have access, because it is not the same component.");
			assert_requirements(TargetType, Time_API::Concepts::Is_Time,"TargetType is not a valid TimeType");
		}
		facet typename TargetType::Interface_Type* timestep_end(call_requirements(requires(ThisType, Is_Dispatched)&& requires(TargetType, Time_API::Concepts::Is_Time)))
		{ 
			return (TargetType::Interface_Type* )_timestep_end;
		}

		

		//==================================================================================================================
		// Local variables and functions, not used outside of base
		//------------------------------------------------------------------------------------------------------------------
		/// File format
		string _file_format;
		vector<Trip_API::Interfaces::Trip_Interface*> _current_trips;
		vector<Trip_API::Interfaces::Trip_Interface*>::iterator trip_itr;
		//------------------------------------------------------------------------------------------------------------------
		// Functions used to extract file_format tag from the input_file_struct used to initialize the component
		/// CSV delimited set_file_format overload
		template<typename FileFormatTag> void set_file_format(call_requires(FileFormatTag,Concepts::Is_CSV_Delimited)) {_file_format="CSV_DELIMITED";}
		/// TAB delimited set_file_format overload
		template<typename FileFormatTag> void set_file_format(call_requires(FileFormatTag,Concepts::Is_Tab_Delimited)) {_file_format="TAB_DELIMITED";}
		/// Error-handler if a non-CSV and non-TAB delimited file type is specified.
		template<typename FileFormatTag> void set_file_format(call_requirements(requires(FileFormatTag,!Concepts::Is_CSV_Delimited) && requires(FileFormatTag,!Concepts::Is_Tab_Delimited)))
		{
			assert_requirements(FileFormatTag,Is_Csv_Delimited,"- Unsupported file_format specified - ");
		}	
	};


	//==================================================================================================================
	/// Demand_Executor component definition
	/// Created by combining the demand interface with the Demand_Base
	//------------------------------------------------------------------------------------------------------------------
	typedef Polaris_Component_Execution<Interfaces::Demand_Interface,Demand_Base> Demand_Executor;
}