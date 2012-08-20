#pragma once
#include "Basic_Concepts.h"

///============================================================================
/// facet - standard declarator for all standard facets
///============================================================================

#define facet public: template<typename ThisType,typename CallerType,typename TargetType>

///============================================================================
/// facet_local - standard declarator for all local facets
///============================================================================

#define facet_local protected: template<typename ThisType,typename TargetType>

///============================================================================
/// facet_simple - standard declarator for all simple facets
///============================================================================

#define facet_simple public: template<typename ThisType>

///============================================================================
/// facet_event - header for a basic event facet
///============================================================================

#define facet_event(FACET_NAME) template<typename ThisType,typename CallerType,typename TargetType> static void FACET_NAME(ThisType* pthis)

struct type_event{};

template<typename Type>
struct member_function_ptr_types<Type,type_event>
{
	typedef void (Type::* type)(NULLTYPE*);
};

#define tag_event(FACET_NAME) void tag_##FACET_NAME(NULLTYPE*);

///============================================================================
/// facet_conditional - header for a basic condition facet
///============================================================================

#define facet_conditional(FACET_NAME) template<typename ThisType,typename CallerType,typename TargetType> static void FACET_NAME(ThisType* pthis,Conditional_Response& response)

///============================================================================
/// facet_accessor - implements the standard get and set dispatch facets
///		includes a check on whether the base has corresponding dispatch
///============================================================================

