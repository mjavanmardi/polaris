#pragma once
#include "Basic_Concepts.h"

///============================================================================
/// facet - standard declarator for all interface facets
///============================================================================

#define facet public: template<typename TargetType>

///============================================================================
/// facet_base - standard declarator for all base facets
///============================================================================

#define facet_base public: template<typename ThisType,typename CallerType,typename TargetType>

///============================================================================
/// declare_facet_event - header for a basic event facet
///============================================================================

struct event_type{};

#define declare_facet_event(FACET_NAME)\
	typedef event_type FACET_NAME##_event_tag;\
	template<typename TargetType> static void FACET_NAME(ThisType* pthis)

template<typename Type>
struct member_function_ptr_types<Type,event_type>
{
	typedef void (Type::* type)(NULLTYPE*);
};

///============================================================================
/// declare_facet_conditional - header for a basic condition facet
///============================================================================

struct conditional_type{};

#define declare_facet_conditional(FACET_NAME)\
	typedef conditional_type FACET_NAME##_conditional_tag;\
	template<typename TargetType> static void FACET_NAME(ThisType* pthis,Conditional_Response& response)

template<typename Type>
struct member_function_ptr_types<Type,conditional_type>
{
	typedef void (Type::* type)(NULLTYPE*,Conditional_Response& response);
};

///============================================================================
/// facet_accessor - implements the standard get and set dispatch facets
///		includes a check on whether the base has corresponding dispatch
///============================================================================

