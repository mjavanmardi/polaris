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
				this_component()->template Initialize<IndexType>(dimensions_hh, dimensions_per,0,0);
				((ComponentType*)this)->template Load_Event<ComponentType>(&Synthesize_Population_Conditional_Event,POPSYN_ITERATIONS::MAIN_INITIALIZE,POPSYN_SUBITERATIONS::PROCESS);
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

			//==============================================================================================================
			// These are the high level population synthesis algorithm at the region-level, which do the fitting and selection for each zone in the region
			template<typename TargetType> void Synthesize_Population(requires(TargetType,check_stripped_type(ComponentType,Concepts::Is_IPF_Compatible) || check_stripped_type(ComponentType,Concepts::Is_IPU_Compatible)))
			{
				this_component()->Synthesize_Population<TargetType>();
			}
			
			template<typename TargetType> void Synthesize_Population(requires(TargetType,check(ComponentType,!Concepts::Is_IPF_Compatible) && check(ComponentType,!Concepts::Is_IPU_Compatible)))
			{
				assert_check(ComponentType,Concepts::Is_IPF_Compatible,"Not IPF Capable...");
				assert_check(ComponentType,Concepts::Is_IPU_Compatible,".. and, Not IPU Capable");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Compatible,Has_MWAY,"doesn't have a joint distribution");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Compatible,Has_Value_Type,"doesn't have a value_type");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Compatible,Has_Marginals,"doesn't have marginals");
				
			}
			
			
		};

	}
}
