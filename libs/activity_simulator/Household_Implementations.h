#pragma once

#include "Household_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "traffic_simulator/Network_Prototype.h"
#include "scenario_manager\Scenario_Prototype.h"

#include "Network_Skimming_Prototype.h"
#include "Activity_Prototype.h"


namespace Household_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Person Agent classes
		//----------------------------------------------------------------------------------
		implementation struct Household_Implementation : public Polaris_Component<MasterType,INHERIT(Household_Implementation),Execution_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Household_Implementation),Data_Object>::Component_Type ComponentType;


			//=======================================================================================================================================================================
			// DATA MEMBERS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			m_prototype(PopSyn::Prototypes::Synthesis_Zone, typename MasterType::synthesis_zone_type, home_synthesis_zone, NONE, NONE);	
			m_prototype(Household_Components::Prototypes::Household_Properties, typename MasterType::household_properties_type, Properties, NONE,NONE);
			m_prototype(Household_Components::Prototypes::Household_Properties,typename MasterType::household_static_properties_type, Static_Properties, NONE, NONE);
			m_prototype(Household_Components::Prototypes::Vehicle_Chooser, typename MasterType::vehicle_chooser_type, Vehicle_Chooser, NONE, NONE);
			
			// Physical world elements
			m_prototype(Network_Components::Prototypes::Network, typename MasterType::network_type, network_reference, NONE, NONE);
			m_prototype(Scenario_Components::Prototypes::Scenario, typename MasterType::scenario_type, scenario_reference, NONE, NONE);

			// Agent ID
			m_data(long,uuid, NONE, NONE);
			m_data(long,internal_id, NONE, NONE);
			
	
			// Household members
			m_container(std::vector<Person_Components::Prototypes::Person<typename MasterType::person_type>*>, Persons_Container, NONE, NONE);
			m_container(std::vector<Vehicle_Components::Prototypes::Vehicle<typename MasterType::vehicle_type>*>, Vehicles_Container, NONE, NONE);

			//=======================================================================================================================================================================
			// INTERFACE DEFINITIONS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			typedef Person_Components::Prototypes::Person<get_component_type(Persons_Container_type)> person_interface;
			typedef Person_Components::Prototypes::Person_Properties<typename person_interface::get_type_of(Static_Properties)> person_properties_interface;
			typedef Pair_Associative_Container< typename type_of(network_reference)::get_type_of(zones_container)> zones_container_interface;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones_container_interface)>  zone_interface;			
			typedef Random_Access_Sequence< typename type_of(network_reference)::get_type_of(activity_locations_container)> locations_container_interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(locations_container_interface)>  location_interface;
			typedef Vehicle_Components::Prototypes::Vehicle<typename get_component_type(Vehicles_Container_type)> vehicle_interface;
			

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

				_Vehicle_Chooser = (Vehicle_Chooser_type)Allocate<type_of(Vehicle_Chooser)>();
				_Vehicle_Chooser->Initialize<ComponentType*>(this);
				//_Vehicle_Chooser->Parent_Household<ComponentType*>(this);
					
				// Add basic traveler properties							
				this->template uuid<int>(id);
				this->template internal_id<int>(id);

				// Initialize all of the persons in the person container
				long perid = 0;
				for (typename Persons_Container_type::iterator p_itr = _Persons_Container.begin(); p_itr != _Persons_Container.end(); ++p_itr, ++perid)
				{
					person_interface* person = static_cast<person_interface*>(*p_itr);
					person->Initialize(perid, this->_home_synthesis_zone, this->_network_reference, this->_scenario_reference);
				}

				
			}
			template<typename IdType, typename NetworkRefType, typename ScenarioRefType> void Initialize(IdType id, NetworkRefType network_ref, ScenarioRefType scenario_ref)
			{
				this->network_reference<NetworkRefType>(network_ref);
				this->scenario_reference<ScenarioRefType>(scenario_ref);
				this->Initialize<IdType>(id);
			}
			template<typename IdType, typename SynthesisZoneType, typename NetworkRefType, typename ScenarioRefType> void Initialize(IdType id, SynthesisZoneType home_zone, NetworkRefType network_ref, ScenarioRefType scenario_ref)
			{
				this->home_synthesis_zone<SynthesisZoneType>(home_zone);
				this->Initialize<IdType,NetworkRefType,ScenarioRefType>(id,network_ref,scenario_ref);		
			}

			template<typename TargetType> void Set_Home_Location()
			{
				_Properties->template Initialize<home_synthesis_zone_type>(this->_home_synthesis_zone);
			}
			
			template<typename VehicleItfType> VehicleItfType Get_Free_Vehicle()
			{
				for (Vehicles_Container_type::iterator v_itr = _Vehicles_Container.begin(); v_itr != _Vehicles_Container.end(); ++v_itr)
				{
					if ((*v_itr)->available()) return static_cast<VehicleItfType>(*v_itr);
				}
				return nullptr;
			}

			template<typename PersonItfType> PersonItfType Get_Primary_Driver()
			{
				person_interface* primary = static_cast<person_interface*>(*(this->_Persons_Container.begin()));

				for (Persons_Container_type::iterator p_itr = this->_Persons_Container.begin(); p_itr != this->_Persons_Container.end(); ++p_itr)
				{
					person_interface* p = static_cast<person_interface*>(*p_itr);
					person_properties_interface* properties = p->Static_Properties<person_properties_interface*>();

					// find the driver with the longest work travel time to make the primary driver (or if no driver has a longest work trip, find the oldest driver)
					if (properties->Age<int>() >= 16 && properties->Journey_To_Work_Travel_Time<Time_Minutes>() >= primary->Static_Properties<person_properties_interface*>()->Journey_To_Work_Travel_Time<Time_Minutes>())
					{
						if (properties->Age<int>() > primary->Static_Properties<person_properties_interface*>()->Age<int>() && properties->Journey_To_Work_Travel_Time<Time_Minutes>() == primary->Static_Properties<person_properties_interface*>()->Journey_To_Work_Travel_Time<Time_Minutes>()) primary = p;
						else if (properties->Journey_To_Work_Travel_Time<Time_Minutes>() >= primary->Static_Properties<person_properties_interface*>()->Journey_To_Work_Travel_Time<Time_Minutes>()) primary = p;
					}
				}
				return static_cast<PersonItfType>(primary);
			}

			// return the estimated iteration when a vehicle will next be available
			template<typename TimeType> TimeType Get_Next_Available_Vehicle_Time()
			{
				Simulation_Timestep_Increment min_available_iter = END;

				// search all household vehicesls
				for (Vehicles_Container_type::iterator v_itr = _Vehicles_Container.begin(); v_itr != _Vehicles_Container.end(); ++v_itr)
				{		
					vehicle_interface* veh = *v_itr;

					// if it is currently available, then next iteration is now -> most likely should not be triggered when calling this function
					if (veh->available()) return iteration();
					
					// get the estimated return home time for unavailable vehicle
					person_interface* p = veh->traveler<person_interface*>();
					Simulation_Timestep_Increment return_iter = p->Get_Estimated_Return_Home_Time<Simulation_Timestep_Increment>();

					if (return_iter < min_available_iter) min_available_iter = return_iter;

				}
				return GLOBALS::Time_Converter.Convert_Value<Simulation_Timestep_Increment,TimeType>(min_available_iter);
			}
		};

	}
}
