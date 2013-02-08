#pragma once
#include "Generic.h"

///============================================================================
/// General Definitions
///============================================================================

#define concept template<typename T=NULLTYPE,typename V=NULLTYPE>

static true_type true_val;

static const int success=sizeof(small_type);

#define strip_modifiers(TYPE) typename remove_cv<typename remove_pointer<typename remove_extent<typename remove_reference<TYPE>::type>::type>::type>::type
#define strip_modifiers_nontemplate(TYPE) remove_cv<typename remove_pointer<typename remove_extent<typename remove_reference<TYPE>::type>::type>::type>::type

//#define begin_requirements_list(NULL_CONCEPT_NAME) typedef strip_modifiers(TYPE_A) T; typedef strip_modifiers(TYPE_B) V; typedef true_type NULL_CONCEPT_NAME; typedef TYPELIST_1(NULL_CONCEPT_NAME) auto_check_list_##NULL_CONCEPT_NAME
//#define begin_requirements_list(NULL_CONCEPT_NAME) typedef TYPE_A T; typedef TYPE_B V; typedef true_type NULL_CONCEPT_NAME; typedef TYPELIST_1(NULL_CONCEPT_NAME) auto_check_list_##NULL_CONCEPT_NAME
//#define end_requirements_list(LAST_CONCEPT) static const bool value=IsTrue<auto_check_list_##LAST_CONCEPT>::value;typedef typename test_condition<value>::type type;

#define define_default_check(...) static const bool value=( __VA_ARGS__ ); typedef typename test_condition<value>::type type;
#define define_sub_check(CHECK_ALIAS,...) static const bool CHECK_ALIAS=( __VA_ARGS__ );

///============================================================================
/// Concept Checks
///============================================================================

