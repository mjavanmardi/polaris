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
		implementation struct Household_Implementation : public Polaris_Component<MasterType,INHERIT(Household_Implementation),Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Household_Implementation),Data_Object>::Component_Type ComponentType;


			//=======================================================================================================================================================================
			// DATA MEMBERS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			m_prototype(PopSyn::Prototypes::Synthesis_Zone< typename MasterType::synthesis_zone_type>, home_synthesis_zone, NONE, NONE);	
			m_prototype(Prototypes::Household_Properties< typename MasterType::household_properties_type>, Properties, NONE,NONE);
			m_prototype(Prototypes::Household_Properties<typename MasterType::household_static_properties_type>, Static_Properties, NONE, NONE);
			
			// Physical world elements
			m_prototype(Network_Components::Prototypes::Network< typename MasterType::network_type>, network_reference, NONE, NONE);
			m_prototype(Scenario_Components::Prototypes::Scenario< typename MasterType::scenario_type>, scenario_reference, NONE, NONE);

			// Agent ID
			m_data(long,uuid, NONE, NONE);
			m_data(long,internal_id, NONE, NONE);
			
	
			// Household members
			m_container(boost::container::vector<typename MasterType::person_type*>, Persons_Container, NONE, NONE);

			//=======================================================================================================================================================================
			// INTERFACE DEFINITIONS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename network_reference_interface_type::get_type_of(zones_container)::value_type>::type>  zone_interface;
			typedef Pair_Associative_Container< typename network_reference_interface_type::get_type_of(zones_container), zone_interface*> zones_container_interface;

			typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename network_reference_interface_type::get_type_of(activity_locations_container)::value_type>::type>  location_interface;
			typedef Random_Access_Sequence< typename network_reference_interface_type::get_type_of(activity_locations_container), location_interface*> locations_container_interface;


			//=======================================================================================================================================================================
			// FEATURES
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			template<typename TargetType> void Initialize(TargetType id)
			{	
				// Set the initial iteration to process
				//this->First_Iteration<Time_Minutes>(1.0);

				// Create and Initialize the Properties faculty
				_Properties = (Properties_interface*)Allocate<type_of(Properties)>();
				_Properties->template Initialize<void>();
				_Properties->template Parent_Household<ComponentType*>(this);

					
				// Add basic traveler properties							
				this->template uuid<int>(id);
				this->template internal_id<int>(id);

				
			}
			template<typename IdType, typename SynthesisZoneType, typename NetworkRefType, typename ScenarioRefType> void Initialize(IdType id, SynthesisZoneType home_zone, NetworkRefType network_ref, ScenarioRefType scenario_ref)
			{
				this->Initialize<IdType>(id);
				this->home_synthesis_zone<SynthesisZoneType>(home_zone);
				this->network_reference<NetworkRefType>network_ref;
				this->scenario_reference<ScenarioRefType>scenario_ref;

			}
			tag_feature_as_available(Initialize);

			template<typename TargetType> void Set_Home_Location()
			{
				_Properties->template Initialize<home_synthesis_zone_interface*>(this->_home_synthesis_zone);
			}

		};

	}
}
