#pragma once

#include "Synthesis_Zone_Prototypes.h"

namespace PopSyn
{

	namespace Implementations
	{
		//==========================================================================================
		// Base zone class for population synthesizer, this should be inherited but not modified
		//------------------------------------------------------------------------------------------
		implementation struct _Synthesis_Zone_Base_Implementation
		{			
			m_data(long long,ID,check(strip_modifiers(TargetType),is_arithmetic),check(strip_modifiers(TargetType),is_arithmetic));
			
			m_prototype(PopSyn::Prototypes::Solver_Settings,typename MasterType::solver_settings_type,Solver_Settings, NONE, NONE);

			m_prototype(PopSyn::Prototypes::Synthesis_Region,typename MasterType::synthesis_region_type, parent_reference, NONE, NONE);
	
			typedef std::unordered_multimap<typename MasterType::household_static_properties_type::ID_type, Household_Components::Prototypes::Household_Properties<typename MasterType::household_static_properties_type>*> __sample_map_type;		
			m_container(__sample_map_type, Sample_Data, NONE, NONE); 

			template<typename TargetType> void Initialize()
			{
				this->_ID=0;
				this->_Sample_Data.clear();
				this->_parent_reference=nullptr;
				this->_Solver_Settings=nullptr;
			}

			// pass through functions to access the Top-level class scenario/network
			template<typename TargetType> TargetType scenario_reference()
			{
				if (_parent_reference == nullptr)
				{
					parent_reference_type pthis = (parent_reference_type)this;
					return pthis->scenario_reference<TargetType>();
				}
				else
				{
					return this->parent_reference<type_of(parent_reference)&>().scenario_reference<TargetType>();
				}
			}
			template<typename TargetType> TargetType network_reference()
			{
				if (_parent_reference == nullptr)
				{
					parent_reference_type pthis = (parent_reference_type)this;
					return pthis->network_reference<TargetType>();
				}
				else
				{
					return this->parent_reference<type_of(parent_reference)&>().network_reference<TargetType>();
				}
			}	
			template<typename TargetType> TargetType file_linker_reference()
			{
				if (_parent_reference == nullptr)
				{
					parent_reference_type pthis = (parent_reference_type)this;
					return pthis->file_linker_reference<TargetType>();
				}
				else
				{
					return this->parent_reference<type_of(parent_reference)&>().file_linker_reference<TargetType>();
				}
			}	
		};


