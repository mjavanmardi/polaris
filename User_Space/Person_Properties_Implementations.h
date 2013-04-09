#pragma once

#include "Person_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Network_Skimming_Prototype.h"
#include "Activity_Prototype.h"
#include "Population_Unit_Implementations.h"
#include "Person_Implementations.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Properties classes
		//----------------------------------------------------------------------------------
		implementation struct ADAPTS_Person_Properties_Implementation : public Polaris_Component<APPEND_CHILD(ADAPTS_Person_Properties_Implementation),MasterType,Data_Object,ParentType>
		{
			typedef true_type Census_Definition_Compliant;

			member_prototype(Prototypes::Person, Parent_Person, typename MasterType::person_type, none, none);

			feature_implementation void Initialize(requires(check_2(ComponentType,CallerType,Is_Same_Entity)))
			{	
				_home_location_id = -1;
				_work_location_id = -1;
			}	tag_feature_as_available(Initialize);

			member_data(int, home_location_id, none, none);
			member_data(int, work_location_id, none, none);
		};

		implementation struct TRANSIMS_Person_Properties_Implementation : public Polaris_Component<APPEND_CHILD(TRANSIMS_Person_Properties_Implementation),MasterType,Data_Object,ParentType>
		{
			member_prototype(Prototypes::Person, Parent_Person, typename MasterType::person_type, none, none);

			feature_implementation void Initialize(requires(check_2(ComponentType,CallerType,Is_Same_Entity)))
			{	
				//this->Length<ComponentType, CallerType, type_of(Length)*>(Allocate<type_of(Length)>());
				//this->Area<ComponentType, CallerType, type_of(Area)*>(Allocate<type_of(Area)>());
				//this->Time<ComponentType, CallerType, type_of(Time)*>(Allocate<type_of(Time)>());
				//this->Speed<ComponentType, CallerType,type_of(Speed)*>(Allocate<type_of(Speed)>());
				//this->Income<ComponentType, CallerType, type_of(Income)*>(Allocate<type_of(Income)>());
			}	tag_feature_as_available(Initialize);

			// Length member
			member_data_component(Basic_Units::Implementations::Length_Implementation<MasterType>,Length,none,none);
			member_component_feature(My_Length, Length, Value, Basic_Units::Prototypes::Length_Prototype);
			// Volume member
			member_data_component(typename Basic_Units::Implementations::Area_Implementation<MasterType>,Area,none,none);
			member_component_feature(My_Area, Area, Value, Basic_Units::Prototypes::Area_Prototype);
			// Time member
			member_data_component(typename Basic_Units::Implementations::Time_Implementation<MasterType>,Time,none,none);
			member_component_feature(My_Time, Time, Value, Basic_Units::Prototypes::Time_Prototype);
			// Speed member
			member_data_component(typename Basic_Units::Implementations::Speed_Implementation<MasterType>,Speed,none,none);
			member_component_feature(My_Speed, Speed, Value, Basic_Units::Prototypes::Speed_Prototype);
			// Income member
			member_data_component(typename Basic_Units::Implementations::Currency_Implementation<MasterType>,Income,none,none);
			member_component_feature(HH_Income, Income, Value, Basic_Units::Prototypes::Currency_Prototype);
		};

	}
}
