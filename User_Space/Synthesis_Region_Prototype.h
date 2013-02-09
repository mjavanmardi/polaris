#pragma once

#include "User_Space.h"


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
			//==============================================================================================================
			// Load the execution event
			feature_prototype void Initialize()
			{
				load_event(ComponentType,Call_Synthesize_Population_Conditional,Call_Synthesize_Population,POPSYN_ITERATIONS::MAIN_PROCESS,POPSYN_SUBITERATIONS::PROCESS,NULLTYPE);
			}

			//==============================================================================================================
			// This handles the main population synthesis loop on the region-level, calls IPF and Selection for each zone
			declare_feature_conditional(Call_Synthesize_Population_Conditional)
			{
				if (_sub_iteration == POPSYN_SUBITERATIONS::PROCESS)
				{
					response.result = true;
					response.next._iteration = END;
					response.next._sub_iteration = 0;
				}
				else
				{
					response.next._iteration = END;
					response.next._sub_iteration = 0;
				}
			}
			declare_feature_event(Call_Synthesize_Population)
			{
				((Synthesis_Region_Prototype<ComponentType,CallerType>*) _this)->Synthesize_Population<NULLTYPE>();
			}
			feature_prototype void Synthesize_Population(requires(check(ComponentType,Concepts::Is_IPF_Capable)))
			{
				//==========================================================
				#pragma region Define Interfaces to Region distribution, marginals, sample, and zones collections
				//==========================================================
				// Get the solution settings
				typedef get_type_of(Target_Joint_Distribution)::unqualified_value_type value_type;
				define_component_interface(solution_settings_itf,get_type_of(Solver_Settings),Prototypes::Solver_Settings_Prototype,NULLTYPE);
				define_container_and_value_interface(zones_itf,zone_itf,get_type_of(Synthesis_Zone_Collection),Containers::Associative_Container_Prototype,Synthesis_Zone_Prototype,NULLTYPE);	
				define_simple_container_interface(mway_itf, get_type_of(Target_Joint_Distribution),Multidimensional_Random_Access_Array_Prototype,value_type,NULLTYPE);
				define_simple_container_interface(marg_itf, get_type_of(Target_Marginal_Distribution),Multidimensional_Random_Access_Array_Prototype,value_type,NULLTYPE);		
				define_container_and_value_interface(sample_itf, pop_unit_itf, get_type_of(Sample_Data), Associative_Container_Prototype, PopSyn::Prototypes::Population_Unit_Prototype ,NULLTYPE);			
				#pragma endregion
				//==========================================================
				// Get the references to the collections from the region
				solution_settings_itf& settings = this->Solver_Settings<solution_settings_itf&>();	/* Get Solution settings		*/
				zones_itf& zones_collection		= this->Synthesis_Zone_Collection<zones_itf&>();	/* Zones Collection				*/
				mway_itf& mway					= Target_Joint_Distribution<mway_itf&>();			/* Joint Distribution			*/
				marg_itf& marg					= Target_Marginal_Distribution<marg_itf&>();		/* Marginal Distribution		*/
				sample_itf* sample				= Sample_Data<sample_itf*>();						/* Population Sample Collection	*/

				//======================================================================
				// MAIN SYNTHESIS ROUTINE. 
				//----------------------------------------------------------
				// A. Fit the region distribution to region marginal
				Synthesis_Zone_Prototype<ComponentType,CallerType>::Fit_Joint_Distribution_To_Marginal_Data<NULLTYPE>();
				//----------------------------------------------------------
				// B. Cycle through zones and solve for each
				for (zones_itf::iterator zone_itr = zones_collection.begin(); zone_itr != zones_collection.end(); ++zone_itr)
				{
					zone_itf* zone = zone_itr->second;								/* Get interface to zone */
					zone->Target_Joint_Distribution<mway_itf&>().Copy(mway);		/*1. push region dist to zone */
					zone->Fit_Joint_Distribution_To_Marginal_Data<NULLTYPE>();		/*2. Fit the zone distribution to the zone marginals */
					zone->Select_Synthetic_Population_Units<sample_itf*>(sample);	/*3. Select households from regional sample into the zone synthesized sample */
				}	
				cout <<endl<<"Zone " << this->ID<uint>() <<" is complete.";
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
			feature_accessor(Output_Stream,check_2(strip_modifiers(ReturnValueType),ostream, is_same), check_2(strip_modifiers(SetValueType),ostream, is_same));
		};

	}
}