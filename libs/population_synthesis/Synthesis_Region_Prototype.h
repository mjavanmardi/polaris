#pragma once

#include "Population_Synthesis_Includes.h"
#include "Synthesis_Zone_Implementations.h"


using namespace std;

//---------------------------------------------------------
//	SUMMARY FILE CLASS
//---------------------------------------------------------
namespace PopSyn
{
	namespace Prototypes
	{
		prototype struct Synthesis_Region : public Synthesis_Zone<ComponentType>
		{	
			tag_as_prototype;

			template<typename IndexType> void Initialize(IndexType dimensions_hh, IndexType dimensions_per)
			{
				this_component()->template Init<NT>();
				this_component()->template Initialize<IndexType>(dimensions_hh, dimensions_per);
				((ComponentType*)this)->Load_Event<ComponentType>(&Synthesize_Population_Conditional_Event,POPSYN_ITERATIONS::MAIN_INITIALIZE,POPSYN_SUBITERATIONS::PROCESS);		
			}

			void Validate_Addresses()
			{
				this_component()->Validate_Addresses();
			}

			accessor(Temporary_Sample_Data, NONE, NONE);
			accessor(Synthesis_Zone_Collection, NONE, NONE);
			accessor(Output_Stream,check_2(strip_modifiers(TargetType),ostream, is_same), check_2(strip_modifiers(TargetType),ostream, is_same));

			//==============================================================================================================
			// This handles the main population synthesis loop on the region-level, calls IPF and Selection for each zone
			static void Synthesize_Population_Conditional_Event(ComponentType* _this,Event_Response& response)
			{
				if (sub_iteration() == PopSyn::Types::POPSYN_SUBITERATIONS::PROCESS)
				{
					((Synthesis_Region<ComponentType>*) _this)->template Synthesize_Population<NT>();
					response.next._iteration = END;
					response.next._sub_iteration = 0;
				}
				else
				{
					response.next._iteration = END;
					response.next._sub_iteration = 0;
				}
			}

			template<typename TargetType> void Synthesize_Population(requires(TargetType,check_stripped_type(ComponentType,Concepts::Is_IPF_Compatible)))
			{
				typedef Prototypes::Synthesis_Zone<ComponentType> base_type;
				base_type* base_itf = (base_type*)this;

				//==========================================================
				#pragma region Define Interfaces to Region distribution, marginals, sample, and zones collections
				//==========================================================
				// Get the solution settings
				typedef typename get_type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Prototypes::Solver_Settings<typename get_type_of(Solver_Settings)> solution_settings_itf;
				typedef Pair_Associative_Container<typename get_type_of(Synthesis_Zone_Collection)> zones_itf;
				typedef Synthesis_Zone<get_mapped_component_type(typename get_type_of(Synthesis_Zone_Collection))> zone_itf;

				typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Joint_Distribution),value_type> mway_itf;
				typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Marginal_Distribution),value_type> marg_itf;
				typedef  Pair_Associative_Container< typename get_type_of(Sample_Data)> sample_itf;
				typedef  Person_Components::Prototypes::Person_Properties <get_mapped_component_type(typename get_type_of(Sample_Data))>  pop_unit_itf;

				#pragma endregion


				//==========================================================
				// Get the references to the collections from the region
				solution_settings_itf& settings = this->template Solver_Settings<solution_settings_itf&>();				/* Get Solution settings		*/
				zones_itf& zones_collection		= this->template Synthesis_Zone_Collection<zones_itf&>();				/* Zones Collection				*/
				mway_itf& mway					= base_itf->template Target_Joint_Distribution<mway_itf&>();			/* Joint Distribution			*/
				marg_itf& marg					= base_itf->template Target_Marginal_Distribution<marg_itf&>();			/* Marginal Distribution		*/
				mway_itf& mway_per				= base_itf->template Target_Person_Joint_Distribution<mway_itf&>();		/* Joint Distribution			*/
				marg_itf& marg_per				= base_itf->template Target_Person_Marginal_Distribution<marg_itf&>();	/* Marginal Distribution		*/
				sample_itf* sample				= base_itf->template Sample_Data<sample_itf*>();						/* Population Sample Collection	*/


