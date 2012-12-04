#pragma once
#include "Containers.h"

///============================================================================
/// prototype - standard declarator for all prototypes
///============================================================================

#define prototype template<typename ComponentType,typename CallerType=NULLTYPE>
#define forward_declare_prototype template<typename ComponentType,typename CallerType>

#define tag_as_prototype\
	typedef ComponentType Component_Type;\
	typedef CallerType Caller_Type;\
	typedef true_type Is_Prototype;

///============================================================================
/// implementation - standard declarator for all implementations
///============================================================================

#define implementation template<typename MasterType>

///============================================================================
/// feature - standard declarator for all prototype features
///============================================================================

#define feature_prototype public: template<typename TargetType>

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
	typedef void (Type::* type)(NULLTYPE*,Conditional_Response&);
};

///============================================================================
/// declare_feature_communication_handler - header for a basic handler feature
///============================================================================

struct communication_handler_type{};

#define declare_feature_communication_handler(FEATURE_NAME)\
	typedef communication_handler_type FEATURE_NAME##_communication_handler_tag;\
	template<typename TargetType> static void FEATURE_NAME(void* _this,char* message)

template<typename Type>
struct member_function_ptr_types<Type,communication_handler_type>
{
	typedef void (Type::* type)(NULLTYPE*,char*);
};

///============================================================================
/// define_get_set_checks - implements a mini-concept to check for implementation existence
///============================================================================

#define define_get_exists_check(FEATURE_NAME, GET_ALIAS)\
	public:\
	template<typename T>\
	struct GET_ALIAS\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_getter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};

#define define_set_exists_check(FEATURE_NAME, SET_ALIAS)\
	template<typename T>\
	struct SET_ALIAS\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_setter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};

#define define_get_set_exists_check(FEATURE_NAME, GET_ALIAS, SET_ALIAS) define_get_exists_check(FEATURE_NAME, GET_ALIAS) define_set_exists_check(FEATURE_NAME, SET_ALIAS)

///============================================================================
/// feature_accessor - implements the standard get and set dispatch features
///		includes a check on whether the implementation has corresponding dispatch
///============================================================================

//#define feature_accessor_with_interface(FEATURE_NAME, INTERFACE_PROTOTYPE, INTERFACE_ALIAS,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
//	template<typename CallerType>\
//	struct INTERFACE_ALIAS\
//	{\
//		typedef INTERFACE_PROTOTYPE<typename ComponentType::FEATURE_NAME##_type,CallerType> _interface;\
//	};\
//	feature_accessor(FEATURE_NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)
//
//#define interface_of(INTERFACE_ALIAS,CALLER_TYPE) INTERFACE_ALIAS<CALLER_TYPE>::_interface


