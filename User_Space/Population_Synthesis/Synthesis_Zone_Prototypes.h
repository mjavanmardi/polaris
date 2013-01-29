#pragma once

#include "User_Space\User_Space_includes.h"
#include "User_Space\Population_Synthesis\Population_Unit_Implementations.h"

using namespace std;

//---------------------------------------------------------
//	SUMMARY FILE CLASS
//---------------------------------------------------------
namespace PopSyn
{
	namespace Concepts
	{
		concept struct Is_IPF_Capable
		{
			check_typename_defined(Has_Value_Type, Value_Type);
			check_typename_state(Has_Marginals, Has_Marginals_In_Distribution, true_type);
			define_default_check(Has_Marginals && Has_Value_Type);
		};

		concept struct Is_Probabilistic_Selection
		{
			check_typename_defined(Has_Value_Type, Value_Type);
			check_typename_state(Has_Probabilistic_Selection_Defined, Probabilistic_Selection_Type, true_type);
			define_default_check(Has_Probabilistic_Selection_Defined && Has_Value_Type);
		};

		concept struct Is_Loss_Function_Selection
		{
			check_typename_defined(Has_Value_Type, Value_Type);
			check_typename_state(Has_Loss_Function_Selection_Defined, Loss_Function_Selection_Type, true_type);
			define_default_check(Has_Probabilistic_Selection_Defined && Has_Value_Type);
		};


		concept struct Is_IPF_Solver_Setting
		{
			check_typed_member(Has_IPF_Tolerance_d, IPF_Tolerance, double);
			check_typed_member(Has_IPF_Tolerance_f, IPF_Tolerance, float);
			check_typed_member(Has_Max_Iterations_int, Max_Iterations, int);
			check_typed_member(Has_Max_Iterations_uint, Max_Iterations, unsigned int);
			check_typed_member(Has_Max_Iterations_long, Max_Iterations, long);
			check_typed_member(Has_Max_Iterations_ulong, Max_Iterations, long);

			define_default_check((Has_IPF_Tolerance_d || Has_IPF_Tolerance_f) && (Has_Max_Iterations_int || Has_Max_Iterations_uint || Has_Max_Iterations_long || Has_Max_Iterations_ulong));
		};
	}

	namespace Prototypes
	{
		prototype struct Solver_Settings_Prototype : public ComponentType
		{
			tag_as_prototype;
			feature_prototype void Initialize(typename TargetType::ParamType tolerance, typename TargetType::Param2Type iterations)
			{
				this_component()->initialize<ComponentType,CallerType,TargetType>(tolerance,iterations);
			}
			feature_accessor(Tolerance,check(ReturnValueType, is_arithmetic),not_available);
			feature_accessor(Iterations,check(ReturnValueType, is_arithmetic),not_available);
		};

		prototype struct Synthesis_Zone_Prototype : public ComponentType
		{
			tag_as_prototype;
			feature_prototype void Initialize()
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>();
			}

