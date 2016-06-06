#pragma once

#include "Population_Synthesis_Includes.h"
#include "activity_simulator/Household_Prototype.h"
#include "activity_simulator/Person_Prototype.h"
#include <iomanip>

namespace PopSyn
{
	namespace Prototypes
	{
		prototype struct Population_Synthesizer ADD_DEBUG_INFO
		{
			tag_as_prototype; // Declare class as a polaris prototype
			typedef typename ComponentType::Master_Type MasterType;

			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			// Initializers, with and without setting a network reference.  If the version without networktype is used the events which require network information are skipped
			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			template<typename ScenarioType, requires(ScenarioType, check_stripped_type(ScenarioType, Concepts::Scenario_Has_Popsyn_Configuration_Data))>
			void Initialize(ScenarioType scenario)
			{
				this_component()->template Initialize<ScenarioType>(scenario);
			}
			template<typename NetworkType, typename ScenarioType> 
			void Initialize(NetworkType network, ScenarioType scenario, requires(NetworkType, check_stripped_type(NetworkType, Network_Components::Concepts::Is_Transportation_Network) && check_stripped_type(ScenarioType, Concepts::Scenario_Has_Popsyn_Configuration_Data)))
			{
				this->template Initialize<ScenarioType>(scenario);
				this->template network_reference<NetworkType>(network);
			}
			template<typename NetworkType, typename ScenarioType, requires(NetworkType, !check_stripped_type(NetworkType, Network_Components::Concepts::Is_Transportation_Network) || !check_stripped_type(ScenarioType, Concepts::Scenario_Has_Popsyn_Configuration_Data))>
			void Initialize(NetworkType network, ScenarioType scenario)
			{
				assert_check(NetworkType, Network_Components::Concepts::Is_Transportation_Network, "Error, the specified NetworkType is not a valid Transportation network.");
				assert_check(ScenarioType, Scenario_Components::Concepts::Has_Popsyn_Configuration_Data, "Error, the specified ScenarioType is not a valid Scenario reference.");
			}
			
			template<typename NetworkType, typename ScenarioType> void Read_From_Database(NetworkType network, ScenarioType scenario, requires(NetworkType, check_stripped_type(NetworkType, Network_Components::Concepts::Is_Transportation_Network)))
			{
				this_component()->Read_From_Database<NetworkType,ScenarioType>(network, scenario);	
			}
			template<typename NetworkType, typename ScenarioType, requires(NetworkType, !check_stripped_type(NetworkType, Network_Components::Concepts::Is_Transportation_Network))>
			void Read_From_Database(NetworkType network, ScenarioType scenario)
			{
				assert_check(NetworkType, Network_Components::Concepts::Is_Transportation_Network, "Error, the specified NetworkType is not a valid Transportation network. Must have a valid transportation network in order to read synthetic population from database");
			}
			
			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			// Required Features - necessary for any synthesis routine
			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			accessor(Synthesis_Regions_Collection, NONE, NONE);
			accessor(Solution_Settings, NONE, NONE);
			accessor(scenario_reference, NONE, NONE);
			accessor(network_reference, NONE, NONE);
			accessor(file_linker, NONE,NONE);
			accessor(timer, NONE, NONE);
			

			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			// Output features - optional, used to write intermediate and final results
			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			accessor(Output_Stream,check_2(strip_modifiers(TargetType),ofstream, is_same), check_2(strip_modifiers(TargetType),ofstream, is_same));
			accessor(Marginal_Output_Stream,check_2(strip_modifiers(TargetType),ofstream, is_same), check_2(strip_modifiers(TargetType),ofstream, is_same));
			accessor(Log_File,check_2(strip_modifiers(TargetType),ofstream, is_same), check_2(strip_modifiers(TargetType),ofstream, is_same));
		};
	}
}

