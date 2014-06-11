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
		prototype struct Synthesis_Zone ADD_DEBUG_INFO
		{
			tag_as_prototype;

			//===================================================================================================================================
			// Initializer functions - take in the dimensional sizes for the households and persons
			template<typename TargetType> void Initialize()
			{
				this_component()->template Initialize<TargetType>();
			}
			template<typename IndexType> void Initialize(IndexType dimensions_hh, IndexType dimensions_per)
			{
				this_component()->template Initialize<IndexType>(dimensions_hh, dimensions_per);
			}

			//===================================================================================================================================
			// Functions to read household/person and marginal data and add them to the sample and distribution containers
			template<typename TargetType> void Add_Household_Sample(File_IO::File_Reader& fr, TargetType linker)
			{
				this_component()->template Add_Household_Sample<TargetType>(fr, linker);
			}
			template<typename TargetType> void Add_Person_Sample(File_IO::File_Reader& fr, TargetType linker)
			{
				this_component()->template Add_Person_Sample<TargetType>(fr, linker);
			}
			template<typename TargetType> void Add_Marginal_Data(File_IO::File_Reader& fr, TargetType linker)
			{
				this_component()->template Add_Marginal_Data<TargetType>(fr, linker);
			}
			template<typename InterfaceType> void Add_To_Synthetic_Distributions(InterfaceType unit, int count=1)
			{
				this_component()->template Add_To_Synthetic_Distributions<InterfaceType>(unit,count);
			}

			//===================================================================================================================================
			// Defintion of the IPF procedure which can only be called for and IPF capable implementation
			template<typename TargetType> void Fit_Joint_Distribution_To_Marginal_Data(bool household, requires(TargetType,check(ComponentType,Concepts::Is_IPF_Compatible)))
			{
				typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = this->scenario_reference<_Scenario_Interface*>();
				// Get the solution settings
				typedef PopSyn::Prototypes::Solver_Settings<typename get_type_of(Solver_Settings)> solution_settings_itf;
				solution_settings_itf& settings = this->Solver_Settings<solution_settings_itf&>();

				// IPF version of fitting the joint distribution to marginal distribution
				typedef typename get_type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Joint_Distribution)> mway_itf;
				typedef Multidimensional_Random_Access_Array<typename get_type_of(Target_Marginal_Distribution)> marg_itf;

				typename mway_itf::iterator itr;
				typename marg_itf::iterator marg_itr;
				typedef typename marg_itf::index_type index;


				// get the distribution
				mway_itf* mway = this->Target_Person_Joint_Distribution<mway_itf*>();
				if (household) mway = this->Target_Joint_Distribution<mway_itf*>();
		
				typename mway_itf::const_index_type dimensions = mway->dimensions();
				typename mway_itf::size_type num_dim = (typename mway_itf::size_type)(dimensions.size());

				// get the marginals
				marg_itf* marg = this->Target_Person_Marginal_Distribution<marg_itf*>();	
				if (household) marg = this->Target_Marginal_Distribution<marg_itf*>();		

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
							for (itr = mway->begin(d,i); itr != mway->end(); ++itr) sum += *itr;

							// 4. calculate the error against the known marginal value
							value_type marg_val = (*marg)[index(d,i)];
							if (marg_val == 0)
							{
								// 5a. update the values in the distribution by the error factor
								for (itr = mway->begin(d,i); itr != mway->end(); ++itr) *itr = 0;
							}
							else
							{
								value_type temp_err = (marg_val != 0 ) ? (sum / marg_val) : 1;
								if (abs((value_type)(temp_err - 1.0)) > max_error) max_error = abs((value_type)(temp_err - 1.0));
								// 5b. update the values in the distribution by the error factor
								if (temp_err > 0) for (itr = mway->begin(d,i); itr != mway->end(); ++itr) *itr = *itr / temp_err;
							}
							
						}
					}

					iterations++;
				}
			}
			template<typename TargetType> void Fit_Joint_Distribution_To_Marginal_Data(requires(TargetType,check(ComponentType,!Concepts::Is_IPF_Compatible)))
			{
				assert_check(ComponentType,Concepts::Is_IPF_Compatible,"Not IPF Capable");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_JMWAY,"doesn't have a joint distribution");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Value_Type,"doesn't have a value_type");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Marginals,"doesn't have marginals");
				
			}


			//===================================================================================================================================
			// Defintion of the Household/Person selection procedure - can be used for IPF, Combinatorial Optimization, etc. methods
			template<typename TargetType> void Select_Synthetic_Population_Units(TargetType Region_Sample_Ptr, 
				requires(TargetType,check(ComponentType, Concepts::Is_Probabilistic_Selection) && check(ComponentType,Concepts::Is_IPF_Compatible) && check(strip_modifiers(TargetType),polaris::Container_Concepts::Is_Associative)))
			{
				// Interface typedefs
				typedef Network_Components::Prototypes::Network<typename ComponentType::Master_Type::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
				typedef Pair_Associative_Container< typename get_type_of(Sample_Data)> sample_itf;
				typedef Household_Components::Prototypes::Household_Properties <get_mapped_component_type( typename get_type_of(Sample_Data))>  pop_unit_itf;
				typedef Random_Access_Sequence< typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_itf;
				typedef Person_Components::Prototypes::Person_Properties <typename get_component_type(person_sample_itf)>  person_unit_itf;
				typedef Random_Access_Sequence<typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_data_itf;
				typedef Prototypes::Solver_Settings<typename get_type_of(Solver_Settings)> solution_settings_itf;
				typedef typename get_type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Joint_Distribution),value_type> mway_itf;
				typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Marginal_Distribution),value_type> marg_itf;
				typedef typename marg_itf::index_type marg_index;

				solution_settings_itf& settings = this->Solver_Settings<solution_settings_itf&>();

				// Get references to the distributions and scale them down by the synthesis percentage
				mway_itf& mway = this->Target_Joint_Distribution<mway_itf&>();
				mway.scale(settings.template Percentage_to_synthesize<double>());		
				mway_itf& mway_per = this->Target_Person_Joint_Distribution<mway_itf&>();
				mway_per.scale(settings.template Percentage_to_synthesize<double>());

				// Get references to the distributions used to store the generated results
				mway_itf& hh_mway_temp = this->Synthesized_Joint_Distribution<mway_itf&>();
				mway_itf& per_mway_temp = this->Synthesized_Person_Joint_Distribution<mway_itf&>();
				marg_itf& hh_marg_temp = this->Synthesized_Marginal_Distribution<marg_itf&>();
				marg_itf& per_marg_temp = this->Synthesized_Person_Marginal_Distribution<marg_itf&>();

				// Get pointers to the regional and zonal household samples		
				sample_itf* sample = (sample_itf*)Region_Sample_Ptr;
				sample_itf* zone_sample = this->Sample_Data<sample_itf*>();

				std::vector<double> sample_weights;
				std::vector<pop_unit_itf*> sample_units;
				std::vector<pop_unit_itf*> remaining_sample_units;

				// loop through all cells in the regional mway matrix, and make N = Mway(i,j,...) attempts to add each pop_unit corresponding to that index
				// The code below grabs a range for each index in the MWAY matrix which has corresponding samples which can be chosen.
				// note that this routine does not work when a loss function can be used (i.e. households are not uniquely assigned to one mway index)

				for (typename sample_itf::iterator itr = sample->begin(); itr != sample->end(); ++itr)
				{
					sample_weights.clear();
					sample_units.clear();
					remaining_sample_units.clear();

					//----------------------------------------------------------------------------------
					// get a pointer to the value of the iterator for IDE support
					pop_unit_itf* stored_pop_unit = (pop_unit_itf*)itr->second;

					// get the current key (i.e. index into mway)
					typename sample_itf::key_type index = itr->first;
					double num_required = mway[index];
					double num_persons_remaining = mway_per.sum();
					

					//----------------------------------------------------------------------------------
					//get the cumulative weight of all items matching the current index by iterating over them
					double cumulative_weight = 0;
					pair<typename sample_itf::iterator,typename sample_itf::iterator> range = sample->equal_range(index);			
					while (range.first != range.second)
					{
						sample_units.push_back(range.first->second);
						remaining_sample_units.push_back(range.first->second);
						++range.first;
					}

					//-------------------------------------------------------------------------------------------------------------------------------
					// loop over all sample units which link to the current cell(index) and make num_required attempts to add each one
					std::vector<pop_unit_itf*>::reverse_iterator s_itr = sample_units.rbegin();
					for (; s_itr != sample_units.rend(); ++s_itr)
					{
						// Update the remaining weights and cumulative weight after removal of previous household
						cumulative_weight = Get_Weights_For_Sample<pop_unit_itf*>(remaining_sample_units,sample_weights,num_persons_remaining);

						stored_pop_unit = (pop_unit_itf*)(*s_itr);
						double w = sample_weights.back();

						// probability of creating the household
						double p = w/cumulative_weight;

						// make num_required attempts to add this household
						int num_attempts = (int)num_required;
						for (int i = 0; i<num_attempts; ++i)
						{
							// add probabilistically, if rand is less than probability of seeing this household out of all similar households (w/sum(w))
							if (GLOBALS::Uniform_RNG.Next_Rand<double>() < p)
							{					
								// create the actual person agent from the census static properties and add to the zones created person agent boost::container::list
								this->Add_Household_and_Persons<pop_unit_itf*>(stored_pop_unit,num_persons_remaining);
								num_required--;
							}
						}
						// reduce the number required by the number generated and the cumulative weight
						cumulative_weight -= w;

						// remove household for update in next iteration
						remaining_sample_units.pop_back();
					}

					//----------------------------------------------------------------------------------
					// add a copy of the last unit until num_required < 1
					while (num_required > 1.0)
					{
						// create the actual person agent
						this->Add_Household_and_Persons<pop_unit_itf*>(stored_pop_unit,num_persons_remaining);
						num_required--;
					}

					//----------------------------------------------------------------------------------
					// if a fractional num_required is left, add another unit with probability of num_required
					if (num_required > 0.0 && GLOBALS::Uniform_RNG.Next_Rand<double>() < num_required)
					{
						// create the actual person agent
						this->Add_Household_and_Persons<pop_unit_itf*>(stored_pop_unit,num_persons_remaining);
						num_required--;
					}

					// if at the end of the map, break so that increment does not go past end of data_structure, otherwise step over all items in sample that had the same index and go to next index
					itr = range.second;
					if (itr == sample->end()) break;
				}	
			}
			template<typename TargetType> void Select_Synthetic_Population_Units(TargetType Region_Sample_Ptr, 
				requires(TargetType,!(check(ComponentType, Concepts::Is_Probabilistic_Selection) || !check(ComponentType,Concepts::Is_IPF_Compatible) || !check(strip_modifiers(TargetType),polaris::Container_Concepts::Is_Associative))))
			{
				assert_check(ComponentType, Concepts::Is_Probabilistic_Selection,"Not probabilistic selection defined.");
				/*assert_check(TargetType, is_pointer,"Is not a pointer");*/
				assert_check(strip_modifiers(TargetType), polaris::Container_Concepts::Is_Associative, "Container is not associative.");
			}
			template<typename SampleType> double Get_Weights_For_Sample(std::vector<SampleType>& sample, std::vector<double>& sample_weights, double num_persons_remaining, requires(SampleType, check(SampleType,is_pointer)))
			{
				// Interface typedefs
				typedef Network_Components::Prototypes::Network<typename ComponentType::Master_Type::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
				typedef Pair_Associative_Container< typename get_type_of(Sample_Data)> sample_itf;
				typedef Household_Components::Prototypes::Household_Properties <get_mapped_component_type( typename get_type_of(Sample_Data))>  pop_unit_itf;
				typedef Random_Access_Sequence< typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_itf;
				typedef Person_Components::Prototypes::Person_Properties <typename get_component_type(person_sample_itf)>  person_unit_itf;
				typedef Random_Access_Sequence<typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_data_itf;
				typedef Prototypes::Solver_Settings<typename get_type_of(Solver_Settings)> solution_settings_itf;
				typedef typename get_type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Joint_Distribution),value_type> mway_itf;
				typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Marginal_Distribution),value_type> marg_itf;
				typedef typename marg_itf::index_type marg_index;

				mway_itf& mway_per = this->Target_Person_Joint_Distribution<mway_itf&>();

				sample_weights.clear();

				//----------------------------------------------------------------------------------
				//get the cumulative weight of all items matching the current index by iterating over them
				double cumulative_weight = 0;
				for (std::vector<SampleType>::iterator itr = sample.begin(); itr != sample.end(); ++itr)
				{
					pop_unit_itf* hh = (pop_unit_itf*)(*itr);
					// Household weight
					double w = hh->template Weight<double>();

					// correct for likelihood of observing individual household members
					double w_sum = 0;
					int w_count = 0;
					if (mway_per.size()>0)
					{
						person_sample_itf* persons = hh->Persons_Container<person_sample_itf*>();
						for (person_sample_itf::iterator per_itr = persons->begin(); per_itr != persons->end(); per_itr++) w_sum += mway_per[(*per_itr)->Index<int>()]/num_persons_remaining;
						w_count++;
					}
					// factor the weight by the average person suitability in the household
					w = w * (w_sum/w_count);

					sample_weights.push_back(w);

					cumulative_weight += w;
				}
				return cumulative_weight;
			}

			//===================================================================================================================================
			// This method creates the actual agent object - if the popsyn is linked to a simulation, otherwise it returns nothing (use this for popsyns which only output the results)
			template<typename TargetType> void Add_Household_and_Persons(TargetType stored_pop_unit, double& persons_remaining)
			{
				typedef Network_Components::Prototypes::Network<typename ComponentType::Master_Type::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(Synthetic_Households_Container)> households_itf;
				typedef  Household_Components::Prototypes::Household<typename get_component_type(households_itf)>  household_itf;
				typedef  Random_Access_Sequence< typename household_itf::get_type_of(Persons_Container)> persons_itf;
				typedef  Person_Components::Prototypes::Person<typename get_component_type(persons_itf)>  person_itf;
				typedef  Pair_Associative_Container< typename get_type_of(Sample_Data)> sample_itf;
				typedef  Household_Components::Prototypes::Household_Properties <typename get_mapped_component_type(sample_itf)>  pop_unit_itf;
				typedef  Random_Access_Sequence< typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_itf;
				typedef  Person_Components::Prototypes::Person_Properties <typename get_component_type(person_sample_itf)>  person_unit_itf;
				typedef typename get_type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Joint_Distribution),value_type> mway_itf;
				typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Marginal_Distribution),value_type> marg_itf;
				typedef typename marg_itf::index_type marg_index;

				// create the actual household agent
				this->Create_Household<TargetType>(stored_pop_unit);

				// Track the updates to the household distributions
				Add_To_Synthetic_Distributions<TargetType>(stored_pop_unit);	

				mway_itf& mway_per = this->Target_Person_Joint_Distribution<mway_itf&>();

				if (mway_per.size()>0)
				{
					person_sample_itf* persons = stored_pop_unit->Persons_Container<person_sample_itf*>();
					for (person_sample_itf::iterator per_itr = persons->begin(); per_itr != persons->end(); per_itr++)
					{
						// update joint distribution
						person_unit_itf* per = (person_unit_itf*)(*per_itr);
						mway_per[per->Index<int>()]-=1;

						// Track the updates to the person distributions
						Add_To_Synthetic_Distributions<person_unit_itf*>(per);

						persons_remaining--;
					}
				}
			}		
			template<typename TargetType> void Create_Household(TargetType static_properties, requires(TargetType, sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Simulator_Usable_Prototype)))
			{
				//=============================================================================================
				#pragma region Define interfaces
				//---------------------------------------------------------------------------------------------
				typedef Network_Components::Prototypes::Network<typename ComponentType::Master_Type::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(Synthetic_Households_Container)> households_itf;
				typedef  Household_Components::Prototypes::Household<typename get_component_type(households_itf)>  household_itf;
				typedef  Random_Access_Sequence< typename household_itf::get_type_of(Persons_Container)> persons_itf;
				typedef  Person_Components::Prototypes::Person<typename get_component_type(persons_itf)>  person_itf;
				typedef  Pair_Associative_Container< typename get_type_of(Sample_Data)> sample_itf;
				typedef  Household_Components::Prototypes::Household_Properties <typename get_mapped_component_type(sample_itf)>  pop_unit_itf;		
				typedef  Random_Access_Sequence< typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_itf;
				typedef  Person_Components::Prototypes::Person_Properties <typename get_component_type(person_sample_itf)>  person_unit_itf;
				#pragma endregion

				// Get the zones synthetic household container
				households_itf* household_container = (households_itf*)this->Synthetic_Households_Container<persons_itf*>();

				// Get an interface to the static household
				pop_unit_itf* pop_unit = (pop_unit_itf*)static_properties;

				// create new household using sample unit
				household_itf* hh=(household_itf*)Allocate<typename get_component_type(households_itf)>();
				hh->template Static_Properties<pop_unit_itf*>(pop_unit);

				// get the static persons associated with the static household
				person_sample_itf* person_units = pop_unit->template Persons_Container<person_sample_itf*>();

				// create new person agent from each person unit properties class in the hh unit properties class
				typename person_sample_itf::iterator person_unit_itr = person_units->begin();
				for (;person_unit_itr != person_units->end(); ++person_unit_itr)
				{
					person_itf* person =(person_itf*)Allocate<typename person_itf::Component_Type>();
					person->template Static_Properties<person_unit_itf*>((person_unit_itf*)(*person_unit_itr));
					hh->template Persons_Container<persons_itf&>().push_back(person);
					person->template Household<household_itf*>(hh);
				}
				household_container->push_back(hh);
			}
			template<typename TargetType> void Create_Household(TargetType static_properties, requires(TargetType, !sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Simulator_Usable_Prototype) && sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Synthesis_Usable_Prototype)))
			{
				typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
				
				typedef  Random_Access_Sequence< typename get_type_of(Synthetic_Households_Container)> households_itf;
				typedef  Household_Components::Prototypes::Household_Properties<typename get_component_type( typename get_type_of(Synthetic_Households_Container))>  household_itf;
				
				typedef  Random_Access_Sequence<get_type_of(Synthetic_Persons_Container)> persons_itf;
				typedef  Person_Components::Prototypes::Person_Properties<typename get_component_type(typename get_type_of(Synthetic_Persons_Container))>  person_itf;

				// interface to the ACS sample data classes
				typedef  Pair_Associative_Container< typename get_type_of(Sample_Data)> sample_itf;
				typedef  Household_Components::Prototypes::Household_Properties <typename get_mapped_component_type(typename get_type_of(Sample_Data))>  pop_unit_itf;
				
				
				typedef  Random_Access_Sequence< typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_itf;
				typedef  Person_Components::Prototypes::Person_Properties<typename get_component_type(typename pop_unit_itf::get_type_of(Persons_Container))>  person_unit_itf;
				

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
			accessor(Target_Person_Joint_Distribution, NONE, NONE);
			accessor(Target_Person_Marginal_Distribution, NONE, NONE);
			accessor(Synthesized_Joint_Distribution, NONE, NONE);
			accessor(Synthesized_Marginal_Distribution, NONE, NONE);
			accessor(Synthesized_Person_Joint_Distribution, NONE, NONE);
			accessor(Synthesized_Person_Marginal_Distribution, NONE, NONE);

			//===================================================================================================================================
			// Accessors for the Synthesized Population (if the component is a zone) or the sample population (if it is a region)
			accessor(Synthetic_Households_Container, NONE, NONE);	// This is the container holding the flyweighted synthesized data (i.e. multiple pointers to single object) created during the synthesis routine
			accessor(Synthetic_Persons_Container, NONE, NONE);		// same as above for the synthetic persons
			accessor(Sample_Data, NONE, NONE);						// This is the container holding the base sample data used in the synthesis routine
			accessor(scenario_reference, NONE, NONE);				
			accessor(network_reference, NONE, NONE);
			accessor(parent_reference, NONE, NONE);					// Reference to the parent region which contains the zone (or the parent popsyn when prototype used for regions)
			
			//===================================================================================================================================
			// OPTIONAL: Other accessors that may be of use
			accessor(ID, NONE, NONE);
			accessor(Solver_Settings, NONE, NONE);
			accessor(Activity_Locations_Container, NONE, NONE);
		};

	}
}