		//==========================================================================================
		// Recreate the three classes below for each population synthesis algorithm
		//
		// More description about usage and inheritance patterns here (POLARIS component must be inherited first)
		//------------------------------------------------------------------------------------------
		// This is the base class which is specific for the solution algorithm
		// this class holds data members and methods which are common to both the zone and the region - and is inherited by the region class
		implementation struct _Polaris_Synthesis_Zone_Implementation : public _Synthesis_Zone_Base_Implementation<MasterType>
		{
			typedef _Synthesis_Zone_Base_Implementation<MasterType> base_type;

			//------------------------------------------------------------------------------------------
			// Type definitions which are checked by various concepts
			typedef true_type Has_Marginal_Distribution;
			typedef true_type Has_Joint_Distribution;
			typedef true_type Probabilistic_Selection_Type;
			typedef double Value_Type;

			//------------------------------------------------------------------------------------------
			// Containers for holding the joint and marginal distributions
			m_container(m_array<double>,Target_Joint_Distribution, NONE,NONE);
			m_container(s_array<double>,Target_Marginal_Distribution, NONE, NONE);
			m_container(s_array<double>,Test_Marginal_Distribution, NONE, NONE);
			m_container(m_array<double>,Target_Person_Joint_Distribution, NONE,NONE);
			m_container(s_array<double>,Target_Person_Marginal_Distribution, NONE, NONE);
			m_container(s_array<double>,Test_Person_Marginal_Distribution, NONE, NONE);
			m_container(m_array<double>,Synthesized_Joint_Distribution, NONE,NONE);
			m_container(s_array<double>,Synthesized_Marginal_Distribution, NONE, NONE);
			m_container(s_array<double>,Synthesized_Test_Marginal_Distribution, NONE, NONE);
			m_container(m_array<double>,Synthesized_Person_Joint_Distribution, NONE,NONE);
			m_container(s_array<double>,Synthesized_Person_Marginal_Distribution, NONE, NONE);
			m_container(s_array<double>,Synthesized_Test_Person_Marginal_Distribution, NONE, NONE);


			//------------------------------------------------------------------------------------------
			// Initializers which creates and intializes the data containers based on the stated dimensions of the household and person control variables
			template<typename ContainerType> void Initialize(ContainerType dims_hh, ContainerType dims_per, int dims_hh_test, int dims_per_test)
			{
				// Call base initializer
				_Synthesis_Zone_Base_Implementation<MasterType>::template Initialize<NT>();

				// Create the dimension vectors for the hh and person distributions from linker
				typename Target_Joint_Distribution_type::index_type dimensions_hh;
				typename Target_Person_Joint_Distribution_type::index_type dimensions_per;
				typename Target_Joint_Distribution_type::index_type dimensions_hh_test;
				typename Target_Person_Joint_Distribution_type::index_type dimensions_per_test;

				for (strip_modifiers(ContainerType)::iterator i = dims_hh.begin(); i != dims_hh.end(); ++i)
				{
					dimensions_hh.push_back(*i);
				}
				for (strip_modifiers(ContainerType)::iterator i = dims_per.begin(); i != dims_per.end(); ++i)
				{
					dimensions_per.push_back(*i);
				}
				dimensions_hh_test.push_back(dims_hh_test);
				dimensions_per_test.push_back(dims_per_test);

				//-----------------------------------------------------------------------------------------
				// Initialize the distribution and marginals
				this->_Target_Joint_Distribution.resize(dimensions_hh,0.0);
				this->_Target_Marginal_Distribution.resize(dimensions_hh,0.0);
				this->_Test_Marginal_Distribution.resize(dimensions_hh_test,0.0);
				this->_Target_Person_Joint_Distribution.resize(dimensions_per,0.0);
				this->_Target_Person_Marginal_Distribution.resize(dimensions_per,0.0);
				this->_Test_Person_Marginal_Distribution.resize(dimensions_per_test,0.0);
				this->_Synthesized_Joint_Distribution.resize(dimensions_hh,0.0);
				this->_Synthesized_Marginal_Distribution.resize(dimensions_hh,0.0);
				this->_Synthesized_Test_Marginal_Distribution.resize(dimensions_hh_test,0.0);
				this->_Synthesized_Person_Joint_Distribution.resize(dimensions_per,0.0);
				this->_Synthesized_Person_Marginal_Distribution.resize(dimensions_per,0.0);
				this->_Synthesized_Test_Person_Marginal_Distribution.resize(dimensions_per_test,0.0);
			}

			// Create the marginal distribution for the zone and update the marginal distribution for the region from the current input file line from 'fr'
			template<typename TargetType> void Add_Marginal_Data(File_IO::File_Reader& fr, TargetType linker)
			{
				typedef get_mapped_component_type(base_type::Sample_Data_type) sample_type;
				typedef Household_Components::Prototypes::Household_Properties<sample_type> pop_unit_itf;
				typedef Multidimensional_Random_Access_Array<Target_Marginal_Distribution_type> marginal_itf;
				marginal_itf* regional_marg, *regional_person_marg;
				if (base_type::_parent_reference != nullptr) regional_marg = base_type::_parent_reference->template Target_Marginal_Distribution<marginal_itf*>();
				if (base_type::_parent_reference != nullptr) regional_person_marg = base_type::_parent_reference->template Target_Person_Marginal_Distribution<marginal_itf*>();

				double x;

				// First read the household marginal columns
				Target_Marginal_Distribution_type::const_dimensional_type dimensions_hh = _Target_Marginal_Distribution.dimensions();
				// For each control variable dimension i
				for (typename Target_Marginal_Distribution_type::size_type i=0; i<dimensions_hh.size(); i++)
				{
					// For each category j within control dimension i
					for (typename Target_Marginal_Distribution_type::size_type j=0; j<dimensions_hh[i]; j++)
					{
						// read the value of marginal i,j from the input file
						if (!fr.Get_Data<double>(x,linker->get_sf3_column(i,j))) break;
						// set the marginal distribution value for i,j
						_Target_Marginal_Distribution[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] = x;
						// Add to the region marginal distribution
						if (base_type::_parent_reference != nullptr) (*regional_marg)[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] += x;
					}

				}

				// Then read the person marginals - same process as above
				Target_Person_Marginal_Distribution_type::const_dimensional_type dimensions_per = _Target_Person_Marginal_Distribution.dimensions();
				for (typename Target_Person_Marginal_Distribution_type::size_type i=0; i<dimensions_per.size(); i++)
				{
					for (typename Target_Person_Marginal_Distribution_type::size_type j=0; j<dimensions_per[i]; j++)
					{
						if (!fr.Get_Data<double>(x,linker->get_sf3_column(i,j,false))) break;
						_Target_Person_Marginal_Distribution[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] = x;
						if (base_type::_parent_reference != nullptr) (*regional_person_marg)[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] += x;
					}

				}

				// Finally read the test marginals
				Test_Marginal_Distribution_type::const_dimensional_type dimensions_hh_test = _Test_Marginal_Distribution.dimensions();
				// For each control variable dimension i
				for (typename Test_Marginal_Distribution_type::size_type i=0; i<dimensions_hh_test.size(); i++)
				{
					// For each category j within control dimension i
					for (typename Test_Marginal_Distribution_type::size_type j=0; j<dimensions_hh_test[i]; j++)
					{
						// read the value of marginal i,j from the input file
						if (!fr.Get_Data<double>(x,linker->get_sf3_column(i,j,true,true))) break;
						// set the marginal distribution value for i,j
						_Test_Marginal_Distribution[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] = x;
					}

				}
				Test_Person_Marginal_Distribution_type::const_dimensional_type dimensions_per_test = _Test_Person_Marginal_Distribution.dimensions();
				for (typename Test_Person_Marginal_Distribution_type::size_type i=0; i<dimensions_per_test.size(); i++)
				{
					for (typename Test_Person_Marginal_Distribution_type::size_type j=0; j<dimensions_per_test[i]; j++)
					{
						if (!fr.Get_Data<double>(x,linker->get_sf3_column(i,j,false,true))) break;
						_Test_Person_Marginal_Distribution[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] = x;

					}

				}
			}

			// This is the IPF routine which applies for both zone and region, household and person distributions
			template<typename AnalysisUnitType> void Fit_Joint_Distribution_To_Marginal_Data()
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = this->template scenario_reference<_Scenario_Interface*>();

				// Get the solution settings
				typedef PopSyn::Prototypes::Solver_Settings<type_of(typename base_type::Solver_Settings)> solution_settings_itf;
				solution_settings_itf& settings = this->template Solver_Settings<solution_settings_itf&>();

				// IPF version of fitting the joint distribution to marginal distribution
				typedef typename type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Multidimensional_Random_Access_Array< type_of(Target_Joint_Distribution)> mway_itf;
				typedef Multidimensional_Random_Access_Array<type_of(Target_Marginal_Distribution)> marg_itf;

				typename mway_itf::iterator itr;
				typename marg_itf::iterator marg_itr;
				typedef typename marg_itf::index_type index;


				// get the proper distribution based on Analysis Unit type (i.e. household or person)
				mway_itf* mway = this->Get_Joint_Distribution<AnalysisUnitType,mway_itf*>();
		
				typename mway_itf::const_index_type dimensions = mway->dimensions();
				typename mway_itf::size_type num_dim = (typename mway_itf::size_type)(dimensions.size());

				// get the marginals
				marg_itf* marg = this->Get_Marginal_Distribution<AnalysisUnitType,marg_itf*>();	

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


			//------------------------------------------------------------------------------------------
			// Member functions specific to the IPF routine
			template<typename AnalysisUnitType> void Integerize_Joint_Distribution()
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = this->template scenario_reference<_Scenario_Interface*>();
				// Get the solution settings
				typedef PopSyn::Prototypes::Solver_Settings<type_of(typename base_type::Solver_Settings)> solution_settings_itf;
				solution_settings_itf& settings = this->template Solver_Settings<solution_settings_itf&>();

				// IPF version of fitting the joint distribution to marginal distribution
				typedef typename type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Multidimensional_Random_Access_Array< type_of(Target_Joint_Distribution)> mway_itf;
				typedef Multidimensional_Random_Access_Array<type_of(Target_Marginal_Distribution)> marg_itf;

				typename mway_itf::iterator itr;
				typename marg_itf::iterator marg_itr;
				typename mway_itf::index_type index;
				typename marg_itf::index_type marg_index;
				typedef typename mway_itf::value_type value_type;


				// get the proper distribution based on Analysis Unit type (i.e. household or person)
				mway_itf* mway = this->Get_Joint_Distribution<AnalysisUnitType,mway_itf*>();
		
				typename mway_itf::const_index_type dimensions = mway->dimensions();
				typename mway_itf::size_type num_dim = (typename mway_itf::size_type)(dimensions.size());

				// get the marginals
				marg_itf* marg = this->Get_Marginal_Distribution<AnalysisUnitType,marg_itf*>();	

				// Main Execution loop - loop over each dimension, and each index within each dimensions and fit to the marginal
				value_type max_error = (value_type)INT_MAX;
				uint iterations = 0;
				
				for (itr = mway->begin(); itr != mway->end(); ++itr)
				{
					index = itr.get_index();
					
					bool valid = true;

					// for each marginal that this item is associated with
					double factor = 1.0;
					for (int i = 0; i < index.size(); ++i)
					{
						int j = index[i];
						marg_index.first = i;
						marg_index.second = j;

						double sum = mway->sum(i,j);
						double marginal = (*marg)[marg_index];
						factor *= (1.0 - (sum - marginal)); 
					}
					// if none of the marginals are exceeded
					value_type remain = (*itr) - (value_type)((int)(*itr));

					if (GLOBALS::Uniform_RNG.Next_Rand<value_type>() < remain * factor)
					{
						(*itr) += (value_type)(1.0 - remain);
					}
					else
					{
						(*itr) -= remain;
					}
				}
			}
			template<typename SampleType> double Get_Weights_For_Sample(std::vector<SampleType>& sample, std::vector<double>& sample_weights, double num_persons_remaining, requires(SampleType, check(SampleType,is_pointer)))
			{
				// Interface typedefs
				typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Pair_Associative_Container< type_of(typename base_type::Sample_Data)> sample_itf;
				typedef Household_Components::Prototypes::Household_Properties <get_mapped_component_type( type_of(base_type::Sample_Data))>  pop_unit_itf;
				typedef Random_Access_Sequence< typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_itf;
				typedef Person_Components::Prototypes::Person_Properties <get_component_type(person_sample_itf)>  person_unit_itf;
				typedef Random_Access_Sequence<typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_data_itf;
				typedef Prototypes::Solver_Settings<type_of(typename base_type::Solver_Settings)> solution_settings_itf;
				typedef typename type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Multidimensional_Random_Access_Array< type_of(Target_Joint_Distribution),value_type> mway_itf;
				typedef Multidimensional_Random_Access_Array< type_of(Target_Marginal_Distribution),value_type> marg_itf;
				typedef typename marg_itf::index_type marg_index;

				mway_itf& mway_per = this->Target_Person_Joint_Distribution<mway_itf&>();

				sample_weights.clear();

				//----------------------------------------------------------------------------------
				//get the cumulative weight of all items matching the current index by iterating over them
				double cumulative_weight = 0;
				for (auto itr = sample.begin(); itr != sample.end(); ++itr)
				{
					pop_unit_itf* hh = (pop_unit_itf*)(*itr);
					// Household weight
					double w = hh->template Weight<double>();

					// correct for likelihood of observing individual household members
					double w_sum = 0;
					int w_count = 0;
					if (mway_per.size()>0)
					{
						person_sample_itf* persons = hh->template Persons_Container<person_sample_itf*>();
						for (auto per_itr = persons->begin(); per_itr != persons->end(); per_itr++) w_sum += mway_per[(*per_itr)->template Index<int>()]/num_persons_remaining;
						w_count++;
					}
					// factor the weight by the average person suitability in the household
					w = w * (w_sum/w_count);

					sample_weights.push_back(w);

					cumulative_weight += w;
				}
				return cumulative_weight;
			}			
			template<typename UnitInterfaceType, typename ReturnType> ReturnType Get_Joint_Distribution( requires(UnitInterfaceType,check_2(strip_modifiers(UnitInterfaceType)::Component_Type,typename MasterType::household_static_properties_type,is_same) && (check(ReturnType,is_pointer) || check(ReturnType,is_reference))))
			{
				return this->Target_Joint_Distribution<ReturnType>();
			}			
			template<typename UnitInterfaceType, typename ReturnType> ReturnType Get_Joint_Distribution( requires(UnitInterfaceType,check_2(strip_modifiers(UnitInterfaceType)::Component_Type,typename MasterType::person_static_properties_type,is_same) && (check(ReturnType,is_pointer) || check(ReturnType,is_reference))))
			{
				return this->Target_Person_Joint_Distribution<ReturnType>();
			}			
			template<typename UnitInterfaceType, typename ReturnType> ReturnType Get_Marginal_Distribution( requires(UnitInterfaceType,check_2(strip_modifiers(UnitInterfaceType)::Component_Type,typename MasterType::household_static_properties_type,is_same) && (check(ReturnType,is_pointer) || check(ReturnType,is_reference))))
			{
				return this->Target_Marginal_Distribution<ReturnType>();
			}			
			template<typename UnitInterfaceType, typename ReturnType> ReturnType Get_Marginal_Distribution( requires(UnitInterfaceType,check_2(strip_modifiers(UnitInterfaceType)::Component_Type,typename MasterType::person_static_properties_type,is_same) && (check(ReturnType,is_pointer) || check(ReturnType,is_reference))))
			{
				return this->Target_Person_Marginal_Distribution<ReturnType>();
			}			
		};

