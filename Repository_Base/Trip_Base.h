#pragma once

#include "Trip_Interfaces.h"


namespace Trip_Components
{
	namespace Bases
	{
		struct Trip_Base
		{
		protected:

			///====================================================================
			/// Trip Initializers requires valid Time and Location Types
			template<typename ThisType, typename CallerType, typename TimeType, typename LocationType>
			void Initialize(int ID, typename TimeType::Interface_Type* start_time_val, typename TimeType::Interface_Type* end_time_val, typename LocationType::Interface_Type* origin, typename LocationType::Interface_Type* destination, call_requirements(
				requires(ThisType,Is_Dispatched) &&
				requires(TimeType,Time_Components::Concepts::Is_Time) &&
				requires(LocationType,Location_Components::Concepts::Is_Location)))
			{
				this->ID<ThisType,ThisType,int>(ID);
				this->Start_Time<ThisType,TimeType>(start_time_val);
				this->End_Time<ThisType,TimeType>(end_time_val);
				this->Origin<ThisType,LocationType>(origin);
				this->Destination<ThisType,LocationType>(destination);

			}
			template<typename ThisType, typename CallerType, typename TimeType, typename LocationType>
			void Initialize(int ID, typename TimeType::Interface_Type* start_time_val, typename TimeType::Interface_Type* end_time_val, typename LocationType::Interface_Type* origin, typename LocationType::Interface_Type* destination, call_requirements(
				requires(ThisType,!Is_Dispatched) ||
				requires(TimeType,!Time_Components::Concepts::Is_Time) ||
				requires(LocationType,!Location_Components::Concepts::Is_Location)))
			{
				assert_requirements(ThisType, Is_Dispatched, " - ThisType is not dispatched - ");
				assert_requirements(TimeType, Time_Components::Concepts::Is_Time, " - TimeType is not a Time Type - ");
				assert_requirements(LocationType, Location_Components::Concepts::Is_Location, " - LocationType is not a Location Type - ");
			}		
			template<typename ThisType, typename CallerType, typename IDType, typename TimeStructType, typename LocationStructType>
			void Initialize(IDType ID, TimeStructType start_time_struct, TimeStructType end_time_struct, LocationStructType origin_struct, LocationStructType destination_struct, call_requirements(
				requires(ThisType,Is_Dispatched) &&
				requires(TimeStructType,Time_Components::Concepts::Has_Time) &&
				(requires(LocationStructType,Location_Components::Concepts::Is_Transims_Location) || requires(LocationStructType,Location_Components::Concepts::Is_Vadim_Location))))
			{
				this->_id = (int)ID;

				this->_orig = Allocate<Location_Components::Components::Location>();
				this->_orig->Initialize<Location_Components::Components::Location,ThisType,LocationStructType>(origin_struct);

				this->_dest = Allocate<Location_Components::Components::Location>();
				this->_dest->Initialize<Location_Components::Components::Location,ThisType,LocationStructType>(destination_struct);

				this->_start = Allocate<Time_Components::Components::Time>();
				this->_start->Initialize<Time_Components::Components::Time,ThisType,TimeStructType>(start_time_struct);

				this->_end = Allocate<Time_Components::Components::Time>();
				this->_end->Initialize<Time_Components::Components::Time,ThisType,TimeStructType>(end_time_struct);
			}
			template<typename ThisType, typename CallerType, typename IDType, typename TimeStructType, typename LocationStructType>
			void Initialize(IDType ID, TimeStructType start_time_struct, TimeStructType end_time_struct, LocationStructType origin_struct, LocationStructType destination_struct, call_requirements(
				requires(ThisType,!Is_Dispatched) ||
				requires(TimeStructType,!Time_Components::Concepts::Has_Time) ||
				(requires(LocationStructType,!Location_Components::Concepts::Is_Transims_Location) && requires(LocationStructType,!Location_Components::Concepts::Is_Vadim_Location))))
			{
				assert_requirements(ThisType, Is_Dispatched, " - ThisType is not dispatched - ");
				assert_requirements(TimeStructType, Time_Components::Concepts::Has_Time, " - TimeType is not a Time Type - ");
				assert_requirements(LocationType, Location_Components::Concepts::Is_Location, " - LocationType is not a Location Type - ");
			}		


