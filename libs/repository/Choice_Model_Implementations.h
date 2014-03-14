#pragma once

#include "Choice_Model_Prototypes.h"


namespace Choice_Model_Components
{
	//==================================================================================================================
	/// Namespace of Base classes related to the Interface(s) from the component namespace.
	/// The bases are combined with the interfaces to create components by adding data.
	//------------------------------------------------------------------------------------------------------------------
	namespace Implementations
	{
		implementation struct Choice_Option_Base : public Polaris_Component<MT,INHERIT(Choice_Option_Base),Data_Object>
		{
			virtual double Calculate_Utility() = 0;
			virtual void Print_Utility() = 0;
		};

		implementation struct Choice_Model_Implementation : public Polaris_Component<MT,INHERIT(Choice_Model_Implementation),Data_Object>
		{
			m_container(std::vector<Prototypes::Choice_Option<Choice_Option_Base<MT>>*>,choice_options, NONE, NONE);
			m_container(std::vector<double>,choice_utilities, NONE, NONE);
			m_container(std::vector<double>,choice_probabilities, NONE, NONE);
		};
		implementation struct MNL_Model_Implementation : public Polaris_Component<MT,INHERIT(MNL_Model_Implementation),Data_Object>
		{
			typedef true_type MNL_Model_tag;
			typedef true_type Probabilistic_Choice_tag;
			typedef true_type Utility_Based_Choice_tag;

			m_container(std::vector<void*>,choice_options, NONE, NONE);
			m_container(std::vector<double>,choice_utilities, NONE, NONE);
			m_container(std::vector<double>,choice_probabilities, NONE, NONE);
		};
	}
}
