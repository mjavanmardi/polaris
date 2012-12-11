#pragma once

#include "User_Space\User_Space_includes.h"
#include "User_Space\Population_Synthesis\Population_Unit_Implementations.h"
#include "User_Space\Population_Synthesis\Synthesis_Zone_Implementations.h"

using namespace std;

//---------------------------------------------------------
//	SUMMARY FILE CLASS
//---------------------------------------------------------
namespace PopSyn
{
	namespace Concepts
	{
	}

	namespace Prototypes
	{
		prototype struct Synthesis_Region_Prototype : public Synthesis_Zone_Prototype<ComponentType,CallerType>
		{
			tag_as_prototype;

			feature_prototype void Initialize()
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void Synthesize_Population(requires(check(ComponentType,Concepts::Is_IPF_Capable)))
			{
				//==========================================================
				// Get the solution settings
				define_component_interface(solution_settings_itf,get_type_of(Solver_Settings),Prototypes::Solver_Settings_Prototype,NULLTYPE);
				solution_settings_itf& settings = this->Solver_Settings<solution_settings_itf&>();

				//==========================================================
				// Get the list of synthesis zones in the region
				define_container_and_value_interface(synthesis_zones_itf,zone_itf,get_type_of(Synthesis_Zone_Collection),Containers::Random_Access_Sequence_Prototype, Prototypes::Synthesis_Zone_Prototype,NULLTYPE);
				synthesis_zones_itf& zones_collection = this->Synthesis_Zone_Collection<synthesis_zones_itf&>();
				synthesis_zones_itf::iterator zone_itr = zones_collection.begin();

				//==========================================================
				// Get the region target distribution, use to fill zone distributions
				typedef get_type_of(Target_Joint_Distribution)::unqualified_value_type value_type;
				define_simple_container_interface(mway_itf, get_type_of(Target_Joint_Distribution),Multidimensional_Random_Access_Array_Prototype,value_type,NULLTYPE);
				mway_itf& mway = Synthesis_Zone_Prototype<ComponentType,CallerType>::Target_Joint_Distribution<mway_itf&>();


				//======================================================================
				// MAIN SYNTHESIS ROUTINE. 

				//----------------------------------------------------------
				// A. Fit the region distribution to region marginal
				Synthesis_Zone_Prototype<ComponentType,CallerType>::Fit_Joint_Distribution_To_Marginal_Data<NULLTYPE>();

				//----------------------------------------------------------
				// B. Cycle through zones and solve for each
				for (zone_itr; zone_itr != zones_collection.end(); ++zone_itr)
				{
					//----------------------------------------------------------
					// 1. Push region distribution to each zone
					(*zone_itr)->Target_Joint_Distribution<mway_itf&>().Copy(mway);

					//----------------------------------------------------------
					// 2. Fit the zone distribution to the zone marginals
					(*zone_itr)->Fit_Joint_Distribution_To_Marginal_Data<NULLTYPE>();

					//----------------------------------------------------------
					// 3. Select households from regional sample into the zone synthesized sample

					//----------------------------------------------------------
					// 4. Done. output results
				}
						
			}

			feature_prototype void Synthesize_Population(requires(check(ComponentType,!Concepts::Is_IPF_Capable)))
			{
				assert_check(ComponentType,Concepts::Is_IPF_Capable,"Not IPF Capable");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Joint_Distribution_Double,"doesn't have a double joint distribution");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Joint_Distribution_Float,"doesn't have a float joint distribution");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Value_Type,"doesn't have a value_type");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Marginals,"doesn't have marginals");
				
			}

			feature_accessor(Synthesis_Zone_Collection,none,none);

		};

	}
}