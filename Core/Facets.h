#pragma once
#include "Basic_Concepts.h"

///============================================================================
/// facet - standard declarator for all standard facets
///============================================================================

#define facet public: template<typename ThisType,typename CallerType,typename TargetType>

///============================================================================
/// local_facet - standard declarator for all local facets
///============================================================================

#define local_facet protected: template<typename ThisType,typename TargetType>

///============================================================================
/// simple_facet - standard declarator for all simple facets
///============================================================================

#define simple_facet public: template<typename ThisType>




///============================================================================
/// event_facet - header for a basic event facet
///============================================================================

#define event_facet(FACET_NAME) template<typename ThisType,typename CallerType,typename TargetType> static void FACET_NAME(ThisType* pthis)

struct event_type{};

template<typename Type>
struct member_function_ptr_types<Type,event_type>
{
	typedef void (Type::* type)(NULLTYPE*);
};

#define event_tag(FACET_NAME) void FACET_NAME##_tag(NULLTYPE*);

///============================================================================
/// conditional_facet - header for a basic condition facet
///============================================================================

#define conditional_facet(FACET_NAME) template<typename ThisType,typename CallerType,typename TargetType> static void FACET_NAME(ThisType* pthis,Conditional_Response& response)

///============================================================================
/// accessor_facet - implements the standard get and set dispatch facets
///		includes a check on whether the base has corresponding dispatch
///============================================================================

#define accessor_facet(FACET_NAME)\
	public:\
	template<typename T>\
	struct FACET_NAME##_get_check\
	{\
		template<typename member_function_ptr_types<typename T::Base_Type,getter_type>::type> struct base_tester{};\
		template<typename member_function_ptr_types<typename T::Interface_Type,getter_type>::type> struct interface_tester{};\
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
		template<typename member_function_ptr_types<typename T::Base_Type,setter_type>::type> struct base_tester{};\
		template<typename member_function_ptr_types<typename T::Interface_Type,setter_type>::type> struct interface_tester{};\
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
/// local_accessor_facet - implements the standard get and set dispatch facets
///		local implies that only the protected versions are created
///============================================================================

#define local_accessor_facet_test(FACET_NAME)\
	protected:\
	template<typename T>\
	struct FACET_NAME##_get_check\
	{\
		template<typename member_function_ptr_types<typename T::Base_Type,getter_type>::type> struct base_tester{};\
		template<typename member_function_ptr_types<typename T::Interface_Type,getter_type>::type> struct interface_tester{};\
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
		template<typename member_function_ptr_types<typename T::Base_Type,setter_type>::type> struct base_tester{};\
		template<typename member_function_ptr_types<typename T::Interface_Type,setter_type>::type> struct interface_tester{};\
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
/// interface_accessor_facet - implements the standard get and set dispatch facets
///		takes the TargetType as the component so checks can be performed
///		returns the interface implied by TargetType
///============================================================================

#define interface_accessor_facet(FACET_NAME)\
	public:\
	template<typename T>\
	struct FACET_NAME##_get_check\
	{\
			template<typename member_function_ptr_types<typename T::Base_Type,getter_type>::type> struct base_tester{};\
			template<typename member_function_ptr_types<typename T::Interface_Type,getter_type>::type> struct interface_tester{};\
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
			template<typename member_function_ptr_types<typename T::Base_Type,setter_type>::type> struct base_tester{};\
			template<typename member_function_ptr_types<typename T::Interface_Type,setter_type>::type> struct interface_tester{};\
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
/// ptr_accessor_facet - implements the standard get and set dispatch facets
///		takes the TargetType as the component so checks can be performed
///		returns the pointer implied by TargetType
///============================================================================

#define ptr_accessor_facet(FACET_NAME)\
	public:\
	template<typename T>\
	struct FACET_NAME##_get_check\
	{\
			template<typename member_function_ptr_types<typename T::Base_Type,getter_type>::type> struct base_tester{};\
			template<typename member_function_ptr_types<typename T::Interface_Type,getter_type>::type> struct interface_tester{};\
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
			template<typename member_function_ptr_types<typename T::Base_Type,setter_type>::type> struct base_tester{};\
			template<typename member_function_ptr_types<typename T::Interface_Type,setter_type>::type> struct interface_tester{};\
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
/// getter_facet - catches the standard get dispatches meeting concepts
///============================================================================

struct getter_type{};

template<typename Type>
struct member_function_ptr_types<Type,getter_type>
{
	typedef NULLTYPE (Type::* type)(void);
};

#define getter_tag(FACET_NAME) NULLTYPE FACET_NAME##_tag(void);

#define getter_facet(FACET_NAME,DATA_NAME) \
	template<typename ThisType,typename CallerType,typename TargetType> TargetType FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched))){return DATA_NAME;}\
	getter_tag(FACET_NAME);

#define getter_facet_ext(FACET_NAME,DATA_NAME,...) \
	template<typename ThisType,typename CallerType,typename TargetType> TargetType FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched) && ( __VA_ARGS__ ))){return DATA_NAME;}\
	getter_tag(FACET_NAME);

