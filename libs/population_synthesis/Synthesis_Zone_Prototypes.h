#pragma once

#include "Population_Synthesis_Includes.h"


using namespace std;

//---------------------------------------------------------
//	SUMMARY FILE CLASS
//---------------------------------------------------------
namespace PopSyn
{
	namespace Types
	{
		enum POPSYN_ITERATIONS
		{
			MAIN_INITIALIZE,
			MAIN_PROCESS
		};
		enum POPSYN_SUBITERATIONS
		{
			INITIALIZE=23,
			START_TIMING,
			PROCESS,
			STOP_TIMING,
			OUTPUT
		};
	} using namespace Types;

	namespace Prototypes
	{
		prototype struct Solver_Settings ADD_DEBUG_INFO
		{
			tag_as_prototype;

			template<typename ParamType> void Initialize(ParamType tolerance, ParamType percent_to_synthesize, ParamType iterations)
			{
				this_component()->template Initialize<ParamType>(tolerance,percent_to_synthesize,iterations);
			}
			accessor(Tolerance,check(strip_modifiers(TargetType), is_arithmetic), NONE);
			accessor(Iterations,check(strip_modifiers(TargetType), is_arithmetic), NONE);
			accessor(Percentage_to_synthesize, NONE, NONE);
		};

		prototype struct Synthesis_Zone ADD_DEBUG_INFO
		{
			tag_as_prototype;

			template<typename TargetType> void Initialize()
			{
				this_component()->template Initialize<TargetType>();
			}

			//===================================================================================================================================
			// Defintion of the IPF procedure which can only be called for and IPF capable implementation
			template<typename TargetType> void Fit_Joint_Distribution_To_Marginal_Data(requires(TargetType,check(ComponentType,Concepts::Is_IPF_Capable)))
			{
				// Get the solution settings
				typedef Prototypes::Solver_Settings<typename get_type_of(Solver_Settings)> solution_settings_itf;
				solution_settings_itf& settings = this->Solver_Settings<solution_settings_itf&>();

				// IPF version of fitting the joint distribution to marginal distribution
				typedef typename get_type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Joint_Distribution)> mway_itf;
				typedef Multidimensional_Random_Access_Array<typename get_type_of(Target_Marginal_Distribution)> marg_itf;

				typename mway_itf::iterator itr;
				typename marg_itf::iterator marg_itr;
				typedef typename marg_itf::index_type index;


				// get the distribution
				mway_itf& mway = this->Target_Joint_Distribution<mway_itf&>();
				typename mway_itf::const_index_type dimensions = mway.dimensions();
				typename mway_itf::size_type num_dim = (typename mway_itf::size_type)(dimensions.size());

				// get the marginals
				marg_itf& marg = this->Target_Marginal_Distribution<marg_itf&>();	


				// Main Execution loop - loop over each dimension, and each index within each dimensions and fit to the marginal
				value_type max_error = (value_type)INT_MAX;
				uint iterations = 0;
				while (iterations < settings.template Iterations<uint>() && max_error > settings.template Tolerance<value_type>())
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
							if (marg_val == 0)
							{
								// 5a. update the values in the distribution by the error factor
								for (itr = mway.begin(d,i); itr != mway.end(); ++itr) *itr = 0;
							}
							else
							{
								value_type temp_err = (marg_val != 0 ) ? (sum / marg_val) : 1;
								if (abs((value_type)(temp_err - 1.0)) > max_error) max_error = abs((value_type)(temp_err - 1.0));
								// 5b. update the values in the distribution by the error factor
								if (temp_err > 0) for (itr = mway.begin(d,i); itr != mway.end(); ++itr) *itr = *itr / temp_err;
							}
							
						}
					}

