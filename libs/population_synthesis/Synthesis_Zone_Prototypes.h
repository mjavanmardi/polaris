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
			template<typename IndexType> void Initialize(IndexType dimensions_hh, IndexType dimensions_per, int dimensions_hh_test=0, int dimensions_per_test=0)
			{
				this_component()->template Initialize<IndexType>(dimensions_hh, dimensions_per,dimensions_hh_test,dimensions_per_test);
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
			// Defintion of the distribution fitting procedure, which can be called by IPF and IPU capable implementation
			template<typename AnalysisUnitType> void Fit_Joint_Distribution_To_Marginal_Data(requires(AnalysisUnitType,check(ComponentType,Concepts::Is_IPF_Compatible) || check(ComponentType,Concepts::Is_IPU_Compatible)))
			{
				this_component()->Fit_Joint_Distribution_To_Marginal_Data<AnalysisUnitType>();
			}
			template<typename AnalysisUnitType> void Fit_Joint_Distribution_To_Marginal_Data(requires(AnalysisUnitType,check(ComponentType,!Concepts::Is_IPF_Compatible) && !check(ComponentType,Concepts::Is_IPU_Compatible)))
			{
				assert_check(ComponentType,Concepts::Is_IPF_Compatible,"Not IPF Capable");
				assert_check(ComponentType,Concepts::Is_IPU_Compatible," And not Combinatorial-optimization compatible.");
//				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_JMWAY,"doesn't have a joint distribution");
//				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Value_Type,"doesn't have a value_type");
//				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Marginals,"doesn't have marginals");
				
			}
			template<typename AnalysisUnitType> void Integerize_Joint_Distribution(requires(AnalysisUnitType,check(ComponentType,Concepts::Is_IPF_Compatible) || check(ComponentType,Concepts::Is_IPU_Compatible)))
			{
				this_component()->Integerize_Joint_Distribution<AnalysisUnitType>();
			}
			template<typename AnalysisUnitType> void Integerize_Joint_Distribution(requires(AnalysisUnitType,check(ComponentType,!Concepts::Is_IPF_Compatible) && !check(ComponentType,Concepts::Is_IPU_Compatible)))
			{
				assert_check(ComponentType,Concepts::Is_IPF_Compatible,"Not IPF Capable");
				assert_check(ComponentType,Concepts::Is_IPU_Compatible," And not Combinatorial-optimization compatible.");
//				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_JMWAY,"doesn't have a joint distribution");
//				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Value_Type,"doesn't have a value_type");
//				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Marginals,"doesn't have marginals");
				
			}


			//===================================================================================================================================
			// Defintion of the Household/Person selection procedure - can be used for IPF, IPU, etc. methods
			template<typename TargetType, requires(TargetType, check(ComponentType, Concepts::Is_Probabilistic_Selection) && (check(ComponentType, Concepts::Is_IPF_Compatible) || check(ComponentType, Concepts::Is_IPU_Compatible))/* && check(strip_modifiers(TargetType),polaris::Container_Concepts::Is_Associative)*/)>
			void Select_Synthetic_Population_Units(TargetType Region_Sample_Ptr)
			{
				this_component()->Select_Synthetic_Population_Units<TargetType>(Region_Sample_Ptr);
			}

			template<typename TargetType, requires(TargetType, !(check(ComponentType, Concepts::Is_Probabilistic_Selection) || (!check(ComponentType, Concepts::Is_IPF_Compatible) && !check(ComponentType, Concepts::Is_IPU_Compatible))/* || !check(strip_modifiers(TargetType),polaris::Container_Concepts::Is_Associative))*/))>
			void Select_Synthetic_Population_Units(TargetType Region_Sample_Ptr)
			{
				assert_check(ComponentType, Concepts::Is_Probabilistic_Selection,"Not probabilistic selection defined.");
				/*assert_check(TargetType, is_pointer,"Is not a pointer");*/
				//assert_check(strip_modifiers(TargetType), polaris::Container_Concepts::Is_Associative, "Container is not associative.");
			}
			
			//===================================================================================================================================
			// This method creates the actual agent object - if the popsyn is linked to a simulation, otherwise it returns nothing (use this for popsyns which only output the results)
			template<typename TargetType, requires(TargetType, sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Simulator_Usable_Prototype))>
			void Create_Household(TargetType static_properties)
			{
				//=============================================================================================
				#pragma region Define interfaces
				//---------------------------------------------------------------------------------------------
				typedef Network_Components::Prototypes::Network<typename ComponentType::Master_Type::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(Synthetic_Households_Container)> households_itf;
				typedef  Household_Components::Prototypes::Household<get_component_type(households_itf)>  household_itf;
				typedef  Random_Access_Sequence< typename household_itf::get_type_of(Persons_Container)> persons_itf;
				typedef  Person_Components::Prototypes::Person<get_component_type(persons_itf)>  person_itf;
				typedef  Pair_Associative_Container< typename get_type_of(Sample_Data)> sample_itf;
				typedef  Household_Components::Prototypes::Household_Properties <get_mapped_component_type(sample_itf)>  pop_unit_itf;
				typedef  Random_Access_Sequence< typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_itf;
				typedef  Person_Components::Prototypes::Person_Properties <get_component_type(person_sample_itf)>  person_unit_itf;
				#pragma endregion

				// Get the zones synthetic household container
				households_itf* household_container = (households_itf*)this->Synthetic_Households_Container<persons_itf*>();

				// Get an interface to the static household
				pop_unit_itf* pop_unit = (pop_unit_itf*)static_properties;

				// create new household using sample unit
				household_itf* hh=(household_itf*)Allocate<get_component_type(households_itf)>();
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
					person->person_itf::template Household<household_itf*>(hh);
				}
				household_container->push_back(hh);
			}
			
			template<typename TargetType, requires(TargetType, !sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Simulator_Usable_Prototype)), requires(TargetType, sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Synthesis_Usable_Prototype))> 
			void Create_Household(TargetType static_properties)
			{
				typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
				
				typedef  Random_Access_Sequence< typename get_type_of(Synthetic_Households_Container)> households_itf;
				typedef  Household_Components::Prototypes::Household_Properties<get_component_type(get_type_of(Synthetic_Households_Container))>  household_itf;
				
				typedef  Random_Access_Sequence<typename household_itf::get_type_of(Persons_Container)> persons_itf;
				typedef  Person_Components::Prototypes::Person_Properties<typename get_component_type(persons_itf)>  person_itf;

				// interface to the ACS sample data classes
				typedef  Pair_Associative_Container< typename get_type_of(Sample_Data)> sample_itf;
				typedef  Household_Components::Prototypes::Household_Properties <get_mapped_component_type(get_type_of(Sample_Data))>  pop_unit_itf;
				
				
				typedef  Random_Access_Sequence< typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_itf;
				typedef  Person_Components::Prototypes::Person_Properties<get_component_type(pop_unit_itf::get_type_of(Persons_Container))>  person_unit_itf;
				

				households_itf* household_container = (households_itf*)this->Synthetic_Households_Container<households_itf*>();
				//persons_itf* person_container = (persons_itf*)this->Synthetic_Persons_Container<persons_itf*>();

				//// create new household using sample unit
				//pop_unit_itf* pop_unit = (pop_unit_itf*)static_properties;
				//person_sample_itf* person_units = pop_unit->template Persons_Container<person_sample_itf*>();

				//// create new person agent from each person unit properties class in the hh unit properties class
				//typename person_sample_itf::iterator person_unit_itr = person_units->begin();
				//for (;person_unit_itr != person_units->end(); ++person_unit_itr)
				//{
				//	person_unit_itf* person =(person_unit_itf*)(*person_unit_itr);
				//	person_container->push_back((person_itf*)person);
				//}

				household_container->push_back((household_itf*)static_properties);
			}
			
			template<typename TargetType, requires(TargetType, !sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Simulator_Usable_Prototype)), requires(TargetType, !sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Synthesis_Usable_Prototype))> 
			void Create_Household(TargetType static_properties)
			{
				assert_sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Simulator_Usable_Prototype, "The ComponentType must be at least simulation compatible to use this method, or ");
				assert_sub_check(ComponentType, Concepts::Is_Synthesis_Zone, Is_Synthesis_Usable_Prototype, "The ComponentType must be at least simulation compatible to use this method, or ");
			}

			//===================================================================================================================================
			// This method creates the actual agent object - if the popsyn is linked to a simulation, otherwise it returns nothing (use this for popsyns which only output the results)
			template<typename TargetType> void Write_Distribution_Results(ofstream& marginal_outfile, ofstream& joint_outfile)
			{
				this_component()->Write_Distribution_Results<TargetType>(marginal_outfile, joint_outfile);
			}

			//===================================================================================================================================
			// Accessor for the Joint Distribution (IPF and CO only) and marginal distributions used in synthesis
			accessor(Target_Joint_Distribution, NONE, NONE);
			accessor(Target_Marginal_Distribution, NONE, NONE);
			accessor(Test_Marginal_Distribution, NONE, NONE);
			accessor(Target_Person_Joint_Distribution, NONE, NONE);
			accessor(Target_Person_Marginal_Distribution, NONE, NONE);
			accessor(Test_Person_Marginal_Distribution, NONE, NONE);
			accessor(Synthesized_Joint_Distribution, NONE, NONE);
			accessor(Synthesized_Marginal_Distribution, NONE, NONE);
			accessor(Synthesized_Test_Marginal_Distribution, NONE, NONE);
			accessor(Synthesized_Person_Joint_Distribution, NONE, NONE);
			accessor(Synthesized_Person_Marginal_Distribution, NONE, NONE);
			accessor(Synthesized_Test_Person_Marginal_Distribution, NONE, NONE);

			//===================================================================================================================================
			// Accessors for the Synthesized Population (if the component is a zone) or the sample population (if it is a region)
			accessor(Synthetic_Households_Container, NONE, NONE);	// This is the container holding the flyweighted synthesized data (i.e. multiple pointers to single object) created during the synthesis routine
			accessor(Synthetic_Persons_Container, NONE, NONE);		// same as above for the synthetic persons
			accessor(Sample_Data, NONE, NONE);						// This is the container holding the base sample data used in the synthesis routine
			basic_accessor(scenario_reference, NONE, NONE);				
			accessor(network_reference, NONE, NONE);
			basic_accessor(parent_reference, NONE, NONE);					// Reference to the parent region which contains the zone (or the parent popsyn when prototype used for regions)
			accessor(file_linker_reference, NONE, NONE);		
			
			//===================================================================================================================================
			// OPTIONAL: Other accessors that may be of use
			accessor(ID, NONE, NONE);
			accessor(Solver_Settings, NONE, NONE);
			accessor(Activity_Locations_Container, NONE, NONE);
		};

	}
}


