#pragma once

#include "Household_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Network_Skimming_Prototype.h"
#include "Activity_Prototype.h"


namespace Household_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Person Agent classes
		//----------------------------------------------------------------------------------
		implementation struct Household_Implementation : public Polaris_Component<APPEND_CHILD(Household_Implementation),MasterType,Data_Object,ParentType>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<APPEND_CHILD(Household_Implementation),MasterType,Data_Object>::Component_Type ComponentType;


			//=======================================================================================================================================================================
			// DATA MEMBERS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			member_prototype(PopSyn::Prototypes::Synthesis_Zone_Prototype, home_synthesis_zone, typename MasterType::zone,none,none);	
			member_prototype(Prototypes::Household_Properties, Properties, typename MasterType::household_properties_type,none,check_2(ComponentType,CallerType, Is_Same_Entity));
			member_prototype(Prototypes::Household_Properties, Static_Properties,typename MasterType::household_static_properties_type,none,none);
			
			// Physical world elements
			member_prototype(Network_Components::Prototypes::Network_Prototype, network_reference, typename MasterType::network_type, none, none);
			member_prototype(Scenario_Components::Prototypes::Scenario_Prototype, scenario_reference, typename MasterType::scenario_type, none, none);

			// Agent ID
			member_data(long,uuid,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			member_data(long,internal_id,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			
	
			// Household members
			member_container(vector<typename MasterType::person_type*>, Persons_Container, none, none);

			//=======================================================================================================================================================================
			// INTERFACE DEFINITIONS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			define_container_and_value_interface(zones_container_interface, zone_interface, typename network_reference_interface::get_type_of(zones_container),Containers::Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype,ComponentType);
			define_container_and_value_interface(locations_container_interface, location_interface, typename network_reference_interface::get_type_of(activity_locations_container),Containers::Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);

			//=======================================================================================================================================================================
			// FEATURES
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			feature_implementation void Initialize(TargetType id)
			{	
				// Set the initial iteration to process
				//this->First_Iteration<ComponentType,CallerType,Time_Minutes>(1.0);

				// Create and Initialize the Properties faculty
				_Properties = (Properties_interface*)Allocate<type_of(Properties)>();
				_Properties->template Initialize<void>();
				_Properties->template Parent_Household<ComponentType*>(this);

					
				// Add basic traveler properties							
				this->template uuid<ComponentType,ComponentType,int>(id);
				this->template internal_id<ComponentType,ComponentType,int>(id);

				
			}
			feature_implementation void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type home_zone, typename TargetType::Param3Type network_ref, typename TargetType::Param4Type scenario_ref)
			{
				this->Initialize<ComponentType,CallerType, typename TargetType::ParamType>(id);
				this->home_synthesis_zone<ComponentType,CallerType, typename TargetType::Param2Type>(home_zone);
				this->_network_reference = (network_reference_interface*)network_ref;
				this->_scenario_reference = (scenario_reference_interface*)scenario_ref;

			}
			tag_feature_as_available(Initialize);

			feature_implementation void Set_Home_Location()
			{
				_Properties->template Initialize<Target_Type<NT,void,home_synthesis_zone_interface*> >(this->_home_synthesis_zone);
			}

		};

	}
}