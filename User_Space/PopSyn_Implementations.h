#pragma once

#include "PopSyn_Prototype.h"

namespace PopSyn
{
	namespace Implementations
	{
		implementation struct ADAPTS_Population_Synthesis_Implementation : public Polaris_Component_Class<ADAPTS_Population_Synthesis_Implementation, MasterType, Execution_Object, ParentType>
		{
			member_associative_container(concat(dense_hash_map<typename MasterType::region::ID_type, typename MasterType::region*>),Synthesis_Regions_Collection, none,none);
			member_component(typename MasterType::IPF_Solver_Settings,Solution_Settings,none,none);
			member_component(typename MasterType::scenario_type, scenario_reference, none, none);
			member_component(typename MasterType::network_type, network_reference, none, none);

			member_data(Counter, timer,none,none);
			member_data(string, linker_file_path, none,none);
			member_pointer(ostream,Output_Stream,none,none);
			member_pointer(ostream,Marginal_Output_Stream,none,none);

			feature_implementation void Initialize()
			{
				this->_Synthesis_Regions_Collection.set_empty_key(-1);
				this->_Synthesis_Regions_Collection.set_deleted_key(-2);
			}
		};
	}
}