#define facet_accessor(FACET_NAME)\
	public:\
	template<typename T>\
	struct FACET_NAME##_get_check\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FACET_NAME##_getter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};\
	template<typename TargetType>\
	TargetType FACET_NAME(call_requirements((requires(ThisType,Is_Dispatchable) && requires(ThisType,FACET_NAME##_get_check))))\
	{\
		return PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,CallerType,TargetType>();\
	}\
	template<typename TargetType>\
	TargetType FACET_NAME(call_requirements(!(requires(ThisType,Is_Dispatchable) && requires(ThisType,FACET_NAME##_get_check))))\
	{\
		static_assert(FACET_NAME##_get_check<ThisType>::value,"\n\n\n[--------- No Getter Target for " #FACET_NAME " to Dispatch To! ---------]\n\n");\
	}\
	template<typename T>\
	struct FACET_NAME##_set_check\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FACET_NAME##_setter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};\
	template<typename TargetType>\
	void FACET_NAME(TargetType set_value,call_requirements((requires(ThisType,Is_Dispatchable) && requires(ThisType,FACET_NAME##_set_check))))\
	{\
		PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,CallerType,TargetType>(set_value);\
	}\
	template<typename TargetType>\
	void FACET_NAME(TargetType set_value,call_requirements(!(requires(ThisType,Is_Dispatchable) && requires(ThisType,FACET_NAME##_set_check))))\
	{\
		static_assert(FACET_NAME##_set_check<ThisType>::value,"\n\n\n[--------- No Setter Target for " #FACET_NAME " to Dispatch To! ---------]\n\n");\
	}\

///============================================================================
/// facet_accessor_interface - implements the standard get and set dispatch facets
///		takes the TargetType as the component so checks can be performed
///		returns the interface implied by TargetType
///============================================================================

#define facet_accessor_interface(FACET_NAME)\
	public:\
	template<typename T>\
	struct FACET_NAME##_get_check\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FACET_NAME##_getter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};\
	template<typename TargetType>\
	TargetType::Interface_Type* FACET_NAME(call_requirements((requires(ThisType,Is_Dispatchable) && requires(ThisType,FACET_NAME##_get_check))))\
	{\
		return PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,CallerType,TargetType>();\
	}\
	template<typename TargetType>\
	TargetType::Interface_Type* FACET_NAME(call_requirements(!(requires(ThisType,Is_Dispatchable) && requires(ThisType,FACET_NAME##_get_check))))\
	{\
		static_assert(FACET_NAME##_get_check<ThisType>::value,"\n\n\n[--------- No Getter Target for " #FACET_NAME " to Dispatch To! ---------]\n\n");\
	}\
	template<typename T>\
	struct FACET_NAME##_set_check\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FACET_NAME##_setter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};\
	template<typename TargetType>\
	void FACET_NAME(TargetType::Interface_Type* set_value,call_requirements((requires(ThisType,Is_Dispatchable) && requires(ThisType,FACET_NAME##_set_check))))\
	{\
		PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,CallerType,TargetType>(set_value);\
	}\
	template<typename TargetType>\
	void FACET_NAME(TargetType::Interface_Type* set_value,call_requirements(!(requires(ThisType,Is_Dispatchable) && requires(ThisType,FACET_NAME##_set_check))))\
	{\
		static_assert(FACET_NAME##_set_check<ThisType>::value,"\n\n\n[--------- No Setter Target for " #FACET_NAME " to Dispatch To! ---------]\n\n");\
	}\
	protected:\
	template<typename TargetType>\
	TargetType::Interface_Type* FACET_NAME(call_requirements((requires(ThisType,Is_Dispatchable) && requires(ThisType,FACET_NAME##_get_check))))\
	{\
		return PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,ThisType,TargetType>();\
	}\
	template<typename TargetType>\
	TargetType::Interface_Type* FACET_NAME(call_requirements(!(requires(ThisType,Is_Dispatchable) && requires(ThisType,FACET_NAME##_get_check))))\
	{\
		static_assert(FACET_NAME##_get_check<ThisType>::value,"\n\n\n[--------- No Getter Target for " #FACET_NAME " to Dispatch To! ---------]\n\n");\
	}\
	template<typename TargetType>\
	void FACET_NAME(TargetType::Interface_Type* set_value,call_requirements((requires(ThisType,Is_Dispatchable) && requires(ThisType,FACET_NAME##_set_check))))\
	{\
		PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,ThisType,TargetType>(set_value);\
	}\
	template<typename TargetType>\
	void FACET_NAME(TargetType::Interface_Type* set_value,call_requirements(!(requires(ThisType,Is_Dispatchable) && requires(ThisType,FACET_NAME##_set_check))))\
	{\
		static_assert(FACET_NAME##_set_check<ThisType>::value,"\n\n\n[--------- No Setter Target for " #FACET_NAME " to Dispatch To! ---------]\n\n");\
	}\
	public:

///============================================================================
/// facet_getter - catches the standard get dispatches meeting concepts
///============================================================================

struct getter_type{};

template<typename Type>
struct member_function_ptr_types<Type,getter_type>
{
	typedef NULLTYPE (Type::* type)(void);
};

#define declare_facet_getter(FACET_NAME,ADDITIONAL_REQUIREMENTS)\
	template<typename ThisType,typename CallerType,typename TargetType> TargetType FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched) && (ADDITIONAL_REQUIREMENTS)))

#define tag_getter(FACET_NAME) typedef getter_type FACET_NAME##_getter_tag;

///============================================================================
/// facet_setter - catches the standard set dispatches meeting N concepts
///============================================================================

struct setter_type{};

template<typename Type>
struct member_function_ptr_types<Type,setter_type>
{
	typedef void (Type::* type)(NULLTYPE);
};

#define tag_setter(FACET_NAME) typedef setter_type FACET_NAME##_setter_tag;

#define declare_facet_setter(FACET_NAME,ADDITIONAL_REQUIREMENTS)\
	template<typename ThisType,typename CallerType,typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched) && (ADDITIONAL_REQUIREMENTS)))

///============================================================================
/// declare_facet_getter_setter - catches the standard get and set dispatches meeting concepts
///============================================================================

#define declare_facet_getter_setter_basic(FACET_NAME) declare_facet_getter(FACET_NAME);declare_facet_setter(FACET_NAME)
#define declare_facet_getter_setter(FACET_NAME,ADDITIONAL_REQUIREMENTS_GETTER,ADDITIONAL_REQUIREMENTS_SETTER) declare_facet_getter(FACET_NAME,ADDITIONAL_REQUIREMENTS_GETTER);declare_facet_setter(FACET_NAME,ADDITIONAL_REQUIREMENTS_SETTER)

#define tag_getter_setter(FACET_NAME) tag_getter(FACET_NAME);tag_setter(FACET_NAME)

///============================================================================
/// member_data – data member creator, type-definition and basic accessors
///============================================================================

#define member_data_basic(DATA_TYPE,FACET_NAME)\
	protected:\
	typedef DATA_TYPE FACET_NAME##_type;\
	typedef strip_modifiers_nontemplate(DATA_TYPE) FACET_NAME##_type_raw;\
	DATA_TYPE _##FACET_NAME;\
	template<typename ThisType, typename CallerType, typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched))){_##FACET_NAME=(DATA_TYPE)set_value;}\
	tag_setter(FACET_NAME);\
	template<typename ThisType, typename CallerType, typename TargetType> TargetType FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_##FACET_NAME;}\
	tag_getter(FACET_NAME);\
	public:

#define member_data(DATA_TYPE,FACET_NAME,ADDITIONAL_REQUIREMENTS_GETTER,ADDITIONAL_REQUIREMENTS_SETTER)\
	protected:\
	typedef DATA_TYPE FACET_NAME##_type;\
	typedef strip_modifiers_nontemplate(DATA_TYPE) FACET_NAME##_type_raw;\
	DATA_TYPE _##FACET_NAME;\
	template<typename ThisType, typename CallerType, typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched) && ( ADDITIONAL_REQUIREMENTS_SETTER ))){_##FACET_NAME=(DATA_TYPE)set_value;}\
	template<typename ThisType, typename CallerType, typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched) && !( ADDITIONAL_REQUIREMENTS_SETTER ))){static_assert(false,"\n\n\n[--------- one or more unmatched setter requirements: " #ADDITIONAL_REQUIREMENTS_SETTER " ---------]\n\n");}\
	tag_setter(FACET_NAME);\
	template<typename ThisType, typename CallerType, typename TargetType> TargetType FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched) && ( ADDITIONAL_REQUIREMENTS_GETTER ))){return (TargetType)_##FACET_NAME;}\
	template<typename ThisType, typename CallerType, typename TargetType> TargetType FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched) && !( ADDITIONAL_REQUIREMENTS_GETTER ))){static_assert(false,"\n\n\n[--------- one or more unmatched getter requirements: " #ADDITIONAL_REQUIREMENTS_GETTER " ---------]\n\n");}\
	tag_getter(FACET_NAME);\
	public:

#define member_component_basic(COMPONENT_TYPE,FACET_NAME)\
	protected:\
	typedef COMPONENT_TYPE FACET_NAME##_type;\
	COMPONENT_TYPE::Interface_Type* _##FACET_NAME;\
	template<typename ThisType, typename CallerType, typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched))){_##FACET_NAME=(COMPONENT_TYPE::Interface_Type*)set_value;}\
	tag_setter(FACET_NAME);\
	template<typename ThisType, typename CallerType, typename TargetType> TargetType::Interface_Type* FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType::Interface_Type*)_##FACET_NAME;}\
	tag_getter(FACET_NAME);\
	public:

#define member_component(COMPONENT_TYPE,FACET_NAME,ADDITIONAL_REQUIREMENTS_GETTER,ADDITIONAL_REQUIREMENTS_SETTER)\
	protected:\
	typedef COMPONENT_TYPE FACET_NAME##_type;\
	COMPONENT_TYPE::Interface_Type* _##FACET_NAME;\
	template<typename ThisType, typename CallerType, typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched) && ( ADDITIONAL_REQUIREMENTS_SETTER ))){_##FACET_NAME=(COMPONENT_TYPE::Interface_Type*)set_value;}\
	template<typename ThisType, typename CallerType, typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched) && !( ADDITIONAL_REQUIREMENTS_SETTER ))){static_assert(false,"\n\n\n[--------- one or more unmatched setter requirements: " #ADDITIONAL_REQUIREMENTS_SETTER " ---------]\n\n");}\
	tag_setter(FACET_NAME);\
	template<typename ThisType, typename CallerType, typename TargetType> TargetType::Interface_Type* FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched) && ( ADDITIONAL_REQUIREMENTS_GETTER ))){return (TargetType::Interface_Type*)_##FACET_NAME;}\
	template<typename ThisType, typename CallerType, typename TargetType> TargetType::Interface_Type* FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched) && !( ADDITIONAL_REQUIREMENTS_GETTER ))){static_assert(false,"\n\n\n[--------- one or more unmatched getter requirements: " #ADDITIONAL_REQUIREMENTS_GETTER " ---------]\n\n");}\
	tag_getter(FACET_NAME);\
	public:
	


