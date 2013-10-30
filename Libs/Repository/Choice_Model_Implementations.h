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
		implementation struct Choice_Model_Implementation : public Polaris_Component<APPEND_CHILD(Choice_Model_Implementation),NT,Data_Object>
		{
			member_container(vector<void*>,choice_options, none, none);
			member_container(vector<double>,choice_utilities, none, none);
			member_container(vector<double>,choice_probabilities, none, none);
		};
		implementation struct MNL_Model_Implementation : public Polaris_Component<APPEND_CHILD(Choice_Model_Implementation),NT,Data_Object>
		{
			typedef true_type MNL_Model_tag;
			typedef true_type Probabilistic_Choice_tag;
			typedef true_type Utility_Based_Choice_tag;

			member_container(vector<void*>,choice_options, none, none);
			member_container(vector<double>,choice_utilities, none, none);
			member_container(vector<double>,choice_probabilities, none, none);
		};
	}
}