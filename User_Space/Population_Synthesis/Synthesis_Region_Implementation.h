#pragma once

#include "User_Space\Population_Synthesis\Synthesis_Region_Prototypes.h"

namespace PopSyn
{

	namespace Implementations
	{
		implementation struct Synthesis_Region_Implementation
		{
			typedef true_type Has_Marginals_In_Distribution;
			typedef double Value_Type;

			member_data(long,ID,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			member_container(m_array<double>,Target_Joint_Distribution,none,not_available);
			member_container(s_array<double>,Target_Marginal_Distribution, none, not_available);
			member_container(vector<typename MasterType::pop_unit*>, Sample_Data,none,none);

			member_component(typename MasterType::IPF_Solver_Settings,Solver_Settings,none,none);
		};

		implementation struct IPF_Solver_Settings_Implementation
		{
			member_data(double, Tolerance, check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			member_data(int, Iterations, check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
		};
	}
}
