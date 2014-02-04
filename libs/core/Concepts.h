#pragma once
///----------------------------------------------------------------------------------------------------
/// Concepts.h - Custom Concept Checking Library
///----------------------------------------------------------------------------------------------------

#include "Function_Signature.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// General Definitions
	///----------------------------------------------------------------------------------------------------

	#define concept template<typename T=NULLTYPE,typename V=NULLTYPE>

	typedef char small_type;

	static true_type true_val;

	struct large_type{ small_type data[2]; };

	static const int success = sizeof(small_type);

	#define strip_modifiers(TYPE) typename remove_cv<typename remove_pointer<typename remove_extent<typename remove_reference<TYPE>::type>::type>::type>::type
	#define strip_modifiers_nontemplate(TYPE) remove_cv<typename remove_pointer<typename remove_extent<typename remove_reference<TYPE>::type>::type>::type>::type

	#define define_default_check(...) static const bool value=( __VA_ARGS__ ); typedef typename conditional<value,true_type,false_type>::type type;
	#define define_sub_check(CHECK_ALIAS,...) static const bool CHECK_ALIAS=( __VA_ARGS__ );

	#define assert_default_check(TYPE_TO_TEST,CONCEPT_NAME,ERROR_MESSAGE) static_assert(CONCEPT_NAME<TYPE_TO_TEST>::value,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")
	#define assert_sub_check(TYPE_TO_TEST,CONCEPT_NAME,SUB_CHECK_ALIAS,ERROR_MESSAGE) static_assert(CONCEPT_NAME<TYPE_TO_TEST>::SUB_CHECK_ALIAS,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")

	#define assert_default_check_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2,CONCEPT_NAME,ERROR_MESSAGE) static_assert(CONCEPT_NAME<concat(TYPE_TO_TEST_1,TYPE_TO_TEST_2)>::value,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")
	#define assert_sub_check_2(TYPE_TO_TEST,CONCEPT_NAME,SUB_CHECK_ALIAS,ERROR_MESSAGE) static_assert(CONCEPT_NAME<concat(TYPE_TO_TEST_1,TYPE_TO_TEST_2)>::SUB_CHECK_ALIAS,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")

	#define assert_check(TYPE_TO_TEST,CONCEPT_NAME,ERROR_MESSAGE) static_assert(CONCEPT_NAME<strip_modifiers(TYPE_TO_TEST)>::value,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")
	#define assert_check_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2,CONCEPT_NAME,ERROR_MESSAGE) static_assert(CONCEPT_NAME<concat(strip_modifiers(TYPE_TO_TEST_1), strip_modifiers(TYPE_TO_TEST_2))>::value,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")

	#define delay_compilation(A_TEMPLATE_ARGUMENT) True_Concept<A_TEMPLATE_ARGUMENT>::value

	//#define requires(...) char(*)[__VA_ARGS__ && True_Concept<TargetType>::value]=NULL
	
	#define requires(TEMPLATE_ARGUMENT_WHICH_WILL_BE_USED_TO_DEFER_METHOD_COMPILATION,...) char(*)[__VA_ARGS__ && True_Concept<TEMPLATE_ARGUMENT_WHICH_WILL_BE_USED_TO_DEFER_METHOD_COMPILATION>::value]=NULL

	#define method_requires(...) char(*)[__VA_ARGS__]=NULL

	#define check(TYPE_TO_TEST,CONCEPT_NAME) CONCEPT_NAME<TYPE_TO_TEST>::value
	#define check_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2,CONCEPT_NAME) CONCEPT_NAME<concat(TYPE_TO_TEST_1,TYPE_TO_TEST_2)>::value

	#define sub_check(TYPE_TO_TEST,CONCEPT_NAME,SUB_CHECK_ALIAS) CONCEPT_NAME<TYPE_TO_TEST>::SUB_CHECK_ALIAS
	#define sub_check_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2,CONCEPT_NAME,SUB_CHECK_ALIAS) CONCEPT_NAME<concat(TYPE_TO_TEST_1,TYPE_TO_TEST_2)>::SUB_CHECK_ALIAS

	#define null_requirement char(*)[1]=nullptr
	#define null_rq char(*)[1]=nullptr

	#define NONE true
	
	NULLTYPE null_argument;

	///============================================================================
	/// IF - standard compile-time IF statement
	///============================================================================

	#define IF(CHECK_ALIAS,CONDITION,...)\
		template<bool Condition>\
		struct CHECK_ALIAS##_IF\
		{\
			__VA_ARGS__;\
			\
			typedef true_type value;\
		};\
		template<>\
		struct CHECK_ALIAS##_IF<false>\
		{\
			typedef false_type value;\
		};\
		\
		typedef typename CHECK_ALIAS##_IF<CONDITION>::value CHECK_ALIAS##_checked;\

	///============================================================================
	/// True_Concept - concept which always evaluates to true
	///============================================================================

	template<typename TYPE_A=NULLTYPE,typename TYPE_B=NULLTYPE,bool assert_requirements=false>
	struct True_Concept
	{
		typedef true_type type;
		static const bool value=true;
	};

	///============================================================================
	/// False_Concept - concept which always evaluates to false
	///============================================================================

	template<typename TYPE_A=NULLTYPE,typename TYPE_B=NULLTYPE,bool assert_requirements=false>
	struct False_Concept
	{
		typedef false_type type;
		static const bool value=false;
	};




	///----------------------------------------------------------------------------------------------------
	/// Concept Checks
	///----------------------------------------------------------------------------------------------------

	///----------------------------------------------------------------------------------------------------
	/// check_concept - concept must satisfy other concept
	///----------------------------------------------------------------------------------------------------

	#define check_concept(CHECK_ALIAS,CONCEPT_NAME,TYPE_TO_TEST_1,TYPE_TO_TEST_2) static const bool CHECK_ALIAS=CONCEPT_NAME<TYPE_TO_TEST_1,TYPE_TO_TEST_2>::value;

	///----------------------------------------------------------------------------------------------------
	/// general_check - user-defined check
	///----------------------------------------------------------------------------------------------------

	#define general_check(CHECK_ALIAS,...)\
	template<typename TypeChecked>\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct data_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct data_check<U,true>{ static const bool value = __VA_ARGS__; };\
		\
		static const bool value = data_check<TypeChecked>::value;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;

	///----------------------------------------------------------------------------------------------------
	/// check_typedef_name - typedef of given name must exist
	///----------------------------------------------------------------------------------------------------

	#define check_typedef_name(CHECK_ALIAS,NAME)\
	template<typename TypeChecked>\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct data_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct data_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(typename V::##NAME*);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(0))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const bool value = true; };\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = data_check<TypeChecked>::value;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;

	///----------------------------------------------------------------------------------------------------
	/// check_typedef_type - typedef of given name must exist and match given type
	///----------------------------------------------------------------------------------------------------

	#define check_typedef_type(CHECK_ALIAS,NAME,TYPE)\
	template<typename TypeChecked>\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct data_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct data_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(typename V::##NAME*);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(0))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const bool value = is_same<typename V::##NAME,TYPE>::value; };\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = data_check<TypeChecked>::value;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;

	///----------------------------------------------------------------------------------------------------
	/// check_data_member_name - given name must be a data member
	///----------------------------------------------------------------------------------------------------

	#define check_data_member_name(CHECK_ALIAS,NAME)\
	template<typename TypeChecked>\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct data_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct data_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(typename is_member_object_pointer<decltype(&V::NAME)>::type);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(true_val))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const bool value = true; };\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = data_check<TypeChecked>::value;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;

	///----------------------------------------------------------------------------------------------------
	/// check_data_member_type - given name must be a data member and match given type
	///----------------------------------------------------------------------------------------------------

	#define check_data_member_type(CHECK_ALIAS,NAME,TYPE)\
	template<typename TypeChecked>\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct data_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct data_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(typename is_member_object_pointer<decltype(&V::NAME)>::type);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(true_val))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const bool value = is_same<decltype(&V::NAME),TYPE V::*>::value; };\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = data_check<TypeChecked>::value;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;

	///----------------------------------------------------------------------------------------------------
	/// check_data_member_meets_check - given name must be a data member and meet user defined compile time check
	///----------------------------------------------------------------------------------------------------

	#define check_data_member_meets_check(CHECK_ALIAS,NAME,...)\
	template<typename TypeChecked>\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct data_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct data_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(typename is_member_object_pointer<decltype(&V::NAME)>::type);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(true_val))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const bool value = __VA_ARGS__; };\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = data_check<TypeChecked>::value;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;

	///----------------------------------------------------------------------------------------------------
	/// check_method_name - given name must be a member method
	///----------------------------------------------------------------------------------------------------

	#define check_method_name(CHECK_ALIAS,NAME)\
	template<typename TypeChecked>\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct function_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct function_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(typename is_member_function_pointer<decltype(&V::NAME)>::type);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(true_val))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const bool value = true; };\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = function_check<TypeChecked>::value;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;

	///----------------------------------------------------------------------------------------------------
	/// check_method_type - given name must be a method member and match given signature
	///----------------------------------------------------------------------------------------------------

	#define check_method_type(CHECK_ALIAS,NAME,FUNCTION_SIGNATURE)\
	template<typename TypeChecked>\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct function_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct function_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(typename is_member_function_pointer<decltype(&V::NAME)>::type);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(true_val))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const bool value = is_same<decltype(&V::NAME),FUNCTION_SIGNATURE::function_type>::value; };\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = function_check<TypeChecked>::value;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;

	///----------------------------------------------------------------------------------------------------
	/// check_template_method_name - given name must be a method template member
	///----------------------------------------------------------------------------------------------------

	#define check_template_method_name(CHECK_ALIAS,NAME)\
	template<typename TypeChecked>\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct function_check{ static const bool value = true; };\
		template<typename U>\
		struct function_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(void (V::* arg)() = &V::NAME<NULLTYPE>);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const int value = true;};\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = function_check<TypeChecked>::value;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;

	///----------------------------------------------------------------------------------------------------
	/// check_template_method_type - given name must be a method template member and match signature
	///----------------------------------------------------------------------------------------------------

	#define check_template_method_type(CHECK_ALIAS,NAME,FUNCTION_SIGNATURE,...)\
	template<typename TypeChecked>\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct function_check{ static const bool value = true; };\
		template<typename U>\
		struct function_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(void (V::* arg)() = &V::NAME<__VA_ARGS__>);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V> static small_type has_matching_formed_member(typename FUNCTION_SIGNATURE::function_type);\
			template<typename V> static large_type has_matching_formed_member(...);\
			\
			template<typename V>\
			struct form_check<V,true>{ static const int value = (sizeof(has_matching_formed_member<V>(&V::NAME<__VA_ARGS__>))==success);};\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = function_check<TypeChecked>::value;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure<T>::value;

}