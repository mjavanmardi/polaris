#pragma once
#include "Dependencies.h"
#include "RNG_Implementations.h"
///


namespace Choice_Model_Components
{
	//==================================================================================================================
	/// Namespace of useful types.  Add any types that convey semantic infomration regarding inputs, outpute, requirements, etc. 
	/// for components in this namespace.  Use well defined typenames that convey the purpose of the Tag
	//------------------------------------------------------------------------------------------------------------------
	namespace Types
	{
		typedef true_type Deterministic_Choice_tag;
		typedef true_type Probabilistic_Choice_tag;
		typedef true_type Utility_Based_Choice_tag;
		typedef true_type Rule_Based_Choice_tag;
		typedef true_type MNL_Model_tag;
		typedef true_type Mixed_Logit_Model_tag;
		typedef true_type Probit_Model_tag;
		typedef true_type Nested_Logit_Model_tag;
		typedef true_type Ordered_Logit_Model_tag;
	}


	//==================================================================================================================
	/// Namespace which holds all the concepts which enforce requirements and restrictions on template members, including components 
	/// List all concepts that may be used by this namespace here
	//------------------------------------------------------------------------------------------------------------------
	namespace Concepts
	{
		concept struct Is_Choice_Option_Prototype
		{
			check_feature(has_calculate_utility, Component_Type::Calculate_Utility);
			check_concept(is_prototype, Is_Polaris_Prototype);
			define_default_check(has_calculate_utility && is_prototype);
		};

		/// EXAMPLE Concept: replace CONCEPT_NAME and CHECK below, and add/remove requirements as necessary
		concept struct Is_Choice_Model
		{
			check_typename_state(Is_Deterministic, Deterministic_Choice_tag, true_type);
			check_typename_state(Is_Probabilistic, Probabilistic_Choice_tag, true_type);
			check_typename_state(Is_Rule_Based, Rule_Based_Choice_tag, true_type);
			check_typename_state(Is_Utility_Based, Utility_Based_Choice_tag, true_type);
			define_sub_check(Is_Utility_Based_Model, Is_Utility_Based && Is_Probabilistic);
			define_sub_check(Is_Rule_Based_Model, Is_Rule_Based && (Is_Deterministic || Is_Probabilistic));
			define_default_check(Is_Utility_Based_Model || Is_Rule_Based_Model)
		};
		concept struct Is_MNL_Model
		{
			check_concept(Valid_Choice_Model, Is_Choice_Model);
			check_typename_state(Is_MNL, MNL_Model_tag, true_type);
			define_default_check(Is_MNL && Valid_Choice_Model);
		};
		concept struct Is_Mixed_Logit_Model
		{
			check_concept(Valid_Choice_Model, Is_Choice_Model);
			check_typename_state(Is_Mixed_Logit, Mixed_Logit_Model_tag, true_type);
			define_default_check(Is_Mixed_Logit && Valid_Choice_Model);
		};
		concept struct Is_Probit_Model
		{
			check_concept(Valid_Choice_Model, Is_Choice_Model);
			check_typename_state(Is_Probit, Probit_Model_tag, true_type);
			define_default_check(Is_Probit && Valid_Choice_Model);
		};
		concept struct Is_Nested_Logit_Model
		{
			check_concept(Valid_Choice_Model, Is_Choice_Model);
			check_typename_state(Is_Nested_Logit, Nested_Logit_Model_tag, true_type);
			define_default_check(Is_Nested_Logit && Valid_Choice_Model);
		};
		concept struct Is_Probabilistic_Prototype
		{
			check_typename_state(is_probabilistic_choice, Component_Type::Probabilistic_Choice_tag, true_type);
			define_default_check(is_probabilistic_choice);
		};
		concept struct Is_Probabilistic
		{
			check_typename_state(is_probabilistic_choice, Probabilistic_Choice_tag, true_type);
			check_concept(is_prototype, Is_Probabilistic_Prototype);
			define_default_check(is_prototype || is_probabilistic_choice);
		};
	}


	//==================================================================================================================
	/// Namespace which includes the interface(or interfaces if necessary) which links to the various Bases regarding the transportation object
	//------------------------------------------------------------------------------------------------------------------
	namespace Prototypes
	{
		// use this to call the calculate utility method for types in TList's passed to features
		define_feature_dispatcher(Calculate_Utility, calculate_utility_dispatcher);
		define_feature_dispatcher(Print_Utility, print_utility_dispatcher);
		prototype struct Choice_Model ADD_DEBUG_INFO
		{
			tag_as_prototype;

			

			feature_prototype void Initialize()
			{
				define_simple_container_interface(choice_options_itf,typename get_type_of(choice_options),Containers::Random_Access_Sequence_Prototype,void*,ComponentType);
				define_simple_container_interface(utilities_itf,typename get_type_of(choice_utilities),Containers::Random_Access_Sequence_Prototype,float,ComponentType);
				define_simple_container_interface(probabilities_itf,typename get_type_of(choice_probabilities),Containers::Random_Access_Sequence_Prototype,float,ComponentType);
				choice_options_itf* options = this->choice_options<choice_options_itf*>();
				utilities_itf* util = this->choice_utilities<utilities_itf*>();
				probabilities_itf* prob = this->choice_probabilities<probabilities_itf*>();
				options->clear();
				util->clear();
				prob->clear();
			}

			feature_prototype void Add_Choice_Option(TargetType choice_option, requires(check(TargetType, Concepts::Is_Choice_Option_Prototype) && check_as_given(TargetType,is_pointer)))
			{
				// Validate that TargetType is in AvailableTypes
				if (IndexOf<typename Component_Type::TList,strip_modifiers(TargetType) >::value < 0) THROW_EXCEPTION("ERROR: TargetType is not a member of AvailableTypes TypeList.");

				// Push item into vector as anonymous
				define_simple_container_interface(choice_options_itf,typename get_type_of(choice_options),Containers::Random_Access_Sequence_Prototype,void*,ComponentType);
				choice_options_itf* options = this->choice_options<choice_options_itf*>();
				options->push_back((void*)choice_option);				
			}
			feature_prototype void Add_Choice_Option(TargetType choice_option, requires(!check(TargetType, Concepts::Is_Choice_Option_Prototype) || !check_as_given(TargetType,is_pointer)))
			{
				assert_sub_check(TargetType,Concepts::Is_Choice_Option_Prototype, has_calculate_utility,  "TargetType does not have Calculate_Utility feature.");
				assert_sub_check(TargetType,Concepts::Is_Choice_Option_Prototype, is_prototype,  "TargetType is not a valid prototype.");
			}
						
			/// EVALUATE THE AVAILABLE CHOICES (i.e. CALCULATE UTILITY, SET PROBABILITIES, ETC.)
			feature_prototype void Evaluate_Choices(requires(check(ComponentType, Concepts::Is_MNL_Model)))
			{	
				define_simple_container_interface(choice_options_itf,typename get_type_of(choice_options),Containers::Random_Access_Sequence_Prototype,void*,ComponentType);
				define_simple_container_interface(utilities_itf,typename get_type_of(choice_utilities),Containers::Random_Access_Sequence_Prototype,double,ComponentType);
				define_simple_container_interface(probabilities_itf,typename get_type_of(choice_probabilities),Containers::Random_Access_Sequence_Prototype,double,ComponentType);

				// Local type definition option
				choice_options_itf* choices =	this->choice_options<choice_options_itf*>();
				utilities_itf*		utils =		this->choice_utilities<utilities_itf*>();
				probabilities_itf*	probs =		this->choice_probabilities<probabilities_itf*>();
				typename choice_options_itf::iterator	itr = choices->begin();
				typename utilities_itf::iterator			u_itr = utils->begin();
				generic_prototype<generic_implementation<NT>>* choice;

				double u, p;
				double utility_sum = 0;

				
				for (itr; itr!= choices->end(); itr++)
				{
					//cout << endl << "Option " << i <<": ";
					choice = (generic_prototype<generic_implementation<NT>>*)(*itr);
					//u = choice->Calculate_Utility<float>();
					u = dispatch_to_feature(calculate_utility_dispatcher,typename Component_Type::TList,choice,Target_Type<double,double>);
					//cout << ", utility = " << u;
					utils->push_back(u);
					utility_sum = utility_sum + exp(u);
				}

				// if sum(exp(u)) = 0 then no valid choices can be made as all options have -ininity utility
				if (utility_sum == 0)
				{
					THROW_WARNING("WARNING: all options have zero probability [sum(exp(u))=0], unable to evaluate choices.  Num choices was=" << choices->size()); 
					// print all choices
					int i = 0;
					for (itr; itr!= choices->end(); itr++, i++)
					{
						cout << endl << "Option " << i <<": ";
						choice = (generic_prototype<generic_implementation<NT>>*)(*itr);
						u = dispatch_to_feature(print_utility_dispatcher,typename Component_Type::TList,choice,Target_Type<double,double>);
					}
					choices->clear();
					probs->clear();
					utils->clear();
					return;
				}

				for (u_itr=utils->begin(); u_itr!= utils->end(); u_itr++)
				{
					u = *u_itr;
					p = exp(u)/utility_sum;
					if (ISNAN(p)){ THROW_WARNING("ERROR: p is not a number. U=" << u << ", exp(u)="<<exp(u) << ", u_sum="<<utility_sum); probs->push_back(0.0);}
					else probs->push_back(p);
				}

			}
			feature_prototype void Evaluate_Choices(requires(!check(ComponentType, Concepts::Is_MNL_Model)))
			{
				assert_check(ComponentType, Concepts::Is_MNL_Model, "ComponentType is not an MNL or Rule-based model");
			}
			
			/// SELECT FROM THE AVAILABLE CHOICES FOR SIMULATION
			feature_prototype TargetType Choose(int& selected_index, requires(check(ComponentType, Concepts::Is_Probabilistic)))
			{
				define_simple_container_interface(choice_options_itf,typename get_type_of(choice_options),Containers::Random_Access_Sequence_Prototype,void*,ComponentType);
				define_simple_container_interface(probabilities_itf,typename get_type_of(choice_probabilities),Containers::Random_Access_Sequence_Prototype,double,ComponentType);

				// Local type definition option
				choice_options_itf* choices = this->choice_options<choice_options_itf*>();
				probabilities_itf* probs = this->choice_probabilities<probabilities_itf*>();
				typename probabilities_itf::iterator p_itr = probs->begin(); 

				double cumulative_probability = 0;
				
				double rand = Uniform_RNG.Next_Rand<double>();

				int i = 0;
				for (typename choice_options_itf::iterator itr = choices->begin(); itr != choices->end(); ++itr, ++p_itr)
				{
					cumulative_probability += *p_itr;
					if (ISNAN(cumulative_probability)) THROW_WARNING("WARNING: p is not a number. p=" << *p_itr << ", cum_p="<<cumulative_probability << ", index="<<i);
					if (rand < cumulative_probability) 
					{
						selected_index = i;
						return (TargetType)(*itr);
					}
					i++;
				}
				
				THROW_WARNING("WARNING: No choice selected from choice model (prob, cumulative_prob, selected_index) "<<rand << ", " << cumulative_probability << ", " << selected_index);
				return nullptr;
			}
			feature_prototype TargetType Choose(int& selected_index,requires(!check(ComponentType, Concepts::Is_Probabilistic)))
			{
				assert_check(ComponentType, Concepts::Is_Probabilistic, "ComponentType does not specify if Choice is Deterministic or Probabilistic");
			}
		 
			feature_prototype TargetType Choice_At(int selected_index)
			{
				define_simple_container_interface(choice_options_itf,typename get_type_of(choice_options),Containers::Random_Access_Sequence_Prototype,void*,ComponentType);

				// Local type definition option
				choice_options_itf* choices = this->choice_options<choice_options_itf*>();

				return (TargetType)choices->at(selected_index);
			}
			/// ACCESSORS
			feature_accessor(choice_options,none,none);
			feature_accessor(choice_utilities,none,none);
			feature_accessor(choice_probabilities,none,none);
		};
	}
}