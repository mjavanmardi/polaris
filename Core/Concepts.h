#pragma once
#include "Generic.h"

///============================================================================
/// General Definitions
///============================================================================

#define concept template<typename TYPE_A=NULLTYPE,typename TYPE_B=NULLTYPE,bool assert_requirements=false> struct

static const int success=sizeof(small_type);

#define strip_modifiers(TYPE) typename remove_cv<typename remove_pointer<typename remove_extent<typename remove_reference<typename TYPE>::type>::type>::type>::type
#define strip_modifiers_nontemplate(TYPE) remove_cv<typename remove_pointer<typename remove_extent<typename remove_reference<typename TYPE>::type>::type>::type>::type

//#define begin_requirements_list(NULL_CONCEPT_NAME) typedef strip_modifiers(TYPE_A) T; typedef strip_modifiers(TYPE_B) V; typedef true_type NULL_CONCEPT_NAME; typedef TYPELIST_1(NULL_CONCEPT_NAME) auto_check_list_##NULL_CONCEPT_NAME
#define begin_requirements_list(NULL_CONCEPT_NAME) typedef TYPE_A T; typedef TYPE_B V; typedef true_type NULL_CONCEPT_NAME; typedef TYPELIST_1(NULL_CONCEPT_NAME) auto_check_list_##NULL_CONCEPT_NAME
#define end_requirements_list(LAST_CONCEPT) static const bool value=IsTrue<auto_check_list_##LAST_CONCEPT>::value;typedef typename test_condition<value>::type type;

///============================================================================
/// Concept Checks
///============================================================================

