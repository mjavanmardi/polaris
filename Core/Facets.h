#pragma once
#include "Containers.h"

///============================================================================
/// facet - standard declarator for all interface facets
///============================================================================

#define facet public: template<typename TargetType>

///============================================================================
/// facet_base - standard declarator for all base facets
///============================================================================

#define facet_base public: template<typename ComponentType,typename CallerType,typename TargetType>

///============================================================================
/// declare_facet_event - header for a basic event facet
///============================================================================

struct event_type{};

#define declare_facet_event(FACET_NAME)\
	typedef event_type FACET_NAME##_event_tag;\
	template<typename TargetType> static void FACET_NAME(void* _this)

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
	template<typename TargetType> static void FACET_NAME(void* _this,Conditional_Response& response)

template<typename Type>
struct member_function_ptr_types<Type,conditional_type>
{
	typedef void (Type::* type)(NULLTYPE*,Conditional_Response& response);
};

///============================================================================
/// define_get_set_checks - implements a mini-concept to check for base existence
///============================================================================

#define define_get_exists_check(FACET_NAME)\
	public:\
	template<typename T>\
	struct FACET_NAME##_get_exists\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FACET_NAME##_getter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};

#define define_set_exists_check(FACET_NAME)\
	template<typename T>\
	struct FACET_NAME##_set_exists\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FACET_NAME##_setter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};