			///====================================================================
			/// Local ID Variable
			int _id;
			declare_facet_getter(ID,requires(ThisType,Is_Dispatched)&&requires(TargetType,is_arithmetic))
			{
				return (TargetType)_id;
			}
			facet TargetType ID(call_requirements(requires(ThisType,!Is_Dispatched)||requires(TargetType,!is_arithmetic)))
			{
				assert_requirements(ThisType, Is_Dispatched, "Your ThisType is not dispatched");
				assert_requirements(TargetType, is_arithmetic, "Your TargetType is not arithmetic");
			}
			declare_facet_setter(ID,requires(ThisType,Is_Dispatched)&&requires(TargetType,is_arithmetic))
			{
				_id = (int)set_value;
			}
			facet TargetType ID(TargetType set_value, call_requirements(requires(ThisType,!Is_Dispatched)||requires(TargetType,!is_arithmetic)))
			{
				assert_requirements(ThisType, Is_Same_Component, "Invalid access, Set Accessor can only be called by same component");
				assert_requirements(ThisType, Is_Dispatched, "Your ThisType is not dispatched");
				assert_requirements(TargetType, is_arithmetic, "Your TargetType is not arithmetic");
			}

			///====================================================================
			/// Trip start and end time variables
			Time_Components::Interfaces::Time_Interface* _start;
			tag_getter(Start_Time);
			tag_setter(Start_Time);
			template<typename ThisType, typename CallerType, typename TargetType>
			typename TargetType::Interface_Type* Start_Time(call_requirements(requires(ThisType,Is_Dispatched)&&requires(TargetType,Time_Components::Concepts::Is_Time)))
			{
				return (TargetType::Interface_Type*)_start;
			}
			template<typename ThisType, typename CallerType, typename TargetType>
			typename TargetType::Interface_Type* Start_Time(call_requirements(requires(ThisType,!Is_Dispatched)||requires(TargetType,!Time_Components::Concepts::Is_Time)))
			{
				assert_requirements(ThisType, Is_Dispatched, "Your ThisType is not dispatched");
				assert_requirements(TargetType, Time_Components::Concepts::Is_Time, "Your TargetType is not a valid TimeType");
			}
			template<typename ThisType, typename TargetType>
			void Start_Time(typename TargetType::Interface_Type* set_value, call_requirements(requires(ThisType,Is_Dispatched)&&requires(TargetType,Time_Components::Concepts::Is_Time)))
			{
				_start = (Time_Components::Interfaces::Time_Interface*)set_value;
			}
			template<typename ThisType, typename TargetType>
			void Start_Time(typename TargetType::Interface_Type* set_value, call_requirements(requires(ThisType,!Is_Dispatched)||requires(TargetType,!Time_Components::Concepts::Is_Time)))
			{
				assert_requirements(ThisType, Is_Dispatched, "Your ThisType is not dispatched");
				assert_requirements(TargetType, Time_Components::Concepts::Is_Time, "Your TargetType is not a valid TimeType");
			}

			Time_Components::Interfaces::Time_Interface* _end;
			tag_getter(End_Time);
			tag_setter(End_Time);
			template<typename ThisType, typename CallerType, typename TargetType>
			typename TargetType::Interface_Type* End_Time(call_requirements(requires(ThisType,Is_Dispatched)&&requires(TargetType,Time_Components::Concepts::Is_Time)))
			{
				return (TargetType::Interface_Type*)_end;
			}
			template<typename ThisType, typename CallerType, typename TargetType>
			typename TargetType::Interface_Type* End_Time(call_requirements(requires(ThisType,!Is_Dispatched)&&requires(TargetType,!Time_Components::Concepts::Is_Time)))
			{
				assert_requirements(ThisType, Is_Dispatched, "Your ThisType is not dispatched");
				assert_requirements(TargetType, Time_Components::Concepts::Is_Time, "Your TargetType is not a valid TimeType");
			}
			template<typename ThisType, typename TargetType>
			void End_Time(typename TargetType::Interface_Type* set_value, call_requirements(requires(ThisType,Is_Dispatched)&&requires(TargetType,Time_Components::Concepts::Is_Time)))
			{
				_end = (Time_Components::Interfaces::Time_Interface*)set_value;
			}
			template<typename ThisType, typename TargetType>
			void End_Time(typename TargetType::Interface_Type* set_value, call_requirements(requires(ThisType,!Is_Dispatched)&&requires(TargetType,!Time_Components::Concepts::Is_Time)))
			{
				assert_requirements(ThisType, Is_Dispatched, "Your ThisType is not dispatched");
				assert_requirements(TargetType, Time_Components::Concepts::Is_Time, "Your TargetType is not a valid TimeType");
			}

