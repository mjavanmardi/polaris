#pragma once
///----------------------------------------------------------------------------------------------------
/// Forms.h - POLARIS Forms Library
///----------------------------------------------------------------------------------------------------

#include "Requirements.h"

namespace polaris
{

	///----------------------------------------------------------------------------------------------------
	/// General Macros
	///----------------------------------------------------------------------------------------------------

	#define this_component() ((ComponentType*)this)

	#define type_of(NAME) NAME##_type
	#define get_type_of(NAME) Component_Type::NAME##_type

	#define NONE true
	#define NA false

	///----------------------------------------------------------------------------------------------------
	/// prototype - standard declarator for all prototypes
	///----------------------------------------------------------------------------------------------------

	//#define ADD_DEBUG_INFO : public ComponentType
	#define ADD_DEBUG_INFO

	#define prototype template<typename ComponentType>

	#define MT MasterType

	#define tag_as_prototype\
		typedef ComponentType Component_Type;\
		typedef typename ComponentType::Master_Type Master_Type;\
		typedef true_type Is_Prototype;\
		const int Identify() const {return this_component()->Identify();}\
		template<typename TargetType> bool Is_Type() const {return this_component()->Identify() == TargetType::component_id;}\
		const int uuid(){return this_component()->uuid();}

	prototype struct Null_Prototype{typedef ComponentType Component_Type;};

	///----------------------------------------------------------------------------------------------------
	/// implementation - standard declarator for all implementations
	///----------------------------------------------------------------------------------------------------

	#define implementation template<typename MasterType,typename InheritanceList = NULLTYPELIST>

	///----------------------------------------------------------------------------------------------------
	/// declare_event - header for a basic event feature
	///----------------------------------------------------------------------------------------------------

	struct event_type{};

	#define declare_event(EVENT_NAME)\
		typedef event_type EVENT_NAME##_event_tag;\
		static void EVENT_NAME(void* _this,Event_Response& resp)

	///----------------------------------------------------------------------------------------------------
	/// declare_feature_communication_handler - header for a basic handler feature
	///----------------------------------------------------------------------------------------------------

	struct communication_handler_type{};

	#define declare_communication_handler(HANDLER_NAME)\
		typedef communication_handler_type HANDLER_NAME##_communication_handler_tag;\
		template<typename TargetType> static void HANDLER_NAME(void* _this,char* message)

	///----------------------------------------------------------------------------------------------------
	/// define_get_set_checks - implements a mini-concept to check for implementation existence
	///----------------------------------------------------------------------------------------------------