					iterations++;
				}
					
			}
			template<typename TargetType> void Fit_Joint_Distribution_To_Marginal_Data(requires(TargetType,check(ComponentType,!Concepts::Is_IPF_Capable)))
			{
				assert_check(ComponentType,Concepts::Is_IPF_Capable,"Not IPF Capable");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Joint_Distribution_Double,"doesn't have a double joint distribution");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Joint_Distribution_Float,"doesn't have a float joint distribution");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Value_Type,"doesn't have a value_type");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Marginals,"doesn't have marginals");
				
			}


			//===================================================================================================================================
			// Defintion of the Household/Person selection procedure - can be used for IPF, Combinatorial Optimization, etc. methods
			template<typename TargetType> void Select_Synthetic_Population_Units(TargetType Region_Sample_Ptr, 
				requires(TargetType,check(ComponentType, Concepts::Is_Probabilistic_Selection)  /*&& check(TargetType,is_pointer)*/ && check(strip_modifiers(TargetType),polaris::Container_Concepts::Is_Associative)))
			{
				typedef Network_Components::Prototypes::Network<typename ComponentType::Master_Type::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
				typedef Prototypes::Solver_Settings<typename get_type_of(Solver_Settings)> solution_settings_itf;
				solution_settings_itf& settings = this->Solver_Settings<solution_settings_itf&>();

				// Get the fitted distribution
				typedef typename get_type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Joint_Distribution),value_type> mway_itf;
				mway_itf& mway = this->Target_Joint_Distribution<mway_itf&>();
				mway.scale(settings.template Percentage_to_synthesize<double>());
			

				// Get pointers to the regional and zonal household samples		
				typedef  Pair_Associative_Container< typename get_type_of(Sample_Data)> sample_itf;
				typedef  Household_Components::Prototypes::Household_Properties <get_data_type( typename get_type_of(Sample_Data))>  pop_unit_itf;

				sample_itf* sample = (sample_itf*)Region_Sample_Ptr;
				sample_itf* zone_sample = this->Sample_Data<sample_itf*>();
				
				// Get interface to the zone random number generator
				typedef RNG_Components::Prototypes::RNG<typename get_type_of(Rand)> rand_itf;
				rand_itf& rand = this->Rand<rand_itf&>();

				// loop through all cells in the regional mway matrix, and make N = Mway(i,j,...) attempts to add each pop_unit corresponding to that index
				// The code below grabs a range for each index in the MWAY matrix which has corresponding samples which can be chosen.
				// note that this routine does not work when a loss function can be used (i.e. households are not uniquely assigned to one mway index)
				int num_created = 0;
				for (typename sample_itf::iterator itr = sample->begin(); itr != sample->end(); ++itr)
				{
					//----------------------------------------------------------------------------------
					// get a pointer to the value of the iterator for IDE support
					pop_unit_itf* stored_pop_unit = (pop_unit_itf*)itr->second;

					// get the current key (i.e. index into mway)
					typename sample_itf::key_type index = itr->first;
					double num_required = mway[index];

					//if (stored_pop_unit->Household_size<int>() == 5)
					//{
					//	int tes = 1;
					//}
									
					//----------------------------------------------------------------------------------
					//get the cumulative weight of all items corresponding to current index
					pair<typename sample_itf::iterator,typename sample_itf::iterator> range = sample->equal_range(index);
					double cumulative_weight = 0;
					while (range.first != range.second)
					{
						double w = range.first->second->template Weight<double>();
						cumulative_weight += w;
						++range.first;
					}

					//----------------------------------------------------------------------------------
					// grab all sample units which link to this cell and make num_required attempts to add each unit in the range
					range = sample->equal_range(index);
					
					while (range.first != range.second)
					{
						stored_pop_unit = range.first->second;

						int num_generated=0;
						double w = range.first->second->template Weight<double>();

						int attempts_to_make = (int)num_required;//(int)((double)num_required * settings.template Percentage_to_synthesize<double>());
						
						for (int i = 0; i<attempts_to_make; ++i)
						{
							if (rand.template Next_Rand<double>() < w/cumulative_weight)
							{					
								int size = sizeof(typename sample_itf::Component_Type);
								num_generated++;// += (int)(1.0 / settings.template Percentage_to_synthesize<double>());

								// create the actual person agent from the census static properties and add to the zones created person agent boost::container::list
								this->Create_Household<pop_unit_itf*>(stored_pop_unit);

								num_created++;// += (int)(1.0 / settings.template Percentage_to_synthesize<double>());
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
					while (num_required > 1.0 /*/ settings.template Percentage_to_synthesize<double>()*/)
					{
						// create the actual person agent
						this->Create_Household<pop_unit_itf*>(stored_pop_unit);
						num_created++;	//+=(int)(1.0 /*/ settings.template Percentage_to_synthesize<double>()*/);
						num_required--;	//-=(int)(1.0 /*/ settings.template Percentage_to_synthesize<double>()*/);
					}

					//----------------------------------------------------------------------------------
					// if a fractional num_required is left, add another unit with probability of num_required
					if (num_required > 0.0 && rand.template Next_Rand<double>() < num_required /** settings.template Percentage_to_synthesize<double>()*/)
					{
						// create the actual person agent
						this->Create_Household<pop_unit_itf*>(stored_pop_unit);
						num_created++;	//+=(int)(1.0 / settings.template Percentage_to_synthesize<double>());
						num_required--;	//-=(int)(1.0 / settings.template Percentage_to_synthesize<double>());
					}

					// if at the end of the map, break so that increment does not go past end of data_structure, otherwise step over all items in sample that had the same index and go to next index
					itr = range.second;
					if (itr == sample->end()) break;
				}		
			}
			template<typename TargetType> void Select_Synthetic_Population_Units(TargetType Region_Sample_Ptr, 
				requires(TargetType,!(check(ComponentType, Concepts::Is_Probabilistic_Selection) /*&& check(TargetType,is_pointer)*/ && check(strip_modifiers(TargetType),polaris::Container_Concepts::Is_Associative))))
			{
				assert_check(ComponentType, Concepts::Is_Probabilistic_Selection,"Not probabilistic selection defined.");
				/*assert_check(TargetType, is_pointer,"Is not a pointer");*/
				assert_check(strip_modifiers(TargetType), polaris::Container_Concepts::Is_Associative, "Container is not associative.");
			}
			
			//===================================================================================================================================
			// This method creates the actual agent object - if the popsyn is linked to a simulation, otherwise it returns nothing (use this for popsyns which only output the results)
			template<typename TargetType> void Create_Household(TargetType static_properties, requires(TargetType, sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Simulator_Usable_Prototype)))
			{
				typedef Network_Components::Prototypes::Network<typename ComponentType::Master_Type::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;

				typedef  Random_Access_Sequence< typename get_type_of(Synthetic_Households_Container)> households_itf;
				typedef  Household_Components::Prototypes::Household<get_data_type(get_type_of(Synthetic_Households_Container))>  household_itf;
				

				typedef  Random_Access_Sequence< typename household_itf::get_type_of(Persons_Container)> persons_itf;
				typedef  Person_Components::Prototypes::Person<get_data_type(typename household_itf::get_type_of(Persons_Container))>  person_itf;
				

				// interface to the ACS sample data classes
				typedef  Household_Components::Prototypes::Household_Properties <typename remove_pointer< typename get_type_of(Sample_Data)::value_type>::type>  pop_unit_itf;
				typedef  Pair_Associative_Container< typename get_type_of(Sample_Data), pop_unit_itf*> sample_itf;

				typedef  Person_Components::Prototypes::Person_Properties <typename remove_pointer< typename pop_unit_itf::get_type_of(Persons_Container)::value_type>::type>  person_unit_itf;
				typedef  Random_Access_Sequence< typename pop_unit_itf::get_type_of(Persons_Container), person_unit_itf*> person_sample_itf;

				households_itf* household_container = (households_itf*)this->Synthetic_Households_Container<persons_itf*>();

				// create new household using sample unit
				household_itf* hh=(household_itf*)Allocate<typename get_type_of(Synthetic_Households_Container)::value_type>();
				hh->template Static_Properties<pop_unit_itf*>(static_properties);

				pop_unit_itf* pop_unit = (pop_unit_itf*)static_properties;
				person_sample_itf* person_units = pop_unit->template Persons_Container<person_sample_itf*>();

				// create new person agent from each person unit properties class in the hh unit properties class
				typename person_sample_itf::iterator person_unit_itr = person_units->begin();
				for (;person_unit_itr != person_units->end(); ++person_unit_itr)
				{
					person_itf* person =(person_itf*)Allocate<typename person_itf::Component_Type>();
					person->template Static_Properties<person_unit_itf*>(*person_unit_itr);
					hh->template Persons_Container<persons_itf&>().push_back(person);
					person->template Household<household_itf*>(hh);
				}
				//
				household_container->push_back(hh);
			}
			template<typename TargetType> void Create_Household(TargetType static_properties, requires(TargetType, !sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Simulator_Usable_Prototype) && sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Synthesis_Usable_Prototype)))
			{
				typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
				
				typedef  Random_Access_Sequence< typename get_type_of(Synthetic_Households_Container)> households_itf;
				typedef  Household_Components::Prototypes::Household_Properties<typename get_value_type( typename get_type_of(Synthetic_Households_Container))>  household_itf;
				
				typedef  Random_Access_Sequence<get_type_of(Synthetic_Persons_Container)> persons_itf;
				typedef  Person_Components::Prototypes::Person_Properties<typename get_value_type(typename get_type_of(Synthetic_Persons_Container))>  person_itf;

				// interface to the ACS sample data classes
				typedef  Pair_Associative_Container< typename get_type_of(Sample_Data)> sample_itf;
				typedef  Household_Components::Prototypes::Household_Properties <typename get_data_type(typename get_type_of(Sample_Data))>  pop_unit_itf;
				
				
				typedef  Random_Access_Sequence< typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_itf;
				typedef  Person_Components::Prototypes::Person_Properties<typename get_value_type(typename pop_unit_itf::get_type_of(Persons_Container))>  person_unit_itf;
				

				households_itf* household_container = (households_itf*)this->Synthetic_Households_Container<households_itf*>();
				persons_itf* person_container = (persons_itf*)this->Synthetic_Persons_Container<persons_itf*>();

				// create new household using sample unit
				pop_unit_itf* pop_unit = (pop_unit_itf*)static_properties;
				person_sample_itf* person_units = pop_unit->template Persons_Container<person_sample_itf*>();

				// create new person agent from each person unit properties class in the hh unit properties class
				typename person_sample_itf::iterator person_unit_itr = person_units->begin();
				for (;person_unit_itr != person_units->end(); ++person_unit_itr)
				{
					person_unit_itf* person =(person_unit_itf*)(*person_unit_itr);
					person_container->push_back((person_itf*)person);
				}
				//
				household_container->push_back((household_itf*)static_properties);
			}
			template<typename TargetType> void Create_Household(TargetType static_properties, requires(TargetType, !sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Simulator_Usable_Prototype) && !sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Synthesis_Usable_Prototype)))
			{
				assert_sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Simulator_Usable_Prototype, "The ComponentType must be at least simulation compatible to use this method, or ");
				assert_sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Synthesis_Usable_Prototype, "The ComponentType must be at least simulation compatible to use this method, or ");
			}

			//===================================================================================================================================
			// Accessor for the Joint Distribution (IPF and CO only) and marginal distributions used in synthesis
			accessor(Target_Joint_Distribution, NONE, NONE);
			accessor(Target_Marginal_Distribution, NONE, NONE);
			accessor(Synthetic_Households_Container, NONE, NONE);
			accessor(Synthetic_Persons_Container, NONE, NONE);
			accessor(Activity_Locations_Container, NONE, NONE);

			//===================================================================================================================================
			// REQUIRED: Accessor for the Synthesized Population (if the component is a zone) or the sample population (if it is a region)
			accessor(Sample_Data, NONE, NONE);
			accessor(scenario_reference, NONE, NONE);
			accessor(network_reference, NONE, NONE);
			accessor(parent_reference, NONE, NONE);
			
			//===================================================================================================================================
			// OPTIONAL: Other accessors that may be of use
			accessor(ID, NONE, NONE);
			accessor(Rand, NONE, NONE);
			accessor(Solver_Settings, NONE, NONE);
			accessor(Selection_Settings, NONE, NONE);

			template<typename ParamType, typename ReturnType> ReturnType Get_1D_Index(ParamType& multi_dimensional_index_vector)
			{
				return this_component()->template Get_1D_Index<ParamType,ReturnType>(multi_dimensional_index_vector);
			}
		};

	}
}


