#pragma once

#include "User_Space\Population_Synthesis\PopSyn_Prototype.h"

namespace PopSyn
{
	namespace Implementations
	{
		implementation struct ADAPTS_Population_Synthesis_Implementation
		{
			typedef hash_map<typename MasterType::region::ID_type, typename MasterType::region*> region_map_type;
			member_associative_container(region_map_type,Synthesis_Regions_Collection, none,none);
			member_component(typename MasterType::IPF_Solver_Settings,Solution_Settings,none,none);
			member_data(string, linker_file_path, none,none);
			member_pointer(ostream,Output_Stream,none,none);
			member_pointer(ostream,Marginal_Output_Stream,none,none);
		};
	}
}