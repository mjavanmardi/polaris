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

#define implementation template<typename MasterType,typename ParentType=NULLTYPE,typename InheritanceList=NULLTYPELIST>

///============================================================================
/// feature - standard declarator for all prototype features
///============================================================================

#define feature_prototype public: template<typename TargetType>
#define feature public: template<typename TargetType>

///============================================================================
/// feature_implementation - standard declarator for all implementation features
///============================================================================

#define feature_implementation public: template<typename ComponentType,typename CallerType,typename TargetType>

///====================================================================================
/// feature_implementation_definition - standard definition for implementation features
///====================================================================================

#define feature_implementation_definition\
	template<typename MasterType,typename ParentType,typename InheritanceList>\
	template<typename ComponentType,typename CallerType,typename TargetType>

///============================================================================
/// declare_feature_event - header for a basic event feature
///============================================================================

struct event_type{};

#define declare_feature_event(FEATURE_NAME)\
	typedef event_type FEATURE_NAME##_event_tag;\
	template<typename TargetType> static void FEATURE_NAME(void* _this)

#define declare_feature_event_implementation(FEATURE_NAME)\
	typedef event_type FEATURE_NAME##_event_tag;\
	template<typename ComponentType,typename CallerType,typename TargetType> static void FEATURE_NAME(void* _this)

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

#define declare_feature_conditional_implementation(FEATURE_NAME)\
	typedef conditional_type FEATURE_NAME##_conditional_tag;\
	template<typename ComponentType,typename CallerType,typename TargetType> static void FEATURE_NAME(void* _this,Conditional_Response& response)

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

#define define_feature_exists_check(FEATURE_NAME, CHECK_ALIAS)\
	public:\
	template<typename T>\
	struct CHECK_ALIAS\
	{\
	template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_feature_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool member_exists=sizeof(has_matching_typename<T>(0))==success;\
		template<class U,bool B> struct p_conditional{typedef false_type type;};\
		template<class U> struct p_conditional<U,true>{typedef typename is_same<typename U::FEATURE_NAME##_feature_tag,true_type>::type type;};\
		static const bool value=(member_exists && p_conditional<T,member_exists>::type::value);\
	};

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

#define define_get_exists_delay_check(FEATURE_NAME, GET_ALIAS)\
	public:\
	template<typename T, typename CALLERTYPE>\
	struct GET_ALIAS\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_getter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};

