#pragma once

#include "User_Space\Population_Synthesis\PopSyn_Prototype.h"

namespace PopSyn
{
	namespace Implementations
	{
		implementation struct ADAPTS_Population_Synthesis_Implementation
		{
			member_associative_container(concat(hash_map<typename MasterType::region::ID_type, typename MasterType::region*>),Synthesis_Regions_Collection, none,none);
			member_component(typename MasterType::IPF_Solver_Settings,Solution_Settings,none,none);
			member_component(typename MasterType::scenario_type, scenario_reference, none, none);
			member_component(typename MasterType::network_type, network_reference, none, none);

			member_data(Counter, timer,none,none);
			member_data(string, linker_file_path, none,none);
			member_pointer(ostream,Output_Stream,none,none);
			member_pointer(ostream,Marginal_Output_Stream,none,none);
		};
	}
}