#define define_get_set_exist_check(FACET_NAME) define_get_exists_check(FACET_NAME) define_set_exists_check(FACET_NAME)

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
	TargetType FACET_NAME(call_requirements((requires(ComponentType,FACET_NAME##_get_check))))\
	{\
		return component_cast_self().template FACET_NAME<ComponentType,CallerType,TargetType>();\
	}\
	template<typename TargetType>\
	TargetType FACET_NAME(call_requirements(!requires(ComponentType,FACET_NAME##_get_check)))\
	{\
		static_assert(FACET_NAME##_get_check<ComponentType>::value,"\n\n\n[--------- No Getter Target for " #FACET_NAME " to Dispatch To! ---------]\n\n");\
	}\
	template<typename T>\
	struct FACET_NAME##_set_check\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FACET_NAME##_setter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};\
	template<typename TargetType>\
	void FACET_NAME(TargetType set_value,call_requirements((requires(ComponentType,FACET_NAME##_set_check))))\
	{\
		component_cast_self().template FACET_NAME<ComponentType,CallerType,TargetType>(set_value);\
	}\
	template<typename TargetType>\
	void FACET_NAME(TargetType set_value,call_requirements(!requires(ComponentType,FACET_NAME##_set_check)))\
	{\
		static_assert(FACET_NAME##_set_check<ComponentType>::value,"\n\n\n[--------- No Setter Target for " #FACET_NAME " to Dispatch To! ---------]\n\n");\
	}\


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
	template<typename ComponentType,typename CallerType,typename TargetType> TargetType FACET_NAME(call_requirements(requires(ComponentType,Is_Dispatched) && (ADDITIONAL_REQUIREMENTS)))

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
	template<typename ComponentType,typename CallerType,typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ComponentType,Is_Dispatched) && (ADDITIONAL_REQUIREMENTS)))

///============================================================================
/// declare_facet_getter_setter - catches the standard get and set dispatches meeting concepts
///============================================================================

#define declare_facet_getter_setter_basic(FACET_NAME) declare_facet_getter(FACET_NAME);declare_facet_setter(FACET_NAME)
#define declare_facet_getter_setter(FACET_NAME,ADDITIONAL_REQUIREMENTS_GETTER,ADDITIONAL_REQUIREMENTS_SETTER) declare_facet_getter(FACET_NAME,ADDITIONAL_REQUIREMENTS_GETTER);declare_facet_setter(FACET_NAME,ADDITIONAL_REQUIREMENTS_SETTER)

#define tag_getter_setter(FACET_NAME) tag_getter(FACET_NAME);tag_setter(FACET_NAME)

///============================================================================
/// member_data and member_pointer – member creator, type-definition and basic accessors
///============================================================================

#define member_pointer_basic(DATA_TYPE,FACET_NAME)\
	protected:\
		DATA_TYPE* _##FACET_NAME;\
	public:\
		typedef DATA_TYPE FACET_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FACET_NAME(call_requirements(!requires(TargetType,is_pointer)))\
		{return (TargetType)(*_##FACET_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FACET_NAME(call_requirements(requires(TargetType,is_pointer)))\
		{return (TargetType)(_##FACET_NAME);}\
		tag_getter(FACET_NAME);\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FACET_NAME(TargetType value,call_requirements(!requires(TargetType,is_pointer)))\
		{(*_##FACET_NAME)=((DATA_TYPE)value);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FACET_NAME(TargetType value,call_requirements(requires(TargetType,is_pointer)))\
		{_##FACET_NAME=(DATA_TYPE*)value;}\
		tag_setter(FACET_NAME);

#define member_data_basic(DATA_TYPE,FACET_NAME)\
	protected:\
		DATA_TYPE _##FACET_NAME;\
	public:\
		typedef DATA_TYPE FACET_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FACET_NAME(call_requirements(!requires(TargetType,is_pointer)))\
		{return (TargetType)(_##FACET_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FACET_NAME(call_requirements(requires(TargetType,is_pointer)))\
		{return (TargetType)(&_##FACET_NAME);}\
		tag_getter(FACET_NAME);\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FACET_NAME(TargetType value,call_requirements(!requires(TargetType,is_pointer)))\
		{_##FACET_NAME=((DATA_TYPE)value);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FACET_NAME(TargetType value,call_requirements(requires(TargetType,is_pointer)))\
		{_##FACET_NAME=(DATA_TYPE)(*value);}\
		tag_setter(FACET_NAME);

///============================================================================
/// member_component – member creator, type-definition and basic accessors
///============================================================================

#define member_component_basic(COMPONENT_TYPE,FACET_NAME)\
	protected:\
		COMPONENT_TYPE* _##FACET_NAME;\
	public:\
		typedef COMPONENT_TYPE FACET_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FACET_NAME(call_requirements(!requires(TargetType,is_pointer)))\
		{return (TargetType)(*_##FACET_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FACET_NAME(call_requirements(requires(TargetType,is_pointer)))\
		{return (TargetType)(_##FACET_NAME);}\
		tag_getter(FACET_NAME);\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FACET_NAME(TargetType value,call_requirements(!requires(TargetType,is_pointer)))\
		{(*_##FACET_NAME)=(COMPONENT_TYPE&)value;}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FACET_NAME(TargetType value,call_requirements(requires(TargetType,is_pointer)))\
		{(_##FACET_NAME)=(COMPONENT_TYPE*)value;}\
		tag_setter(FACET_NAME);

///============================================================================
/// member_container – member creator, type-definition and basic accessors
///============================================================================

#define member_container_basic(CONTAINER_TYPE,FACET_NAME)\
	protected:\
		Polaris_Container<CONTAINER_TYPE> _##FACET_NAME;\
	public:\
		typedef Polaris_Container<CONTAINER_TYPE> FACET_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FACET_NAME(call_requirements(!requires(TargetType,is_pointer)))\
		{return (TargetType)(_##FACET_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FACET_NAME(call_requirements(requires(TargetType,is_pointer)))\
		{return (TargetType)(&_##FACET_NAME);}\
		tag_getter(FACET_NAME);\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FACET_NAME(TargetType value,call_requirements(!requires(TargetType,is_pointer)))\
		{_##FACET_NAME=(Polaris_Container<CONTAINER_TYPE>&)value;}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FACET_NAME(TargetType value,call_requirements(requires(TargetType,is_pointer)))\
		{_##FACET_NAME=(Polaris_Container<CONTAINER_TYPE>&)(*value);}\
		tag_setter(FACET_NAME);


///============================================================================
/// local definition macros
///============================================================================

#define define_component_interface(COMPONENT_ALIAS,INTERFACE_TEMPLATE,FACET_LINKED_TO_TYPE,CALLER_TYPE) typedef INTERFACE_TEMPLATE<typename ComponentType::FACET_LINKED_TO_TYPE##_type,CALLER_TYPE> COMPONENT_ALIAS

#define define_container_and_value_interface(INTERFACE_TEMPLATE,FACET_LINKED_TO_TYPE,CONTAINER_ALIAS,VALUE_INTERFACE_TEMPLATE,VALUE_ALIAS,CALLER_TYPE)\
	typedef VALUE_INTERFACE_TEMPLATE<typename ComponentType::FACET_LINKED_TO_TYPE##_type::unqualified_value_type,CALLER_TYPE> VALUE_ALIAS;\
	typedef INTERFACE_TEMPLATE<typename ComponentType::FACET_LINKED_TO_TYPE##_type,CALLER_TYPE,VALUE_ALIAS*> CONTAINER_ALIAS

#define define_container_interface(INTERFACE_TEMPLATE,FACET_LINKED_TO_TYPE,CONTAINER_ALIAS,TARGET_VALUE_INTERFACE,CALLER_TYPE)\
	typedef INTERFACE_TEMPLATE<typename ComponentType::FACET_LINKED_TO_TYPE##_type,CALLER_TYPE,TARGET_VALUE_INTERFACE> CONTAINER_ALIAS