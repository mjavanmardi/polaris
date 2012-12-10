#pragma once

#include "User_Space\User_Space_includes.h"
#include "User_Space\Population_Synthesis\Population_Unit_Implementations.h"
#include "User_Space\Population_Synthesis\Synthesis_Zone_Prototypes.h"

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
				// Get the solution settings
				define_component_interface(solution_settings_itf,get_type_of(Solver_Settings),Prototypes::Solver_Settings_Prototype,NULLTYPE);
				solution_settings_itf& settings = this->Solver_Settings<solution_settings_itf&>();

				// Get the list of synthesis zones in the region
				define_container_and_value_interface(synthesis_zones_itf,zone_itf,get_type_of(Synthesis_Zone_Collection),Containers::Random_Access_Sequence_Prototype, Prototypes::Synthesis_Zone_Prototype,NULLTYPE);
				synthesis_zones_itf& zones_collection = this->Synthesis_Zone_Collection<synthesis_zones_itf&>();
				synthesis_zones_itf::iterator zone_itf = zones_collection.begin();

				// IPF version of fitting the joint distribution to marginal distribution
				typedef get_type_of(Target_Joint_Distribution)::unqualified_value_type value_type;
				define_simple_container_interface(mway_itf, get_type_of(Target_Joint_Distribution),Multidimensional_Random_Access_Array_Prototype,value_type,NULLTYPE);
				define_simple_container_interface(marg_itf, get_type_of(Target_Marginal_Distribution),Multidimensional_Random_Access_Array_Prototype,value_type,NULLTYPE);

				mway_itf::iterator itr;
				marg_itf::iterator marg_itr;
				typedef marg_itf::index_type index;


				// get the distribution
				mway_itf& mway = this->Target_Joint_Distribution<mway_itf&>();
				mway_itf::const_index_type dimensions = mway.dimensions();
				mway_itf::size_type num_dim = (mway_itf::size_type)(dimensions.size());

				// get the marginals
				marg_itf& marg = this->Target_Marginal_Distribution<marg_itf&>();
				
				// Main Execution loop - loop over each dimension, and each index within each dimensions and fit to the marginal
				value_type max_error = (value_type)INT_MAX;
				int iterations = 0;
				while (iterations < settings.Iterations<uint>() && max_error > settings.Tolerance<value_type>())
				{
					// 0. reset max error to 0, then store the highest value for current iteration
					max_error = 0;

					// 1.  Loop over each dimension
					for (int d = 0; d < num_dim; ++d)
					{
						// 2. loop over each index in the marginal for each dimension
						for (int i = 0; i < dimensions[d]; ++i)
						{
							// 3. get the current sum in the distribution for the current marginal
							value_type sum = 0;
							for (itr = mway.begin(d,i); itr != mway.end(); ++itr) sum += *itr;

							// 4. calculate the error against the known marginal value
							value_type marg_val = marg[index(d,i)];
							value_type temp_err = (marg_val != 0 ) ? (sum / marg_val) : 1;
							if (abs((value_type)(temp_err - 1.0)) > max_error) max_error = abs((value_type)(temp_err - 1.0));

							// 5. update the values in the distribution by the error factor
							for (itr = mway.begin(d,i); itr != mway.end(); ++itr) *itr = *itr / temp_err;
						}
					}

					iterations++;
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