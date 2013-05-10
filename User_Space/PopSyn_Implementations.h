#pragma once

#include "PopSyn_Prototype.h"

namespace PopSyn
{
	namespace Implementations
	{
		implementation struct ADAPTS_Population_Synthesis_Implementation : public Polaris_Component<APPEND_CHILD(ADAPTS_Population_Synthesis_Implementation), MasterType, Execution_Object, ParentType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<APPEND_CHILD(ADAPTS_Population_Synthesis_Implementation),MasterType,Execution_Object>::Component_Type ComponentType;

			member_associative_container(concat(dense_hash_map<typename MasterType::region::ID_type, typename MasterType::region*>),Synthesis_Regions_Collection, none,none);
			member_component(typename MasterType::IPF_Solver_Settings,Solution_Settings,none,none);
			member_component(typename MasterType::scenario_type, scenario_reference, none, none);
			member_component(typename MasterType::network_type, network_reference, none, none);

			member_data(Counter, timer,none,none);
			member_data(bool, write_marginal_output_flag,check(ReturnValueType,is_integral),check(SetValueType,is_integral));
			member_data(bool, write_full_output_flag,check(ReturnValueType,is_integral),check(SetValueType,is_integral));

			member_data(string, linker_file_path, none,none);
			member_data(ofstream,Log_File,none,none);
			member_data(ofstream,Output_Stream,none,none);
			member_data(ofstream,Marginal_Output_Stream,none,none);

			feature_implementation void Initialize()
			{
				this->_Synthesis_Regions_Collection.set_empty_key(-1);
				this->_Synthesis_Regions_Collection.set_deleted_key(-2);
			}
		};
	}
}