		// This class holds the data members which are specific to the zone class only for the simplified synthesis (i.e. not creating execution agents)
		implementation struct Polaris_Synthesis_Zone_Implementation_Simple : public Polaris_Component< MasterType,INHERIT(Polaris_Synthesis_Zone_Implementation_Simple), Data_Object>, _Polaris_Synthesis_Zone_Implementation<MasterType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Polaris_Synthesis_Zone_Implementation_Simple),Data_Object>::Component_Type ComponentType;		
			typedef _Polaris_Synthesis_Zone_Implementation<MasterType> BaseType;

			template<typename InterfaceType, requires(InterfaceType, check_2(strip_modifiers(InterfaceType)::Component_Type, typename MasterType::household_static_properties_type, is_same))>
			void Add_To_Synthetic_Distributions(InterfaceType unit, int count)
			{
				typedef Prototypes::Popsyn_File_Linker<typename MasterType::popsyn_file_linker_type> linker_itf;

				linker_itf* linker = this->template file_linker_reference<linker_itf*>();

				// Track the updates to the household distributions
				this->_Synthesized_Joint_Distribution[unit->template Index<int>()]+=count;
				auto index = this->_Synthesized_Joint_Distribution.get_index(unit->template Index<int>());
				for (int i=0; i< index.size(); i++)
				{
					this->_Synthesized_Marginal_Distribution(i,index[i])+=count;
				}

				// update the test distribution if applicable
				if (unit->template Test_Index<int>() > 0) this->_Synthesized_Test_Marginal_Distribution(0,unit->template Test_Index<int>())+=count;
			}

			template<typename InterfaceType, requires(InterfaceType, check_2(strip_modifiers(InterfaceType)::Component_Type, typename MasterType::person_static_properties_type, is_same))>
			void Add_To_Synthetic_Distributions(InterfaceType unit, int count)
			{
				// Track the updates to the person distributions
				if (this->_Synthesized_Person_Joint_Distribution.size()>0)
				{
					this->_Synthesized_Person_Joint_Distribution[unit->template Index<int>()]+=count;
					auto index = this->_Synthesized_Person_Joint_Distribution.get_index(unit->template Index<int>());
					for (int i=0; i< index.size(); i++)
					{
						this->_Synthesized_Person_Marginal_Distribution(i,index[i])+=count;
					}
				}
				if (unit->template Test_Index<int>() >= 0) this->_Synthesized_Test_Person_Marginal_Distribution(0,unit->template Test_Index<int>())+=count;
			}

			template<typename InterfaceType, requires(InterfaceType, !check_2(strip_modifiers(InterfaceType)::Component_Type, typename MasterType::person_static_properties_type, is_same) && !check_2(strip_modifiers(InterfaceType)::Component_TYpe, typename MasterType::household_static_properties_type, is_same))>
			void Add_To_Synthetic_Distributions(InterfaceType unit, int count)
			{
				assert_check_2(strip_modifiers(InterfaceType)::Component_Type,typename MasterType::person_static_properties_type,is_same,"The object type is not the same as defined in MasterType::person_static_properties_type.");
				assert_check_2(strip_modifiers(InterfaceType)::Component_Type,typename MasterType::household_static_properties_type,is_same,"The object type is not the same as defined in MasterType::household_static_properties_type.");
			}

			using_member(BaseType,Sample_Data);
			using_member(BaseType,Target_Joint_Distribution);
			using_member(BaseType,Target_Marginal_Distribution);
			using_member(BaseType,Solver_Settings);

			template<typename TargetType> void Select_Synthetic_Population_Units(TargetType Region_Sample_Ptr)
			{
				// Interface typedefs
				typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Pair_Associative_Container< type_of(Sample_Data)> sample_itf;
				typedef Household_Components::Prototypes::Household_Properties <get_mapped_component_type( type_of(Sample_Data))>  pop_unit_itf;
				typedef Random_Access_Sequence< typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_itf;
				typedef Person_Components::Prototypes::Person_Properties <get_component_type(person_sample_itf)>  person_unit_itf;
				typedef Random_Access_Sequence<typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_data_itf;
				typedef Prototypes::Solver_Settings<type_of(Solver_Settings)> solution_settings_itf;
				typedef typename type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Multidimensional_Random_Access_Array< type_of(Target_Joint_Distribution),value_type> mway_itf;
				typedef Multidimensional_Random_Access_Array< type_of(Target_Marginal_Distribution),value_type> marg_itf;
				typedef typename marg_itf::index_type marg_index;

				solution_settings_itf& settings = this->template Solver_Settings<solution_settings_itf&>();

				// Get references to the distributions and scale them down by the synthesis percentage
				mway_itf& mway = this->template Target_Joint_Distribution<mway_itf&>();
				mway.scale(settings.template Percentage_to_synthesize<double>());
				mway_itf& mway_per = this->template Target_Person_Joint_Distribution<mway_itf&>();
				mway_per.scale(settings.template Percentage_to_synthesize<double>());

				// Get references to the distributions used to store the generated results
				mway_itf& hh_mway_temp = this->template Synthesized_Joint_Distribution<mway_itf&>();
				mway_itf& per_mway_temp = this->template Synthesized_Person_Joint_Distribution<mway_itf&>();
				marg_itf& hh_marg_temp = this->template Synthesized_Marginal_Distribution<marg_itf&>();
				marg_itf& per_marg_temp = this->template Synthesized_Person_Marginal_Distribution<marg_itf&>();

				// Get pointers to the regional and zonal household samples
				sample_itf* sample = (sample_itf*)Region_Sample_Ptr;
				sample_itf* zone_sample = this->template Sample_Data<sample_itf*>();

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
					auto s_itr = sample_units.rbegin();
					for (; s_itr != sample_units.rend(); ++s_itr)
					{
						// Update the remaining weights and cumulative weight after removal of previous household
						cumulative_weight = this->template Get_Weights_For_Sample<pop_unit_itf*>(remaining_sample_units,sample_weights,num_persons_remaining);

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
								// create the actual person agent from the census static properties and add to the zones created person agent std::list
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

			template<typename TargetType> void Add_Household_and_Persons(TargetType stored_pop_unit, double& persons_remaining)
			{
				typedef Prototypes::Synthesis_Zone<ComponentType> this_itf;
				typedef Pair_Associative_Container< type_of(Sample_Data)> sample_itf;
				typedef Household_Components::Prototypes::Household_Properties <get_mapped_component_type(sample_itf)>  pop_unit_itf;
				typedef Random_Access_Sequence< typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_itf;
				typedef Person_Components::Prototypes::Person_Properties <get_component_type(person_sample_itf)>  person_unit_itf;
				typedef Multidimensional_Random_Access_Array< type_of(Target_Joint_Distribution)> mway_itf;
				typedef Multidimensional_Random_Access_Array< type_of(Target_Marginal_Distribution)> marg_itf;
				this_itf* pthis = (this_itf*)this;

				// create the actual household agent
				pthis->template Create_Household<TargetType>(stored_pop_unit);

				// Track the updates to the household distributions
				pthis->template Add_To_Synthetic_Distributions<TargetType>(stored_pop_unit);

				mway_itf& mway_per = this->template Target_Person_Joint_Distribution<mway_itf&>();

				person_sample_itf* persons = stored_pop_unit->template Persons_Container<person_sample_itf*>();
				for (auto per_itr = persons->begin(); per_itr != persons->end(); per_itr++)
				{
					// update joint distribution
					person_unit_itf* per = (person_unit_itf*)(*per_itr);
					if (mway_per.size()>0) mway_per[per->template Index<int>()]-=1;

					// Track the updates to the person distributions
					pthis->template Add_To_Synthetic_Distributions<person_unit_itf*>(per);

					persons_remaining--;
				}
				//for (const auto& per : persons)
				//{
				//	// update joint distribution
				//	if (mway_per.size()>0) mway_per[per->template Index<int>()] -= 1;

				//	// Track the updates to the person distributions
				//	pthis->template Add_To_Synthetic_Distributions<person_unit_itf*>(per);

				//	persons_remaining--;
				//}

			}

			// Method for writing the distribution results to the marginal and joint results files
			template<typename TargetType> void Write_Distribution_Results(ofstream& marg_out, ofstream& sample_out)
			{
				// Interface typedefs
				typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Pair_Associative_Container< type_of(Sample_Data)> sample_itf;
				typedef Household_Components::Prototypes::Household_Properties <get_mapped_component_type( type_of(Sample_Data))>  pop_unit_itf;
				typedef Random_Access_Sequence< typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_itf;
				typedef Person_Components::Prototypes::Person_Properties <get_component_type(person_sample_itf)>  person_unit_itf;
				typedef Random_Access_Sequence<typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_data_itf;
				typedef Prototypes::Solver_Settings<type_of(Solver_Settings)> solution_settings_itf;
				typedef typename type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Multidimensional_Random_Access_Array< type_of(Target_Joint_Distribution),value_type> joint_itf;
				typedef Multidimensional_Random_Access_Array< type_of(Target_Marginal_Distribution),value_type> marginal_itf;

				_Scenario_Interface* scenario = this->template scenario_reference<_Scenario_Interface*>();

				// write the full population results
				if (scenario->template write_full_output<bool>())
				{
					sample_out <<endl<<endl<<"ZONE_ID: "<<this->template ID<long long int>();
					this->template Target_Joint_Distribution<joint_itf*>()->write(sample_out);
					sample_out <<endl;
					this->template Synthesized_Joint_Distribution<joint_itf*>()->write(sample_out);
					sample_out <<endl;
					// Add the synthesized distribution back to the target distribution as this is subtracted during the synthesis process
					this->template Target_Person_Joint_Distribution<joint_itf&>() + this->template Synthesized_Person_Joint_Distribution<joint_itf&>();
					this->template Target_Person_Joint_Distribution<joint_itf*>()->write(sample_out);
					sample_out <<endl;
					this->template Synthesized_Person_Joint_Distribution<joint_itf*>()->write(sample_out);
					sample_out <<endl;
				}

				// write the marginal results
				if (scenario->template write_marginal_output<bool>())
				{
					marginal_itf& marg_hh =		this->template Target_Marginal_Distribution<marginal_itf&>();
					marginal_itf& marg_per =	this->template Target_Person_Marginal_Distribution<marginal_itf&>();
					marginal_itf& syn_marg_hh = this->template Synthesized_Marginal_Distribution<marginal_itf&>();
					marginal_itf& syn_marg_per= this->template Synthesized_Person_Marginal_Distribution<marginal_itf&>();
					typedef typename marginal_itf::index_type index;

					marg_out <<this->template ID<long long int>()<<'\t';

					//-------------------------------------------------------------
					// Household level marginals:
					//-------------------------------------------------------------
					// Print original
					for (int i = 0; i < (int)marg_hh.num_dimensions(); ++i)
					{
						for (int d = 0; d < (int)marg_hh.dimensions()[i]; ++d) marg_out << marg_hh[index(i,d)]<<'\t';
					}
					marg_out<<'\t';
					// Print simulated
					for (int i = 0; i < (int)marg_hh.num_dimensions(); ++i)
					{
						for (int d = 0; d < (int)marg_hh.dimensions()[i]; ++d) marg_out << syn_marg_hh[index(i,d)]<<'\t';
					}
					marg_out<<'\t';
					// Print difference
					double sum_err = 0;
					double sum_tot = 0;
					for (int i = 0; i < (int)marg_hh.num_dimensions(); ++i)
					{
						for (int d = 0; d < (int)marg_hh.dimensions()[i]; ++d)
						{
							sum_err += abs(syn_marg_hh[index(i,d)] - marg_hh[index(i,d)]);
							sum_tot += marg_hh[index(i,d)];
							marg_out << (syn_marg_hh[index(i,d)] - marg_hh[index(i,d)]) / (marg_hh[index(i,d)])<<'\t';
						}
					}
					marg_out <<"\t\t"<<sum_err/sum_tot<<"\t\t\t";

					//-------------------------------------------------------------
					// Person level marginals:
					//-------------------------------------------------------------
					// Print original
					for (int i = 0; i < (int)marg_per.num_dimensions(); ++i)
					{
						for (int d = 0; d < (int)marg_per.dimensions()[i]; ++d) marg_out << marg_per[index(i,d)]<<'\t';
					}
					marg_out<<'\t';
					// Print simulated
					for (int i = 0; i < (int)marg_per.num_dimensions(); ++i)
					{
						for (int d = 0; d < (int)marg_per.dimensions()[i]; ++d) marg_out << syn_marg_per[index(i,d)]<<'\t';
					}
					marg_out<<'\t';
					// Print difference
					sum_err = 0;
					sum_tot = 0;
					for (int i = 0; i < (int)marg_per.num_dimensions(); ++i)
					{
						for (int d = 0; d < (int)marg_per.dimensions()[i]; ++d)
						{
							sum_err += abs(syn_marg_per[index(i,d)] - marg_per[index(i,d)]);
							sum_tot += marg_per[index(i,d)];
							marg_out << (syn_marg_per[index(i,d)] - marg_per[index(i,d)]) / (marg_per[index(i,d)])<<'\t';
						}
					}
					marg_out <<"\t\t"<<sum_err/sum_tot<<endl;
				}
			}

			template<typename TargetType> double Calculate_Fit_Statistics()
			{

			}

			// Static versions of the agent containers
			m_container(std::vector<Household_Components::Prototypes::Household_Properties<typename MasterType::household_static_properties_type>*>, Synthetic_Households_Container, NONE, NONE);
			//m_container(std::vector<Person_Components::Prototypes::Person_Properties<typename MasterType::person_static_properties_type>*>, Synthetic_Persons_Container, NONE, NONE);

		};

		// This class holds the data members for the full population synthesis which connects to the larger simulation model
		implementation struct Polaris_Synthesis_Zone_Implementation_Full : public Polaris_Synthesis_Zone_Implementation_Simple<MasterType,INHERIT(Polaris_Synthesis_Zone_Implementation_Full)>
		{
			// Tag as implementation
			typedef typename Polaris_Synthesis_Zone_Implementation_Simple<MasterType,INHERIT(Polaris_Synthesis_Zone_Implementation_Full)>::Component_Type ComponentType;

			// Full versions of the agent containers
			m_container(std::vector<Household_Components::Prototypes::Household<typename MasterType::household_type>*>, Synthetic_Households_Container, NONE, NONE);
			m_container(std::vector<int>, Activity_Locations_Container, NONE, NONE);
		};
	}
}