				//======================================================================
				// MAIN SYNTHESIS ROUTINE. 
				//----------------------------------------------------------
				// A. Fit the region distribution to region marginal for the household (true) and person (false) levels
				base_itf->template Fit_Joint_Distribution_To_Marginal_Data<NULLTYPE>(true);
				base_itf->template Fit_Joint_Distribution_To_Marginal_Data<NULLTYPE>(false);

				//----------------------------------------------------------
				// B. Cycle through zones and solve for each
				for (typename zones_itf::iterator zone_itr = zones_collection.begin(); zone_itr != zones_collection.end(); ++zone_itr)
				{
					zone_itf* zone = zone_itr->second;												/* Get interface to zone */
					zone->template Target_Joint_Distribution<mway_itf&>().Copy(mway);				/*1a. push region distribution to zone */
					zone->template Target_Person_Joint_Distribution<mway_itf&>().Copy(mway_per);	/*1b. push region distribution to zone */
					zone->template Fit_Joint_Distribution_To_Marginal_Data<NULLTYPE>(true);			/*2a. Fit the zone hh distribution to the zone marginals */
					zone->template Fit_Joint_Distribution_To_Marginal_Data<NULLTYPE>(false);		/*2a. Fit the zone person distribution to the zone marginals */
					zone->template Select_Synthetic_Population_Units<sample_itf*>(sample);			/*3. Select households from regional sample into the zone synthesized sample */

				}	
				cout <<endl<<"Region " << this->template ID<uint>() <<" is complete."<<endl;
			}

			template<typename TargetType> void Synthesize_Population(requires(TargetType,check_stripped_type(ComponentType,Concepts::Is_Combinatorial_Optimization_Compatible)))
			{
				typedef Prototypes::Synthesis_Zone<ComponentType> base_type;
				base_type* base_itf = (base_type*)this;

				//==========================================================
				#pragma region Define Interfaces to Region distribution, marginals, sample, and zones collections
				//==========================================================
				// Get the solution settings
				typedef typename get_type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Prototypes::Solver_Settings<typename get_type_of(Solver_Settings)> solution_settings_itf;
				typedef Pair_Associative_Container<typename get_type_of(Synthesis_Zone_Collection)> zones_itf;
				typedef Synthesis_Zone<get_mapped_component_type(typename get_type_of(Synthesis_Zone_Collection))> zone_itf;

				//typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Joint_Distribution),value_type> mway_itf;
				//typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Marginal_Distribution),value_type> marg_itf;
				typedef  Pair_Associative_Container< typename get_type_of(Sample_Data)> sample_itf;
				typedef  Person_Components::Prototypes::Person_Properties <get_mapped_component_type(typename get_type_of(Sample_Data))>  pop_unit_itf;

				#pragma endregion
				//==========================================================
				// Get the references to the collections from the region
				solution_settings_itf& settings = this->template Solver_Settings<solution_settings_itf&>();			/* Get Solution settings		*/
				zones_itf& zones_collection		= this->template Synthesis_Zone_Collection<zones_itf&>();			/* Zones Collection				*/
				sample_itf* sample				= base_itf->template Sample_Data<sample_itf*>();					/* Population Sample Collection	*/

				//======================================================================
				// MAIN SYNTHESIS ROUTINE. - WRITE THE HIGH LEVEL CO ROUTINE HERE
				//----------------------------------------------------------
				// A. Fit the region distribution to region marginal

				// DO STUFF AT REGION LEVEL

				//----------------------------------------------------------
				// B. Cycle through zones and solve for each
				for (typename zones_itf::iterator zone_itr = zones_collection.begin(); zone_itr != zones_collection.end(); ++zone_itr)
				{
					// DO STUFF AT ZONAL LEVEL
				}	
				cout <<endl<<"Zone " << this->template ID<uint>() <<" is complete.";
			}

			template<typename TargetType> void Synthesize_Population(requires(TargetType,check(ComponentType,!Concepts::Is_IPF_Compatible) && check(ComponentType,!Concepts::Is_Combinatorial_Optimization_Compatible)))
			{
				assert_check(ComponentType,Concepts::Is_IPF_Compatible,"Not IPF Capable");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Compatible,Has_MWAY,"doesn't have a joint distribution");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Compatible,Has_Value_Type,"doesn't have a value_type");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Compatible,Has_Marginals,"doesn't have marginals");
				
			}
			
			
		};

	}
}