#define feature_accessor(FEATURE_NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	public:\
	template<typename T>\
	struct FEATURE_NAME##_set_check\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_setter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};\
	template<typename SetValueType>\
	void FEATURE_NAME(SetValueType set_value,requires_setter(check(ComponentType,FEATURE_NAME##_set_check) && (SETTER_REQUIREMENTS)))\
	{\
		this_component()->template FEATURE_NAME<ComponentType,CallerType,SetValueType>(set_value);\
	}\
	template<typename SetValueType>\
	void FEATURE_NAME(SetValueType set_value,requires_setter(!check(ComponentType,FEATURE_NAME##_set_check) || !(SETTER_REQUIREMENTS)))\
	{\
		static_assert(FEATURE_NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #FEATURE_NAME " exists, did you remember to use the macro \"tag_setter_as_available\"? ---------]\n\n");\
		static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
	}\
	template<typename T>\
	struct FEATURE_NAME##_get_check\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_getter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};\
	template<typename ReturnValueType>\
	ReturnValueType FEATURE_NAME(requires_getter(check(ComponentType,FEATURE_NAME##_get_check) && (GETTER_REQUIREMENTS)))\
	{\
		return this_component()->template FEATURE_NAME<ComponentType,CallerType,ReturnValueType>();\
	}\
	template<typename ReturnValueType>\
	ReturnValueType FEATURE_NAME(requires_getter(!check(ComponentType,FEATURE_NAME##_get_check) || !(GETTER_REQUIREMENTS)))\
	{\
		static_assert(FEATURE_NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for " #FEATURE_NAME " exists, did you remember to use the macro \"tag_getter_as_available\"? ---------]\n\n");\
		static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
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

#define tag_getter_as_available(FEATURE_NAME) typedef getter_type FEATURE_NAME##_getter_tag;

///============================================================================
/// feature_setter - catches the standard set dispatches meeting N concepts
///============================================================================

struct setter_type{};

template<typename Type>
struct member_function_ptr_types<Type,setter_type>
{
	typedef void (Type::* type)(NULLTYPE);
};

#define tag_setter_as_available(FEATURE_NAME) typedef setter_type FEATURE_NAME##_setter_tag;

///============================================================================
/// declare_feature_getter_setter - catches the standard get and set dispatches meeting concepts
///============================================================================

#define tag_getter_setter_as_available(FEATURE_NAME) tag_getter(FEATURE_NAME);tag_setter(FEATURE_NAME)

///============================================================================
/// member_data and member_pointer – member creator, type-definition and basic accessors
///============================================================================

#define member_pointer(DATA_TYPE,FEATURE_NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	protected:\
		DATA_TYPE* _##FEATURE_NAME;\
	public:\
		typedef DATA_TYPE FEATURE_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
		ReturnValueType FEATURE_NAME(requires_getter(!check_as_given(ReturnValueType,is_pointer) && (GETTER_REQUIREMENTS)))\
		{return (ReturnValueType)(*_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
		ReturnValueType FEATURE_NAME(requires_getter(check_as_given(ReturnValueType,is_pointer) && (GETTER_REQUIREMENTS)))\
		{return (ReturnValueType)(_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
		ReturnValueType FEATURE_NAME(requires_getter(!(GETTER_REQUIREMENTS)))\
		{static_assert(!(GETTER_REQUIREMENTS) && True_Concept<ReturnValueType>::value,"\n\n\n[--------- One or more getter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");}\
		tag_getter_as_available(FEATURE_NAME);\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		void FEATURE_NAME(SetValueType value,requires_setter(!check_as_given(SetValueType,is_pointer) && (SETTER_REQUIREMENTS)))\
		{_##FEATURE_NAME=(DATA_TYPE*)(&value);}\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		void FEATURE_NAME(SetValueType value,requires_setter(check_as_given(SetValueType,is_pointer) && (SETTER_REQUIREMENTS)))\
		{_##FEATURE_NAME=(DATA_TYPE*)(value);}\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		SetValueType FEATURE_NAME(SetValueType value, requires_setter(!(SETTER_REQUIREMENTS)))\
		{static_assert(!(SETTER_REQUIREMENTS) && True_Concept<SetValueType>::value,"\n\n\n[--------- One or more setter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\
		tag_setter_as_available(FEATURE_NAME);

#define member_data(DATA_TYPE,FEATURE_NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	protected:\
		DATA_TYPE _##FEATURE_NAME;\
	public:\
		typedef DATA_TYPE FEATURE_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
		ReturnValueType FEATURE_NAME(requires_getter(!check_as_given(ReturnValueType,is_pointer) && (GETTER_REQUIREMENTS)))\
		{return (ReturnValueType)(_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
		ReturnValueType FEATURE_NAME(requires_getter(check_as_given(ReturnValueType,is_pointer) && (GETTER_REQUIREMENTS)))\
		{return (ReturnValueType)(&_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
		ReturnValueType FEATURE_NAME(requires_getter(!(GETTER_REQUIREMENTS)))\
		{static_assert((GETTER_REQUIREMENTS) && True_Concept<ReturnValueType>::value,"\n\n\n[--------- One or more getter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");}\
		tag_getter_as_available(FEATURE_NAME);\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		void FEATURE_NAME(SetValueType value,requires_setter(!check_as_given(SetValueType,is_pointer) && (SETTER_REQUIREMENTS)))\
		{_##FEATURE_NAME=((DATA_TYPE)value);}\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		void FEATURE_NAME(SetValueType value,requires_setter(check_as_given(SetValueType,is_pointer) && (SETTER_REQUIREMENTS)))\
		{_##FEATURE_NAME=(DATA_TYPE)(*value);}\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		SetValueType FEATURE_NAME(SetValueType value, requires_setter(!(SETTER_REQUIREMENTS)))\
		{static_assert((SETTER_REQUIREMENTS) && True_Concept<SetValueType>::value,"\n\n\n[--------- One or more setter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\
		tag_setter_as_available(FEATURE_NAME);

///============================================================================
/// member_component – member creator, type-definition and basic accessors
///============================================================================

#define member_component(COMPONENT_TYPE,FEATURE_NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	protected:\
		COMPONENT_TYPE* _##FEATURE_NAME;\
	public:\
		typedef COMPONENT_TYPE FEATURE_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
		ReturnValueType FEATURE_NAME(requires_getter(!check_as_given(ReturnValueType,is_pointer) && (GETTER_REQUIREMENTS)))\
		{return (ReturnValueType)(*_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
		ReturnValueType FEATURE_NAME(requires_getter(check_as_given(ReturnValueType,is_pointer) && (GETTER_REQUIREMENTS)))\
		{return (ReturnValueType)(_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
		ReturnValueType FEATURE_NAME(requires_getter(!(GETTER_REQUIREMENTS)))\
		{static_assert((GETTER_REQUIREMENTS) && True_Concept<ReturnValueType>::value,"\n\n\n[--------- One or more getter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");}\
		tag_getter_as_available(FEATURE_NAME);\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		void FEATURE_NAME(SetValueType value,requires_setter(!check_as_given(SetValueType,is_pointer) && (SETTER_REQUIREMENTS)))\
		{_##FEATURE_NAME=(COMPONENT_TYPE*)(&value);}\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		void FEATURE_NAME(SetValueType value,requires_setter(check_as_given(SetValueType,is_pointer) && (SETTER_REQUIREMENTS)))\
		{_##FEATURE_NAME=(COMPONENT_TYPE*)(value);}\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		SetValueType FEATURE_NAME(SetValueType value, requires_setter(!(SETTER_REQUIREMENTS)))\
		{static_assert((SETTER_REQUIREMENTS) && True_Concept<SetValueType>::value,"\n\n\n[--------- One or more setter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\
		tag_setter_as_available(FEATURE_NAME);

///============================================================================
/// member_container – member creator, type-definition and basic accessors
///============================================================================

#define member_container(CONTAINER_TYPE,FEATURE_NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	protected:\
		Polaris_Container<CONTAINER_TYPE> _##FEATURE_NAME;\
	public:\
		typedef Polaris_Container<CONTAINER_TYPE> FEATURE_NAME##_type;\
		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
		ReturnValueType FEATURE_NAME(requires_getter(!check_as_given(ReturnValueType,is_pointer) && GETTER_REQUIREMENTS))\
		{return (ReturnValueType)(_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
		ReturnValueType FEATURE_NAME(requires_getter(check_as_given(ReturnValueType,is_pointer) && GETTER_REQUIREMENTS))\
		{return (ReturnValueType)(&_##FEATURE_NAME);}\
		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
		ReturnValueType FEATURE_NAME(requires_getter(!(GETTER_REQUIREMENTS)))\
		{static_assert((GETTER_REQUIREMENTS) && True_Concept<ReturnValueType>::value,"\n\n\n[--------- One or more getter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");}\
		tag_getter_as_available(FEATURE_NAME);\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		void FEATURE_NAME(SetValueType value,requires_setter(!check_as_given(SetValueType,is_pointer) && SETTER_REQUIREMENTS))\
		{_##FEATURE_NAME=(Polaris_Container<CONTAINER_TYPE>&)value;}\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		void FEATURE_NAME(SetValueType value,requires_setter(check_as_given(SetValueType,is_pointer) && SETTER_REQUIREMENTS))\
		{_##FEATURE_NAME=(Polaris_Container<CONTAINER_TYPE>&)(*value);}\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		SetValueType FEATURE_NAME(SetValueType value, requires_setter(!(SETTER_REQUIREMENTS)))\
		{static_assert((SETTER_REQUIREMENTS) && True_Concept<SetValueType>::value,"\n\n\n[--------- One or more setter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\
		tag_setter_as_available(FEATURE_NAME);

///============================================================================
/// polaris_variable / member_component_feature – PoDs with type tags
///============================================================================

#define polaris_variable(VARIABLE_NAME, VARIABLE_VALUE_TYPE, TYPE_TRAIT_TAGS,...) \
struct VARIABLE_NAME \
{													\
	typedef true_type TYPE_TRAIT_TAGS, __VA_ARGS__; \
	typedef VARIABLE_VALUE_TYPE ValueType; \
	typedef true_type polaris_variable_type;\
	VARIABLE_NAME(VARIABLE_VALUE_TYPE val): Value(val){} \
	VARIABLE_NAME(): Value(){} \
	VARIABLE_VALUE_TYPE Value; \
	operator VARIABLE_VALUE_TYPE(){return Value;}; \
	VARIABLE_NAME& operator=(VARIABLE_VALUE_TYPE& obj){Value = obj; return *this;}  \
	bool operator==(VARIABLE_VALUE_TYPE& obj){return Value == obj; }  \
	bool operator!=(VARIABLE_VALUE_TYPE& obj){return Value != obj; }  \
	bool operator>(VARIABLE_VALUE_TYPE& obj){return Value > obj; }  \
	bool operator>=(VARIABLE_VALUE_TYPE& obj){return Value >= obj; }  \
	bool operator<(VARIABLE_VALUE_TYPE& obj){return Value < obj; }  \
	bool operator<=(VARIABLE_VALUE_TYPE& obj){return Value <= obj; }  \
	VARIABLE_NAME& operator+(VARIABLE_VALUE_TYPE& obj){Value = Value + obj; return *this;}  \
	VARIABLE_NAME& operator-(VARIABLE_VALUE_TYPE& obj){Value = Value - obj; return *this;}  \
	VARIABLE_NAME& operator*(VARIABLE_VALUE_TYPE& obj){Value = Value * obj; return *this;}  \
	VARIABLE_NAME& operator/(VARIABLE_VALUE_TYPE& obj){Value = Value / obj; return *this;}  \
	VARIABLE_NAME& operator++(){Value = Value + 1; return *this;}  \
	VARIABLE_NAME& operator--(){Value = Value - 1; return *this;}  \
};

#define member_component_feature(FEATURE_NAME, MEMBER_COMPONENT_NAME, MEMBER_COMPONENT_FEATURE, MEMBER_COMPONENT_PROTOTYPE)\
	define_get_set_exists_check(MEMBER_COMPONENT_NAME,get_##MEMBER_COMPONENT_NAME, set_##MEMBER_COMPONENT_NAME);\
	template<typename ComponentType, typename CallerType, typename ReturnValueType>\
	ReturnValueType FEATURE_NAME(requires_getter(check(ComponentType,get_##MEMBER_COMPONENT_NAME) && check_2(ComponentType,type_of_feature(MEMBER_COMPONENT_NAME),Is_Same_Entity)))\
	{\
		define_component_interface(MEMBER_COMPONENT_NAME##_itf,typename ComponentType::MEMBER_COMPONENT_NAME,MEMBER_COMPONENT_PROTOTYPE,ComponentType);\
		MEMBER_COMPONENT_NAME##_itf* itf = this_component()->MEMBER_COMPONENT_NAME<ComponentType,CallerType,MEMBER_COMPONENT_NAME##_itf*>();\
		return itf->MEMBER_COMPONENT_FEATURE<ReturnValueType>();\
	}\
	template<typename ComponentType, typename CallerType, typename SetValueType>\
	void FEATURE_NAME(SetValueType value, requires_setter(check(ComponentType,set_##MEMBER_COMPONENT_NAME)))\
	{\
		define_component_interface(MEMBER_COMPONENT_NAME##_itf,typename ComponentType::MEMBER_COMPONENT_NAME,MEMBER_COMPONENT_PROTOTYPE,ComponentType);\
		MEMBER_COMPONENT_NAME##_itf* itf = this_component()->MEMBER_COMPONENT_NAME<ComponentType,CallerType,MEMBER_COMPONENT_NAME##_itf*>();\
		itf->MEMBER_COMPONENT_FEATURE<SetValueType>(value);\
	}\
	tag_getter_as_available(FEATURE_NAME);\
	tag_setter_as_available(FEATURE_NAME);
	
///============================================================================
/// local definition macros
///============================================================================

#define define_component_interface(COMPONENT_INTERFACE_ALIAS, FEATURE_TYPE, PROTOTYPE_TEMPLATE, CALLER_TYPE)\
	typedef PROTOTYPE_TEMPLATE<typename FEATURE_TYPE,CALLER_TYPE> COMPONENT_INTERFACE_ALIAS

#define define_container_and_value_interface(CONTAINER_ALIAS, VALUE_ALIAS, CONTAINER_TYPE, CONTAINER_PROTOTYPE, VALUE_PROTOTYPE,CALLER_TYPE)\
	typedef VALUE_PROTOTYPE<typename CONTAINER_TYPE::unqualified_value_type,CALLER_TYPE> VALUE_ALIAS;\
	typedef CONTAINER_PROTOTYPE<typename CONTAINER_TYPE,CALLER_TYPE,VALUE_ALIAS*> CONTAINER_ALIAS

#define define_container_interface(CONTAINER_ALIAS, CONTAINER_TYPE, CONTAINER_PROTOTYPE, VALUE_PROTOTYPE,CALLER_TYPE)\
	typedef VALUE_PROTOTYPE<typename CONTAINER_TYPE::unqualified_value_type,CALLER_TYPE> value_alias;\
	typedef CONTAINER_PROTOTYPE<typename CONTAINER_TYPE,CALLER_TYPE,value_alias*> CONTAINER_ALIAS

#define define_simple_container_interface(CONTAINER_ALIAS, CONTAINER_TYPE, CONTAINER_PROTOTYPE, VALUE_TYPE, CALLER_TYPE)\
	typedef CONTAINER_PROTOTYPE<typename CONTAINER_TYPE,CALLER_TYPE,VALUE_TYPE> CONTAINER_ALIAS

#define type_of(FEATURE_NAME) FEATURE_NAME##_type
#define get_type_of(FEATURE_NAME) Component_Type::FEATURE_NAME##_type

///============================================================================
/// component and interface cast macros
///============================================================================

#define this_component() ((ComponentType*)this)
#define target_to_component(TARGET_COMPONENT_TYPE,POINTER_TO_TARGET) ((TARGET_COMPONENT_TYPE&)*POINTER_TO_TARGET)
#define target_to_interface(INTERFACE_ALIAS,POINTER_TO_TARGET) ((INTERFACE_ALIAS&)(*POINTER_TO_TARGET))
#define iterator_to_interface(INTERFACE_ALIAS,ITERATOR_VARIABLE) ( (INTERFACE_ALIAS&) (*(*ITERATOR_VARIABLE)) )
#define access(INTERFACE_ALIAS,FEATURE_FUNCTION) ( (INTERFACE_ALIAS&) FEATURE_FUNCTION<INTERFACE_ALIAS&>() )