			//===================================================================================================================================
			// Defintion of the IPF procedure which can only be called for and IPF capable implementation
			feature_prototype void Fit_Joint_Distribution_To_Marginal_Data(requires(check(ComponentType,Concepts::Is_IPF_Capable)))
			{
				// Get the solution settings
				define_component_interface(solution_settings_itf,get_type_of(Solver_Settings),Prototypes::Solver_Settings_Prototype,NULLTYPE);
				solution_settings_itf& settings = this->Solver_Settings<solution_settings_itf&>();

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
				uint iterations = 0;
				while (iterations < settings.Iterations<uint>() && max_error > settings.Tolerance<value_type>())
				{
					// 0. reset max error to 0, then store the highest value for current iteration
					max_error = 0;

					// 1.  Loop over each dimension
					for (uint d = 0; d < num_dim; ++d)
					{
						// 2. loop over each index in the marginal for each dimension
						for (uint i = 0; i < dimensions[d]; ++i)
						{
							// 3. get the current sum in the distribution for the current marginal
							value_type sum = 0;
							for (itr = mway.begin(d,i); itr != mway.end(); ++itr) sum += *itr;

							// 4. calculate the error against the known marginal value
							value_type marg_val = marg[index(d,i)];
							value_type temp_err = (marg_val != 0 ) ? (sum / marg_val) : 1;
							if (abs((value_type)(temp_err - 1.0)) > max_error) max_error = abs((value_type)(temp_err - 1.0));

							// 5. update the values in the distribution by the error factor
							if (temp_err > 0) for (itr = mway.begin(d,i); itr != mway.end(); ++itr) *itr = *itr / temp_err;
						}
					}

					iterations++;
				}
						
			}
			feature_prototype void Fit_Joint_Distribution_To_Marginal_Data(requires(check(ComponentType,!Concepts::Is_IPF_Capable)))
			{
				assert_check(ComponentType,Concepts::Is_IPF_Capable,"Not IPF Capable");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Joint_Distribution_Double,"doesn't have a double joint distribution");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Joint_Distribution_Float,"doesn't have a float joint distribution");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Value_Type,"doesn't have a value_type");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Marginals,"doesn't have marginals");
				
			}
			feature_prototype void Normalize_Sample()
			{
				define_container_and_value_interface(sample_itf,unit_itf,get_type_of(Sample_Data),Containers::Associative_Container_Prototype,PopSyn::Prototypes::Population_Unit_Prototype,NULLTYPE);
				sample_itf& sample = this->Sample_Data<sample_itf&>();
				
				
				for (sample_itf::iterator itr = sample.begin(); itr != sample.end(); ++itr)
				{
					double sum = 0;
					sample_itf::key_type stored_key = itr->first;
					pair<sample_itf::iterator,sample_itf::iterator> range = sample.equal_range(stored_key);
					while (range.first != range.second)
					{
						sum += range.first->second->Weight<double>();
						++range.first;
					}

					range = sample.equal_range(stored_key);
					while (range.first != range.second)
					{
						range.first->second->Weight<double>(range.first->second->Weight<double>()/sum);
						++range.first;
					}
					itr = range.second;
					if (itr == sample.end()) break;
				}
			}

			//===================================================================================================================================
			// Defintion of the Household/Person selection procedure - can be used for IPF, Combinatorial Optimization, etc. methods
			feature_prototype void Select_Synthetic_Population_Units(TargetType Region_Sample_Ptr, 
				requires(check(ComponentType, Concepts::Is_Probabilistic_Selection)   && check_as_given(TargetType,is_pointer) && check(TargetType,Containers::Concepts::Is_Associative)))
			{
				define_component_interface(_Network_Interface,MasterType::network_type,Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_component_interface(_Scenario_Interface,MasterType::scenario_type,Scenario_Components::Prototypes::Scenario_Prototype,ComponentType);

				// Get the fitted distribution
				typedef get_type_of(Target_Joint_Distribution)::unqualified_value_type value_type;
				define_simple_container_interface(mway_itf, get_type_of(Target_Joint_Distribution),Multidimensional_Random_Access_Array_Prototype,value_type,NULLTYPE);
				mway_itf& mway = this->Target_Joint_Distribution<mway_itf&>();

				// Get pointers to the regional and zonal household samples
				define_container_and_value_interface(sample_itf, pop_unit_itf, get_type_of(Sample_Data), Associative_Container_Prototype, PopSyn::Prototypes::Population_Unit_Prototype ,NULLTYPE);
				sample_itf* sample = Region_Sample_Ptr;
				sample_itf* zone_sample = this->Sample_Data<sample_itf*>();
				
				// Get interface to the zone random number generator
				define_component_interface(rand_itf,get_type_of(Rand),RNG_Components::Prototypes::RNG_Prototype,NULLTYPE);		
				rand_itf& rand = this->Rand<rand_itf&>();

				// loop through all cells in the regional mway matrix, and make N = Mway(i,j,...) attempts to add each pop_unit corresponding to that index
				// The code below grabs a range for each index in the MWAY matrix which has corresponding samples which can be chosen.
				// note that this routine does not work when a loss function can be used (i.e. households are not uniquely assigned to one mway index)
				for (sample_itf::iterator itr = sample->begin(); itr != sample->end(); ++itr)
				{
					//----------------------------------------------------------------------------------
					// get a pointer to the value of the iterator for IDE support
					pop_unit_itf* stored_pop_unit;

					// get the current key (i.e. index into mway)
					sample_itf::key_type index = itr->first;
					double num_required = mway[index];
				
					// grab all sample units which link to this cell
					pair<sample_itf::iterator,sample_itf::iterator> range = sample->equal_range(index);

					stored_pop_unit = range.first->second;
					double cumulative_weight = 1;

					//----------------------------------------------------------------------------------
					// make num_required attempts to add each unit in the range
					while (range.first != range.second)
					{
						int num_generated=0;
						double w = range.first->second->Weight<double>();
						
						for (int i = 0; i<num_required; ++i)
						{
							if (rand.Next_Rand<double>() < w/cumulative_weight)
							{					
								//pop_unit_itf* p = (pop_unit_itf*)Allocate<pop_unit_itf::Component_Type>(); // ALLOCATE
								pop_unit_itf* p = (pop_unit_itf*)(new pop_unit_itf::Component_Type());
								pop_unit_itf* obj = range.first->second;
								p->Initialize<pop_unit_itf&>(*obj);
								zone_sample->insert(p->Index<uint>(),p);
								int size = sizeof(typename sample_itf::Component_Type);
								num_generated++;

								// create the actual person agent
								/*typedef Person_Components::Prototypes::Person_Prototype<typename MasterType::person_type, ComponentType> _Traveler_Interface;
								_Traveler_Interface* traveler=(_Traveler_Interface*)Allocate<typename MasterType::person_type>();
								traveler->network_reference<_Network_Interface*>(this->network_reference<_Network_Interface*>());
								traveler->scenario_reference<_Scenario_Interface*>(this->scenario_reference<_Scenario_Interface*>());			
								traveler->Initialize<int>(i);*/
							}
						}
						// reduce the number required by the number generated and the cumulative weight
						num_required -= num_generated;
						cumulative_weight -= w;

						//move to the next unit for the current index
						++range.first;
					}

					//----------------------------------------------------------------------------------
					// add a copy of the last unit until num_required < 1
					while (num_required > 1.0)
					{
						//pop_unit_itf* p = (pop_unit_itf*)Allocate<pop_unit_itf::Component_Type>(); // ALLOCATE
						pop_unit_itf* p = (pop_unit_itf*)(new pop_unit_itf::Component_Type());
						pop_unit_itf* obj = stored_pop_unit;
						p->Initialize<pop_unit_itf&>(*obj);
						zone_sample->insert(p->Index<uint>(),p);
						num_required--;
					}

					//----------------------------------------------------------------------------------
					// if a fractional num_required is left, add another unit with probability of num_required
					if (num_required > 0.0 && rand.Next_Rand<double>() < num_required)
					{
						//pop_unit_itf* p = (pop_unit_itf*)Allocate<pop_unit_itf::Component_Type>(); // ALLOCATE
						pop_unit_itf* p = (pop_unit_itf*)(new pop_unit_itf::Component_Type());
						pop_unit_itf* obj = stored_pop_unit;
						p->Initialize<pop_unit_itf&>(*obj);
						zone_sample->insert(p->Index<uint>(),p);
						num_required--;
					}


					// if at the end of the map, break so that increment does not go past end of data_structure
					itr = range.second;
					if (itr == sample->end()) break;
				}		
			}
			feature_prototype void Select_Synthetic_Population_Units(TargetType Region_Sample_Ptr, 
				requires(!(check(ComponentType, Concepts::Is_Probabilistic_Selection) && check_as_given(TargetType,is_pointer) && check(TargetType,Containers::Concepts::Is_Associative))))
			{
				assert_check(ComponentType, Concepts::Is_Probabilistic_Selection,"Not probabilistic selection defined.");
				assert_check(TargetType, is_pointer,"Is not a pointer");
				assert_check_strip(TargetType, Containers::Concepts::Is_Associative, "Container is not associative.");
			}

			//===================================================================================================================================
			// Accessor for the Joint Distribution (IPF and CO only) and marginal distributions used in synthesis
			feature_accessor(Target_Joint_Distribution,none,none);
			feature_accessor(Target_Marginal_Distribution,none,none);

			//===================================================================================================================================
			// REQUIRED: Accessor for the Synthesized Population (if the component is a zone) or the sample population (if it is a region)
			feature_accessor(Sample_Data,none,none);
			feature_accessor(scenario_reference, none, none);
			feature_accessor(network_reference, none, none);
			feature_accessor(parent_reference, none, none);
			
			//===================================================================================================================================
			// OPTIONAL: Other accessors that may be of use
			feature_accessor(ID,none,none);
			feature_accessor(Rand,none,none);
			feature_accessor(Solver_Settings,none,none);
			feature_accessor(Selection_Settings,none,none);

			feature_prototype typename TargetType::ReturnType Get_1D_Index(typename TargetType::ParamType& multi_dimensional_index_vector)
			{
				return this_component()->Get_1D_Index<ComponentType,CallerType,TargetType>(multi_dimensional_index_vector);
			}
		};

	}
}

