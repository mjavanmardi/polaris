#pragma once
#include "Repository_Includes.h"
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

    namespace Implementations
	{
		implementation struct Choice_Option_Base;
		implementation struct Nested_Choice_Option_Base;
	}

	//==================================================================================================================
	/// Namespace which holds all the concepts which enforce requirements and restrictions on template members, including components 
	/// List all concepts that may be used by this namespace here
	//------------------------------------------------------------------------------------------------------------------
	namespace Concepts
	{
		concept struct Is_Choice_Option_Prototype
		{
			check_method_name(has_calculate_utility, Component_Type::Calculate_Utility);
			check_concept(is_prototype, Is_Prototype, T, V);
			define_default_check(has_calculate_utility && is_prototype);
		};

		/// EXAMPLE Concept: replace CONCEPT_NAME and CHECK below, and add/remove requirements as necessary
		concept struct Is_Choice_Model
		{
			check_typedef_type(Is_Deterministic, Deterministic_Choice_tag, true_type);
			check_typedef_type(Is_Probabilistic, Probabilistic_Choice_tag, true_type);
			check_typedef_type(Is_Rule_Based, Rule_Based_Choice_tag, true_type);
			check_typedef_type(Is_Utility_Based, Utility_Based_Choice_tag, true_type);
			define_sub_check(Is_Utility_Based_Model, Is_Utility_Based && Is_Probabilistic);
			define_sub_check(Is_Rule_Based_Model, Is_Rule_Based && (Is_Deterministic || Is_Probabilistic));
			define_default_check(Is_Utility_Based_Model || Is_Rule_Based_Model)
		};
		concept struct Is_Utility_Based_Model
		{
			check_typedef_type(Is_Probabilistic, Probabilistic_Choice_tag, true_type);
			check_typedef_type(Is_Utility_Based, Utility_Based_Choice_tag, true_type);
			define_default_check(Is_Utility_Based && Is_Probabilistic)
		};
		concept struct Is_MNL_Model
		{
			check_concept(Valid_Choice_Model, Is_Choice_Model, T, V);
			check_typedef_name(Is_MNL, MNL_Model_tag);
			define_default_check(Is_MNL && Valid_Choice_Model);
		};
		concept struct Is_Mixed_Logit_Model
		{
			check_concept(Valid_Choice_Model, Is_Choice_Model, T, V);
			check_typedef_type(Is_Mixed_Logit, Mixed_Logit_Model_tag, true_type);
			define_default_check(Is_Mixed_Logit && Valid_Choice_Model);
		};
		concept struct Is_Probit_Model
		{
			check_concept(Valid_Choice_Model, Is_Choice_Model, T, V);
			check_typedef_type(Is_Probit, Probit_Model_tag, true_type);
			define_default_check(Is_Probit && Valid_Choice_Model);
		};
		concept struct Is_Nested_Logit_Model
		{
			check_concept(Valid_Choice_Model, Is_Choice_Model, T, V);
			check_typedef_type(Is_Nested_Logit, Nested_Logit_Model_tag, true_type);
			define_default_check(Is_Nested_Logit && Valid_Choice_Model);
		};
		concept struct Is_Logit_Model
		{
			check_concept(MNL_Model, Is_MNL_Model, T, V);
			check_concept(Mixed_Logit_Model, Is_Mixed_Logit_Model, T, V);
			check_concept(Nested_Logit_Model, Is_Nested_Logit_Model, T, V);
			define_default_check(MNL_Model || Mixed_Logit_Model || Nested_Logit_Model);
		};
		concept struct Is_Probabilistic_Prototype
		{
			check_typedef_name(is_probabilistic_choice, Component_Type::Probabilistic_Choice_tag);
			define_default_check(is_probabilistic_choice);
		};
		concept struct Is_Probabilistic
		{
			check_typedef_name(is_probabilistic_choice, Probabilistic_Choice_tag);
			check_concept(is_prototype, Is_Probabilistic_Prototype, T, V);
			define_default_check(is_prototype || is_probabilistic_choice);
		};
	}


	//==================================================================================================================
	/// Namespace which includes the interface(or interfaces if necessary) which links to the various Bases regarding the transportation object
	//------------------------------------------------------------------------------------------------------------------
	namespace Prototypes
	{
		prototype struct Choice_Option;

		prototype struct Choice_Model ADD_DEBUG_INFO
		{
			tag_as_prototype;

			template<typename TargetType> void Initialize()
			{
				// TODO comma here before, is this the correct form?
				typedef Random_Access_Sequence<typename get_type_of(choice_options)> choice_options_itf;
				typedef Random_Access_Sequence<typename get_type_of(choice_utilities),float> utilities_itf;
				typedef Random_Access_Sequence<typename get_type_of(choice_probabilities),float> probabilities_itf;
				choice_options_itf* options = this->choice_options<choice_options_itf*>();
				utilities_itf* util = this->choice_utilities<utilities_itf*>();
				probabilities_itf* prob = this->choice_probabilities<probabilities_itf*>();
				options->clear();
				util->clear();
				prob->clear();
			}

			template<typename TargetType> void Add_Choice_Option(TargetType choice_option, requires(TargetType,check(strip_modifiers(TargetType), Concepts::Is_Choice_Option_Prototype) && check(TargetType,is_pointer)))
			{
				// Push item into std::vector as anonymous
				typedef Random_Access_Sequence<typename get_type_of(choice_options)> choice_options_itf;
				typedef Prototypes::Choice_Option<get_component_type(choice_options_itf)> choice_option_itf;
				choice_options_itf* options = this->choice_options<choice_options_itf*>();
				options->push_back((choice_option_itf*)choice_option);				
			}
			template<typename TargetType> void Add_Choice_Option(TargetType choice_option, requires(TargetType,!check(strip_modifiers(TargetType), Concepts::Is_Choice_Option_Prototype) || !check(TargetType,is_pointer)))
			{
				assert_sub_check(strip_modifiers(TargetType),Concepts::Is_Choice_Option_Prototype, has_calculate_utility,  "TargetType does not have Calculate_Utility feature.");
				assert_sub_check(strip_modifiers(TargetType),Concepts::Is_Choice_Option_Prototype, is_prototype,  "TargetType is not a valid prototype.");
				assert_check(TargetType,is_pointer,  "TargetType is not a pointer.");
			}
						

			template<typename TargetType> float Evaluate_Choices(requires(TargetType,check(Component_Type, Concepts::Is_Utility_Based_Model)))
			{	
				float logsum = this->Evaluate_Utilities<NT>();
				this->Evaluate_Probabilities<NT>();
				return logsum;
			}	
			
			template<typename TargetType> float Evaluate_Utilities(bool debug=false, requires(TargetType,check(Component_Type, Concepts::Is_Utility_Based_Model)))
			{	
				typedef Random_Access_Sequence<typename get_type_of(choice_options)> choice_options_itf;
				typedef Choice_Option<get_component_type(choice_options_itf)> choice_option_itf;
				typedef Random_Access_Sequence<typename get_type_of(choice_utilities),double> utilities_itf;
				typedef Random_Access_Sequence<typename get_type_of(choice_probabilities),double> probabilities_itf;

				// Local type definition option
				choice_options_itf* choices =	this->choice_options<choice_options_itf*>();
				utilities_itf*		utils =		this->choice_utilities<utilities_itf*>();
				probabilities_itf*	probs =		this->choice_probabilities<probabilities_itf*>();
				typename choice_options_itf::iterator	itr = choices->begin();
				typename utilities_itf::iterator			u_itr = utils->begin();
				choice_option_itf* choice;

				double u, p;
				double utility_sum = 0;

				
				for (itr; itr!= choices->end(); itr++)
				{
					choice = (choice_option_itf*)(*itr);
					u = choice->template Calculate_Utility<ComponentType>();
					utils->push_back(u);
					utility_sum = utility_sum + exp(u);
				}

				// if sum(exp(u)) = 0 then no valid choices can be made as all options have -infinity utility
				if (utility_sum == 0)
				{
					THROW_WARNING("WARNING: all options have zero probability [sum(exp(u))=0], unable to evaluate choices.  Num choices was=" << choices->size()); 
					// print all choices
					int i = 0;
					for (itr; itr!= choices->end(); itr++, i++)
					{
						cout << endl << "Option " << i <<": ";
						choice = (choice_option_itf*)(*itr);
						choice->Print_Utility();
					}
					choices->clear();
					probs->clear();
					utils->clear();
					return 0;
				}

				for (u_itr=utils->begin(); u_itr!= utils->end(); u_itr++)
				{
					u = *u_itr;
					p = exp(u)/utility_sum;
					if (ISNAN(p)){ THROW_WARNING("ERROR: p is not a number. U=" << u << ", exp(u)="<<exp(u) << ", u_sum="<<utility_sum); probs->push_back(0.0);}
					else probs->push_back(p);
				}

				return log(utility_sum);

			}	
			template<typename TargetType> float Evaluate_Utilities(requires(TargetType,!check(Component_Type, Concepts::Is_Utility_Based_Model)))
			{
				assert_check(Component_Type, Concepts::Is_Utility_Based_Model, "ComponentType is not a utility based model");
			}
			
			template<typename TargetType> void Evaluate_Probabilities(requires(TargetType,check(Component_Type, Concepts::Is_Logit_Model)))
			{	
				typedef Random_Access_Sequence<typename get_type_of(choice_options)> choice_options_itf;
				typedef Choice_Option<get_component_type(choice_options_itf)> choice_option_itf;
				typedef Random_Access_Sequence<typename get_type_of(choice_utilities),double> utilities_itf;
				typedef Random_Access_Sequence<typename get_type_of(choice_probabilities),double> probabilities_itf;

				// Local type definition option
				choice_options_itf* choices =	this->choice_options<choice_options_itf*>();
				utilities_itf*		utils =		this->choice_utilities<utilities_itf*>();
				probabilities_itf*	probs =		this->choice_probabilities<probabilities_itf*>();
				typename choice_options_itf::iterator	itr = choices->begin();
				typename utilities_itf::iterator			u_itr = utils->begin();
				choice_option_itf* choice;

				double u, p;
				double utility_sum = 0;

				
				for (itr; itr!= choices->end(); itr++)
				{
					choice = (choice_option_itf*)(*itr);
					utility_sum += exp(choice->template choice_utility<double>());
				}

				// if sum(exp(u)) = 0 then no valid choices can be made as all options have -ininity utility
				if (utility_sum == 0)
				{
					THROW_WARNING("WARNING: all options have zero probability [sum(exp(u))=0], unable to evaluate choices.  Num choices was=" << choices->size()); 
					return;
				}
				for (itr= choices->begin(); itr!= choices->end(); itr++)
				{
					choice = (choice_option_itf*)(*itr);
					u = choice->template choice_utility<double>();
					p = exp(u) / utility_sum;
					if (ISNAN(p)){ THROW_WARNING("ERROR: p is not a number. U=" << u << ", exp(u)="<<exp(u) << ", u_sum="<<utility_sum); choice->template choice_probability<double>(0.0);}
					else choice->template choice_probability<double>(p);

					// Evaluate choice options in nest, if the current option is a nest-level
					if (choice->template is_nest<Component_Type>()) choice->template Evaluate_Probabilities<Component_Type>();
				}
			}	
			template<typename TargetType> void Evaluate_Probabilities(requires(TargetType,!check(Component_Type, Concepts::Is_Logit_Model)))
			{
				assert_check(Component_Type, Concepts::Is_Utility_Based_Model, "ComponentType is not a utility based model");
			}

			template<typename TargetType> void Print_Values()
			{
				typedef Random_Access_Sequence<typename get_type_of(choice_options)> choice_options_itf;
				typedef Choice_Option<get_component_type(choice_options_itf)> choice_option_itf;
				typedef Random_Access_Sequence<typename get_type_of(choice_utilities), double> utilities_itf;
				typedef Random_Access_Sequence<typename get_type_of(choice_probabilities), double> probabilities_itf;

				// Local type definition option
				choice_options_itf* choices = this->choice_options<choice_options_itf*>();
				utilities_itf*		utils = this->choice_utilities<utilities_itf*>();
				probabilities_itf*	probs = this->choice_probabilities<probabilities_itf*>();
				typename choice_options_itf::iterator	itr = choices->begin();
				typename utilities_itf::iterator			u_itr = utils->begin();
				choice_option_itf* choice;

				for (itr = choices->begin(); itr != choices->end(); itr++)
				{
					choice = (choice_option_itf*)(*itr);
					cout << choice->template choice_utility<double>() << ","<<  choice->template choice_probability<double>()<<",";

					// Evaluate choice options in nest, if the current option is a nest-level
					if (choice->template is_nest<Component_Type>()) choice->template Print_Values<Component_Type>();
				}

			}

			/// SELECT FROM THE AVAILABLE CHOICES FOR SIMULATION
			template<typename TargetType> TargetType Choose(int& selected_index, requires(TargetType,check(ComponentType, Concepts::Is_Probabilistic) && check(ComponentType, Concepts::Is_MNL_Model)))
			{
				typedef Random_Access_Sequence<typename get_type_of(choice_options)> choice_options_itf;
				typedef Choice_Option<get_component_type(choice_options_itf)> choice_option_itf;
				typedef Random_Access_Sequence<typename get_type_of(choice_probabilities),double> probabilities_itf;

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
			template<typename TargetType> TargetType Choose(int& selected_index, requires(TargetType,check(ComponentType, Concepts::Is_Probabilistic) && check(ComponentType, Concepts::Is_Nested_Logit_Model)))
			{
				typedef Random_Access_Sequence<typename get_type_of(choice_options)> choice_options_itf;
				typedef Choice_Option<get_component_type(choice_options_itf)> choice_option_itf;
				typedef Random_Access_Sequence<typename get_type_of(choice_probabilities), double> probabilities_itf;

				// Local type definition option
				choice_options_itf* choices = this->choice_options<choice_options_itf*>();
				probabilities_itf* probs = this->choice_probabilities<probabilities_itf*>();
				typename probabilities_itf::iterator p_itr = probs->begin();

				double cumulative_probability = 0;

				double rand = Uniform_RNG.Next_Rand<double>();

				int i = 0;
				for (typename choice_options_itf::iterator itr = choices->begin(); itr != choices->end(); ++itr, ++p_itr)
				{
					choice_option_itf* choice = (*itr);
					// Evaluate choice options in nest, if the current option is a nest - level
					if (choice->template is_nest<Component_Type>())
					{
						choice_options_itf* sub_choices = choice->sub_choice_options<choice_options_itf*>();
						for (typename choice_options_itf::iterator s_itr = sub_choices->begin(); s_itr != sub_choices->end(); ++s_itr)
						{
							cumulative_probability += *p_itr * (*s_itr)->choice_probability<double>(); // multiply nest probability by choice probability
							if (rand < cumulative_probability)
							{
								selected_index = i;
								return (TargetType)(*s_itr);
							}
						}

					}
					else
					{
						cumulative_probability += *p_itr;
					}
					if (rand < cumulative_probability)
					{
						selected_index = i;
						return (TargetType)(*itr);
					}
					i++;
				}

				THROW_WARNING("WARNING: No choice selected from choice model (prob, cumulative_prob, selected_index) " << rand << ", " << cumulative_probability << ", " << selected_index);
				return nullptr;
			}
			template<typename TargetType> TargetType Choose(int& selected_index, requires(TargetType,!check(ComponentType, Concepts::Is_Probabilistic) || (!check(ComponentType, Concepts::Is_Nested_Logit_Model) && !check(ComponentType, Concepts::Is_MNL_Model))))
			{
				assert_check(ComponentType, Concepts::Is_Probabilistic, "ComponentType does not specify if Choice is Deterministic or Probabilistic");
			}
		 
			template<typename TargetType> TargetType Choice_At(int selected_index)
			{
				typedef Random_Access_Sequence<typename get_type_of(choice_options),void*> choice_options_itf;

				// Local type definition option
				choice_options_itf* choices = this->choice_options<choice_options_itf*>();

				return (TargetType)choices->at(selected_index);
			}
			/// ACCESSORS
			accessor(choice_options, NONE, NONE);
			accessor(choice_utilities, NONE, NONE);
			accessor(choice_probabilities, NONE, NONE);
		};

		prototype struct Choice_Option ADD_DEBUG_INFO
		{
			tag_as_prototype;

			///These are only used if the choice option represents a nest
			accessor(sub_choice_options, NONE, NONE);
			accessor(inclusive_value_parameter, NONE, NONE);

			accessor(choice_utility, NONE, NONE);
			accessor(choice_probability, NONE, NONE);

			template<typename TargetType> void Add_Sub_Choice_Option(TargetType choice_option, requires(TargetType,check(strip_modifiers(TargetType), Concepts::Is_Choice_Option_Prototype) && check(TargetType,is_pointer)))
			{
				// Push item into std::vector as anonymous
				typedef Random_Access_Sequence<typename get_type_of(sub_choice_options)> choice_options_itf;
				choice_options_itf* options = this->sub_choice_options<choice_options_itf*>();
				options->push_back((Choice_Option<Implementations::Nested_Choice_Option_Base<NT>>*)choice_option);			
			}		

			void Print_Utility()
			{
				this_component()->Print_Utility();
			}


			template<typename ModelType> double Calculate_Utility(requires(ModelType,check(ModelType, Concepts::Is_MNL_Model)))
			{
				this->choice_utility(this_component()->Calculate_Utility());
				return this->choice_utility<double>();
			}
			template<typename ModelType> double Calculate_Utility(requires(ModelType,check(ModelType, Concepts::Is_Nested_Logit_Model)))
			{
				typedef Choice_Option<Implementations::Nested_Choice_Option_Base<NT>> choice_option_itf;
				typedef Random_Access_Sequence<typename get_type_of(sub_choice_options)> choice_options_itf;
				choice_options_itf* options = this->sub_choice_options<choice_options_itf*>();
				
				// If the option does not represent a nest, simply return its utiltiy value
				if (!is_nest<ModelType>())
				{
					this->choice_utility(this_component()->Calculate_Utility());
					return this->choice_utility<double>();
				}

				// Calculate log-sum
				double IV = 0;
				for (auto itr = options->begin(); itr != options->end(); ++itr)
				{
					choice_option_itf* choice = (choice_option_itf*)(*itr);
					IV += exp(choice->Calculate_Utility<ModelType>());
				}
				IV = this->inclusive_value_parameter<double>() * log(IV);

				this->choice_utility(this_component()->Calculate_Utility() + IV);
				return this->choice_utility<double>();
			}
			template<typename ModelType> double Calculate_Utility(requires(ModelType,!check(ModelType, Concepts::Is_Nested_Logit_Model) && !check(ModelType, Concepts::Is_MNL_Model)))
			{
				assert_check(ModelType, Concepts::Is_Nested_Logit_Model, "Choice option type is not tagged as nested logit,");
				assert_check(ModelType, Concepts::Is_MNL_Model, " or MNL model.");
			}

			template<typename ModelType> void Evaluate_Probabilities(requires(ModelType,check(ModelType, Concepts::Is_Nested_Logit_Model)))
			{	
				typedef Random_Access_Sequence<typename get_type_of(sub_choice_options)> choice_options_itf;
				typedef Choice_Option<get_component_type(choice_options_itf)> choice_option_itf;

				// Local type definition option
				choice_options_itf* choices =	this->sub_choice_options<choice_options_itf*>();
				typename choice_options_itf::iterator	itr = choices->begin();
				choice_option_itf* choice;

				double u, p;
				double utility_sum = 0;

				// return for degenerate nests
				if (choices->size() == 1)
				{
					choice = (choice_option_itf*)(*itr);
					choice->choice_probability<double>(1.0);
					if (choice->is_nest<ModelType>()) choice->Evaluate_Probabilities<ModelType>();
					return;
				}

				// otherwise solve for each subchoice probability
				for (itr; itr!= choices->end(); itr++)
				{
					choice = (choice_option_itf*)(*itr);
					utility_sum += exp(choice->choice_utility<double>());
				}

				// if sum(exp(u)) = 0 then no valid choices can be made as all options have -ininity utility
				if (utility_sum == 0)
				{
					THROW_WARNING("WARNING: all options have zero probability [sum(exp(u))=0], unable to evaluate choices.  Num choices was=" << choices->size()); 
					return;
				}
				for (itr= choices->begin(); itr!= choices->end(); itr++)
				{
					choice = (choice_option_itf*)(*itr);
					u = choice->choice_utility<double>();
					p = exp(u) / utility_sum;
					if (ISNAN(p)){ THROW_WARNING("ERROR: p is not a number. U=" << u << ", exp(u)="<<exp(u) << ", u_sum="<<utility_sum); choice->choice_probability<double>(0.0);}
					else choice->choice_probability<double>(p);

					// if the subchoice is also nested, start process over for that choice
					if (choice->is_nest<ModelType>()) choice->Evaluate_Probabilities<ModelType>();
				}
			}		
			template<typename ModelType> void Evaluate_Probabilities(requires(ModelType,check(ModelType, !Concepts::Is_Nested_Logit_Model)))
			{	
				return;
			}	

			template<typename ModelType> bool is_nest(requires(ModelType,check(ModelType, Concepts::Is_Nested_Logit_Model)))
			{
				typedef Choice_Option<Implementations::Nested_Choice_Option_Base<NT>> choice_option_itf;
				typedef Random_Access_Sequence<typename get_type_of(sub_choice_options)> choice_options_itf;
				choice_options_itf* options = this->sub_choice_options<choice_options_itf*>();
				if (options->size() == 0) return false;
				return true;
			}
			template<typename ModelType> bool is_nest(requires(ModelType,!check(ModelType, Concepts::Is_Nested_Logit_Model)))
			{
				return false;
			}

			template<typename TargetType> void Print_Values()
			{
				typedef Random_Access_Sequence<typename get_type_of(sub_choice_options)> choice_options_itf;
				typedef Choice_Option<get_component_type(choice_options_itf)> choice_option_itf;

				// Local type definition option
				choice_options_itf* choices = this->sub_choice_options<choice_options_itf*>();
				choice_option_itf* choice;

				for (choice_options_itf::iterator itr = choices->begin(); itr != choices->end(); itr++)
				{
					choice = (choice_option_itf*)(*itr);
					cout << choice->template choice_utility<double>() << "," << choice->template choice_probability<double>() << ",";

					// Evaluate choice options in nest, if the current option is a nest-level
					if (choice->template is_nest<Component_Type>()) choice->template Print_Values<Component_Type>();
				}

			}
		};
	}
}