#define check_method(CHECK_ALIAS,METHOD_NAME,METHOD_FORM)\
	struct CHECK_ALIAS##_procedure\
	{\
		template<METHOD_FORM> struct tester{};\
		template<typename U> static small_type has_matching_function_member(tester<&U::METHOD_NAME>*);\
		template<typename U> static large_type has_matching_function_member(...);\
		static const bool value=sizeof(has_matching_function_member<T>(0))==success;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure::value;\

#define check_feature(CHECK_ALIAS,FEATURE_NAME)\
	struct CHECK_ALIAS##_procedure\
	{\
	template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_feature_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
		/*static const bool member_exists=sizeof(has_matching_typename<T>(0))==success;\
		template<class U,bool B> struct p_conditional{typedef false_type type;};\
		template<class U> struct p_conditional<U,true>{typedef typename is_same<typename U::FEATURE_NAME##_feature_tag,true_type>::type type;};\
		static const bool value=(member_exists && p_conditional<T,member_exists>::type::value);\*/\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure::value;

#define check_getter(CHECK_ALIAS, FEATURE_NAME)\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_getter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure::value;\

#define check_setter(CHECK_ALIAS, FEATURE_NAME)\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_setter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure::value;\


#define check_named_member(CHECK_ALIAS,MEMBER_NAME)\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U> static small_type has_matching_named_member(is_member_object_pointer<decltype(&U::MEMBER_NAME)>::type);\
		template<typename U> static large_type has_matching_named_member(...);\
		static const bool value=sizeof(has_matching_named_member<T>(true_val))==success;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure::value;\

#define check_typename_defined(CHECK_ALIAS,TYPENAME_NAME)\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U> static small_type has_matching_typename(typename U::TYPENAME_NAME*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure::value;\

#define check_typed_member(CHECK_ALIAS,MEMBER_NAME,DESIRED_TYPE)\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U> static small_type has_matching_named_member(is_member_object_pointer<decltype(&U::MEMBER_NAME)>::type);\
		template<typename U> static large_type has_matching_named_member(...);\
		static const bool member_exists=sizeof(has_matching_named_member<T>(true_val))==success;\
		template<class U,bool B> struct p_conditional{typedef false_type type;};\
		template<class U> struct p_conditional<U,true>{typedef typename is_same<decltype(&U::MEMBER_NAME),DESIRED_TYPE U::*>::type type;};\
		static const bool value=(member_exists && p_conditional<T,member_exists>::type::value);\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure::value;\

#define check_typename_state(CHECK_ALIAS,TYPENAME_NAME,TYPENAME_STATE)\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U> static small_type has_matching_typename(typename U::TYPENAME_NAME*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool member_exists=sizeof(has_matching_typename<T>(0))==success;\
		template<class U,bool B> struct p_conditional{typedef false_type type;};\
		template<class U> struct p_conditional<U,true>{typedef typename is_same<typename U::TYPENAME_NAME,TYPENAME_STATE>::type type;};\
		static const bool value=(member_exists && p_conditional<T,member_exists>::type::value);\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure::value;\

#define check_typename_state_interface(CHECK_ALIAS,TYPENAME_NAME,TYPENAME_STATE)\
	struct REQUIREMENT_NAME\
	{\
		template<typename U> static small_type has_matching_typename(typename U::Component_Type::TYPENAME_NAME*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool member_exists=sizeof(has_matching_typename<T>(0))==success;\
		template<class U,bool B> struct p_conditional{typedef false_type type;};\
		template<class U> struct p_conditional<U,true>{typedef typename is_same<typename U::Component_Type::TYPENAME_NAME,TYPENAME_STATE>::type type;};\
		static const bool value=(member_exists && p_conditional<T,member_exists>::type::value);\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure::value;\

#define check_typename_match(CHECK_ALIAS,TYPENAME_NAME)\
	struct CHECK_ALIAS##_procedure\
	{\
		template<typename U> static small_type has_matching_typename_T(typename U::TYPENAME_NAME*);\
		template<typename U> static large_type has_matching_typename_T(...);\
		static const bool member_exists_T=sizeof(has_matching_typename_T<T>(0))==success;\
		template<typename U> static small_type has_matching_typename_V(typename U::TYPENAME_NAME*);\
		template<typename U> static large_type has_matching_typename_V(...);\
		static const bool member_exists=(member_exists_T && (sizeof(has_matching_typename_V<V>(0))==success));\
		template<class U,class W,bool B> struct p_conditional{typedef false_type type;};\
		template<class U,class W> struct p_conditional<U,W,true>{typedef typename is_same<typename U::TYPENAME_NAME,typename W::TYPENAME_NAME>::type type;};\
		static const bool value=(member_exists && p_conditional<T,V,member_exists>::type::value);\
	};\
	static const bool CHECK_ALIAS=CHECK_ALIAS##_procedure::value;\


#define check_type_match(CHECK_ALIAS,TYPE_TO_MATCH)\
	static const bool CHECK_ALIAS=is_same<T,TYPE_TO_MATCH>::value;

#define check_type_same(CHECK_ALIAS)\
	static const bool CHECK_ALIAS=is_same<T,V>::value;

#define REQUIRES_CONCEPT_PARAMS(TYPE_TO_TEST_1,TYPE_TO_TEST_2) TYPE_TO_TEST_1,TYPE_TO_TEST_2
#define check_concept(CHECK_ALIAS,CONCEPT_NAME) static const bool CHECK_ALIAS=CONCEPT_NAME<REQUIRES_CONCEPT_PARAMS(T,V)>::value;

///============================================================================
/// Call requirement macros and assertion macros
///============================================================================

#define assert_default_check(TYPE_TO_TEST,CONCEPT_NAME,ERROR_MESSAGE) static_assert(CONCEPT_NAME<TYPE_TO_TEST>::value,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")
#define assert_check(TYPE_TO_TEST,CONCEPT_NAME,ERROR_MESSAGE) static_assert(CONCEPT_NAME<strip_modifiers(TYPE_TO_TEST)>::value,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")
#define assert_check_as_given(TYPE_TO_TEST,CONCEPT_NAME,ERROR_MESSAGE) static_assert(CONCEPT_NAME<TYPE_TO_TEST>::value,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")


#define assert_sub_check(TYPE_TO_TEST,CONCEPT_NAME,SUB_CHECK_ALIAS,ERROR_MESSAGE) static_assert(CONCEPT_NAME<TYPE_TO_TEST>::SUB_CHECK_ALIAS,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")

#define ASSERT_CHECK_PARAMS_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2) TYPE_TO_TEST_1,TYPE_TO_TEST_2

#define assert_default_check_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2,CONCEPT_NAME,ERROR_MESSAGE) static_assert(CONCEPT_NAME<ASSERT_CHECK_PARAMS_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2)>::value,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")
#define assert_check_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2,CONCEPT_NAME,ERROR_MESSAGE) static_assert(CONCEPT_NAME<ASSERT_CHECK_PARAMS_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2)>::value,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")

#define assert_sub_check_2(TYPE_TO_TEST,CONCEPT_NAME,SUB_CHECK_ALIAS,ERROR_MESSAGE) static_assert(CONCEPT_NAME<ASSERT_REQUIREMENTS_PARAMS_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2)>::SUB_CHECK_ALIAS,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")

#define CALL_CHECK_PARAMS_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2) TYPE_TO_TEST_1,TYPE_TO_TEST_2

#define requires(...) char(*)[__VA_ARGS__ && True_Concept<TargetType>::value]=NULL
#define requires_getter(...) char(*)[__VA_ARGS__ && True_Concept<ReturnValueType>::value]=NULL
#define requires_setter(...) char(*)[__VA_ARGS__ && True_Concept<SetValueType>::value]=NULL
#define check(TYPE_TO_TEST,CONCEPT_NAME) CONCEPT_NAME<strip_modifiers(TYPE_TO_TEST)>::value
#define check_as_given(TYPE_TO_TEST,CONCEPT_NAME) CONCEPT_NAME<TYPE_TO_TEST>::value
#define check_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2,CONCEPT_NAME) CONCEPT_NAME<CALL_CHECK_PARAMS_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2)>::value

#define sub_check(TYPE_TO_TEST,CONCEPT_NAME,SUB_CHECK_ALIAS) CONCEPT_NAME<strip_modifiers(TYPE_TO_TEST)>::SUB_CHECK_ALIAS
#define sub_check_as_given(TYPE_TO_TEST,CONCEPT_NAME,SUB_CHECK_ALIAS) CONCEPT_NAME<TYPE_TO_TEST>::SUB_CHECK_ALIAS
#define sub_check_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2,CONCEPT_NAME,SUB_CHECK_ALIAS) CONCEPT_NAME<CALL_REQUIREMENTS_PARAMS_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2)>::SUB_CHECK_ALIAS

#define concat(...) __VA_ARGS__

//#define requires_member(CONCEPT_NAME) CONCEPT_NAME<strip_modifiers(TargetType)>::value