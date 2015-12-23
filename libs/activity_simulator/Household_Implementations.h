#pragma once

#include "Household_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "traffic_simulator\Network_Prototype.h"
#include "traffic_simulator\Scenario_Prototype.h"
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
			m_prototype(PopSyn::Prototypes::Synthesis_Zone, typename MasterType::synthesis_zone_type, home_synthesis_zone, NONE, NONE);	
			m_prototype(Household_Components::Prototypes::Household_Properties, typename MasterType::household_properties_type, Properties, NONE,NONE);
			m_prototype(Household_Components::Prototypes::Household_Properties,typename MasterType::household_static_properties_type, Static_Properties, NONE, NONE);
			
			// Physical world elements
			m_prototype(Network_Components::Prototypes::Network, typename MasterType::network_type, network_reference, NONE, NONE);
			m_prototype(Scenario_Components::Prototypes::Scenario, typename MasterType::scenario_type, scenario_reference, NONE, NONE);

			// Agent ID
			m_data(long,uuid, NONE, NONE);
			m_data(long,internal_id, NONE, NONE);
			
	
			// Household members
			m_container(boost::container::vector<Person_Components::Prototypes::Person<typename MasterType::person_type>*>, Persons_Container, NONE, NONE);

			//=======================================================================================================================================================================
			// INTERFACE DEFINITIONS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			typedef Pair_Associative_Container< typename type_of(network_reference)::get_type_of(zones_container)> zones_container_interface;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones_container_interface)>  zone_interface;
			
			typedef Random_Access_Sequence< typename type_of(network_reference)::get_type_of(activity_locations_container)> locations_container_interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(locations_container_interface)>  location_interface;
			

			//=======================================================================================================================================================================
			// FEATURES
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			template<typename TargetType> void Initialize(TargetType id)
			{	
				// Set the initial iteration to process
				//this->First_Iteration<Time_Minutes>(1.0);

				// Create and Initialize the Properties faculty
				_Properties = (Properties_type)Allocate<type_of(Properties)>();
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
				this->network_reference<NetworkRefType>(network_ref);
				this->scenario_reference<ScenarioRefType>(scenario_ref);

			}

			template<typename TargetType> void Set_Home_Location()
			{
				_Properties->template Initialize<home_synthesis_zone_type>(this->_home_synthesis_zone);
			}

		};

	}
}
