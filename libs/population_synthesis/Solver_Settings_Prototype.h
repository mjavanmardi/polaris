#pragma once

#include "Population_Synthesis_Includes.h"

using namespace std;

//---------------------------------------------------------
//	SUMMARY FILE CLASS
//---------------------------------------------------------
namespace PopSyn
{
	namespace Prototypes
	{
		prototype struct Solver_Settings ADD_DEBUG_INFO
		{
			tag_as_prototype;
			
			// General initializer
			template<typename ScenarioItfType> void Initialize(ScenarioItfType scenario, requires(ScenarioItfType, check(ScenarioItfType, is_pointer)))
			{
				this_component()->template Initialize<ScenarioItfType>(scenario);
			}
			template<typename ScenarioItfType> void Initialize(ScenarioItfType scenario, requires(ScenarioItfType, !check(ScenarioItfType, is_pointer)))
			{
				assert_check(ScenarioItfType, is_pointer, "ScenarioItfType must be a pointer.");
			}
			
			// General data members for any solver - add as needed
			accessor(Tolerance,check(strip_modifiers(TargetType), is_arithmetic), NONE);
			accessor(Iterations,check(strip_modifiers(TargetType), is_arithmetic), NONE);
			accessor(Percentage_to_synthesize, NONE, NONE);
		};
	};
}