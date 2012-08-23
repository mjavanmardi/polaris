#pragma once

#include "Repository_User_Includes.h"
#include "Transims_Demand_API.h"

namespace Demand_API
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
	///
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
				Time_API::Interfaces::Time_Interface* tstart = timestep_start<ThisType,CallerType,Time_API::Time>();
				Time_API::Interfaces::Time_Interface* tend = timestep_end<ThisType,CallerType,Time_API::Time>();
				
				tstart->Write<Time_API::Time,CallerType,int>();
				auto s = tstart->Convert_Time<Time_API::Time, CallerType, Time_API::Data_Structures::Time_DRSeconds>();
				auto e = tend->Convert_Time<Time_API::Time, CallerType, Time_API::Data_Structures::Time_DRSeconds>();

				Time start = Time((int)s.Time);
				Time end = Time((int)e.Time);

				vector<Trip_Info*> v;
				vector<Trip_Info*>::iterator itr;

				Get_Trips(start,end, v);

				int trip_count = 1;
				
				for (itr=v.begin(); itr != v.end(); itr++)
				{
					this->push_trip<ThisType,ThisType,Trip_API::Trip>(*itr);
					trip_count++;
				}
				cout <<endl<<endl<<"TIMESTEP "<<iteration<<endl<<"Trip count = "<<this->Trip_Count<ThisType,CallerType,int>()<<endl;
			}
			/// Time incrementing facet
			facet void Increment_Time()
			{
				auto start = timestep_start<ThisType,CallerType,Time_API::Time>();
				auto end = timestep_end<ThisType,CallerType,Time_API::Time>();
				auto tstep = this->t_step<ThisType,CallerType,Time_API::Data_Structures::Time_Seconds&>();

				start->Add_Time<Time_API::Time,CallerType>(tstep);
				end->Add_Time<Time_API::Time,CallerType>(tstep);
			}
			/// Display the current timestep information
			facet void Display_Timestep()
			{
				cout<<"Timestep Start:  ";
				timestep_start<ThisType,ThisType,Time_API::Time>()->Write<Time_API::Time,CallerType,int>();
				cout<<endl<<"Timestep End:  ";
				timestep_end<ThisType,ThisType,Time_API::Time>()->Write<Time_API::Time,CallerType,int>();			
			}
			//------------------------------------------------------------------------------------------------------------------
			//  Base function dispatchers
			//------------------------------------------------------------------------------------------------------------------
			/// Push a trip returned from external source into the local trip list through a base function call
			facet void push_trip(Trip_Info* trip, call_requirements(requires(ThisType,Is_Dispatchable) && requires(TargetType, Trip_API::Concepts::Is_Trip)))
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
			typedef Demand_API::Types::ModelTypes::TransimsType TransimsType;
			typedef Demand_API::Types::FileFormatTypes::CSV_Delimited CSV_Delimited;
			typedef Demand_API::Types::FileTypes::ControlFile ControlFile;
			char* filename;
		};
		struct Transims_Control_File_TAB_Struct
		{
			typedef Demand_API::Types::ModelTypes::TransimsType TransimsType;
			typedef Demand_API::Types::FileFormatTypes::Tab_Delimited Tab_Delimited;
			typedef Demand_API::Types::FileTypes::ControlFile ControlFile;
			char* filename;
		};
		struct Transims_Trip_File_CSV_Struct
		{
			typedef Demand_API::Types::ModelTypes::TransimsType TransimsType;
			typedef Demand_API::Types::FileFormatTypes::CSV_Delimited CSV_Delimited;
			typedef Demand_API::Types::FileTypes::TripFile TripFile;
			char* filename;
		};
		struct Transims_Trip_File_TAB_Struct
		{
			typedef Demand_API::Types::ModelTypes::TransimsType TransimsType;
			typedef Demand_API::Types::FileFormatTypes::Tab_Delimited Tab_Delimited;
			typedef Demand_API::Types::FileTypes::TripFile TripFile;
			char* filename;
		};
		struct Transims_Trip_File_BIN_Struct
		{
			typedef Demand_API::Types::ModelTypes::TransimsType TransimsType;
			typedef Demand_API::Types::FileFormatTypes::Binary Binary;
			typedef Demand_API::Types::FileTypes::TripFile TripFile;
			char* filename;
		};
	}

}