#define requires_method(LINKED_CONCEPT,REQUIREMENT_NAME,METHOD_FORM,ERROR_MESSAGE)\
	struct REQUIREMENT_NAME\
	{\
		template<METHOD_FORM> struct tester{};\
		template<typename U> static small_type has_matching_function_member(tester<&U::REQUIREMENT_NAME>*);\
		template<typename U> static large_type has_matching_function_member(...);\
		static const bool value=sizeof(has_matching_function_member<T>(0))==success;\
		static_assert(value || !assert_requirements,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n");\
	};\
	typedef typename Append<auto_check_list_##LINKED_CONCEPT,REQUIREMENT_NAME>::Result auto_check_list_##REQUIREMENT_NAME;

#define requires_feature(LINKED_CONCEPT,FEATURE_NAME,FEATURE_TYPE,ERROR_MESSAGE)\
	struct FEATURE_NAME\
	{\
	template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_FEATURE_TYPE*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
		static_assert(value || !assert_requirements,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n");\
	};\
	typedef typename Append<auto_check_list_##LINKED_CONCEPT,FEATURE_NAME>::Result auto_check_list_##FEATURE_NAME;

#define requires_named_member(LINKED_CONCEPT,REQUIREMENT_NAME,ERROR_MESSAGE)\
	struct REQUIREMENT_NAME\
	{\
		template<typename U> static small_type has_matching_named_member(decltype(&U::REQUIREMENT_NAME));\
		template<typename U> static large_type has_matching_named_member(...);\
		static const bool value=sizeof(has_matching_named_member<T>(0))==success;\
		static_assert(value || !assert_requirements,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n");\
	};\
	typedef typename Append<auto_check_list_##LINKED_CONCEPT,REQUIREMENT_NAME>::Result auto_check_list_##REQUIREMENT_NAME;

#define requires_typename_defined(LINKED_CONCEPT,REQUIREMENT_NAME,ERROR_MESSAGE)\
	struct REQUIREMENT_NAME\
	{\
		template<typename U> static small_type has_matching_typename(typename U::REQUIREMENT_NAME*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
		static_assert(value || !assert_requirements,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n");\
	};\
	typedef typename Append<auto_check_list_##LINKED_CONCEPT,REQUIREMENT_NAME>::Result auto_check_list_##REQUIREMENT_NAME;

#define requires_typed_member(LINKED_CONCEPT,REQUIREMENT_NAME,DESIRED_TYPE,ERROR_MESSAGE)\
	struct REQUIREMENT_NAME\
	{\
		template<typename U> static small_type has_matching_named_member(decltype(&U::REQUIREMENT_NAME));\
		template<typename U> static large_type has_matching_named_member(...);\
		static const bool member_exists=sizeof(has_matching_named_member<T>(0))==success;\
		template<class U,bool B> struct p_conditional{typedef false_type type;};\
		template<class U> struct p_conditional<U,true>{typedef typename is_same<decltype(&U::REQUIREMENT_NAME),DESIRED_TYPE U::*>::type type;};\
		static const bool value=(member_exists && p_conditional<T,member_exists>::type::value);\
	};\
	typedef typename Append<auto_check_list_##LINKED_CONCEPT,REQUIREMENT_NAME>::Result auto_check_list_##REQUIREMENT_NAME;

#define requires_typename_state(LINKED_CONCEPT,REQUIREMENT_NAME,TYPENAME_STATE,ERROR_MESSAGE)\
	struct REQUIREMENT_NAME\
	{\
		template<typename U> static small_type has_matching_typename(typename U::REQUIREMENT_NAME*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool member_exists=sizeof(has_matching_typename<T>(0))==success;\
		template<class U,bool B> struct p_conditional{typedef false_type type;};\
		template<class U> struct p_conditional<U,true>{typedef typename is_same<typename U::REQUIREMENT_NAME,TYPENAME_STATE>::type type;};\
		static const bool value=(member_exists && p_conditional<T,member_exists>::type::value);\
		static_assert(value || !assert_requirements,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n");\
	};\
	typedef typename Append<auto_check_list_##LINKED_CONCEPT,REQUIREMENT_NAME>::Result auto_check_list_##REQUIREMENT_NAME;

#define requires_concept_checked_member(LINKED_CONCEPT,REQUIREMENT_NAME,CONCEPT_NAME,ERROR_MESSAGE)\
	struct REQUIREMENT_NAME\
	{\
		template<typename U> static small_type has_matching_named_member(decltype(&U::REQUIREMENT_NAME));\
		template<typename U> static large_type has_matching_named_member(...);\
		static const bool member_exists=sizeof(has_matching_named_member<T>(0))==success;\
		template<typename U,typename V> static V extract_type(V U::* val);\
		template<class U,bool B> struct p_conditional{typedef false_type type;};\
		template<class U> struct p_conditional<U,true>{typedef typename CONCEPT_NAME<decltype(extract_type(&U::REQUIREMENT_NAME))>::type type;};\
		static const bool value=(member_exists && p_conditional<T,member_exists>::type::value);\
	};\
	typedef typename Append<auto_check_list_##LINKED_CONCEPT,REQUIREMENT_NAME>::Result auto_check_list_##REQUIREMENT_NAME;

#define requires_typename_match(LINKED_CONCEPT,REQUIREMENT_NAME,ERROR_MESSAGE)\
	struct REQUIREMENT_NAME\
	{\
		template<typename U> static small_type has_matching_typename_T(typename U::REQUIREMENT_NAME*);\
		template<typename U> static large_type has_matching_typename_T(...);\
		static const bool member_exists_T=sizeof(has_matching_typename_T<T>(0))==success;\
		template<typename U> static small_type has_matching_typename_V(typename U::REQUIREMENT_NAME*);\
		template<typename U> static large_type has_matching_typename_V(...);\
		static const bool member_exists=(member_exists_T && (sizeof(has_matching_typename_V<V>(0))==success));\
		template<class U,class W,bool B> struct p_conditional{typedef false_type type;};\
		template<class U,class W> struct p_conditional<U,W,true>{typedef typename is_same<typename U::REQUIREMENT_NAME,typename W::REQUIREMENT_NAME>::type type;};\
		static const bool value=(member_exists && p_conditional<T,V,member_exists>::type::value);\
		static_assert(value || !assert_requirements,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n");\
	};\
	typedef typename Append<auto_check_list_##LINKED_CONCEPT,REQUIREMENT_NAME>::Result auto_check_list_##REQUIREMENT_NAME;

#define REQUIRES_CONCEPT_PARAMS(TYPE_TO_TEST_1,TYPE_TO_TEST_2) TYPE_TO_TEST_1,TYPE_TO_TEST_2,assert_requirements
#define requires_concept(LINKED_CONCEPT,REQUIREMENT_NAME) typedef typename Append<auto_check_list_##LINKED_CONCEPT,REQUIREMENT_NAME<REQUIRES_CONCEPT_PARAMS(T,V)>>::Result auto_check_list_##REQUIREMENT_NAME;

///============================================================================
/// Call requirement macros and assertion macros
///============================================================================

#define ASSERT_REQUIREMENTS_PARAMS(TYPE_TO_TEST) TYPE_TO_TEST,NULLTYPE,true
#define assert_requirements(TYPE_TO_TEST,CONCEPT_NAME,ERROR_MESSAGE) static_assert(CONCEPT_NAME<ASSERT_REQUIREMENTS_PARAMS(TYPE_TO_TEST)>::value,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")
#define assert_requirements_std(TYPE_TO_TEST,CONCEPT_NAME,ERROR_MESSAGE) static_assert(CONCEPT_NAME<TYPE_TO_TEST>::value,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")

#define ASSERT_REQUIREMENTS_PARAMS_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2) TYPE_TO_TEST_1,TYPE_TO_TEST_2,true
#define ASSERT_REQUIREMENTS_PARAMS_STD_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2) TYPE_TO_TEST_1,TYPE_TO_TEST_2

#define assert_requirements_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2,CONCEPT_NAME,ERROR_MESSAGE) static_assert(CONCEPT_NAME<ASSERT_REQUIREMENTS_PARAMS_2(typename TYPE_TO_TEST_1,typename TYPE_TO_TEST_2)>::value,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")
#define assert_requirements_std_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2,CONCEPT_NAME,ERROR_MESSAGE) static_assert(CONCEPT_NAME<ASSERT_REQUIREMENTS_PARAMS_STD_2(typename TYPE_TO_TEST_1,typename TYPE_TO_TEST_2)>::value,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n")

#define CALL_REQUIREMENTS_PARAMS_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2) TYPE_TO_TEST_1,TYPE_TO_TEST_2
#define call_requires(TYPE_TO_TEST,CONCEPT_NAME) char(*)[CONCEPT_NAME<TYPE_TO_TEST>::value]=NULL
#define call_requires_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2,CONCEPT_NAME) char(*)[CONCEPT_NAME<CALL_REQUIREMENTS_PARAMS_2(typename TYPE_TO_TEST_1,typename TYPE_TO_TEST_2)>::value]=NULL

#define call_requirements(...) char(*)[__VA_ARGS__ && True_Concept<TargetType>::value]=NULL
#define requires(TYPE_TO_TEST,CONCEPT_NAME) CONCEPT_NAME<TYPE_TO_TEST>::value
#define requires_2(TYPE_TO_TEST_1,TYPE_TO_TEST_2,CONCEPT_NAME) CONCEPT_NAME<CALL_REQUIREMENTS_PARAMS_2(typename TYPE_TO_TEST_1,typename TYPE_TO_TEST_2)>::value