	#define define_feature_exists_check(NAME, CHECK_ALIAS)\
		public:\
		template<typename T>\
		struct CHECK_ALIAS\
		{\
		template<typename U> static small_type has_matching_typename(typename U::NAME##_feature_tag*);\
			template<typename U> static large_type has_matching_typename(...);\
			static const bool member_exists=sizeof(has_matching_typename<T>(0))==success;\
			template<class U,bool B> struct p_conditional{typedef false_type type;};\
			template<class U> struct p_conditional<U,true>{typedef typename is_same<typename U::NAME##_feature_tag,true_type>::type type;};\
			static const bool value=(member_exists && p_conditional<T,member_exists>::type::value);\
		};
	
	#define define_get_exists_check(NAME, GET_ALIAS)\
		public:\
		template<typename T>\
		struct GET_ALIAS\
		{\
			template<typename U> static small_type has_matching_typename(typename U::NAME##_getter_tag*);\
			template<typename U> static large_type has_matching_typename(...);\
			static const bool value=sizeof(has_matching_typename<T>(0))==success;\
		};
	
	#define define_set_exists_check(NAME, SET_ALIAS)\
		public:\
		template<typename T>\
		struct SET_ALIAS\
		{\
			template<typename U> static small_type has_matching_typename(typename U::NAME##_setter_tag*);\
			template<typename U> static large_type has_matching_typename(...);\
			static const bool value=sizeof(has_matching_typename<T>(0))==success;\
		};
	
	#define define_get_exists_delay_check(NAME, GET_ALIAS)\
		public:\
		template<typename T>\
		struct GET_ALIAS\
		{\
			template<typename U> static small_type has_matching_typename(typename U::NAME##_getter_tag*);\
			template<typename U> static large_type has_matching_typename(...);\
			static const bool value=sizeof(has_matching_typename<T>(0))==success;\
		};
	
	#define define_set_exists_delay_check(NAME, SET_ALIAS)\
		public:\
		template<typename T>\
		struct SET_ALIAS\
		{\
			template<typename U> static small_type has_matching_typename(typename U::NAME##_setter_tag*);\
			template<typename U> static large_type has_matching_typename(...);\
			static const bool value=sizeof(has_matching_typename<T>(0))==success;\
		};
	
	#define define_get_set_exists_check(NAME, GET_ALIAS, SET_ALIAS) define_get_exists_check(NAME, GET_ALIAS) define_set_exists_check(NAME, SET_ALIAS)
	#define define_get_set_exists_delay_check(NAME, GET_ALIAS, SET_ALIAS) define_get_exists_delay_check(NAME, GET_ALIAS) define_set_exists_delay_check(NAME, SET_ALIAS)

	///----------------------------------------------------------------------------------------------------
	/// tag_feature_as_available - generically tag a feature as available
	///----------------------------------------------------------------------------------------------------

	#define tag_feature_as_available(NAME) typedef true_type NAME##_feature_tag;

	///----------------------------------------------------------------------------------------------------
	/// tag_getter_as_available - tags the getter as available
	///----------------------------------------------------------------------------------------------------

	struct getter_type{};

	#define tag_getter_as_available(NAME) typedef getter_type NAME##_getter_tag;

	///----------------------------------------------------------------------------------------------------
	/// tag_setter_as_available - tags the setter as available
	///----------------------------------------------------------------------------------------------------

	struct setter_type{};

	#define tag_setter_as_available(NAME) typedef setter_type NAME##_setter_tag;

	///----------------------------------------------------------------------------------------------------
	/// tag_getter_setter_as_available - tag getter and setter as available
	///----------------------------------------------------------------------------------------------------

	#define tag_getter_setter_as_available(NAME) tag_getter_as_available(NAME);tag_setter_as_available(NAME)
	
	///----------------------------------------------------------------------------------------------------
	/// tag_feature_signature_as_available - tag signature - DEPRECATED
	///----------------------------------------------------------------------------------------------------

	#define tag_feature_signature_as_available(FEATURE_NAME,NUM_ARGS) typedef true_type FEATURE_NAME##_##NUM_ARGS##args_feature_tag;

	///----------------------------------------------------------------------------------------------------
	/// accessor - implements the standard get / set accessors
	///		includes a tagless check on whether the implementation has corresponding accessors
	///----------------------------------------------------------------------------------------------------

	#define accessor(NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		public:\
			template<typename TypeChecked>\
			struct NAME##_set_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(void (V::* arg)(NT,char(*)[1]) = &V::NAME<NT>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					static const bool value = (sizeof(has_matching_named_member<U>(nullptr))==success);\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename TargetType>\
			void NAME(TargetType set_value,requires(TargetType,check(ComponentType,NAME##_set_check) && (SETTER_REQUIREMENTS)))\
			{\
				this_component()->template NAME<TargetType>(set_value);\
			}\
			template<typename TargetType>\
			void NAME(TargetType set_value,requires(TargetType,!check(ComponentType,NAME##_set_check) || !(SETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #NAME " exists ---------]\n\n");\
				static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
			}\
			\
			template<typename TypeChecked>\
			struct NAME##_get_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(NT (V::* arg)(char(*)[1]) = &V::NAME<NT>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					static const bool value = (sizeof(has_matching_named_member<U>(nullptr))==success);\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,check(ComponentType,NAME##_get_check) && (GETTER_REQUIREMENTS)))\
			{\
				return this_component()->template NAME<TargetType>();\
			}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,!check(ComponentType,NAME##_get_check) || !(GETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for " #NAME " exists ---------]\n\n");\
				static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
			}\

	//#define accessor(NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	//	public:\
	//		template<typename TypeChecked>\
	//		struct NAME##_set_check\
	//		{\
	//			template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
	//			struct function_check{ static const bool value = true; };\
	//			template<typename U>\
	//			struct function_check<U,true>\
	//			{\
	//				template<typename V> static small_type has_matching_named_member(void (V::* arg)(NT,char(*)[1]) = &V::NAME<NT>);\
	//				template<typename V> static large_type has_matching_named_member(...);\
	//				\
	//				template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
	//				struct form_check{ static const bool value = false; };\
	//				\
	//				template<typename V>\
	//				struct form_check<V,true>{ static const int value = true;};\
	//				\
	//				static const bool value = form_check<U>::value;\
	//			};\
	//			\
	//			static const bool value = function_check<TypeChecked>::value;\
	//		};\
	//		template<typename TargetType>\
	//		void NAME(TargetType set_value,requires(TargetType,check(ComponentType,NAME##_set_check) && (SETTER_REQUIREMENTS)))\
	//		{\
	//			this_component()->template NAME<TargetType>(set_value);\
	//		}\
	//		template<typename TargetType>\
	//		void NAME(TargetType set_value,requires(TargetType,!check(ComponentType,NAME##_set_check) || !(SETTER_REQUIREMENTS)))\
	//		{\
	//			static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #NAME " exists ---------]\n\n");\
	//			static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
	//		}\
	//		\
	//		template<typename TypeChecked>\
	//		struct NAME##_get_check\
	//		{\
	//			template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
	//			struct function_check{ static const bool value = true; };\
	//			template<typename U>\
	//			struct function_check<U,true>\
	//			{\
	//				template<typename V> static small_type has_matching_named_member(NT (V::* arg)(char(*)[1]) = &V::NAME<NT>);\
	//				template<typename V> static large_type has_matching_named_member(...);\
	//				\
	//				template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
	//				struct form_check{ static const bool value = false; };\
	//				\
	//				template<typename V>\
	//				struct form_check<V,true>{ static const int value = true;};\
	//				\
	//				static const bool value = form_check<U>::value;\
	//			};\
	//			\
	//			static const bool value = function_check<TypeChecked>::value;\
	//		};\
	//		template<typename TargetType>\
	//		TargetType NAME(requires(TargetType,check(ComponentType,NAME##_get_check) && (GETTER_REQUIREMENTS)))\
	//		{\
	//			return this_component()->template NAME<TargetType>();\
	//		}\
	//		template<typename TargetType>\
	//		TargetType NAME(requires(TargetType,!check(ComponentType,NAME##_get_check) || !(GETTER_REQUIREMENTS)))\
	//		{\
	//			static_assert(NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for " #NAME " exists ---------]\n\n");\
	//			static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
	//		}\

	///----------------------------------------------------------------------------------------------------
	/// typed_accessor - implements get / set accessors which explicitly access stated value
	///		includes a tagless check on whether the implementation has corresponding accessors
	///----------------------------------------------------------------------------------------------------

	#define typed_accessor(ACCESS_TYPE,NAME)\
		public:\
			void NAME(ACCESS_TYPE set_value)\
			{\
				this_component()->template NAME<ACCESS_TYPE>(set_value);\
			}\
			ACCESS_TYPE NAME()\
			{\
				return this_component()->template NAME<ACCESS_TYPE>();\
			}


	///----------------------------------------------------------------------------------------------------
	/// explicit_accessor - implements get / set accessors which explicitly access stated value
	///		includes a tagless check on whether the implementation has corresponding accessors
	///----------------------------------------------------------------------------------------------------

	#define explicit_accessor(ACCESS_TYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		public:\
			template<typename TypeChecked>\
			struct NAME##_set_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(void (V::* arg)(NT,char(*)[1]) = &V::NAME<NT>);\
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
			template<typename TargetType>\
			void NAME(ACCESS_TYPE set_value,requires(TargetType,check(ComponentType,NAME##_set_check) && (SETTER_REQUIREMENTS)))\
			{\
				this_component()->template NAME<ACCESS_TYPE>(set_value);\
			}\
			template<typename TargetType>\
			void NAME(ACCESS_TYPE set_value,requires(TargetType,!check(ComponentType,NAME##_set_check) || !(SETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #NAME " exists ---------]\n\n");\
				static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
			}\
			\
			template<typename TypeChecked>\
			struct NAME##_get_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(NT (V::* arg)(char(*)[1]) = &V::NAME<NT>);\
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
			template<typename TargetType>\
			ACCESS_TYPE NAME(requires(TargetType,check(ComponentType,NAME##_get_check) && (GETTER_REQUIREMENTS)))\
			{\
				return this_component()->template NAME<ACCESS_TYPE>();\
			}\
			template<typename TargetType>\
			ACCESS_TYPE NAME(requires(TargetType,!check(ComponentType,NAME##_get_check) || !(GETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for " #NAME " exists ---------]\n\n");\
				static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
			}\

	///----------------------------------------------------------------------------------------------------
	/// tag_based_accessor - implements the standard get / set accessors
	///		includes a tag-based check on whether the implementation has corresponding accessors
	///----------------------------------------------------------------------------------------------------

	#define tag_based_accessor(NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		public:\
			template<typename T>\
			struct NAME##_set_check\
			{\
				template<typename U> static small_type has_matching_typename(typename U::NAME##_setter_tag*);\
				template<typename U> static large_type has_matching_typename(...);\
				static const bool value=sizeof(has_matching_typename<T>(0))==success;\
			};\
			template<typename TargetType>\
			void NAME(TargetType set_value,requires(TargetType,check(ComponentType,NAME##_set_check) && (SETTER_REQUIREMENTS)))\
			{\
				this_component()->template NAME<TargetType>(set_value);\
			}\
			template<typename TargetType>\
			void NAME(TargetType set_value,requires(TargetType,!check(ComponentType,NAME##_set_check) || !(SETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #NAME " exists, did you remember to use the macro \"tag_setter_as_available\"? ---------]\n\n");\
				static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
			}\
			template<typename T>\
			struct NAME##_get_check\
			{\
				template<typename U> static small_type has_matching_typename(typename U::NAME##_getter_tag*);\
				template<typename U> static large_type has_matching_typename(...);\
				static const bool value=sizeof(has_matching_typename<T>(0))==success;\
			};\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,check(ComponentType,NAME##_get_check) && (GETTER_REQUIREMENTS)))\
			{\
				return this_component()->template NAME<TargetType>();\
			}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,!check(ComponentType,NAME##_get_check) || !(GETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for " #NAME " exists, did you remember to use the macro \"tag_getter_as_available\"? ---------]\n\n");\
				static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
			}\

	///----------------------------------------------------------------------------------------------------
	/// m_data – member creator, type-definition and basic accessors
	///----------------------------------------------------------------------------------------------------

	#define m_data(DATA_TYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
			DATA_TYPE _##NAME;\
		public:\
			typedef typename remove_pointer<DATA_TYPE>::type NAME##_type;\
			typedef DATA_TYPE NAME##_raw_type;\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(&_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(*_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,!(GETTER_REQUIREMENTS)))\
			{static_assert((GETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(DATA_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(DATA_TYPE)(value);}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(DATA_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(DATA_TYPE)(*value);}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(DATA_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=((DATA_TYPE)(&value));}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) && check(concat(DATA_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(DATA_TYPE)(value);}\
			template<typename TargetType>\
			void NAME(TargetType value, requires(TargetType,!(SETTER_REQUIREMENTS)))\
			{static_assert((SETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\

	///----------------------------------------------------------------------------------------------------
	/// t_* - implements get / set and member placement which explicitly access stated value
	///		includes a tagless check on whether the implementation has corresponding accessors
	///----------------------------------------------------------------------------------------------------

	#define t_data(DATA_TYPE,NAME)\
			DATA_TYPE _##NAME;\
			typedef DATA_TYPE NAME##_type;\
		public:\
			void NAME(DATA_TYPE set_value)\
			{\
				_##NAME = set_value;\
			}\
			DATA_TYPE NAME()\
			{\
				return _##NAME;\
			}

	#define t_object(DATA_TYPE,NAME)\
			DATA_TYPE _##NAME;\
			typedef DATA_TYPE NAME##_type;\
		public:\
			void NAME(DATA_TYPE& set_value)\
			{\
				_##NAME = set_value;\
			}\
			DATA_TYPE& NAME()\
			{\
				return _##NAME;\
			}

	#define t_pointer(DATA_TYPE,NAME)\
			DATA_TYPE _##NAME;\
			typedef DATA_TYPE NAME##_type;\
		public:\
			void NAME(DATA_TYPE set_value)\
			{\
				_##NAME = set_value;\
			}\
			DATA_TYPE NAME()\
			{\
				return _##NAME;\
			}

	//#define t_data(DATA_TYPE,NAME)\
	//	DATA_TYPE _##NAME;\
	//	typedef remove_pointer<DATA_TYPE>::type NAME##_type;\
	//	template<bool Condition>\
	//	struct DATA_TYPE##_IF\
	//	{\
	//		typedef DATA_TYPE type;\
	//	};\
	//	template<>\
	//	struct DATA_TYPE##_IF<false>\
	//	{\
	//		typedef DATA_TYPE& type;\
	//	};\
	//	typedef typename DATA_TYPE##_IF<is_pointer<DATA_TYPE>::value>::type NAME##_access_type;\
	//	public:\
	//		void NAME(NAME##_access_type set_value)\
	//		{\
	//			_##NAME = set_value;\
	//		}\
	//		NAME##_access_type NAME()\
	//		{\
	//			return _##NAME;\
	//		}

	///----------------------------------------------------------------------------------------------------
	/// prototype_accessor - implements the standard get / set accessors for a prototype
	///		includes a tagless check on whether the implementation has corresponding accessors
	///----------------------------------------------------------------------------------------------------

	#define prototype_accessor(PROTOTYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		public:\
			template<typename TypeChecked>\
			struct NAME##_set_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(void (V::* arg)(NT,char(*)[1]) = &V::NAME<NT>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
					struct form_check{ static const bool value = false; };\
					\
					template<typename V> static small_type has_matching_formed_member( void (V::* arg)(NT,char(*)[1]) );\
					template<typename V> static large_type has_matching_formed_member( NT (V::* arg)(char(*)[1]) );\
					template<typename V> static large_type has_matching_formed_member(...);\
					\
					template<typename V>\
					struct form_check<V,true>{ static const int value = (sizeof(has_matching_formed_member<V>(&V::NAME<NT>))==success);};\
					\
					static const bool value = form_check<U>::value;\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename TargetType>\
			void NAME(PROTOTYPE<TargetType>* set_value,requires(TargetType,check(ComponentType,NAME##_set_check) && (SETTER_REQUIREMENTS)))\
			{\
				this_component()->template NAME<PROTOTYPE<TargetType>*>(set_value);\
			}\
			template<typename TargetType>\
			void NAME(PROTOTYPE<TargetType>* set_value,requires(TargetType,!check(ComponentType,NAME##_set_check) || !(SETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #NAME " exists ---------]\n\n");\
				static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
			}\
			\
			template<typename TypeChecked>\
			struct NAME##_get_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(NT (V::* arg)(char(*)[1]) = &V::NAME<NT>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
					struct form_check{ static const bool value = false; };\
					\
					template<typename V> static small_type has_matching_formed_member( NT (V::* arg)(char(*)[1]) );\
					template<typename V> static large_type has_matching_formed_member( void (V::* arg)(NT,char(*)[1]) );\
					template<typename V> static large_type has_matching_formed_member(...);\
					\
					template<typename V>\
					struct form_check<V,true>{ static const int value = (sizeof(has_matching_formed_member<V>(&V::NAME<NT>))==success);};\
					\
					static const bool value = form_check<U>::value;\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename TargetType>\
			PROTOTYPE<TargetType>* NAME(requires(TargetType,check(ComponentType,NAME##_get_check) && (GETTER_REQUIREMENTS)))\
			{\
				return this_component()->template NAME<PROTOTYPE<TargetType>*>();\
			}\
			template<typename TargetType>\
			PROTOTYPE<TargetType>* NAME(requires(TargetType,!check(ComponentType,NAME##_get_check) || !(GETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for " #NAME " exists ---------]\n\n");\
				static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
			}\

	///----------------------------------------------------------------------------------------------------
	/// m_prototype – member prototype creator, type-definition and basic accessors
	///----------------------------------------------------------------------------------------------------

	#define m_prototype(DATA_TYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
			DATA_TYPE* _##NAME;\
		public:\
			typedef typename DATA_TYPE::Component_Type NAME##_type;\
			typedef DATA_TYPE NAME##_interface_type;\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(*_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,!(GETTER_REQUIREMENTS)))\
			{static_assert((GETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=((DATA_TYPE)(&value));}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(DATA_TYPE*)(value);}\
			template<typename TargetType>\
			void NAME(TargetType value, requires(TargetType,!(SETTER_REQUIREMENTS)))\
			{static_assert((SETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\


       #define member_component_feature(FEATURE_NAME, MEMBER_COMPONENT_NAME, MEMBER_COMPONENT_FEATURE, MEMBER_COMPONENT_PROTOTYPE)\
	   define_get_set_exists_check(MEMBER_COMPONENT_NAME,get_##MEMBER_COMPONENT_NAME##_##MEMBER_COMPONENT_FEATURE, set_##MEMBER_COMPONENT_NAME##_##MEMBER_COMPONENT_FEATURE);\
       template<typename TargetType>\
       TargetType FEATURE_NAME()\
       {\
              typedef MEMBER_COMPONENT_PROTOTYPE<type_of(typename ComponentType::MEMBER_COMPONENT_NAME)> MEMBER_COMPONENT_NAME##_itf;\
              MEMBER_COMPONENT_NAME##_itf* itf = this->template MEMBER_COMPONENT_NAME<MEMBER_COMPONENT_NAME##_itf*>();\
              return itf->template MEMBER_COMPONENT_FEATURE<TargetType>();\
       }\
       template<typename TargetType>\
       void FEATURE_NAME(TargetType value)\
       {\
              typedef MEMBER_COMPONENT_PROTOTYPE<type_of(typename ComponentType::MEMBER_COMPONENT_NAME)> MEMBER_COMPONENT_NAME##_itf;\
              MEMBER_COMPONENT_NAME##_itf* itf = this->template MEMBER_COMPONENT_NAME<MEMBER_COMPONENT_NAME##_itf*>();\
              itf->template MEMBER_COMPONENT_FEATURE<TargetType>(value);\
       }
       tag_getter_as_available(FEATURE_NAME);\
       tag_setter_as_available(FEATURE_NAME);

       #define member_component_and_feature_accessor(FEATURE_NAME, MEMBER_COMPONENT_FEATURE_TO_ACCESS, MEMBER_COMPONENT_PROTOTYPE, MEMBER_COMPONENT_TYPE)\
       m_data(MEMBER_COMPONENT_TYPE,_##FEATURE_NAME,NONE,NONE);\
       member_component_feature(FEATURE_NAME,_##FEATURE_NAME,MEMBER_COMPONENT_FEATURE_TO_ACCESS,MEMBER_COMPONENT_PROTOTYPE);

}