#define facet_accessor(FACET_NAME)\
	public:\
	template<typename T>\
	struct FACET_NAME##_get_check\
	{\
		template<typename member_function_ptr_types<typename T::Base_Type,type_getter>::type> struct base_tester{};\
		template<typename member_function_ptr_types<typename T::Interface_Type,type_getter>::type> struct interface_tester{};\
		template<typename U> static small_type has_matching_function_member(base_tester<&U::FACET_NAME##_tag>*);\
		template<typename U> static small_type has_matching_function_member(interface_tester<&U::FACET_NAME##_tag>*);\
		template<typename U> static large_type has_matching_function_member(...);\
		static const bool value=sizeof(has_matching_function_member<T>(0))==success;\
	};\
	template<typename ThisType,typename CallerType,typename TargetType>\
	TargetType FACET_NAME(call_requires(ThisType,Is_Dispatchable))\
	{\
		static_assert(FACET_NAME##_get_check<ThisType>::value,"\n\n\n[--------- No Getter Target to Dispatch To! ---------]\n\n");\
		return PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,CallerType,TargetType>();\
	}\
	template<typename T>\
	struct FACET_NAME##_set_check\
	{\
		template<typename member_function_ptr_types<typename T::Base_Type,type_setter>::type> struct base_tester{};\
		template<typename member_function_ptr_types<typename T::Interface_Type,type_setter>::type> struct interface_tester{};\
		template<typename U> static small_type has_matching_function_member(base_tester<&U::FACET_NAME##_tag>*);\
		template<typename U> static small_type has_matching_function_member(interface_tester<&U::FACET_NAME##_tag>*);\
		template<typename U> static large_type has_matching_function_member(...);\
		static const bool value=sizeof(has_matching_function_member<T>(0))==success;\
	};\
	template<typename ThisType,typename CallerType,typename TargetType>\
	void FACET_NAME(TargetType set_value,call_requires(ThisType,Is_Dispatchable))\
	{\
		static_assert(FACET_NAME##_set_check<ThisType>::value,"\n\n\n[--------- No Setter Target to Dispatch To! ---------]\n\n");\
		PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,CallerType,TargetType>(set_value);\
	}\
	protected:\
	template<typename ThisType,typename TargetType>\
	TargetType FACET_NAME(call_requires(ThisType,Is_Dispatchable))\
	{\
		return PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,ThisType,TargetType>();\
	}\
	template<typename ThisType,typename TargetType>\
	void FACET_NAME(TargetType set_value,call_requires(ThisType,Is_Dispatchable))\
	{\
		PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,ThisType,TargetType>(set_value);\
	}\
	public:

///============================================================================
/// facet_accessor_local - implements the standard get and set dispatch facets
///		local implies that only the protected versions are created
///============================================================================

#define facet_accessor_local(FACET_NAME)\
	protected:\
	template<typename T>\
	struct FACET_NAME##_get_check\
	{\
		template<typename member_function_ptr_types<typename T::Base_Type,type_getter>::type> struct base_tester{};\
		template<typename member_function_ptr_types<typename T::Interface_Type,type_getter>::type> struct interface_tester{};\
		template<typename U> static small_type has_matching_function_member(base_tester<&U::FACET_NAME##_tag>*);\
		template<typename U> static small_type has_matching_function_member(interface_tester<&U::FACET_NAME##_tag>*);\
		template<typename U> static large_type has_matching_function_member(...);\
		static const bool value=sizeof(has_matching_function_member<T>(0))==success;\
	};\
	template<typename ThisType,typename TargetType>\
	typename ThisType::FACET_NAME##_type FACET_NAME(call_requires(ThisType,Is_Dispatchable))\
	{\
		static_assert(FACET_NAME##_get_check<ThisType>::value,"\n\n\n[--------- No Getter Target to Dispatch To! ---------]\n\n");\
		return PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>>>();\
	}\
	template<typename T>\
	struct FACET_NAME##_set_check\
	{\
		template<typename member_function_ptr_types<typename T::Base_Type,type_setter>::type> struct base_tester{};\
		template<typename member_function_ptr_types<typename T::Interface_Type,type_setter>::type> struct interface_tester{};\
		template<typename U> static small_type has_matching_function_member(base_tester<&U::FACET_NAME##_tag>*);\
		template<typename U> static small_type has_matching_function_member(interface_tester<&U::FACET_NAME##_tag>*);\
		template<typename U> static large_type has_matching_function_member(...);\
		static const bool value=sizeof(has_matching_function_member<T>(0))==success;\
	};\
	template<typename ThisType,typename TargetType>\
	void FACET_NAME(typename ThisType::FACET_NAME##_type set_value,call_requires(ThisType,Is_Dispatchable))\
	{\
		static_assert(FACET_NAME##_set_check<ThisType>::value,"\n\n\n[--------- No Setter Target to Dispatch To! ---------]\n\n");\
		PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>>(set_value);\
	}\
	public:

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
			template<typename member_function_ptr_types<typename T::Base_Type,type_getter>::type> struct base_tester{};\
			template<typename member_function_ptr_types<typename T::Interface_Type,type_getter>::type> struct interface_tester{};\
			template<typename U> static small_type has_matching_function_member(base_tester<&U::FACET_NAME##_tag>*);\
			template<typename U> static small_type has_matching_function_member(interface_tester<&U::FACET_NAME##_tag>*);\
			template<typename U> static large_type has_matching_function_member(...);\
			static const bool value=sizeof(has_matching_function_member<T>(0))==success;\
	};\
	template<typename ThisType,typename CallerType,typename TargetType>\
	typename TargetType::Interface_Type* FACET_NAME(call_requires(ThisType,Is_Dispatchable))\
	{\
			static_assert(FACET_NAME##_get_check<ThisType>::value,"\n\n\n[--------- No Getter Target to Dispatch To! ---------]\n\n");\
			return PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,CallerType,TargetType>();\
	}\
	template<typename T>\
	struct FACET_NAME##_set_check\
	{\
			template<typename member_function_ptr_types<typename T::Base_Type,type_setter>::type> struct base_tester{};\
			template<typename member_function_ptr_types<typename T::Interface_Type,type_setter>::type> struct interface_tester{};\
			template<typename U> static small_type has_matching_function_member(base_tester<&U::FACET_NAME##_tag>*);\
			template<typename U> static small_type has_matching_function_member(interface_tester<&U::FACET_NAME##_tag>*);\
			template<typename U> static large_type has_matching_function_member(...);\
			static const bool value=sizeof(has_matching_function_member<T>(0))==success;\
	};\
	template<typename ThisType,typename CallerType,typename TargetType>\
	void FACET_NAME(typename TargetType::Interface_Type* set_value,call_requires(ThisType,Is_Dispatchable))\
	{\
			static_assert(FACET_NAME##_set_check<ThisType>::value,"\n\n\n[--------- No Setter Target to Dispatch To! ---------]\n\n");\
			PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,CallerType,TargetType>(set_value);\
	}\
	protected:\
	template<typename ThisType,typename TargetType>\
	typename TargetType::Interface_Type* FACET_NAME(call_requires(ThisType,Is_Dispatchable))\
	{\
			return PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,ThisType,TargetType>();\
	}\
	template<typename ThisType,typename TargetType>\
	void FACET_NAME(typename TargetType::Interface_Type* set_value,call_requires(ThisType,Is_Dispatchable))\
	{\
			PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,ThisType,TargetType>(set_value);\
	}\
	public:

///============================================================================
/// facet_accessor_ptr - implements the standard get and set dispatch facets
///		takes the TargetType as the component so checks can be performed
///		returns the pointer implied by TargetType
///============================================================================

#define facet_accessor_ptr(FACET_NAME)\
	public:\
	template<typename T>\
	struct FACET_NAME##_get_check\
	{\
		template<typename member_function_ptr_types<typename T::Base_Type,type_getter>::type> struct base_tester{};\
		template<typename member_function_ptr_types<typename T::Interface_Type,type_getter>::type> struct interface_tester{};\
		template<typename U> static small_type has_matching_function_member(base_tester<&U::FACET_NAME##_tag>*);\
		template<typename U> static small_type has_matching_function_member(interface_tester<&U::FACET_NAME##_tag>*);\
		template<typename U> static large_type has_matching_function_member(...);\
		static const bool value=sizeof(has_matching_function_member<T>(0))==success;\
	};\
	template<typename ThisType,typename CallerType,typename TargetType>\
	TargetType* FACET_NAME(call_requires(ThisType,Is_Dispatchable))\
	{\
		static_assert(FACET_NAME##_get_check<ThisType>::value,"\n\n\n[--------- No Getter Target to Dispatch To! ---------]\n\n");\
		return PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,CallerType,TargetType>();\
	}\
	template<typename T>\
	struct FACET_NAME##_set_check\
	{\
		template<typename member_function_ptr_types<typename T::Base_Type,type_setter>::type> struct base_tester{};\
		template<typename member_function_ptr_types<typename T::Interface_Type,type_setter>::type> struct interface_tester{};\
		template<typename U> static small_type has_matching_function_member(base_tester<&U::FACET_NAME##_tag>*);\
		template<typename U> static small_type has_matching_function_member(interface_tester<&U::FACET_NAME##_tag>*);\
		template<typename U> static large_type has_matching_function_member(...);\
		static const bool value=sizeof(has_matching_function_member<T>(0))==success;\
	};\
	template<typename ThisType,typename CallerType,typename TargetType>\
	void FACET_NAME(TargetType* set_value,call_requires(ThisType,Is_Dispatchable))\
	{\
		static_assert(FACET_NAME##_set_check<ThisType>::value,"\n\n\n[--------- No Setter Target to Dispatch To! ---------]\n\n");\
		PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,CallerType,TargetType>(set_value);\
	}\
	protected:\
	template<typename ThisType,typename TargetType>\
	typename TargetType* FACET_NAME(call_requires(ThisType,Is_Dispatchable))\
	{\
		return PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,ThisType,TargetType>();\
	}\
	template<typename ThisType,typename TargetType>\
	void FACET_NAME(TargetType* set_value,call_requires(ThisType,Is_Dispatchable))\
	{\
		PTHIS(ThisType)->FACET_NAME<Dispatch<ThisType>,ThisType,TargetType>(set_value);\
	}\
	public:

///============================================================================
/// facet_getter - catches the standard get dispatches meeting concepts
///============================================================================

struct type_getter{};

template<typename Type>
struct member_function_ptr_types<Type,type_getter>
{
	typedef NULLTYPE (Type::* type)(void);
};

#define facet_getter_basic(FACET_NAME)\
	tag_getter(FACET_NAME);\
	template<typename ThisType,typename CallerType,typename TargetType> TargetType FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched)))

#define facet_getter(FACET_NAME,...)\
	tag_getter(FACET_NAME);\
	template<typename ThisType,typename CallerType,typename TargetType> TargetType FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched) && (__VA_ARGS__)))

#define tag_getter(FACET_NAME) NULLTYPE tag_##FACET_NAME(void);

///============================================================================
/// facet_setter - catches the standard set dispatches meeting N concepts
///============================================================================

struct type_setter{};

template<typename Type>
struct member_function_ptr_types<Type,type_setter>
{
	typedef void (Type::* type)(NULLTYPE);
};

#define tag_setter(FACET_NAME) void tag_##FACET_NAME(NULLTYPE);

#define facet_setter_basic(FACET_NAME)\
	tag_setter(FACET_NAME);\
	template<typename ThisType,typename CallerType,typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched)))

#define facet_setter(FACET_NAME,...)\
	tag_setter(FACET_NAME);\
	template<typename ThisType,typename CallerType,typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched) && (__VA_ARGS__)))

///============================================================================
/// facet_getter_setter - catches the standard get and set dispatches meeting concepts
///============================================================================

#define tag_getter_setter(FACET_NAME) tag_getter(FACET_NAME);tag_setter(FACET_NAME)

#define facet_getter_setter_basic(FACET_NAME) facet_getter(FACET_NAME);facet_setter(FACET_NAME)
#define facet_getter_setter(FACET_NAME,...) facet_getter(FACET_NAME,__VA_ARGS__);facet_setter(FACET_NAME,__VA_ARGS__)

///============================================================================
/// data_member – data member creator, type-definition and basic accessors
///============================================================================

#define data_member(DATA_TYPE,FACET_NAME)\
	protected:\
	typedef DATA_TYPE type_##FACET_NAME;\
	DATA_TYPE _##FACET_NAME;\
	template<typename ThisType, typename CallerType, typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched))){_##FACET_NAME=(DATA_TYPE)set_value;}\
	tag_setter(FACET_NAME);\
	template<typename ThisType, typename CallerType, typename TargetType> TargetType FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_##FACET_NAME;}\
	tag_getter(FACET_NAME);\
	public:

#define data_member_ext(DATA_TYPE,FACET_NAME,...)\
	protected:\
	typedef DATA_TYPE type_##FACET_NAME;\
	DATA_TYPE _##FACET_NAME;\
	template<typename ThisType, typename CallerType, typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched) && (__VA_ARGS__))){_##FACET_NAME=(DATA_TYPE)set_value;}\
	tag_setter(FACET_NAME);\
	template<typename ThisType, typename CallerType, typename TargetType> TargetType FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched) && (__VA_ARGS__))){return (TargetType)_##FACET_NAME;}\
	tag_getter(FACET_NAME);\
	public:

