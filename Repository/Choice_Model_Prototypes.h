#pragma once
#include "Repository_Includes.h"

//==================================================================================================================
/// EXAMPLE Component namespace -	used to organize similar high level transportation objects, i.e. Traveler, Vehicle,
///									Link, etc. which may have many different implementations depending on usage
///
/// Each component namespace consists of 6 standard sub-namespaces across two files.  The namespaces and files are:
///
///	1. [COMPONENT_NAME]_Interfaces.h file:
///		a.) Types namespace
///		b.) Concepts namespace
///		c.) Interfaces namespace
///		d.) Data Structures namespace
///
///	2. [COMPONENT_NAME]_Bases.h file:
///		e.) Bases namespace
///		f.) Components namespace
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
	}


	//==================================================================================================================
	/// Namespace which includes the interface(or interfaces if necessary) which links to the various Bases regarding the transportation object
	//------------------------------------------------------------------------------------------------------------------
	namespace Prototypes
	{
		prototype struct Choice_Option_Prototype
		{
			tag_as_prototype;

			/// INTERFACE FUNCTION EXAMPLE - this example dispatched the Initialize function call to the component base
			feature void Initialize()
			{
				this_component()->Initialize<CallerType,TargetType>();
			}

			feature void Evaluate_Probability(requires(check(ComponentType, Concepts::Choice_Is_Rule_Based)))
			{
				// Dispatch to probability rules written in base
				TargetType prob = this_component()->Evaluate_Probability<CallerType,TargetType>();
				this->probability<TargetType>(prob);
			}

			feature void Evaluate_Utility(call_requirements(requires(ComponentType, Concepts::Choice_Is_Utility_Based)))
			{
				TargetType util = this_component()->Evaluate_Utility<CallerType,TargetType>();
				this->utility<TargetType>(util);
			}
			 
			feature_accessor(choice_data_interface);

			feature_accessor(utility);

			feature_accessor(probability);
		};


		//------------------------------------------------------------------------------------------------------------------
		/// RENAME THE Inteface struct below.  This is the inteface to a POLARIS component
		//------------------------------------------------------------------------------------------------------------------
		prototype struct Choice_Model_Prototype
		{
			tag_as_prototype;
			define_container_and_value_interface(_choices_interface,_choice_interface,type_of(choice_options),Polaris_Back_Insertion_Sequence_Prototype,Choice_Option_Prototype,ComponentType);

			/// BASIC INITIALIZER
			feature void Initialize()
			{
				return this_component()->Initialize<CallerType,TargetType>();
			}

			feature void Add_Choice_Option(TargetType new_choice_option_data, call_requires(strip_modifiers(TargetType), Is_Polaris_Component))
			{
				_choices_interface* choices = this->choice_options<_choices_interface*>();
				_choice_interface* choice = (_choice_interface*)Allocate<_choice_interface_type>();
				choice->choice_data_interface<TargetType>(new_choice_option_data);
				choices->push_back(choice);
			}
			feature void Add_Choice_Option(TargetType new_choice_option_data, call_requires(strip_modifiers(TargetType), !Is_Polaris_Component))
			{
				assert_requirements(TargetType,Is_Polaris_Component, "TargetType is not a polaris component.");
			}

			/// SELECT FROM THE AVAILABLE CHOICES FOR SIMULATION
			feature Choice_Option_Prototype<TargetType>* Make_Choice(call_requirements(requires(ComponentType, Concepts::Choice_Is_Deterministic)))
			{	
				// Local type definition option
				_choices_interface* choices = this->choice_options<_choices_interface*>();
				_choice_interface* choice;
				_choices_interface::iterator itr = choices->begin();

				float cumulative_probability = 0;
				float rand = this->random_probability_value<float>();
				
				for (itr; itr != choices->end(); itr++)
				{
					choice = (*itr);
					cumulative_probability += choice->probability<float>();
					if (rand < cumulative_probability) return choice;
				}

			}
			feature Choice_Option_Prototype<TargetType>* Make_Choice(call_requirements(requires(ComponentType, Concepts::Choice_Is_Probabilistic)))
			{
				// Local type definition option
				_choices_interface* choices = this->choice_options<_choices_interface*>();
				_choice_interface* choice;
				_choices_interface::iterator itr = choices->begin();

				float cumulative_probability = 0;
				float rand = this->random_probability_value<float>();
				
				for (itr; itr != choices->end(); itr++)
				{
					choice = (*itr);
					cumulative_probability += choice->probability<float>();
					if (rand < cumulative_probability) return choice;
				}
			}
			feature Choice_Option_Prototype<TargetType>* Make_Choice(call_requirements(!(requires(ComponentType, Concepts::Choice_Is_Deterministic) || requires(ComponentType, Concepts::Choice_Is_Probabilistic))))
			{
				assert_requirements(ComponentType, Concepts::Choice_Is_Deterministic, "ComponentType does not specify if Choice is Deterministic or Probabilistic");
			}
			
				
			/// EVALUATE THE AVAILABLE CHOICES (i.e. CALCULATE UTILITY, SET PROBABILITIES, ETC.)
			feature void Evaluate_Choices(call_requirements(
				requires(ComponentType, Concepts::Is_MNL_Model) &&
				requires(typename ComponentType::choice_options_type::unqualified_value_type, Concepts::Choice_Is_Utility_Based) &&
				requires(TargetType, is_arithmetic)))
			{	
				// Local type definition option
				_choices_interface* choices = this->choice_options<_choices_interface*>();
				_choice_interface* choice;
				_choices_interface::iterator itr = choices->begin();

				typename TargetType utility_sum = 0;

				for (itr; itr!= choices->end(); itr++)
				{
					choice = (*itr);
					choice->Evaluate_Utility<TargetType>();
					utility_sum = utility_sum + choice->utility<TargetType>();
				}
				for (itr=choices->begin(); itr!= choices->end(); itr++)
				{
					choice = (*itr);
					TargetType util = choice->utility<TargetType>();
					choice->probability<TargetType>(util/utility_sum);
					cout << endl << "Choice Probability: "<<choice->probability<TargetType>();
				}

			}
			feature void Evaluate_Choices(call_requirements(
				requires(ComponentType, Concepts::Choice_Is_Rule_Based) &&
				requires(TargetType, is_arithmetic)))
			{
				// Local type definition option
				_choices_interface* choices = this->choice_options<_choices_interface*>();
				_choice_interface* choice;
				_choices_interface::iterator itr = choices->begin();

				//typename TargetType::ReturnType utility_sum = 0;

				//for (itr; itr!= choices->end(); itr++)
				//{
				//	choice = (*itr);
				//	choice->Evaluate_Utility<typename TargetType::ReturnType>();
				//	utility_sum = utility_sum + choice->utility<typename TargetType::ReturnType>();
				//}
				//for (itr; itr!= choices->end(); itr++)
				//{
				//	choice = (*itr);
				//	typename TargetType::ReturnType util = choice->utility<typename TargetType::ReturnType>();
				//	choice->probability<typename TargetType::ReturnType>(util/utility_sum);
				//}
			}
			feature void Evaluate_Choices(call_requirements(!(
				(requires(ComponentType, Concepts::Is_MNL_Model) || requires(ComponentType, Concepts::Choice_Is_Rule_Based)) &&
				requires(TargetType, is_arithmetic))))
			{
				assert_requirements(ComponentType, Concepts::Is_MNL_Model, "ComponentType is not an MNL or Rule-based model");
				assert_requirements(TargetType, Is_Target_Type_Struct, "TargetType is not a valid Target_Type struct.");
				assert_requirements_std(typename TargetType::ReturnType,is_arithmetic, "TargetType::ReturnType is not an arithmetic type.");
				assert_requirements(typename TargetType::ParamType,Is_Polaris_Component,"TargetType::ParamType is not a valid Polaris Component Type.");
			}

			 
			/// ACCESSORS
			feature_accessor(chooser);
			feature_accessor(generator);
			feature_accessor(choice_options);
			feature_accessor(random_probability_value);
		};

	}


	//==================================================================================================================
	/// Namespace of useful related data structures for the components
	//------------------------------------------------------------------------------------------------------------------
	namespace Data_Structures
	{
		typedef float Data_Type;
		polaris_variable(Name, Data_Type, Type_Trait_1, Type_Trait_2, Type_Trait_3);
	}
}