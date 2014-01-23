#pragma once

#include "User_Space_Includes.h"
#include "Synthesis_Zone_Implementations.h"



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
		prototype struct Synthesis_Region : public Synthesis_Zone<ComponentType>
		{	
			tag_as_prototype;
			//==============================================================================================================
			// Load the execution event
			template<typename TargetType> void Initialize()
			{
				this_component()->template Initialize<TargetType>();
				//TODO
//load_event(ComponentType,Call_Synthesize_Population_Conditional,Call_Synthesize_Population, PopSyn::Types::POPSYN_ITERATIONS::MAIN_PROCESS,PopSyn::Types::POPSYN_SUBITERATIONS::PROCESS,NULLTYPE);
			}

			accessor(Temporary_Sample_Data, NONE, NONE);

			//==============================================================================================================
			// This handles the main population synthesis loop on the region-level, calls IPF and Selection for each zone
			template<typename TargetType> void Call_Synthesize_Population_Conditional()
			{
				if (_subiteration() == PopSyn::Types::POPSYN_SUBITERATIONS::PROCESS)
				{
					response.result = true;
					response.next.iteration() = END;
					response.next._subiteration() = 0;
				}
				else
				{
					response.next.iteration() = END;
					response.next._subiteration() = 0;
				}
			}
			declare_event(Call_Synthesize_Population)
			{
				((Synthesis_Region<ComponentType>*) _this)->template Synthesize_Population<NULLTYPE>();
			}
			template<typename TargetType> void Synthesize_Population(requires(TargetType,check(ComponentType,Concepts::Is_IPF_Capable)))
			{
				typedef Prototypes::Synthesis_Zone<ComponentType> base_type;
				base_type* base_itf = (base_type*)this;

				//==========================================================
				#pragma region Define Interfaces to Region distribution, marginals, sample, and zones collections
				//==========================================================
				// Get the solution settings
				typedef typename get_type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Prototypes::Solver_Settings<typename get_type_of(Solver_Settings)> solution_settings_itf;
				typedef Synthesis_Zone<typename remove_pointer<typename get_type_of(Synthesis_Zone_Collection)::value_type>::type> zone_itf;
				typedef Pair_Associative_Container<typename get_type_of(Synthesis_Zone_Collection),zone_itf*> zones_itf;

				typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Joint_Distribution),value_type> mway_itf;
				typedef Multidimensional_Random_Access_Array< typename get_type_of(Target_Marginal_Distribution),value_type> marg_itf;
				typedef  Person_Components::Prototypes::Person_Properties <typename remove_pointer< typename get_type_of(Sample_Data)::value_type>::type>  pop_unit_itf;
				typedef  Pair_Associative_Container< typename get_type_of(Sample_Data), pop_unit_itf*> sample_itf;

				#pragma endregion
				//==========================================================
				// Get the references to the collections from the region
				solution_settings_itf& settings = this->template Solver_Settings<solution_settings_itf&>();			/* Get Solution settings		*/
				zones_itf& zones_collection		= this->template Synthesis_Zone_Collection<zones_itf&>();			/* Zones Collection				*/
				mway_itf& mway					= base_itf->template Target_Joint_Distribution<mway_itf&>();		/* Joint Distribution			*/
				marg_itf& marg					= base_itf->template Target_Marginal_Distribution<marg_itf&>();		/* Marginal Distribution		*/
				sample_itf* sample				= base_itf->template Sample_Data<sample_itf*>();					/* Population Sample Collection	*/

				//======================================================================
				// MAIN SYNTHESIS ROUTINE. 
				//----------------------------------------------------------
				// A. Fit the region distribution to region marginal
				base_itf->template Fit_Joint_Distribution_To_Marginal_Data<NULLTYPE>();

				//----------------------------------------------------------
				// B. Cycle through zones and solve for each
				for (typename zones_itf::iterator zone_itr = zones_collection.begin(); zone_itr != zones_collection.end(); ++zone_itr)
				{
					zone_itf* zone = zone_itr->second;								/* Get interface to zone */
					zone->template Target_Joint_Distribution<mway_itf&>().Copy(mway);		/*1. push region dist to zone */
					zone->template Fit_Joint_Distribution_To_Marginal_Data<NULLTYPE>();		/*2. Fit the zone distribution to the zone marginals */
					zone->template Select_Synthetic_Population_Units<sample_itf*>(sample);	/*3. Select households from regional sample into the zone synthesized sample */
				}	
				cout <<endl<<"Zone " << this->template ID<uint>() <<" is complete.";
			}
			template<typename TargetType> void Synthesize_Population(requires(TargetType,check(ComponentType,!Concepts::Is_IPF_Capable)))
			{
				assert_check(ComponentType,Concepts::Is_IPF_Capable,"Not IPF Capable");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Joint_Distribution_Double,"doesn't have a double joint distribution");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Joint_Distribution_Float,"doesn't have a float joint distribution");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Value_Type,"doesn't have a value_type");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Marginals,"doesn't have marginals");
				
			}
			accessor(Synthesis_Zone_Collection, NONE, NONE);
			accessor(Output_Stream,check_2(strip_modifiers(TargetType),ostream, is_same), check_2(strip_modifiers(TargetType),ostream, is_same));
		};

	}
}
