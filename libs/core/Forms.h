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

	//#define this_component() ((ComponentType*)this)
	#define this_component() (static_cast<ComponentType*>(this))

	#define type_of(NAME) NAME##_accessible_type
	#define get_type_of(NAME) Component_Type::NAME##_accessible_type
	
	#define NONE true
	#define NA false

	///----------------------------------------------------------------------------------------------------
	/// prototype - standard declarator for all prototypes
	///----------------------------------------------------------------------------------------------------

//#ifdef _DEBUG
	#define ADD_DEBUG_INFO : public ComponentType
//#else
//	#define ADD_DEBUG_INFO
//#endif

	#define prototype template<typename ComponentType>

	#define MT MasterType
	
	#define tag_as_prototype\
		typedef ComponentType Component_Type;\
		typedef true_type Is_Prototype;\
		const int Identify() const {return this_component()->Identify();}\
		template<typename TargetType> bool Is_Type() const {return this_component()->Identify() == TargetType::component_id;}\
		const int pid(){return this_component()->pid();}

	//#define tag_as_prototype\
	//	typedef ComponentType Component_Type;\
	//	typedef typename ComponentType::Master_Type Master_Type;\
	//	typedef true_type Is_Prototype;\
	//	const int Identify() const {return this_component()->Identify();}\
	//	template<typename TargetType> bool Is_Type() const {return this_component()->Identify() == TargetType::component_id;}\
	//	const int uuid(){return this_component()->pid();}

	prototype struct Null_Prototype{tag_as_prototype;};

	///----------------------------------------------------------------------------------------------------
	/// implementation - standard declarator for all implementations
	///----------------------------------------------------------------------------------------------------

	#define declare_implementation template<typename MasterType,typename InheritanceList = NULLTYPELIST>
	#define define_implementation template<typename MasterType,typename InheritanceList>
	#define implementation template<typename MasterType,typename InheritanceList = NULLTYPELIST>

	#define define_static_member_variable(IMPL_NAME, VARIABLE_NAME) template<typename MasterType, typename InheritanceList> typename IMPL_NAME<MasterType, InheritanceList>::type_of(VARIABLE_NAME) IMPL_NAME<MasterType, InheritanceList>::_##VARIABLE_NAME
	
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

	// THIS ONE DOESNT CHECK IF THE M_ EXISTS IN THE IMPLEMENTATION
	#define basic_accessor(NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		public:\
			template<typename TargetType>\
			void NAME(TargetType set_value)\
			{\
				static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
				this_component()->template NAME<TargetType>(set_value);\
			}\
			template<typename TargetType>\
			TargetType NAME()\
			{\
				static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #NAME "\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
				return this_component()->template NAME<TargetType>();\
			}\


	// THIS IS JOSH's TESTING ACCESSOR CODE
	#define accessor(NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		public:\
			template<typename TypeChecked>\
			struct NAME##_data_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename _U>\
				struct function_check<_U,true>\
				{\
					template<typename _V> static small_type has_matching_named_member(typename is_member_object_pointer<decltype(&_V::_##NAME)>::type);\
					template<typename _V> static large_type has_matching_named_member(...);\
					\
					template<typename _V,bool _P>\
					struct form_check{ static const bool value = false; };\
					\
					template<typename _V>\
					struct form_check<_V,true>{ static const bool value = true; };\
					\
					static const bool performcheck = (sizeof(has_matching_named_member<_U>(true_val))==success);\
					static const bool value = form_check<_U,performcheck>::value;\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename TypeChecked>\
			struct NAME##_set_func_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename _U>\
				struct function_check<_U,true>\
				{\
					template<typename _V> static small_type has_matching_named_member(typename is_same<decltype(declval<_U>().template NAME<int>(0)), void>::type);\
					template<typename _V> static large_type has_matching_named_member(...);\
					\
					template<typename _V,bool _P>\
					struct form_check{ static const bool value = false; };\
					\
					template<typename _V>\
					struct form_check<_V,true>{ static const bool value = true; };\
					\
					static const bool performcheck = (sizeof(has_matching_named_member<_U>(true_val))==success);\
					static const bool value = form_check<_U,performcheck>::value;\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename T>\
			struct NAME##_set_tag_check\
			{\
				template<typename U> static small_type has_matching_typename(typename U::NAME##_setter_tag*);\
				template<typename U> static large_type has_matching_typename(...);\
				static const bool value=sizeof(has_matching_typename<T>(0))==success;\
			};\
			template<typename TargetType>\
			void NAME(TargetType set_value,requires(TargetType,(check(ComponentType,NAME##_data_check) || check(ComponentType,NAME##_set_tag_check) || check(ComponentType,NAME##_set_func_check)) && (SETTER_REQUIREMENTS)))\
			{\
				this_component()->template NAME<TargetType>(set_value);\
			}\
			template<typename TargetType>\
			void NAME(TargetType set_value,requires(TargetType,(!check(ComponentType,NAME##_data_check) && !check(ComponentType,NAME##_set_tag_check) && !check(ComponentType,NAME##_set_func_check)) || !(SETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_data_check<ComponentType>::value || NAME##_set_func_check<ComponentType>::value || NAME##_set_tag_check<ComponentType>::value,"\n\n\n[--------- Can't find set accessor, data member, or tag for '" #NAME "' in implementation ---------]\n\n");\
				static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
			}\
			\
			template<typename T>\
			struct NAME##_get_tag_check\
			{\
				template<typename U> static small_type has_matching_typename(typename U::NAME##_getter_tag*);\
				template<typename U> static large_type has_matching_typename(...);\
				static const bool value=sizeof(has_matching_typename<T>(0))==success;\
			};\
			template<typename TypeChecked>\
			struct NAME##_get_func_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename _U>\
				struct function_check<_U,true>\
				{\
					template<typename _V> static small_type has_matching_named_member(typename is_same<decltype(declval<_U>().template NAME<int>()), int>::type);\
					template<typename _V> static large_type has_matching_named_member(...);\
					\
					template<typename _V,bool _P>\
					struct form_check{ static const bool value = false; };\
					\
					template<typename _V>\
					struct form_check<_V,true>{ static const bool value = true; };\
					\
					static const bool performcheck = (sizeof(has_matching_named_member<_U>(true_val))==success);\
					static const bool value = form_check<_U,performcheck>::value;\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,(check(ComponentType,NAME##_data_check) || check(ComponentType,NAME##_get_func_check) || check(ComponentType,NAME##_get_tag_check)) && (GETTER_REQUIREMENTS)))\
			{\
				return this_component()->template NAME<TargetType>();\
			}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,(!check(ComponentType,NAME##_data_check) && !check(ComponentType,NAME##_get_func_check) && !check(ComponentType,NAME##_get_tag_check)) || !(GETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_data_check<ComponentType>::value || NAME##_get_func_check<ComponentType>::value || NAME##_get_tag_check<ComponentType>::value,"\n\n\n[--------- Can't find get accesor, data member, or tag for '" #NAME "' in implementation ---------]\n\n");\
				static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
			}\


	// THIS CHECK WAS MICHAEL'S NEW IMPLEMENTATION - THE NAMED MEMBER sTUFF DOESN"T WORK
	#define tagless_accessor(NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		public:\
			template<typename TypeChecked>\
			struct NAME##_set_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(void (V::* arg)() = &V::NAME<NT>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
					struct form_check{ static const bool value = false; };\
					template<typename V>\
					struct form_check<V,true>\
					{\
						template<typename Z> static small_type has_matching_formed_member( decltype( ((Z*)judge)->state<NT>(NT()) ) value = nullptr );\
						template<typename Z> static large_type has_matching_formed_member(...);\
						static const int value = (sizeof(has_matching_formed_member<V>())==success);\
					};\
					\
					static const bool value = form_check<U>::value;\
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
			static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #NAME " exists ---------]\n\n");\
			\
			template<typename TypeChecked>\
			struct NAME##_get_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(void (V::* arg)() = &V::NAME<NT>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
					struct form_check{ static const bool value = false; };\
					template<typename V>\
					struct form_check<V,true>\
					{\
						template<typename Z> static small_type has_matching_formed_member( decltype( ((Z*)judge)->state<NT>() ) value );\
						template<typename Z> static large_type has_matching_formed_member(...);\
						static const int value = (sizeof(has_matching_formed_member<V>(NT()))==success);\
					};\
					\
					static const bool value = form_check<U>::value;\
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
			static_assert(NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for " #NAME " exists ---------]\n\n");\

	//#define required_accessor2(NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	//	public:\
	//		template<typename TypeChecked>\
	//		struct NAME##_set_check\
	//		{\
	//			template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
	//			struct function_check{ static const bool value = true; };\
	//			template<typename U>\
	//			struct function_check<U,true>\
	//			{\
	//				template< typename C, typename Ev, typename = void >					\
	//				struct has_receive														\
	//					: std::false_type													\
	//				{};																		\
	//																						\
	//				template< typename C, typename Ev >										\
	//				struct has_receive< C, Ev, typename std::enable_if<						\
	//					std::is_same<														\
	//					decltype(std::declval<C>().NAME##(std::declval<TypeCheched>(TypeChecked))),		\
	//					void																\
	//					>::value															\
	//				>::type >																\
	//					: std::true_type													\
	//				{};																		\
	//			};																			\	
	//			\
	//			/*static const bool value = function_check<TypeChecked>::value;*/\
	//		};\
	//		template<typename TargetType>\
	//				void NAME(TargetType set_value,requires(TargetType,check(ComponentType,NAME##_set_check) && (SETTER_REQUIREMENTS)))\
	//		{\
	//			this_component()->template NAME<TargetType>(set_value);\
	//		}\
	//		template<typename TargetType>\
	//		void NAME(TargetType set_value,requires(TargetType,!check(ComponentType,NAME##_set_check) || !(SETTER_REQUIREMENTS)))\
	//		{\
	//			static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #NAME " exists ---------]\n\n");\
	//			static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
	//		}\
	//		static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #NAME " exists ---------]\n\n");\
	//		\
	//		template<typename TypeChecked>\
	//		struct NAME##_get_check\
	//		{\
	//			template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
	//			struct function_check{ static const bool value = true; };\
	//			template<typename U>\
	//			struct function_check<U,true>\
	//			{\
	//				template<typename V> static small_type has_matching_named_member(void (V::* arg)() = &V::NAME<NT>);\
	//				template<typename V> static large_type has_matching_named_member(...);\
	//				\
	//				template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
	//				struct form_check{ static const bool value = false; };\
	//				template<typename V>\
	//				struct form_check<V,true>\
	//				{\
	//					template<typename Z> static small_type has_matching_formed_member( decltype( ((Z*)judge)->state<NT>() ) value );\
	//					template<typename Z> static large_type has_matching_formed_member(...);\
	//					static const int value = (sizeof(has_matching_formed_member<V>(NT()))==success);\
	//				};\
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
	//		static_assert(NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for " #NAME " exists ---------]\n\n");\

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

#define simple_typed_accessor(ACCESS_TYPE,NAME)\
		public:\
			void NAME(ACCESS_TYPE set_value)\
			{\
				this->template NAME<ACCESS_TYPE>(set_value);\
			}\
			ACCESS_TYPE NAME()\
			{\
				return this->template NAME<ACCESS_TYPE>();\
			}

#define cast_typed_accessor(ACCESS_TYPE,NAME)\
		public:\
			void NAME(ACCESS_TYPE set_value)\
			{\
				this_component_cast()->template NAME<ACCESS_TYPE>(set_value);\
			}\
			ACCESS_TYPE NAME()\
			{\
				return this_component_cast()->template NAME<ACCESS_TYPE>();\
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

			#define new_accessor(NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
              public:\
                     template<typename TypeChecked>\
                     struct NAME##_set_func_check\
                     {\
                           template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
                           struct function_check{ static const bool value = true; };\
                           template<typename _U>\
                           struct function_check<_U,true>\
                           {\
                                  template<typename _V> static small_type has_matching_named_member(typename is_same<decltype(declval<_U>().NAME<int>(0)), void>::type);\
                                  template<typename _V> static large_type has_matching_named_member(...);\
                                  \
                                  template<typename _V,bool _P>\
                                  struct form_check{ static const bool value = false; };\
                                  \
                                  template<typename _V>\
                                  struct form_check<_V,true>{ static const bool value = true; };\
                                  \
                                  static const bool performcheck = (sizeof(has_matching_named_member<_U>(true_val))==success);\
                                  static const bool value = form_check<_U,performcheck>::value;\
                           };\
                           \
                           static const bool value = function_check<TypeChecked>::value;\
                     };\
                     template<typename T>\
                     struct NAME##_set_tag_check\
                     {\
                           template<typename U> static small_type has_matching_typename(typename U::NAME##_setter_tag*);\
                           template<typename U> static large_type has_matching_typename(...);\
                           static const bool value=sizeof(has_matching_typename<T>(0))==success;\
                     };\
                     template<typename TargetType>\
                     void NAME(TargetType set_value,requires(TargetType,(check(ComponentType,NAME##_set_tag_check) || check(ComponentType,NAME##_set_func_check)) && (SETTER_REQUIREMENTS)))\
                     {\
                           this_component()->template NAME<TargetType>(set_value);\
                     }\
                     template<typename TargetType>\
                     void NAME(TargetType set_value,requires(TargetType,(!check(ComponentType,NAME##_set_tag_check) && !check(ComponentType,NAME##_set_func_check)) || !(SETTER_REQUIREMENTS)))\
                     {\
                           static_assert(NAME##_set_func_check<ComponentType>::value || NAME##_set_tag_check<ComponentType>::value,"\n\n\n[--------- Can't find set accessor or tag for '" #NAME "' in implementation ---------]\n\n");\
                           static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
                     }\
                     \
                     template<typename TypeChecked>\
                     struct NAME##_get_func_check\
                     {\
                           template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
                           struct function_check{ static const bool value = true; };\
                           template<typename _U>\
                           struct function_check<_U,true>\
                           {\
                                  template<typename _V> static small_type has_matching_named_member(typename is_same<decltype(declval<_U>().NAME<int>()), int>::type);\
                                  template<typename _V> static large_type has_matching_named_member(...);\
                                  \
                                  template<typename _V,bool _P>\
                                  struct form_check{ static const bool value = false; };\
                                  \
                                  template<typename _V>\
                                  struct form_check<_V,true>{ static const bool value = true; };\
                                  \
                                  static const bool performcheck = (sizeof(has_matching_named_member<_U>(true_val))==success);\
                                  static const bool value = form_check<_U,performcheck>::value;\
                           };\
                           \
                           static const bool value = function_check<TypeChecked>::value;\
                     };\
                     template<typename T>\
                     struct NAME##_get_tag_check\
                     {\
                           template<typename U> static small_type has_matching_typename(typename U::NAME##_getter_tag*);\
                           template<typename U> static large_type has_matching_typename(...);\
                           static const bool value=sizeof(has_matching_typename<T>(0))==success;\
                     };\
					template<typename TargetType>\
					TargetType NAME(requires(TargetType, (check(ComponentType, NAME##_get_func_check) || check(ComponentType, NAME##_get_tag_check)) && (GETTER_REQUIREMENTS)))\
                     {\
                           return this_component()->template NAME<TargetType>();\
                     }\
                     template<typename TargetType>\
                     TargetType NAME(requires(TargetType,(!check(ComponentType,NAME##_get_func_check) && !check(ComponentType,NAME##_get_tag_check)) || !(GETTER_REQUIREMENTS)))\
                     {\
                           static_assert(NAME##_get_func_check<ComponentType>::value || NAME##_get_tag_check<ComponentType>::value,"\n\n\n[--------- Can't find get accessor or tag for '" #NAME "' in implementation ---------]\n\n");\
                           static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
                     }\


#define more_basic_accessor(NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
       public:\
              template<typename TargetType>\
              void NAME(TargetType set_value)\
              {\
                    this_component()->NAME(set_value);\
              }\
              template<typename TargetType>\
              TargetType NAME()\
              {\
                    return this_component()->NAME();\
              }\


/////----------------------------------------------------------------------------------------------------
	///// tag_based_accessor - implements the standard get / set accessors
	/////		includes a tag-based check on whether the implementation has corresponding accessors - This is the only one which works currently
	/////----------------------------------------------------------------------------------------------------

	//#define accessor(NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
	//	public:\
	//		template<typename T>\
	//		struct NAME##_set_check\
	//		{\
	//			template<typename U> static small_type has_matching_typename(typename U::NAME##_setter_tag*);\
	//			template<typename U> static large_type has_matching_typename(...);\
	//			static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	//		};\
	//		template<typename TargetType>\
	//		void NAME(TargetType set_value,requires(TargetType,check(ComponentType,NAME##_set_check) && (SETTER_REQUIREMENTS)))\
	//		{\
	//			this_component()->template NAME<TargetType>(set_value);\
	//		}\
	//		template<typename TargetType>\
	//		void NAME(TargetType set_value,requires(TargetType,!check(ComponentType,NAME##_set_check) || !(SETTER_REQUIREMENTS)))\
	//		{\
	//			static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for \"" #NAME "\" exists in implementation. Check to make sure data member is defined using m_data, m_prototype, m_container, etc. ---------]\n\n");\
	//			static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
	//		}\
	//		template<typename T>\
	//		struct NAME##_get_check\
	//		{\
	//			template<typename U> static small_type has_matching_typename(typename U::NAME##_getter_tag*);\
	//			template<typename U> static large_type has_matching_typename(...);\
	//			static const bool value=sizeof(has_matching_typename<T>(0))==success;\
	//		};\
	//		template<typename TargetType>\
	//		TargetType NAME(requires(TargetType,check(ComponentType,NAME##_get_check) && (GETTER_REQUIREMENTS)))\
	//		{\
	//			return this_component()->template NAME<TargetType>();\
	//		}\
	//		template<typename TargetType>\
	//		TargetType NAME(requires(TargetType,!check(ComponentType,NAME##_get_check) || !(GETTER_REQUIREMENTS)))\
	//		{\
	//			static_assert(NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for \"" #NAME "\" exists in implementation. Check to make sure data member is defined using m_data, m_prototype, m_container, etc. ---------]\n\n");\
	//			static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
	//		}\

	///----------------------------------------------------------------------------------------------------
	/// using_member � convenience macro to assist in inheritance
	///----------------------------------------------------------------------------------------------------
	
	#define using_member(BASE,NAME)\
		using BASE::_##NAME;\
		using typename BASE::NAME##_type;\
		using typename BASE::NAME##_accessible_type;
		
	///----------------------------------------------------------------------------------------------------
	/// m_data � member creator, type-definition and basic accessors
	///----------------------------------------------------------------------------------------------------

	#define m_data(DATA_TYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		protected:\
			DATA_TYPE _##NAME;\
		public:\
			typedef DATA_TYPE NAME##_type;\
			typedef NAME##_type NAME##_accessible_type;\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(&_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(dereference(_##NAME));}\
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
			tag_getter_as_available(NAME);\
			tag_setter_as_available(NAME);

	///----------------------------------------------------------------------------------------------------
	/// m_data � member creator, type-definition and basic accessors
	///----------------------------------------------------------------------------------------------------

	#define m_static_data(DATA_TYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		protected:\
			static DATA_TYPE _##NAME;\
		public:\
			typedef DATA_TYPE NAME##_type;\
			typedef NAME##_type NAME##_accessible_type;\
			template<typename TargetType>\
			static TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(_##NAME);}\
			template<typename TargetType>\
			static TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(&_##NAME);}\
			template<typename TargetType>\
			static TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(dereference(_##NAME));}\
			template<typename TargetType>\
			static TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(_##NAME);}\
			template<typename TargetType>\
			static TargetType NAME(requires(TargetType,!(GETTER_REQUIREMENTS)))\
			{static_assert((GETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");}\
			template<typename TargetType>\
			static void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(DATA_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(DATA_TYPE)(value);}\
			template<typename TargetType>\
			static void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(DATA_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(DATA_TYPE)(*value);}\
			template<typename TargetType>\
			static void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(DATA_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=((DATA_TYPE)(&value));}\
			template<typename TargetType>\
			static void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) && check(concat(DATA_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(DATA_TYPE)(value);}\
			template<typename TargetType>\
			static void NAME(TargetType value, requires(TargetType,!(SETTER_REQUIREMENTS)))\
			{static_assert((SETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\
			tag_getter_as_available(NAME);\
			tag_setter_as_available(NAME);

	///----------------------------------------------------------------------------------------------------
	/// t_* - implements get / set and member placement which explicitly access stated value
	///		includes a tagless check on whether the implementation has corresponding accessors
	///----------------------------------------------------------------------------------------------------

	#define t_data(DATA_TYPE,NAME)\
		public:\
			DATA_TYPE _##NAME;\
		public:\
			typedef DATA_TYPE NAME##_type;\
			void NAME(DATA_TYPE set_value)\
			{\
				_##NAME = set_value;\
			}\
			DATA_TYPE NAME()\
			{\
				return _##NAME;\
			}

	#define t_static_data(DATA_TYPE,NAME)\
		public:\
			static DATA_TYPE _##NAME;\
		public:\
			typedef DATA_TYPE NAME##_type;\
			void NAME(DATA_TYPE set_value)\
			{\
				_##NAME = set_value;\
			}\
			DATA_TYPE NAME()\
			{\
				return _##NAME;\
			}

	#define t_object(DATA_TYPE,NAME)\
		public:\
			DATA_TYPE _##NAME;\
		public:\
			typedef DATA_TYPE NAME##_type;\
			void NAME(DATA_TYPE& set_value)\
			{\
				_##NAME = set_value;\
			}\
			DATA_TYPE& NAME()\
			{\
				return _##NAME;\
			}

	//#define t_pointer(DATA_TYPE,NAME)\
	//		DATA_TYPE* _##NAME;\
	//		typedef DATA_TYPE* NAME##_type;\
	//	public:\
	//		void NAME(DATA_TYPE* set_value)\
	//		{\
	//			_##NAME = set_value;\
	//		}\
	//		DATA_TYPE* NAME()\
	//		{\
	//			return _##NAME;\
	//		}

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
	/// m_prototype � member prototype creator, type-definition and basic accessors
	///----------------------------------------------------------------------------------------------------

	#define m_prototype(PROTOTYPE,COMPONENT_TYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		protected:\
			PROTOTYPE<COMPONENT_TYPE>* _##NAME;\
		public:\
			typedef PROTOTYPE<COMPONENT_TYPE>* NAME##_type;\
			typedef PROTOTYPE<COMPONENT_TYPE> NAME##_interface;\
			typedef COMPONENT_TYPE NAME##_component_type;\
			typedef NAME##_component_type NAME##_accessible_type;\
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
			void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && !check_2(TargetType,nullptr_t,is_same)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=((PROTOTYPE<COMPONENT_TYPE>*)(&value));}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) || check_2(TargetType,nullptr_t,is_same)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(PROTOTYPE<COMPONENT_TYPE>*)(value);}\
			template<typename TargetType>\
			void NAME(TargetType value, requires(TargetType,!(SETTER_REQUIREMENTS)))\
			{static_assert((SETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\
			tag_getter_as_available(NAME);\
			tag_setter_as_available(NAME);

	#define m_static_prototype(PROTOTYPE,COMPONENT_TYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		protected:\
			static PROTOTYPE<COMPONENT_TYPE>* _##NAME;\
		public:\
			typedef PROTOTYPE<COMPONENT_TYPE>* NAME##_type;\
			typedef PROTOTYPE<COMPONENT_TYPE> NAME##_interface;\
			typedef COMPONENT_TYPE NAME##_component_type;\
			typedef NAME##_component_type NAME##_accessible_type;\
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
			void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && !check_2(TargetType,nullptr_t,is_same)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=((PROTOTYPE<COMPONENT_TYPE>*)(&value));}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) || check_2(TargetType,nullptr_t,is_same)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(PROTOTYPE<COMPONENT_TYPE>*)(value);}\
			template<typename TargetType>\
			void NAME(TargetType value, requires(TargetType,!(SETTER_REQUIREMENTS)))\
			{static_assert((SETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\
			tag_getter_as_available(NAME);\
			tag_setter_as_available(NAME);


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
       void FEATURE_NAME(TargetType value,TargetType* = nullptr)\
       {\
              typedef MEMBER_COMPONENT_PROTOTYPE<type_of(typename ComponentType::MEMBER_COMPONENT_NAME)> MEMBER_COMPONENT_NAME##_itf;\
              MEMBER_COMPONENT_NAME##_itf* itf = this->template MEMBER_COMPONENT_NAME<MEMBER_COMPONENT_NAME##_itf*>();\
              itf->template MEMBER_COMPONENT_FEATURE<TargetType>(value);\
       }\
       tag_getter_as_available(FEATURE_NAME);\
       tag_setter_as_available(FEATURE_NAME);

    #define member_component_and_feature_accessor(FEATURE_NAME, MEMBER_COMPONENT_FEATURE_TO_ACCESS, MEMBER_COMPONENT_PROTOTYPE, MEMBER_COMPONENT_TYPE)\
       m_data(MEMBER_COMPONENT_TYPE,_##FEATURE_NAME,NONE,NONE);\
       member_component_feature(FEATURE_NAME,_##FEATURE_NAME,MEMBER_COMPONENT_FEATURE_TO_ACCESS,MEMBER_COMPONENT_PROTOTYPE);

}