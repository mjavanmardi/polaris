#pragma once

#include "Solver_Settings_Prototype.h"

namespace PopSyn
{

	namespace Implementations
	{
		implementation struct IPF_Solver_Settings_Implementation : public Polaris_Component< MasterType,INHERIT(IPF_Solver_Settings_Implementation), Data_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(IPF_Solver_Settings_Implementation),Data_Object>::Component_Type ComponentType;

			// Initializer - check the applicable requirement concepts to make sure proper attributes are defined in scenario
			template<typename ScenarioItfType> void Initialize(ScenarioItfType scenario, requires(ScenarioItfType, check(strip_modifiers(ScenarioItfType), Concepts::Scenario_Has_IPF_Configuration_Data)))
			{
				this->Tolerance< double>(scenario->template ipf_tolerance<double>());
				this->Percentage_to_synthesize< double>(scenario->template percent_to_synthesize<double>());
				this->Iterations< int>(scenario->template maximum_iterations<int>());
			}
			template<typename ScenarioItfType> void Initialize(ScenarioItfType scenario, requires(ScenarioItfType, !check(strip_modifiers(ScenarioItfType), Concepts::Scenario_Has_IPF_Configuration_Data)))
			{
				assert_check(strip_modifiers(ScenarioItfType), Concepts::Scenario_Has_IPF_Configuration_Data, "ScenarioItfType does not have required IPF configuration data.");
			}
			
			// Required attributes for IPF solver
			m_data(double, Tolerance, check(strip_modifiers(TargetType),is_arithmetic),check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, Iterations, check(strip_modifiers(TargetType),is_arithmetic),check(strip_modifiers(TargetType), is_arithmetic));
			m_data(double, Percentage_to_synthesize, check(strip_modifiers(TargetType),is_arithmetic),check(strip_modifiers(TargetType), is_arithmetic));
		};
	}
}