#define define_set_exists_delay_check(FEATURE_NAME, SET_ALIAS)\
	template<typename T, typename CALLERTYPE>\
	struct SET_ALIAS\
	{\
		template<typename U> static small_type has_matching_typename(typename U::FEATURE_NAME##_setter_tag*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	};

#define define_get_set_exists_check(FEATURE_NAME, GET_ALIAS, SET_ALIAS) define_get_exists_check(FEATURE_NAME, GET_ALIAS) define_set_exists_check(FEATURE_NAME, SET_ALIAS)
#define define_get_set_exists_delay_check(FEATURE_NAME, GET_ALIAS, SET_ALIAS) define_get_exists_delay_check(FEATURE_NAME, GET_ALIAS) define_set_exists_delay_check(FEATURE_NAME, SET_ALIAS)

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




#define tag_feature_as_available(FEATURE_NAME) typedef true_type FEATURE_NAME##_feature_tag;



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

#define tag_getter_setter_as_available(FEATURE_NAME) tag_getter_as_available(FEATURE_NAME);tag_setter_as_available(FEATURE_NAME)

///============================================================================
/// member_data and member_pointer – member creator, type-definition and basic accessors
///============================================================================

#define member_pointer(DATA_TYPE,FEATURE_NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	public:\
	    DATA_TYPE* _##FEATURE_NAME;\
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
	public:\
		DATA_TYPE _##FEATURE_NAME;\
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
	public:\
		COMPONENT_TYPE* _##FEATURE_NAME;\
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

#define member_data_component(COMPONENT_TYPE,FEATURE_NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	public:\
		COMPONENT_TYPE _##FEATURE_NAME;\
		typedef COMPONENT_TYPE FEATURE_NAME##_type;\
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
		{_##FEATURE_NAME=(COMPONENT_TYPE)(value);}\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		void FEATURE_NAME(SetValueType value,requires_setter(check_as_given(SetValueType,is_pointer) && (SETTER_REQUIREMENTS)))\
		{_##FEATURE_NAME=(COMPONENT_TYPE)(*value);}\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		SetValueType FEATURE_NAME(SetValueType value, requires_setter(!(SETTER_REQUIREMENTS)))\
		{static_assert((SETTER_REQUIREMENTS) && True_Concept<SetValueType>::value,"\n\n\n[--------- One or more setter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\
		tag_setter_as_available(FEATURE_NAME);


///============================================================================
/// member_prototype – similar to member_component, but stores prototype
///============================================================================

#define member_prototype(PROTOTYPE,FEATURE_NAME,COMPONENT_TYPE,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		PROTOTYPE<COMPONENT_TYPE,ComponentType>* _##FEATURE_NAME;\
	public:\
		typedef COMPONENT_TYPE FEATURE_NAME##_type;\
		typedef PROTOTYPE<COMPONENT_TYPE,ComponentType> FEATURE_NAME##_interface;\
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
		{_##FEATURE_NAME=(PROTOTYPE<COMPONENT_TYPE,ComponentType>*)(&value);}\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		void FEATURE_NAME(SetValueType value,requires_setter(check_as_given(SetValueType,is_pointer) && (SETTER_REQUIREMENTS)))\
		{_##FEATURE_NAME=(PROTOTYPE<COMPONENT_TYPE,ComponentType>*)(value);}\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		SetValueType FEATURE_NAME(SetValueType value, requires_setter(!(SETTER_REQUIREMENTS)))\
		{static_assert((SETTER_REQUIREMENTS) && True_Concept<SetValueType>::value,"\n\n\n[--------- One or more setter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\
		tag_setter_as_available(FEATURE_NAME);

//#define member_data_prototype(COMPONENT_TYPE,FEATURE_NAME,PROTOTYPE,CALLER_TYPE,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
//	public:\
//		PROTOTYPE<COMPONENT_TYPE,ComponentType>* _##FEATURE_NAME;\
//		typedef COMPONENT_TYPE FEATURE_NAME##_type;\
//		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
//		ReturnValueType FEATURE_NAME(requires_getter(!check_as_given(ReturnValueType,is_pointer) && (GETTER_REQUIREMENTS)))\
//		{return (ReturnValueType)(_##FEATURE_NAME);}\
//		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
//		ReturnValueType FEATURE_NAME(requires_getter(check_as_given(ReturnValueType,is_pointer) && (GETTER_REQUIREMENTS)))\
//		{return (ReturnValueType)(&_##FEATURE_NAME);}\
//		template<typename ComponentType, typename CallerType, typename ReturnValueType>\
//		ReturnValueType FEATURE_NAME(requires_getter(!(GETTER_REQUIREMENTS)))\
//		{static_assert((GETTER_REQUIREMENTS) && True_Concept<ReturnValueType>::value,"\n\n\n[--------- One or more getter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");}\
//		tag_getter_as_available(FEATURE_NAME);\
//		template<typename ComponentType, typename CallerType, typename SetValueType>\
//		void FEATURE_NAME(SetValueType value,requires_setter(!check_as_given(SetValueType,is_pointer) && (SETTER_REQUIREMENTS)))\
//		{_##FEATURE_NAME=(COMPONENT_TYPE)(value);}\
//		template<typename ComponentType, typename CallerType, typename SetValueType>\
//		void FEATURE_NAME(SetValueType value,requires_setter(check_as_given(SetValueType,is_pointer) && (SETTER_REQUIREMENTS)))\
//		{_##FEATURE_NAME=(COMPONENT_TYPE)(*value);}\
//		template<typename ComponentType, typename CallerType, typename SetValueType>\
//		SetValueType FEATURE_NAME(SetValueType value, requires_setter(!(SETTER_REQUIREMENTS)))\
//		{static_assert((SETTER_REQUIREMENTS) && True_Concept<SetValueType>::value,"\n\n\n[--------- One or more setter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\
//		tag_setter_as_available(FEATURE_NAME);


///============================================================================
/// member_container – member creator, type-definition and basic accessors
///============================================================================

#define member_container(CONTAINER_TYPE,FEATURE_NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	public:\
		Polaris_Container<CONTAINER_TYPE> _##FEATURE_NAME;\
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

#define member_associative_container(CONTAINER_TYPE,FEATURE_NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	public:\
		Polaris_Associative_Container<CONTAINER_TYPE> _##FEATURE_NAME;\
		typedef Polaris_Associative_Container<CONTAINER_TYPE> FEATURE_NAME##_type;\
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
		{_##FEATURE_NAME=(Polaris_Associative_Container<CONTAINER_TYPE>&)value;}\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		void FEATURE_NAME(SetValueType value,requires_setter(check_as_given(SetValueType,is_pointer) && SETTER_REQUIREMENTS))\
		{_##FEATURE_NAME=(Polaris_Associative_Container<CONTAINER_TYPE>&)(*value);}\
		template<typename ComponentType, typename CallerType, typename SetValueType>\
		SetValueType FEATURE_NAME(SetValueType value, requires_setter(!(SETTER_REQUIREMENTS)))\
		{static_assert((SETTER_REQUIREMENTS) && True_Concept<SetValueType>::value,"\n\n\n[--------- One or more setter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\
		tag_setter_as_available(FEATURE_NAME);


#define member_component_feature(FEATURE_NAME, MEMBER_COMPONENT_NAME, MEMBER_COMPONENT_FEATURE, MEMBER_COMPONENT_PROTOTYPE)\
	define_get_set_exists_check(MEMBER_COMPONENT_NAME,get_##MEMBER_COMPONENT_NAME, set_##MEMBER_COMPONENT_NAME);\
	template<typename ComponentType, typename CallerType, typename ReturnValueType>\
	ReturnValueType FEATURE_NAME(requires_getter(check(ComponentType,get_##MEMBER_COMPONENT_NAME) /*&& check_2(ComponentType, type_of(MEMBER_COMPONENT_NAME),Is_Same_Entity)*/))\
	{\
		define_component_interface(MEMBER_COMPONENT_NAME##_itf,type_of(typename ComponentType::MEMBER_COMPONENT_NAME),MEMBER_COMPONENT_PROTOTYPE,ComponentType);\
		MEMBER_COMPONENT_NAME##_itf* itf = this->template MEMBER_COMPONENT_NAME<ComponentType,CallerType,MEMBER_COMPONENT_NAME##_itf*>();\
		return itf->template MEMBER_COMPONENT_FEATURE<ReturnValueType>();\
	}\
	template<typename ComponentType, typename CallerType, typename ReturnValueType>\
	ReturnValueType FEATURE_NAME(requires_getter(!check(ComponentType,get_##MEMBER_COMPONENT_NAME) /*|| !check_2(ComponentType, type_of(MEMBER_COMPONENT_NAME),Is_Same_Entity)*/))\
	{\
		assert_check(ComponentType,get_##MEMBER_COMPONENT_NAME,"Getter for \"" #MEMBER_COMPONENT_NAME"\" could not be found.");\
		/*assert_check_2(ComponentType, type_of(MEMBER_COMPONENT_NAME),Is_Same_Entity,"Component does not have permission to access " #MEMBER_COMPONENT_NAME "based on the entity type.");*/\
	}\
	template<typename ComponentType, typename CallerType, typename SetValueType>\
	void FEATURE_NAME(SetValueType value, requires_setter(check(ComponentType,set_##MEMBER_COMPONENT_NAME) /*&& check_2(ComponentType,typename type_of(MEMBER_COMPONENT_NAME),Is_Same_Entity)*/))\
	{\
		define_component_interface(MEMBER_COMPONENT_NAME##_itf,type_of(typename ComponentType::MEMBER_COMPONENT_NAME),MEMBER_COMPONENT_PROTOTYPE,ComponentType);\
		MEMBER_COMPONENT_NAME##_itf* itf = this->template MEMBER_COMPONENT_NAME<ComponentType,CallerType,MEMBER_COMPONENT_NAME##_itf*>();\
		itf->template MEMBER_COMPONENT_FEATURE<SetValueType>(value);\
	}\
	template<typename ComponentType, typename CallerType, typename SetValueType>\
	void FEATURE_NAME(SetValueType value, requires_setter(!check(ComponentType,set_##MEMBER_COMPONENT_NAME) /*|| !check_2(ComponentType,typename type_of(MEMBER_COMPONENT_NAME),Is_Same_Entity)*/))\
	{\
		assert_check(ComponentType,set_##MEMBER_COMPONENT_NAME,"Setter for \"" #MEMBER_COMPONENT_NAME"\" could not be found.");\
		/*assert_check_2(ComponentType, type_of(MEMBER_COMPONENT_NAME),Is_Same_Entity,"Component does not have permission to access " #MEMBER_COMPONENT_NAME" based on the entity type.");*/\
	}\
	tag_getter_as_available(FEATURE_NAME);\
	tag_setter_as_available(FEATURE_NAME);
	
///============================================================================
/// local definition macros
///============================================================================

#define define_component_interface(COMPONENT_INTERFACE_ALIAS, FEATURE_TYPE, PROTOTYPE_TEMPLATE, CALLER_TYPE)\
	typedef PROTOTYPE_TEMPLATE<FEATURE_TYPE,CALLER_TYPE> COMPONENT_INTERFACE_ALIAS

#define define_container_and_value_interface(CONTAINER_ALIAS, VALUE_ALIAS, CONTAINER_TYPE, CONTAINER_PROTOTYPE, VALUE_PROTOTYPE,CALLER_TYPE)\
	typedef VALUE_PROTOTYPE<CONTAINER_TYPE::unqualified_value_type,CALLER_TYPE> VALUE_ALIAS;\
	typedef CONTAINER_PROTOTYPE<CONTAINER_TYPE,CALLER_TYPE,VALUE_ALIAS*> CONTAINER_ALIAS

#define define_container_and_value_interface_unqualified_container(CONTAINER_ALIAS, VALUE_ALIAS, CONTAINER_TYPE, CONTAINER_PROTOTYPE, VALUE_PROTOTYPE,CALLER_TYPE)\
	typedef VALUE_PROTOTYPE<typename CONTAINER_TYPE::unqualified_value_type,CALLER_TYPE> VALUE_ALIAS;\
	typedef CONTAINER_PROTOTYPE<CONTAINER_TYPE,CALLER_TYPE,VALUE_ALIAS*> CONTAINER_ALIAS

#define define_container_interface(CONTAINER_ALIAS, CONTAINER_TYPE, CONTAINER_PROTOTYPE, VALUE_PROTOTYPE,CALLER_TYPE)\
	typedef VALUE_PROTOTYPE<CONTAINER_TYPE::unqualified_value_type,CALLER_TYPE> value_alias;\
	typedef CONTAINER_PROTOTYPE<CONTAINER_TYPE,CALLER_TYPE,value_alias*> CONTAINER_ALIAS

#define define_container_interface_unqualified_container(CONTAINER_ALIAS, CONTAINER_TYPE, CONTAINER_PROTOTYPE, VALUE_PROTOTYPE,CALLER_TYPE)\
	typedef VALUE_PROTOTYPE<typename CONTAINER_TYPE::unqualified_value_type,CALLER_TYPE> value_alias;\
	typedef CONTAINER_PROTOTYPE<CONTAINER_TYPE,CALLER_TYPE,value_alias*> CONTAINER_ALIAS

#define define_simple_container_interface(CONTAINER_ALIAS, CONTAINER_TYPE, CONTAINER_PROTOTYPE, VALUE_TYPE, CALLER_TYPE)\
	typedef CONTAINER_PROTOTYPE<CONTAINER_TYPE,CALLER_TYPE,VALUE_TYPE> CONTAINER_ALIAS

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

///============================================================================
/// Target_Type structure for large function signatures
///============================================================================

template<typename Control_Type=NULLTYPE, typename Return_Type=NULLTYPE, typename Param_Type=NULLTYPE, typename Param2_Type=NULLTYPE, typename Param3_Type=NULLTYPE, typename Param4_Type=NULLTYPE, typename Param5_Type=NULLTYPE, typename Param6_Type=NULLTYPE, typename Param7_Type=NULLTYPE, typename Param8_Type=NULLTYPE>
struct Target_Type
{
	typedef Control_Type ControlType;
	typedef Return_Type ReturnType;
	typedef Param_Type ParamType;
	typedef Param2_Type Param2Type;
	typedef Param3_Type Param3Type;
	typedef Param4_Type Param4Type;
	typedef Param5_Type Param5Type;
	typedef Param6_Type Param6Type;
	typedef Param7_Type Param7Type;
	typedef Param8_Type Param8Type;
};

concept struct Is_Target_Type_Struct
{
	check_typename_defined(check1,ReturnType);
	check_typename_defined(check2,ParamType);
	check_typename_defined(check3,Param2Type);

	define_default_check(check1 && check2 && check3);
};