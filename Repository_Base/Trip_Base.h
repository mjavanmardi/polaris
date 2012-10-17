#pragma once

#include "Trip_Interfaces.h"


namespace Trip_Components
{
	namespace Bases
	{
		struct Trip_Base
		{
			typedef true_type TripType;


			///====================================================================
			/// Trip Initializers requires valid Time and Location Types
			feature_implementation void Initialize(typename TargetType::ParamType ID, typename TargetType::Param2Type start_time_val, typename TargetType::Param2Type end_time_val, typename TargetType::Param3Type origin, typename TargetType::Param3Type destination, call_requirements(
				requires(ComponentType,Is_Dispatched) &&
				requires(typename TargetType::ParamType, is_arithmetic) &&
				requires(strip_modifiers(typename TargetType::Param2Type),Time_Components::Concepts::Is_Time) &&
				requires(strip_modifiers(typename TargetType::Param3Type),Location_Components::Concepts::Is_Location)))
			{
				this->_ID = ID;
				this->Start_Time<ComponentType, NULLTYPE, strip_modifiers(TargetType::Param2Type)::This_Type>(start_time_val);
				this->End_Time<ComponentType, NULLTYPE, strip_modifiers(TargetType::Param2Type)::This_Type>(end_time_val);
				this->Origin<ComponentType, NULLTYPE, strip_modifiers(TargetType::Param3Type)::This_Type>(origin);
				this->Destination<ComponentType, NULLTYPE, strip_modifiers(TargetType::Param3Type)::This_Type>(destination);
			}

			feature_implementation void Initialize(typename TargetType::ParamType ID, typename TargetType::Param2Type start_time_val, typename TargetType::Param2Type end_time_val, typename TargetType::Param3Type origin, typename TargetType::Param3Type destination, call_requirements(
				requires(ComponentType,Is_Dispatched) &&
				requires(typename TargetType::ParamType, is_arithmetic) &&
				requires(strip_modifiers(typename TargetType::Param2Type),Time_Components::Concepts::Has_Time) &&
				requires(strip_modifiers(typename TargetType::Param3Type),Location_Components::Concepts::Is_Transims_Location)))
			{
				this->_ID = (int)ID;
				
				this->_Origin = (Location_Components::Components::Location::Interface_Type<Location_Components::Components::Location>::type*)Allocate<Location_Components::Components::Location>();
				this->_Origin->Initialize<typename TargetType::Param3Type>(origin);

				this->_Destination = (Location_Components::Components::Location::Interface_Type<Location_Components::Components::Location>::type*)Allocate<Location_Components::Components::Location>();
				this->_Destination->Initialize<typename TargetType::Param3Type>(destination);

				this->_Start_Time = (Time_Components::Components::Time::Interface_Type<Time_Components::Components::Time>::type*)Allocate<Time_Components::Components::Time>();
				this->_Start_Time->Initialize<typename TargetType::Param2Type>(start_time_val);

				this->_End_Time = (Time_Components::Components::Time::Interface_Type<Time_Components::Components::Time>::type*)Allocate<Time_Components::Components::Time>();
				this->_End_Time->Initialize<typename TargetType::Param2Type>(end_time_val);
			}

			feature_implementation void Initialize(typename TargetType::ParamType ID, typename TargetType::Param2Type start_time_val, typename TargetType::Param2Type end_time_val, typename TargetType::Param3Type origin, typename TargetType::Param3Type destination, call_requirements(
				requires(ComponentType,!Is_Dispatched) ||
				requires(typename TargetType::ParamType, !is_arithmetic) ||
				(requires(strip_modifiers(typename TargetType::Param2Type),!Time_Components::Concepts::Has_Time) && requires(strip_modifiers(typename TargetType::Param2Type), !Time_Components::Concepts::Is_Time)) ||
				(requires(strip_modifiers(typename TargetType::Param3Type),!Location_Components::Concepts::Is_Transims_Location) && requires(strip_modifiers(typename TargetType::Param3Type),!Location_Components::Concepts::Is_Location))))
			{
				assert_requirements(ComponentType, Is_Dispatched, " - ComponentType is not dispatched - ");
				assert_requirements_std(typename TargetType::ParamType, is_arithmetic, " - TargetType::ParamType is not arithmetic - ");
				assert_requirements(strip_modifiers(typename TargetType::Param2Type), Time_Components::Concepts::Has_Time, " - TimeType is not a Time Type - ");
				assert_requirements(strip_modifiers(typename TargetType::Param3Type), Location_Components::Concepts::Is_Location, " - LocationType is not a Location Type - ");
			}		


			//==================================================================================================================
			// Trip Component Accessors and local data members
			//------------------------------------------------------------------------------------------------------------------
			/// Local ID Variable
			member_data(int, ID, requires(TargetType,is_arithmetic), requires(TargetType,is_arithmetic));
			///-------------------------------------------------------------------
			/// Trip start and end time variables
			member_component(Time_Components::Components::Time, Start_Time, requires(TargetType,Time_Components::Concepts::Is_Time), requires(TargetType,Time_Components::Concepts::Is_Time));
			member_component(Time_Components::Components::Time, End_Time, requires(TargetType,Time_Components::Concepts::Is_Time), requires(TargetType,Time_Components::Concepts::Is_Time));
			///-------------------------------------------------------------------
			/// Trip origin and destination variables
			member_component(Location_Components::Components::Location,Origin, requires(TargetType,Location_Components::Concepts::Is_Location), requires(TargetType,Location_Components::Concepts::Is_Location));
			member_component(Location_Components::Components::Location,Destination, requires(TargetType,Location_Components::Concepts::Is_Location), requires(TargetType,Location_Components::Concepts::Is_Location));


			feature_implementation void Write()
			{
				cout<<endl<<"TRIP:"<<endl;
				cout <<"ID: "<<_ID<<endl;

				cout <<"Start Time: "<<endl;
				_Start_Time->Write<TargetType>();
				cout <<endl<<"End Time: "<<endl;
				_End_Time->Write<TargetType>();
				cout<<endl;
				cout <<"Origin: ";
				_Origin->Write<TargetType>();
				cout<<endl;
				cout <<"Destination: ";
				_Destination->Write<TargetType>();
				cout<<endl;
			}
		};
	}

	namespace Components
	{
		typedef Polaris_Component<Interfaces::Trip_Interface,Bases::Trip_Base,NULLTYPE,NULLTYPE,Data_Object> Trip;
	}
}