///============================================================================
/// setter_facet - catches the standard set dispatches meeting N concepts
///============================================================================

struct setter_type{};

template<typename Type>
struct member_function_ptr_types<Type,setter_type>
{
	typedef void (Type::* type)(NULLTYPE);
};

#define setter_tag(FACET_NAME) void FACET_NAME##_tag(NULLTYPE);

#define setter_facet(FACET_NAME,DATA_NAME) \
	template<typename ThisType,typename CallerType,typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched))){DATA_NAME=set_value;}\
	setter_tag(FACET_NAME);

#define setter_facet_ext(FACET_NAME,DATA_NAME,...) template<typename ThisType,typename CallerType,typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched) && ( __VA_ARGS__ ))){DATA_NAME=set_value;}\
	setter_tag(FACET_NAME);

///============================================================================
/// getter_setter_facet - catches the standard get and set dispatches meeting concepts
///============================================================================

#define getter_setter_facet(FACET_NAME,DATA_NAME) getter_facet(FACET_NAME,DATA_NAME);setter_facet(FACET_NAME,DATA_NAME)

#define getter_setter_facet_ext(FACET_NAME,DATA_NAME,...) getter_facet(FACET_NAME,DATA_NAME,__VA_ARGS__);setter_facet(FACET_NAME,DATA_NAME,__VA_ARGS__)





///============================================================================
/// data_member – data member creator, type-definition and basic accessors
///============================================================================

#define data_member(DATA_TYPE,FACET_NAME)\
	protected:\
	typedef DATA_TYPE FACET_NAME##_type;\
	DATA_TYPE _##FACET_NAME;\
	template<typename ThisType, typename CallerType, typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched))){_##FACET_NAME=(DATA_TYPE)set_value;}\
	setter_tag(FACET_NAME);\
	template<typename ThisType, typename CallerType, typename TargetType> TargetType FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_##FACET_NAME;}\
	getter_tag(FACET_NAME);\
	public:

#define data_member_ext(DATA_TYPE,FACET_NAME,...)\
	protected:\
	typedef DATA_TYPE FACET_NAME##_type;\
	DATA_TYPE _##FACET_NAME;\
	template<typename ThisType, typename CallerType, typename TargetType> void FACET_NAME(TargetType set_value,call_requirements(requires(ThisType,Is_Dispatched) && (__VA_ARGS__))){_##FACET_NAME=(DATA_TYPE)set_value;}\
	setter_tag(FACET_NAME);\
	template<typename ThisType, typename CallerType, typename TargetType> TargetType FACET_NAME(call_requirements(requires(ThisType,Is_Dispatched) && (__VA_ARGS__))){return (TargetType)_##FACET_NAME;}\
	getter_tag(FACET_NAME);\
	public:

