#pragma once
#include "Containers.h"

///============================================================================
/// prototype - standard declarator for all prototypes
///============================================================================

#define prototype template<typename ComponentType,typename CallerType>

///============================================================================
/// implementation - standard declarator for all implementations
///============================================================================

#define implementation template<typename MasterType>

///============================================================================
/// feature - standard declarator for all prototype features
///============================================================================

#define feature public: template<typename TargetType>

///============================================================================
/// feature_implementation - standard declarator for all implementation features
///============================================================================

#define feature_implementation public: template<typename ComponentType,typename CallerType,typename TargetType>

///============================================================================
/// declare_feature_event - header for a basic event feature
///============================================================================

struct event_type{};

#define declare_feature_event(FEATURE_NAME)\
	typedef event_type FEATURE_NAME##_event_tag;\
	template<typename TargetType> static void FEATURE_NAME(void* _this)

template<typename Type>
struct member_function_ptr_types<Type,event_type>
{
	typedef void (Type::* type)(NULLTYPE*);
};

///============================================================================
/// declare_feature_conditional - header for a basic condition feature
///============================================================================

struct conditional_type{};

#define declare_feature_conditional(FEATURE_NAME)\
	typedef conditional_type FEATURE_NAME##_conditional_tag;\
	template<typename TargetType> static void FEATURE_NAME(void* _this,Conditional_Response& response)

template<typename Type>
struct member_function_ptr_types<Type,conditional_type>
{
	typedef void (Type::* type)(NULLTYPE*,Conditional_Response& response);
};

///============================================================================
/// define_get_set_checks - implements a mini-concept to check for implementation existence
///============================================================================

#define define_get_exists_check(FEATURE_NAME)\
	public:\
	template<typename T>\
	struct FEATURE_NAME##_get_exists\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_getter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};

#define define_set_exists_check(FEATURE_NAME)\
	template<typename T>\
	struct FEATURE_NAME##_set_exists\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_setter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};

#define define_get_set_exists_check(FEATURE_NAME) define_get_exists_check(FEATURE_NAME) define_set_exists_check(FEATURE_NAME)

///============================================================================
/// feature_accessor - implements the standard get and set dispatch features
///		includes a check on whether the implementation has corresponding dispatch
///============================================================================

