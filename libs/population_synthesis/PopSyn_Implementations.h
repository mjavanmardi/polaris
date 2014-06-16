#pragma once

#include "PopSyn_Prototype.h"

namespace PopSyn
{
	namespace Implementations
	{
		implementation struct ADAPTS_Population_Synthesis_Implementation : public Polaris_Component< MasterType,INHERIT(ADAPTS_Population_Synthesis_Implementation), Execution_Object>
		{
			/// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(ADAPTS_Population_Synthesis_Implementation),Execution_Object>::Component_Type ComponentType;

			/// Solution settings to use
			m_prototype(PopSyn::Prototypes::Solver_Settings,typename MasterType::solver_settings_type,Solution_Settings, NONE, NONE);
			/// Scenario settings - read in from the scenario json file
			m_prototype(Scenario_Components::Prototypes::Scenario,typename MasterType::scenario_type, scenario_reference, NONE, NONE);
			/// Reference to the model network, if doing population synthesis only - with no ABM simulation - set this to network_type to NULLTYPE
			m_prototype(Network_Components::Prototypes::Network,typename MasterType::network_type, network_reference, NONE, NONE);
			/// Reference to the file linker
			m_prototype(PopSyn::Prototypes::Popsyn_File_Linker,typename MasterType::popsyn_file_linker_type, file_linker, NONE,NONE);
			
			// List of synthesis regions to operate on, keyed on region id
			typedef dense_hash_map<typename MasterType::synthesis_region_type::ID_type, Prototypes::Synthesis_Region<typename MasterType::synthesis_region_type>*> region_map_type;
			m_container(region_map_type, Synthesis_Regions_Collection, NONE, NONE);
			
			m_data(Counter, timer, NONE, NONE);


			//m_data(string, linker_file_path, NONE, NONE);
			m_data(ofstream,Log_File, NONE, NONE);
			m_data(ofstream,Output_Stream, NONE, NONE);
			m_data(ofstream,Marginal_Output_Stream, NONE, NONE);

			template<typename TargetType> void Initialize()
			{
				this->_Synthesis_Regions_Collection.set_empty_key(-1);
				this->_Synthesis_Regions_Collection.set_deleted_key(-2);
			}
		};
	}
}
