#pragma once

#include "PopSyn_Prototype.h"

namespace PopSyn
{
	namespace Implementations
	{
		implementation struct ADAPTS_Population_Synthesis_Implementation : public Polaris_Component< MasterType,INHERIT(ADAPTS_Population_Synthesis_Implementation), Execution_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(ADAPTS_Population_Synthesis_Implementation),Execution_Object>::Component_Type ComponentType;

			m_container(concat(dense_hash_map<typename MasterType::region::ID_type, typename MasterType::region*>),Synthesis_Regions_Collection, NONE, NONE);
			m_prototype(PopSyn::Prototypes::Solver_Settings<typename MasterType::IPF_Solver_Settings>,Solution_Settings, NONE, NONE);
			m_prototype(Null_Prototype<typename MasterType::scenario_type>, scenario_reference, NONE, NONE);
			m_prototype(Null_Prototype<typename MasterType::network_type>, network_reference, NONE, NONE);

			m_data(Counter, timer, NONE, NONE);
			m_data(bool, write_marginal_output_flag,check(strip_modifiers(TargetType),is_integral),check(strip_modifiers(TargetType),is_integral));
			m_data(bool, write_full_output_flag,check(strip_modifiers(TargetType),is_integral),check(strip_modifiers(TargetType),is_integral));

			m_data(string, linker_file_path, NONE, NONE);
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