			///====================================================================
			/// Trip origin and destination location variables
			Location_Components::Interfaces::Location_Interface* _orig;
			tag_getter(Origin);
			tag_setter(Origin);
			template<typename ThisType, typename CallerType, typename TargetType>
			typename TargetType::Interface_Type* Origin(call_requirements(requires(ThisType,Is_Dispatched)&&requires(TargetType,Location_Components::Concepts::Is_Location)))
			{
				return (TargetType::Interface_Type*)_orig;
			}
			template<typename ThisType, typename CallerType, typename TargetType>
			typename TargetType::Interface_Type* Origin(call_requirements(requires(ThisType,Is_Dispatched)&&requires(TargetType,!Location_Components::Concepts::Is_Location)))
			{
				assert_requirements(TargetType, Time_Components::Concepts::Is_Time, "Your TargetType is not a valid TimeType");
			}
			template<typename ThisType, typename TargetType>
			void Origin(typename TargetType::Interface_Type* set_value, call_requirements(requires(ThisType,Is_Dispatched)&&requires(TargetType,Location_Components::Concepts::Is_Location)))
			{
				_orig = (Location_Components::Interfaces::Location_Interface*)set_value;
			}
			template<typename ThisType, typename TargetType>
			void Origin(typename TargetType::Interface_Type* set_value, call_requirements(requires(ThisType,Is_Dispatched)&&requires(TargetType,!Location_Components::Concepts::Is_Location)))
			{
				assert_requirements(TargetType, Time_Components::Concepts::Is_Time, "Your TargetType is not a valid TimeType");
			}

			Location_Components::Interfaces::Location_Interface* _dest;
			tag_getter(Destination);
			tag_setter(Destination);
			template<typename ThisType, typename CallerType, typename TargetType>
			typename TargetType::Interface_Type* Destination(call_requirements(requires(ThisType,Is_Dispatched)&&requires(TargetType,Location_Components::Concepts::Is_Location)))
			{
				return (TargetType::Interface_Type*)_dest;
			}
			template<typename ThisType, typename CallerType, typename TargetType>
			typename TargetType::Interface_Type* Destination(call_requirements(requires(ThisType,Is_Dispatched)&&requires(TargetType,!Location_Components::Concepts::Is_Location)))
			{
				assert_requirements(TargetType, Time_Components::Concepts::Is_Time, "Your TargetType is not a valid TimeType");
			}
			template<typename ThisType, typename TargetType>
			void Destination(typename TargetType::Interface_Type* set_value, call_requirements(requires(ThisType,Is_Dispatched)&&requires(TargetType,Location_Components::Concepts::Is_Location)))
			{
				_dest = (Location_Components::Interfaces::Location_Interface*)set_value;
			}
			template<typename ThisType, typename TargetType>
			void Destination(typename TargetType::Interface_Type* set_value, call_requirements(requires(ThisType,Is_Dispatched)&&requires(TargetType,!Location_Components::Concepts::Is_Location)))
			{
				assert_requirements(TargetType, Time_Components::Concepts::Is_Time, "Your TargetType is not a valid TimeType");
			}


			template<typename ThisType, typename CallerType, typename TargetType>
			void Write()
			{
				cout<<endl<<"TRIP:"<<endl;
				cout <<"ID: "<<_id<<endl;

				cout <<"Start Time: "<<endl;
				_start->Write<Time_Components::Components::Time,CallerType,TargetType>();
				cout <<endl<<"End Time: "<<endl;
				_end->Write<Time_Components::Components::Time,CallerType,TargetType>();
				cout<<endl;
				cout <<"Origin: ";
				_orig->Write<Location_Components::Components::Location, CallerType, TargetType>();
				cout<<endl;
				cout <<"Destination: ";
				_dest->Write<Location_Components::Components::Location, CallerType, TargetType>();
				cout<<endl;
			}
		};
	}

	namespace Components
	{
		typedef Polaris_Component<Interfaces::Trip_Interface,Bases::Trip_Base,NULLTYPE,NULLTYPE,Data_Object> Trip;
	}
}