#define feature_accessor(FEATURE_NAME)\
	public:\
	template<typename T>\
	struct FEATURE_NAME##_get_check\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_getter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};\
	template<typename TargetType>\
	TargetType FEATURE_NAME(call_requirements((requires(ComponentType,FEATURE_NAME##_get_check))))\
	{\
		return cast_self_to_component().template FEATURE_NAME<ComponentType,CallerType,TargetType>();\
	}\
	template<typename TargetType>\
	TargetType FEATURE_NAME(call_requirements(!requires(ComponentType,FEATURE_NAME##_get_check)))\
	{\
		static_assert(FEATURE_NAME##_get_check<ComponentType>::value,"\n\n\n[--------- No Getter Target for " #FEATURE_NAME " to Dispatch To! ---------]\n\n");\
	}\
	template<typename T>\
	struct FEATURE_NAME##_set_check\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_setter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};\
	template<typename TargetType>\
	void FEATURE_NAME(TargetType set_value,call_requirements((requires(ComponentType,FEATURE_NAME##_set_check))))\
	{\
		cast_self_to_component().template FEATURE_NAME<ComponentType,CallerType,TargetType>(set_value);\
	}\
	template<typename TargetType>\
	void FEATURE_NAME(TargetType set_value,call_requirements(!requires(ComponentType,FEATURE_NAME##_set_check)))\
	{\
		static_assert(FEATURE_NAME##_set_check<ComponentType>::value,"\n\n\n[--------- No Setter Target for " #FEATURE_NAME " to Dispatch To! ---------]\n\n");\
	}\


///============================================================================
/// feature_getter - catches the standard get dispatches meeting concepts
///============================================================================

struct getter_type{};

template<typename Type>
struct member_function_ptr_types<Type,getter_type>
{
	typedef NULLTYPE (Type::* type)(void);
};

#define declare_feature_getter(FEATURE_NAME,ADDITIONAL_REQUIREMENTS)\
	template<typename ComponentType,typename CallerType,typename TargetType> TargetType FEATURE_NAME(call_requirements(requires(ComponentType,Is_Dispatched) && (ADDITIONAL_REQUIREMENTS)))

#define tag_getter(FEATURE_NAME) typedef getter_type FEATURE_NAME##_getter_tag;

///============================================================================
/// feature_setter - catches the standard set dispatches meeting N concepts
///============================================================================

struct setter_type{};

template<typename Type>
struct member_function_ptr_types<Type,setter_type>
{
	typedef void (Type::* type)(NULLTYPE);
};

#define tag_setter(FEATURE_NAME) typedef setter_type FEATURE_NAME##_setter_tag;

#define declare_feature_setter(FEATURE_NAME,ADDITIONAL_REQUIREMENTS)\
	template<typename ComponentType,typename CallerType,typename TargetType> void FEATURE_NAME(TargetType set_value,call_requirements(requires(ComponentType,Is_Dispatched) && (ADDITIONAL_REQUIREMENTS)))

///============================================================================
/// declare_feature_getter_setter - catches the standard get and set dispatches meeting concepts
///============================================================================

#define declare_feature_getter_setter_basic(FEATURE_NAME) declare_feature_getter(FEATURE_NAME);declare_feature_setter(FEATURE_NAME)
#define declare_feature_getter_setter(FEATURE_NAME,ADDITIONAL_REQUIREMENTS_GETTER,ADDITIONAL_REQUIREMENTS_SETTER) declare_feature_getter(FEATURE_NAME,ADDITIONAL_REQUIREMENTS_GETTER);declare_feature_setter(FEATURE_NAME,ADDITIONAL_REQUIREMENTS_SETTER)

#define tag_getter_setter(FEATURE_NAME) tag_getter(FEATURE_NAME);tag_setter(FEATURE_NAME)

///============================================================================
/// member_data and member_pointer – member creator, type-definition and basic accessors
///============================================================================

#define member_pointer_basic(DATA_TYPE,FEATURE_NAME)\
	protected:\
		DATA_TYPE* _##FEATURE_NAME;\
	public:\
		typedef DATA_TYPE FEATURE_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(!requires(TargetType,is_pointer)))\
		{return (TargetType)(*_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(requires(TargetType,is_pointer)))\
		{return (TargetType)(_##FEATURE_NAME);}\
		tag_getter(FEATURE_NAME);\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(!requires(TargetType,is_pointer)))\
		{(*_##FEATURE_NAME)=((DATA_TYPE)value);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(requires(TargetType,is_pointer)))\
		{_##FEATURE_NAME=(DATA_TYPE*)value;}\
		tag_setter(FEATURE_NAME);

#define member_pointer(DATA_TYPE,FEATURE_NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	protected:\
		DATA_TYPE* _##FEATURE_NAME;\
	public:\
		typedef DATA_TYPE FEATURE_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(!requires(TargetType,is_pointer) && GETTER_REQUIREMENTS))\
		{return (TargetType)(*_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(requires(TargetType,is_pointer) && GETTER_REQUIREMENTS))\
		{return (TargetType)(_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(!(GETTER_REQUIREMENTS)))\
		{static_assert(false,"\n\n\n[--------- Some of " #FEATURE_NAME" getter requirements from {"#GETTER_REQUIREMENTS"} were not matched---------]\n\n");}\
		tag_getter(FEATURE_NAME);\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(!requires(TargetType,is_pointer) && SETTER_REQUIREMENTS))\
		{_##FEATURE_NAME=(DATA_TYPE*)(&value);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(requires(TargetType,is_pointer) && SETTER_REQUIREMENTS))\
		{_##FEATURE_NAME=(DATA_TYPE*)(value);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(TargetType value, call_requirements(!(SETTER_REQUIREMENTS)))\
		{static_assert(false,"\n\n\n[--------- Some of " #FEATURE_NAME" setter requirements from: {"#SETTER_REQUIREMENTS"}, were not matched---------]\n\n");}\
		tag_setter(FEATURE_NAME);


#define member_data_basic(DATA_TYPE,FEATURE_NAME)\
	protected:\
		DATA_TYPE _##FEATURE_NAME;\
	public:\
		typedef DATA_TYPE FEATURE_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(!requires(TargetType,is_pointer)))\
		{return (TargetType)(_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(requires(TargetType,is_pointer)))\
		{return (TargetType)(&_##FEATURE_NAME);}\
		tag_getter(FEATURE_NAME);\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(!requires(TargetType,is_pointer)))\
		{_##FEATURE_NAME=((DATA_TYPE)value);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(requires(TargetType,is_pointer)))\
		{_##FEATURE_NAME=(DATA_TYPE)(*value);}\
		tag_setter(FEATURE_NAME);

#define member_data(DATA_TYPE,FEATURE_NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	protected:\
		DATA_TYPE _##FEATURE_NAME;\
	public:\
		typedef DATA_TYPE FEATURE_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(!requires(TargetType,is_pointer) && GETTER_REQUIREMENTS))\
		{return (TargetType)(_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(requires(TargetType,is_pointer) && GETTER_REQUIREMENTS))\
		{return (TargetType)(&_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(!(GETTER_REQUIREMENTS)))\
		{static_assert(false,"\n\n\n[--------- Some of " #FEATURE_NAME" getter requirements from {"#GETTER_REQUIREMENTS"} were not matched---------]\n\n");}\
		tag_getter(FEATURE_NAME);\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(!requires(TargetType,is_pointer) && SETTER_REQUIREMENTS))\
		{_##FEATURE_NAME=((DATA_TYPE)value);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(requires(TargetType,is_pointer) && SETTER_REQUIREMENTS))\
		{_##FEATURE_NAME=(DATA_TYPE)(*value);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(TargetType value, call_requirements(!(SETTER_REQUIREMENTS)))\
		{static_assert(false,"\n\n\n[--------- Some of " #FEATURE_NAME" setter requirements from: {"#SETTER_REQUIREMENTS"}, were not matched---------]\n\n");}\
		tag_setter(FEATURE_NAME);


///============================================================================
/// member_component – member creator, type-definition and basic accessors
///============================================================================

#define member_component_basic(COMPONENT_TYPE,FEATURE_NAME)\
	protected:\
		COMPONENT_TYPE* _##FEATURE_NAME;\
	public:\
		typedef COMPONENT_TYPE FEATURE_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(!requires(TargetType,is_pointer)))\
		{return (TargetType)(*_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(requires(TargetType,is_pointer)))\
		{return (TargetType)(_##FEATURE_NAME);}\
		tag_getter(FEATURE_NAME);\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(!requires(TargetType,is_pointer)))\
		{(*_##FEATURE_NAME)=(COMPONENT_TYPE&)value;}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(requires(TargetType,is_pointer)))\
		{(_##FEATURE_NAME)=(COMPONENT_TYPE*)value;}\
		tag_setter(FEATURE_NAME);

#define member_component(COMPONENT_TYPE,FEATURE_NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	protected:\
		COMPONENT_TYPE* _##FEATURE_NAME;\
	public:\
		typedef COMPONENT_TYPE FEATURE_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(!requires(TargetType,is_pointer) && GETTER_REQUIREMENTS))\
		{return (TargetType)(*_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(requires(TargetType,is_pointer) && GETTER_REQUIREMENTS))\
		{return (TargetType)(_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(!(GETTER_REQUIREMENTS)))\
		{static_assert(false,"\n\n\n[--------- Some of " #FEATURE_NAME" getter requirements from {"#GETTER_REQUIREMENTS"} were not matched---------]\n\n");}\
		tag_getter(FEATURE_NAME);\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(!requires(TargetType,is_pointer) && SETTER_REQUIREMENTS))\
		{_##FEATURE_NAME=((COMPONENT_TYPE*)(&value);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(requires(TargetType,is_pointer) && SETTER_REQUIREMENTS))\
		{_##FEATURE_NAME=(COMPONENT_TYPE)(*value);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(TargetType value, call_requirements(!(SETTER_REQUIREMENTS)))\
		{static_assert(false,"\n\n\n[--------- Some of " #FEATURE_NAME" setter requirements from: {"#SETTER_REQUIREMENTS"}, were not matched---------]\n\n");}\
		tag_setter(FEATURE_NAME);

///============================================================================
/// member_container – member creator, type-definition and basic accessors
///============================================================================

#define member_container_basic(CONTAINER_TYPE,FEATURE_NAME)\
	protected:\
		Polaris_Container<CONTAINER_TYPE> _##FEATURE_NAME;\
	public:\
		typedef Polaris_Container<CONTAINER_TYPE> FEATURE_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(!requires(TargetType,is_pointer)))\
		{return (TargetType)(_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(requires(TargetType,is_pointer)))\
		{return (TargetType)(&_##FEATURE_NAME);}\
		tag_getter(FEATURE_NAME);\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(!requires(TargetType,is_pointer)))\
		{_##FEATURE_NAME=(Polaris_Container<CONTAINER_TYPE>&)value;}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(requires(TargetType,is_pointer)))\
		{_##FEATURE_NAME=(Polaris_Container<CONTAINER_TYPE>&)(*value);}\
		tag_setter(FEATURE_NAME);


#define member_container(CONTAINER_TYPE,FEATURE_NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	protected:\
		Polaris_Container<CONTAINER_TYPE> _##FEATURE_NAME;\
	public:\
		typedef Polaris_Container<CONTAINER_TYPE> FEATURE_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(!requires(TargetType,is_pointer) && GETTER_REQUIREMENTS))\
		{return (TargetType)(_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(requires(TargetType,is_pointer) && GETTER_REQUIREMENTS))\
		{return (TargetType)(&_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(call_requirements(!(GETTER_REQUIREMENTS)))\
		{static_assert(false,"\n\n\n[--------- Some of " #FEATURE_NAME" getter requirements from {"#GETTER_REQUIREMENTS"} were not matched---------]\n\n");}\
		tag_getter(FEATURE_NAME);\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(!requires(TargetType,is_pointer) && SETTER_REQUIREMENTS))\
		{_##FEATURE_NAME=(Polaris_Container<CONTAINER_TYPE>&)value;}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		void FEATURE_NAME(TargetType value,call_requirements(requires(TargetType,is_pointer) && SETTER_REQUIREMENTS))\
		{_##FEATURE_NAME=(Polaris_Container<CONTAINER_TYPE>&)(*value);}\
		template<typename ComponentType, typename CallerType, typename TargetType>\
		TargetType FEATURE_NAME(TargetType value, call_requirements(!(SETTER_REQUIREMENTS)))\
		{static_assert(false,"\n\n\n[--------- Some of " #FEATURE_NAME" setter requirements from: {"#SETTER_REQUIREMENTS"}, were not matched---------]\n\n");}\
		tag_setter(FEATURE_NAME);


///============================================================================
/// local definition macros
///============================================================================

#define define_component_interface(COMPONENT_ALIAS,PROTOTYPE_TEMPLATE,FEATURE_LINKED_TO_TYPE,CALLER_TYPE) typedef PROTOTYPE_TEMPLATE<typename ComponentType::FEATURE_LINKED_TO_TYPE##_type,CALLER_TYPE> COMPONENT_ALIAS

#define define_container_and_value_interface(PROTOTYPE,FEATURE_LINKED_TO_TYPE,CONTAINER_ALIAS,VALUE_PROTOTYPE,VALUE_ALIAS,CALLER_TYPE)\
	typedef VALUE_PROTOTYPE<typename ComponentType::FEATURE_LINKED_TO_TYPE##_type::unqualified_value_type,CALLER_TYPE> VALUE_ALIAS;\
	typedef PROTOTYPE<typename ComponentType::FEATURE_LINKED_TO_TYPE##_type,CALLER_TYPE,VALUE_ALIAS*> CONTAINER_ALIAS

#define define_container_interface(PROTOTYPE,FEATURE_LINKED_TO_TYPE,CONTAINER_ALIAS,TARGET_VALUE_INTERFACE,CALLER_TYPE)\
	typedef PROTOTYPE<typename ComponentType::FEATURE_LINKED_TO_TYPE##_type,CALLER_TYPE,TARGET_VALUE_INTERFACE> CONTAINER_ALIAS